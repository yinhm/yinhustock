// XgnExtDataDlg.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "XgnExtDataDlg.h"
#include "XgnExtModify.h"
#include "CFormularCompute.h"
#include "MainFrm.h"
#include "CFormularContent.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineFileKLine.h"
#include "DownLoadDlg.h"
#include "ProgressDialog.h"
#include "GetSetReg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define Ext_data_test
/////////////////////////////////////////////////////////////////////////////

indicatorInfoArr XgnExtDataDlg::m_infoArr;
int XgnExtDataDlg::m_nValidCount[256];
DWORD XgnExtDataDlg::m_dwTimer = 300;

XgnExtDataDlg::XgnExtDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XgnExtDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(XgnExtDataDlg)
	m_nDaysLast = 5;
	m_dwTimer = 300;
	//}}AFX_DATA_INIT
}


void XgnExtDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XgnExtDataDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON_MODIFY, m_modify);
	DDX_Control(pDX, IDC_BUTTON_FRESH2, m_refresh2);
	DDX_Control(pDX, IDC_BUTTON_FRESH, m_refresh);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_delete);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_add);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LIST1, m_listData);
	DDX_Text(pDX, IDC_EDIT2, m_nDaysLast);
	DDV_MinMaxUInt(pDX, m_nDaysLast, 1, 50000);
	DDX_Text(pDX, IDC_EDIT1, m_dwTimer);
	DDV_MinMaxDWord(pDX, m_dwTimer, 1, 60000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XgnExtDataDlg, CDialog)
	//{{AFX_MSG_MAP(XgnExtDataDlg)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, OnButtonFresh)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON_FRESH2, OnButtonFreshAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL XgnExtDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listData.SetExtendedStyle(m_listData.GetExtendedStyle()|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP);
	m_listData.InsertColumn(0,"名称",LVCFMT_LEFT,80);
	m_listData.InsertColumn(1,"说明",LVCFMT_LEFT,120);
	m_listData.InsertColumn(2,"序列号",LVCFMT_LEFT,60);
	m_listData.InsertColumn(3,"目标周期",LVCFMT_LEFT,60);


	InsertListInfo();
	m_dwTimer = GetTimegap();

	UpdateData(FALSE);
	
	
	return TRUE;  
}
void XgnExtDataDlg::OnButtonFresh() 
{
	UpdateData();
	
	int nItem = 0;
	
	POSITION pos = this->m_listData .GetFirstSelectedItemPosition();
    while(pos != NULL)
	{
        nItem = m_listData.GetNextSelectedItem(pos);
		if(nItem>= m_infoArr.GetSize ()) return ;
		RefreshData(m_infoArr[nItem], false,true);

	}
	AfxMessageBox("操作完成！");

	
}


void XgnExtDataDlg::OnButtonModify() 
{

	int nItem = 0;

	POSITION pos = this->m_listData .GetFirstSelectedItemPosition();
    if(pos == NULL)
	{
		TRACE0("No items were selected!\n");
		return;
	}
    else
	{
        nItem = m_listData.GetNextSelectedItem(pos);
	}

	if(nItem>= m_infoArr.GetSize ()) return ;

	XgnExtModify dlg(this);
	dlg.m_nSelInfo = nItem;
	dlg.m_infoData = this->m_infoArr [nItem];
	dlg.m_nID = m_infoArr [nItem].m_nID ;

	if(dlg.DoModal () == IDOK)
	{
		m_infoArr [nItem] = dlg.m_infoData ;
	
		RefreshData(dlg.m_infoData, true);
		InsertListInfo();
	}
	AfxMessageBox("操作完成！");
	

	;
	
}

void XgnExtDataDlg::OnButtonDelete() 
{

	int nItem = 0;

	POSITION pos = this->m_listData .GetFirstSelectedItemPosition();
    if(pos == NULL)
	{
		TRACE0("No items were selected!\n");
		return;
	}
    else
	{
        nItem = m_listData.GetNextSelectedItem(pos);
	}

	if(nItem>= m_infoArr.GetSize ()) return ;

	FileExtendData::Remove(m_infoArr[nItem].m_nID );
	InsertListInfo();
}

void XgnExtDataDlg::OnOK() 
{

	GetSetReg reg;
	char ch[512];
	CString s ;
	s.Format("%d",m_dwTimer);
	strcpy(ch,s);
	DWORD ncb = sizeof(ch);
	DWORD typ = REG_SZ;
	if(reg.SetValue ("software\\xgn","RealUploadTimer",typ,(unsigned char *)ch,ncb) == ERROR_SUCCESS)
	{
	}
	CDialog::OnOK();
}

void XgnExtDataDlg::OnCancel() 
{
	
	
	CDialog::OnCancel();
}

