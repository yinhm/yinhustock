// DIAGSGLMM.cpp : implementation file
//

#include "stdafx.h"
#include "DIAGSGLMM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogGSGLMM::CDialogGSGLMM(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGSGLMM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGSGLMM)
	m_input = _T("");
	//}}AFX_DATA_INIT
}


void CDialogGSGLMM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGSGLMM)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_EDIT1, m_editcor);
	DDX_Text(pDX, IDC_EDIT1, m_input);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGSGLMM, CDialog)
	//{{AFX_MSG_MAP(CDialogGSGLMM)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDialogGSGLMM::OnOK() 
{

	UpdateData();
	if(m_input==password)
		CDialog::OnOK();
	else 
	{
		AfxMessageBox("√‹¬Î¥ÌŒÛ£¨«Î÷ÿ–¬ ‰»Î",MB_OK);
		m_editcor.SetFocus();
	}
}

BOOL CDialogGSGLMM::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_editcor.SetFocus();
	UpdateData(FALSE);
	
	return FALSE;  
}
