// DialogCwdLeftPop.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DialogCwdLeftPop.h"
#include "CTaiScreenParent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OEM_LT


CDialogCwdLeftPop::CDialogCwdLeftPop(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCwdLeftPop::IDD, pParent)
{
	m_pIndex = NULL;  
	nKind = -1;        
	m_hItem = NULL;

	m_pParent = pParent;
	//{{AFX_DATA_INIT(CDialogCwdLeftPop)
		
	//}}AFX_DATA_INIT
}


void CDialogCwdLeftPop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCwdLeftPop)
	DDX_Control(pDX, IDC_TREE1, m_treeFormularCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCwdLeftPop, CDialog)
	//{{AFX_MSG_MAP(CDialogCwdLeftPop)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTreeFormularAll)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDialogCwdLeftPop::OnCancel() 
{

	

}

void CDialogCwdLeftPop::OnOK() 
{

	
	CDialog::OnOK();
}

BOOL CDialogCwdLeftPop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_treeFormularCtrl.m_bRefreshEquation   = true;
	m_treeFormularCtrl.m_bNewGroup			=true;
	m_treeFormularCtrl.m_bDeleteGroup		=true;
	m_treeFormularCtrl.m_bModifyGroupName	=true;
	m_treeFormularCtrl.m_bDeleteEquation	=true;
	m_treeFormularCtrl.m_bNewEquation		=true;
#ifdef OEM
	m_treeFormularCtrl.m_bModifyEquation	=false;
#endif
#ifdef OEM_LT
	m_treeFormularCtrl.m_bNewEquation		=false;
	m_treeFormularCtrl.m_bModifyEquation	=false;
#endif
	
	m_treeFormularCtrl.m_bScreenStock 	=true;
	m_treeFormularCtrl.m_bShowIndicator =true;
	m_treeFormularCtrl.m_bScreenStockIndicator 	=true;
	m_treeFormularCtrl.m_bShowKlineGroup =true;

	return TRUE;  
}

void CDialogCwdLeftPop::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	

	CRect r2;
	this->GetClientRect (r2);
	r2.DeflateRect (1,1);
	if(m_treeFormularCtrl.m_hWnd )
		this->m_treeFormularCtrl .MoveWindow (r2);

	
}

void CDialogCwdLeftPop::OnClose() 
{
	
}

void CDialogCwdLeftPop::OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	HTREEITEM hItem = this->m_treeFormularCtrl.GetSelectedItem ();
	if(hItem == NULL)
	{
	  return;
	}
	if ((hItem != NULL) )
	{
	   this->m_treeFormularCtrl .SelectItem(hItem);
	}
    m_hItem = hItem;
	int nKindRoot,nKindItem;
	m_pIndex = NULL;
	bool bSucc = m_treeFormularCtrl.GetCurrentItemInfo (nKindRoot,nKindItem,m_pIndex);	

	int i = 0,j=0;
	CString temp;
	this->nKind = nKindRoot;
	if(nKind == -7 || nKind == -8)
		m_pIndex = CTaiScreenParent::GetSpecialLJishu(-nKind-7);


	*pResult = 0;
}

void CDialogCwdLeftPop::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_treeFormularCtrl.CommandShowIndicator();

	*pResult = 0;
}

void CDialogCwdLeftPop::InsertData()
{
	
	this->m_treeFormularCtrl.InsertEqutionTree (0,true);

	this->m_treeFormularCtrl.InsertEqutionTree (1,true);

	this->m_treeFormularCtrl.InsertEqutionTree (2);


}

void CDialogCwdLeftPop::RefreshJishuTreeCtrl()
{
	m_treeFormularCtrl.DeleteAllItems();


	this->m_treeFormularCtrl.InsertEqutionTree (0);

	this->m_treeFormularCtrl.InsertEqutionTree (1);
	
	this->m_treeFormularCtrl.InsertEqutionTree (2);

	this->m_pIndex = NULL;
}

LRESULT CDialogCwdLeftPop::OnNcHitTest(CPoint point) 
{

	UINT un = CDialog::OnNcHitTest(point);
	if(un!= HTRIGHT)
		un = HTNOWHERE ;
	return un;
}
