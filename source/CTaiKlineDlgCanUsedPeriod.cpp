// CTaiKlineDlgCanUsedPeriod.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgCanUsedPeriod.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgCanUsedPeriod dialog


CTaiKlineDlgCanUsedPeriod::CTaiKlineDlgCanUsedPeriod(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgCanUsedPeriod::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgCanUsedPeriod)
	m_bHs = FALSE;
	m_b1Min = FALSE;
	m_b5Min = FALSE;
	m_b15Min = FALSE;
	m_b30Min = FALSE;
	m_b60Min = FALSE;
	m_bDay = FALSE;
	m_bWeek = FALSE;
	m_bMonth = FALSE;
	m_bYear = FALSE;
	m_bMany = FALSE;
	//}}AFX_DATA_INIT
}


void CTaiKlineDlgCanUsedPeriod::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgCanUsedPeriod)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Check(pDX, IDC_CHECK1, m_bHs);
	DDX_Check(pDX, IDC_CHECK2, m_b1Min);
	DDX_Check(pDX, IDC_CHECK3, m_b5Min);
	DDX_Check(pDX, IDC_CHECK4, m_b15Min);
	DDX_Check(pDX, IDC_CHECK5, m_b30Min);
	DDX_Check(pDX, IDC_CHECK6, m_b60Min);
	DDX_Check(pDX, IDC_CHECK7, m_bDay);
	DDX_Check(pDX, IDC_CHECK8, m_bWeek);
	DDX_Check(pDX, IDC_CHECK9, m_bMonth);
	DDX_Check(pDX, IDC_CHECK10, m_bYear);
	DDX_Check(pDX, IDC_CHECK11, m_bMany);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgCanUsedPeriod, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgCanUsedPeriod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgCanUsedPeriod message handlers

void CTaiKlineDlgCanUsedPeriod::OutSetting(BYTE *pByte)
{
	pByte[0]=m_bHs ;
	pByte[1]=m_b1Min ;
	pByte[2]=m_b5Min ;
	pByte[3]=m_b15Min ;
	pByte[4]=m_b30Min ;
	pByte[5]=m_b60Min ;
	pByte[6]=m_bDay ;
	pByte[7]=m_bWeek ;
	pByte[8]=m_bMonth ;
	pByte[9]=m_bYear ;
	pByte[10]=m_bMany ;
}

void CTaiKlineDlgCanUsedPeriod::InputString(BYTE *pByte)
{
	m_bHs		=pByte[0] ;
	m_b1Min	=pByte[1] ;
	m_b5Min	=pByte[2] ;
	m_b15Min	=pByte[3] ;
	m_b30Min	=pByte[4] ;
	m_b60Min	=pByte[5] ;
	m_bDay	=pByte[6] 	;
	m_bWeek	=pByte[7] ;
	m_bMonth	=pByte[8] ;
	m_bYear	=pByte[9] ;
	m_bMany	=pByte[10];
}

void CTaiKlineDlgCanUsedPeriod::OnOK() 
{

	UpdateData();
	if(m_bHs	== FALSE &&
		m_b1Min	== FALSE &&
		m_b5Min	== FALSE &&
		m_b15Min== FALSE &&
		m_b30Min== FALSE &&
		m_b60Min== FALSE &&
		m_bDay	== FALSE &&
		m_bWeek	== FALSE &&
		m_bMonth== FALSE &&
		m_bYear	== FALSE &&
		m_bMany	== FALSE )
	{
		AfxMessageBox("数据无效，请重新设置数据！");
		return;
	}

	CDialog::OnOK();
}

BOOL CTaiKlineDlgCanUsedPeriod::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	SetWindowText("可用周期");
	
	return TRUE;  
}
