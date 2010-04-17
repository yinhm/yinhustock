// CatalogStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CatalogStatic.h"
#include "MyRichView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BLUE	RGB(0, 0, 238)
#define PURPLE  RGB(85,  26, 139)
#define RED		RGB(255, 0, 0)

#define STATICRES 3000
/////////////////////////////////////////////////////////////////////////////
// CCatalogStatic

CCatalogStatic::CCatalogStatic()
{
	m_hCursor			= NULL;                 
    m_crLinkColor       = BLUE;		
    m_crVisitedColor    = PURPLE;

	m_crHoverColor		= RED;
    m_bVisited          = FALSE;  
	m_bOverControl		= FALSE;
    m_bUnderline        = TRUE;                 
    m_bAdjustToFit      = TRUE;                 
}

CCatalogStatic::~CCatalogStatic()
{
}


BEGIN_MESSAGE_MAP(CCatalogStatic, CStatic)
	//{{AFX_MSG_MAP(CCatalogStatic)
	ON_CONTROL_REFLECT(STN_CLICKED, OnClicked)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

HBRUSH CCatalogStatic::CtlColor(CDC *pDC, UINT nCtlColor)
{
	ASSERT(nCtlColor == CTLCOLOR_STATIC);

	if(m_bOverControl)
		pDC->SetTextColor(m_crHoverColor);
	else if(m_bVisited)
		pDC->SetTextColor(m_crVisitedColor);
	else
		pDC->SetTextColor(m_crLinkColor);

	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);

}

BOOL CCatalogStatic::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message)
{
	if(m_hCursor)
	{
		::SetCursor(m_hCursor);
	}
	return FALSE;
}

void CCatalogStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);

	CRect rect;
	GetClientRect(&rect);

	if(rect.PtInRect(point))
	{
		m_bOverControl = TRUE;

		SetCapture();
		RedrawWindow();
		return;
	}
	else
	{
		m_bOverControl = FALSE;
		ReleaseCapture();
		RedrawWindow();
		return;
	}

}

void CCatalogStatic::PreSubclassWindow() 
{
	DWORD dwStyle = GetStyle();
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 13;
    lf.lfWidth = 0;
	lf.lfWeight = 200;
    lf.lfUnderline = m_bUnderline;
	strcpy(lf.lfFaceName, "ËÎÌå");

	m_Font.CreateFontIndirect(&lf);
	SetFont(&m_Font);

	PositionWindow();
	SetDefaultCursor();
	
	CStatic::PreSubclassWindow();
}

BOOL CCatalogStatic::PreTranslateMessage(MSG* pMsg) 
{
	return CStatic::PreTranslateMessage(pMsg);
}

void CCatalogStatic::PositionWindow()
{
	if (!::IsWindow(GetSafeHwnd()) || !m_bAdjustToFit) 
        return;

  
    CRect rect;
    GetWindowRect(rect);

    CWnd* pParent = GetParent();
    if (pParent)
        pParent->ScreenToClient(rect);

  
    CString strWndText;
    GetWindowText(strWndText);

    CDC* pDC = GetDC();
    CFont* pOldFont = pDC->SelectObject(&m_Font);
    CSize Extent = pDC->GetTextExtent(strWndText);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

  
    DWORD dwStyle = GetStyle();

    
    if (dwStyle & SS_CENTERIMAGE)
        rect.DeflateRect(0, (rect.Height() - Extent.cy)/2);
    else
        rect.bottom = rect.top + Extent.cy;

    if (dwStyle & SS_CENTER)   
        rect.DeflateRect((rect.Width() - Extent.cx)/2, 0);
    else if (dwStyle & SS_RIGHT) 
        rect.left  = rect.right - Extent.cx;
    else 
        rect.right = rect.left + Extent.cx;

   
    SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
}

void CCatalogStatic::SetDefaultCursor()
{
	if (m_hCursor == NULL)               
    {
      
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
    }
}

void CCatalogStatic::OnClicked()
{
	m_bVisited = TRUE; 
	
	CTaiTestRichView *pView = (CTaiTestRichView *)GetParent();

	int n = GetDlgCtrlID();

	if(n == 4001)
	{
		if(pView->nCurrentPage == 1) return;
		pView->HidePage(pView->nCurrentPage);
		pView->ShowPage(1);
		pView->nCurrentPage = 1;
	}
	else if(n == 4002)
	{
		if(pView->nCurrentPage == pView->nCatalogPage + pView->m_CatalogMap.GetCount())
			return;
		pView->HidePage(pView->nCurrentPage);
		pView->ShowPage(pView->nCurrentPage+1);
	}
	else if(n == 4003)
	{
		if(pView->nCurrentPage == 1) return;
		pView->HidePage(pView->nCurrentPage);
		pView->ShowPage(pView->nCurrentPage -1);
	}
	else
	{
		n -= 3000;
		PAFFICHEFILE  pAff;
		if(pView->aFile.m_FileMap .Lookup(n, pAff))
		{
			pView->HidePage(pView->nCurrentPage);
			pView->nCurrentPage = pView->nCatalogPage + n + 1;
			pView->ShowPage(pView->nCurrentPage);
		}
		
	}


	
}
