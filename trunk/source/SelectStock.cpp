// SelectStock.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"
#include "SelectStock.h"
#include "BGridCtrl.h"
#include "keybrodeangel.h"
#include "SetParam.h"
#include "CSharesBigBillDlg.h"
#include "CSharesInformation.h"
#include "CAlertSystem.h"
#include "ManageInvest.h"
#include "EditAccount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SelectStock dialog
#define WM_USER_ADDSTOCK WM_USER+1200

SelectStock::SelectStock(CWnd* pParent /*=NULL*/,BOOL bOnlyAddStock,bool bScreenStockBlock,BOOL bCloseWindows)
	: CDialog(SelectStock::IDD, pParent)
{
	//{{AFX_DATA_INIT(SelectStock)
	m_nRadio = 0;
	//}}AFX_DATA_INIT
	m_bOnlyAddStock=bOnlyAddStock;
	m_bCloseWindows=bCloseWindows;
//	if(m_bOnlyAddStock)
//	   m_nRadio = 1;
	m_bScreenStockBlock = bScreenStockBlock;
	m_pSelectResultList = NULL;
	m_nCurTab = 0;
	m_bSingleSelect = false;
	m_stkKind = 0;

}

SelectStock::SelectStock(UINT IDDLG, CWnd* pParent ,BOOL bOnlyAddStock,bool bScreenStockBlock,BOOL bCloseWindows)
	: CDialog(IDDLG, pParent)
{
	m_nRadio = 0;
	m_bOnlyAddStock=bOnlyAddStock;
	m_bCloseWindows=bCloseWindows;
//	if(m_bOnlyAddStock)
//	   m_nRadio = 1;
	m_bScreenStockBlock = bScreenStockBlock;
	m_pSelectResultList = NULL;
	m_nCurTab = 0;
	m_bSingleSelect = false;
	m_stkKind = 0;

}



void SelectStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectStock)
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_LIST2, m_StockList);
	DDX_Control(pDX, IDC_LIST1, m_StockTypeList);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectStock, CDialog)
	//{{AFX_MSG_MAP(SelectStock)
	ON_NOTIFY(NM_CLICK, IDC_TAB_TYPE, OnClickTab)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SelectStock message handlers

