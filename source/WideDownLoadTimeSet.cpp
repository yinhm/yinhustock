// WideDownLoadTimeSet.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "WideDownLoadTimeSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownLoadNetTimeSet dialog


CDownLoadNetTimeSet::CDownLoadNetTimeSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadNetTimeSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownLoadNetTimeSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	time_t now; 
	time( &now );
	m_End = now;
	m_Start = now;
}
CDownLoadNetTimeSet::CDownLoadNetTimeSet(time_t StartTime,time_t EndTime,CWnd* pParent /*=NULL*/)
	: CDialog(CDownLoadNetTimeSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownLoadNetTimeSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_End = EndTime;
	m_Start = StartTime;

}


void CDownLoadNetTimeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownLoadNetTimeSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_DateTimeCtrl(pDX, IDC_START, m_Start);
	DDX_DateTimeCtrl(pDX, IDC_END, m_End);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownLoadNetTimeSet, CDialog)
	//{{AFX_MSG_MAP(CDownLoadNetTimeSet)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////

void CDownLoadNetTimeSet::OnOK()
{
	this->UpdateData(TRUE);
	if(this->m_Start.GetTime()>this->m_End.GetTime())
	{
		AfxMessageBox("请选择正确下载明细时间!");
		return ;
	}
	CDialog::OnOK();
}
BOOL CDownLoadNetTimeSet::OnInitDialog() 
{
	this->CenterWindow();
	this->UpdateData(FALSE);
	return TRUE;
}