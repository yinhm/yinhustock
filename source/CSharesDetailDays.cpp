// CSharesDetailDays.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CSharesDetailDays.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSharesDetailDays dialog


CSharesDetailDays::CSharesDetailDays(CWnd* pParent /*=NULL*/)
	: CDialog(CSharesDetailDays::IDD, pParent)
{
	m_nLower=1;
	m_nUpper=3;
	m_bIncludeToday=FALSE;
	//{{AFX_DATA_INIT(CSharesDetailDays)
	m_nDays = 1;
	//}}AFX_DATA_INIT
}


void CSharesDetailDays::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSharesDetailDays)
	DDX_Control(pDX, IDC_SPIN1, m_spinDay);
	DDX_Text(pDX, IDC_EDIT1, m_nDays);
	DDV_MinMaxUInt(pDX, m_nDays, 1, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSharesDetailDays, CDialog)
	
END_MESSAGE_MAP()


BOOL CSharesDetailDays::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_spinDay.SetRange(m_nLower,m_nUpper);
	if (m_nDays==-1)
		m_spinDay.SetPos(m_nLower);
	else
		m_spinDay.SetPos(m_nDays);
	m_spinDay.SetBuddy(GetDlgItem(IDC_EDIT1));

	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK1);
	if (m_bIncludeToday)
		pCheck->SetCheck(1);
	else
		pCheck->SetCheck(0);
	pCheck->SetWindowText(_T("°üÀ¨½ñÌì"));
	return TRUE;  
}

void CSharesDetailDays::OnOK() 
{
	if (!UpdateData(TRUE))
		return;

	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK1);
	if (pCheck->GetCheck()==0)
		m_bIncludeToday=FALSE;
	else
		m_bIncludeToday=TRUE;
	
	CDialog::OnOK();
}

void CSharesDetailDays::SetRange(int nLower,int nUpper)
{
	if (nLower>nUpper)
	{
		m_nLower=1;
		m_nUpper=3;
	}
	else
	{
		m_nLower=nLower;
		m_nUpper=nUpper;
	}
}

void CSharesDetailDays::SetDays(int nDays)
{
	if (nDays<m_nLower)
		m_nDays=m_nLower;
	else if(nDays>m_nUpper)
		m_nDays=m_nUpper;
	else
		m_nDays=nDays;
}

void CSharesDetailDays::SetToday(BOOL bInclude)
{
	m_bIncludeToday=bInclude;
}

int CSharesDetailDays::GetDays()
{
	return m_nDays;
}

BOOL CSharesDetailDays::GetToday()
{
	return m_bIncludeToday;
}
