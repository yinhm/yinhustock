// SheetScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "SheetScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSheetScrollBar

CSheetScrollBar::CSheetScrollBar()
{
	m_bRefrash=TRUE;
}

CSheetScrollBar::~CSheetScrollBar()
{
}


BEGIN_MESSAGE_MAP(CSheetScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CSheetScrollBar)
	ON_WM_PAINT()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetScrollBar message handlers

void CSheetScrollBar::OnPaint() 
{
		CPaintDC dc(this); 
		CRect m_Rect;
		GetClientRect(&m_Rect);
		CDC dcMem;
		CBitmap bmp;

		dcMem.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc,m_Rect.Width(),m_Rect.Height()) ;
		CBitmap *pbmOld=dcMem.SelectObject(&bmp);
		Print( &dcMem,PRF_ERASEBKGND|PRF_CLIENT);

		dc.BitBlt(m_Rect.left ,m_Rect.top,m_Rect.right,m_Rect.bottom,&dcMem,0,0,SRCCOPY);
    
		dcMem.SelectObject(pbmOld);
		bmp.DeleteObject();
		dcMem.DeleteDC();
}

void CSheetScrollBar::OnDrawScrollBarTest(CRect * m_Rect)
{
	CPaintDC dc(this); 
	CDC dcMem;
	CBitmap bmp;
	CRect *ClientRect;
	ClientRect=m_Rect;

    dcMem.CreateCompatibleDC(&dc);
    bmp.CreateCompatibleBitmap(&dc,ClientRect->Width(),ClientRect->Height()) ;
    CBitmap *pbmOld=dcMem.SelectObject(&bmp);
    Print( &dcMem,PRF_CLIENT);

	dc.BitBlt(ClientRect->left ,ClientRect->top,ClientRect->right,ClientRect->bottom,&dcMem,0,0,SRCCOPY);
    
	bmp.DeleteObject();
	dcMem.SelectObject(pbmOld);
	dcMem.DeleteDC();
 
}