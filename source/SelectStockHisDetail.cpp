// SelectStockHisDetail.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "SelectStockHisDetail.h"
#include "CSharesDetailDays.h"

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

#ifdef _UNICODE
	#define TOCH(strSource,strDest,nCount) WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,(LPCWSTR)strDest,0,strSource,nCount,NULL);
#else
	#define TOCH(strSource,strDest,nCount) strcpy(strDest,(LPCSTR)strSource);
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectStockHisDetail dialog


CSelectStockHisDetail::CSelectStockHisDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectStockHisDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectStockHisDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectStockHisDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStockHisDetail)
	DDX_Control(pDX, IDC_LIST3, m_SelectedStockList);
	DDX_Control(pDX, IDC_LIST2, m_StockList);
	DDX_Control(pDX, IDC_LIST1, m_StockTypeList);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_DEL, m_btnDel);
//	DDX_Control(pDX, IDC_ADD, m_btnAdd);
}


BEGIN_MESSAGE_MAP(CSelectStockHisDetail, CDialog)
	//{{AFX_MSG_MAP(CSelectStockHisDetail)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnStockTypeClick)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnStockDblclk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, OnSelectedStockDblclk)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CHANGEDAY, OnChangeday)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CSelectStockHisDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rc;
	m_SelectedStockList.GetWindowRect(&rc);
	m_SelectedStockList.InsertColumn(0,_T("代码"),LVCFMT_LEFT,rc.Width() * 1/6,0);
	m_SelectedStockList.InsertColumn(1,_T("名称"),LVCFMT_LEFT,rc.Width() * 1/3,1);
	m_SelectedStockList.InsertColumn(2,_T("下载最近天数"),LVCFMT_LEFT,rc.Width() * 1/4,2);
	m_SelectedStockList.InsertColumn(3,_T("包括今天"),LVCFMT_LEFT,rc.Width() * 1/4,3);
	DWORD dwNewStyle=m_SelectedStockList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	m_SelectedStockList.SetExtendedStyle(dwNewStyle);

	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	m_pImageListSmall=new CImageList;
	ASSERT(m_pImageListSmall != NULL);    
	m_pImageListSmall->Create(IDB_BITMAP12,16,0,RGB(192,192,192));

   	CString	strItem= _T("StockType");
	this->m_StockTypeList.SetImageList(m_pImageListSmall, LVSIL_SMALL);
	this->m_StockTypeList.InsertColumn(0, strItem);
	m_StockTypeList.SetColumnWidth( 0, 80 );

    InsertStockTypeToListExt(0);
	InsertStockColumnToList();

	NMHDR pNMHDR;
	int result;
	m_StockTypeList.SetItemState(1,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	OnStockTypeClick(&pNMHDR, (LRESULT* )&result);

	m_pImageListSmall->Detach ();
	delete m_pImageListSmall;
	CenterWindow();
	return TRUE; 

}

BOOL CSelectStockHisDetail::InsertStockColumnToList()
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

BOOL CSelectStockHisDetail::InsertStockTypeToListExt(int nKind)
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

			lvitem.mask = LVIF_TEXT | LVIF_IMAGE | TVIF_PARAM;
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
	return TRUE;
}

