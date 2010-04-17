// DIALOGGSSM.cpp : implementation file
//

#include "stdafx.h"
#include "DIALOGGSSM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogLOGGSSM::CDialogLOGGSSM(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLOGGSSM::IDD, pParent)
{
	m_bReadOnly = true;
	//{{AFX_DATA_INIT(CDialogLOGGSSM)
	m_edit = _T("");
	//}}AFX_DATA_INIT
}


void CDialogLOGGSSM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLOGGSSM)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLOGGSSM, CDialog)
	//{{AFX_MSG_MAP(CDialogLOGGSSM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDialogLOGGSSM::OnOK() 
{
	
	UpdateData();
	
	CDialog::OnOK();
}

BOOL CDialogLOGGSSM::OnInitDialog() 
{
	CString m=m_edit;

	int j=0;
	CDialog::OnInitDialog();

	if(m_bReadOnly == true)
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetReadOnly(TRUE);

	UpdateData(FALSE);

	
	return TRUE;  
}
