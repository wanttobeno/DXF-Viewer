// DXF ViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DXF Viewer.h"
#include "DXF ViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDXFViewerDlg dialog

CDXFViewerDlg::CDXFViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDXFViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDXFViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_DrawingWindow.m_pDrawing = &drawing;
}

void CDXFViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDXFViewerDlg)
	DDX_Control(pDX, IDC_DRAWINGWINDOW, m_DrawingWindow);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDXFViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CDXFViewerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_VIEW, OnBtnView)
	ON_BN_CLICKED(IDC_BTN_ZOOMIN, OnBtnZoomin)
	ON_BN_CLICKED(IDC_BTN_ZOOMOUT, OnBtnZoomout)
	ON_BN_CLICKED(IDC_BTN_ZOOMEXTENTS, OnBtnZoomextents)
//	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
//	ON_WM_SYSCOMMAND()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
//ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDXFViewerDlg message handlers

BOOL CDXFViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDXFViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CDXFViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


size_t CheckUTF8Tail( const unsigned char * src_str, size_t nsrc_leng)
{
	int count_bytes = 0; 
	unsigned char byte_one = 0, byte_other = 0x3f; // 用于位与运算以提取位值 0x3f-->00111111 	
	size_t	nPos = 0;
	size_t	nCount = nsrc_leng;


	while(nCount)
	{ 
		count_bytes = 1;

		if (src_str[0] <= 0x7f){
			count_bytes = 1; // ASCII字符: 0xxxxxxx( ~ 01111111)
		} 
		if ( (src_str[0] >= 0xc0) && (src_str[0] <= 0xdf) ){ 
			count_bytes = 2; // 110xxxxx(110 00000 ~ 110 111111)
		} 
		if ( (src_str[0] >= 0xe0) && (src_str[0] <= 0xef) ){ 
			count_bytes = 3; // 1110xxxx(1110 0000 ~ 1110 1111)
		} 
		if ( (src_str[0] >= 0xf0) && (src_str[0] <= 0xf7) ){ 
			count_bytes = 4; // 11110xxx(11110 000 ~ 11110 111)
		} 
		if ( (src_str[0] >= 0xf8) && (src_str[0] <= 0xfb) ){ 
			count_bytes = 5; // 111110xx(111110 00 ~ 111110 11)
		} 
		if ( (src_str[0] >= 0xfc) && (src_str[0] <= 0xfd) ){ 
			count_bytes = 6; // 1111110x(1111110 0 ~ 1111110 1)
		} 

		if( (nPos+count_bytes) <= nsrc_leng )
		{
			nCount -= count_bytes;
			nPos += count_bytes;
			src_str += count_bytes;
			continue;
		}

		break;
	}

	return nPos;
}

