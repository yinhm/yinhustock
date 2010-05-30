// ExportData.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ExportData.h"
#include "mainfrm.h"
#include "CTaiKlineFileKLine.h"
#include "ImportData.h"
#include "BrowseForFolder.h"
#include "SBDestination.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////





IMPLEMENT_DYNCREATE(CExportData, CPropertyPage)

CExportData::CExportData() : CPropertyPage(CExportData::IDD)
{
	//{{AFX_DATA_INIT(CExportData)
	m_AssignPeriod = -1;
	m_NumberOfDays = 0;
	m_install_start = 0;
	m_install_end = 0;
	//}}AFX_DATA_INIT
	m_nShijianflag=0;
	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;
	pDoc = CMainFrame::m_taiShanDoc ;
}

CExportData::~CExportData()
{
}

void CExportData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportData)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, 1022, m_button1);
	DDX_Control(pDX, IDC_LIST, m_listfw);
	DDX_Control(pDX, 1008, m_NumberOfD);
	DDX_Control(pDX, 1011, m_DataFormat);
	DDX_Control(pDX, 1017, m_contSpin);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressYield);
	DDX_Control(pDX, 1021, m_destination);
	DDX_Radio(pDX, 1013, m_AssignPeriod);
	DDX_Text(pDX, 1008, m_NumberOfDays);
	DDV_MinMaxInt(pDX, m_NumberOfDays, 1, 6400);
	DDX_DateTimeCtrl(pDX, 1012, m_install_start);
	DDX_DateTimeCtrl(pDX, 1019, m_install_end);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportData, CPropertyPage)
	//{{AFX_MSG_MAP(CExportData)
	ON_BN_CLICKED(1013, Onzjts)
	ON_BN_CLICKED(1014, Onsdxz)
	ON_BN_CLICKED(1022, OnButtonDestination)
	ON_BN_CLICKED(IDCANCEL, OnButtonBeginproduce)
	ON_CBN_SELCHANGE(1011, OnSelchangeDatetFormat)
	ON_CBN_SELCHANGE(1015, OnSelchangeFiletype)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_DRAWL,drwalist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CExportData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_contSpin.SetRange32 (1,6400);
	
	m_AssignPeriod=0;
    GetDlgItem(1012)->EnableWindow(FALSE);
	GetDlgItem(1019)->EnableWindow(FALSE);

	m_destination.ReplaceSel("StockData.day");
    m_ProgressYield.SetPos(0);
	m_NumberOfDays=1;
  
    ((CComboBox*)GetDlgItem(1015))->SetCurSel(0);
	((CComboBox*)GetDlgItem(1011))->SetCurSel(0);
	
	m_install_start=CTime(1999,1,1,1,1,1);
	m_install_end=CTime::GetCurrentTime();

	LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	myitem.iImage=6;

	m_listfw.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	CString stockgroup[]={"上海","深圳","创业"};
	for(int i=0;i<3;i++)
	{ 
		if(i==2)
			myitem.iImage=9;
		myitem.iItem=i;
	    myitem.pszText=(LPTSTR)stockgroup[i].GetBuffer(stockgroup[i].GetLength());
	    m_listfw.InsertItem(&myitem);
		if(i==2)
		   m_stockrange.Add(FALSE);
		else
		   m_stockrange.Add(TRUE);
	}

	UpdateData(FALSE);
	return TRUE;  
}

void CExportData::OnOK() 
{

	
	CPropertyPage::OnOK();
}

void CExportData::Onzjts() 
{

    GetDlgItem(1012)->EnableWindow(FALSE);
	GetDlgItem(1019)->EnableWindow(FALSE);
	GetDlgItem(1008)->EnableWindow(TRUE);
    m_nShijianflag=0;
}

void CExportData::Onsdxz() 
{

    GetDlgItem(1008)->EnableWindow(FALSE);
	GetDlgItem(1012)->EnableWindow(TRUE);
    GetDlgItem(1019)->EnableWindow(TRUE);
	m_nShijianflag=1;
}

