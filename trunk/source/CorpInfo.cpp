// CorpInfo.cpp : implementation file


#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CorpInfo.h"
#include "KEYBRODEANGEL.h"
#include "mainfrm.h"
#include "CTaiChuQuanSetDlg.h"
#include "CTaiChuQuanInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCorpInfo property page

IMPLEMENT_DYNCREATE(CCorpInfo, CPropertyPage)

CCorpInfo::CCorpInfo() : CPropertyPage(CCorpInfo::IDD)
{
	//{{AFX_DATA_INIT(CCorpInfo)
	m_strStockCode = _T("");
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_taiShanDoc;
}

CCorpInfo::~CCorpInfo()
{
}

void CCorpInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCorpInfo)
	DDX_Control(pDX, IDC_LIST5, m_ctrlChuQuan);
	DDX_Text(pDX, 1008, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCorpInfo, CPropertyPage)
	//{{AFX_MSG_MAP(CCorpInfo)
	ON_EN_CHANGE(1008, OnChangeStockSymbol)
	ON_BN_CLICKED(1012, OnAddItem)
	ON_BN_CLICKED(1022, OnModifyCq)
	ON_BN_CLICKED(1004, OnDeleteInfo)
	ON_BN_CLICKED(1005, OnButtonLeadout)
	ON_BN_CLICKED(1006, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCorpInfo message handlers

BOOL CCorpInfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	LVCOLUMN listColumn;
	char * arColumn[5]={"除权日期","送股数","配股数","配股价", "分红"};
	listColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt=LVCFMT_CENTER;
	listColumn.cx=76;
	for( int nColumn=0;nColumn<5;nColumn++)
	{
		listColumn.iSubItem=nColumn;
		listColumn.pszText=arColumn[nColumn];
		m_ctrlChuQuan.InsertColumn(nColumn, &listColumn);
	}

	ListView_SetExtendedListViewStyle(  m_ctrlChuQuan.m_hWnd, LVS_EX_FULLROWSELECT);
	COLORREF color=RGB(255,255,220);
    m_ctrlChuQuan.SetBkColor( color );
	m_ctrlChuQuan.SetTextBkColor( color );

	
	EnableButton(FALSE);

	for(int i=0;i<80;i++)
		m_nChuQuanKind[i]=0;
	
	return TRUE;  
}

BOOL CCorpInfo::PreTranslateMessage(MSG* pMsg) 
{

		if( GetFocus() == GetDlgItem(1008) && pMsg->message==WM_CHAR && ( (pMsg->wParam>='0'&&pMsg->wParam<='9') || (pMsg->wParam>='A'&&pMsg->wParam<='z') )  )
	{
		CTaiKeyBoardAngelDlg KeyboardWizard;

		if(pMsg->wParam>='a' && pMsg->wParam<='z' )
			pMsg->wParam-=32;
			
		KeyboardWizard.input=pMsg->wParam;

		KeyboardWizard.ischar= ( pMsg->wParam>='0' && pMsg->wParam<='9' )? false:true;

		KeyboardWizard.DoModal();
	    
		if(KeyboardWizard.isresultstock )
		{
			CString szSymbol=KeyboardWizard.result;
			CReportData* pDat;
		 	if ( (CMainFrame::m_taiShanDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0) , pDat) )     //检测该股票是否已记录在内存模板中
				GetDlgItem(1008)->SetWindowText(szSymbol);
		}
		return TRUE;
	}

	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CCorpInfo::OnChangeStockSymbol() 
{
	
	CString StockSymbol;
	GetDlgItem(1008)->GetWindowText(StockSymbol);
	int stolength=StockSymbol.GetLength();
	CString StockNo="            ";
	((CEdit*)GetDlgItem(IDC_STANAME))->SetWindowText(StockNo.GetBuffer(12));
	if(stolength!=4&&stolength!=6)
		return;

	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	

	CReportData* pdt=NULL;
	if(pDoc->m_sharesInformation.Lookup(StockSymbol.GetBuffer (0),pdt))
	{
		EnableButton(TRUE);
        if(m_ctrlChuQuan.GetItemCount()>0)
			m_ctrlChuQuan.DeleteAllItems();
		CString sName = pdt->name ;
		((CEdit*)GetDlgItem(IDC_STANAME))->SetWindowText(sName);
		Split *pSplit=NULL;
		int nTotleTimes=GetChuQuanInfo(StockSymbol.GetBuffer(0),pSplit);
		if((nTotleTimes>0)&&(pSplit!=NULL))
		{
			
			POWER Power;
			for(int i=0;i<nTotleTimes;i++)
			{
				CTime time=CTime::GetCurrentTime();
				CString szTime=time.Format("%Y/%m/%d");
				Power.fAllocate=pSplit[i].Allocate;
				Power.fAllocatePrice=pSplit[i].AllocatePrice;
				Power.fDividend=pSplit[i].Bonus;
				Power.fGive=pSplit[i].Give;
				Power.nFlags=pSplit[i].nFlags;
				Power.nTime=pSplit[i].nTime;
                InsertItem(m_ctrlChuQuan.GetItemCount(),szTime,Power);
				m_nChuQuanKind[i]=Power.nFlags;
			}
		}
	}
	else
	{
		
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("");
		EnableButton(FALSE);
		if(m_ctrlChuQuan.GetItemCount()>0)
		{
			m_ctrlChuQuan.DeleteAllItems( );
			for(int i=0;i<80;i++)
				m_nChuQuanKind[i]=0;
		}
	}
}

