// SelectStockHsDetail2.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "SelectStockHsDetail2.h"
#include "CSharesDetailDays.h"
#include "MainFrm.h"
#include "CAlertSystem.h"

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
// CSelectStockHsDetail2 dialog


CSelectStockHsDetail2::CSelectStockHsDetail2(CWnd* pParent /*=NULL*/)
	: SelectStock(CSelectStockHsDetail2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectStockHsDetail2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectStockHsDetail2::DoDataExchange(CDataExchange* pDX)
{
	SelectStock::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectStockHsDetail2)
	DDX_Control(pDX, IDC_LIST3, m_SelectedStockList);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectStockHsDetail2, SelectStock)
	//{{AFX_MSG_MAP(CSelectStockHsDetail2)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_CHANGEDAY, OnChangeday)
	ON_BN_CLICKED(IDC_ADD2, OnAdd2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CSelectStockHsDetail2::OnDel() 
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

BOOL CSelectStockHsDetail2::QuickSort(int *nItem, int low, int high)
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

void CSelectStockHsDetail2::OnChangeday() 
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

void CSelectStockHsDetail2::OnOK() 
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
			int stkKind = m_SelectedStockList.GetItemData (i);
			int nToday=m_SelectedStockList.GetItemText(i,3)==_T("否") ? 1 : 0;
			for (int j=nToday;j<(nDays+nToday);j++)
			{
				strTime=strTimeArray.GetAt(j);
				if (CSharesCompute::GetMarketKind(stkKind) == SH_MARKET_EX)
					strFile=_T("sh")+strTime+strID+_T(".hst");
				else
					strFile=_T("sz")+strTime+strID+_T(".hst");
				m_FileArray.Add(strFile);
			}
		}
	}
	CDialog::OnOK();
}

void CSelectStockHsDetail2::OnCancel() 
{
	
	CDialog::OnCancel();
}

void CSelectStockHsDetail2::OnAdd2() 
{

	SymbolKindArr sSymbolArray;
	GetSymbolArray(sSymbolArray);

	static CString strDays=_T("1");
	static CString strIncludeToday;
	CTime t=CTime::GetCurrentTime();
	if (t.GetDayOfWeek()>1 && t.GetDayOfWeek()<7 && t.GetHour()>16)
		strIncludeToday=_T("是");
	else
		strIncludeToday=_T("否");

	int iItem=0;
	int iActualItem;
	int nCount=sSymbolArray.GetSize();
	for (int i = 0;i<nCount;i++)
	{

		CString strID=sSymbolArray[i].m_chSymbol ;
		CString strName = "";
		CReportData* pdt = 0;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup(strID.GetBuffer (0),pdt,sSymbolArray[i].m_nSymbolKind))
			strName = pdt->name;
		if(!pdt) 
		{
			ASSERT(FALSE);
			continue;
		}

		strID.ReleaseBuffer ();


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
		lvitem.mask = LVIF_TEXT| LVIF_PARAM;
		lvitem.iItem = iItem++;
		lvitem.iSubItem = 0;
		lvitem.lParam = pdt->kind ;
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

void CSelectStockHsDetail2::GetSymbolArray(SymbolKindArr &sSymbolArray)
{
	
	CArrayStockType typeArr;
	CTaiShanDoc *m_pDoc = CMainFrame::m_taiShanDoc ;
	int n ;
	CString s;
	SharesNameArr3 stockInfo;

	int nCurTab=((CTabCtrl*)GetDlgItem(IDC_TAB_TYPE))->GetCurSel();
	if(nCurTab>0)
	{
		n=m_StockTypeList.GetSelectedCount( ) ;
        int temp=m_StockTypeList.GetNextItem( -1, LVNI_SELECTED) ;
		int index=0;
        while(temp>=0)
		{
           stockInfo.m_nType =m_StockTypeList.GetItemData(temp);
		   m_StockTypeList.GetItemText( temp, 0, stockInfo.m_sSymbol, 18);
           temp=m_StockTypeList.GetNextItem( temp, LVNI_SELECTED) ;
	   	   typeArr.Add(stockInfo);
		}
		m_StockTypeList.SetSelectionMark(-1 );
	}
	else
	{
		n=m_StockList.GetSelectedCount( ) ;
        int temp=m_StockList.GetNextItem( -1, LVNI_SELECTED) ;
        while(temp>=0)
		{
			int nKind=m_StockList.GetItemData(temp);

           stockInfo.m_nType =-1-nKind;
		   m_StockList.GetItemText( temp, 1, stockInfo.m_sSymbol, 18);
           temp=m_StockList.GetNextItem( temp, LVNI_SELECTED) ;
	   	   typeArr.Add(stockInfo);
		}
		m_StockList.SetSelectionMark(-1 );
	}
	
	CAlertSystem::GetSymbolArray(typeArr, sSymbolArray);

}

BOOL CSelectStockHsDetail2::OnInitDialog() 
{
	SelectStock::OnInitDialog();
	

	CRect rc;
	m_SelectedStockList.GetWindowRect(&rc);
	m_SelectedStockList.InsertColumn(0,_T("代码"),LVCFMT_LEFT,rc.Width() * 1/6,0);
	m_SelectedStockList.InsertColumn(1,_T("名称"),LVCFMT_LEFT,rc.Width() * 1/3,1);
	m_SelectedStockList.InsertColumn(2,_T("下载最近天数"),LVCFMT_LEFT,rc.Width() * 1/4,2);
	m_SelectedStockList.InsertColumn(3,_T("包括今天"),LVCFMT_LEFT,rc.Width() * 1/4,3);
	DWORD dwNewStyle=m_SelectedStockList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES;
	m_SelectedStockList.SetExtendedStyle(dwNewStyle);

	CenterWindow();
	
	return TRUE;  
}
