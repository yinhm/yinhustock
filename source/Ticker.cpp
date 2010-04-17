/******************************************************************************
Module name: Ticker.cpp
Written by:  CRanjeet::RanjeetChakraborty() Copyright (c) 2000.
			 ranjeetc@hotmail.com	| ranjeet.c@mphasis.com
			 
Purpose:     Provides the implementation file for a scrolling news/stock ticker for your MFC applications.
******************************************************************************/
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// It's free - so you get what you pay for.
//
//Special thanks to Fabian Toader for providing The CJumpyDraw class
//I have borrowed the DrawLinearWash function from his class to provide
//the gradient look.

#include "stdafx.h"
#include "Ticker.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTicker


CTicker::CTicker()
{
	m_TickerTextCharVersion = NULL;
	//Default FontName
	m_TickerFontName = "Comic Sans MS";
	m_TickerFontHeight = 15;

	m_pTickerFont = new CFont;
	m_pTickerFont->CreateFont ( m_TickerFontHeight ,0,0,0,0,FALSE,FALSE,
			0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH, m_TickerFontName);

	m_TickerTextColor = RGB(255,255,255);
	m_TickerRateTextColor = RGB(0,255,0);

	m_EdgeType	 = EDGE_SUNKEN;
	m_BorderType = BF_FLAT;

	m_bIsBkgBmp = FALSE;


	m_Mode = MODE_RATES_TICKER; 

	SetClassLong(m_hWnd,GCL_HCURSOR,(LONG)::LoadCursor(NULL,IDC_CROSS));

}

