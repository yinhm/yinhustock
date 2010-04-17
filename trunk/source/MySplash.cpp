// MySplash.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "MySplash.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiTestSplash

CTaiTestSplash::CTaiTestSplash()
{
 
	 hBitmap=NULL;
}

CTaiTestSplash::~CTaiTestSplash()
{
}


BEGIN_MESSAGE_MAP(CTaiTestSplash, CWnd)
	//{{AFX_MSG_MAP(CTaiTestSplash)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTaiTestSplash message handlers
BOOL CTaiTestSplash::Create(CWnd* pParentWnd /*= NULL*/)
//BOOL CTaiTestSplash::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	
	CString strFileName;
    strFileName = "OEMInfo\\startup.bmp";
	hBitmap=(HBITMAP) LoadImage( ::AfxGetApp()->m_hInstance, \
		strFileName,IMAGE_BITMAP, 0,0, LR_DEFAULTSIZE|LR_LOADFROMFILE );
    
	if(hBitmap==NULL)
	   return FALSE;
  
    m_bitmap.Attach(hBitmap);

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_EX_TOPMOST | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);	


	//return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CTaiTestSplash::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	CenterWindow();

	SetTimer(1, 8000, NULL);

	return 0;
}

void CTaiTestSplash::OnPaint() 
{
    CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);
 

	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	
	dcImage.SetTextColor(RGB(0,255,255));
	dcImage.SetTextAlign(TA_CENTER | TA_BOTTOM);
	dcImage.SetBkMode(TRANSPARENT);


	///////////////////////////////////////
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

void CTaiTestSplash::OnTimer(UINT nIDEvent) 
{
	this->HideWindow();

}

void CTaiTestSplash::HideWindow()
{
 	DestroyWindow();
	
}

void CTaiTestSplash::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
	this->ShowWindow(SW_HIDE);

}

void CTaiTestSplash::PostNcDestroy() 
{

    m_bitmap.Detach();
	DeleteObject(this->hBitmap);
	delete this;
	CWnd::PostNcDestroy();
}
