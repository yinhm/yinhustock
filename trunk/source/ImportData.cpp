// ImportData.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ImportData.h"
#include "mainfrm.h"
#include "CTaiKlineFileKLine.h"
#include "SBDestination.h"
#include "BrowseForFolder.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int shijianflag=0;



IMPLEMENT_DYNCREATE(CImportData, CPropertyPage)

CImportData::CImportData() : CPropertyPage(CImportData::IDD)
{
	//{{AFX_DATA_INIT(CImportData)
	m_importAll = 0;
	m_install_end = 0;
	m_install_start = 0;
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_taiShanDoc ;

	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;
    m_nImportorcancel=0;
    m_bImport=TRUE;
}

CImportData::~CImportData()
{
}

void CImportData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportData)
	DDX_Control(pDX, 1012, m_button1);
	DDX_Control(pDX, IDC_INSTALLEND, m_ctrlTimeEnd);
	DDX_Control(pDX, IDC_INSTALLSTART, m_ctrlTimeStart);
	DDX_Control(pDX, IDC_LIST, m_listfw);
	DDX_Control(pDX, IDOK, m_okbutton);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	DDX_Control(pDX, 1042, m_ComboDatatype);
	DDX_Control(pDX, 1011, m_ComboFiletype);
	DDX_Control(pDX, 1008, m_DataSource);
	DDX_Radio(pDX, 1013, m_importAll);
	DDX_DateTimeCtrl(pDX, IDC_INSTALLEND, m_install_end);
	DDX_DateTimeCtrl(pDX, IDC_INSTALLSTART, m_install_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportData, CPropertyPage)
	//{{AFX_MSG_MAP(CImportData)
	ON_CBN_SELCHANGE(1011, OnSelchangedatatype)
	ON_CBN_SELCHANGE(1042, OnSelchangedataformat)
	ON_LBN_SETFOCUS(IDC_LIST, OnSetfocusfwxz)
	ON_EN_SETFOCUS(1008, OnSetfocus)
	ON_BN_CLICKED(1013, Onqbyr)
	ON_BN_CLICKED(1014, Onsdyr)
	ON_EN_SETFOCUS(1019, OnSetfocusup)
	ON_EN_SETFOCUS(1020, OnSetfocusdown)
	ON_BN_CLICKED(IDOK, Onimport)
	ON_BN_CLICKED(1012, OnButtonSearchfixdata)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_DRAWLIST,drawzqsc)
END_MESSAGE_MAP()




void CImportData::OnOK() 
{

	
	CPropertyPage::OnOK();
}



BOOL CImportData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


    ((CComboBox *)GetDlgItem(1042))->SetCurSel(0);
    ((CComboBox *)GetDlgItem(1011))->SetCurSel(0);

    m_ComboDatatype.InsertString( 0, "数据格式");
	m_ComboDatatype.InsertString( 1, "钱龙数据格式");
	m_ComboDatatype.InsertString( 2, "分析家数据格式");
	m_ComboDatatype.InsertString( 3, "汇金数据格式");
	m_ComboDatatype.InsertString( 4, "胜龙数据格式");
	
	m_ComboDatatype.SetCurSel(0);
    m_DataSource.ReplaceSel(pDoc->m_CurrentWorkDirectory+"\\StockData.day");

	m_importAll = 0;
	
	GetDlgItem(IDC_INSTALLSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALLEND)->EnableWindow(FALSE);

    
    m_install_start=CTime(1990,1,1,1,1,1);
	m_install_end  =CTime::GetCurrentTime();

	LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	myitem.iImage=6;
	m_listfw.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	CString stockgroup[]={"上海","深圳","创业"};
	for(int i=0;i<3;i++)
	{
		myitem.iItem=i;
		if(i==2)
			myitem.iImage=9;
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

void CImportData::OnSelchangedatatype() 
{
	
	if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==0  )		
	{
		m_DataSource.SetWindowText(pDoc->m_CurrentWorkDirectory+"\\StockData.day");
	    GetDlgItem(1042)->EnableWindow(TRUE);
		GetDlgItem(1013)->EnableWindow(TRUE);
	    GetDlgItem(1014)->EnableWindow(TRUE);
		if(shijianflag==1)
		{
		   GetDlgItem(IDC_INSTALLSTART)->EnableWindow(TRUE);
	       GetDlgItem(IDC_INSTALLEND)->EnableWindow(TRUE);
		}
	}
	else if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==1  )	
	{
		m_DataSource.SetWindowText(pDoc->m_CurrentWorkDirectory+"\\StockData.min");
		GetDlgItem(1042)->EnableWindow(FALSE);
	   	GetDlgItem(1013)->EnableWindow(TRUE);
	    GetDlgItem(1014)->EnableWindow(TRUE);
		if(shijianflag==1)
		{
		   GetDlgItem(IDC_INSTALLSTART)->EnableWindow(TRUE);
	       GetDlgItem(IDC_INSTALLEND)->EnableWindow(TRUE);
		}
	}
	else if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==2  )	
	{
		m_DataSource.SetWindowText("C:\\");
		GetDlgItem(1042)->EnableWindow(FALSE);
		GetDlgItem(1013)->EnableWindow(FALSE);
		GetDlgItem(1014)->EnableWindow(FALSE);
		if(shijianflag==1)
		{
		   GetDlgItem(IDC_INSTALLSTART)->EnableWindow(FALSE);
	       GetDlgItem(IDC_INSTALLEND)->EnableWindow(FALSE);
		}
	}

}