BOOL SelectStock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;

	m_pImageListSmall = new CImageList();
	ASSERT( m_pImageListSmall != NULL);    
	m_pImageListSmall->Create(IDB_BITMAP12,16,0,RGB(192,192,192));

   	CString         strItem= _T("StockType");
	this->m_StockTypeList.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	this->m_StockTypeList.InsertColumn(0, strItem);
	m_StockTypeList.SetColumnWidth( 0, 80 );

    InsertStockTypeToListExt(0);
	InsertStockColumnToList();
	if(	m_bOnlyAddStock)
	{
		CWnd *pWnd=this->GetDlgItem(IDC_RADIO1);
		pWnd->EnableWindow(FALSE);
	}


	TC_ITEM tci;
	CString str;
	tci.mask=TCIF_TEXT;

	str="所有个股";
	tci.pszText=(LPSTR)(LPCTSTR)str;
	tci.cchTextMax=str.GetLength();
	((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->InsertItem(0,&tci);

	if(m_bOnlyAddStock == FALSE)
	{
		str="常用板块";
		tci.pszText=(LPSTR)(LPCTSTR)str;
		tci.cchTextMax=str.GetLength();
		((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->InsertItem(1,&tci);
	}

	if(m_bScreenStockBlock == true)
	{
		str="选股结果";
		tci.pszText=(LPSTR)(LPCTSTR)str;
		tci.cchTextMax=str.GetLength();
		((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->InsertItem(2,&tci);
	}

	if(m_bScreenStockBlock == true)
	{
	}

	if(m_bCloseWindows == TRUE)
	{
		GetDlgItem(IDC_ADD)->ShowWindow(SW_HIDE);

		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
	}

	((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->SetCurSel(m_nCurTab);
	if(m_nCurTab == 0)
	{
		NMHDR pNMHDR;
		int result;
		m_StockTypeList.SetItemState(1,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
		OnClickList1(&pNMHDR, (LRESULT* )&result);
	}

	m_pImageListSmall->Detach ();
	delete m_pImageListSmall;

	if(m_bSingleSelect == true)
	{
		m_StockList.SetExtendedStyle(m_StockList.GetExtendedStyle()|LVS_EX_HEADERDRAGDROP);
	}
	return TRUE; 
}
BOOL SelectStock::InsertStockTypeToList() 
{
   	CRect           rect;
	int             iItem;
	LV_ITEM         lvitem;
	LPTSTR          pStrTemp;

    iItem=0;

	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SHZS;
	CString strIconShortDesc="上证指数";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 


	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SHAG;
	strIconShortDesc="上证Ａ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SHBG;
	strIconShortDesc="上证Ｂ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SHZQ;
	strIconShortDesc="上证债券";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SHJIJIN;
	strIconShortDesc="上证基金";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SZZS;
	strIconShortDesc="深证指数";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 


	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SZAG;
	strIconShortDesc="深证Ａ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SZBG;
	strIconShortDesc="深证Ｂ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SZZQ;
	strIconShortDesc="深证债券";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 
	
	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=SZJIJIN;
	strIconShortDesc="深圳基金";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=STKTYPE;
	strIconShortDesc="板块指数";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 


	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=CHOOSESTK;
	strIconShortDesc="自选股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	m_StockTypeNameArray.RemoveAll ();
    m_pDoc->m_ManagerStockTypeData.GetAllStockTypeName(m_StockTypeNameArray);
    int l_nCount=m_StockTypeNameArray.GetSize();
    for(int i=0;i<l_nCount;i++)
	{
		iItem++;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
		lvitem.iItem =  iItem ;
		lvitem.iSubItem = 0;
		lvitem.lParam=(long )i+1000;
		strIconShortDesc=m_StockTypeNameArray.GetAt(i);
		pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		lvitem.pszText = pStrTemp;
		lvitem.iImage = 0;
		m_StockTypeList.InsertItem(&lvitem); 
	}
	return TRUE;
}


void SelectStock::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	int nCurTab=((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->GetCurSel();
    if(nCurTab!=0)
		return;

	m_StockList.DeleteAllItems();
	m_StockTypeList.SetSelectionMark(-1 );
	int temp=m_StockTypeList.GetNextItem( -1, LVNI_SELECTED) ;
	if(temp==-1)
		return;
    int nParam=m_StockTypeList.GetItemData(temp); 
	if(nParam==CHOOSESTK)
	{
		SymbolKindArr l_StockArray;
		m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
		int DisplayCount=l_StockArray.GetSize();
		for(int row=0 ;row <  (int)DisplayCount  ;row++)         
		{
			 CReportData *Cdat;
			 SymbolKind symbolkind=l_StockArray.GetAt(row);
	 		 CString l_strCode=symbolkind.m_chSymbol;
			 int nKind=symbolkind.m_nSymbolKind;
			 if (m_pDoc->m_sharesInformation.Lookup(l_strCode.GetBuffer(0) , Cdat,nKind) == TRUE)     //检测该股票是否已记录在内存模板中
			 {
                 InsertStockToList(Cdat->name,Cdat->id,nKind);
			 }
		}
	}
	else if(nParam<CHOOSESTK)
	{
		int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(nParam);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			m_pDoc->m_sharesInformation.GetStockItem(nParam,j,Cdat);
			if(Cdat==NULL)
				continue;
            InsertStockToList(Cdat->name,Cdat->id,Cdat->kind );
		}
	}
	else 
	{
		CString l_sStockTypeName=this->m_StockTypeNameArray.GetAt(nParam-1000);
        SymbolKindArr l_StockCodeArray;
	    m_pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
        
	    int DisplayCount=l_StockCodeArray.GetSize();
		for(int row=0 ;row <  (int)DisplayCount  ;row++)          
		{    
			 CReportData *Cdat;
			 CString m_zqdm; 
			 SymbolKind symbolkind;
			 symbolkind=l_StockCodeArray.GetAt(row) ;
			 m_zqdm=symbolkind.m_chSymbol;
			 int nKind=symbolkind.m_nSymbolKind;
	 		 if (m_pDoc->m_sharesInformation.Lookup(m_zqdm.GetBuffer(0) , Cdat,nKind) == TRUE)     //检测该股票是否已记录在内存模板中
			 {
                InsertStockToList(Cdat->name,Cdat->id,nKind);
			 }
		}
	}
}
BOOL SelectStock::InsertStockColumnToList()
{
   	CRect           rect;
	CString         strItem1= _T("name");
	CString         strItem2= _T("code");
    
	this->m_StockList.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	
	this->m_StockList.InsertColumn(0, strItem1);
	m_StockList.SetColumnWidth( 0, 100 );
	this->m_StockList.InsertColumn(1, strItem2);
	return TRUE;
}
BOOL SelectStock::InsertStockToList(char *name,char *code,int nKind)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LPTSTR   pStrTemp1, pStrTemp2;

	iItem=this->m_StockList.GetItemCount();
	for (iSubItem = 0; iSubItem <2; iSubItem++)
	{
		if (iSubItem == 0)
			iIcon = 1; 

		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM  : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;
		
	
		CString strIconDesc, strIconShortDesc;
        strIconShortDesc=name;
	    strIconDesc=code;
		pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		pStrTemp2= strIconDesc.GetBuffer(strIconDesc.GetLength());
		lvitem.pszText = iSubItem == 0? pStrTemp1 : pStrTemp2;
		lvitem.lParam=nKind;
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_StockList.InsertItem(&lvitem); 
		else
			this->m_StockList.SetItem(&lvitem); 
	} 
	return TRUE;
}

void SelectStock::OnRadio1() 
{
	
	m_StockList.DeleteAllItems();

	this->UpdateData(TRUE);
	m_StockTypeList.SetSelectionMark(-1 );


}

void SelectStock::OnRadio2() 
{
	
	this->UpdateData(TRUE);
	m_StockTypeList.SetSelectionMark(0 );
}

void SelectStock::OnAdd() 
{

	SHARES_DATA_ADD *pAddMxtj;
	HGLOBAL hMem;
	LPVOID  hp;
	int nCurTab=((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->GetCurSel();
	if(nCurTab>0)
	{
		int listcount=m_StockTypeList.GetSelectedCount( ) ;
		hMem = GlobalAlloc( GPTR, ( listcount)* sizeof( SHARES_DATA_ADD) );
	    hp=GlobalLock(hMem);
		if( hp )
		{
			pAddMxtj= (SHARES_DATA_ADD *)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return;
		}
        int temp=m_StockTypeList.GetNextItem( -1, LVNI_SELECTED) ;
		int index=0;
        while(temp>=0)
		{
           pAddMxtj[index].StockType=m_StockTypeList.GetItemData(temp);
		   m_StockTypeList.GetItemText( temp, 0, pAddMxtj[index].name, 10);
           temp=m_StockTypeList.GetNextItem( temp, LVNI_SELECTED) ;
		   index++;
		}
		CWnd *pWnd=this->GetParent();

		if(m_pSelectResultList == NULL)
			::SendMessage(pWnd->m_hWnd,WM_USER_ADDSTOCK,(long )pAddMxtj,listcount);
		else
		{
			for(int i=0;i<listcount;i++)
			{
				FillToListCtrlItem(m_pSelectResultList,pAddMxtj[i].name, pAddMxtj[i].StockType );
			}
		}
		m_StockTypeList.SetSelectionMark(-1 );
	}
	else
	{
		int listcount=m_StockList.GetSelectedCount( ) ;
		hMem = GlobalAlloc( GPTR, ( listcount)* sizeof( SHARES_DATA_ADD) );
	    hp=GlobalLock(hMem);
		if( hp )
		{
			pAddMxtj= (SHARES_DATA_ADD *)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return;
		}
        int temp=m_StockList.GetNextItem( -1, LVNI_SELECTED) ;
		int nKind;
		int index=0;
        while(temp>=0)
		{
		   nKind=m_StockList.GetItemData(temp);
           pAddMxtj[index].StockType=-1-nKind;
		   m_StockList.GetItemText( temp, 1, pAddMxtj[index].name, 10);
           temp=m_StockList.GetNextItem( temp, LVNI_SELECTED) ;
		   index++;
		}
		CWnd *pWnd=this->GetParent();

		if(m_pSelectResultList == NULL)
			::SendMessage(pWnd->m_hWnd,WM_USER_ADDSTOCK,(long )pAddMxtj,listcount);
		else
		{
			for(int i=0;i<listcount;i++)
			{
				FillToListCtrlItem(m_pSelectResultList,pAddMxtj[i].name, pAddMxtj[i].StockType );
			}
		}
		m_StockList.SetSelectionMark(-1 );
	}
	
	if(pAddMxtj)
	{
	   GlobalUnlock(pAddMxtj);        
	   GlobalFree(pAddMxtj);
	}
}


void SelectStock::OnClickTab(NMHDR* pNMHDR, LRESULT* pResult) 
{

	int nCurTab=((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->GetCurSel();
	CWnd* pWnd1=GetDlgItem(IDC_LIST1);
	CWnd* pWnd2=GetDlgItem(IDC_LIST2);

	CRect rect,rect2;
	pWnd1->GetWindowRect(&rect);
	ScreenToClient(rect);
	pWnd2->GetWindowRect(&rect2);
	ScreenToClient(rect2);

	switch(nCurTab)
	{
	case 0:
		pWnd2->ShowWindow(SW_SHOW);
		pWnd1->MoveWindow(rect.left,rect.top,rect2.left-5-rect.left, rect.Height() );
		break;
	default:
		pWnd2->ShowWindow(SW_HIDE);
		pWnd1->MoveWindow(rect.left,rect.top,rect2.right-rect.left, rect.Height() );
		break;
	}
	if(nCurTab >=0 && nCurTab<=3)
	{
		InsertStockTypeToListExt(nCurTab);
	}
	*pResult = 0;
}

BOOL SelectStock::InsertStockTypeToListExt(int nKind)
{
	m_StockTypeList.DeleteAllItems( );

	if(nKind<2)
		InsertStockTypeToList();
	else if( nKind == 2 )
	{
		POSITION position=m_pDoc->m_Tjxgblocklist.GetHeadPosition();
		int i=0;
   		CRect           rect;
		int             iItem;
		LV_ITEM         lvitem;
		LPTSTR          pStrTemp;

		lvitem.lParam=3000;
		char ch2[10] = "当前页";
		pStrTemp= ch2; 

		lvitem.pszText = pStrTemp;
		lvitem.iImage = 0;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
		lvitem.iItem =  0 ;
		lvitem.iSubItem = 0;
		lvitem.iImage = 0;
		m_StockTypeList.InsertItem(&lvitem); 

		while(position)
		{
			BLOCKSTOCK * blocknow=m_pDoc->m_Tjxgblocklist.GetNext(position);

			CString sBlock = blocknow->blockname;

			iItem=this->m_StockTypeList.GetItemCount();

			lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
			lvitem.iItem =  iItem ;
			lvitem.iSubItem = 0;
			lvitem.lParam=2000+i;
			lvitem.iImage = 0;
			i++;
			CString strIconShortDesc=sBlock;
			pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
			lvitem.pszText = pStrTemp;
			m_StockTypeList.InsertItem(&lvitem); 

		}
	}
	else 
	{
		CEditAccount EditAccount;
		EditAccount.m_Operate=2;
		SymbolKindArr SymbolArray;
		CString AccountName,Password;
		if(EditAccount.DoModal()==IDOK)
		{
			EditAccount.m_szName;
			AccountName=EditAccount.m_szName+".acc";
			Password=EditAccount.m_szPassword;
			CSuperviseInvest::AlarmCallInvest(SymbolArray,AccountName,Password);
		}

		LV_ITEM   lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
		lvitem.iSubItem = 0;
		lvitem.iImage = 0;

		for( int i=0;i<SymbolArray.GetSize();i++)
		{
			lvitem.iItem =  i;
			SymbolKind symbolkind=SymbolArray[i];
			CString strIconShortDesc=symbolkind.m_chSymbol;
			int nKind=symbolkind.m_nSymbolKind;
			LPTSTR  pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
			lvitem.pszText = pStrTemp;
			lvitem.lParam=-1-nKind;
			m_StockTypeList.InsertItem(&lvitem); 

		}
	}
	return TRUE;

}

void SelectStock::FillToListCtrlItem(CListCtrl *pList, char *Id, int nParam)
{
	CTaiShanDoc *m_pDoc = CMainFrame::m_taiShanDoc ;
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LPTSTR   pStrTemp1, pStrTemp2;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=Id;
	
	int StockType = 0;
	if(nParam <= -1)
		StockType = 1;
	if(nParam >= 2000)
		StockType = 2;

    if(pList->FindItem(&lvf)>=0)
		return;
    iItem= pList->GetItemCount();
	for (iSubItem = 0; iSubItem <2; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  

		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

	
		CString strIconDesc, strIconShortDesc;
        strIconShortDesc=Id;
		iIcon = 1; 
		if(StockType==1)
		   strIconDesc="股票";
		else if(StockType==0)
		{
			iIcon=0;
		   strIconDesc="板块";
		}
		else
		{
			iIcon=0;
		   strIconDesc="选股";
		}

		pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		pStrTemp2= strIconDesc.GetBuffer(strIconDesc.GetLength());
		lvitem.pszText = iSubItem == 0? pStrTemp1 : pStrTemp2;

		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem = pList->InsertItem(&lvitem); 
		else
			 pList->SetItem(&lvitem); 
	} 
	 pList->SetItemData( iItem, nParam );
}

void SelectStock::GetStockSymbolArray(CListCtrl *pList, SymbolKindArr &sSymbolArray)
{
	CArrayStockType typeArr;
	CTaiShanDoc *m_pDoc = CMainFrame::m_taiShanDoc ;
	int n = pList->GetItemCount();
	CString s;
	SharesNameArr3 stockInfo;
	for(int i=0;i<n;i++)
	{
		s = pList->GetItemText (i,0);
		strcpy(stockInfo.m_sSymbol ,s);
		stockInfo.m_nType = pList->GetItemData (i);
		typeArr.Add(stockInfo);
	}

	CAlertSystem::GetSymbolArray(typeArr, sSymbolArray);
}
/**/

void SelectStock::DeleteSelectedItem(CListCtrl *pList)
{
    CString Id;
	int temp=pList->GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
		return;
	while(temp>=0)
	{
		pList->DeleteItem(temp);
        temp=pList->GetNextItem( -1,LVNI_SELECTED   ) ;
	}
}
BOOL SelectStock::PreTranslateMessage(MSG* pMsg)
{

	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
			int nCurTab=((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->GetCurSel();
			if(nCurTab!=0)
				return CDialog::PreTranslateMessage(pMsg);
			
		    CTaiKeyBoardAngelDlg m_diakeybrode;
			if(pMsg->wParam>='a'&&pMsg->wParam<='z')
				pMsg->wParam-=32;
			char input=pMsg->wParam;
			m_diakeybrode.input=input;
			if(pMsg->wParam>='0'&&pMsg->wParam<='9')
				m_diakeybrode.ischar=false;
			else
				m_diakeybrode.ischar=true;

		m_diakeybrode.DoModal();
		if(m_diakeybrode.isresultok)
		{
		   char result[10];            
           strcpy(result,m_diakeybrode.result);
	   
		   if(strlen(result)==6||strlen(result)==4)
		   {
				LVFINDINFO  lvf;
				lvf.flags=LVFI_STRING;
				CReportData *pCdat;
				int l_pos;
				m_pDoc->m_sharesInformation.Lookup(result,pCdat,m_diakeybrode.m_stkKind );
				if(pCdat==NULL)
					return TRUE;
				lvf.psz=pCdat->name;
				if((l_pos=m_StockList.FindItem(&lvf))>=0)
				{
                   LVITEM lv;
				   lv.mask=LVIF_STATE;
				   lv.iItem=l_pos;
				   lv.state=LVIS_SELECTED;
                   m_StockList.SetFocus();
                   m_StockList.EnsureVisible( l_pos, TRUE );
                   m_StockList.SetItemState(l_pos,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
				}
				return TRUE;
		   }
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void SelectStock::OnCancel() 
{

	
	CDialog::OnCancel();
}

void SelectStock::OnOK() 
{

	
	if(m_bCloseWindows)
	{
        int temp=m_StockList.GetNextItem( -1, LVNI_SELECTED) ;
		if(temp>=0)
		{
           char strCode[10]; 
		   m_StockList.GetItemText( temp, 1,strCode , 10);
           m_strStockCode=strCode;
		   m_stkKind = m_StockList.GetItemData (temp);
		}
		else
		{
           MessageBox("请正确选择股票代码","操作提示窗口",MB_OK);
           return ;
		}
		CDialog::OnOK();

	}
}

void SelectStock::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{

	if(m_bCloseWindows)
		OnOK();	
	*pResult = 0;
}
