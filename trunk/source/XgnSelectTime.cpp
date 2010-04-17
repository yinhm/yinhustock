// XgnSelectTime.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "XgnSelectTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// XgnSelectTime dialog


XgnSelectTime::XgnSelectTime(CWnd* pParent /*=NULL*/)
	: CDialog(XgnSelectTime::IDD, pParent)
{
	//{{AFX_DATA_INIT(XgnSelectTime)
	m_tmSelect = 0;
	//}}AFX_DATA_INIT
}


void XgnSelectTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XgnSelectTime)
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_conTm);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tmSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XgnSelectTime, CDialog)
	//{{AFX_MSG_MAP(XgnSelectTime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XgnSelectTime message handlers

BOOL XgnSelectTime::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_tmSelect = CTime::GetCurrentTime ();
	UpdateData(FALSE);

	CTime tm(2001,1,1,8,0,0);
	CTime tm2(2037,1,1,8,0,0);
	m_conTm.SetRange(&tm,&tm2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void XgnSelectTime::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
