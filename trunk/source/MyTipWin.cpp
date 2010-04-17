// MyTipWin.cpp : implementation file
//

#include "stdafx.h"
#include "MyTipWin.h"
#include "CTaiShanApp.h"
#include "MainFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define TITLETIP_CLASSNAME _T("ZTitleTip")

/////////////////////////////////////////////////////////////////////////////
// CTaiTestTipWin

CTaiTestTipWin::CTaiTestTipWin()
{
	m_pOld = CPoint(0,0);
	CString str="1223345667889";
	m_stringList.AddTail(str);
	m_stringList.AddTail("力量");
	m_stringList.AddTail("了解");

	max=0;
	m_pParentWnd = 0;
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

CTaiTestTipWin::~CTaiTestTipWin()
{
}


BEGIN_MESSAGE_MAP(CTaiTestTipWin, CWnd)
	//{{AFX_MSG_MAP(CTaiTestTipWin)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////


int CTaiTestTipWin::CalcSize()
{
	CDC* pDC=GetDC();
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];

	ft.CreateFontIndirect (&lgf );
	pDC->SetTextAlign( TA_LEFT );
	pDC->SetBkMode(TRANSPARENT);

	int nMaxY=GetSystemMetrics(SM_CYSCREEN);
	m_height=(pDC->GetOutputTextExtent("上海")).cy+2;
	int count=m_stringList.GetCount ();
	m_maxHeight=count*m_height+5;



	CString str;
	CFont* pOldFont=pDC->SelectObject(&ft);
	int len;
	max=0;
    for(POSITION pos = m_stringList.GetHeadPosition(); pos != NULL; )  
    {
		str=(m_stringList.GetNext( pos ) );   
		len=5+(pDC->GetOutputTextExtent(str)).cx;
		if(len>max)
			max=len;
    }
    pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
  return max;
}
int calc(int i)
{

	int j;
	j=8*i;

	return j;
}
void CTaiTestTipWin::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove( nFlags,  point) ;
}
void CTaiTestTipWin::OnPaint() 
{
	CPaintDC dc(this); 
	CalcSize();
	


	CString str;

	CRect rt;
	GetClientRect(rt);
	CBrush brush;
	if (!brush.CreateSolidBrush(RGB(255,255,220)))
		return;
	CBrush* pOldBrush=dc.SelectObject(&brush);
	dc.FillRect(rt,&brush);
	dc.SelectObject(pOldBrush);

	dc.SetTextColor( RGB(0,0,0));

	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	ft.CreateFontIndirect (&lgf );
	dc.SetTextAlign( TA_LEFT );
	dc.SetBkMode(TRANSPARENT);

	CFont* pOldFont=dc.SelectObject(&ft);
	dc.SetBkMode (TRANSPARENT   );
	int i=0;
   for(POSITION pos = m_stringList.GetHeadPosition(); pos != NULL;i++ )  
   {
		str=(m_stringList.GetNext( pos ) );   
		dc.TextOut (2,i*m_height+2,str);
   }
   dc.SelectObject(pOldFont);
	int f1=8;
}

void CTaiTestTipWin::SetString(CString &s)
{
	m_stringList.RemoveAll ();
	int start=0;
	while(start!=-1)
	{
		int first=start;
		start=s.Find ("\n",start);
		CString tmp;
		if(start!=-1)
		{
			tmp=s.Mid( first, start-first);
			m_stringList.AddTail (tmp);
			start++;
		}
		else
			break;
	}
}

void CTaiTestTipWin::InitMember()
{

}

BOOL CTaiTestTipWin::Create(CWnd * pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_pParentWnd = pParentWnd;

	return CreateEx( dwExStyle, TITLETIP_CLASSNAME, NULL, dwStyle, 0, 0, 0, 0, 
		NULL, NULL, NULL );
}

void CTaiTestTipWin::Show(LPCTSTR lpszTitleText, CPoint p)
{
	ASSERT( ::IsWindow( GetSafeHwnd() ) );


	if( GetFocus() == NULL )
		return;


	m_pParentWnd->ClientToScreen( &p );

	CClientDC dc(this);
	CString strTitle = lpszTitleText;

	this->SetString (strTitle);
	this->CalcSize ();

	m_pOld = p;
	SetTimer(10,200,NULL);

}

void CTaiTestTipWin::Hide()
{
  	if (!::IsWindow(GetSafeHwnd()))
        return;

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
      ReleaseCapture();

	ShowWindow( SW_HIDE );
}

BOOL CTaiTestTipWin::PreTranslateMessage(MSG* pMsg) 
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

void CTaiTestTipWin::OnTimer(UINT nIDEvent) 
{
	
	KillTimer(10);
	SetWindowPos(&wndTop,m_pOld.x+15,m_pOld.y+15,
					max,m_maxHeight,SWP_SHOWWINDOW|SWP_NOACTIVATE   );

	CWnd::OnTimer(nIDEvent);
}
