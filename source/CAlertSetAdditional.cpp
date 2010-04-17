// CAlertSetAdditional.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CAlertSetAdditional.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertSetAdditional dialog


CAlertSetAdditional::CAlertSetAdditional(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertSetAdditional::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlertSetAdditional)
	m_bSound = FALSE;
	m_bContinueWatch = FALSE;
	m_bMessageBox = FALSE;
	//}}AFX_DATA_INIT
}


void CAlertSetAdditional::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertSetAdditional)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Check(pDX, IDC_CHECK1, m_bSound);
	DDX_Check(pDX, IDC_CHECK3, m_bContinueWatch);
	DDX_Check(pDX, IDC_CHECK2, m_bMessageBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertSetAdditional, CDialog)

END_MESSAGE_MAP()


void CAlertSetAdditional::OnOK() 
{

	
	CDialog::OnOK();
}