void CImportData::OnSelchangedataformat() 
{

	switch( m_ComboDatatype.GetCurSel( ) )
	{
	case 0:    
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel(pDoc->m_CurrentWorkDirectory+"\\StockData.day");
		if(m_ComboFiletype.GetCount()==1)
		{
			m_ComboFiletype.InsertString(1,"五分钟数据");
            m_ComboFiletype.InsertString(2,"一分钟数据");
		}
		break;
	case 1:   
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;
	case 2:   
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\superstk.dad");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;
	case 3:   
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\HUIJIN");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;
	case 4:    
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\SLON");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;

	}


}

void CImportData::OnSetfocusfwxz() 
{

	
}

void CImportData::OnSetfocus() 
{

	
}


void CImportData::Onqbyr() 
{

	GetDlgItem(IDC_INSTALLSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALLEND)->EnableWindow(FALSE);
    shijianflag=0;
}

void CImportData::Onsdyr() 
{

	GetDlgItem(IDC_INSTALLSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_INSTALLEND)->EnableWindow(TRUE);
    shijianflag=1;
}

void CImportData::OnSetfocusup() 
{
	
	
}

void CImportData::OnSetfocusdown() 
{

	
}

void CImportData::Onimport() 
{
	
	BOOL IsChange=FALSE;
	if( pDoc->m_systemOption.autoday==TRUE )
	{
		pDoc->m_systemOption.autoday=FALSE;
		IsChange=TRUE;
	}
	

	UpdateData(TRUE);
	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;

	m_nImportorcancel++;
	if(m_nImportorcancel>1)
	{
	
		GetDlgItem(IDOK)->SetWindowText("导入");
		return;
	}	
  
	int length=m_stockrange.GetSize();
	for (int i=0;i<length;i++)
		if(m_stockrange.GetAt(i)==TRUE)
			if(i==0)
				m_Shanghai=TRUE;
			else if (i==1)
				m_Shenzhen=TRUE;

	if (m_Shanghai==FALSE&&m_Shenzhen==FALSE)
	{
		AfxMessageBox("请选择范围! ");
	    m_nImportorcancel=0;
		return ;
	}

	if(m_install_start<=0||m_install_end<=0)
	{
		AfxMessageBox("时间设置错误!");
	
		m_nImportorcancel=0;
		return;
	}
	int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_install_end.Format("%Y%m%d"));
	CTime CurrentTime=CTime::GetCurrentTime();
   
		
	if (shijianflag==1)
		if(BeginDate > EndDate)	
		{
			AfxMessageBox("时间颠倒,请重新设置!");
			m_nImportorcancel=0;
			return;
		}
		else if(m_install_start>CurrentTime) 
		{
			AfxMessageBox("开始时间不正确,请重新设置!");
			m_nImportorcancel=0;
			return;
		}

	
	if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==0  ) 
	{
	
		CString srcfilename;
        switch (m_ComboDatatype.GetCurSel())
		{
		case 0:    
              m_DataSource.GetWindowText(srcfilename);
			if(  srcfilename.IsEmpty() )
			{
				AfxMessageBox("文件名不应为空!");
				m_nImportorcancel=0;
				return;
			}
		
			if(IsFileType(srcfilename)!=0)
			{
				AfxMessageBox("非合法日线数据!");
				m_nImportorcancel=0;
				return;
			}
           
			if( srcfilename[0]=='a'||srcfilename[0]=='b'||srcfilename[0]=='A'||srcfilename[0]=='B')
			{
				if( !InstallFromFloppyDisk(srcfilename) )
				{
					AfxMessageBox("安装没能正常进行!",MB_ICONASTERISK);
					m_nImportorcancel=0;
					return;
				}
				srcfilename="c:\\source.day";
			}
				else 
			{
				srcfilename.MakeLower();
				if( srcfilename.Find("tl_disk") != -1 )
				{
					if( ! InstallFromSeveralDiskfile(srcfilename) )
					{
						AfxMessageBox("安装没能正常进行！",MB_ICONASTERISK);
						m_nImportorcancel=0;
						return;
					}
					srcfilename="c:\\source.day";
				}
			}
            GetDlgItem(IDOK)->SetWindowText("取消");
			InstallWanshen(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("导入");
			break;
	   case 1:   
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("取消");
			InstallQianlong(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			if(m_nImportorcancel==1)
			  
		    m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("导入");
			break;
		case 2: 
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("取消");
			InstallFenxijia(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("导入");
			break;
	    case 3:   
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("取消");
			InstallHuijin(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("导入");
			break;
		case 4:    
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("取消");
			InstallShenglong(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("导入");
			break;

		default:
			AfxMessageBox("没有选择安装数据的类型!");
			break;
		}
	
		}
		else if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==1  )  
		{
		InstallMinteData();
		m_nImportorcancel=0;

		}
		else 
		{
			CString SrcFileName;
		    m_DataSource.GetWindowText(SrcFileName);
			CFileFind finder;
			if(!finder.FindFile(SrcFileName+"\\*.1mn"))
			{
				AfxMessageBox("找不到文件!");
				m_nImportorcancel=0;
		        return ;
			}
		    InstallMin(SrcFileName);
			m_progress1.SetRange(0,100);
			m_progress1.SetPos(100);
			m_progress1.SetPos(0);
			m_nImportorcancel=0;
		}

		if( IsChange )
			pDoc->m_systemOption.autoday=FALSE;
     
}



void CImportData::OnButtonSearchfixdata() 
{
    UpdateData(TRUE);
	CString filename;
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==2||(m_ComboDatatype.GetCurSel()==1||m_ComboDatatype.GetCurSel()==3||m_ComboDatatype.GetCurSel()==4)
		&&((CComboBox*)GetDlgItem(1011))->GetCurSel()!=1)
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

		CString FileName;

		CString Filter="日线数据(*.day)|*.day||";  
		if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
		{
			FileName=pDoc->m_Directory.CurrentDir;
			FileName+="StockData.day";
		}
		else if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
		{
			FileName=pDoc->m_Directory.CurrentDir;
			FileName+="StockData.min";
			DefExt="*.min";
			Filter="5分钟K线数据(*.min)|*.min||";
		}
		int nSel = m_ComboDatatype.GetCurSel();
		if(nSel == 2)
		{
			FileName=pDoc->m_Directory.CurrentDir;
			FileName+="StockData.dad";
			DefExt="*.dad";
			Filter="分析家日线数据(*.dad)|*.dad||";
		}
		CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,
	    		(LPCTSTR)FileName,
				 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
				(LPCSTR)Filter,this);
		if(bOpenFileDialog.DoModal()==IDOK)
		{
			 filename=bOpenFileDialog.GetPathName();
			 CString fname=bOpenFileDialog.GetFileName();
			 CString NewFileName=filename;
			 NewFileName.TrimRight(fname);
			 strcpy(pDoc->m_Directory.CurrentDir,NewFileName.GetBuffer(0));
		}
	}
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==2||m_ComboDatatype.GetCurSel()==1||m_ComboDatatype.GetCurSel()==3||m_ComboDatatype.GetCurSel()==4)
		filename=m_news;
	m_DataSource.SetSel(0,-1);
	m_DataSource.ReplaceSel(filename);
	m_DataSource.GetWindowText(SourceFileName);
    UpdateData(FALSE);
	
}


