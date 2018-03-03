// DXF Viewer.h : main header file for the DXF VIEWER application
//

#if !defined(AFX_DXFVIEWER_H__FF064911_4ED1_4656_B44F_543A66FC05BD__INCLUDED_)
#define AFX_DXFVIEWER_H__FF064911_4ED1_4656_B44F_543A66FC05BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDXFViewerApp:
// See DXF Viewer.cpp for the implementation of this class
//

class CDXFViewerApp : public CWinApp
{
public:
	CDXFViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDXFViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDXFViewerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DXFVIEWER_H__FF064911_4ED1_4656_B44F_543A66FC05BD__INCLUDED_)
