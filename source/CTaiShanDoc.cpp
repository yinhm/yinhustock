// CTaiShanDoc.cpp : implementation of the CTaiShanDoc class
// Tel:13366898744
#include "stdafx.h"
#include <afxmt.h>
#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "resource.h"
#include "DownLoadDlg.h"
#include "WideDownLoadDlg.h"
#include "MyTreeView.h"
#include "CTaiShanApp.h"
#include "MainFrm.h"
#include "ProgressDialog.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "BGridCtrl.h"
#include "CSharesBigBillDlg.h"
#include "CTaiShanKlineShowView.h"
#include "DiaCopy.h"
#include "StructTaiShares.h"
#include "CTaiShanDoc.h"
#include "ReorganizeDayLineData.h"
#include "CheckData.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDrawLine.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineFileHS.h"
#include "CAlertSystem.h"
#include "CTaiShanTesting.h"
#include "GetSetReg.h"
#define ID_GPHQXS     5003
#define SYSTEMHEAD    1000

extern CTime g_timeUseEnd;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDD_DIALOG_PROGRESS             130

#define MLINEHEAD  16
#define MLINESMALLHEAD 20
#define MLINEBLK   48*25*3
#define MLINEBLKBYTECOUNT 48*25*3*32     

#ifdef __cplusplus
extern "C" {
  long ReadDog( void );
  long WriteDog( void );
  short DogAddr,DogBytes;
  void * DogData;
  }
#else
long ReadDog( void );
long WriteDog( void );
short DogAddr,DogBytes;
void * DogData;
#endif

#define MAXRAISE 3.0
extern class CTaiShanDoc *m_taiShanDoc;


BLOCKSTOCK::BLOCKSTOCK(CString string )
{
	this->blockname=string;

}
void BLOCKSTOCK::AddStockToTail(SharesSymbol* sharesymbol)
{
	this->stocklist.AddTail(sharesymbol);
}
void BLOCKSTOCK::AddStockToHead(SharesSymbol* sharesymbol)
{
	this->stocklist.AddHead(sharesymbol);
}
SharesSymbol* BLOCKSTOCK::GetAt(POSITION position)
{   SharesSymbol* result;
	result=this->stocklist.GetAt(position);
	return result;

}


IMPLEMENT_DYNCREATE(CTaiShanDoc, CDocument)

BEGIN_MESSAGE_MAP(CTaiShanDoc, CDocument)
	//{{AFX_MSG_MAP(CTaiShanDoc)
	ON_COMMAND(ID_OPEN_DOCUMENT, OnOpenDocument)
	ON_COMMAND(ID_TOOL_CLOSEWORK, OnToolClosework)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTaiShanDoc::m_bAppAuthorized = TRUE;

CTaiShanDoc::CTaiShanDoc()
{
	

		m_sShF10 = _T("");
		m_sSzF10 = _T("");
		m_sNews = _T("");
		m_BlockCalcTime=0;
		CWinApp* pApp = AfxGetApp();
		CString strSection       = "file_set_path";
		CString strStringItem  =  "shF10";
		m_sShF10 = pApp->GetProfileString(strSection, strStringItem);
		strStringItem  =  "szF10";
		m_sSzF10 = pApp->GetProfileString(strSection, strStringItem);
		strStringItem  =  "news_path";
		m_sNews = pApp->GetProfileString(strSection, strStringItem);


		m_infoInit.nCountIndex=1;
		m_infoInit.initIndex[0].yFloatBottom=0.25;
		m_infoInit.initIndex[1].yFloatBottom=0.25;
		m_infoInit.initIndex[2].yFloatBottom=0.25;
		m_infoInit.initIndex[3].yFloatBottom=0.25;

		m_infoInit.nCountMin1=3;
		m_infoInit.initMin1[0].yFloatBottom=0.25;
		m_infoInit.initMin1[1].yFloatBottom=0.25;
		m_infoInit.initMin1[2].yFloatBottom=0.25;
		m_infoInit.initMin1[3].yFloatBottom=0.25;

		
		m_infoInit.flag[0]=0;
		m_infoInit.flag[1]=1;
		m_infoInit.flag[2]=2;
		m_infoInit.flag[3]=1;
		m_infoInit.flag_dapan[0]=0;
		m_infoInit.flag_dapan[1]=1;
		m_infoInit.flag_dapan[2]=2;
		m_infoInit.flag_dapan[3]=1;

		strcpy(m_infoInit.initIndex[0].nameIndex,"MA");
		strcpy(m_infoInit.initIndex[1].nameIndex,"KDJ");
		strcpy(m_infoInit.initIndex[2].nameIndex,"KDJ");
		strcpy(m_infoInit.initIndex[3].nameIndex,"KDJ");

		m_propertyInitiate.daysOfDayKline = 0;   
		m_propertyInitiate.daysOfMin5Kline = 15;   
		m_propertyInitiate.daysOfManyKline = 50;

		m_propertyInitiate.bDoPowerSelectingStock = TRUE;     
		m_propertyInitiate.bDoVolPowering = TRUE;    
		
	
		
	memset(&m_fontdefault, 0, sizeof m_fontdefault);
	m_fontdefault.lfHeight = -16;
	lstrcpy(m_fontdefault.lfFaceName, _T("宋体"));
	m_fontdefault.lfOutPrecision = OUT_TT_PRECIS;
	m_fontdefault.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_fontdefault.lfQuality = PROOF_QUALITY;
	m_fontdefault.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
	

	m_nHave_Olddata=0; 

	m_nSharesSh=1;
	m_nSharesSz=1;
	m_nSharesSzEb=0;

	m_WsStock2000View=NULL;
	for(int j=0;j<4;j++)
	for(int i=0;i<30;i++)
		m_nColumnIndex[j][i]=-1;
	m_sharesSymbol = CSharesCompute::GetIndexSymbol(0);
	m_stkKind=0;
	m_NineShowView = NULL;
	m_nCurrentPage=0;
	m_nIsClose=FALSE;

	m_sharesCompute.SetMainDocument(this);
	InitSetp=0;
	m_taiViewF9 = NULL;
	m_taiViewF10 = NULL;
	m_pStockCjMxTj=NULL;
	m_pStockBigBillData=NULL;
    m_pDownLoadDlg=NULL;
	m_bOpenStockTypeDlg=FALSE;


	m_strUser=_T("");
	m_strCompany=_T("");
	m_strSerial=_T("");
	m_strCredit=_T("");
    m_CalcBlockCounts=10;

	for(int j=0;j<MAXCOLUME;j++)
	{
		m_nColumnWidth[j]=60;
	}

	m_nPreDisplayRow = 0;
	m_pStockDataShow = NULL;
}