BOOL CImportData::InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline)
{
	int NumOfDisks=0, 
		nDisk=0;    
	CFile SrcFile;    
	CFile DestFile;  
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !DestFile.Open("c:\\source"+sExd,CFile::modeWrite|CFile::modeCreate) )
	{
		AfxMessageBox("打开文件出错!");
		return 0;
	}

	int IndexOfDisk=1;
	int nRepeat=0;      

	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
		AfxMessageBox("插入第 "+str+" 号磁盘。");
		
		CFileFind finder;
		BOOL bWorking=finder.FindFile(FloppyFileName.Left(3)+"*.*");
		while(bWorking)
		{
			bWorking=finder.FindNextFile();
			FloppyFileName=finder.GetFilePath();
		}

		if( !SrcFile.Open(FloppyFileName,CFile::modeRead) )
		{
			AfxMessageBox("打开文件出错!");
			return 0;
		}
		SrcFile.Read(&NumOfDisks,4); 
		SrcFile.Read(&nDisk,4);     
		int LengthOfFile=SrcFile.GetLength(); 
		
	
		if( nDisk != IndexOfDisk )
		{
			nRepeat++;
			AfxMessageBox("重新插入第"+str+"张磁盘。");
			SrcFile.Close();
			if( nRepeat>5) return 0;
			continue;
		}

		LengthOfFile-=8;
		BYTE *Buffer=new BYTE[LengthOfFile];
		SrcFile.Read(Buffer,LengthOfFile);  
		SrcFile.Close();
		DestFile.Write(Buffer,LengthOfFile);
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); 
    DestFile.Close();  
	return 1;
}



