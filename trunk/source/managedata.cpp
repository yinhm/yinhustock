// ManageData.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ManageData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperviseData

IMPLEMENT_DYNAMIC(CSuperviseData, CPropertySheet)

CSuperviseData::CSuperviseData(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CSuperviseData::CSuperviseData(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CSuperviseData::~CSuperviseData()
{
}


BEGIN_MESSAGE_MAP(CSuperviseData, CPropertySheet)
	//{{AFX_MSG_MAP(CSuperviseData)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////



BOOL CSuperviseData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    int ActiveIndex=GetActiveIndex();
    int Idd;
	switch(ActiveIndex)
	{
	case 0:
		Idd=IDD_DATAIMPORT;
        break;
	case 1:
		Idd=IDD_DATAEXPOR;
		break;
    case 2:
		Idd=IDD_CLEAR_DATA;
		break;
	case 3:
		Idd=IDD_CAIWUINOUT;
		break;
	case 4:
		Idd=IDD_CHUQUANDATA;
		break;
	case 5:
		Idd=IDD_CONTRAST_SYMBOL;
		break;
	default:
        Idd=IDD_MANAGEDATA; 

	}
        

	DoHtmlHelp(this);return TRUE;
}

BOOL CSuperviseData::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	m_ok.SubclassDlgItem( IDOK, this );
	m_cancel.SubclassDlgItem( IDCANCEL, this );

	
	
	return bResult;
}

void CSuperviseData::DoDataExchange(CDataExchange* pDX) 
{
	CPropertySheet::DoDataExchange(pDX);

}