CTaiShanDoc::~CTaiShanDoc()
{
	((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc = NULL;
	((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_MDIChildWnd = NULL;
	m_NineShowView = NULL;

	if (m_pStockDataShow)
	{
		GlobalUnlock((HGLOBAL)m_pStockDataShow);
		GlobalFree((HGLOBAL)m_pStockDataShow);
	}
}




void CTaiShanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	
	}
	else
	{
	
	}
}



#ifdef _DEBUG
void CTaiShanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaiShanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif 



BOOL CTaiShanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	try
	{
		CreateDirectory("DATA",NULL);
		CreateDirectory("html",NULL);
		CreateDirectory("bitmap",NULL);
		CreateDirectory("NEWS",NULL);
		CreateDirectory("NEWS\\shanghai",NULL);
		CreateDirectory("NEWS\\shenzhen",NULL);
		CreateDirectory("NEWS\\cj",NULL);
		CreateDirectory("板块数据",NULL);
		CreateDirectory("DATA\\indicator",NULL);
	}
	catch(...)
	{
	}
    m_bCloseReceiver=FALSE;
	m_bAppAuthorized=((CTaiShanApp*)AfxGetApp())->m_bAppAuthorized;
	CFile SysFile;
	if(SysFile.Open((LPCTSTR)g_xtsx_hz,CFile::modeRead,NULL))
	{
       int FileID;
	   SysFile.Read(&FileID,sizeof(int));
	   if(FileID!=65798813)
	   {
		   AfxMessageBox("打开文件错误，非系统属性文件！",MB_ICONSTOP);
		   SysFile.Close();
		   return FALSE;
	   }
	   try
	   {
    SysFile.Seek(16,CFile::begin);
    SysFile.Read(&m_colorArrayDefault[0],sizeof(COLORREF)*6*24);
	SysFile.Read(&m_colorArray,sizeof(COLORREF)*34);
	SysFile.Read(&m_fontdefault,sizeof(LOGFONT));
	SysFile.Seek(sizeof(LOGFONT),CFile::current);//
	SysFile.Read(&m_fontstr,sizeof(LOGFONT)*fontstructlength);
	SysFile.Read(&m_propertyInitiate,sizeof(PROPERTY_INIT));
    SysFile.Seek(40,CFile::current);//
	SysFile.Read(&m_syshqset,sizeof(SYSTEMHQSET));
	SysFile.Seek(40,CFile::current);//
	SysFile.Read(&m_systemdata,sizeof(SYSTEMDATA));
	SysFile.Seek(40,CFile::current);//
	SysFile.Read(&m_systemOption,sizeof(SYSTEM_OPTION));
	SysFile.Seek(40,CFile::current);//
	SysFile.Read(&m_dadanchoice,sizeof(ALERTCHOICE));
	SysFile.Seek(40,CFile::current);//
	SysFile.Read(&m_propertyInitiate2,sizeof(SUB_FIGUER_INIT_INFO));
	SysFile.Seek(40,CFile::current);//
	SysFile.Read(&m_SystemInitData,sizeof(SYSTEMINITDATA));
	SysFile.Seek(40,CFile::current);
	SysFile.Read(&m_Directory,sizeof(SETCURRENTDIRECTORY));
	SysFile.Seek(40,CFile::current);
    SysFile.Read(&m_colorArrayDefault[0],sizeof(COLORREF)*12*34);


    SysFile.Read(m_nColumnWidth,sizeof(int)*MAXCOLUME);
	for(int j=0;j<MAXCOLUME;j++)
	{
		if(m_nColumnWidth[j]<10 ||m_nColumnWidth[j]>1200)
		m_nColumnWidth[j]=60;
	}
	   }
	   catch(...)
	   {
	   }
    SysFile.Close();
	}


	m_spz.Open();	
	m_dyz.Open();	
	m_4or6=0;
	m_pPingCe=new CTaiShanTesting(this);
	if (m_pPingCe==NULL)
		AfxMessageBox("initial Ping Ce failed!");
//  begin


	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	pFm->m_taiShanDoc = this;


		
	CTaiKlineFileKLine::OpenAll();
	

	CTime tt;
	CString strTime;
	CString strUpdate;
	strUpdate="20011112";
	tt=CTime::GetCurrentTime();
	strTime=tt.Format("%Y%m%d");
	if(strcmp(strUpdate,strTime)==0||strcmp(strUpdate,strTime)==-1)
	{
		if(_access("transfered.dat",0)==-1)
		{	m_4or6=1;
			//CTaiKlineFileKLine::Transfer4To6();
			CFile f;
			int nt=1112;
			f.Open("transfered.dat",CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
			f.Write(&nt,sizeof(int));
			f.Close();			
		}
		m_4or6=1;			
		
	}
	else
	{
		m_4or6=0;
		//CTaiKlineFileKLine::Transfer4To6();		
	}


	CTaiKlineFileHS::OpenAll();

	time_t now;
	time(&now);
	m_week = *localtime(&now);


#ifdef WIDE_NET_VERSION
	Init_dat();
    Init_StockData(2);  
#else
	chk_date();
	Init_dat();
	Init_EveryDay();
#endif

 	::GetCurrentDirectory(MAX_PATH,m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	m_CurrentWorkDirectory.ReleaseBuffer();
	
    m_pTechniqueColumn=new TechniqueColumnInfo(this);
	
    if(m_SystemInitData.InitComplete!=INITDATAID)
	{
		strcpy(m_SystemInitData.StockTypeName,"板块股票");      
		strcpy(m_SystemInitData.ScreenStockName,"条件股票");   
		for(int i=0;i<30;i++)
		   m_SystemInitData.ColumnIndex[i]=-1;                 
		m_SystemInitData.ColumnType=0;             
		for(int i=0;i<30;i++)
		{
           if(i==0)
		   {
		      m_SystemInitData.ColumnWidth[i]=40;          
			  continue;
		   }
           if(i==1) 
		   {
		      m_SystemInitData.ColumnWidth[i]=70;        
			  continue;
		   }
           if(i==2) 
		   {
		      m_SystemInitData.ColumnWidth[i]=75;       
			  continue;
		   }
	       m_SystemInitData.ColumnWidth[i]=70;          
		}
		for(int i=0;i<MAXJISHUCOUNT;i++)
		{
		   memset(&m_SystemInitData.TechniqueColumn[i],0,sizeof(Index_Technique));
		   m_SystemInitData.TechniqueColumn[i].IsUse=FALSE;        
           m_SystemInitData.TechniqueColumn[i].nID =i+9000;
		}
	    m_SystemInitData.InitComplete=INITDATAID;
	}
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
	   if(m_SystemInitData.TechniqueColumn[i].nID != (UINT)(i+9000))
	   {
		   memset(&m_SystemInitData.TechniqueColumn[i],0,sizeof(Index_Technique));
		   m_SystemInitData.TechniqueColumn[i].IsUse=FALSE;        
		   m_SystemInitData.TechniqueColumn[i].nID =i+9000;
	   }
	}



	memset(&m_fontdefault, 0, sizeof m_fontdefault);
	m_fontdefault.lfHeight = 16;
	lstrcpy(m_fontdefault.lfFaceName, _T("宋体"));
	m_fontdefault.lfOutPrecision = OUT_TT_PRECIS;
	m_fontdefault.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_fontdefault.lfQuality = PROOF_QUALITY;
	m_fontdefault.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;


		char  blank[2]={' ',' '};             
     
		

	((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc=this;
	


	m_imagelist=new CImageList();
    m_imagelist->Create(16,16, ILC_MASK, 21, 2);

	int idb[21] = {IDB_BITMAP21,IDB_BITMAP2,IDB_BITMAP3,IDB_BITMAP4,IDB_BITMAP5,
		IDB_BITMAP6,IDB_BITMAP7,IDB_BITMAP8,IDB_BITMAP19,IDB_BITMAP20,
		IDB_BITMAP_110,IDB_BITMAP_111,IDB_BITMAP_112,IDB_BITMAP_113,IDB_BITMAP_114,
		IDB_BITMAP_131,IDB_BITMAP_132,IDB_BITMAP_133,IDB_BITMAP_134,IDB_BITMAP_114,
		IDB_BITMAP_135};
	for(int i=0;i<21;i++)
	{
		m_bitmap.LoadBitmap(idb[i]);
		m_imagelist->Add(&m_bitmap, (COLORREF)0xffffff);
		m_bitmap.DeleteObject();
	}


	long length=0;



	CFormularContent::ReadWriteIndex(this, 0, TRUE);
	CFormularContent::ReadWriteIndex(this, 1, TRUE);
	CFormularContent::ReadWriteIndex(this, 2, TRUE);



	POSITION pos = GetFirstViewPosition(); 
	while (pos != NULL) 
	{
      CView* pView = GetNextView(pos);
	  if(pView->IsKindOf( RUNTIME_CLASS(CTaiShanReportView)))
	  {
		    CTaiShanReportView *m_View;
			m_View=(CTaiShanReportView *)pView;
            m_View->InitGridCtrl();
			m_WsStock2000View=m_View;
	  }
	}   


	CAlertSystem::StartAlarm();

	pFm->m_dlgLeftPop .InsertData();

	return TRUE;
}
void CTaiShanDoc::InitTjxg()
{
	CFile  blocknamefile;
	char test[5];
	int length=0;
	char  blank[2]={' ',' '};
	BOOL istrue;
	if(blocknamefile.Open("板块数据/条件选股名.DAT",CFile::modeReadWrite))
	{
		char blocknamehead[4]={65,79,88,15};
		blocknamefile.Read(&test,4);
		istrue=true;
		blocknamefile.Read(&length,4);
		if(length>=100)
           return ; 
		if(istrue)
		{
		  for(int i=0;i<length;i++)
		  {
			CString name;
			char namechar[13];
			blocknamefile.Read(&namechar,12);
			blocknamefile.Read(&blank,2);
			name=namechar;
			BLOCKSTOCK *block=new BLOCKSTOCK(name);
			m_Tjxgblocklist.AddTail(block);
		  }
		}
		blocknamefile.Close();
	}
	else
	{
		blocknamefile.Open("板块数据/条件选股名.DAT",CFile::modeCreate);
        blocknamefile.Close(); 
	    blocknamefile.Open("板块数据/条件选股名.DAT",CFile::modeReadWrite);
		char blocknamehead[4]={65,79,88,15};
		length=0;
		blocknamefile.Write(&blocknamehead,4);
		blocknamefile.Write(&length,4);
		blocknamefile.Close();
	}

	POSITION position=m_Tjxgblocklist.GetHeadPosition();
	for(int i=0;i<length;i++)
	{
		BLOCKSTOCK * blocknow=m_Tjxgblocklist.GetNext(position);
		int stocklength;
		CFile stockfile;
		CString filepath="板块数据/"+blocknow->blockname;
    	if(stockfile.Open((LPCTSTR)filepath,CFile::modeReadWrite))
		{
			stockfile.Read(&stocklength,4);
			for(int j=0;j<stocklength;j++)
			{
				SharesSymbol *symbol=new SharesSymbol;
				stockfile.Read(symbol->sharesymbol,6);
                stockfile.Read(&symbol->nKind,4);
				symbol->sharesymbol[6]='\0';
				stockfile.Read(&blank,2);
				blocknow->stocklist.AddTail(symbol);
			}
			stockfile.Close();
		}
	}
}



void CTaiShanDoc::OnCloseDocument() 
{
	m_nIsClose=TRUE;

	CFile fl;

	if(m_WsStock2000View!=NULL)
	   m_WsStock2000View->KillTimer(ID_GPHQXS);

	if(	m_pDownLoadDlg)
		::SendMessage(	m_pDownLoadDlg->m_hWnd,WM_CLOSE,0,0); 

	if(	m_pStockCjMxTj)
		::SendMessage(	m_pStockCjMxTj->m_hWnd,WM_CLOSE,0,0); 
    if(m_pStockBigBillData)
		delete m_pStockBigBillData;
	int tmp=GetStocktime(0) ;     
	if(m_week.tm_wday!=0&&m_week.tm_wday!=6&&m_nANT[0]==239&&!m_bCloseWorkDone&&tmp>902)
	{
		int rtn = MessageBox(NULL,"还没有做收盘作业，是否进行收盘操作？","警告",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
           StockCloseWork();
		}
	}

	
    if(this->m_bInitDone)
		SaveFileData();      

    
	delete m_imagelist;

	POSITION pos = m_Tjxgblocklist.GetHeadPosition();
    while(pos!=NULL)
	{
        BLOCKSTOCK * m_pBlockStock;
        m_pBlockStock = m_Tjxgblocklist.GetNext(pos);
        
		POSITION m_pos=m_pBlockStock->stocklist.GetHeadPosition();
        while(m_pos)
		{
           SharesSymbol* m_pStockSymbol;
           m_pStockSymbol=m_pBlockStock->stocklist.GetNext(m_pos);
		   delete m_pStockSymbol;
		}
        m_pBlockStock->stocklist.RemoveAll();
		delete m_pBlockStock;
	}
	m_Tjxgblocklist.RemoveAll();



	int length=m_formuar_choose.GetSize();
	FreeKeyBoardAngel();


	m_spz.Close();		
	m_dyz.Close();
	if (m_pPingCe!=NULL)
	{
		delete m_pPingCe;
		m_pPingCe=NULL;
	}

	CFile SysFile;
	SysFile.Open((LPCTSTR)g_xtsx_hz,CFile::modeCreate|CFile::modeWrite,NULL);
	int FileID=65798813;
	SysFile.Write(&FileID,sizeof(int));
	SysFile.Seek(16,CFile::begin);
    SysFile.Write(&m_colorArrayDefault[0],sizeof(COLORREF)*6*24);
	SysFile.Write(&m_colorArray,sizeof(COLORREF)*34);
	SysFile.Write(&m_fontdefault,sizeof(LOGFONT));
	SysFile.Seek(sizeof(LOGFONT),CFile::current);//
	SysFile.Write(&m_fontstr,sizeof(LOGFONT)*fontstructlength);
	SysFile.Write(&m_propertyInitiate,sizeof(PROPERTY_INIT));
    SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_syshqset,sizeof(SYSTEMHQSET));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_systemdata,sizeof(SYSTEMDATA));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_systemOption,sizeof(SYSTEM_OPTION));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_dadanchoice,sizeof(ALERTCHOICE));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_propertyInitiate2,sizeof(SUB_FIGUER_INIT_INFO));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_SystemInitData,sizeof(SYSTEMINITDATA));
    SysFile.Seek(40,CFile::current);
	SysFile.Write(&m_Directory,sizeof(SETCURRENTDIRECTORY));
    SysFile.Seek(40,CFile::current);


	SysFile.Write(m_colorArrayDefault,sizeof(COLORREF)*12*34);

    SysFile.Write(m_nColumnWidth,sizeof(int)*MAXCOLUME);


    SysFile.Close();



	CFormularContent::ClearIndex(this, 0);
	CFormularContent::ClearIndex(this, 1);
	CFormularContent::ClearIndex(this, 2);


	CTaiKlineFileKLine::CloseAll();
	CTaiKlineFileHS::CloseAll();

	CDocument::OnCloseDocument();
}



