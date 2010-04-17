// CTaiKlineSetMaxMin.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineSetMaxMin.h"
#include "MainFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CTaiKlineSetMaxMin::CTaiKlineSetMaxMin(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineSetMaxMin::IDD, pParent)
{
	pView = (CTaiShanKlineShowView* )pParent;
	ASSERT(pView!=NULL);
	//{{AFX_DATA_INIT(CTaiKlineSetMaxMin)
	m_fMax1 = 0.0f;
	m_fMin1 = 0.0f;
	m_fMax2 = 0.0f;
	m_fMin2 = 0.0f;
	m_fMax3 = 0.0f;
	m_fMin3 = 0.0f;
	m_fMax4 = 0.0f;
	m_fMin4 = 0.0f;
	m_fMax5 = 0.0f;
	m_fMin5 = 0.0f;
	//}}AFX_DATA_INIT
}


void CTaiKlineSetMaxMin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineSetMaxMin)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT16, m_conMin5);
	DDX_Control(pDX, IDC_EDIT10, m_conMax5);
	DDX_Control(pDX, IDC_EDIT9, m_conMin4);
	DDX_Control(pDX, IDC_EDIT8, m_conMax4);
	DDX_Control(pDX, IDC_EDIT7, m_conMin3);
	DDX_Control(pDX, IDC_EDIT5, m_conMax3);
	DDX_Control(pDX, IDC_EDIT4, m_conMin2);
	DDX_Control(pDX, IDC_EDIT3, m_conMax2);
	DDX_Control(pDX, IDC_EDIT2, m_conMin1);
	DDX_Control(pDX, IDC_EDIT1, m_conMax1);
	DDX_Text(pDX, IDC_EDIT1, m_fMax1);
	DDX_Text(pDX, IDC_EDIT2, m_fMin1);
	DDX_Text(pDX, IDC_EDIT3, m_fMax2);
	DDX_Text(pDX, IDC_EDIT4, m_fMin2);
	DDX_Text(pDX, IDC_EDIT5, m_fMax3);
	DDX_Text(pDX, IDC_EDIT7, m_fMin3);
	DDX_Text(pDX, IDC_EDIT8, m_fMax4);
	DDX_Text(pDX, IDC_EDIT9, m_fMin4);
	DDX_Text(pDX, IDC_EDIT10, m_fMax5);
	DDX_Text(pDX, IDC_EDIT16, m_fMin5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineSetMaxMin, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineSetMaxMin)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTaiKlineSetMaxMin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	int nm=pView->m_infoInit.nCountIndex;

	switch(nm)
	{
	case 1:
		m_conMax2.EnableWindow (FALSE);
		m_conMin2.EnableWindow (FALSE);
	case 2:
		m_conMax3.EnableWindow (FALSE);
		m_conMin3.EnableWindow (FALSE);
	case 3:
		m_conMax4.EnableWindow (FALSE);
		m_conMin4.EnableWindow (FALSE);
	case 4:
		m_conMax5.EnableWindow (FALSE);
		m_conMin5.EnableWindow (FALSE);
		break;
	}

	switch(nm)
	{
	case 5:
		m_fMax5 = pView->pKlineDrawing ->m_max_sun [4];
		m_fMin5 = pView->pKlineDrawing ->m_min_sun [4];
	case 4:
		m_fMax4 = pView->pKlineDrawing ->m_max_sun [3];
		m_fMin4 = pView->pKlineDrawing ->m_min_sun [3];
	case 3:
		m_fMax3 = pView->pKlineDrawing ->m_max_sun [2];
		m_fMin3 = pView->pKlineDrawing ->m_min_sun [2];
	case 2:
		m_fMax2 = pView->pKlineDrawing ->m_max_sun [1];
		m_fMin2 = pView->pKlineDrawing ->m_min_sun [1];
	case 1:
		m_fMax1 = pView->pKlineDrawing ->m_max_sun [0];
		m_fMin1 = pView->pKlineDrawing ->m_min_sun [0];
		break;
	}
	
	UpdateData(FALSE);
	return TRUE;  
}

void CTaiKlineSetMaxMin::OnOK() 
{

	UpdateData(TRUE);
	
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	int nm=pView->m_infoInit.nCountIndex;
	switch(nm)
	{
	case 5:
		pView->pKlineDrawing ->m_max_sun [4] = m_fMax5;
		pView->pKlineDrawing ->m_min_sun [4] = m_fMin5;
	case 4:
		pView->pKlineDrawing ->m_max_sun [3] = m_fMax4;
		pView->pKlineDrawing ->m_min_sun [3] = m_fMin4;
	case 3:
		pView->pKlineDrawing ->m_max_sun [2] = m_fMax3;
		pView->pKlineDrawing ->m_min_sun [2] = m_fMin3;
	case 2:
		pView->pKlineDrawing ->m_max_sun [1] = m_fMax2;
		pView->pKlineDrawing ->m_min_sun [1] = m_fMin2;
	case 1:
		pView->pKlineDrawing ->m_max_sun [0] = m_fMax1;
		pView->pKlineDrawing ->m_min_sun [0] = m_fMin1;
		break;
	}

	CDialog::OnOK();
}

BOOL CTaiKlineSetMaxMin::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	DoHtmlHelp(this);return TRUE;
}