CTicker::~CTicker()// NOTE: the ClassWizard will add member functions here
{

	if(m_pTickerFont){
		delete m_pTickerFont;
		m_pTickerFont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CTicker, CStatic)
	//{{AFX_MSG_MAP(CTicker)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTicker message handlers

void CTicker::OnTimer(UINT nIDEvent) 
{
	CStatic::OnTimer(nIDEvent);
	if(nIDEvent == ID_TICKER_TIMER){
		i -= m_TickerSpeed ;
		Invalidate ();
	}
}


void CTicker::OnPaint() 
{// NOTE: the ClassWizard will add member functions here

	CPaintDC dc(this); 

	CDC dcMem;
	dcMem.CreateCompatibleDC (&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, m_TickerRect.Width(), m_TickerRect.Height());
	CBitmap* pOldBmp = dcMem.SelectObject(&bmpMem);

	if(m_bIsBkgBmp){
		CBrush hBrush;
		hBrush.CreatePatternBrush (&m_BmpBkg);
		FillRect(dcMem.m_hDC, &m_TickerRect, (HBRUSH)hBrush);  
	}

	dcMem.FillSolidRect (&m_TickerRect ,RGB(192,192,192)); 
	dcMem.SetBkMode(TRANSPARENT);

	CFont* pOldFont;



	if(m_TickerTextCharVersion != NULL && m_Mode == MODE_RATES_TICKER){
	
		if(i <= - dcMem.GetTextExtent (GetTickerText()).cx ){
			i = m_TickerRect.Width ();
		}
		CString pr = GetTickerText ();
		int from = 0,to = 0,origin = i, yCoordFlag = 0;
			while (pr != ""){
						
				from = to;	
				to = m_TextToDisplay.Find('|',from+1);
				pr = m_TextToDisplay.Mid (from+1,to-from-1);				
				if(pr != ""){ 
					switch (yCoordFlag ){ 
													
						
					case 0: 
						dcMem.SetTextColor(m_TickerTextColor);
						dcMem.TextOut (origin,0,pr,pr.GetLength());
						yCoordFlag = 1;
						break;
					case 1:
						dcMem.SetTextColor(m_TickerRateTextColor); 
						dcMem.TextOut (origin,3,pr,pr.GetLength());
						yCoordFlag = 0;
						break;
					}
					origin = origin + (dcMem.GetTextExtent (pr).cx) ;
				}
			}
	}

	else if(m_Mode == MODE_REGULAR_TICKER){
	
		if(i <= - dcMem.GetTextExtent (GetTickerText()).cx ){
			i = m_TickerRect.Width ();
		}
		dcMem.SetTextColor(m_TickerTextColor); 
		dcMem.TextOut (i,0,GetTickerText ());
	}
// NOTE: the ClassWizard will add member functions here
	dcMem.Draw3dRect(&m_TickerRect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	dc.BitBlt(m_TickerRect.left,m_TickerRect.top,m_TickerRect.Width(),m_TickerRect.Height(),&dcMem,0,0,SRCCOPY);

	
	HBITMAP hBmp = (HBITMAP)m_BmpDragBar;
	if(hBmp != NULL){
		BITMAP bm; m_BmpDragBar.GetBitmap(&bm);
		DrawBitmap(dc.m_hDC ,hBmp ,(m_TickerRect.right- bm.bmWidth),0);
	}


    dcMem.SelectObject(pOldBmp);
	DeleteObject(bmpMem.m_hObject);
	dcMem.DeleteDC();

}

void CTicker::DrawBitmap(HDC hdc, HBITMAP hBitmap, int xStart, int yStart)
{// NOTE: the ClassWizard will add member functions here
	BITMAP bm;
	HDC    hdcMem;
	POINT  ptSize, ptOrg;

	hdcMem = CreateCompatibleDC (hdc);
	SelectObject(hdcMem, hBitmap);
	SetMapMode(hdcMem, GetMapMode(hdc));
	GetObject (hBitmap, sizeof(BITMAP), (LPVOID)&bm);
	ptSize.x = bm.bmWidth ;
	ptSize.y = bm.bmHeight ;
	DPtoLP(hdc, &ptSize, 1);
	ptOrg.x = 0;
	ptOrg.y = 0;
	DPtoLP(hdcMem, &ptOrg, 1);
	::BitBlt(hdc,xStart, yStart, ptSize.x, ptSize.y, hdcMem, ptOrg.x, ptOrg.y, SRCCOPY);  
	DeleteDC(hdcMem);
}


void CTicker::StartTicker(int mode)
{	
	
	m_Mode = mode;
	GetClientRect (&m_TickerRect);
	i = m_TickerRect.Width ();
	KillTimer (ID_TICKER_TIMER);
	m_bTickerOn = TRUE;
	SetTimer(ID_TICKER_TIMER,50,NULL);
} 


void CTicker::StopTicker()
{
	m_bTickerOn = FALSE;
	KillTimer (ID_TICKER_TIMER);
}

void CTicker::ResumeTicker()
{
	if(!m_bTickerOn){
		KillTimer (ID_TICKER_TIMER);
		SetTimer (ID_TICKER_TIMER,40,NULL);	
		m_bTickerOn = TRUE;
	}
}

void CTicker::SetTickerText(CString DisplayStr)
{
	m_TextToDisplay = DisplayStr ;

	if(m_TickerTextCharVersion){
		delete m_TickerTextCharVersion;
	}
	m_TickerTextCharVersion = new TCHAR[DisplayStr.GetLength() + 1];
	strcpy(m_TickerTextCharVersion ,(LPCTSTR)DisplayStr);
}// NOTE: the ClassWizard will add member functions here


CString CTicker::GetTickerText()
{
	return m_TextToDisplay;
}

CRect CTicker::GetTickerRect()
{
	return m_TickerRect ;
}

void CTicker::SetTickerRect(CRect rect)
{
	m_TickerRect = rect ;
}

void CTicker::SetTickerFontName(CString FontNameStr)
{
	m_TickerFontName = FontNameStr ;
}

CString CTicker::GetTickerFontName()
{
	return m_TickerFontName ;
}

int CTicker::GetTickerFontHeight()
{
	return m_TickerFontHeight ;
}

void CTicker::SetTickerFontHeight(int ht)
{
	m_TickerFontHeight = ht;
	if(m_pTickerFont)
		delete m_pTickerFont;
		m_pTickerFont = NULL;
	

	m_pTickerFont = new CFont;
	m_pTickerFont->CreateFont ( m_TickerFontHeight ,0,0,0,0,FALSE,FALSE,
			0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH, m_TickerFontName);
}


char* CTicker::GetTickerTextCharVersion()
{// NOTE: the ClassWizard will add member functions here
	return m_TickerTextCharVersion ;
}

void CTicker::SetTickerTextCharVersion(char *Str)
{
	m_TickerTextCharVersion = Str;
}



void CTicker::DrawLinearWash(CDC* pDC, CRect* prc, int iSegments,
								COLORREF crStart, COLORREF crEnd)
{

	COLORREF cr;
	int iR = GetRValue(crStart);
	int iG = GetGValue(crStart);
	int iB = GetGValue(crStart);
	int idR = (GetRValue(crEnd) - iR) / (iSegments - 1);
	int idG = (GetGValue(crEnd) - iG) / (iSegments - 1);
	int idB = (GetBValue(crEnd) - iB) / (iSegments - 1);
	int icx = prc->Width() / iSegments, iLeft = prc->left, iRight;
	pDC->SelectStockObject(NULL_PEN);

	for (int i = 0; i < iSegments; i++, iR += idR, iG += idG, iB += idB)
	{
	

		if (i == (iSegments - 1))
			iRight = prc->right;
		else
			iRight = iLeft + icx;

		cr = RGB(iR, iG, iB);
		
		{
			
			CBrush br(cr);
			CBrush* pbrOld = pDC->SelectObject(&br);
			pDC->Rectangle(iLeft, prc->top, iRight + 1, prc->bottom);
			pDC->SelectObject(pbrOld);
		}
	
		iLeft = iRight;
	}
}


void CTicker::SetTickerSpeed(int sp)
{// NOTE: the ClassWizard will add member functions here
	m_TickerSpeed = sp;
}

int CTicker::GetTickerSpeed()
{
	return m_TickerSpeed;
}

void CTicker::SetTickerTextColor(COLORREF TextColor)
{
	m_TickerTextColor = TextColor;
}

void CTicker::SetTickerTextColor(COLORREF TextColor, COLORREF RatesTextColor)
{
	m_TickerTextColor = TextColor;
	m_TickerRateTextColor = RatesTextColor ;
}

void CTicker::OnLButtonDown(UINT nFlags, CPoint point) 
{
	AfxMessageBox("kk");

}


  
void CTicker::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CStatic::OnMouseMove(nFlags, point);
	
	StopTicker ();	

}



void CTicker::OnDestroy() 
{
	CStatic::OnDestroy();

	
}

void CTicker::ShowRatesWithBkgBmp(CString pszFileName,COLORREF bkg, COLORREF fore, LPCTSTR pszFontName, int FontHeight, int TickerSpeed, int mode, int BmpResId)
{
	m_bIsBkgBmp = TRUE;
	m_BmpBkg.LoadBitmap (BmpResId);
	ShowRates(pszFileName, bkg, fore, pszFontName, FontHeight, TickerSpeed, mode);
}

void CTicker::ShowRates(CString pszFileName, COLORREF bkg, COLORREF fore, LPCTSTR pszFontName, int FontHeight, int TickerSpeed, int mode)
{//×Ó´°¿Ú¿ò¼Ü
	
	int completelen = strlen(AfxGetApp()->m_pszHelpFilePath);
	char psz[1000];
	strcpy(psz,AfxGetApp()->m_pszHelpFilePath);
	psz[completelen - 14] = '\0';

	strcpy(psz,pszFileName);

	HANDLE hFile = CreateFile ( psz,GENERIC_ALL,
		FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(hFile != INVALID_HANDLE_VALUE){
		DWORD dwSize = GetFileSize(hFile, NULL), dwActualSize;
		char* pBuff=  new char[dwSize];
		ReadFile ( hFile,pBuff, dwSize, &dwActualSize,NULL);
		CString strTempBuff = pBuff;
		CloseHandle(hFile);
		delete pBuff;
		SetTickerText(strTempBuff);
	}
	else{
		SetTickerText("");
	}

	SetTickerTextColor (bkg,fore);
	SetTickerFontName (pszFontName);
	SetTickerFontHeight (FontHeight);
	SetTickerSpeed (TickerSpeed);  
	StartTicker(mode);	

}