void CTaiShanDoc::Init_dat()                                             
{
	FILE *inq;
	int shanghai_shenzhen,to_find;
	char str[80];
	inq=_fsopen( "Stocktime.dat", "rt", SH_DENYNO );
	if ( inq!=NULL )
	{
		shanghai_shenzhen = -1;
		while ( fscanf( inq, "%s", str )>0 )
		{
			to_find = -1;
			if ( !strcmpi( str, "[ShangHai]" )  )
				shanghai_shenzhen = 0;
			else if ( !strcmpi( str, "[ShenZhen]" ) )
				shanghai_shenzhen = 1;
			else if ( !strcmpi( str, "[ShenZhenEB]"))
				shanghai_shenzhen = 2;
			else if ( !strcmpi( str, "[ShangHai]" ) ||!strcmpi( str, "[ShenZhen]" ) ||!strcmpi( str, "[ShenZhenEB]" ))
				shanghai_shenzhen = -1;
			else if ( !strcmpi( str, "start_A_hour=" ) )
				to_find = 0;
			else if ( !strcmpi( str, "start_A_min=" ) )
				to_find = 1;
			else if ( !strcmpi( str, "end_A_hour=" ) )
				to_find = 2;
			else if ( !strcmpi( str, "end_A_min=" ) )
				to_find = 3;
			else if ( !strcmpi( str, "start_B_hour=" ) )
				to_find = 4;
			else if ( !strcmpi( str, "start_B_min=" ) )
				to_find = 5;
			else if ( !strcmpi( str, "end_B_hour=" ) )
				to_find = 6;
			else if ( !strcmpi( str, "end_B_min=" ) )
				to_find = 7;
			if ( to_find>=0 && shanghai_shenzhen>=0 )
			{
				fscanf( inq, "%s", str );
				if ( to_find==0 )
					m_nDel_Start_A_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==1 )
					m_nDel_Start_A_min[shanghai_shenzhen] = atoi( str );
				else if ( to_find==2 )
					m_nDel_End_A_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==3 )
					m_nDel_End_A_min[shanghai_shenzhen] = atoi( str );
				else if ( to_find==4 )
					m_nDel_Start_B_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==5 )
					m_nDel_Start_B_min[shanghai_shenzhen] = atoi( str );
				else if ( to_find==6 )
					m_nDel_End_B_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==7 )
					m_nDel_End_B_min[shanghai_shenzhen] = atoi( str );
			}
		}
		fclose(inq);
		m_nMaxMaxANT=0;
		for ( int i=0; i<3; i++ )
		{
			m_nANT[i]=0;
			m_nOldANT[i]=0;
			m_nDel_Half_ANT[i] = (m_nDel_End_A_hr[i]-m_nDel_Start_A_hr[i])*60-m_nDel_Start_A_min[i]+m_nDel_End_A_min[i]-1;
			if ( m_nDel_Start_B_hr[i]>=0 )
				m_nMaxANT[i] = m_nDel_Half_ANT[i]+((m_nDel_End_B_hr[i]-m_nDel_Start_B_hr[i])*60-m_nDel_Start_B_min[i]+m_nDel_End_B_min[i]);
			if ( m_nMaxANT[i]>m_nMaxMaxANT )
				m_nMaxMaxANT = m_nMaxANT[i];
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			m_nANT[i]=0;
			m_nOldANT[i]=0;
			m_nMaxANT[i]=239;
			m_nDel_Half_ANT[i]=119;
			m_nDel_Start_A_hr[i]=9;
			m_nDel_Start_A_min[i]=30;
			m_nDel_Start_B_hr[i]=13;
			m_nDel_Start_B_min[i]=0;
			m_nDel_End_A_hr[i]=11;
			m_nDel_End_A_min[i]=30;
			m_nDel_End_B_hr[i]=15;
			m_nDel_End_B_min[i]=0;
		}
			m_nMaxMaxANT=239;
	}
    for(int i=0 ;i<3 ;i++)           
	{
		this->m_lStartMinA[i]=m_nDel_Start_A_hr[i]*60 + m_nDel_Start_A_min[i];
		this->m_lEndMinA[i]=m_nDel_End_A_hr[i]*60 + m_nDel_End_A_min[i];
		this->m_lStartMinB[i]=m_nDel_Start_B_hr[i]*60 + m_nDel_Start_B_min[i];
		this->m_lEndMinB[i]=m_nDel_End_A_hr[i]*60 + m_nDel_End_A_min[i];
	}
	m_nMaxMaxANT++;
    Chk_Ystc();                                          
}

void CTaiShanDoc::Chk_Ystc()                                         
{
  FILE *in;
  long day;
		if( (in=_fsopen(g_realtime,"rb",SH_DENYNO))!=NULL)
		{  
			fseek(in,4,SEEK_SET);
			if ((fread(&day,4,1,in))>0)
			{
				if (m_lDay<=day)
					m_nHave_Olddata=1;
				else
				{
					m_nHave_Olddata=0;
				}
			}
			fclose(in);
		}
		else
			m_nHave_Olddata=0;
}


void CTaiShanDoc::chk_date()                    
{
        CTime m_Time = CTime::GetCurrentTime();
        m_lDay=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
}


int CTaiShanDoc::GetStocktime(int mode)       
{

  int tmp;
  struct tm* osTime = new tm;                            
  
  CTime m_Time = CTime::GetCurrentTime();
  osTime = m_Time.GetLocalTm( osTime );

  if (mode==0)
     tmp=osTime->tm_hour *60+ osTime->tm_min ;
  else 
  {                                                                     
	  if (osTime->tm_hour < m_nDel_Start_B_hr[mode-1] || m_nDel_Start_B_hr[mode-1]<0)
	  {   
           tmp=(osTime->tm_hour -m_nDel_Start_A_hr[mode-1])*60 + osTime->tm_min  - m_nDel_Start_A_min[mode-1] ;
           if (tmp>m_nDel_Half_ANT[mode-1])                                 
               tmp=m_nDel_Half_ANT[mode-1];
	  }
	  else 
	  {                                                                  
           tmp=(osTime->tm_hour - m_nDel_Start_B_hr[mode-1])*60+osTime->tm_min - m_nDel_Start_B_min[mode-1];
           if (tmp<0)
               tmp=m_nDel_Half_ANT[mode-1];                              
           else
		   {
               tmp+=(m_nDel_Half_ANT[mode-1]+1);
               if (tmp>m_nMaxANT[mode-1] )                                 
                   tmp=m_nMaxANT[mode-1] ;
		   }
	  }
  }
  m_nNowHour=osTime->tm_hour ;                                            
  m_nNowMin=osTime->tm_min ;                                                
  m_nNowSec=osTime->tm_sec ;                                              

  if (tmp<-88)                                    
    tmp=-88;

  delete osTime;

  return(tmp);
}





long CTaiShanDoc::GetStockDay(time_t time)
{
	if (time <= 0)
		return 0;

	int tmp;
	CTime m_Time = time;
	tmp = ((long)m_Time.GetYear()) * 10000L + (long)(m_Time.GetMonth()) * 100 + (long)(m_Time.GetDay());

	return (tmp);
}

void CTaiShanDoc::Init_EveryDay()
{
	int tmp;
	tmp = GetStocktime(0);

	try
	{
		if (m_week.tm_wday == 0 || m_week.tm_wday == 6)
		{
			Init_StockData(2);

			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->HqStock_Init();
		}
		else if (m_nHave_Olddata == 1)
		{
			Init_StockData(1);
		}
		else if (tmp >= m_lStartMinA[0] || tmp >= m_lStartMinA[1])
		{
			int rtn = MessageBox(NULL, "清除昨天分时数据吗？", "警告", MB_YESNO | MB_ICONWARNING);
			if (rtn == 6)
			{
				ClearRealData();
			}
			else
			{
				Init_StockData(2);
			}
		}
		else if ((tmp >= m_lStartMinA[0] - 5 ) || (tmp >= m_lStartMinA[1] - 5))
		{
			int rtn = MessageBox(NULL, "清除昨天分时数据吗？", "警告", MB_YESNO | MB_ICONWARNING);
			if (rtn == 6)
			{
				ClearRealData();
			}
			else
			{
				Init_StockData(2);
			}
		}
		else if (tmp < m_lStartMinA[0] - 5 || tmp < m_lStartMinA[1] - 5)
		{
			int rtn = MessageBox(NULL, "清除昨天分时数据吗？", "警告", MB_YESNO | MB_ICONWARNING);
			if (rtn == 6)
			{
				ClearRealData();
			}
			else
			{
				Init_StockData(2);
			}
		}
	}
	catch(...)
	{
	}

	CheckKind();
	m_sharesInformation.AddStocksIfEmpty();
}

void CTaiShanDoc::Init_StockData(int mode)
{
	LoadStockData(mode);

	if (mode == 2)
	{
		m_nANT[0] = m_nOldANT[0];
		m_nANT[1] = m_nOldANT[1];
		m_nANT[2] = m_nOldANT[2];
		m_bInitDone = FALSE;
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		m_nANT[i] = GetStocktime(i + 1);
		if (m_nANT[i] < 0)
		{
			m_nANT[i] = 0;
		}
	}

	//if (!m_bCloseReceiver)
	//{
	//	((CMainFrame*)(AfxGetApp()->m_pMainWnd))->HqStock_Init();
	//}

	m_bInitDone = TRUE;
}

