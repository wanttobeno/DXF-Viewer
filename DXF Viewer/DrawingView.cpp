// DrawingView.cpp : implementation file
//

#include "stdafx.h"
#include "DXF Viewer.h"
#include "DrawingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingView

CDrawingView::CDrawingView()
{
	m_pDrawing = NULL;
	m_Pan = false;
}

CDrawingView::~CDrawingView()
{
}


BEGIN_MESSAGE_MAP(CDrawingView, CStatic)
	//{{AFX_MSG_MAP(CDrawingView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingView message handlers

void CDrawingView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_pDrawing!=NULL)
	{
		if(m_pDrawing->isOpen())
		{
			//m_pDrawing->Paint(dc.m_hDC);
			// Ë«»º³å
			CDC tempdc;
			CBitmap tempbitmap;
			RECT rect;
			GetClientRect(&rect);
			tempdc.CreateCompatibleDC(&dc);
			tempbitmap.CreateCompatibleBitmap(&dc, rect.right-rect.left, rect.bottom-rect.top);
			tempdc.SelectObject(&tempbitmap);
			m_pDrawing->Paint(tempdc.m_hDC);
			dc.BitBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, &tempdc, rect.left, rect.top, SRCCOPY);
			DeleteDC(tempdc);
			DeleteObject(tempbitmap);
		}
		else
		{
			CRect r;
			GetClientRect(r);
			dc.SelectObject(GetStockObject(BLACK_BRUSH));
			dc.Rectangle(r);
		}
	}
}

bool CDrawingView::SetDrawing(CDrawing *pDrawing)
{
	m_pDrawing = pDrawing;
	return true;
}

void CDrawingView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	PrevMousePnt = point;
	m_Pan = true;
	CStatic::OnLButtonDown(nFlags, point);
}

void CDrawingView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_Pan = false;
	CStatic::OnLButtonUp(nFlags, point);
}

void CDrawingView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	PrevMousePnt = point;
	m_Pan = true;
	CStatic::OnMButtonDown(nFlags, point);
}

void CDrawingView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_Pan = false;
	CStatic::OnMButtonUp(nFlags, point);
}

void CDrawingView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_Pan && GetDC()->PtVisible(point))
	{
		if(m_pDrawing->isOpen())
		{
			double OnePixel;
			VIEW drwview;
			m_pDrawing->GetViewProperties(&drwview);
			OnePixel = 1/(drwview.PPU*drwview.ZoomLevel);

			drwview.ViewLeft = drwview.ViewLeft - OnePixel*(point.x-PrevMousePnt.x);
			drwview.ViewBottom = drwview.ViewBottom + OnePixel*(point.y-PrevMousePnt.y);
			m_pDrawing->SetViewProperties(&drwview);
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

			PrevMousePnt = point;
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}