void XgnExtDataDlg::OnButtonAdd() 
{

	XgnExtModify dlg(this);
	if(dlg.DoModal () == IDOK)
	{
	
		RefreshData(dlg.m_infoData, true);
		InsertListInfo();
	}
	AfxMessageBox("操作完成！");
	
	
}

void XgnExtDataDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	
	*pResult = 0;
}

void XgnExtDataDlg::InsertListInfo()
{
	m_infoArr.RemoveAll ();
	//IndicatorExtendData::GetIndicatorInfo(m_infoArr);
	GetCountValid(m_nValidCount);

	this->m_listData .DeleteAllItems ();
	int n = this->m_infoArr .GetSize ();
	for(int i = 0;i<n;i++)
	{
		CString s= m_infoArr[i].m_chDataName;
		m_listData.InsertItem(i, s);
		s = m_infoArr[i].m_chName ;
		s+= "(" ;
		s+= m_infoArr[i].m_chSubName ;
		s += ")  ";
		s += CTaiScreenParent::GetPeriodName(m_infoArr[i].m_nKlineKindCompute );
		m_listData.SetItemText(i,1,s );
		s.Format ("%d",m_infoArr[i].m_nID );
		m_listData.SetItemText(i,2,s);
		s = "日K线";
		if(m_infoArr[i].m_nKlineKind == 1)
			s = "5分钟K线";
		m_listData.SetItemText(i,3,s );
	}
}

int XgnExtDataDlg::GetAValidID()
{
	int n = this->m_infoArr .GetSize ();
	if(n<=0) return 0;

	int i = 0;
	int j = 0;
	while(true)
	{
		for( i = 0;i<n;i++)
		{
			if(this->m_infoArr [i].m_nID == j)
				break;
		}
		if(i==n)
			break;
		j++;
	}
	return j;

}

bool XgnExtDataDlg::IsValidID(int nID)
{
	int n = this->m_infoArr .GetSize ();
	int i = 0;
	for( i = 0;i<n;i++)
	{
		if(this->m_infoArr [i].m_nID == nID)
			return false;
	}
	return true;
}