void CTaiShanDoc::LoadStockData(int mode)
{
	CString path;
	GetCurrentDirectory(MAX_PATH, path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer();

	if (mode != 3)
	{
		m_sharesInformation.InitRealTimeData(path);
		m_ManagerStockTypeData.InitStockTypeData(path);
		InitChooseAndStockType();
	}

	switch(mode)
	{
	case 0:
	case 3:
		CreateFileData(mode); 
		m_sharesInformation.ClearRealDataMinute();
		CTaiKlineFileHS::m_fileHsSh->ZeroHsCountEach();
		CTaiKlineFileHS::m_fileHsSz->ZeroHsCountEach();
		CTaiTestTreeView::CheckNewsInfomationFileTime();
		break;
	case 1:
		LoadFileData(mode);
		break;
	case 2:
		LoadFileData(mode);
		break;
	}
}

void CTaiShanDoc::CreateFileData(int mode)
{
	FILE* fp;
	fp = _fsopen("news\\news.log", "w+b", SH_DENYNO);
	fclose(fp);

	CString FileName;
	FileName.Format("news\\shanghai\\%d.dat", m_lDay);
	fp = _fsopen(FileName.GetBuffer(0), "w+b", SH_DENYNO);
	fclose(fp);

	FileName.Format("news\\shenzhen\\%d.dat", m_lDay);
	fp = _fsopen(FileName.GetBuffer(0), "w+b", SH_DENYNO);
	fclose(fp);

	FileName.Format("news\\cj\\%d.dat", m_lDay);
	fp = _fsopen(FileName.GetBuffer(0), "w+b", SH_DENYNO);
	fclose(fp);

	REALDATA* RealFileHead;
	m_sharesInformation.SetRealDataHead(RealFileHead);
	RealFileHead->Day = m_lDay;
	RealFileHead->OldANT[0] = m_nANT[0];
	RealFileHead->OldANT[1] = m_nANT[1];
	RealFileHead->OldANT[2] = m_nANT[2];
	RealFileHead->CloseWorkDone = m_bCloseWorkDone = FALSE;
}

void CTaiShanDoc::LoadFileData(int mode)
{
	REALDATA* RealFileHead;
	RealFileHead = m_sharesInformation.GetRealDataHead();

	m_lDay = RealFileHead->Day;
	m_nOldANT[0] = RealFileHead->OldANT[0];
	m_nOldANT[1] = RealFileHead->OldANT[1];
	m_nOldANT[2] = RealFileHead->OldANT[2];
	m_bCloseWorkDone = RealFileHead->CloseWorkDone;

	if (m_week.tm_wday == 0 || m_week.tm_wday == 6)
	{
		m_nANT[0] = m_nOldANT[0];
		m_nANT[1] = m_nOldANT[1];
		m_nANT[2] = m_nOldANT[2];
	}

	CFile fl;
	if (fl.Open ("news\\news.log", CFile::modeCreate | CFile::modeNoTruncate))
	{
		fl.Close();
	}

	FILE* fp;
	fp = _fsopen("news\\news.log", "r+b", SH_DENYNO);
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp) && !ferror(fp))
	{
		CString FileName;
		char str[50] = "";
		fread(str, 50, 1, fp);
		FileName = str;
		if (strlen(str) > 0)
		{
			m_NewsFileNameMap[FileName] = FileName;
		}
	}

	fclose(fp);
}

void CTaiShanDoc::SaveFileData()
{
	REALDATA* RealFileHead;

	chk_date();
	m_sharesInformation.SetRealDataHead(RealFileHead);
	RealFileHead->Day = m_lDay;
	RealFileHead->OldANT[0] = m_nOldANT[0];
	RealFileHead->OldANT[1] = m_nOldANT[1];
	RealFileHead->CloseWorkDone = m_bCloseWorkDone;
	RealFileHead->FileExitDone = 12345678;
}

void CTaiShanDoc::ClearRealData()
{
	STOCKTYPEHEAD* pStockTypeHead;
	Init_StockData(0);
	m_ManagerStockTypeData.GetStockTypeHeadPoint(pStockTypeHead);
	pStockTypeHead->m_lLastTime = 0;
}

void CTaiShanDoc::CheckKind()
{
	for (int i = 0; i < STOCKTYPE; i++)
	{
		int temp = m_sharesInformation.GetStockTypeCount(i);
		for (int j = 0; j < temp; j++)
		{
			CReportData* Cdat;
			CString StockId;
			m_sharesInformation.GetStockItem(i, j, Cdat);
			if (Cdat == NULL)
				continue;

			StockId = Cdat->id;
			if (StockId.GetLength() < 4)
				continue;

			int nKind = m_sharesInformation.GetStockKind(Cdat->wMarket, Cdat->id);
			if (nKind != Cdat->kind)
			{
				m_sharesInformation.MoveStock(Cdat->id, Cdat->kind, nKind);
				j--;
			}

			//if (Cdat->kind == SHAG)
			//{
			//	if (StockId[0] == '5')
			//	{
			//		Cdat->kind = SHJIJIN;
			//	}
			//}
			//if (Cdat->kind == SZAG)
			//{
			//	if (StockId[0] == '1')
			//	{
			//		if (StockId[1] == '7' || StockId[1] == '8')
			//		{
			//			Cdat->kind = SZJIJIN;
			//		}
			//	}
			//}
		}
	}
}

void CTaiShanDoc::OnCalcHqDataProgress()
{
#ifndef WIDE_NET_VERSION
	int tmp;
	int i;

	this->m_sharesInformation.CalcIndexBuyAndSell();
	float rd_sh=0,rd_sz=0,rd_eb=0;
	if (m_nANT[0]>=0)
		rd_sh=OnCalcDpTidxdData(0) ;                       
	if (m_nANT[1]>=0)
		rd_sz=OnCalcDpTidxdData(1) ;                          
	if (m_nANT[2]>=0)
		rd_eb=OnCalcDpTidxdData(2) ;                      

	if(!m_bOpenStockTypeDlg&&m_BlockCalcTime%this->m_SystemInitData.BlockTime==0)
	{
		if(InitSetp>3) 
		{
			int temp=GetStocktime(1);
			if(temp<0) temp=0;

			if(this->m_bInitDone&&m_CalcBlockCounts!=0&&temp!=this->m_nANT[0])
			{
				m_ManagerStockTypeData.CalcMinuteRealTimeIndex(); 
				m_ManagerStockTypeData.CalcAllStockTypeData();  
				m_CalcBlockCounts--; 
			}
			else
				m_ManagerStockTypeData.CalcRealTimeStockTypeIndex();
		}
	}
	m_BlockCalcTime++;
	if(m_BlockCalcTime>=100)
		m_BlockCalcTime=0;


	// 状态栏显示指数
	CReportData *p1A0001, *p2A01, *p2D01;
	m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(0)).GetBuffer(0), p1A0001, SHZS);
	m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(1)).GetBuffer(0), p2A01, SZZS);
	CString seb = CSharesCompute::GetIndexSymbol(2);
	m_sharesInformation.Lookup(seb.GetBuffer(0), p2D01, 2);

	if (p1A0001 != NULL && p2A01 != NULL)
	{
		if (p1A0001->nowp == 0)
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0.5, 0.5, 0);
		}
		else if (p2D01 != NULL)
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
				p1A0001->nowp, p1A0001->nowp - p1A0001->ystc, p1A0001->totv, p1A0001->totp / 10000,
				p2A01->nowp, p2A01->nowp - p2A01->ystc, p2A01->totv, p2A01->totp / 100000000,
				p2D01->nowp, p2D01->nowp - p2D01->ystc, p2D01->totv, p2D01->totp / 100000000,
				rd_sh / 100, rd_sz / 100, rd_eb / 100);
		}
		else
		{
			((CMainFrame*)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
				p1A0001->nowp, p1A0001->nowp - p1A0001->ystc, p1A0001->totv, p1A0001->totp / 100000000,
				p2A01->nowp, p2A01->nowp - p2A01->ystc, p2A01->totv, p2A01->totp / 100000000,
				0, 0, 0, 0,
				rd_sh / 100, rd_sz / 100, rd_eb / 100);
		}
	}


	CTime m_Time = CTime::GetCurrentTime();
	if(m_Time.GetDayOfWeek()==1||m_Time.GetDayOfWeek()==7   )
		return;
	long temp=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
	if(!this->m_bInitDone||m_lDay<temp)     
	{
		tmp=GetStocktime(0) ;      
		if ((tmp > m_lStartMinA[0] - 5 )|| (tmp > m_lStartMinA[1] - 5 ) )  
		{
			STOCKTYPEHEAD *pStockTypeHead;
			this->m_bInitDone=TRUE;
			chk_date();
			Init_StockData(3);                         
			this->m_ManagerStockTypeData.GetStockTypeHeadPoint(pStockTypeHead);

			m_nOldANT[0]=0;
			m_nOldANT[1]=0;
			m_nOldANT[2]=0;
			chk_date();
			m_bCloseWorkDone=FALSE;

		}else 
		{
			this->m_bInitDone=FALSE;
			return ;
		}
	}

	// 自动收盘
	if (!m_bCloseWorkDone && m_systemOption.autoclose)
	{
		tmp = GetStocktime(0);
		if (tmp == 920)
		{
			m_bCloseWorkDone = TRUE;
			StockCloseWork();
		}
	}

	for(i =0 ;i<3;i++)                    
	{
		m_nANT[i]=GetStocktime(i+1) ;
		if(m_nANT[i] <0 )
			m_nANT[i]=0;
		if(m_nANT[i]==m_nOldANT[i])
			return;
		else
		{                                            
			m_nOldANT[i]=m_nANT[i]; 
		}
	}
#endif
}

float CTaiShanDoc::OnCalcDpTidxdData(int which_stk)                      
{
    int i,count;
    float tmp,tmp1;
    count=0;
	tmp=0;
	tmp1=0;
    if(which_stk!=0 && which_stk!=1&&which_stk!=2)
       return 0;
	m_sharesInformation.CalcUpDown (which_stk);

	Rsdn1 **Nidx2;
	Tidxd **Tidx2;
	this->m_sharesInformation.GetIndexRsdn(Nidx2); 
    this->m_sharesInformation.GetIndexTidxd(Tidx2); 

    Nidx2[which_stk][m_nANT[which_stk]].rsn=Nidx2[which_stk][m_nANT[which_stk]].dnn=0;
    m_nRN[which_stk]=m_nRN1[which_stk]=m_nRD[which_stk]=m_nDN[which_stk]=m_nDN1[which_stk]=m_nCalStk[which_stk]=0;
    int Start,End;

	int nKind[3] = {SHAG,SHBG,SHJIJIN};
	if(which_stk==1)
	{
		nKind[0]=SZAG;
		nKind[1]=SZBG;
		nKind[2]=SZJIJIN;

	}

    for(int k = 0;k<3;k++)
	{
		i = nKind[k];
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CString str;
			CReportData * Cdat;
			m_sharesInformation.GetStockItem(i, j, Cdat);
			if(Cdat==NULL)
				continue;

			if (Cdat->ystc>0 && Cdat->nowp>0)
			{
				tmp1=(float)(Cdat->nowp-Cdat->ystc)/(float)Cdat->ystc;
				if ( tmp1<MAXRAISE )         
				{
					tmp+=tmp1;
					count++;
				}

				if (Cdat->nowp>Cdat->ystc)
					Nidx2[which_stk][m_nANT[which_stk]].rsn++;
				else if (Cdat->nowp<Cdat->ystc)
					Nidx2[which_stk][m_nANT[which_stk]].dnn++;
			}
			if (Cdat->rdp=='3' || Cdat->rdp=='5')
				m_nRN[which_stk]++;
			else if (Cdat->rdp=='4' || Cdat->rdp=='6')
				m_nDN[which_stk]++;
			else if (Cdat->rdp=='1')
				m_nRN1[which_stk]++;
			else if (Cdat->rdp=='2')
				m_nDN1[which_stk]++;
			else if (Cdat->rdp=='0')
				m_nRD[which_stk]++;
			m_nCalStk[which_stk]++;
		}
	}
	if(count)
	  tmp=tmp/(float)count*10000;
    else
	  tmp=0;

	Tidx2[which_stk][m_nANT[which_stk]].sec5=(int)tmp;
    if(m_nCalStk[which_stk]>0)
	{
	   Tidx2[which_stk][m_nANT[which_stk]].rp=(short)((float)(m_nRN[which_stk]*2+m_nRN1[which_stk]*3+m_nRD[which_stk])*100.0/(float)(m_nCalStk[which_stk]*3));
	   Tidx2[which_stk][m_nANT[which_stk]].dp=(short)((float)(m_nDN[which_stk]*2+m_nDN1[which_stk]*3+m_nRD[which_stk])*100.0/(float)(m_nCalStk[which_stk]*3));
	}

    if(Tidx2[which_stk][m_nANT[which_stk]].dp==0)
		return 50;
	return 100.0f*Tidx2[which_stk][m_nANT[which_stk]].rp/(Tidx2[which_stk][m_nANT[which_stk]].dp +Tidx2[which_stk][m_nANT[which_stk]].rp);


}




