
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ChuquanData.h"

#include "StkDatabase.h"

#include "KEYBRODEANGEL.h"
#include "mainfrm.h"
#include "CTaiChuQuanSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChuquanData property page

IMPLEMENT_DYNCREATE(CChuquanData, CPropertyPage)

CChuquanData::CChuquanData() : CPropertyPage(CChuquanData::IDD)
{
	pDoc = CMainFrame::m_taiShanDoc;

	//{{AFX_DATA_INIT(CChuquanData)
	m_strStockCode = _T("");
	//}}AFX_DATA_INIT
}

CChuquanData::~CChuquanData()
{
}

void CChuquanData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CChuquanData)
	DDX_Control(pDX, 1022, m_button22);
	DDX_Control(pDX, 1012, m_button12);
	DDX_Control(pDX, 1007, m_button7);
	DDX_Control(pDX, 1006, m_button6);
	DDX_Control(pDX, 1004, m_button4);
	DDX_Control(pDX, IDC_LISTCQ, m_ctrlChuQuan);
	DDX_Text(pDX, 1008, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChuquanData, CPropertyPage)
	ON_WM_HELPINFO()
	//{{AFX_MSG_MAP(CChuquanData)
	ON_EN_CHANGE(1008, OnChangeStockSymbol)
	ON_BN_CLICKED(1022, OnAddNew)
	ON_BN_CLICKED(1012, OnModifyCq)
	ON_BN_CLICKED(1004, OnDeleteInfo)
	ON_BN_CLICKED(1007, OnImport)
	ON_BN_CLICKED(1006, OnExport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CChuquanData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CRect Rect;
	LVCOLUMN listColumn;
	char* arColumn[5] = { "除权日期", "送股", "配股", "配股价", "分红" };
	listColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	listColumn.fmt = LVCFMT_CENTER;
	listColumn.cx = 76;
	for (int nColumn = 0; nColumn < 5; nColumn++)
	{
		listColumn.iSubItem = nColumn;
		listColumn.pszText = arColumn[nColumn];

		m_ctrlChuQuan.InsertColumn(nColumn, &listColumn);
	}

	ListView_SetExtendedListViewStyle( m_ctrlChuQuan.m_hWnd, LVS_EX_FULLROWSELECT);

	EnableButton(FALSE);

	for (int i = 0; i < 80; i++)
		m_nChuQuanKind[i] = 0;

	return TRUE;  
}

void CChuquanData::EnableButton(BOOL bEnable)
{
	GetDlgItem(1012)->EnableWindow(bEnable);
	GetDlgItem(1022)->EnableWindow(bEnable);
	GetDlgItem(1004)->EnableWindow(bEnable);
}

BOOL CChuquanData::PreTranslateMessage(MSG* pMsg) 
{

	if( GetFocus() == GetDlgItem(1008) && pMsg->message==WM_CHAR && ( (pMsg->wParam>='0'&&pMsg->wParam<='9') || (pMsg->wParam>='A'&&pMsg->wParam<='z') )  )
	{
		CTaiKeyBoardAngelDlg KeyboardWizard;

		if(pMsg->wParam>='a' && pMsg->wParam<='z' )
			pMsg->wParam-=32;

		KeyboardWizard.input=pMsg->wParam;

		KeyboardWizard.ischar= ( pMsg->wParam>='0' && pMsg->wParam<='9' )? false:true;

		KeyboardWizard.DoModal(); //检测该股票是否已记录在内存模板中

		if(KeyboardWizard.isresultstock )
		{
			CString szSymbol=KeyboardWizard.result;

			m_nKind=KeyboardWizard.m_stkKind;

			CReportData* pDat;
			if ( (CMainFrame::m_taiShanDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0) , pDat,m_nKind) )    
				GetDlgItem(1008)->SetWindowText(szSymbol);
			else 
				GetDlgItem(1008)->SetWindowText("");
		}
		return TRUE;
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CChuquanData::OnChangeStockSymbol() 
{

	CString strStockCode;
	GetDlgItem(1008)->GetWindowText(strStockCode);         


	if(strStockCode.GetLength()==4||strStockCode.GetLength()==6)
	{

		CReportData *Cdat=NULL;
		if(m_ctrlChuQuan.GetItemCount()>0)
			m_ctrlChuQuan.DeleteAllItems();
		if(pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),Cdat,m_nKind))
		{

			EnableButton(TRUE);

			CString sName=Cdat->name;
			GetDlgItem(IDC_STANAME)->SetWindowText(sName);


			Split *pSplit=NULL;

			int nTotleTimes=GetChuQuanInfo(strStockCode.GetBuffer(0),pSplit);
			if((nTotleTimes>0)&&(pSplit!=NULL))
			{
				POWER Power;
				for(int i=0;i<nTotleTimes;i++)
				{
					CTime time=pSplit[i].nTime;
					CString szTime=time.Format("%Y/%m/%d");

					Power.fAllocate=pSplit[i].Allocate;
					Power.fAllocatePrice=pSplit[i].AllocatePrice;
					Power.fDividend=pSplit[i].Bonus;
					Power.fGive=pSplit[i].Give;
					Power.nFlags=pSplit[i].nFlags;
					Power.nTime=pSplit[i].nTime;
					InsertItem(i,szTime,Power);
					m_nChuQuanKind[i]=Power.nFlags;
				}
			}		
		}
	}
	else 
	{

		GetDlgItem(IDC_STANAME)->SetWindowText("");

		if(m_ctrlChuQuan.GetItemCount()>0)
		{
			m_ctrlChuQuan.DeleteAllItems( );
			for(int i=0;i<80;i++)
				m_nChuQuanKind[i]=0;
		}
		EnableButton(FALSE);
	}

}