void CImportData::InstallWanshen(CString srcfilename, int BeginDate, int EndDate) 
{
	{
	
	BOOL IsSoft=FALSE;	
	CString SourceFilename=srcfilename;

	m_DataSource.GetWindowText(SourceFilename);

	
	if( SourceFilename[0]=='a'||SourceFilename[0]=='b'||SourceFilename[0]=='A'||SourceFilename[0]=='B')
	{
		SourceFilename="c:\\source.day";
		IsSoft=TRUE;
	}
	
	CString StrName=SourceFilename;
	StrName.MakeLower();
	if( StrName.Find("tl_disk") != -1 )
	{
		SourceFilename="c:\\source.day";
		IsSoft=TRUE;
	}
	CFile SourceFile;
	if(!SourceFile.Open(SourceFilename,CFile::modeRead))
	{
		AfxMessageBox("路径中包括错误的路径，请重新设置");
		return;
	}
	

	int NumStock=0 ;             
	int FileID; 
	SourceFile.Read(&FileID,4);
	//if( FileID!=FILEID22)
	//{
	//	AfxMessageBox("非"+g_strCompanyName+"日线数据格式!");
	//	return;
	//}
	SourceFile.Read(&NumStock,sizeof(int));
	SourceFile.Seek(16,CFile::begin);
	
	
	m_progress1.SetRange(0,NumStock);
	

	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;     

	int wMarketType;

	for( int nStock=0; nStock<NumStock; nStock++)
	{

		MSG message;
		if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}  
        if(m_nImportorcancel==2)
			return;
	
		m_progress1.SetPos(nStock);
		m_InstallArray.RemoveAll();

	
		SourceFile.Read(StockName,8);

		SourceFile.Read(&wMarketType,sizeof(int));
	
		SourceFile.Read(&NumKline,4);
	
		for( int nKline=0; nKline<NumKline; nKline++)
		{
			SourceFile.Read( &wanshen,sizeof(Kline) );
			m_InstallArray.Add(wanshen);
		}
		CString symbol(StockName);
		
		if(StockName[0]=='8'&&(StockName[1]=='k'||StockName[1]=='K'))
			continue;
	
		InstallStockKlineEach( symbol,wMarketType, BeginDate, EndDate);
	}



	SourceFile.Close();
    m_progress1.SetPos(0);
	if(IsSoft)
		SourceFile.Remove(SourceFilename);
	}
}

int CImportData::InstallFromSeveralDiskfile(CString FileName,bool bDayKline)
{
	int NumOfDisks=0,
		nDisk=0;   
	CFile SrcFile; 
	CFile DestFile;  
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !DestFile.Open("c:\\source"+sExd,CFile::modeWrite|CFile::modeCreate) )
	{
		AfxMessageBox("打开文件出错!");
		return 0;
	}

	int IndexOfDisk=1;  
	int nRepeat=0;      
	
	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
	    if(IndexOfDisk<11)
		   FileName.Delete( FileName.GetLength()-5 );
		else 
			FileName.Delete( FileName.GetLength()-6,2 );
		FileName.Insert( FileName.GetLength()-4, str );
		if( !SrcFile.Open(FileName,CFile::modeRead) )
		{
			AfxMessageBox("打开文件出错!");
			return 0;
		}
		SrcFile.Read(&NumOfDisks,4);  
		SrcFile.Read(&nDisk,4);     
		int LengthOfFile=SrcFile.GetLength(); 
		
	
		if( nDisk != IndexOfDisk )
		{
			SrcFile.Close();
			return 0;
		}

		LengthOfFile-=8;
		BYTE *Buffer=new BYTE[LengthOfFile];
		
		SrcFile.Read(Buffer,LengthOfFile);  
		SrcFile.Close();
		int nid = *((int*)Buffer);
		DestFile.Write(Buffer,LengthOfFile);
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); 

	DestFile.Close();

	return 1;

}
void CImportData::InstallStockKlineEach(CString symbol,int wMarketType,int BeginDate,int EndDate,bool bDayKline)
{
		
	if(symbol == "") return;

		char symb[10];
		strcpy(symb,symbol);
		DWORD dwKind=CSharesInformation::GetStockKind(wMarketType,symb);

		if(wMarketType == 0) dwKind = CFormularCompute::GetStockKind(symb);

		CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (symbol,dwKind,bDayKline);
		
		CTime tm;
	
		int nFixStyle=GetCheckedRadioButton(1013,1014);
		switch(nFixStyle)
		{
		case 1014:  
			{
			for( int i=0;i<m_InstallArray.GetSize();i++)
			{
				tm=m_InstallArray[i].day;
				if(  atoi( tm.Format("%Y%m%d") ) < BeginDate  )
				{
					m_InstallArray.RemoveAt(i);
					i--;
				}
				else
					break;
			}
			int nSize=m_InstallArray.GetSize();
			while( nSize-- )
			{
				tm=m_InstallArray[nSize].day;
				if(  atoi( tm.Format("%Y%m%d") ) > EndDate  )
					m_InstallArray.RemoveAt(nSize);
				else
					break;
			}
			
		 
			if( m_InstallArray.GetSize() ==0 ) 
				break;
			}

		case 1013 : 
			{
				std::string code(symbol);
				pFile ->WriteKLine(code,m_InstallArray.GetData(),m_InstallArray.GetSize(),CTaiKlineFileKLine::overWriteAll);
			}
			break;
		}
}