void XgnExtDataDlg::RefreshData(IndicatorInfo &infoData, bool bClear,bool bTime )
{
#ifdef Ext_data_test
	return;
#endif

	//if(bClear == true)
	//{
	//	FileExtendData::Remove(infoData.m_nID );
	//}
	//FileExtendData::SetIndicatorInfo(infoData,infoData.m_nID);

	CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc ;
	MSG message;
	int nDays=60000;

	int m_totalCount=pDoc->m_sharesInformation.GetCount();
	long m_index=0;
	IndexDataInfo nIndexSave;
	nIndexSave.iLine = infoData.m_reserved ;
	nIndexSave.iKind  = -1 ;
	strcpy(nIndexSave.sIndex_name  , infoData.m_chName)  ;
	nIndexSave.bBuyCase  = 0;
	nIndexSave.iSunIndex = 0;
	nIndexSave.iDataType  = CTaiScreenParent::GetPeriodFoot(infoData.m_nKlineKindCompute );//infoData.m_reserved ;//nLKind[indexSv->iDataType]
	nIndexSave.iBeginFoot  = 0;
	memcpy(nIndexSave.fParam ,infoData.fParam ,8*4);
	if(bTime == true)
	{
		int nn = 1;
		if(infoData.m_nKlineKind ==5) nn = 48;
		nDays = CTaiScreenParent::GetCountValid(&nIndexSave,pDoc, 10) + this->m_nDaysLast*nn;
	}

	ARRAY_BE line;
	line.b = -1;
	line.e = -1;
	line.line = NULL;

	int nKlineDay = 0;
	Kline* pKlineDay = NULL;
	bool bDay = true;
	if(infoData.m_nKlineKind == 1)
		bDay = false;
	bool bOnlyCopy = false;
	if(infoData.m_nKlineKind == 1 && infoData.m_nKlineKindCompute ==1 ||
		infoData.m_nKlineKind == 5 && infoData.m_nKlineKindCompute ==5 )
		bOnlyCopy = true;

	EnableWindow(FALSE);
#ifndef _DEBUG
	try
	{
#endif
    for(int i=0;i<STOCKTYPE;i++)
	{
		int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			bool bSucc;
			Kline *pKline=NULL;
			CBuySellList l_BuySellList;
			pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;
            int GetDays=0;

				GetDays = CTaiScreenParent::ReadKLine(StockId,i, pKline, nDays,infoData.m_nKlineKindCompute ,&l_BuySellList, 48 ,TRUE);

//				GetDays = CTaiScreenParent::ReadKLine(StockId,i, pKline, nDays,infoData.m_nKlineKindCompute ,&l_BuySellList, 48 ,TRUE,&m_tmE,&m_tmB);

			if(GetDays<=0)
				continue;
			if(bOnlyCopy)
			{
				if(pKlineDay) delete [] pKlineDay;
				pKlineDay = new Kline[GetDays];
				nKlineDay = GetDays;
				memcpy(pKlineDay,pKline,nKlineDay*sizeof(Kline));
			}
			else
			{

					nKlineDay=CTaiScreenParent::ReadKLine(StockId,i, pKlineDay, nDays,infoData.m_nKlineKind ,&l_BuySellList, 48 ,TRUE);

//					nKlineDay=CTaiScreenParent::ReadKLine(StockId,i, pKlineDay, nDays,infoData.m_nKlineKind ,&l_BuySellList, 48 ,TRUE,&m_tmE,&m_tmB);
				if(pKlineDay == 0  || nKlineDay<= 0)
				{

					continue;
				}
			}

			line.line = new float[GetDays];
			float f =CTaiScreenParent::FormularComputeShares(StockId,i,GetDays,&nIndexSave,bSucc,pKline, &l_BuySellList ,false,&line);

			if(line.b <0 ||line.e <0 || bSucc == false)
			{
				CString ss;
				ss.Format ("b=%d, e = %d, succ = %d",line.b,line.e,(int)bSucc);

				continue;
			}

		
			{
			
				int nValid = 0;
				int m = line.b ;
				float fPre = 0;

			
				RecordIndicator* pData = new RecordIndicator[nKlineDay];
				for(int k = 0;k<nKlineDay;k++)
				{
					int t1 = FileExtendData::RegularTime(pKlineDay[k].day, bDay);
					pData[nValid].m_fValue  = fPre;
					pData[nValid].m_time   = t1;
					for(;m<GetDays;m++)
					{
						int t2 = FileExtendData::RegularTime(pKline[m].day, bDay);
						if(t2<t1) continue;
						if(t2==t1)
						{
						
							pData[nValid].m_fValue  = line.line[m];
							pData[nValid].m_time   = t1;
							fPre = pData[nValid].m_fValue;
						
							nValid++;
							m++;
							break;
						}
						break;
					}
				}

				//if(nValid>0)
				//{
				//	CString sSymbol = FormatStr(Cdat);
				//	IndicatorExtendData ext;
				//	ext.WriteData(pData,sSymbol,infoData.m_nID,nValid);
				//}

				delete [] pData;
			}

            if(!bSucc)
			{
			}
			if(pKline)
			{
			    delete pKline;      
				pKline=NULL;
			}

			if(line.line) delete [] line.line;
			//int FileExtendData::WriteData(RecordIndicator* pData,CString sSymbol,int nindicatorextendID,int nCount) 

			CTaiKlineFileHS::RemoveHs(l_BuySellList);
	//*******************************************************
			m_index++;
			int spos=100*m_index /m_totalCount   ;
			this->m_Progress.SetPos(spos);
	//**************************************************************
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
	}
	this->m_Progress.SetPos(0);
	if(pKlineDay) delete [] pKlineDay;
#ifndef _DEBUG
	}
	catch(...)
	{
	}
#endif

	EnableWindow();

}

CString XgnExtDataDlg::FormatStr(CReportData *pdt)
{
	ASSERT(pdt);
	int i = pdt->kind ;
	CString stockID = pdt->id ;
	DWORD dw = CMainFrame::m_taiShanDoc ->m_sharesCompute .GetMarketKind(i);
	CString s1 = "z";
	if(dw == SH_MARKET_EX)
		s1 = "s";
	CString sSymbol = s1+pdt->id;
	return sSymbol;

}
bool XgnExtDataDlg::GetLineBE(Kline* pKline,int nKline,ARRAY_BE & lineOut,int nID,CReportData* pdt) 
{
	if(!IsValidData( nID))
		return false;
#ifdef Client_Xgn
	InOutHeadData head;
	FileExtendData fl;
	int  nm = nID;
	fl.GetIndicatorInfo(head.m_info,nm);

#endif


	//FileExtendData ext;
	//RecordIndicator* pData = NULL;
	//CString sSymbol = FormatStr(pdt);
	//int nDays = -1;
	//int n = ext.ReadData(pData, sSymbol, nID,nDays) ;

	//int m = 0 ;
	//float fPre = 0;
	//int nFirst = -1;

	//bool bDay = true;
	//IndicatorInfo data;
	//bool b = ext.GetIndicatorInfo( data, nID);
	//if(b == false)
	//	return false;
	//if(data.m_nKlineKind != 5)
	//	bDay = false;

	//for(int i = lineOut.b ;i<nKline;i++)
	//{
	//	int t1 = FileExtendData::RegularTime(pKline[i].day, bDay);
	//	lineOut.line [i]  = fPre;
	//	for(;m<n;m++)
	//	{
	//		int t2 = FileExtendData::RegularTime(pData[m].m_time , bDay);
	//		if(t2<t1) continue;
	//		if(t2==t1)
	//		{
	//			if(nFirst == -1)
	//				nFirst = i;
	//			lineOut.line [i]  = pData[m].m_fValue;
	//			m++;
	//			fPre = lineOut.line [i] ;
	//			break;
	//		}
	//		break;
	//	}
	//	;
	//}
	//if(nFirst != -1)
	//	lineOut.b = nFirst;
	//else
	//	return false;

	//if(pData) delete [] pData;

	return true;
}

