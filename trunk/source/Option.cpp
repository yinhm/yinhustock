// Option.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "Option.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COPTION

IMPLEMENT_DYNAMIC(COPTION, CPropertySheet)

COPTION::COPTION(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_bInit = false;
}

COPTION::COPTION(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_bInit = false;
}

COPTION::~COPTION()
{
}


BEGIN_MESSAGE_MAP(COPTION, CPropertySheet)
	//{{AFX_MSG_MAP(COPTION)
	ON_WM_HELPINFO()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COPTION message handlers

void COPTION::AddNewPage()
{
    AddPage(&m_syswg);
	AddPage(&m_syspara);
	AddPage(&m_syssetup);
}

BOOL COPTION::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    int ActiveIndex= GetActiveIndex();
	int Idd;
	switch(ActiveIndex)
	{
	case 0:
		Idd=IDD_WG2_SYSTEMINFO;
		break;
	case 1:
		Idd=IDD_SET_PARAM;
		break;
	case 2:
		Idd=IDD_SZ_SYSTEMINFO;
		break;
	default:
		Idd=IDD_WG2_SYSTEMINFO;
	}

	DoHtmlHelp(this);return TRUE;

}

void COPTION::DoDataExchange(CDataExchange* pDX) 
{
	
	
	CPropertySheet::DoDataExchange(pDX);
	if(m_bInit == false)
	{

	m_bInit = true;
	}
}

int COPTION::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	
	return 0;
}

BOOL COPTION::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	m_ok.SubclassDlgItem( IDOK, this );
	m_cancel.SubclassDlgItem( IDCANCEL, this );
	
	return bResult;
}