void CDXFViewerDlg::OnBtnView() 
{
	// TODO: Add your control notification handler code here
	static char szFilter[] = "DXF file format(*.dxf) |*.dxf|All Files (*.*)|*.*||";
	
	CFileDialog OpenDlg( TRUE, NULL, NULL/*LastFilePath*/, OFN_HIDEREADONLY | OFN_EXPLORER, szFilter, NULL );
	OpenDlg.m_ofn.lpstrTitle = "Open DXF File";
	
	if(OpenDlg.DoModal()==IDOK)
	{	// Openning sample dxf file
		CString DxfFileName( OpenDlg.GetPathName() );

#ifdef ZY_UTF8_TO_ANSI
		////////////UTF-8转换为ANSI	
		char szPath[256]={0};
		GetTempPath(256,szPath);
		char szTmpfile[256]={0};
		GetTempFileName(szPath,"NewDxf",0,szTmpfile);
		FILE* pAnsi = fopen(szTmpfile,"wb");
		FILE* pf = fopen(DxfFileName,"rb");

		if (!pAnsi || !pf)
		{
			if (pAnsi)
				fclose(pAnsi);
			if (pf)
				fclose(pf);
			return ;
		}
////////////////////////////////////////////////////
		char	szBuf[4096] = {0};
		wchar_t wBuf[4096] = {0};
		char	szEnd[128] = {0};
		//char*	pszBuf;


		fseek(pf,0,SEEK_END);
		int nSize = ftell(pf);
		fseek(pf,0,SEEK_SET);

		// 读取头文件
		//fread(szBuf, sizeof(char), 3, pf);			
		//if(((BYTE)szBuf[0] != 0xef) || ((BYTE)szBuf[1] != 0xbb) || ((BYTE)szBuf[2] != 0xbf) )
		//{
		//	fclose(pAnsi);
		//	fclose(pf);
		//	DeleteFile(szUtfFile);
		//	DeleteFile(strTempAnsi.c_str());
		//	return /*false*/;
		//}
		nSize -= 3;			

		int nEnd = 0;
		int nMax = 0;

		while(nSize > 0)
		{
			//最大可读量
			nMax = min(nSize,2048-nEnd);
			fread(szBuf+nEnd, sizeof(char), nMax, pf);
			nSize -= nMax;

			//累加上次余留
			nMax += nEnd;
			int nPos = CheckUTF8Tail((unsigned char*)szBuf, nMax);
			nEnd = nMax - nPos;

			_ASSERT(nPos > 0);
			if(nPos <= 0)
			{
				fclose(pAnsi);
				fclose(pf);
				// DeleteFile(szUtfFile);
				// DeleteFile(strTempAnsi.c_str());
				return /*false*/;
			}

			if (nEnd)
				memcpy(szEnd,szBuf+nPos,nEnd);
			// DoTrans
			int nTran = ::MultiByteToWideChar(CP_UTF8,0, szBuf, nPos, wBuf, 4096);
			nTran = ::WideCharToMultiByte(CP_ACP,0,wBuf,nTran,szBuf,4096,0,NULL);
			fwrite(szBuf, sizeof(char), nTran, pAnsi);

			if (nEnd)
				memcpy(szBuf,szEnd,nEnd);

		}		

		::fflush(pAnsi);
		fclose(pAnsi);
		fclose(pf);

#endif // ZY_UTF8_TO_ANSI
////////////////////////////////////////////////////////////////

		if(drawing.isOpen())
			drawing.Destroy();
		
		// Create Drawing
		if(!drawing.Create())
		{
			MessageBox("Fatal error on creating CDrawing!", "Error", MB_ICONERROR | MB_OK);
			return;
		}
		
		// Initialize Progressbar
	//	CProgressDlg ProgDlg;
	//	ProgDlg.Create(this);
	//	ProgDlg.SetRange(0, 100);
		
		// Load data from dxf file
		//	if(!drawing.LoadDXFFile(DxfFileName, ProgDlg.m_hWnd))
#ifdef ZY_UTF-8_ANSI
	if(!drawing.LoadDXFFile(szTmpfile, NULL))
#else
	if(!drawing.LoadDXFFile(DxfFileName, NULL))
#endif // ZY_UTF-8_ANSI
		{
			//ProgDlg.DestroyWindow();
			MessageBox("Error while loading dxf file!", "Error", MB_ICONERROR | MB_OK);
			return;
		}
		
		// Close Progress window
		// ProgDlg.DestroyWindow();
		
		// Initiaze Drawing View Window
		CRect r;
		m_DrawingWindow.GetClientRect(r);
		drawing.InitView(0, 0, r.Width(), r.Height());
		drawing.ZoomExtents();
		m_DrawingWindow.RedrawWindow();
	}
}

void CDXFViewerDlg::OnBtnZoomin() 
{
	// TODO: Add your control notification handler code here
	if(drawing.isOpen())
	{
		drawing.SetZoomLevel( drawing.GetZoomLevel() * 1.5 );
		m_DrawingWindow.RedrawWindow();
	}
}

void CDXFViewerDlg::OnBtnZoomout() 
{
	// TODO: Add your control notification handler code here
	if(drawing.isOpen())
	{
		drawing.SetZoomLevel( drawing.GetZoomLevel() / 1.5 );
		m_DrawingWindow.RedrawWindow();
	}
}

void CDXFViewerDlg::OnBtnZoomextents() 
{
	// TODO: Add your control notification handler code here
	if(drawing.isOpen())
	{
		drawing.ZoomExtents();
		m_DrawingWindow.RedrawWindow();
	}
}

BOOL CDXFViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if(drawing.isOpen())
	{
		if(zDelta>0)
			drawing.SetZoomLevel( drawing.GetZoomLevel() * 1.5 );
		else
			drawing.SetZoomLevel( drawing.GetZoomLevel() / 1.5 );
		m_DrawingWindow.RedrawWindow();
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
