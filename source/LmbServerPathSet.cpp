// LmbServerPathSet.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "LmbServerPathSet.h"
#include "CTaiShanDoc.h"
#include "mainfrm.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LmbServerPathSet dialog


LmbServerPathSet::LmbServerPathSet(CWnd* pParent /*=NULL*/)
	: CDialog(LmbServerPathSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(LmbServerPathSet)
	m_serverPath = _T("");
	//}}AFX_DATA_INIT
	CWinApp* pApp = AfxGetApp();
	CString strSection       = "file_set_path";
	CString strStringItem  =  "m_serverPath";
	m_serverPath = pApp->GetProfileString(strSection, strStringItem);
}


void LmbServerPathSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LmbServerPathSet)
	DDX_Text(pDX, IDC_EDIT1, m_serverPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LmbServerPathSet, CDialog)
	//{{AFX_MSG_MAP(LmbServerPathSet)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LmbServerPathSet message handlers

void LmbServerPathSet::OnCancel() 
{

	
	CDialog::OnCancel();
}

void LmbServerPathSet::OnOK() 
{
	
	UpdateData();
	if(TestPath(true))
		CDialog::OnOK();
}

void LmbServerPathSet::OnButton1() 
{

	CSBDestination sb(m_hWnd, IDS_BFF_TITLE);
	sb.SetFlags(BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT);
	sb.SetInitialSelection(m_serverPath);
	if (sb.SelectFolder())
	{
		m_serverPath = sb.GetSelectedFolder();
		UpdateData(FALSE);
	}

	
}

int LmbServerPathSet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	lpCreateStruct->dwExStyle|=WS_EX_TOPMOST  ; 

	
	return 0;
}

bool LmbServerPathSet::TestPath(bool bMessage)
{
    CWinApp* pApp = AfxGetApp();
    CString strSection       = "file_set_path";
	CString strStringItem  ;  
	


	CFileFind finder;
	m_serverPath .TrimRight("\\");
	m_serverPath .TrimRight("/");
	BOOL bWorking = finder.FindFile(m_serverPath+"\\data\\shanghai\\buysell.dat");
	if(bWorking==FALSE || m_serverPath=="")
	{
		if(bMessage==true)AfxMessageBox("请重新设置服务器路径!");
		return false;
	}
	bWorking = finder.FindFile(m_serverPath+"\\data\\shenzhen\\buysell.dat");
	if(bWorking==FALSE)
	{
		if(bMessage==true)AfxMessageBox("请重新设置服务器路径!");
		return false;
	}

	strStringItem  =  "m_serverPath";
	pApp->WriteProfileString(strSection, strStringItem, m_serverPath);  
	return true;

}