void XgnExtDataDlg::RefreshDataAuto(IndicatorInfo &infoData, int nCountValid, int nTimeNow,bool bCloseWork,CProgressDialog* pDlg)
{
#ifdef Ext_data_test
	return;
#endif
	CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc ;
	int nDays=nCountValid;
	if(bCloseWork == true)
	{
		if(infoData.m_nKlineKind == 1)
			nDays+=48;
	}
	if(nCountValid <= -1)
		nDays = 6000;

	int m_totalCount=pDoc->m_sharesInformation.GetCount();
	long m_index=0;
	IndexDataInfo nIndexSave = ChangeInfo(infoData);

	ARRAY_BE line;
	line.b = -1;
	line.e = -1;
	line.line = NULL;

	int nKlineDay = 0;
	static Kline* pKlineDay = NULL;
	static Kline *pKline=NULL;

	bool bDay = true;
	if(infoData.m_nKlineKind == 1)
		bDay = false;
	bool bOnlyCopy = false;
	if(infoData.m_nKlineKind == 1 && infoData.m_nKlineKindCompute ==1 ||
		infoData.m_nKlineKind == 5 && infoData.m_nKlineKindCompute ==5 )
		bOnlyCopy = true;

	InOutHeadData head;
	CString sFileName =GetTodayPath(infoData.m_nID);

	CFile * pFileIO = NULL;
	{
		//FileExtendData fl;
		//if(fl.GetIndicatorInfo(head.m_info ,infoData.m_nID))
		//{
		//	pFileIO = new CFile;
		//	if(!pFileIO->Open (sFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		//	{
		//		delete pFileIO;
		//		pFileIO = 0;
		//	}
		//	else
		//	{
		//		pFileIO->Seek (sizeof(InOutHeadData),CFile::begin);
		//	}
		//}

	}
	if(pFileIO == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	int nTimetToday = /*FileExtendData::RegularTime(CTime::GetCurrentTime ().GetTime(), true)*/0;

	static CBuySellList l_BuySellList;
	try
	{
    for(int i=0;i<STOCKTYPE;i++)
	{
		int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			bool bSucc;
			pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			if(Cdat->totv <=0 || Cdat->nowp <= 0.001 )
				continue;
			StockId =Cdat->id ;
            int GetDays=CTaiScreenParent::ReadKLine(StockId,i, pKline, nDays,infoData.m_nKlineKindCompute ,&l_BuySellList, 48 ,TRUE);

			if(GetDays<=0)
				continue;
			if(nTimetToday!=FileExtendData::RegularTime(pKline[GetDays-1].day, true))
				continue;

			if(bOnlyCopy)
			{
				if(pKlineDay) delete [] pKlineDay;
				pKlineDay = new Kline[GetDays];
				nKlineDay = GetDays;
				memcpy(pKlineDay,pKline,nKlineDay*sizeof(Kline));
			}
			else
			{
				nKlineDay=CTaiScreenParent::ReadKLine(StockId,i, pKlineDay, nDays,infoData.m_nKlineKind ,&l_BuySellList, 48 ,TRUE);
				if(pKlineDay == 0  || nKlineDay<= 0)
				{
					ASSERT(FALSE);
					continue;
				}
			}

			ARRAY_BE line;
			line.b = -1;
			line.e = -1;
			line.line = NULL;

			ARRAY_BE* pline = NULL;
			if(bCloseWork == true)
			{
				line.line = new float[GetDays];
				pline = &line;
			}


			float f =CTaiScreenParent::FormularComputeShares(StockId,i,GetDays,&nIndexSave,bSucc,pKline, &l_BuySellList ,false,pline);
			if(bSucc == false)
				continue;

			RecordIndicator extdata ;
			extdata.m_fValue = f;
			extdata.m_time = pKline[GetDays-1].day ;

			if(bCloseWork == true)
			{
				int nValid = 1;
				RecordIndicator extdata2[48] ;
				if(infoData.m_nKlineKind == 5)
				{
					extdata2[0].m_fValue = line.line [GetDays-1] ;
					extdata2[0].m_time = extdata.m_time;
				}
				else
				{
					if(line.b>=0 && GetDays - line.b>=48)
					{
						if(FileExtendData::RegularTime(pKline[GetDays-48].day, true) ==nTimetToday)
						{
							nValid = 48;
							for(int l = 0;l<48;l++)
							{
								extdata2[l].m_fValue = line.line[GetDays-48+l];
								extdata2[l].m_time  = pKline[GetDays-48+l].day ;
							}
						}
						else
							nValid = 0;
					}
					else
						nValid = 0;
				}
				CString sSymbol = FormatStr(Cdat);
				//IndicatorExtendData ext;
				//ext.WriteData(extdata2,sSymbol,infoData.m_nID,nValid);

				if(nValid>0)
				{
					int nRcd = nValid;
					CString sSymbol = FormatStr(Cdat);
					pFileIO->Write (sSymbol.GetBuffer (0),8);
					pFileIO->Write (&nRcd,4);
					pFileIO->Write (&extdata2,sizeof(RecordIndicator)*nRcd);
				}

		

			}

			if(line.line) delete [] line.line;


		}
//*********************************************************
		if(pDlg)
		{
			CString symbol = "";
			int spos=100* i /STOCKTYPE   ;
			pDlg->DisplayZqxx(symbol, spos); 
		}
//**************************************************************
	}

	if(pFileIO != NULL)
	{
		pFileIO->SeekToBegin ();
		pFileIO->Write (&head,sizeof(InOutHeadData));
		pFileIO->Close();
		delete pFileIO;

	    IZipPtr pIZip(__uuidof(Zip));
		CString s2 = sFileName;
		s2.Replace (".extio",".zip");
		pIZip->Init();
		pIZip->SetDes (s2.GetBuffer (0));
		pIZip->AddFile (sFileName.GetBuffer (0));
		pIZip->ZipFiles();

		CFile::Remove (sFileName);

	}


	}
	catch(...)
	{
	}
}

