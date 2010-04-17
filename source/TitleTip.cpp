////////////////////////////////////////////////////////////////////////////
// TitleTip.cpp : implementation file
//by yml
//tel:13366898744

#include "stdafx.h"
#include "TitleTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleTip

CTitleTip::CTitleTip()
{

	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, TITLETIP_CLASSNAME, &wndcls)))
	{
	
		wndcls.style			= CS_SAVEBITS ;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= LoadCursor( hInst, IDC_ARROW );
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= TITLETIP_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}
}

CTitleTip::~CTitleTip()
{
}


BEGIN_MESSAGE_MAP(CTitleTip, CWnd)
	//{{AFX_MSG_MAP(CTitleTip)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////


BOOL CTitleTip::Create(CWnd * pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_pParentWnd = pParentWnd;

	return CreateEx( dwExStyle, TITLETIP_CLASSNAME, NULL, dwStyle, 0, 0, 0, 0, 
		NULL, NULL, NULL );
}



void CTitleTip::Show(CRect rectTitle, LPCTSTR lpszTitleText, int xoffset /*=0*/)
{// NOTE: the ClassWizard will add member functions here
	ASSERT( ::IsWindow( GetSafeHwnd() ) );
	ASSERT( !rectTitle.IsRectEmpty() );

	
	if( IsWindowVisible() ) 
		return;


	if( GetFocus() == NULL )
		return;


	m_rectTitle.top    = -1;
	m_rectTitle.left   = -xoffset-1;
	m_rectTitle.right  = rectTitle.Width()-xoffset;
	m_rectTitle.bottom = rectTitle.Height()+1;


	m_pParentWnd->ClientToScreen( rectTitle );

	CClientDC dc(this);
	CString strTitle = _T(" ");	strTitle += lpszTitleText; strTitle += _T(" ");

	CFont *pFont = m_pParentWnd->GetFont(); 
	CFont *pFontDC = dc.SelectObject( pFont );
	CSize size = dc.GetTextExtent( strTitle );

	CRect rectDisplay = rectTitle;
	rectDisplay.left += xoffset;
	rectDisplay.right = rectDisplay.left + size.cx + xoffset;


	if( rectDisplay.right <= rectTitle.right-xoffset )
		return;


	SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top, 
			rectDisplay.Width(), rectDisplay.Height(), 
			SWP_SHOWWINDOW|SWP_NOACTIVATE );

	dc.SetBkMode( TRANSPARENT );
	dc.TextOut( 0, 0, strTitle );
	dc.SelectObject( pFontDC );

	SetCapture();
}

void CTitleTip::Hide()
{
  	if (!::IsWindow(GetSafeHwnd()))
        return;

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
        ReleaseCapture();

	ShowWindow( SW_HIDE );
}

void CTitleTip::OnMouseMove(UINT nFlags, CPoint point) 
{// NOTE: the ClassWizard will add member functions here
    if (!m_rectTitle.PtInRect(point)) 
    {
        Hide();
        
    
        ClientToScreen( &point );
        CWnd *pWnd = WindowFromPoint( point );
        if ( pWnd == this ) 
            pWnd = m_pParentWnd;
        
        int hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
        
        if (hittest == HTCLIENT) {
            pWnd->ScreenToClient( &point );
            pWnd->PostMessage( WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y) );
        } else {
            pWnd->PostMessage( WM_NCMOUSEMOVE, hittest, MAKELONG(point.x,point.y) );
        }
    }
}

BOOL CTitleTip::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd;
	int hittest;
	POINTS pts = MAKEPOINTS( pMsg->lParam );

	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		POINT  point;
		point.x = pts.x;
		point.y = pts.y;
		ClientToScreen( &point );
		pWnd = WindowFromPoint( point );
		if( pWnd == this ) 
			pWnd = m_pParentWnd;

		hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

		if (hittest == HTCLIENT) {
			pWnd->ScreenToClient( &point );
			pMsg->lParam = MAKELONG(point.x,point.y);
		} else {
			switch (pMsg->message) {
			case WM_LBUTTONDOWN: 
				pMsg->message = WM_NCLBUTTONDOWN;
				break;
			case WM_RBUTTONDOWN: 
				pMsg->message = WM_NCRBUTTONDOWN;
				break;
			case WM_MBUTTONDOWN: 
				pMsg->message = WM_NCMBUTTONDOWN;
				break;
			}
			pMsg->wParam = hittest;
			pMsg->lParam = MAKELONG(point.x,point.y);
		}

        Hide();
		pWnd->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
		return TRUE;	
		
	case WM_KEYDOWN:
        Hide();
		m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
		return TRUE;
	case WM_SYSKEYDOWN:
        Hide();
		m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
		return TRUE;
	}

	if( GetFocus() == NULL )
	{
        Hide();
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}