BOOL CSelectStockHisDetail::InsertStockTypeToList() 
{
   	CRect           rect;
	int             iItem;
	LV_ITEM         lvitem;
	LPTSTR          pStrTemp;

    iItem=0;

	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=0;
	CString strIconShortDesc="上证指数";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 


	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=1;
	strIconShortDesc="上证Ａ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=2;
	strIconShortDesc="上证Ｂ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=3;
	strIconShortDesc="深证指数";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 


	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=4;
	strIconShortDesc="深证Ａ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE | TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=5;
	strIconShortDesc="深证Ｂ股";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=6;
	strIconShortDesc="上证债券";
	pStrTemp= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
	lvitem.pszText = pStrTemp;
	lvitem.iImage = 0;
	m_StockTypeList.InsertItem(&lvitem); 

	iItem++;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE|TVIF_PARAM;
	lvitem.iItem =  iItem ;
	lvitem.iSubItem = 0;
	lvitem.lParam=7;
	strIconShortDesc="深证债券";
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
	lvitem.lParam=SZJIJIN;
	strIconShortDesc="深证基金";
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

void CSelectStockHisDetail::OnStockTypeClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_StockList.DeleteAllItems();
	m_StockTypeList.SetSelectionMark(-1 );
	int i,j;
	BOOL bFound;
	int nSelected;
	CString strID;

	int temp=m_StockTypeList.GetNextItem( -1, LVNI_SELECTED) ;
	if(temp==-1)
		return;
    int nParam=m_StockTypeList.GetItemData(temp); 
	nSelected=m_SelectedStockList.GetItemCount();
	if(nParam==CHOOSESTK)
	{
		CStringArray l_StockArray;
		m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
		int DisplayCount=l_StockArray.GetSize();
		for(i=0; i <(int)DisplayCount; i++)          
		{
			 CReportData *Cdat;
			 CString l_strCode=l_StockArray.GetAt(i);
	 		 if (m_pDoc->m_sharesInformation.Lookup(l_strCode.GetBuffer(0) , Cdat) == TRUE)     //检测该股票是否已记录在内存模板中
			 {
				 bFound=FALSE;
				 for (j=0; j<nSelected; j++)
				 {
					strID=m_SelectedStockList.GetItemText(j,0);
					if (Cdat->id==strID)
					{
						bFound=TRUE;
						break;
					}
				 }

				 if (!bFound)
					InsertStockToList(Cdat->name,Cdat->id);
			 }
		}
	}
	else if(nParam<CHOOSESTK)
	{
		int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(nParam);
		for(i=0; i<temp; i++)
		{
			CReportData *Cdat;
			m_pDoc->m_sharesInformation.GetStockItem(nParam,i,Cdat);
			if(Cdat==NULL)
				continue;

			 bFound=FALSE;
			 for (j=0; j<nSelected; j++)
			 {
				strID=m_SelectedStockList.GetItemText(j,0);
				if (Cdat->id==strID)
				{
					bFound=TRUE;
					break;
				}
			 }
			 if (!bFound)
				InsertStockToList(Cdat->name,Cdat->id);
		}
	}
	else 
	{
		CString l_sStockTypeName=this->m_StockTypeNameArray.GetAt(nParam-1000);
        CStringArray l_StockCodeArray;
	    m_pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
        
	    int DisplayCount=l_StockCodeArray.GetSize();
		for(int i=0; i<(int)DisplayCount; i++)        
		{    
			 CReportData *Cdat;
			 CString m_zqdm; 
			 m_zqdm=l_StockCodeArray.GetAt(i) ;
	 		 if (m_pDoc->m_sharesInformation.Lookup(m_zqdm.GetBuffer(0) , Cdat) == TRUE)     //检测该股票是否已记录在内存模板中
			 {
				 bFound=FALSE;
				 for (j=0; j<nSelected; j++)
				 {
					strID=m_SelectedStockList.GetItemText(j,0);
					if (Cdat->id==strID)
					{
						bFound=TRUE;
						break;
					}
				 }

				 if (!bFound)
					InsertStockToList(Cdat->name,Cdat->id);
			 }
		}
	}
	*pResult = 0;
}

BOOL CSelectStockHisDetail::InsertStockToList(char *name,char *code)
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

		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_StockList.InsertItem(&lvitem);
		else
			this->m_StockList.SetItem(&lvitem);
	} 
	return TRUE;
}

void CSelectStockHisDetail::OnStockDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}

void CSelectStockHisDetail::OnSelectedStockDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnChangeday();
	*pResult = 0;
}

void CSelectStockHisDetail::OnDel() 
{
	int nCount=m_SelectedStockList.GetItemCount();
	if (nCount<1)
	{
		if (m_SelectedStockList.GetItemCount()>0)
			AfxMessageBox("请选择想要删除的项.");
		return;
	}
	int* nItemArray=new int[nCount];
	POSITION pos=m_SelectedStockList.GetFirstSelectedItemPosition();
	int i=0;
	while(pos)
		nItemArray[i++]=m_SelectedStockList.GetNextSelectedItem(pos);
	if (nCount>1)
	{
		if (!QuickSort(nItemArray,0,nCount-1))
		{
			delete []nItemArray;
			return;
		}
	}

	for (i=nCount-1; i>-1; i--)
		m_SelectedStockList.DeleteItem(nItemArray[i]);
	delete []nItemArray;
}

void CSelectStockHisDetail::OnAdd() 
{
	static CString strDays=_T("1");
	static CString strIncludeToday;
	CTime t=CTime::GetCurrentTime();
	if (t.GetDayOfWeek()>1 && t.GetDayOfWeek()<7 && t.GetHour()>16)
		strIncludeToday=_T("是");
	else
		strIncludeToday=_T("否");

	int iItem=0;
	int iActualItem;
	int nCount=(int)m_StockList.GetSelectedCount();
	if (nCount>0)
	{
		POSITION pos=m_StockList.GetFirstSelectedItemPosition( );
		while (pos)
		{
			int nItem = m_StockList.GetNextSelectedItem(pos);
			CString strID=m_StockList.GetItemText(nItem,1);
			CString strName=m_StockList.GetItemText(nItem,0);

			BOOL bFound=FALSE;
			int nCount=m_SelectedStockList.GetItemCount();
			for (int i=0;i<nCount;i++)
			{
				if (strID==m_SelectedStockList.GetItemText(i,0))
				{
					bFound=TRUE;
					break;
				}
			}
			if (bFound)
				continue;

			LV_ITEM lvitem;
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iItem++;
			lvitem.iSubItem = 0;
			lvitem.pszText = strID.GetBuffer(0);
			iActualItem=m_SelectedStockList.InsertItem(&lvitem);

			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iActualItem;
			lvitem.iSubItem = 1;
			lvitem.pszText = strName.GetBuffer(0);
			m_SelectedStockList.SetItem(&lvitem);

			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iActualItem;
			lvitem.iSubItem = 2;
			lvitem.pszText = strDays.GetBuffer(0);
			m_SelectedStockList.SetItem(&lvitem);

			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = iActualItem;
			lvitem.iSubItem = 3;
			lvitem.pszText = strIncludeToday.GetBuffer(0);
			m_SelectedStockList.SetItem(&lvitem);
		}
	}
}