void CTaiShanDoc::StockCloseWork()
{
	
	CDiaCopy *pdlg;
	CString filedes;
	CString filesrc;

      int lastant=0;
	long m_totalCount;
	CString m_zqdm;
    struct tm when; 

#ifdef TEST
	CTime timenow = CTime::GetCurrentTime();
	if(((CTaiShanApp *)AfxGetApp())->t.GetDay() != timenow.GetDay() )
	{
	  AfxMessageBox("非法时间会导致收盘错误。",MB_OK|MB_ICONSTOP);
	  return;
	}
#endif

	time_t now; 
	Rsdn1 **Nidx2;
	Tidxd **Tidx2;

	this->m_sharesInformation.GetIndexRsdn(Nidx2); 
    this->m_sharesInformation.GetIndexTidxd(Tidx2); 

	time( &now );

    when = *localtime( &now );
	if(when.tm_wday==0||when.tm_wday==6)
		return ;


	CProgressDialog *dlg;
	dlg=new CProgressDialog;
	dlg->Create(IDD_DIALOG_PROGRESS,NULL);
    dlg->ShowWindow(SW_SHOW); 
	dlg->m_DialogDown =FALSE;

	CButton *m_pButton;
	m_pButton =(CButton *)dlg->GetDlgItem(IDOK); 
    m_pButton->EnableWindow(FALSE) ;

	dlg->SetWindowText(_T("沪深日线收盘作业"));

	m_totalCount=m_sharesInformation.GetCount();
	if(m_totalCount<=0)
	{
		m_pButton->EnableWindow(TRUE) ;
		dlg->m_DialogDown =TRUE;
		dlg->ShowWindow(SW_HIDE);
		dlg->OnClose() ; 
		return;
	}
	long m_index=0;


	double ltpAll[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
    for(int i=0;i<STOCKTYPE;i++)
	{
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			Kline pIndexDay;
			KlineGeneral pStockDay;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;

	
			try
			{
			if(Cdat->pBaseInfo)
			{
				if(i!=SHZS && i!=SZZS)
				{
					if(i == SHBG|| i == SZBG)
						ltpAll[i] += Cdat->pBaseInfo ->Bg*100  ;
					else
						ltpAll[i] += Cdat->pBaseInfo ->ltAg*100 ;
				}
			}
			}
			catch(...)
			{
			}
		

			if(strcmp(Cdat->id,CSharesCompute::GetIndexSymbol(0))==0)                           
			{
				pIndexDay.day =now;
				pIndexDay.open =Cdat->opnp;
				pIndexDay.high=Cdat->higp ;
				pIndexDay.low =Cdat->lowp ;
				pIndexDay.close =Cdat->nowp;
				pIndexDay.vol =Cdat->totv;
 				pIndexDay.amount =Cdat->totp ;
		        pIndexDay.advance =Nidx2[0][m_nANT[0]].rsn;
			    pIndexDay.decline =Nidx2[0][m_nANT[0]].dnn;
			}else if(strcmp(Cdat->id,CSharesCompute::GetIndexSymbol(1))==0)
			{
				pIndexDay.day =now;
				pIndexDay.open =Cdat->opnp;
				pIndexDay.high=Cdat->higp ;
				pIndexDay.low =Cdat->lowp ;
				pIndexDay.close =Cdat->nowp;
				pIndexDay.vol =Cdat->totv;
				pIndexDay.amount =Cdat->totp ;
			    pIndexDay.advance =Nidx2[1][m_nANT[1]].rsn;
			    pIndexDay.decline =Nidx2[1][m_nANT[1]].dnn;
			}
			else
			{
				pStockDay.day =now;
				pStockDay.open =Cdat->opnp;
				pStockDay.high=Cdat->higp ;
				pStockDay.low =Cdat->lowp ;
				pStockDay.close =Cdat->nowp;
				pStockDay.vol =Cdat->totv;
				pStockDay.amount =Cdat->totp ;
				pStockDay.volPositive =Cdat->rvol;
				memcpy(&pIndexDay,&pStockDay,sizeof(Kline));
			}
           
			if(Cdat->kind!=SHZS&&Cdat->kind!=SZZS&&Cdat->kind!=EBZS)
			{
				if( Cdat->nowp !=0&&Cdat->opnp!=0&&Cdat->totp!=0&&Cdat->totv!=0)
				{
					try{
					   CTaiKlineFileKLine::WriteKLineS(StockId,Cdat->kind,&pIndexDay,1,0);  
					}
					catch(...)
					{
					}
				}
			}
			else
			{
				if( Cdat->nowp !=0&&Cdat->opnp!=0)
				{
					try{
					   CTaiKlineFileKLine::WriteKLineS(StockId,Cdat->kind,&pIndexDay,1,0);   
					}
					catch(...)
					{
					}
				}
			}
	
			m_index++;
			m_zqdm.Format ("%s(%s)",Cdat->name ,Cdat->id);
			int spos=100*m_index /(m_totalCount+1)   ;
			dlg->DisplayZqxx(m_zqdm, spos); 

	}
	}


	try
	{
	for(int i = 0;i<4;i=i+3)
	{
		for(int j=0;j<3;j++)
		{
			CReportData *Cdat;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			if(!Cdat->pBaseInfo)
			{
				BASEINFO *pBaseItem;
				if(!m_sharesInformation.AddBaseinfoPoint(Cdat->id,i,pBaseItem))              
				{
					AfxMessageBox("增加股票基本资料空间区域时出错！");
					continue;

				}
				Cdat->pBaseInfo=pBaseItem;
			}

			if(i==0 && j==0)
				Cdat->pBaseInfo ->ltAg = ltpAll[1] + ltpAll[2];
			else if(i==3 && j==0)
				Cdat->pBaseInfo ->ltAg = ltpAll[4] + ltpAll[5];
			else
				Cdat->pBaseInfo ->ltAg = ltpAll[i+j] ;
		}
	}
	}
	catch(...)
	{
	}



	dlg->SetWindowText(_T("沪深5分钟线收盘作业"));
	m_index=0;
    
	m_bCloseWorkDone=FALSE;
	Kline *MinKline;
	HGLOBAL	hMem = GlobalAlloc( GPTR, (48)* sizeof( Kline) ) ;
	LPVOID hp=GlobalLock(hMem);
    if( hp )
	    MinKline= (Kline *)hp;
	else
	    AfxMessageBox("can't alloc Buffers",MB_ICONSTOP) ; 
    for(int i=0;i<STOCKTYPE;i++)
	{
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId;
	        CTaiKlineTransferKline  nTransferKline;
			nTransferKline.m_bUseMinute1Data = true;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId=Cdat->id ;
			int nClount=0;
			nTransferKline.TransferKlineToday(StockId,Cdat->kind,MinKline,nClount,1);
			if(nClount == 48)
			{
		   CTaiKlineFileKLine::GetFilePointer(StockId,Cdat->kind,false)->WriteKLine(StockId,MinKline,48,0);   //增加K线数据
			}
	
			m_index++;
			m_zqdm.Format ("%s(%s)",Cdat->name ,Cdat->id);
			int spos=100 * m_index /(m_totalCount+1) ;
			dlg->DisplayZqxx(m_zqdm, spos); 

	}
	}
    GlobalUnlock((HGLOBAL)MinKline);       
	GlobalFree( (HGLOBAL)MinKline);

	m_bCloseWorkDone=TRUE;

	dlg->SetWindowText(_T("沪深历史回忆数据收盘作业"));
    CTaiKlineFileHS::DoCloseWorkHs(dlg);

	if(m_sharesInformation.GetCount()>100)
	  WriteStockInfoToFile();
    m_pButton->EnableWindow(TRUE) ;
	dlg->m_DialogDown =TRUE;
	dlg->ShowWindow(SW_HIDE);
	dlg->OnClose() ; 


	CTaiShanKlineShowView::OnDataChangAll(200);

	REALDATA *RealFileHead;          
    this->m_sharesInformation.SetRealDataHead(RealFileHead);
	RealFileHead->CloseWorkDone= m_bCloseWorkDone;
    this->m_sharesInformation.SaveRealDataToFile(RealFileHead,sizeof(REALDATA));



	CFile fl;
	if(fl.Open ("data\\DoPowerToday.txt",CFile::modeReadWrite))
	{
		fl.Close();
		CFile::Remove ("data\\DoPowerToday.txt");
		CStringArray sArr;
		Kline* pKline = NULL;
		for(int i=0;i<STOCKTYPE;i++)
		{
			int temp=m_sharesInformation.GetStockTypeCount(i);
			for(int j=0;j<temp;j++)
			{
				CReportData *Cdat;
				CString StockId;
				m_sharesInformation.GetStockItem(i,j,Cdat);
				if(Cdat==NULL)
					continue;
				StockId=Cdat->id ;
				int nClount=0;
			   nClount = CTaiKlineFileKLine::ReadKLineS(StockId,Cdat->kind,pKline,2);   

				if(nClount ==2)
				{
					if(int(Cdat->ystc*100)!= int( pKline[0].close*100) 
						&& Cdat->ystc>0 && Cdat->nowp >0)
						sArr.Add (StockId);
				}
			}
		}

		if(fl.Open ("data\\DoPowerToday.txt",CFile::modeReadWrite|CFile::modeCreate))
		{
			if(sArr.GetSize ()>0)
			{
				{
					for(int i = 0;i<sArr.GetSize ();i++)
					{
						CString ss = sArr[i];
						ss+="\r\n";
						fl.Write(ss.GetBuffer (0),ss.GetLength ());
					}
				}
			}
			fl.Close ();
		}
	}


    if(!m_systemOption.organizedata)
		return;


}
void CTaiShanDoc::DeleteContents() 
{

	CDocument::DeleteContents();
}