void CImportData::InstallQianlong(CString srcfilename, int BeginDate, int EndDate) 
{
	if (shijianflag==1)
	if(  BeginDate<19700101 ||BeginDate>20380101 ||
		 EndDate  <19700101 ||EndDate  >20380101 || BeginDate > EndDate )
	{
		AfxMessageBox("必须输入正确的时间!",MB_ICONASTERISK);
		return;
	}

	{
	
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		m_DataSource.GetWindowText(FinderName);
		if(!finder.FindFile(FinderName+"\\*.*"))
		{
			AfxMessageBox("打开文件出错,请检查相关设置!");
			return;
		}
		BOOL bwork = finder.FindFile(FinderName+"\\*.*");
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}
		if( !NumFile )
		{
			AfxMessageBox("没有要安装的数据!");
			return;
		}

		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		Kline Wanshen;
		CString StockName;  
		CString szTime;
		CTime tm;

	
		BOOL bWorking = finder.FindFile(srcfilename+"\\*.day");
		while( bWorking )
		{
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			if(m_nImportorcancel==2)
				return;
			//
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();
  
		
			CString QianlongFileName=finder.GetFilePath();
			CFile QianlongFile((LPCTSTR)QianlongFileName,CFile::modeRead);
			int FileLength=QianlongFile.GetLength();
			
			StockName=finder.GetFileTitle();
			
			if( StockName.GetLength() !=6&&StockName.GetLength() !=4 )		
				continue;
			
			m_InstallArray.RemoveAll();   
		
			int numKline=FileLength/40;
			
			if( numKline <= 0 ) 
			{
				QianlongFile.Close();
				continue;
			}

			QIANLONG* Qianlong=new QIANLONG[numKline];
			QianlongFile.Read( Qianlong, sizeof(QIANLONG)*numKline );
			QianlongFile.Close();

			LONG time;
			for( int i=0; i < numKline; i++)
			{
				if( GetStockTime(Qianlong[i].dwDate,time) && Qianlong[i].dwLow  >0 && Qianlong[i].dwVolume  >0 ) 
				{
					Wanshen.day   = time; 
					Wanshen.open   = (float)(Qianlong[i].dwOpen/1000.0);
					Wanshen.high   = (float)(Qianlong[i].dwHigh/1000.0);
					Wanshen.low    = (float)(Qianlong[i].dwLow/1000.0);
					Wanshen.close  = (float)(Qianlong[i].dwClose/1000.0);
					Wanshen.vol = (float)(Qianlong[i].dwVolume);
					Wanshen.amount = (float)(Qianlong[i].dwAmount*1000.0);
					Wanshen.advance=		  Qianlong[i].wUp;
					Wanshen.decline=		  Qianlong[i].wDown;
					m_InstallArray.Add(Wanshen);
				}
			}
			delete []Qianlong;
			
			
			StockName.MakeUpper();
		
			InstallStockKlineEach( StockName, 0,BeginDate, EndDate);
		
		}
		m_progress1.SetPos(0);
	}

}

BOOL CImportData::GetStockTime(int ymd, LONG& tm)
{
	if( ymd<20380101 && ymd > 19901201 )
	{
		CTime time( ymd/10000, (ymd%10000)/100, ymd%100, 9, 30, 0);
		tm=time.GetTime();
		return 1;
	}
	else
		return 0;

}