int CChuquanData::GetChuQuanInfo(CString strStockCode, PSplit &pSplit)
{ //检测该股票是否已记录在内存模板中
	return pDoc->m_sharesInformation.GetChuQuanInfo(strStockCode,m_nKind,pSplit);
}

void CChuquanData::InsertItem(int nItem, CString strNo, POWER& nPower)
{
	CString str;

	m_ctrlChuQuan.InsertItem (nItem, strNo);

	str.Format("%6.2f",nPower.fGive*10);
	m_ctrlChuQuan.SetItemText(nItem,1,str);

	str.Format("%6.2f",nPower.fAllocate*10);
	m_ctrlChuQuan.SetItemText(nItem,2,str);

	str.Format("%6.2f",nPower.fAllocatePrice);
	m_ctrlChuQuan.SetItemText(nItem,3,str);

	str.Format("%6.2f",(float)(nPower.fDividend*10));
	m_ctrlChuQuan.SetItemText(nItem,4,str);
}

void CChuquanData::OnAddNew() 
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
			AfxMessageBox("同一天只能除权一次!",MB_ICONASTERISK);
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

BOOL CChuquanData::IsAlreadyChuQuan(int nSeconds)
{
	CTime tm(nSeconds);
	CString DateToAdd=tm.Format("%Y/%m/%d");
	for( int i=0;i<this->m_ctrlChuQuan.GetItemCount();i++)
	{
		CString str=this->m_ctrlChuQuan.GetItemText(i,0);

		if( DateToAdd == str)
			return FALSE;
	}
	return TRUE;
}

void CChuquanData::AddChuQuanInfo(CString strStockCode,POWER *pPower)
{ //检测该股票是否已记录在内存模板中
	Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.AddChuQuanInfo(strStockCode,m_nKind,&pSplit);
}

