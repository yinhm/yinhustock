// TechDlgParamExplain.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgParamExplain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgParamExplain dialog


CTaiKlineDlgParamExplain::CTaiKlineDlgParamExplain(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgParamExplain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgParamExplain)
	m_sExplain = _T("");
	//}}AFX_DATA_INIT
}


void CTaiKlineDlgParamExplain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgParamExplain)
	DDX_Text(pDX, IDC_EDIT1, m_sExplain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgParamExplain, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgParamExplain)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgParamExplain message handlers