void CImportData::InstallFenxijia(CString srcfilename ,int BeginDate, int EndDate) 
{

	CFile SourceFile;
	if(IsFileType(srcfilename)!=3)
	{
		AfxMessageBox("不是分析家日线数据文件格式!");
		return;
	}
	if(!SourceFile.Open(srcfilename,CStkFile::modeRead))
	{
		AfxMessageBox("打开文件出错,请检查相关设置!");
		return;
	}
	int NumStock=0;
    int Flag1;
	SourceFile.Read(&Flag1,4);
	if(Flag1!=872159628)
	{
		AfxMessageBox("不是分析家日线数据文件格式!");
		return;
	}

	SourceFile.Seek(8,CFile::begin);
	SourceFile.Read(&NumStock,sizeof(int));

	int NumKlinesOfFxj =  (SourceFile.GetLength()-16)/32;

	if( NumKlinesOfFxj<0 )
		return;

	SourceFile.Seek(16,CFile::begin);
	Kline *tianlong=new Kline[NumKlinesOfFxj]; 
	SourceFile.Read(tianlong,32*NumKlinesOfFxj);

	m_progress1.SetRange32(0,NumStock);   

	CTime tm; 
	tm=CTime::GetCurrentTime();
	int CurrentDate=atoi(tm.Format("%Y%m%d"));

	int nFixStyle=GetCheckedRadioButton(IDC_RADIO_COVER,IDC_RADIO_INSTALLDATE); 

	{//
	CString StockSymbol,PreStockSymbol;
	for( int i=0;i<NumKlinesOfFxj;i++)
	{
		if( tianlong[i].day == -1 )
		{
			m_progress1.OffsetPos(1);  


			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
           if(m_nImportorcancel==2)
		   {
				SourceFile.Close();   
				delete []tianlong; 
			   return;
		   }
		  StockSymbol=(char*)&tianlong[i]+6;
			if(StockSymbol.GetLength () == 6||StockSymbol.GetLength () == 4) 
			{
				
				InstallStockKlineEach( PreStockSymbol,0, BeginDate, EndDate);
		
				m_InstallArray.RemoveAll();
			}
		  PreStockSymbol=StockSymbol;
		}
		else
		{
			tm=tianlong[i].day;
			if( tm>0 && atoi(tm.Format("%Y%m%d")) <=CurrentDate )
				m_InstallArray.Add(tianlong[i]);
		}
	}
	}
	delete []tianlong;  
    m_progress1.SetPos(0);
	SourceFile.Close();  

}
void CImportData::InstallHuijin(CString srcfilename, int BeginDate, int EndDate) 
{
	if (shijianflag==1)
	{
		if(  BeginDate<19700101 ||BeginDate>20380101||
			 EndDate  <19700101 ||EndDate>20380101 )
		{
			AfxMessageBox("必须输入正确的时间!");
			return;
		}
		if( BeginDate>EndDate)
		{
			AfxMessageBox("输入时间非法!",MB_ICONASTERISK);
			return;
		}
	}
	{

	
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		m_DataSource.GetWindowText(FinderName);
	    if(!finder.FindFile(FinderName+"\\*.*"))
		{
			AfxMessageBox("打开文件出错,请检查相关设置!");
			return;
		}
		BOOL bwork = finder.FindFile(FinderName+"\\*.*");
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("没有要安装的数据!");
			return;
		}

		m_progress1.SetRange32(0,NumFile);

	
		Kline Tianlong;
		CString StockName; 
		CString szTime;
		CTime tm;

	
		BOOL bWorking = finder.FindFile(srcfilename+"\\*.*");
		while( bWorking )
		{
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
		    
			if(m_nImportorcancel==2)
				return;
			//
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

		
			CString HuijinFileName=finder.GetFilePath();
			CFile HuijinFile(HuijinFileName,CFile::modeRead);
			int FileLength=HuijinFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength()!=8 && StockName.GetLength()!=6) 	continue;  //if not shanghai , then return.
			StockName=StockName.Right(StockName.GetLength()-2);

			m_InstallArray.RemoveAll();    
			
			int numKline=FileLength/29;

			if( numKline<=0 )
			{
				HuijinFile.Close();
				continue;
			}

			HUIJIN hj;
			HUIJIN* huijin=new HUIJIN[numKline];
			for( int j=0; j < numKline; j++)
			{

				HuijinFile.Read( &hj, 29 );
				huijin[j]=hj;
			}
			HuijinFile.Close();

			LONG time;
			for( int i=0; i < numKline; i++)
			{
				if( GetStockTime(huijin[i].nDate,time) )
				{
					Tianlong.day   = time;
					Tianlong.open   = huijin[i].fOpen;
					Tianlong.high   = huijin[i].fHigh;
					Tianlong.low    = huijin[i].fLow;
					Tianlong.close  = huijin[i].fClose;
					Tianlong.vol = (float)huijin[i].nVolume;
					Tianlong.amount = (float)(huijin[i].nVolume*huijin[i].fMean*100.0);
					Tianlong.advance= 0;
					Tianlong.decline= 0;
					m_InstallArray.Add(Tianlong);
				}
			}
			delete []huijin;
			
			//
			if(m_InstallArray.GetSize()==0)	
				continue;
		
			InstallStockKlineEach( StockName,0, BeginDate, EndDate);
		
		}
			
	  
		}
        m_progress1.SetPos(0);

}