void* CTaiShanDoc::LookUpArray(Formu_Array1 &js, CString &str)
{
	int num=js.GetSize();
	for(int i=0;i<num;i++)
	{
		if(strcmp(js.ElementAt(i)->name,str)==0)
		{
			return js.ElementAt(i);
		}
	}
	return NULL;
}
void CTaiShanDoc::OnOpenDocument() 
{

}






void CTaiShanDoc::OnToolClosework() 
{

#ifdef TEST
	CTime t3 = CTime::GetCurrentTime();
    CTime t2(2001, 6, 30, 0, 0, 0 );
	if(t3 > t2)
	{
	  AfxMessageBox("使用期已过！",MB_ICONSTOP);
	  return;
	}
#endif

	int tmp=GetStocktime(0) ;   
    if(tmp<902)
		return;

    struct tm when; 
	time_t now; 
	time( &now );
    when = *localtime( &now );
	if(when.tm_wday==0||when.tm_wday==6)
		return ;
	if(!m_bCloseWorkDone)
    {
	   m_bCloseWorkDone=TRUE;
       StockCloseWork();
	}
	else
	{
		int rtn = MessageBox(NULL,"当天已收盘！是否再做收盘作业？","警告",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
			m_bCloseWorkDone=TRUE;
	        StockCloseWork();
		}
	}
}
void  CTaiShanDoc::StockNameConvert(char *StockName,char *pyjc)
{
	char *p;
	char sPy;
	int n;
	unsigned char c1;
	unsigned char c2;
	char str[10];
	int index=0;
	p=StockName;
	n=strlen(StockName);
	if (m_dyz.GetPy(StockName, pyjc))
		return;

	try
	{
	while(*p!='\n'&&n!=0)
	{
		if( (*p&0x80 )==0)
		{
			if(*p!=0x20)
			{
				str[index++]=*p;
			}
			p++;
			n--;
		}
		else
		{
			c1=*p; p++; n--;
			c2=*p; p++; n--;
			CString mzqmc;
			mzqmc.Format("%X%X",c1,c2); 
	
			if(mzqmc>=_T("B0A1")&&mzqmc<_T("B0C5"))
			   str[index++]='A';
		
			else if(mzqmc>=_T("B0C5")&&mzqmc<_T("B2C1"))
			   str[index++]='B';
			
			else if(mzqmc>=_T("B2C1")&&mzqmc<_T("B4EE"))
			   str[index++]='C';
		
			else if(mzqmc>=_T("B4EE")&&mzqmc<_T("B6EA"))
			   str[index++]='D';
		
			else if(mzqmc>=_T("B6EA")&&mzqmc<_T("B7A2"))
			   str[index++]='E';
				
			else if(mzqmc>=_T("B7A2")&&mzqmc<_T("B8C1"))
			   str[index++]='F';
			
			else if(mzqmc>=_T("B8C1")&&mzqmc<_T("B9FE"))
			   str[index++]='G';
			
			else if(mzqmc>=_T("B9FE")&&mzqmc<_T("BBF7"))
			   str[index++]='H';
			
			else if(mzqmc>=_T("BBF7")&&mzqmc<_T("BFA6"))
			   str[index++]='J';
			
			else if(mzqmc>=_T("BFA6")&&mzqmc<_T("C0AC"))
			   str[index++]='K';
		
			else if(mzqmc>=_T("C0AC")&&mzqmc<_T("C2E8"))
			   str[index++]='L';
		
			else if(mzqmc>=_T("C2E8")&&mzqmc<_T("C4C4"))
			   str[index++]='M';
			
			else if(mzqmc>=_T("C4C4")&&mzqmc<_T("C5B6"))
			   str[index++]='N';
			
			else if(mzqmc>=_T("C5B6")&&mzqmc<_T("C5BE"))
			   str[index++]='O';
				
			else if(mzqmc>=_T("C5BE")&&mzqmc<_T("C6DA"))
			   str[index++]='P';
			
			else if(mzqmc>=_T("C6DA")&&mzqmc<_T("C8BB"))
			   str[index++]='Q';
			
			else if(mzqmc>=_T("C8BB")&&mzqmc<_T("C8F6"))
			   str[index++]='R';
			
			else if(mzqmc>=_T("C8F6")&&mzqmc<_T("CBFA"))
			   str[index++]='S';
			
			else if(mzqmc>=_T("CBFA")&&mzqmc<_T("CDDA"))
			   str[index++]='T';
				
			else if(mzqmc>=_T("CDDA")&&mzqmc<_T("CEF4"))
			   str[index++]='W';
			
			else if(mzqmc>=_T("CEF4")&&mzqmc<_T("D1B9"))
			   str[index++]='X'; 
			
			else if(mzqmc>=_T("D1B9")&&mzqmc<_T("D4D1"))
			   str[index++]='Y';
		
			else if(mzqmc>=_T("D4D1")&&mzqmc<_T("D7F9"))
			   str[index++]='Z';
			else if(mzqmc==_T("A3C1"))
			   str[index++]='A';
			else if(mzqmc==_T("A3C2"))
			   str[index++]='B';
			else if(mzqmc>=_T("A3B0")&&mzqmc<=_T("A3B9"))
			{
           		str[index++]=c2-128;  
			}
			else if(m_spz.GetPy(p-2,sPy))
			{
				str[index++]=sPy;
			}
		}
	}
	}
	catch(...)
	{

	}
	str[index]='\0';
	strcpy(pyjc,str);
}
BOOL CTaiShanDoc::IdleProc( LONG lCount )
{
    if(InitSetp>=4) return FALSE;

    if(InitSetp==0)
	{
		InitializeKeyBoardAngle(TRUE);
		InitSetp=1;
	}
	else if(InitSetp==1)
	{
        m_sharesInformation.InitBaseInfoData( );
		InitSetp=2;
	}
	else if(InitSetp==2)
	{
        InitTjxg();      
		InitSetp=3;
	}else if(InitSetp==3)
	{

		InitSetp=4;
	}
	return TRUE;
}



void CTaiShanDoc::InitializeKeyBoardAngle(BOOL IsAddStock)
{	
	
   CString keystring[]={"61","62","63","64","65","66","69",
	                   "03","04","06",
           			   "81","82","83","84","85","86","87","89",
				
					   "11","12","13","14","15","16",
					   "21","22","23","24","25","26",
					   "31","32","33","34","35","36",
					   "41","42","43","44","45","46",
					   "51","52","53","54","55","56" ,
					   "01","02","05","10","08",
					   "0","1","2","3","4",
					   "5","6","7","8","9",
					   "71","72","73","07","09"
   };

  CString datastring[]={"沪A涨幅","沪B涨幅","深A涨幅","深B涨幅","沪券涨幅","深券涨幅","创股涨幅",
	  "上证领先","深证领先","自选股",
	  "沪A排名","沪B排名","深A排名", "深B排名",
	  "沪债排名","深债排名","板板排名","创股排名",
	  
	  "沪A量排","沪B量排","深A量排","深B量排","沪券量排","深券量排",
      "沪A金排","沪B金排","深A金排","深B金排","沪券金排","深券金排",
      "沪A量比","沪B量比","深A量比","深B量比","沪券量比","深券量比",
      "沪A震幅","沪B震幅","深A震幅","深B震幅","沪券震幅","深券震幅",
      "沪A委比","沪B委比","深A委比","深B委比","沪券委比","深券委比",
      "明细","分价","技术分析","个股资料","K线切换",
	  "分笔成交","1分钟线","5分钟线","15分钟线","30分钟线",
	  "60分钟线","日线","周线","月线","多日线",
	  "上交所公告","深交所公告","财经报道","沪/深指数切换","打开/隐藏画线工具"
  };

	int i;
    for(i=0;i<68;i++) 
	{
		DATA_KEYBOARD *keyboarddata=new DATA_KEYBOARD;
		strcpy(keyboarddata->key,keystring[i]);
		strcpy(keyboarddata->data,datastring[i]);
		keyboarddata->id=1;
		m_hotkey.Add(keyboarddata);
	}
    
	for(i=0;i<10;i++)
	{
		DATA_KEYBOARD *keyboarddata=new DATA_KEYBOARD;
		CString key,data;
		key.Format("%d",90+i);
		strcpy(keyboarddata->key,key);
		data=m_cybkname[i];
		strcpy(keyboarddata->data,data);
		keyboarddata->id=1;
		m_hotkey.Add(keyboarddata);
	}

    
	if(IsAddStock)
	{
		for(int nStockType=0;nStockType<STOCKTYPE;nStockType++)
		{
			for(i=0;i<m_sharesInformation.GetStockTypeCount(nStockType);i++)
			{
				CReportData *pDat1=NULL;
				m_sharesInformation.GetStockItem(nStockType,i,pDat1);
				if(pDat1)
				{
			
 					DATA_KEYBOARD *keyboarddata=new DATA_KEYBOARD;
					keyboarddata->id=0;
					strcpy(keyboarddata->key, pDat1->id);
					strcpy(keyboarddata->data,pDat1->name);
					keyboarddata->m_stkKind = pDat1->kind ;
					int nLen=m_keynumberdata.GetSize();
					int j;
					if(nLen>0)
					{
						for(j=0;j<nLen;j++)
						{
							if(strcmp(m_keynumberdata.GetAt(j)->key,keyboarddata->key)>=0)
								break;
						}
						if(j>=nLen)
							m_keynumberdata.Add(keyboarddata);
						else
							m_keynumberdata.InsertAt(j,keyboarddata);
					}
					else
						m_keynumberdata.Add(keyboarddata);
				
				
			
					DATA_KEYBOARD *KeyBoardHypy=new DATA_KEYBOARD;
					KeyBoardHypy->id=2;
					strcpy(KeyBoardHypy->key,pDat1->Gppyjc);
					strcpy(KeyBoardHypy->data,pDat1->id);
					KeyBoardHypy->m_stkKind = pDat1->kind ;
					nLen=m_keychardata.GetSize();
					if(nLen>0)
					{
						for(j=0;j<nLen;j++)
						{
							if(strcmp(m_keychardata.GetAt(j)->key,KeyBoardHypy->key)>=0)
								break;
						}
						if(j>=nLen)
							m_keychardata.Add(KeyBoardHypy);
						else
							m_keychardata.InsertAt(j,KeyBoardHypy);
					}
					else
						m_keychardata.Add(KeyBoardHypy);
				}
			}    
		}
	}

	for(int j=0;j<78;j++)       
	{
		DATA_KEYBOARD * hotkeydata;
		hotkeydata=m_hotkey.GetAt(j);
		int length=m_keynumberdata.GetSize();
		for(i=0;i<length;i++)
		{
			if(strcmp(m_keynumberdata.GetAt(i)->key,hotkeydata->key)>=0)
				break;
		}
		DATA_KEYBOARD *datatoinsert=new DATA_KEYBOARD;
		strcpy(datatoinsert->key,hotkeydata->key);
		strcpy(datatoinsert->data,hotkeydata->data);
		datatoinsert->id=hotkeydata->id;
		if(i>=length)
			m_keynumberdata.Add(datatoinsert);
		else
			m_keynumberdata.InsertAt(i,datatoinsert,1);
	}

	
	int lengthindex=this->m_formuar_index.GetSize();
	for(int i=0;i<lengthindex;i++)
	{
		CString name;
		CString sn;
		name=this->m_formuar_index.GetAt(i)->name;
		sn = name;
		DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
		if(name.GetLength()<=4)
			name+="指标";
		else
			name+="线";
		keytoadd->id=3;
		strcpy(keytoadd->key,sn);
		strcpy(keytoadd->data,name.GetBuffer(name.GetLength()));
		int keylength=m_keychardata.GetSize();
		int j;
		for(j=0;j<keylength;j++)
		{
			if(strcmp(m_keychardata.GetAt(j)->key,keytoadd->key)>=0)
				break;
		}
		m_keychardata.InsertAt(j,keytoadd);
	}

	const int hotcharlength=7;
	CString  hotcharkey[hotcharlength]={"QLDJ","HLDJ","QEDJ","HEDJ","TWR","BAR","K"};
	CString  hotchardata[hotcharlength]={"前量堆积","后量堆积","前额堆积","后额堆积","宝塔线","美国线","蜡烛线"};
	for(i=0;i<hotcharlength;i++)
	{
		char *keychar,*datachar;
		keychar=hotcharkey[i].GetBuffer(hotcharkey[i].GetLength());
		datachar=hotchardata[i].GetBuffer(hotchardata[i].GetLength());
		DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
		keytoadd->id=3;
		strcpy(keytoadd->key,keychar);
		strcpy(keytoadd->data,datachar);
		int keylength=m_keychardata.GetSize();
		int j;
		for(j=0;j<keylength;j++)
		{
			if(strcmp(m_keychardata.GetAt(j)->key,keytoadd->key)>=0)
				break;
		}
		m_keychardata.InsertAt(j,keytoadd);			
	}
}