bool XgnExtDataDlg::GetCountValid(int *pOut)
{
	int n = m_infoArr.GetSize ();
	if(n<=0)
		IndicatorExtendData::GetIndicatorInfo(m_infoArr);
	n = m_infoArr.GetSize ();

	for(int i = 0;i<n;i++)
	{
		IndexDataInfo nIndexSave = ChangeInfo(m_infoArr[i]);
		pOut[i] = CTaiScreenParent::GetCountValid (&nIndexSave,CMainFrame::m_taiShanDoc ,10);
		if(pOut[i] <= -1)
			pOut[i] = 1000;
		ASSERT(pOut[i]>0);
	}

	return true;
}

IndexDataInfo XgnExtDataDlg::ChangeInfo(IndicatorInfo &infoData)
{
	IndexDataInfo nIndexSave;
	nIndexSave.iLine = infoData.m_reserved ;
	nIndexSave.iKind  = -1 ;
	strcpy(nIndexSave.sIndex_name  , infoData.m_chName)  ;
	nIndexSave.bBuyCase  = 0;
	nIndexSave.iSunIndex = 0;
	nIndexSave.iDataType  = CTaiScreenParent::GetPeriodFoot(infoData.m_nKlineKindCompute );//infoData.m_reserved ;//nLKind[indexSv->iDataType]
	nIndexSave.iBeginFoot  = 0;
	memcpy(nIndexSave.fParam ,infoData.fParam ,8*4);
	return nIndexSave;
}
void XgnExtDataDlg::DoCloseWork(CProgressDialog* pDlg)
{
#ifdef Ext_data_test
	return;
#endif
	pDlg->SetWindowText(_T("沪深指标扩展数据收盘作业"));

	GetCountValid(m_nValidCount);

	CString sPath = GetTodayPath(0,true);
	CreateDirectory(sPath.GetBuffer (0),NULL);

	//clear file
	//FileInOutData::RemoveFiles(sPath);

	int n = m_infoArr.GetSize ();
	for(int i = 0;i<n;i++)
	{
		RefreshDataAuto(m_infoArr[i], m_nValidCount[i], 48,true,pDlg);
	}
	if(n<=0) return;
	bool bOk = ReadWriteDownLoadInfoFile(-1, true);


	CString sLocalCurrentDirectory = GetCurrentPath()+"upload\\indicator";
	CString sServerCurrentDirectory = "/download/indicator";

	//FtpParaClass::FILE_COUNT_INFO *pInfo = new FtpParaClass::FILE_COUNT_INFO;
	//strcpy(pInfo[0].fileName,sPath);
	//pInfo[0].ufileFlag = FtpGetSend::DIRECTORYICON;
	//FtpGetSend::PrepareFileSend(pInfo, 1 , sLocalCurrentDirectory, sServerCurrentDirectory);
}


