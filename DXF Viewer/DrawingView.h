#if !defined(AFX_DRAWINGVIEW_H__C7A18EB6_7630_491F_A48A_3C0137613E1A__INCLUDED_)
#define AFX_DRAWINGVIEW_H__C7A18EB6_7630_491F_A48A_3C0137613E1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingView.h : header file
//

//#include "..\\Interface\\VC\\CadLib.h"
#include "DrawCode/CadLib.h"
#ifdef _DEBUG
	#pragma comment(lib,"DrawCode/Debug/DrawCode.lib")
#else
	#pragma comment(lib,"DrawCode/Release/DrawCode.lib")
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawingView window

// ZY
// 必须开启控件的notiy
class CDrawingView : public CStatic
{
// Construction
public:
	CDrawingView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingView)
	//}}AFX_VIRTUAL

// Implementation
public:
	bool SetDrawing(CDrawing* pDrawing);
	CDrawing* m_pDrawing;
	virtual ~CDrawingView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDrawingView)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CPoint PrevMousePnt;
	bool m_Pan;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGVIEW_H__C7A18EB6_7630_491F_A48A_3C0137613E1A__INCLUDED_)
