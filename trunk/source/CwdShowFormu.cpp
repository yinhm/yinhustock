// CwdShowFormu.cpp : implementation file
// by ÓáÃ÷Â¼

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CwdShowFormu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////



CFxjShowFormu::CFxjShowFormu(CWnd* pParent /*=NULL*/)
	: CDialog(CFxjShowFormu::IDD, pParent)
{
	m_title = "";
	//{{AFX_DATA_INIT(CFxjShowFormu)
	m_s = _T("");
	//}}AFX_DATA_INIT
}


void CFxjShowFormu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjShowFormu)
	DDX_Control(pDX, IDC_EDIT1, m_con);
	DDX_Text(pDX, IDC_EDIT1, m_s);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjShowFormu, CDialog)
	//{{AFX_MSG_MAP(CFxjShowFormu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CFxjShowFormu::OnCancel() 
{
	CDialog::OnCancel();
}

void CFxjShowFormu::OnOK() 
{

	int nb = 0,ne = 0;
	m_con.GetSel (nb,ne);
	if(!(nb>=0 && ne-nb>0))
	{
		m_con.SetSel (0,-1);
	}
	m_con.Copy ();
	CDialog::OnOK();
}

BOOL CFxjShowFormu::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_title);
	
	return TRUE;  
}
