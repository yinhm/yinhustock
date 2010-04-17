// CTaiKlineShowText.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiKlineShowText.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDrawLine.h"
#include "CTaiKlineDoKline.h"
//#include "vwbaseDoc.h"
#define TM_SHOW_TEXT   1
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MESSAGE_SHOW_MY WM_USER+122



CTaiKlineShowText::CTaiKlineShowText(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineShowText::IDD, pParent)
{
	pView = (CTaiShanKlineShowView*) pParent;
	ASSERT(pView!=NULL);

	//{{AFX_DATA_INIT(CTaiKlineShowText)
	m_editText = _T("");
	//}}AFX_DATA_INIT
}


void CTaiKlineShowText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineShowText)
	DDX_Text(pDX, IDC_EDIT2, m_editText);
	DDV_MaxChars(pDX, m_editText, 180);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineShowText, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineShowText)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MESSAGE_SHOW_MY,ShowMyWin)
END_MESSAGE_MAP()

BOOL CTaiKlineShowText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_editText="";
	m_bMove=true;

	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	x=pView->m_toScreen.x;
	y=pView->m_toScreen.y;

	SetTimer(TM_SHOW_TEXT,5,NULL);


	return TRUE;  
}

void CTaiKlineShowText::OnChangeEdit2() 
{
	
	int n = m_editText.GetLength();
	if(n>=31)
		AfxMessageBox("超过最大字符容量",MB_ICONSTOP);
	else
		UpdateData(TRUE);

	
}


void CTaiKlineShowText::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{

	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CTaiKlineShowText::PreCreateWindow(CREATESTRUCT& cs) 
{

	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	x=pView->pDrawLine ->m_pt[0].x;
	y=pView->pDrawLine ->m_pt[0].y;
	cs.x=x;
	cs.y=y;
	cs.cx=60;
	cs.cy=20;
	return CDialog::PreCreateWindow(cs);
}
void CTaiKlineShowText::OnOK() 
{

	CDialog::OnOK();
}

void CTaiKlineShowText::ShowWin()
{
}


int CTaiKlineShowText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	
	return 0;
}
LRESULT CTaiKlineShowText::ShowMyWin(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	x=pView->m_toScreen.x;
	y=pView->m_toScreen.y;
	MoveWindow(x,y,90,20);
	RedrawWindow();
	return 0;
}

void CTaiKlineShowText::OnPaint() 
{
	CPaintDC dc(this); 
	CDialog::OnPaint() ;
	

}

void CTaiKlineShowText::OnCancelMode() 
{
	CDialog::OnCancelMode();
	

	
}

int CTaiKlineShowText::DoModal() 
{

	
	return CDialog::DoModal();
}

void CTaiKlineShowText::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	
}

BOOL CTaiKlineShowText::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{

	int i;
	i=0;
	return CDialog::Create(IDD, pParentWnd);
}

void CTaiKlineShowText::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	

	
}

void CTaiKlineShowText::OnTimer(UINT nIDEvent) 
{
	
	KillTimer(TM_SHOW_TEXT);
	ShowMyWin(0, 0);
	CDialog::OnTimer(nIDEvent);
}

void CTaiKlineShowText::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	

	
}

void CTaiKlineShowText::OnActivateApp(BOOL bActive, DWORD hTask) 
{
	CDialog::OnActivateApp(bActive, hTask);
	

	
}