void CExportData::OnButtonDestination() 
{
	
    CString filename;
	UpdateData(TRUE);
	CString FileName;
	if((((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)||(((CComboBox*)GetDlgItem(1015))->GetCurSel()==0&&((CComboBox*)GetDlgItem(1011))->GetCurSel()==3))
	{
        CSBDestination sb(m_hWnd, IDS_BFF_TITLE);
	    sb.SetFlags(BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT);
	    sb.SetInitialSelection(m_news);
	    if (sb.SelectFolder())
		{
		   m_news.TrimRight (" ");
		   m_news.TrimRight ("\r\n");
		   m_news.TrimRight (";");
		   m_news.TrimRight (" ");

		   CString sNews = sb.GetSelectedFolder();
			m_news = sNews;
		   
		}
	}

	else
	{
		CString DefExt="*.day";
		CString NewFileName;  

		CString Filter="日线数据(*.day)|*.day||"; 
		if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==0&&((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
		{
			NewFileName="StockData.day";
		}
		if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==1&&((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
		{
			DefExt="*.min";
			NewFileName="StockData.min";
			Filter="5分钟数据(*.min)|*.min||";  //
		}
		if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
		{
			DefExt="*.txt";
			NewFileName="StockData.txt";
			Filter="文本格式数据(*.txt)|*.txt||";  //
		}
		if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==2)
		{
			DefExt="*.xls";
			NewFileName="StockData.xls";
			Filter="xls格式数据(*.xls)|*.xls||";  //
		}
		FileName=pDoc->m_Directory.CurrentDir;
		FileName+=NewFileName;
		CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,
			(LPCTSTR)FileName,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
			(LPCSTR)Filter,this);
		if( bSaveFileDialog.DoModal() ==IDOK )
		{
			filename=bSaveFileDialog.GetPathName();
			CString fname=bSaveFileDialog.GetFileName();
			CString NewFileN=filename;
			NewFileN.TrimRight(fname);
			strcpy(pDoc->m_Directory.CurrentDir,NewFileN.GetBuffer(0));
		
		}

	}
	if((((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)||(((CComboBox*)GetDlgItem(1015))->GetCurSel()==0&&((CComboBox*)GetDlgItem(1011))->GetCurSel()==3))
		{
			filename=m_news;
		
		}
	m_destination.SetSel(0,-1);
	m_destination.ReplaceSel(filename);
	UpdateData(FALSE);

}

void CExportData::OnButtonBeginproduce() 
{

    if(!UpdateData(TRUE))
		return;
	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;
	int length=m_stockrange.GetSize();
	for (int i=0;i<length;i++)
		if(m_stockrange.GetAt(i)==TRUE)
			if(i==0)
			{
				m_Shanghai=TRUE;
			}
			else if (i==1)
			{
				m_Shenzhen=TRUE;
			}
     if (m_Shanghai==FALSE&&m_Shenzhen==FALSE)
	 {
		 AfxMessageBox("请选择范围 ");
		 return;
	 }
	if(m_install_start<=0||m_install_end<=0)
	{
		AfxMessageBox("时间设置错误!");
		return;
	}
	 int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	 int EndDate  =atoi(m_install_end.Format("%Y%m%d"));
     
	 if(m_nShijianflag==1)
	 {
		 if(BeginDate>EndDate)
		 {
        	 AfxMessageBox("时间颠倒,重新输入!");
			 return;
  		 }
		 if(BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") ))
		 {
        	AfxMessageBox("没有数据!",MB_ICONASTERISK);
			return;
		 }
	 }
	SetCapture();
	BeginWaitCursor();

	::SetCurrentDirectory( pDoc->m_CurrentWorkDirectory );
     

	CString DestFileName;
	m_destination.GetWindowText(DestFileName);

	if(  DestFileName.IsEmpty() )
	{
		AfxMessageBox("文件名不应为空!");
		return;
	}

	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
	{
		if(((CComboBox *)GetDlgItem(1015))->GetCurSel()!=2)
		{
			int IndexChar=DestFileName.ReverseFind('\\');
			int IndexPoint=DestFileName.ReverseFind('.');
			if(IndexPoint>IndexChar)
			    DestFileName=DestFileName.Left(IndexPoint);
		
		if(((CComboBox *)GetDlgItem(1015))->GetCurSel()==0)
		    DestFileName+=".day";
		else 
			DestFileName+=".min";
		m_destination.SetWindowText(DestFileName);
		}
	}
	if( ((CComboBox *)GetDlgItem(1015))->GetCurSel()==0)
	   {
		   
		   switch( ((CComboBox*)GetDlgItem(1011))->GetCurSel() )
		   {
		   case 0:
               ProduceDayKline(DestFileName,BeginDate,EndDate,0);
			   break;
           case 1:
			   ProduceDayKline(DestFileName,BeginDate,EndDate,3);
			   break;

		   }
	   }
	   if ( ((CComboBox*)GetDlgItem(1015))->GetCurSel()==1)
	   {
		   switch( ((CComboBox*)GetDlgItem(1011))->GetCurSel() )
		   {
		   case 0:
			   ProduceMinuteData(0);
			   break;
		   case 1:
			   ProduceMinuteData(1);
			   break;
		   case 2:
			   ProduceMinuteData(2);
			   break;
		   }
	   }
	    		   
	  if( ((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)
	   {
	    if(this->IsDirectoryExist()) 
           return;  

	    m_ProgressYield.SetRange(0,8);
        m_ProgressYield.SetPos(0);
		for(int i=0;i<=8;i++)
		{
			m_ProgressYield.OffsetPos(1);
			int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
			for(int j=0;j<temp;j++)
			{
				CString str;
				CReportData * Cdat;
				pDoc->m_sharesInformation.GetStockItem(i, j, Cdat);
				if(Cdat==NULL)
					continue;
		        CString s = Cdat->id;
				//  add begin46
				int nKind=Cdat->kind;
				//  add end46
                if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
				    ProduceMin(s.GetBuffer(0),0,nKind);
				if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
			        ProduceMin(s.GetBuffer(0),1,nKind);
				if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==2)
					ProduceMin(s.GetBuffer(0),2,nKind);
			}
		}
		m_ProgressYield.SetPos(8);
		m_ProgressYield.SetPos(0);
	

	}
					   
	ReleaseCapture();
	EndWaitCursor();
   
	   
}



void CExportData::ProduceDayKline(CString DestFileName,int BeginDate,int EndDate,int bFormat)
{
	
	if(bFormat!=3)
	
		if( IsFileExists( DestFileName ) )
			if( AfxMessageBox("存在同名文件,是否覆盖？",MB_ICONQUESTION|MB_YESNO )==IDNO )
			    return;

	if(bFormat==3)
	{
	    DestFileName.TrimRight("\\");
		DestFileName+="\\";
		CFileFind finder;
		if (!finder.FindFile(DestFileName+"*.*")) 
		{
			if(!(::CreateDirectory(DestFileName,NULL)))
			   {
				   AfxMessageBox("路径中包括错误的路径,请重新设置!");
				   m_destination.SetFocus();
				   return;
			   }
		}

	}
	
    CString FloppyFileName;
	if (DestFileName.Mid(0,3)=="a:\\"||DestFileName.Mid(0,3)=="b:\\"||DestFileName.Mid(0,3)=="A:\\"||DestFileName.Mid(0,3)=="B:\\")
		
		{
			if(bFormat==3)
			{
				AfxMessageBox("文件过大,请导到硬盘!",MB_ICONQUESTION);
				m_destination.SetFocus();
				return;
			}
		    FloppyFileName=DestFileName;
		    DestFileName="C:\\Source.day";
		}

	if(bFormat!=3)
       if(!m_DestFile.Open(DestFileName,CFile::modeWrite|CFile::modeCreate))
	   {
           AfxMessageBox("路径中包括错误的路径,请重新设置!");
		   m_destination.SetFocus();
		   return;
	   }
	
	if(bFormat==0)
        m_DestFile.Seek(16,CFile::begin);
	int NumberOfStock=0;
    
	if (m_Shanghai==TRUE)
	{
		if (m_AssignPeriod==1)
		   YieldDay(g_daysh,NumberOfStock,bFormat);
		else
	       AssignDays(g_daysh,NumberOfStock,bFormat);
	}

	if(m_Shenzhen==TRUE)
	{
        if(m_AssignPeriod==1)  
			YieldDay(g_daysz, NumberOfStock,bFormat);
		else 
			AssignDays(g_daysz, NumberOfStock,bFormat);  
	}

	//if(bFormat==0)
	//{
	//    int FileID=FILEID22;
	//	m_DestFile.SeekToBegin();
	//	m_DestFile.Write(&FileID,4);
	//	m_DestFile.Write(&NumberOfStock,4);
	//}

	if(bFormat!=3)
		m_DestFile.Close();
	
	if(bFormat!=0)
	
	;
	else 
	{
	
		if (FloppyFileName.Mid(0,3)=="a:\\"||FloppyFileName.Mid(0,3)=="b:\\"||FloppyFileName.Mid(0,3)=="A:\\"||FloppyFileName.Mid(0,3)=="B:\\")
	
		   Produce2FloppyDisk(DestFileName,FloppyFileName);   
		
		else 
		{
			CFile file(DestFileName,CFile::modeRead);
			int FileLength=file.GetLength();
			CString Path,Name;
			Path=file.GetFilePath();
			Name=file.GetFileName();
			file.Close();
			Path=Path.Left(Path.GetLength()-Name.GetLength()-1);
			if(FileLength>1457000)
			{
			   if(AfxMessageBox("分解生成的数据文件吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
			   {
				   UpdateWindow();
				   SplitOneFileToManyFiles(Path,DestFileName,true);
			   }
			}
			
		}
	}
	

}
BOOL CExportData::IsFileExists( CString filename)
{
	CFile file;
	if(file.Open(_T(filename),CFile::modeRead,NULL) )
	{
		file.Close();
		return TRUE;
	}
    return FALSE;
}
void CExportData::YieldDay(CString filename,int &NumberOfStock,int bFormat)
{

	//  add begin46
	int wMarketType;
	//  add end46
	CTaiKlineFileKLine * pFile=NULL;
	int STOCKSYMBOLLENGTH=0,MaxProgress=0;
	if (filename==g_daysh)
	{
		//  add begin46
		wMarketType=SH_MARKET_EX;
		//  add end46
		STOCKSYMBOLLENGTH=6;
		pFile=CTaiKlineFileKLine::m_fileDaySh;
		MaxProgress=pFile->GetStockNumber();
	}
	else if(filename=="data/shenzhen/day.dat")
	{
		//  add begin46
		wMarketType=SZ_MARKET_EX;
		//  add end46
		STOCKSYMBOLLENGTH=4;
		pFile=CTaiKlineFileKLine::m_fileDaySz;
		MaxProgress=pFile->GetStockNumber();
	}

	int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	int EndDate =atoi(m_install_end.Format("%Y%m%d"));
	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//处理异常
	{
		AfxMessageBox("没有数据！",MB_ICONASTERISK);
		return;
	}



	m_ProgressYield.SetRange(0,MaxProgress);
	
	Kline *pKline=NULL;
	CTime tmb=m_install_start;
	CTime tme=m_install_end;
	for(int k=0;k<MaxProgress;k++)
	{
		
		m_ProgressYield.OffsetPos(1);
		int n=pFile->ReadKlinePeriod(k,pKline,tmb,tme);
		if(n==0)
			continue;
		CString s=pFile->GetSymbol(k);
	
		if(s[0]=='8'&&(s[1]=='K'||s[1]=='k'))
			continue;
	
		
		
		if(bFormat==0)
		{
		
		m_DestFile.Write(s.GetBuffer(0),8);
		//  add begin46
		m_DestFile.Write(&wMarketType,sizeof(int));
		//  add end46
		m_DestFile.Write(&n,sizeof(int));
		m_DestFile.Write(pKline,sizeof(Kline)*n);
		}
		if(bFormat==1)
		{
			
			m_DestFile.Write(s.GetBuffer(0),s.GetLength());
			s.ReleaseBuffer();
	
			s.Format("\t%d\r\n",n);
            m_DestFile.Write(s.GetBuffer(0),s.GetLength());  
			s.ReleaseBuffer();
			for(int ii=0;ii<n;ii++)
			{
			
			   CTime Wtm;
			   Wtm=pKline[ii].day;
			   int dayklinetime=atoi(Wtm.Format("%Y%m%d"));
			   CString Wstr;
			   
			   Wstr.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",dayklinetime,pKline[ii].open,pKline[ii].high,pKline[ii].low,pKline[ii].close,pKline[ii].vol,pKline[ii].amount);
			   int nWstrlen;
			   nWstrlen=Wstr.GetLength();
			   m_DestFile.Write(Wstr.GetBuffer(nWstrlen),nWstrlen);
		
			}
		}
		if(bFormat==2)
		{
			m_DestFile.Write(s.GetBuffer(0),s.GetLength());
			s.ReleaseBuffer();
	
			s.Format("\t%d\r\n",n);
            m_DestFile.Write(s.GetBuffer(0),s.GetLength());  
			s.ReleaseBuffer();
			for(int ii=0;ii<n;ii++)
			{
				CTime Wtm;
				Wtm=pKline[ii].day;
				int dayklinetime=atoi(Wtm.Format("%Y%m%d"));
				CString Wstr;
				
				Wstr.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",dayklinetime,pKline[ii].open,pKline[ii].high,pKline[ii].low,pKline[ii].close,pKline[ii].vol,pKline[ii].amount);
                int nWstrlen;
				nWstrlen=Wstr.GetLength();
				m_DestFile.Write(Wstr.GetBuffer(nWstrlen),nWstrlen);

			}
		}
        if(bFormat==3)
		{
			CString GetUserPathName;
			m_destination.GetWindowText(GetUserPathName);
			GetUserPathName.TrimRight("\\");
			if(wMarketType == SZ_MARKET_EX)
                GetUserPathName+="\\SZNSE\\";
			else
				GetUserPathName+="\\SHASE\\";
            ::CreateDirectory(GetUserPathName,NULL);
			GetUserPathName+=s;
			GetUserPathName+=".day";
			CFile StockFile(GetUserPathName,CFile::modeWrite|CFile::modeCreate);
		   for(int ii=0;ii<n;ii++)
			{
			     CTime Wtm;
				 Wtm=pKline[ii].day;
				 DWORD dayklinetime=atoi(Wtm.Format("%Y%m%d"));
				 QIANLONG Qianlong;
			     Qianlong.dwDate=dayklinetime;
			     Qianlong.dwOpen=DWORD(pKline[ii].open*1000);
			     Qianlong.dwHigh=DWORD(pKline[ii].high*1000);
			     Qianlong.dwLow =DWORD(pKline[ii].low *1000);
			     Qianlong.dwClose=DWORD(pKline[ii].close*1000);
			     Qianlong.dwVolume=DWORD(pKline[ii].vol);
			     Qianlong.dwAmount=DWORD(pKline[ii].amount/1000);
				 Qianlong.wUp=WORD(pKline[ii].advance);
				 Qianlong.wDown=WORD(pKline[ii].decline);
		         StockFile.Write(&Qianlong,sizeof(QIANLONG));
				 
			}
		   StockFile.Close();
		}	
	
		
		NumberOfStock++;
	}
	if(pKline!=NULL)
		delete []pKline;
	m_ProgressYield.SetPos(0);
}


void CExportData::AssignDays(CString filename, int &NumberOfStock,int bFormat)  
{

	//  add begin46
	int wMarketType;
	//  add end46
	CTaiKlineFileKLine*pFile=NULL;
	int MaxProgress=0;
	if(filename==g_daysz)
	{
		//  add begin46
		wMarketType=SH_MARKET_EX;
		//  add end46
		pFile=CTaiKlineFileKLine::m_fileDaySh;
		MaxProgress=pFile->GetStockNumber();
	}
	else if (filename==g_daysz)
	{
		//  add begin46
		wMarketType=SZ_MARKET_EX;
		//  add end46
		pFile=CTaiKlineFileKLine::m_fileDaySz;
		MaxProgress=pFile->GetStockNumber();
	}

	m_ProgressYield.SetRange32(0,MaxProgress);
	
	Kline*pKline=NULL;
	for(int k=0;k<MaxProgress;k++)
	{
	
		m_ProgressYield.OffsetPos(1);
        int n=pFile->ReadKLine(k,pKline,m_NumberOfDays);
		if(n==0)
			continue;
		CString s= pFile->GetSymbol(k);
        if(s[0]=='8'&&(s[1]=='K'||s[1]=='k'))
			continue;		
		
		if(bFormat==0)
		{
			m_DestFile.Write(s.GetBuffer(0),8);
			//  add begin46
			m_DestFile.Write(&wMarketType,sizeof(int));;
			//  add end46
		    m_DestFile.Write(&n,sizeof(int));
		    m_DestFile.Write(pKline,sizeof(Kline)*n);
		}
		if(bFormat==1)
		{
			m_DestFile.Write(s.GetBuffer(0),s.GetLength());
			s.ReleaseBuffer();
		  
			s.Format("\t%d\r\n",n);
            m_DestFile.Write(s.GetBuffer(0),s.GetLength());  
			s.ReleaseBuffer();
			for(int i=0;i<n;i++)
			{
		        // int i=0;
		        CTime Wtm;
		        Wtm=pKline[i].day;
		        int dayklinetime=atoi(Wtm.Format("%Y%m%d"));
		        CString Wstr;
		        
		        Wstr.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",dayklinetime,pKline[i].open,pKline[i].high,pKline[i].low,pKline[i].close,pKline[i].vol,pKline[i].amount);
		        int nWstrlen;
		        nWstrlen=Wstr.GetLength();
		        m_DestFile.Write(Wstr.GetBuffer(nWstrlen),nWstrlen);
			}
		}
		if(bFormat==2)
		{
			m_DestFile.Write(s.GetBuffer(0),s.GetLength());
			s.ReleaseBuffer();
		   
			s.Format("\r%d\r\n",n);
            m_DestFile.Write(s.GetBuffer(0),s.GetLength());  
			s.ReleaseBuffer();
            for(int ii=0;ii<n;ii++)
			{
				CTime Wtm;
				Wtm=pKline[ii].day;
				int dayklinetime=atoi(Wtm.Format("%Y%m%d"));
				CString Wstr;
				
				Wstr.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",dayklinetime,pKline[ii].open,pKline[ii].high,pKline[ii].low,pKline[ii].close,pKline[ii].vol,pKline[ii].amount);
                int nWstrlen;
				nWstrlen=Wstr.GetLength();
				m_DestFile.Write(Wstr.GetBuffer(nWstrlen),nWstrlen);
			}
		}
        if(bFormat==3)
		{
			CString GetUserPathName;
			m_destination.GetWindowText(GetUserPathName);
			GetUserPathName.TrimRight("\\");
			if(wMarketType == SZ_MARKET_EX)
                GetUserPathName+="\\SZNSE\\";
			else
				GetUserPathName+="\\SHASE\\";
            ::CreateDirectory(GetUserPathName,NULL);
			GetUserPathName+=s;
			GetUserPathName+=".day";
			CFile StockFile(GetUserPathName,CFile::modeWrite|CFile::modeCreate);
		   
            for(int ii=0;ii<n;ii++)
			{
			     
				 CTime Wtm;
				 Wtm=pKline[ii].day;
				 DWORD dayklinetime=atoi(Wtm.Format("%Y%m%d"));
				 QIANLONG Qianlong;
			     Qianlong.dwDate=dayklinetime;
			     Qianlong.dwOpen=DWORD(pKline[ii].open*1000);
			     Qianlong.dwHigh=DWORD(pKline[ii].high*1000);
			     Qianlong.dwLow =DWORD(pKline[ii].low *1000);
			     Qianlong.dwClose=DWORD(pKline[ii].close*1000);
			     Qianlong.dwVolume=DWORD(pKline[ii].vol);
			     Qianlong.dwAmount=DWORD(pKline[ii].amount/1000);
				 Qianlong.wUp=WORD(pKline[ii].advance);
				 Qianlong.wDown=WORD(pKline[ii].decline);
		         StockFile.Write(&Qianlong,sizeof(QIANLONG));
				
			}
		}
		NumberOfStock++;
	}
	m_ProgressYield.SetPos(0);
	if(pKline!=NULL)
		delete [] pKline;
}


void CExportData::Produce2FloppyDisk(CString DestFileName,CString FloppyFileName)
{
	CFile SrcFile; 
   
	if(!SrcFile.Open(DestFileName,CFile::modeRead))
	{
		AfxMessageBox("打开文件出错!");
		return;
	}
	int LengthOfSrcFile=SrcFile.GetLength();

	int BytesPerDisk=1457000;
	int NumOfDisks=LengthOfSrcFile%BytesPerDisk==0?LengthOfSrcFile/BytesPerDisk:LengthOfSrcFile/BytesPerDisk+1;
	CString str;
	str.Format("%d",NumOfDisks);
	str="要导出到软盘上, 大约需要 " + str + " 张软盘(3.5寸)。";
	if( AfxMessageBox(str,MB_OKCANCEL)==IDCANCEL )
	{
		SrcFile.Close();
		SrcFile.Remove(DestFileName);
		return;
	}
	DWORD SectorsPerCluster,BytesPerSector,NumberOfFreeClusters,TotalNumberOfClusters;
	for(int nDisk=1;nDisk<=(NumOfDisks+1);nDisk++)
	{
	
		int nSize=min(BytesPerDisk,LengthOfSrcFile);
		if(nSize<=0)
		{
	
			SrcFile.Close();
			SrcFile.Remove(DestFileName);
			return;
		}
	    
		str.Format("%d",nDisk);
	    str="请插入第 "+str+" 张软盘。";
		if( AfxMessageBox(str,MB_OKCANCEL)==IDCANCEL )
		{
			SrcFile.Close();
			SrcFile.Remove(DestFileName);
			return;
		}

	
		::GetDiskFreeSpace(FloppyFileName.Mid(0,2),&SectorsPerCluster,&BytesPerSector,&NumberOfFreeClusters,&TotalNumberOfClusters);
		int FreeSpacePerDisk=NumberOfFreeClusters*SectorsPerCluster*BytesPerSector;
		
		while(FreeSpacePerDisk<(BytesPerDisk+8))
		{
			AfxMessageBox("请删除磁盘上的所有数据,或者格式化磁盘。按确定继续导出!",MB_ICONASTERISK);
			::GetDiskFreeSpace(FloppyFileName.Mid(0,2),&SectorsPerCluster,&BytesPerSector,&NumberOfFreeClusters,&TotalNumberOfClusters);
		    FreeSpacePerDisk=NumberOfFreeClusters*SectorsPerCluster*BytesPerSector;
			//return;
		}
		CFile FloppyFile(FloppyFileName,CFile::modeCreate|CFile::modeWrite);
		FloppyFile.Write(&NumOfDisks,sizeof(4));
		FloppyFile.Write(&nDisk,sizeof(4));
		BYTE *Buffer=new BYTE[nSize];
		SrcFile.Read(Buffer,nSize);
		FloppyFile.Write(Buffer,nSize);
		delete []Buffer;
        FloppyFile.Close();
		LengthOfSrcFile-=nSize;
	}
	SrcFile.Close();
	SrcFile.Remove(DestFileName);
}

void CExportData::SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline)
{
    CFile SrcFile;   

	CString sExd = ".day";
	if(bDayKline==false)
		sExd=".min";
	if(!SrcFile.Open(DestFileName,CFile::modeRead))
	{
		AfxMessageBox("打开文件出错!");
		return;
	}
	int LengthOfSrcFile=SrcFile.GetLength();
    
	int BytesPerDisk=1457000;
    int NumOfDisks= LengthOfSrcFile%BytesPerDisk==0 ? LengthOfSrcFile/BytesPerDisk:LengthOfSrcFile/BytesPerDisk+1;  
	for (int nDisk=1;nDisk<=NumOfDisks;nDisk++)
	{
       
		int nSize=min(BytesPerDisk,LengthOfSrcFile);
		if( nSize<= 0)
		{
		
			return;
		}

		CString DiskXFileName;
		DiskXFileName.Format("%d",nDisk);
		DiskXFileName=Dir+"\\TL_Disk"+DiskXFileName+sExd;
		CFile DiskXFile( DiskXFileName,CFile::modeCreate|CFile::modeWrite);
		DiskXFile.Write(&NumOfDisks,sizeof(4));  
		DiskXFile.Write(&nDisk,sizeof(4));      
		BYTE *Buffer=new BYTE[nSize];   
		SrcFile.Read(Buffer,nSize);   
		DiskXFile.Write(Buffer,nSize); 
		delete []Buffer;             
		DiskXFile.Close();
		
		LengthOfSrcFile-=nSize;     
	}
	SrcFile.Close();
	SrcFile.Remove(DestFileName);
}
	
void CExportData::ProduceDayWbKline(CString DestFileName,int BeginDate,int EndDate)
{
	if( IsFileExists( DestFileName ) )
		if( AfxMessageBox("存在同名文件,是否覆盖？",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return;
    CString FloppyFileName;
	UpdateWindow();
	
	if (DestFileName[0]=='a'||DestFileName[0]=='b'||DestFileName[0]=='A'||DestFileName[0]=='B')
	{
		FloppyFileName=DestFileName;
		DestFileName="C:\\Source.day";
	}


}

void CExportData::OnSelchangeDatetFormat() 
{
	
	UpdateData(TRUE);
    switch(((CComboBox*)GetDlgItem(1011))->GetCurSel())
	{
	case 0:
		m_destination.SetSel(0,-1);
		if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)
		   m_destination.ReplaceSel(pDoc->m_CurrentWorkDirectory);
		else
		   m_destination.ReplaceSel("StockData.day");
		if(((CComboBox *)GetDlgItem(1015))->GetCount()==1)
		{
			((CComboBox *)GetDlgItem(1015))->InsertString(1,"五分钟数据");
			((CComboBox *)GetDlgItem(1015))->InsertString(2,"一分钟数据");
		}
		break;
	case 1:
		m_destination.SetSel(0,-1);
		if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)
		   m_destination.ReplaceSel(pDoc->m_CurrentWorkDirectory);
		else
		   m_destination.ReplaceSel("StockData.txt");
        if(((CComboBox *)GetDlgItem(1015))->GetCount()==1)
		{
			((CComboBox *)GetDlgItem(1015))->InsertString(1,"五分钟数据");
			((CComboBox *)GetDlgItem(1015))->InsertString(2,"一分钟数据");
		}
		break;

	case 2:
		m_destination.SetSel(0,-1);
		if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)
		   m_destination.ReplaceSel(pDoc->m_CurrentWorkDirectory);
		else
		   m_destination.ReplaceSel("StockData.xls");
		if(((CComboBox *)GetDlgItem(1015))->GetCount()==1)
		{
			((CComboBox *)GetDlgItem(1015))->InsertString(1,"五分钟数据");
			((CComboBox *)GetDlgItem(1015))->InsertString(2,"一分钟数据");
		}
		break;
	case 3:
		m_destination.SetSel(0,-1);
		m_destination.ReplaceSel(pDoc->m_CurrentWorkDirectory);
        if(((CComboBox *)GetDlgItem(1015))->GetCount()==3)
		{
			((CComboBox *)GetDlgItem(1015))->DeleteString(1);
		    ((CComboBox *)GetDlgItem(1015))->DeleteString(1);
		}
		break;
	}
	
}
int CExportData::YieldMinuteFile(bool bShangHai,bool bAssignDays,int bFormat)
{
	
	//  add begin46
	int wMarketType;
	//  add end46
	CTaiKlineFileKLine* pFile = NULL;
	int MaxProgress=0;
	if( bShangHai == true )
	{
		//  add begin46
		wMarketType=SH_MARKET_EX;
		//  add end46
		pFile = CTaiKlineFileKLine::m_fileMin5Sh ;
	}
	else 
	{
		//  add begin46
		wMarketType=SZ_MARKET_EX;
		//  add end46
		pFile = CTaiKlineFileKLine::m_fileMin5Sz ;
	}
	MaxProgress=pFile->GetStockNumber();


	int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_install_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//处理异常
	{
		AfxMessageBox("没有数据!",MB_ICONASTERISK);
		return 0;
	}



	m_ProgressYield.SetRange(0,MaxProgress);
    
	
	if(bAssignDays == true && m_NumberOfDays<=0)
		return 0;

	Kline *pKline = NULL;
	CTime tmb = CTime((int)(m_install_start.GetTime())/(24*60*60)*(24*60*60));
	CTime tme = CTime((int)(m_install_end.GetTime())/(24*60*60)*(24*60*60)+(15*60*60)); 
	int NumberOfStock = 0;
	for(int k=0;k<MaxProgress;k++)
	{
	
		m_ProgressYield.OffsetPos(1);

		int n;
		if(bAssignDays == false)
			n= pFile->ReadKlinePeriod(k, pKline, tmb, tme);
		else
			n = pFile->ReadKLine(k, pKline, m_NumberOfDays*48);
		n= n/48;
		if(n==0)
			continue;
		
        if(bFormat==0)
		{
			CString s = pFile->GetSymbol(k);
			m_DestFile.Write(s.GetBuffer (7),7);
			//  add begin46
			m_DestFile.Write(&wMarketType,sizeof(int));
			//  add end46
			m_DestFile.Write(&n , sizeof(int) );
			m_DestFile.Seek(8,CFile::current);
			m_DestFile.Write(pKline,sizeof(Kline)*n*48);
		}
		n= n/48*48;
		if(bFormat==1)
		{
            CString s = pFile->GetSymbol(k);
			m_DestFile.Write(s.GetBuffer(0),s.GetLength());
			s.ReleaseBuffer();
		
			s.Format("\t%d\r\n",n);
            m_DestFile.Write(s.GetBuffer(0),s.GetLength());  
			s.ReleaseBuffer();
			for(int ii=0;ii<n;ii++)
			{
			   CTime Wtm;
			   Wtm=pKline[ii].day;
			   int dayklinetime=atoi(Wtm.Format("%Y%m%d"));
			   CString Wstr;
			   
			   Wstr.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",dayklinetime,pKline[ii].open,pKline[ii].high,pKline[ii].low,pKline[ii].close,pKline[ii].vol,pKline[ii].amount);
			   int nWstrlen;
			   nWstrlen=Wstr.GetLength();
			   m_DestFile.Write(Wstr.GetBuffer(nWstrlen),nWstrlen);
			}
		}
		if(bFormat==2)
		{
           CString s = pFile->GetSymbol(k);
		   m_DestFile.Write(s.GetBuffer(0),s.GetLength());
		   s.ReleaseBuffer();
		 
		   s.Format("\t%d\r\n",n);
            m_DestFile.Write(s.GetBuffer(0),s.GetLength());  
			s.ReleaseBuffer();
		   for(int ii=0;ii<n;ii++)
		   {
		       CTime Wtm;
			   Wtm=pKline[ii].day;
			   int dayklinetime=atoi(Wtm.Format("%Y%m%d"));
			   CString Wstr;
			  
			   Wstr.Format("%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n",dayklinetime,pKline[ii].open,pKline[ii].high,pKline[ii].low,pKline[ii].close,pKline[ii].vol,pKline[ii].amount);
               int nWstrlen;
			   nWstrlen=Wstr.GetLength();
			   m_DestFile.Write(Wstr.GetBuffer(nWstrlen),nWstrlen);
		   }
		}
		NumberOfStock++;
	}

	if(pKline!=NULL)	
		delete [] pKline;
	m_ProgressYield.SetPos(0);

	return NumberOfStock;

}
void CExportData::ProduceMinuteData(int bFormat) 
{
	CString DestFileName,FloppyFileName;
	m_destination.GetWindowText(DestFileName);
	if(  DestFileName.IsEmpty() )
	{
		AfxMessageBox("文件名不应为空!");
		return;
	}

	if( IsFileExists( DestFileName ) )
		if( AfxMessageBox("存在同名文件,是否覆盖？",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return;

	
	if( DestFileName.Mid(0,3)=="a:\\"||DestFileName.Mid(0,3)=="b:\\"||DestFileName.Mid(0,3)=="A:\\"||DestFileName.Mid(0,3)=="B:\\")
	{
		FloppyFileName=DestFileName;
		DestFileName="C:\\Source.min";
	}
    if(!m_DestFile.Open(DestFileName, CFile::modeWrite|CFile::modeCreate))
	{
		AfxMessageBox("路径中包括错误路径,请重新设置!");
		m_destination.SetFocus();
		return;
	}
	int NumberOfStock=0;
	if(bFormat==0)   
		m_DestFile.Seek(8,CFile::begin);  
	if(m_AssignPeriod==1)  
	{
        if(m_Shanghai==TRUE)
		   NumberOfStock = YieldMinuteFile(true,false,bFormat);
		if(m_Shenzhen==TRUE)
		   NumberOfStock += YieldMinuteFile(false,false,bFormat);
	}
	else
	{
		if(m_Shanghai)
			NumberOfStock = YieldMinuteFile(true,true,bFormat);
		if(m_Shenzhen)
			NumberOfStock += YieldMinuteFile(false,true,bFormat);
	}
	//if(bFormat==0)
	//{
 //       int FileID=FILEID22;
	//	m_DestFile.SeekToBegin();
	//	m_DestFile.Write(&FileID,       4  );
	//	m_DestFile.Write(&NumberOfStock,4  );
	//}
	m_DestFile.Close();

	if(bFormat!=0)
    
	   ;
	else 
	{
	
		if( FloppyFileName.Mid(0,3)=="a:\\"||FloppyFileName.Mid(0,3)=="b:\\"||FloppyFileName.Mid(0,3)=="A:\\"||FloppyFileName.Mid(0,3)=="B:\\")
		
			Produce2FloppyDisk(DestFileName,FloppyFileName);
	
		else 
		{
			CFile file(DestFileName,CFile::modeRead);
			int FileLength=file.GetLength();
			CString Path, Name;
			Path=file.GetFilePath();
			Name=file.GetFileName();
			file.Close();
			Path=Path.Left(Path.GetLength()-Name.GetLength()-1);
			if( FileLength >1457000 )
			{
				if( AfxMessageBox("分解生成的数据文件吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
				{
					UpdateWindow();
					SplitOneFileToManyFiles(Path,DestFileName,false);
				}
			}
		
		}
	}
	
}



void CExportData::OnSelchangeFiletype() 
{

	UpdateData();
   if( ((CComboBox *)GetDlgItem(1015))->GetCurSel()==0)
   {
	   if(((CComboBox *)GetDlgItem(1011))->GetCurSel()==0)
	       m_destination.SetWindowText("StockData.day");
	   else if(((CComboBox *)GetDlgItem(1011))->GetCurSel()==1)
           m_destination.SetWindowText("StockData.txt");
       else if(((CComboBox *)GetDlgItem(1011))->GetCurSel()==2)
           m_destination.SetWindowText("StockData.xls");
	   m_destination.EnableWindow(TRUE);
	 
	   if(m_DataFormat.GetCount()==3)
	       m_DataFormat.InsertString(3,"钱龙格式");
	   GetDlgItem(1008)->EnableWindow(TRUE);
	   GetDlgItem(1013)->EnableWindow(TRUE);
	   GetDlgItem(1014)->EnableWindow(TRUE);
       if(m_AssignPeriod)
	   {
		   GetDlgItem(1012)->EnableWindow(TRUE);
		   GetDlgItem(1019)->EnableWindow(TRUE);
		   GetDlgItem(1008)->EnableWindow(FALSE);
	   }
   }
   else if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==1)
   {
	   if(((CComboBox *)GetDlgItem(1011))->GetCurSel()==0)
	       m_destination.SetWindowText("StockData.min");
	   else if(((CComboBox *)GetDlgItem(1011))->GetCurSel()==1)
           m_destination.SetWindowText("StockData.txt");
       else if(((CComboBox *)GetDlgItem(1011))->GetCurSel()==2)
           m_destination.SetWindowText("StockData.xls");
	   m_destination.EnableWindow(TRUE);
	 
	   GetDlgItem(1008)->EnableWindow(TRUE);
	   GetDlgItem(1013)->EnableWindow(TRUE);
	   GetDlgItem(1014)->EnableWindow(TRUE);
	   if(m_AssignPeriod)
	   {
		   GetDlgItem(1012)->EnableWindow(TRUE);
		   GetDlgItem(1019)->EnableWindow(TRUE);
		   GetDlgItem(1008)->EnableWindow(FALSE);
	   }
	   m_DataFormat.DeleteString(3);
   }
   else if(((CComboBox*)GetDlgItem(1015))->GetCurSel()==2)
   {
	   m_destination.SetWindowText(pDoc->m_CurrentWorkDirectory);
	   
	   m_DataFormat.DeleteString(3);
	   GetDlgItem(1008)->EnableWindow(FALSE);
	   GetDlgItem(1013)->EnableWindow(FALSE);
	   GetDlgItem(1012)->EnableWindow(FALSE);
	   GetDlgItem(1014)->EnableWindow(FALSE);
	   GetDlgItem(1019)->EnableWindow(FALSE);
   }
	
}
BOOL CExportData::ProduceMin(LPTSTR lpStockSymbol,int bFormat,int nKind)
{
	
	


	
	CReportData* pcdat1;
	CdatOld  pcdat2;
    CString StockSymbol=lpStockSymbol;

	if( !pDoc->m_sharesInformation.Lookup(lpStockSymbol, pcdat1,nKind) )
		return 0;


	pcdat2.rdp=pcdat1->rdp;         
	strcpy(pcdat2.id,pcdat1->id);          
	//  add begin46
	pcdat2.kind=pcdat1->kind;

	strcpy(pcdat2.name,pcdat1->name);         
	strcpy(pcdat2.Gppyjc,pcdat1->Gppyjc);        

	pcdat2.ystc=pcdat1->ystc;              
	pcdat2.opnp=pcdat1->opnp;             
	pcdat2.higp=pcdat1->higp;             
	pcdat2.lowp=pcdat1->lowp;           
	pcdat2.nowp=pcdat1->nowp;             
	pcdat2.nowv=pcdat1->nowv;              
	pcdat2.totv=pcdat1->totv;             
	pcdat2.totp=pcdat1->totp;            
	pcdat2.pbuy1=pcdat1->pbuy1;          
	pcdat2.vbuy1=pcdat1->vbuy1;             
	pcdat2.pbuy2=pcdat1->pbuy2;            
	pcdat2.vbuy2=pcdat1->vbuy2;             
	pcdat2.pbuy3=pcdat1->pbuy3;            
	pcdat2.vbuy3=pcdat1->vbuy3;                 
	pcdat2.psel1=pcdat1->psel1;            
	pcdat2.vsel1=pcdat1->vsel1;            
	pcdat2.psel2=pcdat1->psel2;            
	pcdat2.vsel2=pcdat1->vsel2;            
	pcdat2.psel3=pcdat1->psel3;
	pcdat2.vsel3=pcdat1->vsel3;   
	pcdat2.accb=pcdat1->accb;            
	pcdat2.accs=pcdat1->accs;              
	pcdat2.volume5=pcdat1->volume5;           
	pcdat2.rvol=pcdat1->rvol;            
	pcdat2.dvol=pcdat1->dvol;            
	pcdat2.lastclmin=pcdat1->lastclmin;      
	pcdat2.initdown=pcdat1->initdown;             
	pcdat2.InOut=pcdat1->InOut;

	pcdat2.nReservation=0xFFFFFFFF;
    if(bFormat==0)
	 
		for(int i=0;i<240;i++)
		{
			pcdat2.m_Kdata[i].Amount=pcdat1->m_Kdata1[i].Amount;
			pcdat2.m_Kdata[i].Price=pcdat1->m_Kdata1[i].Price;
			pcdat2.m_Kdata[i].Volume=pcdat1->m_Kdata1[i].Volume;
		}

	CString szDir;


	int temp_group = pcdat2.kind;



	CString GetUserPath,FileName;
    m_destination.GetWindowText(GetUserPath);
	GetUserPath.TrimRight("\\");
	GetUserPath+="\\";
	if(bFormat==0)
		FileName="导出1分钟二进制数据";
    if(bFormat==1)
		FileName="导出1分钟文本数据";
    if(bFormat==2)
		FileName="导出1分钟EXCEL数据";
//		
	switch(temp_group)
	{
	case 0:
		{
		szDir=GetUserPath+FileName+"\\沪指\\";
		szDir+=lpStockSymbol;
		pcdat2.nowv = pcdat1->nowp;
		if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		
		break;
	}
	case 1:
		{
		szDir=GetUserPath+FileName+"\\沪A\\";
		szDir+=lpStockSymbol;
			if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	case 2:
		{
		szDir=GetUserPath+FileName+"\\沪B\\";
		szDir+=lpStockSymbol;
				if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	case 3:
		{
		szDir=GetUserPath+FileName+"\\深指\\";
		szDir+=lpStockSymbol;
				if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		pcdat2.nowv = pcdat1->nowp;
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	case 4:
		{
		szDir=GetUserPath+FileName+"\\深A\\";
		szDir+=lpStockSymbol;
				if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	case 5:
		{
		szDir=GetUserPath+FileName+"\\深B\\";
		szDir+=lpStockSymbol;
				if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	case 6:
		{
		szDir=GetUserPath+FileName+"\\沪债\\";
		szDir+=lpStockSymbol;
				if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	case 7:
		{
		szDir=GetUserPath+FileName+"\\深债\\";
		szDir+=lpStockSymbol;
			if(bFormat==0)
		   szDir+=".1mn";
		if(bFormat==1)
		   szDir+=".txt";
		if(bFormat==2)
		   szDir+=".xls";
		CFile file(szDir,CFile::modeWrite|CFile::modeCreate);
		if(bFormat==0)
		   file.Write(&pcdat2,sizeof(CdatOld));
		if(bFormat==1)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		}
		if(bFormat==2)
		{
			CString str;
			str.Format("%s\t%s\t%s\t",pcdat1->id,pcdat1->name,pcdat1->Gppyjc);
		    file.Write(str.GetBuffer(0),str.GetLength());
			float *fp=&(pcdat1->ystc);
			for(int point=0;point<25;point++)
			{
				str.Format("%.2f\t",fp[point]);
				file.Write(str.GetBuffer(0),str.GetLength());
			}

			Kdata1 * pf=pcdat1->m_Kdata1;
			for(int i=0;i<240;i++)
			{
				str.Format("%.2f\t%.2f\t%.2f\r\n",pf[i].Amount,pf[i].Price,pf[i].Volume);
			    file.Write(str.GetBuffer(0),str.GetLength());
			}
		
		}
		file.Close();
		break;
		}
	default:
		break;
	}


	return 0;
}
	

BOOL CExportData::IsDirectoryExist()
{
	CFileFind finder;
	BOOL bWorking;
	CString Filepath,FileName,strNl;
	CFile DeleFile;
	CString GetUserPath;
	m_destination.GetWindowText(GetUserPath);
	GetUserPath.TrimRight("\\");
	
	GetUserPath+="\\";
   
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
	{
		FileName="导出1分钟二进制数据";
	    strNl=".1mn";
	}
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
	{
		FileName="导出1分钟文本数据";
		strNl=".txt";
	}
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==2)
	{
		FileName="导出1分钟EXCEL数据";
		strNl=".xls";
	}

	if (!(bWorking=finder.FindFile(GetUserPath+FileName+"\\*.*") ) )
		if(!(::CreateDirectory(GetUserPath+  FileName,NULL)))
		{
			AfxMessageBox("路径中包括错误的路径,请重新设置!");
			m_destination.SetFocus();
			return 1;
		}
	
	if(bWorking)
	if( AfxMessageBox("存在同名文件,是否覆盖？",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return 1;

	if (  !(bWorking=finder.FindFile(GetUserPath+FileName+"\\沪A\\"+strNl) ) )
		::CreateDirectory(GetUserPath+  FileName+"\\沪A",NULL);

	while(bWorking)
	{
		bWorking=finder.FindNextFile();
		Filepath=finder.GetFilePath();
		DeleFile.Remove(Filepath);
	}

	if (  !(bWorking=finder.FindFile(GetUserPath+FileName+"\\沪B\\"+strNl)  ))
		::CreateDirectory(  GetUserPath+FileName+"\\沪B",NULL);
		else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }
	if (  !(bWorking= finder.FindFile(GetUserPath+FileName+"\\沪指\\"+strNl)  ))
		::CreateDirectory( GetUserPath+ FileName+"\\沪指",NULL);
			else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }

	if (  !(bWorking= finder.FindFile(GetUserPath+FileName+"\\沪债\\"+strNl)  ))
		::CreateDirectory(  GetUserPath+FileName+"\\沪债",NULL);
			else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }


	if (  !(bWorking= finder.FindFile(GetUserPath+FileName+"\\深A\\"+strNl)  ))
		::CreateDirectory(GetUserPath+ FileName+"\\深A",NULL);
			else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }

	if (  !(bWorking= finder.FindFile(GetUserPath+FileName+"\\深B\\"+strNl)  ))
		::CreateDirectory( GetUserPath+ FileName+"\\深B",NULL);		
	   else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }

	if (  !(bWorking= finder.FindFile(GetUserPath+FileName+"\\深指\\"+strNl)  ))
		::CreateDirectory(GetUserPath+ FileName+"\\深指",NULL);
			else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }

	if (  !(bWorking=finder.FindFile(GetUserPath+FileName+"\\深债\\"+strNl)  ))
		::CreateDirectory(GetUserPath+  FileName+"\\深债",NULL);
			else while(bWorking)
		   {
			   bWorking=finder.FindNextFile();
			   Filepath=finder.GetFilePath();
			   DeleFile.Remove(Filepath);
		   }

     return 0;
}

void CExportData::ProduceToSoftDisk()
{
    

}

BOOL CExportData::IsCreatePath(CString PathName, BOOL bPath)
{

	return 1;
}

void CExportData::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{

    int ItemIndex=m_listfw.GetNextItem(-1,LVNI_SELECTED);
	if (ItemIndex==-1)
		return;
    LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;

	if(m_stockrange.GetAt(ItemIndex)==TRUE)
	{
		myitem.iImage=9;
		m_stockrange.SetAt(ItemIndex,FALSE);
	}
	else
	{
		myitem.iImage=6;
		m_stockrange.SetAt(ItemIndex,TRUE);
	}
    CString stockgroup[]={"上海","深圳","创业"};	
	myitem.iItem=ItemIndex;
	myitem.pszText=(LPTSTR)stockgroup[ItemIndex].GetBuffer(stockgroup[ItemIndex].GetLength());
	
	m_listfw.SetItem(&myitem);
	m_listfw.Update(ItemIndex);

	
	*pResult = 0;
}

BOOL CExportData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);return TRUE;
	
}
