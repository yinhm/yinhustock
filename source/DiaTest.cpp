// DiaTest.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DiaTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDiaTest::CDiaTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaTest)
	m_edit1 = _T("");
	m_edit2 = _T("");
	m_edit3 = _T("");
	//}}AFX_DATA_INIT
	this->pDoc = NULL;
}


void CDiaTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaTest)
	DDX_Text(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT7, m_edit2);
	DDX_Text(pDX, IDC_EDIT8, m_edit3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaTest, CDialog)
	//{{AFX_MSG_MAP(CDiaTest)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDiaTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetTimer(2,1000,NULL);
	return TRUE;            
}

void CDiaTest::OnTimer(UINT nIDEvent) 
{
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