void CSelectStockHisDetail::OnOK() 
{
	int nCount=m_SelectedStockList.GetItemCount();
	if (nCount>0)
	{
		m_FileArray.RemoveAll();
		CStringArray strTimeArray;
		CString strTime;
		CString strFile;
		CTime t=CTime::GetCurrentTime();
		CTimeSpan ts(1,0,0,0);
		switch(t.GetDayOfWeek())
		{
		case 1:
			t-=ts;
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		case 7:
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		case 6:
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		case 5:
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		case 4:
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			t-=ts;
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		case 3:
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			t-=ts;
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		case 2:
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			t-=ts;
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			t-=ts;
			strTime.Format("%04d%02d%02d",t.GetYear(),t.GetMonth(),t.GetDay());
			strTimeArray.Add(strTime);
			break;
		}

		CString strDays;
		CString strID;
		char sDays[3];
		int nDays;
		for (int i=0;i<nCount;i++)
		{
			strID=m_SelectedStockList.GetItemText(i,0);
			strDays=m_SelectedStockList.GetItemText(i,2);
			TOCH(strDays.GetBuffer(0),sDays,3);
			nDays=atoi(sDays);
			int nToday=m_SelectedStockList.GetItemText(i,3)==_T("否") ? 1 : 0;
			for (int j=nToday;j<(nDays+nToday);j++)
			{
				strTime=strTimeArray.GetAt(j);
				if (CSharesCompute::GetMarketKind(strID) == SH_MARKET_EX)
					strFile=_T("sh")+strTime+strID+_T(".hst");
				else
					strFile=_T("sz")+strTime+strID+_T(".hst");
				m_FileArray.Add(strFile);
			}
		}
	}
	CDialog::OnOK();
}

BOOL CSelectStockHisDetail::QuickSort(int *nItem,int low,int high)
{
    int lo = low;
    int hi = high;
    if( hi <= lo ) return FALSE;

	int pivot=nItem[(low+high)/2];

    while( lo <= hi )
    {
        while (lo<high && nItem[lo]<pivot)
 			++lo;

		while (hi>low && nItem[hi]>pivot)
 			--hi;

        if (lo <= hi)
        {
            if (nItem[lo]!=nItem[hi])
            {
				int nTemp=nItem[lo];
				nItem[lo]=nItem[hi];
				nItem[hi]=nTemp;
            }

            ++lo;
            --hi;
        }
    }

    if( low < hi )
        QuickSort(nItem, low, hi);

    if( lo < high )
        QuickSort(nItem, lo, high);

    return TRUE;
}

void CSelectStockHisDetail::OnChangeday() 
{
	int nCount;
	if ((nCount=m_SelectedStockList.GetSelectedCount())<1)
	{
		if (m_SelectedStockList.GetItemCount()>0)
			AfxMessageBox(_T("请选择需要改变天数的项."));
		return;
	}
	
	CSharesDetailDays dlg;
	POSITION pos;
	CString strDays;
	CString strIncludeToday=_T("否");
	char sDays[10];
	int nDays;
	if (nCount==1)
	{
		pos=m_SelectedStockList.GetFirstSelectedItemPosition();
		int nItem=m_SelectedStockList.GetNextSelectedItem(pos);
		strDays=m_SelectedStockList.GetItemText(nItem,2);
		TOCH(strDays.GetBuffer(0),sDays,10);
		nDays=atoi(sDays);
		dlg.SetDays(nDays);
		dlg.SetToday(m_SelectedStockList.GetItemText(nItem,3)!=strIncludeToday);
	}
	if (dlg.DoModal()==IDOK)
	{
		strDays.Format("%d",dlg.GetDays());
		strIncludeToday= dlg.GetToday()==TRUE ? _T("是") : _T("否");
		pos=m_SelectedStockList.GetFirstSelectedItemPosition();
		while(pos)
		{
			LV_ITEM lvitem;

			lvitem.mask = LVIF_TEXT;
			int nItem = m_SelectedStockList.GetNextSelectedItem(pos);
			lvitem.iItem = nItem;
			lvitem.iSubItem = 2;
			lvitem.pszText = strDays.GetBuffer(0);
			m_SelectedStockList.SetItem(&lvitem);
			
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = nItem;
			lvitem.iSubItem = 3;
			lvitem.pszText = strIncludeToday.GetBuffer(0);
			m_SelectedStockList.SetItem(&lvitem);
		}
	}
}

BOOL CSelectStockHisDetail::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
		
			
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
				m_pDoc->m_sharesInformation.Lookup(result,pCdat);
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
