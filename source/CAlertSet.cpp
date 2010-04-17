// CAlertSet.cpp : implementation file
// by 俞明录

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CAlertSet.h"
#include "CTaiScreen.h"
#include "windowsx.h"
#include "CTaiKlineFileHS.h"
#include "SelectStock.h"
#include "CAlertSetAdditional.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlertSet dialog


CAlertSet::CAlertSet(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CAlertSet::IDD, pParent)
{//应用程序类，主程序初始化，初始数据读取等
	m_nItem = -1;
	m_btAddtional[0] = 1;
	m_btAddtional[1] = 1;
	m_btAddtional[2] = 0;
	//{{AFX_DATA_INIT(CAlertSet)
	m_sTitle = _T("");
	//}}AFX_DATA_INIT
	m_pExplainButton = &m_buttonExpl;
	m_pStockSelectList = &m_liststockrange;
}


void CAlertSet::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertSet)
	DDX_Control(pDX, IDC_SETTING_ADDED, m_settingAdded);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonExpl);
	DDX_Control(pDX, IDC_STOCK_LIST, m_liststockrange);
	DDX_Text(pDX, IDC_EDIT1, m_sTitle);
	DDV_MaxChars(pDX, m_sTitle, 15);
	DDX_Control(pDX, IDC_ADD_STOCKS_PARENT, m_btAddS);
	DDX_Control(pDX, IDC_DELETE_STOCKS2, m_btDeleteS);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertSet, CTaiScreenParent)
	//{{AFX_MSG_MAP(CAlertSet)
	ON_BN_CLICKED(IDC_DELETE_STOCKS2, OnDeleteStocks2)
	ON_BN_CLICKED(IDC_ADD_STOCKS_PARENT, OnAddStocksParent)
	ON_BN_CLICKED(IDC_SETTING_ADDED, OnSettingAdded)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_NOTIFY(LVN_KEYDOWN, IDC_STOCK_LIST, OnKeydownStockList)
	ON_WM_HELPINFO()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FORMULAR_ALL, OnSelchangedTreeFormularAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlertSet message handlers

void CAlertSet::OnOK() 
{//Tidxd结构
 
	if(UpdateData() == FALSE)
		return;

	if(this->m_sTitle == "" )
	{
	
		int n = m_indexSaveArray.GetSize ();
		if(n>0)
		{
			{
				m_sTitle = m_indexSaveArray[0].sIndex_name ;
				UpdateData(FALSE) ;
			}
		}

	}
	for(int j = 0;j<CAlertSystem::m_gAlarmArray.GetCount ();j++)
	{
		if(m_nItem>=0 && j== m_nItem)
			continue;
		POSITION pos=CAlertSystem::m_gAlarmArray.FindIndex (j);
		ALERT_SET_ITEM* item = CAlertSystem::m_gAlarmArray.GetAt(pos);
		if(item->m_strAlarmName == m_sTitle)
		{
			AfxMessageBox("预警名重复，请重新设置！");
			return;
		}
	}


	if(this->m_stockchoose .GetSize ()<=0)
	{
		AfxMessageBox("请重新设置股票范围！");return;
	}
	if(this->m_indexSaveArray.GetSize ()<=0)
	{
		AfxMessageBox("请重新设置条件！");return;
	}

	CTaiScreenParent::OnOK();
}

void CAlertSet::OnCancel() 
{

	
	CTaiScreenParent::OnCancel();
}

void CAlertSet::OnDeleteStocks2() 
{

	DeleteStocks();
	CAlertSystem::ListCtrlToSymbolType(m_symbolTypeArray,&m_liststockrange);
	
	
}

void CAlertSet::OnAddStocksParent() 
{//Tidxd结构
	AddStocks();
	CAlertSystem::ListCtrlToSymbolType(m_symbolTypeArray,&m_liststockrange);
	
}

void CAlertSet::OnSettingAdded() 
{

	CAlertSetAdditional dlg;
	dlg.m_bSound = this->m_btAddtional [0];
	dlg.m_bMessageBox  = this->m_btAddtional [1];
	dlg.m_bContinueWatch  = this->m_btAddtional [2];
	if(dlg.DoModal () == IDOK)
	{
		this->m_btAddtional [0]	= dlg.m_bSound		;
		this->m_btAddtional [1]	= dlg.m_bMessageBox	;
		this->m_btAddtional [2]	= dlg.m_bContinueWatch;
	}
}