void CTaiShanDoc::FreeKeyBoardAngel()
{
	int i;
 	int nLength=m_keychardata.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_keychardata.GetAt(i))
			delete m_keychardata.GetAt(i);
	}
	m_keychardata.RemoveAll();

	nLength=m_keynumberdata.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_keynumberdata.GetAt(i))
			delete m_keynumberdata.GetAt(i);
	}
	m_keynumberdata.RemoveAll();

	nLength=m_hotkey.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_hotkey.GetAt(i))
			delete m_hotkey.GetAt(i);
	}
	m_hotkey.RemoveAll();

	nLength=m_hotchar.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_hotchar.GetAt(i))
			delete m_hotchar.GetAt(i);
	}
	m_hotchar.RemoveAll();
}


void CTaiShanDoc::RefreshKeyBoardAngel(BOOL IsAddStock)
{
	FreeKeyBoardAngel();
	InitializeKeyBoardAngle(IsAddStock);
}


void CTaiShanDoc::AddStockToKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy)
{



	strStockCode.TrimLeft();
	strStockCode.TrimRight();
	strStockName.TrimLeft();
	strStockName.TrimRight();
	if(strStockCode.IsEmpty()||strStockName.IsEmpty())
		return;

	
	int nLength=m_keynumberdata.GetSize();
	int nResult;
	int i;
	for(i=0;i<nLength;i++)
	{
		nResult=strcmp(m_keynumberdata.GetAt(i)->key,strStockCode);
		if(nResult==0 && m_keynumberdata.GetAt(i)->m_stkKind == stkKind)		
			return;
		else 
		{
			if (nResult>0)
				break;
		}
	}

	DATA_KEYBOARD *DataToAdd=new DATA_KEYBOARD;
    
	DataToAdd->id=0;
	strcpy(DataToAdd->key,strStockCode);
	strcpy(DataToAdd->data,strStockName);
	DataToAdd->m_stkKind = stkKind;
	m_keynumberdata.InsertAt(i,DataToAdd);
	
	DataToAdd=new DATA_KEYBOARD;
	DataToAdd->id=2;
	strcpy(DataToAdd->key,strStockHypy);
	strcpy(DataToAdd->data,strStockCode);
	DataToAdd->m_stkKind = stkKind;
    nLength=m_keychardata.GetSize();
	for( i=0;i<nLength;i++)
	{
		if(strcmp(m_keychardata.GetAt(i)->key,strStockHypy)>=0 )
			break;
	}
	m_keychardata.InsertAt(i,DataToAdd);
}


BOOL CTaiShanDoc::DeleteStockFromKeyboard(CString strStockCode,int stkKind, CString strStockName, CString strStockHypy)
{
	
	if(strStockCode.IsEmpty()||strStockName.IsEmpty())
		return FALSE;
    BOOL bResult=FALSE; 
	int nLength=m_keynumberdata.GetSize();
	int i;
	for(i=0;i<nLength;i++)
	{
		if(strcmp(m_keynumberdata.GetAt(i)->key,strStockCode)==0 && m_keynumberdata.GetAt(i)->m_stkKind == stkKind)
	
				break;
	}
	if(i<nLength)
	{
		m_keynumberdata.RemoveAt(i);
		bResult=TRUE;
	}

    nLength=m_keychardata.GetSize();
	for( i=0;i<nLength;i++)
	{
	
			if(strcmp(m_keychardata.GetAt(i)->data,strStockCode)==0 && m_keychardata.GetAt(i)->m_stkKind == stkKind)
				break;
	}
	if(i<nLength)
	{
		m_keychardata.RemoveAt(i);
		bResult=TRUE;
	}
	return bResult;
}


BOOL CTaiShanDoc::ModifyStockOfKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy)
{



	if(strStockCode.IsEmpty()||strStockName.IsEmpty())
		return FALSE;
    BOOL bResult=FALSE; 
	DATA_KEYBOARD *DataToAdd=NULL;
	int nLength=m_keynumberdata.GetSize();
	int i;
	for( i=0;i<nLength;i++)
	{
		if(strcmp(m_keynumberdata.GetAt(i)->key,strStockCode)==0 && m_keynumberdata.GetAt(i)->m_stkKind == stkKind)
			break;
	}
	if(i<nLength)
	{
		DataToAdd=new DATA_KEYBOARD;
		m_keynumberdata.RemoveAt(i);
		DataToAdd->id=0;
		strcpy(DataToAdd->key,strStockCode);
		strcpy(DataToAdd->data,strStockName);
		DataToAdd->m_stkKind  = stkKind;
		m_keynumberdata.InsertAt(i,DataToAdd);
		bResult=TRUE;
	}

    nLength=m_keychardata.GetSize();
	for( i=0;i<nLength;i++)
	{
		if(strcmp(m_keychardata.GetAt(i)->data,strStockCode)==0&& m_keychardata.GetAt(i)->m_stkKind == stkKind)
			break;
	}
	if(i<nLength)
	{
		m_keychardata.RemoveAt(i);
		DataToAdd=new DATA_KEYBOARD;
		DataToAdd->id=2;
		strcpy(DataToAdd->key,strStockHypy);
		strcpy(DataToAdd->data,strStockCode);
		DataToAdd->m_stkKind  = stkKind;
		m_keychardata.InsertAt(i,DataToAdd);
		bResult=TRUE;
	}
	return bResult;
}

void CTaiShanDoc::InitChooseAndStockType()
{
	m_ManagerStockTypeData.InitStockPoint();
	if(m_ManagerStockTypeData.GetStockTypeCounts()!=this->m_sharesInformation.GetStockTypeCount(10))
	{
       m_sharesInformation.DeleteAllStockFromStockType();
	}
	if(this->m_sharesInformation.GetStockTypeCount(10)==0)
	  m_ManagerStockTypeData.InitStockTypePoint();                 
}
void CTaiShanDoc::InitStockFiveDaysVolumn()
{
    for(int i=0;i<10;i++)
	{
		if(i==0||i==3||i==8)
			continue;
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CString StockId;
			CReportData *Cdat;
			Kline *pKline=NULL;
			CBuySellList l_BuySellList;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;
			Cdat->volume5=0;
			int count=0;
		   count=CTaiKlineFileKLine::ReadKLineS(StockId,i,pKline,5); 
			if(count==0)
				continue;
			for(int k=0;k<count;k++)
              Cdat->volume5 += pKline[k].vol;
			Cdat->volume5 =Cdat->volume5/count;

			if(pKline)
			delete []pKline;
		}
	}
}
void CTaiShanDoc::InitFiveDaysVolumnForStock(PCdat1 &pCdat)
{
	CString StockId=pCdat->id;
	Kline *pKline=NULL;
	CBuySellList l_BuySellList;

	if(pCdat->kind==0||pCdat->kind==3||pCdat->kind==8)
      return;
	StockId =pCdat->id ;
	pCdat->volume5=0;
	int count=0;
   count=CTaiKlineFileKLine::ReadKLineS(StockId,pCdat->kind,pKline,5); 
	if(count==0)
	{
       return;
	}
	for(int k=0;k<count;k++)
      pCdat->volume5 += pKline[k].vol;
	pCdat->volume5 =pCdat->volume5/count;

	if(pKline)
	delete []pKline;
}

// 保存股票代码表
void CTaiShanDoc::WriteStockInfoToFile()
{
	int StockCount[STOCKTYPE];
	memset(StockCount, 0, sizeof(int) * STOCKTYPE);

	FILE* fp= _fsopen("StockName.dat", "w+b", SH_DENYNO);
	fclose(fp);

	fp = _fsopen("StockName.dat", "r+b", SH_DENYNO);
	for (int i = 0; i < STOCKTYPE; i++)
	{
		StockCount[i] = m_sharesInformation.GetStockTypeCount(i);
	}
	fseek(fp, 0, SEEK_SET);
	fwrite(&StockCount[0], 4, STOCKTYPE, fp);

	for (int i = 0; i < STOCKTYPE; i++)
	{
		int temp = m_sharesInformation.GetStockTypeCount(i);
		for (int j = 0; j < temp; j++)
		{
			CReportData* Cdat;
			m_sharesInformation.GetStockItem(i, j, Cdat);
			if (Cdat == NULL)
				continue;

			fwrite(&Cdat->kind, 1, 1 ,fp);
			fwrite(Cdat->id, 1, 6 ,fp);
			fwrite(Cdat->name, 1, 8, fp);
			fwrite(Cdat->Gppyjc, 1, 4, fp);
			fwrite(&Cdat->sel, 1, 4, fp);
			char ch1 = ' ';
			fwrite(&ch1, 1, 1, fp);
		}
	}

	fclose(fp);
}