void CImportData::InstallShenglong(CString srcfilename, int BeginDate, int EndDate)
{

	if (shijianflag==1)
	if(  BeginDate<19700101 ||BeginDate>20380101||
		 EndDate  <19700101 ||EndDate  >20380101|| 
		 BeginDate>EndDate   )
	{
		AfxMessageBox("非法输入!重新输入正确的时间!");
		return;
	}

	{ 
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
	
		m_DataSource.GetWindowText(FinderName);
		if(!finder.FindFile(FinderName+"\\*.day"))
		{
			AfxMessageBox("打开文件出错,请检查相关设置!");
			return;
		}
		BOOL bwork = finder.FindFile(FinderName+"\\*.day");
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("没有要安装的数据!");
			return;
		}


		
		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		CString StockName, /*股票代码*/ 	szTime;
		CTime tm;

	
		BOOL bWorking = finder.FindFile(srcfilename+"\\*.day");
		while( bWorking )
		{
	
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			
			if(m_nImportorcancel==2)
				return;
			
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

	
			CString ShenglongFileName=finder.GetFilePath();
			CFile ShenglongFile(ShenglongFileName,CFile::modeRead);
			int FileLength=ShenglongFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength() != 6&&StockName.GetLength() != 4 ) continue;
						
			int NumShenglong=FileLength/sizeof(SHENGLONG);                
			
			if( NumShenglong <= 0 )
			{
				ShenglongFile.Close();
				continue;
			}

			SHENGLONG* SLarray=new SHENGLONG[NumShenglong];
	        
			ShenglongFile.Read( SLarray, sizeof(SHENGLONG)*NumShenglong );
			ShenglongFile.Close();

			Kline wsh;
			for( int i=0; i < NumShenglong; i++)
			{
				if( ShenglongToTianlong(SLarray[i],wsh) )
					m_InstallArray.Add(wsh);
			}
			delete []SLarray;

			if( m_InstallArray.GetSize()==0 )
				continue;
		
			InstallStockKlineEach( StockName,0, BeginDate, EndDate);
	
	
		} 
	}
	
	m_progress1.SetPos(0);

}
BOOL CImportData::ShenglongToTianlong(SHENGLONG shenglong,Kline& Wanshen)
{
	shenglong.date=~shenglong.date;
	shenglong.oprice=~shenglong.oprice;
	shenglong.cprice=~shenglong.cprice;
	shenglong.hprice=~shenglong.hprice;
	shenglong.lprice=~shenglong.lprice;
	shenglong.tovalue=~shenglong.tovalue;
	shenglong.tohands=~shenglong.tohands;
	shenglong.topiece=~shenglong.topiece;
	
	LONG tm;
	if(  GetStockTime(shenglong.date,tm) )
	{
		Wanshen.day    = tm; 
		Wanshen.open   = float(shenglong.oprice/1000.0);
		Wanshen.high   = float(shenglong.hprice/1000.0);
		Wanshen.low    = float(shenglong.lprice/1000.0);
		Wanshen.close  = float(shenglong.cprice/1000.0);
		Wanshen.vol    = float(shenglong.tohands);
		Wanshen.amount = float(shenglong.tovalue*1000.0);
		Wanshen.advance=	 0;
		Wanshen.decline=	 0;
		return 1;
	}

	return 0;

	
}
void CImportData::InstallMinteData()
{
	
	CString SourceFilename,srcfilename;
	m_DataSource.GetWindowText(SourceFilename);
	srcfilename = SourceFilename;
	BOOL IsSoft=FALSE;
	
	if(IsFileType(srcfilename)!=1)
	{
		AfxMessageBox("非合法五分钟数据!");
		return;
	}

	if( srcfilename.Mid(0,3)=="a:\\"||srcfilename.Mid(0,3)=="b:\\"||srcfilename.Mid(0,3)=="A:\\"||srcfilename.Mid(0,3)=="B:\\")
	{
		if( !InstallFromFloppyDisk(srcfilename,false) )
		{
			AfxMessageBox("安装没能正常进行!",MB_ICONASTERISK);
			return;
		}
		IsSoft=TRUE;
		srcfilename="c:\\source.min";
	}
	else
	{
		srcfilename.MakeLower();
		if( srcfilename.Find("tl_disk") != -1 )
		{
			if( ! InstallFromSeveralDiskfile(srcfilename,false) )
			{
				AfxMessageBox("安装没能正常进行!",MB_ICONASTERISK);
				return;
			}
			IsSoft=TRUE;
			srcfilename="c:\\source.min";
		}
	}

	UpdateData(TRUE);	

	CFile SourceFile;
	if(!SourceFile.Open(srcfilename,CFile::modeRead))
	{
		AfxMessageBox("路径中包括错误的路径,请重新设置!");
		return;
	}
	

	int NumStock=0 ;             
	int FileID;  
	SourceFile.Read(&FileID,4);
	//if( FileID!=FILEID22)
	//{
	//	AfxMessageBox("非"+g_strCompanyName+"五分钟数据格式!");
	//	return;
	//}
	SourceFile.Read(&NumStock,sizeof(int));
	
	m_progress1.SetRange32(0,NumStock);
	

	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;   
	//  add begin46
	int wMarketType;
	//  add end46
	for( int nStock=0; nStock<NumStock; nStock++)
	{

		MSG message;
		if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}  

		
		m_progress1.SetPos(nStock);
		m_InstallArray.RemoveAll();

	
		SourceFile.Read(StockName,7);
		//  add begin46
		SourceFile.Read(&wMarketType,sizeof(int));
		//  add end46
		SourceFile.Read(&NumKline,4);
		SourceFile.Seek(8,CFile::current);
		for( int nKline=0; nKline<NumKline*48; nKline++)
		{
			SourceFile.Read( &wanshen,sizeof(Kline) );
			m_InstallArray.Add(wanshen);
		}
	
        if(NumKline<=0)
			continue;
		CString symbol(StockName);
		
		if(StockName[0]=='8'&&(StockName[1]=='k'||StockName[1]=='K'))
			continue;
		CTime tm(m_InstallArray[0].day);
		int BeginDate =  atoi(tm.Format ("%Y%m%d"));
		tm = (m_InstallArray[m_InstallArray.GetSize()-1].day);
		int EndDate =  atoi(tm.Format ("%Y%m%d"));
		InstallStockKlineEach( symbol,wMarketType ,BeginDate, EndDate,false);
	
	}


	SourceFile.Close();
    if(IsSoft==TRUE)
        SourceFile.Remove(srcfilename);
	m_progress1.SetPos(0);

}

