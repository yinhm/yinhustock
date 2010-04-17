// CAlertPopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CAlertPopDlg.h"
#include "CAlertSystem.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertPopDlg dialog


CAlertPopDlg::CAlertPopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertPopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertPopDlg)
	m_sInfo = _T("");
	//}}AFX_DATA_INIT
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	pFm->m_pDlgAlarmPop = this;
}


void CAlertPopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertPopDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON1, m_bt1);
	DDX_Control(pDX, IDC_LIST1, m_conInfo);
	DDX_LBString(pDX, IDC_LIST1, m_sInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertPopDlg, CDialog)
	//{{AFX_MSG_MAP(CAlertPopDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnToAlarmSystem)
	ON_BN_CLICKED(IDOK, OnToKlineFiguer)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertPopDlg message handlers

void CAlertPopDlg::OnToAlarmSystem() 
{
	
	CAlertSystem::CreateNew();
	
}

void CAlertPopDlg::OnToKlineFiguer() 
{//为该类的对象

	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	CString rString;
	int n = m_conInfo.GetCount ();
	if(n==0)
		return;

	int nIndex;
	nIndex = m_conInfo.GetCurSel ();
	if(nIndex <0 ||nIndex >n-1)
		nIndex = n-1;
	m_conInfo.GetText (nIndex,rString);
	n = rString.Find (":");
	if(n>0)
	{
		CString s = rString.Left (n);
		if(s.GetLength ()==4 || s.GetLength ()==6)
		{
			int stkKind = m_conInfo.GetItemData (nIndex);
			pFm->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)s.GetBuffer (0),stkKind*10);
			DestroyWindow();
		}
	}
	
}

BOOL CAlertPopDlg::OnInitDialog() 
{//对话框基类
	CDialog::OnInitDialog();
	

	SetTimer(1,15000,NULL);
	return TRUE;  
}

void CAlertPopDlg::OnClose() 
{
	
	DestroyWindow();
	

}

void CAlertPopDlg::PostNcDestroy() 
{//对话框基类
	
	delete this;
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	pFm->m_pDlgAlarmPop = NULL;

	CDialog::PostNcDestroy();
}

void CAlertPopDlg::OnCancel() 
{

	DestroyWindow();
	

}

void CAlertPopDlg::OnTimer(UINT nIDEvent) 
{//对话框基类

	CDialog::OnTimer(nIDEvent);
	if(nIDEvent == 1)
		DestroyWindow();
}

void CAlertPopDlg::OnDblclkList1() 
{//对话框基类

	OnToKlineFiguer() ;
}