void XgnExtDataDlg::ComputeOnce()
{
#ifdef Ext_data_test
	return;
#endif
	CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc ;


	static bool bfirst = true;
	if(bfirst == true)
	{
		XgnExtDataDlg::m_dwTimer = GetTimegap();
		GetCountValid(m_nValidCount);
		bfirst = false;
	}
	static int g_nTime = 0;
	static DWORD g_dwTimerPre = 0;//
	DWORD dwTickCount = GetTickCount();
	if((dwTickCount - g_dwTimerPre)/1000<XgnExtDataDlg::m_dwTimer)
		if(g_dwTimerPre!=0)
			return;

	CTime tmNow = CTime::GetCurrentTime ();
	CTime tm930 (tmNow.GetYear (),tmNow.GetMonth(),tmNow.GetDay (),9,30,0);
	int n930 = tm930.GetTime ();
	int nNow = tmNow.GetTime ();
	if(nNow< n930 || nNow>n930+335*60)
		return;
	else	if(nNow> n930+122*60 && nNow<n930+208*60)
		return;

	static int g_nIndicatorCurr = 0;
	int nCountInd = m_infoArr.GetSize ();

	if(nCountInd<=0)
		return ;

	if(g_nIndicatorCurr == nCountInd)
	{
		;
		CString sLocalCurrentDirectory = GetCurrentPath()+"upload\\indicatorreal";
		CString sServerCurrentDirectory = "/download/indicatorreal";

		FtpParaClass::FILE_COUNT_INFO *pInfo = new FtpParaClass::FILE_COUNT_INFO[nCountInd+2];
		for(int m = 0;m<nCountInd;m++)
		{
			CString sFileName ;
			sFileName.Format("upload\\indicatorreal\\%dReal%3d.zip",m_infoArr[m].m_nID ,g_nTime);
			sFileName.Replace (" ","0");
			sFileName = GetCurrentPath()+sFileName;
			CString sPath = sFileName;
			strcpy(pInfo[m].fileName,sPath);
			pInfo[m].ufileFlag = FtpGetSend::FILEICON;
		}

		int nall = nCountInd;
		{
			bool bOk = ReadWriteDownLoadInfoFile(g_nTime, true);
			if(bOk)
			{
				CString sPath =GetCurrentPath()+ "upload\\indicatorreal\\CurrentTimes.data";
				strcpy(pInfo[nall].fileName,sPath);
				pInfo[nall].ufileFlag = FtpGetSend::FILEICON;
				nall++;
			}
		}

		//FtpGetSend::PrepareFileSend(pInfo, nall , sLocalCurrentDirectory, sServerCurrentDirectory);

	
		g_nIndicatorCurr = 0;
		g_nTime++;
		g_dwTimerPre = GetTickCount();
	}

	int nDays=m_nValidCount[g_nIndicatorCurr];
	if(nDays <= -1)
		nDays = 6000;

	IndicatorInfo infoData = m_infoArr[g_nIndicatorCurr];

	int m_totalCount=pDoc->m_sharesInformation.GetCount();
	long m_index=0;
	IndexDataInfo nIndexSave = ChangeInfo(infoData);

	int nKlineDay = 0;
	static Kline* pKlineDay = NULL;
	static Kline *pKline=NULL;

	bool bDay = true;
	if(infoData.m_nKlineKind == 1)
		bDay = false;
	bool bOnlyCopy = false;
	if(infoData.m_nKlineKind == 1 && infoData.m_nKlineKindCompute ==1 ||
		infoData.m_nKlineKind == 5 && infoData.m_nKlineKindCompute ==5 )
		bOnlyCopy = true;

	InOutHeadData head;
	static CFile * pFileIO = NULL;
	CString sFileName ;
	sFileName.Format("upload\\indicatorreal\\%dReal%3d.extio",infoData.m_nID ,g_nTime);
	sFileName.Replace (" ","0");
	sFileName = GetCurrentPath()+sFileName;

	if(pFileIO==NULL)
	{
		FileExtendData fl;
		if(fl.GetIndicatorInfo(head.m_info ,infoData.m_nID))
		{
			pFileIO = new CFile;
			if(!pFileIO->Open (sFileName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
			{
				delete pFileIO;
				pFileIO = 0;
			}
			else
			{
				pFileIO->Seek (sizeof(InOutHeadData),CFile::begin);
			}
		}

	}
	if(pFileIO == NULL )
	{
		ASSERT(FALSE);
		return;
	}

	int nTimetToday = FileExtendData::RegularTime(CTime::GetCurrentTime ().GetTime(), true);

	static CBuySellList l_BuySellList;

	static int i = 0;
	i%=STOCKTYPE;
	DWORD dwTick = GetTickCount();
    for(;i<STOCKTYPE;i++)
	{
		TRACE("III  =  %d",i);

		int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
		static int j = 0;
		if(j>=temp)
			j = 0;
		for(;j<temp;j++)
		{
			DWORD dwTick2 = GetTickCount();
			if(dwTick2-dwTick>500)
			{
				goto END_2;
			}
			TRACE("jjj  =  %d",j);

			CReportData *Cdat;
			CString StockId ; 
			bool bSucc;
			pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			if(Cdat->totv <=0 || Cdat->nowp <= 0.001 )
				continue;
			StockId =Cdat->id ;
            int GetDays=CTaiScreenParent::ReadKLine(StockId,i, pKline, nDays,infoData.m_nKlineKindCompute ,&l_BuySellList, 48 ,TRUE);

			if(GetDays<=0)
				continue;
			if(nTimetToday!=FileExtendData::RegularTime(pKline[GetDays-1].day, true))
				continue;

			if(bOnlyCopy)
			{
				if(pKlineDay) delete [] pKlineDay;
				pKlineDay = new Kline[GetDays];
				nKlineDay = GetDays;
				memcpy(pKlineDay,pKline,nKlineDay*sizeof(Kline));
			}
			else
			{
				nKlineDay=CTaiScreenParent::ReadKLine(StockId,i, pKlineDay, nDays,infoData.m_nKlineKind ,&l_BuySellList, 48 ,TRUE);
				if(pKlineDay == 0  || nKlineDay<= 0)
				{
					ASSERT(FALSE);
					continue;
				}
			}

			float f =CTaiScreenParent::FormularComputeShares(StockId,i,GetDays,&nIndexSave,bSucc,pKline, &l_BuySellList ,false);
			if(bSucc == false)
				continue;

			RecordIndicator extdata ;
			extdata.m_fValue = f;
			extdata.m_time = pKline[GetDays-1].day ;
			if(infoData.m_nKlineKind ==1 )
				extdata.m_time = Transfer5MinTimet(pKline[GetDays-1].day);

			//to write to file
			int nRcd = 1;
			CString sSymbol = FormatStr(Cdat);
			pFileIO->Write (sSymbol.GetBuffer (0),8);
			pFileIO->Write (&nRcd,4);
			pFileIO->Write (&extdata,sizeof(RecordIndicator)*nRcd);

		

		}
	}

	if(pFileIO != NULL)
	{
		pFileIO->SeekToBegin ();
		pFileIO->Write (&head,sizeof(InOutHeadData));
		pFileIO->Close();
		delete pFileIO;
		pFileIO = NULL;

	
	    IZipPtr pIZip(__uuidof(Zip));
		CString s2 = sFileName;
		s2.Replace (".extio",".zip");
		pIZip->Init();
		pIZip->SetDes (s2.GetBuffer (0));
		pIZip->AddFile (sFileName.GetBuffer (0));
		pIZip->ZipFiles();

		CFile::Remove (sFileName);
	}


	g_nIndicatorCurr++;

END_2:
	;
}

CString XgnExtDataDlg::GetTodayPath(int nID,bool bOnlyPath)
{
	CString sFileName ;
	CTime tmNow = CTime::GetCurrentTime ();
	CString st = tmNow.Format ("%Y%m%d");

	CString sCurrent = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory ;
	sCurrent.TrimRight ("\\");
	sCurrent.TrimRight ("/");
	sCurrent+="\\";
	if(bOnlyPath)
	{
		sFileName.Format("upload\\indicator\\%s",st);
	}
	else
	{
		sFileName.Format("upload\\indicator\\%s\\%3d.extio",st,nID);
		sFileName.Replace (" ","0");
	}
	sFileName = sCurrent+sFileName;

	return sFileName;

}

CString XgnExtDataDlg::GetCurrentPath()
{
	CString sCurrent = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory ;
	sCurrent.TrimRight ("\\");
	sCurrent.TrimRight ("/");
	sCurrent+="\\";
	return sCurrent;

}

bool XgnExtDataDlg::RemoveFiles(CString &sPath)
{
	return true;
}


bool XgnExtDataDlg::ReadWriteDownLoadInfoFile(int nTimesNow, bool bWrite)
{
	if(bWrite == true)
	{
		int nCountInd = m_infoArr.GetSize ();
		CFile fl;
		CString sPath =GetCurrentPath()+ "upload\\indicatorreal\\CurrentTimes.data";
		CString ss;
		ss = CTime::GetCurrentTime().Format ("%Y%m%d");
		if(nTimesNow == -1)
		{
			sPath =GetCurrentPath()+ "upload\\indicator\\";
			sPath+=ss;
			sPath+="\\";
			sPath+=ss;
			sPath+="CurrentIDs.data";
		}
		if(fl.Open (sPath,CFile::modeCreate|CFile::modeWrite))
		{
			fl.Write(ss.GetBuffer (8+1),8);
			ss.ReleaseBuffer ();
			ss.Format ("%4d",nTimesNow);
			fl.Write(ss.GetBuffer (4+1),4);
			ss.ReleaseBuffer ();

			ss.Format ("%4d",nCountInd);
			fl.Write(ss.GetBuffer (4+1),4);
			ss.ReleaseBuffer ();

			for(int ii = 0;ii<nCountInd;ii++)
			{
				ss.Format ("%4d",m_infoArr[ii].m_nID);
				fl.Write(ss.GetBuffer (4+1),4);
				ss.ReleaseBuffer ();
			}

			fl.Close ();
			return true;
		}
		else
			AfxMessageBox("无法打开文件 CurrentTimes.data ！");
	}
	return false;

}

int XgnExtDataDlg::Transfer5MinTimet(int tmIn)
{
	CTime tmNow (tmIn);
	CTime tmb (tmNow.GetYear (),tmNow.GetMonth (),tmNow.GetDay (),9,35,0);
	if(tmNow<tmb)
		return (int)tmb.GetTime ();

	CTimeSpan sp = tmNow-tmb;
	int nMinute = sp.GetMinutes ();

	int nRtn = nMinute%5==0?(nMinute/5):(nMinute/5+1);
	nRtn = nRtn*5*60 + tmb.GetTime ();

	return nRtn;
}

int XgnExtDataDlg::ReadDownLoadInfoFile(CString sFileName, int *&pData, CString &sDate,int& nTimesNow)
{
	int nCountInd = 0;
	CFile fl;
	CString sPath =sFileName;
	char ch[256];
	CString ss;
	nTimesNow = -1;
	if(fl.Open (sPath,CFile::modeRead|CFile::shareDenyNone))
	{
		fl.Read(ch,8);
		ch[8] = 0;
		sDate = ch;

		fl.Read(ch,4);
		ch[4] = 0;
		nTimesNow = atoi(ch);

		fl.Read(ch,4);
		ch[4] = 0;
		nCountInd = atoi(ch);
		if(nCountInd<=0)
			return 0;

		pData = new int[nCountInd];
		for(int ii = 0;ii<nCountInd;ii++)
		{
			int nR = fl.Read(ch,4);
			if(nR != 4)
				return 0;

			ch[4] = 0;
			pData[ii] = atoi(ch);
		}

		fl.Close ();
	}
	return nCountInd;

}

DWORD XgnExtDataDlg::GetTimegap()
{
	DWORD dwTimer = 300;
	GetSetReg reg;
	char ch[512];
	DWORD ncb = sizeof(ch);
	DWORD typ = REG_SZ;
	if(reg.GetValue ("software\\xgn","RealUploadTimer",typ,(unsigned char *)ch,ncb) == ERROR_SUCCESS)
	{
		dwTimer = atoi(ch);
		if(dwTimer>60000)
			dwTimer = 60000;
	}
	return dwTimer;
}

void XgnExtDataDlg::SetProductIdToExtdata(CString sFileName)
{

#ifndef Client_Xgn
	return;
#endif

	for(int j =0;j<sFileName.GetLength();j++)
	{
		if(sFileName[j]<'0' || sFileName[j]>'9')
		{
			ASSERT(FALSE);
			return;
		}
	}

	//{
	//	InOutHeadData head;

	//	FileExtendData fl;
	//	int  n = atoi(sFileName);
	//	fl.GetIndicatorInfo(head.m_info,n);
	//	head.m_info .m_nFileID = GetSerInt();

	//	head.m_info .m_nID = n;
	//	fl.SetIndicatorInfo(head.m_info,head.m_info .m_nID);
	//}

}

bool XgnExtDataDlg::IsValidData(int nID)
{
	return true;
#ifdef Client_Xgn
	FileExtendData ext;
	IndicatorInfo data;
	bool b = ext.GetIndicatorInfo( data, nID);
	if(b == false)
		return false;
	CString s = CMainFrame::m_taiShanDoc ->m_strSerial ;
	if(s.IsEmpty ())
		return false;

	CString s2 = "";
	for(int i = 0;i<s.GetLength ();i++)
	{
		if(s[i]>='0' &&s[i]<='9')
			s2+=s[i];
		if(s2.GetLength ()>=7)
			break;
	}

	if(s2.GetLength ()<7)
		for( i = 0;i<s.GetLength ()-7;i++)
		{
			s2+='1';
		}
	if(data .m_nFileID != atoi(s2))
		return false;
#endif
	return true;
}

void XgnExtDataDlg::OnButtonFreshAll() 
{
	UpdateData();

	int nItem = 0;

	int n = this->m_listData .GetItemCount ();
    for(int i = 0;i<n;i++)
	{
        nItem = i;
		if(nItem>= m_infoArr.GetSize ()) return ;
		RefreshData(m_infoArr[nItem], false,true);
	}
	AfxMessageBox("操作完成！");
	
}
int XgnExtDataDlg::GetSerInt() 
{
	CString s = CMainFrame::m_taiShanDoc ->m_strSerial ;
	CString s2 = "";
	for(int i = 0;i<s.GetLength ();i++)
	{
		if(s[i]>='0' &&s[i]<='9')
			s2+=s[i];
		if(s2.GetLength ()>=7)
			break;
	}

	if(s2.GetLength ()<7)
		for(int i = 0;i<s.GetLength ()-7;i++)
		{
			s2+='1';
		}
	return atoi(s2);
}
