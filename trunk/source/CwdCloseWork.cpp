// CwdCloseWork.cpp : implementation file
// by ÓáÃ÷Â¼

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CwdCloseWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFxjCloseWork property page

IMPLEMENT_DYNCREATE(CFxjCloseWork, CPropertyPage)

CFxjCloseWork::CFxjCloseWork() : CPropertyPage(CFxjCloseWork::IDD)
{
	//{{AFX_DATA_INIT(CFxjCloseWork)

	//}}AFX_DATA_INIT
}

CFxjCloseWork::~CFxjCloseWork()
{
}

void CFxjCloseWork::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjCloseWork)
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjCloseWork, CPropertyPage)
	//{{AFX_MSG_MAP(CFxjCloseWork)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFxjCloseWork message handlers

void CFxjCloseWork::OnButton1() 
{
	SendMessage(WM_COMMAND,ID_TOOL_CLOSEWORK);
}
