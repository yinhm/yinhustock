// DiaGetUpdate.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DiaGetUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDiaGetUpdate::CDiaGetUpdate(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaGetUpdate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaGetUpdate)
	m_days = 0;
	//}}AFX_DATA_INIT
}


void CDiaGetUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaGetUpdate)
	DDX_Text(pDX, IDC_EDIT1, m_days);
	DDV_MinMaxInt(pDX, m_days, 0, 300);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaGetUpdate, CDialog)
	//{{AFX_MSG_MAP(CDiaGetUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDiaGetUpdate::OnOK() 
{
    if(!UpdateData(TRUE))
		return;
	CDialog::OnOK();
}
