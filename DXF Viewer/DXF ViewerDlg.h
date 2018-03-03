// DXF ViewerDlg.h : header file
//

#if !defined(AFX_DXFVIEWERDLG_H__F79E4E27_B776_4427_9CE5_5F94E6420E21__INCLUDED_)
#define AFX_DXFVIEWERDLG_H__F79E4E27_B776_4427_9CE5_5F94E6420E21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawingView.h"

/////////////////////////////////////////////////////////////////////////////
// CDXFViewerDlg dialog

class CDXFViewerDlg : public CDialog
{
// Construction
public:
	CDXFViewerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDXFViewerDlg)
	enum { IDD = IDD_DXFVIEWER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDXFViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDrawing drawing;
	HICON m_hIcon;
	CDrawingView	m_DrawingWindow;
	
	// Generated message map functions
	//{{AFX_MSG(CDXFViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnView();
	afx_msg void OnBtnZoomin();
	afx_msg void OnBtnZoomout();
	afx_msg void OnBtnZoomextents();
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXFVIEWERDLG_H__F79E4E27_B776_4427_9CE5_5F94E6420E21__INCLUDED_)
