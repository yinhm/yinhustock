// NetPathSet.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "NetPathSet.h"
#include "SBDestination.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetPathSet dialog


CNetPathSet::CNetPathSet()
	: CPropertyPage(CNetPathSet::IDD)
{
	//{{AFX_DATA_INIT(CNetPathSet)
	m_serverPath = _T("");	
	m_sDataBakPath = _T("");
	m_nMax0 = 0;
	m_nMax1 = 0;
	m_nMax2 = 0;
	//}}AFX_DATA_INIT
	CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc ;
	m_serverPath  = pDoc->m_serverPath;
	m_sDataBakPath  = pDoc->m_sDataBakPath;

	m_nMax0 = (DWORD)pDoc->m_propertyInitiate.wNetCachNum [0] ;
	m_nMax1 = (DWORD)pDoc->m_propertyInitiate.wNetCachNum [1] ;
	m_nMax2 = (DWORD)pDoc->m_propertyInitiate.wNetCachNum [2] ;
}


void CNetPathSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetPathSet)
	DDX_Control(pDX, IDC_EDIT5, m_f2);
	DDX_Control(pDX, IDC_EDIT3, m_f1);
	DDX_Control(pDX, IDC_EDIT2, m_f0);
	DDX_Text(pDX, IDC_EDIT1, m_serverPath);
	DDX_Text(pDX, IDC_EDIT4, m_sDataBakPath);
	DDX_Text(pDX, IDC_EDIT2, m_nMax0);
	DDV_MinMaxDWord(pDX, m_nMax0, 0, 1000);
	DDX_Text(pDX, IDC_EDIT3, m_nMax1);
	DDV_MinMaxDWord(pDX, m_nMax1, 0, 500);
	DDX_Text(pDX, IDC_EDIT5, m_nMax2);
	DDV_MinMaxDWord(pDX, m_nMax2, 0, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetPathSet, CPropertyPage)
	//{{AFX_MSG_MAP(CNetPathSet)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonSetServerPath)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonBakPathSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetPathSet message handlers

BOOL CNetPathSet::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	
	return TRUE;  
}

void CNetPathSet::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	
	
}

void CNetPathSet::OnOK() 
{
	
	UpdateData();
	SaveDataToReg();
	CPropertyPage::OnOK();
}

void CNetPathSet::OnCancel() 
{

	
	CPropertyPage::OnCancel();
}

void CNetPathSet::OnButtonSetServerPath() 
{

	UpdateData(TRUE);
	CSBDestination sb(m_hWnd, IDS_BFF_TITLE);
	sb.SetFlags(BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT);
	sb.SetInitialSelection(m_serverPath);
	if (sb.SelectFolder())
	{
		m_serverPath = sb.GetSelectedFolder();
		UpdateData(FALSE);
	}
	
}

void CNetPathSet::OnButtonBakPathSet() 
{

	UpdateData(TRUE);
	CSBDestination sb(m_hWnd, IDS_BFF_TITLE);
	sb.SetFlags(BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT);
	sb.SetInitialSelection(m_sDataBakPath);
	if (sb.SelectFolder())
	{
		m_sDataBakPath = sb.GetSelectedFolder();
		UpdateData(FALSE);
	}
	
}

void CNetPathSet::SaveDataToReg()
{
    CWinApp* pApp = AfxGetApp();
    CString strSection       = "file_set_path";
	CString strStringItem  ;

	strStringItem  =  "m_serverPath";
	pApp->WriteProfileString(strSection, strStringItem, m_serverPath);  
	strStringItem  =  "m_sDataBakPath";
	pApp->WriteProfileString(strSection, strStringItem, m_sDataBakPath);  

	CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc ;
	pDoc->m_serverPath  =m_serverPath  ;
	pDoc->m_sDataBakPath  =m_sDataBakPath  ;
	;
	pDoc->m_propertyInitiate.wNetCachNum [0] = (DWORD)m_nMax0 ;
	pDoc->m_propertyInitiate.wNetCachNum [1] = (DWORD)m_nMax1 ;
	pDoc->m_propertyInitiate.wNetCachNum [2] = (DWORD)m_nMax2 ;
	
	if(pDoc->m_propertyInitiate.wNetCachNum [0]>100
		||pDoc->m_propertyInitiate.wNetCachNum [0]>50
		||pDoc->m_propertyInitiate.wNetCachNum [0]>100)
		AfxMessageBox("您缓存的股票个数较大，如果您的内存低于128兆，请重新设置！");

}