void CImportData::InstallMin(CString lpStockSymbol)
{
    CFileFind finder;
	BOOL bWorking;
	CString StockOpenPath;
	if(!(bWorking=finder.FindFile(lpStockSymbol+"\\*.1mn")))
	{
		AfxMessageBox(" 不存在分时数据文件!");
	    return ;
	}
	
	int NumFile=0;
	while(bWorking)
	{
		bWorking=finder.FindNextFile();
		NumFile++;
	}
	if( !NumFile )
	{
		AfxMessageBox("没有要安装的数据!");
		return;
	}
    bWorking=finder.FindFile(lpStockSymbol+"\\*.1mn");
	//
	m_progress1.SetRange(0,NumFile);
	m_progress1.SetPos(0);
	while(bWorking)
	{
        bWorking=finder.FindNextFile();
		StockOpenPath=finder.GetFilePath();
	
        CString Stocklen=finder.GetFileTitle();

        InstallOneMinStock(StockOpenPath);
	    m_progress1.OffsetPos(1);
	
	}

}

void CImportData::InstallOneMinStock(CString FilePath)
{
    CFile StockFile;
	CReportData* pcdat1;
	CdatOld  pcdat2;
    if(!StockFile.Open(FilePath,CFile::modeRead|CFile::shareDenyNone))
	{
	
		return;
	}
	StockFile.Read(&pcdat2,sizeof(CdatOld));
	StockFile.Close();
    if( ! CMainFrame::m_taiShanDoc->m_sharesInformation.Lookup(pcdat2.id, pcdat1,pcdat2.kind) )
	    return;
    pcdat1->lastclmin=239;

	pcdat1->rdp=pcdat2.rdp;           
	strcpy(pcdat1->id,pcdat2.id); 
	//  add begin46
	pcdat1->kind=pcdat2.kind;
	//  add end46
	strcpy(pcdat1->name,pcdat2.name);        
	strcpy(pcdat1->Gppyjc,pcdat2.Gppyjc);       
    //	
	pcdat1->ystc=pcdat2.ystc;           
	pcdat1->opnp=pcdat2.opnp;           
	pcdat1->higp=pcdat2.higp;          
	pcdat1->lowp=pcdat2.lowp;            
	pcdat1->nowp=pcdat2.nowp;            
	pcdat1->nowv=pcdat2.nowv;             
	pcdat1->totv=pcdat2.totv;             
	pcdat1->totp=pcdat2.totp;            
	pcdat1->pbuy1=pcdat2.pbuy1;          
	pcdat1->vbuy1=pcdat2.vbuy1;           
	pcdat1->pbuy2=pcdat2.pbuy2;            
	pcdat1->vbuy2=pcdat2.vbuy2;           
	pcdat1->pbuy3=pcdat2.pbuy3;             
	pcdat1->vbuy3=pcdat2.vbuy3;             
	pcdat1->psel1=pcdat2.psel1;          
	pcdat1->vsel1=pcdat2.vsel1;           
	pcdat1->psel2=pcdat2.psel2;           
	pcdat1->vsel2=pcdat2.vsel2;           
	pcdat1->psel3=pcdat2.psel3;            
	pcdat1->vsel3=pcdat2.vsel3;      
	pcdat1->accb=pcdat2.accb;         
	pcdat1->accs=pcdat2.accs;             
	pcdat1->volume5=pcdat2.volume5;           
	pcdat1->rvol=pcdat2.rvol;            
	pcdat1->dvol=pcdat2.dvol;            
	pcdat1->lastclmin=pcdat2.lastclmin;      
	pcdat1->initdown=pcdat2.initdown;        
	pcdat1->InOut=pcdat2.InOut;

    for(int i=0;i<240;i++)
   {
	   pcdat1->m_Kdata1[i].Price=pcdat2.m_Kdata[i].Price;
	   pcdat1->m_Kdata1[i].Amount=pcdat2.m_Kdata[i].Amount;
	   pcdat1->m_Kdata1[i].Volume=pcdat2.m_Kdata[i].Volume;
   }
}


int CImportData::IsFileType(CString Filename)
{
    Filename.TrimRight(" ");
    if(Filename.Right(4)==".day"||Filename.Right(4)==".DAY")
		return 0;
	else if(Filename.Right(4)==".min"||Filename.Right(4)==".min")
		return 1;
	else if(Filename.Right(4)==".1mn"||Filename.Right(4)==".1MN")
		return 2;
	else if(Filename.Right(4)==".dad"||Filename.Right(4)==".DAD")
		return 3;
	else 
		return 4;
}

void CImportData::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
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
	//
	m_listfw.SetItem(&myitem);
	m_listfw.Update(ItemIndex);

	*pResult = 0;
}

BOOL CImportData::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	DoHtmlHelp(this);return TRUE;
	

}
