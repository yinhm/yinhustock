// CwdSetParamDlg.cpp : implementation file
// by ÓáÃ÷Â¼

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CwdSetParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFxjSetParamDlg dialog


CFxjSetParamDlg::CFxjSetParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFxjSetParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFxjSetParamDlg)
		
	//}}AFX_DATA_INIT
}


void CFxjSetParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjSetParamDlg)
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjSetParamDlg, CDialog)
	//{{AFX_MSG_MAP(CFxjSetParamDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CFxjSetParamDlg::OnOK() 
{
	

}

void CFxjSetParamDlg::OnCancel() 
{
	

}

CString CFxjSetParamDlg::GetText()
{
	CString s;
	m_edit.GetWindowText (s);
	return s;
}

void CFxjSetParamDlg::SetText(CString s,int nMin,int nMax)
{
	m_edit.SetWindowText (s);
	m_spin.SetRange32(nMin, nMax);
}

void CFxjSetParamDlg::SetSelEdit()
{
	m_edit.SetFocus ();
	m_edit.SetSel (0,-1);

}