void CCorpInfo::OnAddItem() 
{

	
	LVITEM	listItem;
	listItem.mask=LVIF_TEXT|LVIF_IMAGE;
	UpdateData(TRUE);
	CTaiChuQuanSetDlg AddChuquan;
	if(AddChuquan.DoModal()==IDOK)
	{
		int nTimes=m_ctrlChuQuan.GetItemCount();
		
		float fGive =AddChuquan.m_fGive/10.0f;
		float fAlloc=AddChuquan.m_fAlloc/10.0f;
		float fPrice=AddChuquan.m_fPrice;
		float fDivid=AddChuquan.m_fDivid/10.0f;
		
		if( fGive==0&&fAlloc==0&&fPrice==0&&fDivid==0)
			return;

		if( !IsAlreadyChuQuan(AddChuquan.m_timet) )
		{
			AfxMessageBox("同一天只能除权一次。",MB_ICONASTERISK);
			return;
		}

		CTime tm(AddChuquan.m_timet);
		CString str;

		str=tm.Format("%Y/%m/%d");
		POWER Power;
		Power.nTime=tm.GetTime();
		Power.fGive=fGive;
		Power.fAllocate=fAlloc;
		Power.fAllocatePrice=fPrice;
		Power.fDividend=fDivid;
		Power.nFlags=AddChuquan.m_kind;
		m_nChuQuanKind[nTimes]=Power.nFlags;
	
		InsertItem(nTimes,str,Power);
	
		AddChuQuanInfo(m_strStockCode,&Power);
	}

	
}


BOOL CCorpInfo::IsAlreadyChuQuan(int nSeconds)
{
    CTime tm(nSeconds);
	int DateToAdd=atoi(tm.Format("%Y%m%d"));
	for( int i=0;i<this->m_ctrlChuQuan.GetItemCount();i++)
	{
		CString str=this->m_ctrlChuQuan.GetItemText(i,0);
		if( DateToAdd == atoi(str.GetBuffer(0)))
			return FALSE;
	}
	return TRUE;
}

void CCorpInfo::InsertItem(int nItem, CString strNo, POWER nPower)
{
    CString str;

	m_ctrlChuQuan.InsertItem (nItem, strNo);

	str.Format("%6.2f",nPower.fGive);
	m_ctrlChuQuan.SetItemText(nItem,1,str);

	str.Format("%6.2f",nPower.fAllocate);
	m_ctrlChuQuan.SetItemText(nItem,2,str);

	str.Format("%6.2f",nPower.fAllocatePrice);
	m_ctrlChuQuan.SetItemText(nItem,3,str);

	str.Format("%6.2f",nPower.fDividend);
	m_ctrlChuQuan.SetItemText(nItem,4,str);
}

void CCorpInfo::AddChuQuanInfo(CString strStockCode, POWER *pPower)
{
    Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.AddChuQuanInfo(strStockCode,&pSplit);
}

void CCorpInfo::OnModifyCq() 
{

	CTaiChuQuanSetDlg AddChuquan;
	AddChuquan.m_OperateID='M';
	UpdateData(TRUE);

	int nSel=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( nSel<0 )
	{
		AfxMessageBox("请选择其中一项.",MB_ICONASTERISK);
		return;
	}

	CString strDate=m_ctrlChuQuan.GetItemText( nSel, 0);
	strDate=strDate.Left(4)+strDate.Mid(5,2)+strDate.Right(2);

	int year=atoi(strDate)/10000;
	int month=(atoi(strDate)/100)%100;
	int day=atoi(strDate)%100;
	CTime tm(year,month,day,8,30,0);

	CString szGive =m_ctrlChuQuan.GetItemText( nSel, 1);
	szGive.TrimLeft();
	CString szAlloc=m_ctrlChuQuan.GetItemText( nSel, 2);
	szAlloc.TrimLeft();
	CString szPrice=m_ctrlChuQuan.GetItemText( nSel, 3);
	szPrice.TrimLeft();
	CString szDivid=m_ctrlChuQuan.GetItemText( nSel, 4);
	szDivid.TrimLeft();

	float fGive =(float(atof( szGive )))*10.0f;
	float fAlloc=(float(atof( szAlloc)))*10.0f;
	float fPrice=(float(atof( szPrice)));
	float fDivid=(float(atof( szDivid)))*10.0f;

	AddChuquan.m_timet=tm.GetTime();
	AddChuquan.m_fGive =fGive;
	AddChuquan.m_fAlloc=fAlloc;
	AddChuquan.m_fPrice=fPrice;
	AddChuquan.m_fDivid=fDivid;
    AddChuquan.m_kind=m_nChuQuanKind[nSel];

	if( AddChuquan.DoModal() == IDOK )
	{
		if( !IsAlreadyChuQuan(AddChuquan.m_timet) )
		{
			AfxMessageBox("同一天只能除权一次。",MB_ICONASTERISK);
			return;
		}

		m_ctrlChuQuan.DeleteItem( nSel );
	
		CTime tm(AddChuquan.m_timet);
		POWER Power;
		Power.nTime=tm.GetTime();
		Power.fGive=AddChuquan.m_fGive/10.0f;
		Power.fAllocate=AddChuquan.m_fAlloc/10.0f;
		Power.fAllocatePrice=AddChuquan.m_fPrice;
		Power.fDividend=AddChuquan.m_fDivid/10.0f;
		Power.nFlags=AddChuquan.m_kind;
		m_nChuQuanKind[nSel]=Power.nFlags;

		if( Power.fGive==0&&Power.fGive==0&&Power.fAllocate==0&&Power.fDividend==0)
			return;

		CString str;
		str=tm.Format("%Y/%m/%d");
		InsertItem(nSel,str,Power);
		ModifyChuQuanInfo(m_strStockCode,nSel,&Power);
	}
}

