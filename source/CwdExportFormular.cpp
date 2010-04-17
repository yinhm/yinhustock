// CwdExportFormular.cpp : implementation file
// by ÓáÃ÷Â¼

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CwdExportFormular.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFxjExportFormular dialog


CFxjExportFormular::CFxjExportFormular(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CFxjExportFormular::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFxjExportFormular)
	m_sBrief = _T("");
	m_sExplain = _T("");
	m_sFormular = _T("");
	m_sPassword = _T("");
	m_bSetPassword = FALSE;
	m_bCompleteSecret = FALSE;
	//}}AFX_DATA_INIT
}


void CFxjExportFormular::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjExportFormular)
	DDX_Text(pDX, IDC_EDIT2, m_sBrief);
	DDX_Text(pDX, IDC_EDIT3, m_sExplain);
	DDX_Text(pDX, IDC_EDIT4, m_sFormular);
	DDX_Text(pDX, IDC_EDIT1, m_sPassword);
	DDX_Check(pDX, IDC_CHECK1, m_bSetPassword);
	DDX_Check(pDX, IDC_CHECK2, m_bCompleteSecret);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjExportFormular, CTaiScreenParent)
	//{{AFX_MSG_MAP(CFxjExportFormular)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FORMULAR_ALL, OnSelchangedTreeFormularAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CFxjExportFormular::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();
	
	this->m_treeFormularCtrl.InsertEqutionTree (0,true);

	this->m_treeFormularCtrl.InsertEqutionTree (1,true);

	this->m_treeFormularCtrl.InsertEqutionTree (2);
	HTREEITEM hh = m_treeFormularCtrl.GetRootItem( );
	m_treeFormularCtrl.EnsureVisible(hh);

	
	return TRUE;  
}

void CFxjExportFormular::OnCancel() 
{
	
	CTaiScreenParent::OnCancel();
}

void CFxjExportFormular::OnOK() 
{

	if(!UpdateData())
		return;
	this->m_treeFormularCtrl .ExportFormular(this->m_bCompleteSecret , this->m_bSetPassword ,this->m_sPassword );
	CTaiScreenParent::OnOK();
}

void CFxjExportFormular::OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CTaiScreenParent::OnSelchangedTreeFormularAll(pNMHDR, pResult) 	;
	GetDlgItem(IDC_LIST_SUB_FORMULAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(SW_HIDE);//
	GetDlgItem(IDC_RICHEDIT1)->ShowWindow(SW_HIDE);//
	
	if(GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)) GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_COMBO_PERIOD)) GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_STATIC_COND_NEED)) GetDlgItem(IDC_STATIC_COND_NEED)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_STATIC_P1)) GetDlgItem(IDC_STATIC_P1)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_STATIC_P2)) GetDlgItem(IDC_STATIC_P2)->ShowWindow(SW_HIDE);//

	if(this->m_pIndex )
	{
		this->m_sBrief = m_pIndex->explainBrief ;
		this->m_sExplain  = m_pIndex->help  ;
		m_sFormular = m_pIndex->GetFormular();
		UpdateData(FALSE);
	}
	*pResult = 0;
}