void CChuquanData::OnModifyCq() 
{

	CTaiChuQuanSetDlg AddChuquan;
	AddChuquan.m_OperateID='M';
	UpdateData(TRUE);

	int nSel=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( nSel<0 )
	{
		AfxMessageBox("请选择其中一项!",MB_ICONASTERISK);
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

	float fGive =float(atof( szGive ));
	float fAlloc=float(atof( szAlloc));
	float fPrice=float(atof( szPrice));
	float fDivid=float(atof( szDivid));

	AddChuquan.m_timet=tm.GetTime();
	AddChuquan.m_fGive =fGive;
	AddChuquan.m_fAlloc=fAlloc;
	AddChuquan.m_fPrice=fPrice;
	AddChuquan.m_fDivid=fDivid;
	AddChuquan.m_kind=m_nChuQuanKind[nSel];

	if( AddChuquan.DoModal() == IDOK )
	{

		CTime tmNew(AddChuquan.m_timet);
		CString DateToCq=tmNew.Format("%Y/%m/%d");
		for( int i=0;i<this->m_ctrlChuQuan.GetItemCount();i++)
		{
			CString str=this->m_ctrlChuQuan.GetItemText(i,0);
			if( DateToCq == str)
			{
				nSel=i;
				break;
			}
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

void CChuquanData::ModifyChuQuanInfo(CString strStockCode, int nWhichItem, POWER *pPower)
{
	Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.ModifyChuQuanInfo(strStockCode,nWhichItem,&pSplit,m_nKind);
}

void CChuquanData::OnDeleteInfo() 
{

	int Index=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( Index<0 )
	{
		AfxMessageBox("请选择其中一项!",MB_ICONASTERISK);
		return;
	}

	if( AfxMessageBox("要删除吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
		return;
	UpdateData(TRUE);
	m_nChuQuanKind[Index]=0;

	DeleteChuQuanInfo(m_strStockCode,Index);

	m_ctrlChuQuan.DeleteItem( Index );


}

void CChuquanData::DeleteChuQuanInfo(CString strStockCode, int nWhichItem)
{//子窗口框架
	pDoc->m_sharesInformation.DeleteChuQuanInfo(strStockCode,nWhichItem,m_nKind);
}

void CChuquanData::OnImport() 
{

	UpdateData(TRUE);

	FXJ_POWER power_fxj;

	int nSplitMask=0;
	int nTotalCount=0;
	int nEachChuQuanNo=0;
	CString m_InFile = "";
	CString DefExt="*.chu,*.pwr";
	CString FileName="";
	CString Filter="数据格式(*.chu) 分析家格式(*.pwr) |*.chu;*.pwr||";

	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bOpenFileDialog.DoModal() !=IDOK )
		return ;
	m_InFile=bOpenFileDialog.GetPathName();

	m_InFile.MakeLower ();

	CFile InFile;
	if(!(InFile.Open(m_InFile,CFile::modeRead,NULL)))
	{
		MessageBox("引入除权文件不可读!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	CString StockSymbol;    
	int Marker;               


	Split pSplit[80];
	BOOL IsSucc;
	if(m_InFile.Right (4) != ".pwr")
	{
		InFile.Close();
		IsSucc=InstallPowerData(m_InFile);

	}

	else 
	{
		{
			CString StockCode;
			WORD nFlag=0;
			InFile.Read(&nFlag,2);
			if( nFlag!=51250 )
			{
				AfxMessageBox("不是分析家的除权数据格式!");
				InFile.Close();
				IsSucc=FALSE;
				return;
			}
			InFile.Seek(8,CFile::begin);
			nEachChuQuanNo=0;
			BOOL bFirstTime=TRUE;
			bool bSh = true;
			while( InFile.Read(&Marker,4) == 4 )
			{
				if( Marker == 0xFFFFFFFF )
				{
					if(bFirstTime==TRUE)
						bFirstTime=FALSE;  
					else
					{

						int stkKind = 0;
						if(bSh == true)
							stkKind = pDoc->m_sharesInformation.GetStockKind(SH_MARKET_EX,StockSymbol.GetBuffer(0));
						else
							stkKind = pDoc->m_sharesInformation.GetStockKind(SZ_MARKET_EX,StockSymbol.GetBuffer(0));
						if(!pDoc->m_sharesInformation.ImportChuQuanInfo(StockSymbol,pSplit,nEachChuQuanNo,stkKind))
						{
							return;
						}
					}

					::ZeroMemory(pSplit,80*sizeof(Split));
					nEachChuQuanNo=0;
					InFile.Read(StockSymbol.GetBuffer(12),12);
					StockSymbol.ReleaseBuffer(); 
					InFile.Seek(4,CFile::current);


					StockSymbol.MakeLower ();
					bSh = false;
					if(StockSymbol.Left (2) == "sh") bSh = true;

					if( StockSymbol.GetLength() == 8 )
						StockSymbol=StockSymbol.Right(6);
					else if(StockSymbol.GetLength() == 6)
						StockSymbol=StockSymbol.Right(4);

					InFile.Read(&power_fxj, sizeof(FXJ_POWER));

					pSplit[nEachChuQuanNo].nTime=power_fxj.nTime;
					pSplit[nEachChuQuanNo].Give=power_fxj.fGive;
					pSplit[nEachChuQuanNo].Allocate=power_fxj.fAlloc;
					pSplit[nEachChuQuanNo].AllocatePrice=power_fxj.fAllocPrice;
					pSplit[nEachChuQuanNo].Bonus=power_fxj.fDividend;
					nEachChuQuanNo++;
				}
				else 
				{

					InFile.Seek(-4,CFile::current);
					InFile.Read(&power_fxj, sizeof(FXJ_POWER));


					pSplit[nEachChuQuanNo].nTime=power_fxj.nTime;
					pSplit[nEachChuQuanNo].Give=power_fxj.fGive;
					pSplit[nEachChuQuanNo].Allocate=power_fxj.fAlloc;
					pSplit[nEachChuQuanNo].AllocatePrice=power_fxj.fAllocPrice;
					pSplit[nEachChuQuanNo].Bonus=power_fxj.fDividend;
					nEachChuQuanNo++;
				}
			}
		}

		InFile.Close();
		IsSucc=TRUE;
	}

	CString sMsg = "引入失败!";
	if(IsSucc)
		sMsg = "引入完毕!";
	AfxMessageBox(sMsg);
	UpdateData(FALSE);


}

void CChuquanData::OnExport() 
{//行情数据结构数组类实现

	CString DefExt="*.chu";
	CString FileName="ChuQuan.chu";
	CString FilePathName;
	CString Filter= "数据格式(*.chu)|*.chu||";
	CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bSaveFileDialog.DoModal() ==IDOK )
	{
		FilePathName=bSaveFileDialog.GetPathName();
		OutChuQuanData(FilePathName);
	}

}

void CChuquanData::OutChuQuanData(CString FilePath)
{
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	UpdateData(TRUE);
	CFile OutFile;
	if(!OutFile.Open(FilePath,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		MessageBox("导出除权信息失败!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	int nHeaderFlag=-1;
	int nTotalCount=0;
	POWER Power;
	struct POWER_1th      
	{
		BYTE  byFree[8];  
		int   nFlags;     
		int   nTime;
		float fGive;      
		float fAllocate;
		float fAllocatePrice;//行情数据结构数组类实现
		float fDividend;
	};
	POWER_1th Power_1th;
	int nSplitMask=-1;

	//int FileId=FILEID22;
	//OutFile.Write(&FileId,4);
	OutFile.Seek(8,CFile::begin);


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

						int a =pDat1->kind;
						OutFile.Write(&a,4);

						for(int j=0;j<pDat1->pBaseInfo->NumSplit;j++)
						{
							memcpy(&Power,&(pDat1->
								pBaseInfo->m_Split[j]),sizeof(POWER));
							Power_1th.fAllocate=Power.fAllocate;
							Power_1th.fAllocatePrice=Power.fAllocatePrice;
							Power_1th.fDividend=Power.fDividend;
							Power_1th.fGive=Power.fGive;
							Power_1th.nTime=Power.nTime;
							OutFile.Write(&Power_1th,sizeof(POWER_1th));
						}
					}
				}
			}
		}
	}

	OutFile.Close();
	//	}
	if(nTotalCount==0)
	{//数据文件读写实现
		AfxMessageBox("没有除权数据!",MB_ICONASTERISK);
		OutFile.Remove(FilePath);
	}
	else
		AfxMessageBox("导出完毕!",MB_ICONASTERISK);
}

BOOL CChuquanData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);return TRUE;


}

BOOL CChuquanData::InstallPowerData(CString FilePath)
{
	CFile InFile;
	if(!InFile.Open(FilePath,CFile::modeRead))
		return FALSE;

	int Marker;              

	int nEachChuQuanNo=0;
	char strStockCode[8]; 
	int nKind;

	Split pSplit[80];
	int nFlag=0;
	InFile.Read(&nFlag,4);
	//if(nFlag!=FILEID22)
	//{
	//	AfxMessageBox("不是"+g_strCompanyName+"除权数据格式!");
	//	InFile.Close();
	//	return FALSE;
	//}
	::ZeroMemory(pSplit,80*sizeof(Split));
	InFile.Seek(8,CFile::begin);
	BOOL bFirstTime=TRUE;

	while( InFile.Read(&Marker,4) == 4 )
	{
		if( Marker == 0xFFFFFFFF )
		{
			if(bFirstTime==TRUE)
				bFirstTime=FALSE;  
			else
				CMainFrame::m_taiShanDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo,nKind);



			::ZeroMemory(pSplit,80*sizeof(Split));//数据文件读写实现
			nEachChuQuanNo=0;
			InFile.Read(strStockCode,8*sizeof(char));
			InFile.Read(&nKind,4);
			InFile.Seek(8,CFile::current);			

			InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].nTime),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Give),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Allocate),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].AllocatePrice),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Bonus),4);
			nEachChuQuanNo++;
		}
		else 
		{

			InFile.Seek(-4,CFile::current);			
			InFile.Seek(8,CFile::current);			

			InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].nTime),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Give),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Allocate),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].AllocatePrice),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Bonus),4);
			nEachChuQuanNo++;
		}
	}

	CMainFrame::m_taiShanDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo,nKind);  
	InFile.Close();
	return TRUE;

}