BOOL CAlertSet::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();
	m_btDeleteS.SetIcon(IDI_ICON_DELETE);
	m_btAddS.SetIcon(IDI_ICON_ADD2);
	m_btOK.SetIcon(IDI_ICON_OK);
	m_btCancel.SetIcon(IDI_EXIT16);
	
	


	this->m_treeFormularCtrl.InsertEqutionTreeOther(-7);

	this->m_treeFormularCtrl.InsertEqutionTreeOther(-8);

	this->m_treeFormularCtrl.InsertEqutionTree (0,true);

	this->m_treeFormularCtrl.InsertEqutionTree (1,true);

	this->m_treeFormularCtrl.InsertEqutionTree (2);
	HTREEITEM hh = m_treeFormularCtrl.GetRootItem( );
	m_treeFormularCtrl.EnsureVisible(hh);

	//////////////////////////////////////////////////
	m_liststockrange.SetImageList(&m_imageList,LVSIL_SMALL);
	FillToListCtrlHead(&m_liststockrange);


	AddIndexArrayToList();


	AddStockToList(&m_liststockrange);
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	ListCtrlSetSelectItem(&m_liststockrange);

	
	for(int i=0;i<8;i++)
      m_sSubFormular.AddString(m_subIndexGroup[i]);
    m_sSubFormular.SetCurSel(0);
	OnSelchangeListSubFormular();
	
	UpdateData(FALSE);
	return TRUE;  
}

void CAlertSet::SetInitData(ALERT_SET_ITEM &itemData)
{//对话框基类
	this->m_and_or = itemData.m_bAnd ;
	memcpy(this->m_btAddtional  , itemData.m_btAddtional ,4) ;
	this->m_sTitle  = itemData.m_strAlarmName  ;


	int n = itemData.m_conditionArray.GetSize ();
	for(int i=0;i<n;i++)
		m_indexSaveArray.Add (itemData.m_conditionArray[i]);


	n = itemData.m_symbolTypeArray .GetSize();
	for(int i=0;i<n;i++)
		m_symbolTypeArray.Add (itemData.m_symbolTypeArray[i]);
}

void CAlertSet::OnButton2() 
{

	ShowExplain(this->m_pIndex);
	
}

void CAlertSet::GetItemData(ALERT_SET_ITEM &itemData)
{
	itemData.m_bAnd = bool(this->m_and_or) ;
	memcpy(itemData.m_btAddtional  , this->m_btAddtional ,4) ;
	strcpy(itemData.m_strAlarmName  , this->m_sTitle ) ;

	
	int n = m_indexSaveArray.GetSize ();
	itemData.m_conditionArray.RemoveAll ();
	for(int i=0;i<n;i++)
		itemData.m_conditionArray.Add (m_indexSaveArray[i]);

	
	n = m_symbolTypeArray.GetSize();
	itemData.m_symbolTypeArray .RemoveAll ();
	for(int i=0;i<n;i++)
		itemData.m_symbolTypeArray.Add (m_symbolTypeArray[i]);


	n = this->m_stockchoose.GetSize();
	SYMBOL_ISALERTED symAlarmed;
	itemData.m_bAlarmedArray.RemoveAll();
	for(int i=0;i<n;i++)
	{
		strcpy(symAlarmed.m_sSymbol ,m_stockchoose[i].m_chSymbol);
		symAlarmed.m_btAlarmed = (BYTE)0;
		symAlarmed.m_stkKind  = m_stockchoose[i].m_nSymbolKind;

		itemData.m_bAlarmedArray .Add (symAlarmed);
	}


}

void CAlertSet::OnKeydownStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	
	*pResult = 0;
}

BOOL CAlertSet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);return TRUE;
	

}

void CAlertSet::NotifySubclass()
{//对话框基类

	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);

}

void CAlertSet::OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CTaiScreenParent::OnSelchangedTreeFormularAll( pNMHDR, pResult) ;

	if(this->m_pIndex )
	{
		this->m_sTitle = m_pIndex->name ;
		UpdateData(FALSE);
	}

	*pResult = 0;
}