void CCorpInfo::ModifyChuQuanInfo(CString strStockCode, int nWhichItem,POWER *pPower)
{
    Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.ModifyChuQuanInfo(strStockCode,nWhichItem,&pSplit);
}

void CCorpInfo::OnDeleteInfo() 
{

	int Index=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( Index<0 )
	{
		AfxMessageBox("请选择其中一项",MB_ICONASTERISK);
		return;
	}

	if( AfxMessageBox("要删除吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
		return;
    UpdateData(TRUE);
	m_nChuQuanKind[Index]=0;

	DeleteChuQuanInfo(m_strStockCode,Index);

	m_ctrlChuQuan.DeleteItem( Index );
	
}

void CCorpInfo::DeleteChuQuanInfo(CString strStockCode, int nWhichItem)
{
	pDoc->m_sharesInformation.DeleteChuQuanInfo(strStockCode,nWhichItem);
}

void CCorpInfo::EnableButton(BOOL bEnable)
{
    GetDlgItem(1012)->EnableWindow(bEnable);
	GetDlgItem(1022)->EnableWindow(bEnable);
	GetDlgItem(1004)->EnableWindow(bEnable);
}

int CCorpInfo::GetChuQuanInfo(CString strStockCode, PSplit &pSplit)
{
   return pDoc->m_sharesInformation.GetChuQuanInfo(strStockCode,pSplit);
}

void CCorpInfo::OnButtonLeadout() 
{

    CTaiChuQuanInDlg dlg;
	dlg.DoModal();
}

void CCorpInfo::OnExport() 
{

	CString DefExt="*.pwr";
	CString FileName="dest.pwr";
	CString FilePathName;
	CString Filter="power file (*.pwr)|*.PWR||";
	CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCSTR)Filter,this);
#ifdef NET_CLIENT
	bSaveFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_sPrivatePath ;
#else
	bSaveFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
#endif
	if( bSaveFileDialog.DoModal() ==IDOK )
		FilePathName=bSaveFileDialog.GetPathName();
    OutChuQuanData(FilePathName);

	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	
}

void CCorpInfo::OutChuQuanData(CString FilePath)
{
    ::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	UpdateData(TRUE);
	CFile OutFile;
	if(!OutFile.Open(FilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		MessageBox("非常抱歉,导出除权信息失败","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	int nHeaderFlag=-1;
	int nTotalCount=0;
	POWER Power;
	int nSplitMask=-1;

	OutFile.Write(&nHeaderFlag,sizeof(int));
	OutFile.Write(&nTotalCount,sizeof(int));
	

	for(int nStockType=0;nStockType<8;nStockType++)
	{
		for(int i=0;i<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);i++)
		{
		
			CReportData *pDat1=NULL;
			pDoc->m_sharesInformation.GetStockItem(nStockType,i,pDat1);
			if(pDat1)
			{
				if(pDat1->pBaseInfo)
				{
					if(pDat1->pBaseInfo->NumSplit>0)
					{
						nTotalCount++;
						OutFile.Write(&nSplitMask,sizeof(int));
						OutFile.Write(&(pDat1->id),8*sizeof(char));
						for(int j=0;j<pDat1->pBaseInfo->NumSplit;j++)
						{
							memcpy(&Power,&(pDat1->
								pBaseInfo->m_Split[j]),sizeof(POWER));
							OutFile.Write(&Power,sizeof(POWER));
						}
					}
				}
			}
		}
	}

	OutFile.Seek(4, CFile::begin);
	OutFile.Write(&nTotalCount, sizeof(int));
	OutFile.Close();

	AfxMessageBox("导出完毕！",MB_ICONASTERISK);

}
