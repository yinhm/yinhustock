// DialogProgressPingce.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DialogProgressPingce.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogProgressPingce::CDialogProgressPingce(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProgressPingce::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogProgressPingce)
	//}}AFX_DATA_INIT
	m_nLower=0;
	m_nUpper=100;
	m_nPos=0;
}


void CDialogProgressPingce::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProgressPingce)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogProgressPingce, CDialog)
	//{{AFX_MSG_MAP(CDialogProgressPingce)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDialogProgressPingce::SetRange(int nLower,int nUpper,BOOL bUpdate)
{
	if (nLower>0 || nUpper>0 || nUpper>=nLower)
	{
		m_nLower=nLower;
		m_nUpper=nUpper;
		if (bUpdate)
			m_Progress.SetRange(m_nLower,m_nUpper);
	}
}

void CDialogProgressPingce::SetPos(int nPos)
{
	if (nPos>=m_nLower && nPos<=m_nUpper)
	{
		m_nPos=nPos;
		m_Progress.SetPos(m_nPos);
	}
}

void CDialogProgressPingce::Close()
{
	OnOK();
}

BOOL CDialogProgressPingce::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Progress.SetRange(m_nLower,m_nUpper);
	m_Progress.SetPos(m_nPos);
	return TRUE;  
}

void CDialogProgressPingce::Complete()
{
	SetPos(m_nUpper);
}