int CTaiShanDoc::GetStockKind(CString strKind)
{
	 if(strKind=="00")
		return SHZS;
	 else if(strKind=="01")
		return SHAG;
	 else if(strKind=="02")
		return SHBG;
	 else if(strKind=="03")
		return SZZS;
	 else if(strKind=="04")
		return SZAG;
	 else if(strKind=="05")
		return SZBG;
	 else if(strKind=="06")
		return SHZQ;
	 else if(strKind=="07")
		return SZZQ;
	 else if(strKind=="08")
	    return SHJIJIN;	 //   
	 else if(strKind=="09")
	    return SZJIJIN;                             
	 else if(strKind=="10")
	    return EBZS;	
	 else if(strKind=="11")
	    return EBAG;                             
	 else if(strKind=="12")
	    return STKTYPE;                       
	 else if(strKind=="20")
	    return CHOOSESTK;
	 return -1;
}
CString CTaiShanDoc::GetStockKindString(int nKind)
{
	switch(nKind)
	{
	case SHZS:                        
		return "00";
		break;
    case SHAG:                           
		return "01";
		break;
	case SHBG:                           
		return "02";
		break;
	case SZZS:                         
		return "03";
		break;
	case SZAG:                           
		return "04";
		break;
	case SZBG:                            
		return "05";
		break;
	case SHZQ:                             
		return "06";
		break;
	case SZZQ:                           
		return "07";
		break;
	case SHJIJIN:                            
		return "08";
		break;
	case SZJIJIN:                           
		return "09";
		break;
	case EBZS:                              
		return "10";
		break;
	case EBAG:                          
		return "11";
		break;
	case STKTYPE:                        
		return "12";
		break;
	case CHOOSESTK:                       
		return "20";
		break;
	}
	return "";
}
#ifdef WIDE_NET_VERSION
void CTaiShanDoc::WideNetTransactSystemRunPara(RCV_SYSTEMRUN_STRUCTEx *pSystemRunPara)
{
	m_nANT[0]=pSystemRunPara->m_btBargainingTime[0];              
	m_nANT[1]=pSystemRunPara->m_btBargainingTime[1];              
	m_nANT[2]=pSystemRunPara->m_btBargainingTime[2];
	m_bCloseWorkDone = pSystemRunPara->m_btCloseWork;
	this->m_bInitDone= pSystemRunPara->m_btTodayInitialize;
	CReportData *p1A0001,*p2A01,*p2D01;
    m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(0)).GetBuffer(0),p1A0001,SHZS);
    m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(1)).GetBuffer(0),p2A01,SZZS);
	CString seb = CSharesCompute::GetIndexSymbol(2);
    m_sharesInformation.Lookup(seb.GetBuffer (0),p2D01,EBZS);
	if(p1A0001!=NULL)
	{
		p1A0001->nowp=pSystemRunPara->m_fNewIndex[0];
		p1A0001->totp=pSystemRunPara->m_fAmount[0];
		p1A0001->totv=pSystemRunPara->m_fVolume[0];
		p1A0001->m_Kdata1[m_nANT[0]].Price=pSystemRunPara->m_fNewIndex[0];
		p1A0001->m_Kdata1[m_nANT[0]].Amount=pSystemRunPara->m_fAmount[0];
		p1A0001->m_Kdata1[m_nANT[0]].Volume=pSystemRunPara->m_fVolume[0];
	}	
	if(p2A01!=NULL)
	{
		p2A01->nowp=pSystemRunPara->m_fNewIndex[1];
		p2A01->totp=pSystemRunPara->m_fAmount[1];
		p2A01->totv=pSystemRunPara->m_fVolume[1];
		p2A01->m_Kdata1[m_nANT[1]].Price=pSystemRunPara->m_fNewIndex[1];
		p2A01->m_Kdata1[m_nANT[1]].Amount=pSystemRunPara->m_fAmount[1];
		p2A01->m_Kdata1[m_nANT[1]].Volume=pSystemRunPara->m_fVolume[1];
	}	
	if(p2D01!=NULL)
	{
		p2D01->nowp=pSystemRunPara->m_fNewIndex[2];
		p2D01->totp=pSystemRunPara->m_fAmount[2];
		p2D01->totv=pSystemRunPara->m_fVolume[2];
		p2D01->m_Kdata1[m_nANT[2]].Price=pSystemRunPara->m_fNewIndex[2];
		p2D01->m_Kdata1[m_nANT[2]].Amount=pSystemRunPara->m_fAmount[2];
		p2D01->m_Kdata1[m_nANT[2]].Volume=pSystemRunPara->m_fVolume[2];
	}	

	if(p1A0001!=NULL&&p2A01!=NULL)
	{
		if(p1A0001->nowp==0)
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0.5 ,0.5 ,0);
		else if(p2D01!=NULL)
		{
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			p1A0001->nowp ,p1A0001->nowp - p1A0001->ystc ,p1A0001->totv,p1A0001->totp/10000,
			p2A01->nowp ,p2A01->nowp - p2A01->ystc,p2A01->totv ,p2A01->totp/100000000,
			p2D01->nowp ,p2D01->nowp - p2D01->ystc,p2D01->totv ,p2D01->totp/100000000,
			pSystemRunPara->m_fRedGreenArmy[0]/100,pSystemRunPara->m_fRedGreenArmy[1]/100,pSystemRunPara->m_fRedGreenArmy[2]/100);
		}else
		{
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			p1A0001->nowp ,p1A0001->nowp - p1A0001->ystc ,p1A0001->totv,p1A0001->totp/100000000,
			p2A01->nowp ,p2A01->nowp - p2A01->ystc,p2A01->totv ,p2A01->totp/100000000,
			0,0,0,0,
			pSystemRunPara->m_fRedGreenArmy[0]/100,pSystemRunPara->m_fRedGreenArmy[1]/100,pSystemRunPara->m_fRedGreenArmy[2]/100);
		}
	}
	if(m_lDay>=pSystemRunPara->m_BargainingDate)
        return ;
	if(m_lDay<pSystemRunPara->m_BargainingDate&&pSystemRunPara->m_btTodayInitialize)
	{
	    SOCKET_ID m_SocketID;
		m_SocketID.iFlag=-1;
		m_SocketID.hWnd=NULL;
		CMainFrame * pMainFrm=((CMainFrame *)(AfxGetApp()->m_pMainWnd));
		if(pMainFrm!=NULL&&pMainFrm->m_pClientTransmitManageWnd)
		{
			if(m_SocketID.iFlag==-1)
			{
				 m_SocketID=pMainFrm->m_pClientTransmitManageWnd->RegisterID(NULL);
			}
			if(m_SocketID.iFlag!=-1)
			{
				 TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
                 m_TransPackageStruct.m_TransmitType=InitStockCodeList;
			     m_TransPackageStruct.m_dwTransmitStockType=QBGP;
				 pMainFrm->m_pClientTransmitManageWnd->GetMarketData(&m_TransPackageStruct,0,m_SocketID,TRUE);
			}
		}
        m_lDay=pSystemRunPara->m_BargainingDate;  
	}
}
void CTaiShanDoc::WideNetTransactIndexAttribute(RCV_INDEXATTRIBUTE_STRUCTEx *pIndexAttribute ,int nParaCounts, int nIndexType)
{
	Rsdn1 **Nidx2;
	Tidxd **Tidx2;
	this->m_sharesInformation.GetIndexRsdn(Nidx2); 
    this->m_sharesInformation.GetIndexTidxd(Tidx2); 
    for(int i=0;i<nParaCounts;i++)
	{
        int MinuteCount=this->m_sharesCompute.GetStockMinute( pIndexAttribute->m_time,nIndexType);
        Nidx2[nIndexType][MinuteCount].dnn=pIndexAttribute[i].dnn;
        Nidx2[nIndexType][MinuteCount].rsn=pIndexAttribute[i].rsn;
        Tidx2[nIndexType][MinuteCount].sec5=pIndexAttribute[i].sec5;
        Tidx2[nIndexType][MinuteCount].dp=pIndexAttribute[i].dp;
        Tidx2[nIndexType][MinuteCount].rp=pIndexAttribute[i].rp;
	}
}
void CTaiShanDoc::WideNetInitMarketBargainingData(RCV_STKLABEL_STRUCTEx *pStkLabel,int nStkLabelCounts)
{
	this->m_sharesInformation.ClearAllRealTimeMarketData();
	this->RefreshKeyBoardAngel(FALSE);
	for(int i=0;i<nStkLabelCounts;i++)
	{
		CReportData *pCdat;
		int nKind=m_sharesInformation.GetStockKind(pStkLabel[i].m_wMarket,pStkLabel[i].m_szLabel);
		if (this->m_sharesInformation.Lookup(pStkLabel[i].m_szLabel,pCdat,nKind) != TRUE)     
		{
			if(strlen(pStkLabel[i].m_szLabel)==6||strlen(pStkLabel[i].m_szLabel)==4)
			{
				if(nKind>=0&&nKind<=10)
				{
					if(!m_sharesInformation.InsertItem(pStkLabel[i].m_szLabel,pCdat,nKind))
						continue;
     
					strcpy(pCdat->name ,pStkLabel[i].m_szName);
					strcpy(pCdat->id ,pStkLabel[i].m_szLabel );
					this->StockNameConvert(pCdat->name,pCdat->Gppyjc );
                    pCdat->kind=nKind;
					AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);

					BASEINFO *m_pStockBase;
					if(m_sharesInformation.LookupBase(pCdat->id,pCdat->kind,m_pStockBase))
					{
					   pCdat->pBaseInfo=m_pStockBase;
					}
				}
			}
		} 
	}
	InitChooseAndStockType();
	m_WsStock2000View->ChangeToPage(0);
}
void CTaiShanDoc::WideNetInitMarketBargainingData2(RCV_STKLABEL2_STRUCTEx *pStkLabel,int nStkLabelCounts)
{
	for(int i=0;i<nStkLabelCounts;i++)
	{
		CReportData *pCdat;
		int nKind=m_sharesInformation.GetStockKind(pStkLabel[i].m_wMarket,pStkLabel[i].m_szLabel);
		if (this->m_sharesInformation.Lookup(pStkLabel[i].m_szLabel,pCdat,nKind) == TRUE)    
		{
            pCdat->ystc=pStkLabel[i].m_fLastClose;
			pCdat->volume5=pStkLabel[i].m_fAverageVolumn;
		} 
	}
}
#endif
