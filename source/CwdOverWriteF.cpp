// CwdOverWriteF.cpp : implementation file
// by ÓáÃ÷Â¼
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CwdOverWriteF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////



CFxjOverWriteF::CFxjOverWriteF(CWnd* pParent /*=NULL*/)
	: CDialog(CFxjOverWriteF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFxjOverWriteF)
	m_s1 = _T("");
	//}}AFX_DATA_INIT
	m_nResponse = 0;
}


void CFxjOverWriteF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjOverWriteF)
	DDX_Text(pDX, IDC_STATIC_1, m_s1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjOverWriteF, CDialog)
	//{{AFX_MSG_MAP(CFxjOverWriteF)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CFxjOverWriteF::OnButtonAll() 
{
	
	m_nResponse = 2;
	CDialog::OnOK();
}

void CFxjOverWriteF::OnCancel() 
{

	
	m_nResponse = 0;
	CDialog::OnCancel();
}

void CFxjOverWriteF::OnOK() 
{

	
	m_nResponse = 1;
	CDialog::OnOK();
}
