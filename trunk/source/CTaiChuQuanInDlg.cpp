// CTaiChuQuanInDlg.cpp : implementation file
// by 俞明录

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiChuQuanInDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanInDlg dialog


CTaiChuQuanInDlg::CTaiChuQuanInDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiChuQuanInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiChuQuanInDlg)
	m_InFile = _T("");
	m_FormName = _T("");
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_taiShanDoc;
}


void CTaiChuQuanInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiChuQuanInDlg)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_READ, m_read);
	DDX_Control(pDX, IDC_IMPORT, m_import);
	DDX_Control(pDX, 1011, m_PowerForm);
	DDX_Text(pDX, IDC_INFILE, m_InFile);
	DDX_Text(pDX, IDC_STATIC_NAME, m_FormName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiChuQuanInDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiChuQuanInDlg)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_CBN_SELCHANGE(1011, OnSelchangeGeshi)
	ON_BN_CLICKED(IDC_READ, OnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CTaiChuQuanInDlg::OnImport() 
{

	UpdateData(TRUE);

	FXJPOWER power_fxj;

	int nSplitMask=0;
	int nTotalCount=0;
	int nEachChuQuanNo=0;


	CFile InFile;
	if(!(InFile.Open(m_InFile,CFile::modeRead,NULL)))
	{
		MessageBox("引入除权文件不可读!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	CString StockSymbol;     
	int Marker;             


	m_FormName="正在引入除权数据...";

    GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_FormName);
	Split pSplit[80];
	BOOL IsSucc;
	if( m_PowerForm.GetCurSel() == 0 )
	{
		InFile.Close();
		IsSucc=InstallPowerData(m_InFile);
		
	}


	else 
	{
		if( m_PowerForm.GetCurSel() == 1 )
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

				   //lmb add
				   StockSymbol.MakeLower ();
				   bSh = false;
				   if(StockSymbol.Left (2) == "sh") bSh = true;

				   if( StockSymbol.GetLength() == 8 )
                      StockSymbol=StockSymbol.Right(6);
				   else if(StockSymbol.GetLength() == 6)
					  StockSymbol=StockSymbol.Right(4);
				  
				   InFile.Read(&power_fxj, sizeof(FXJPOWER));
				   
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
					InFile.Read(&power_fxj, sizeof(FXJPOWER));
					
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
   
	if(IsSucc)
	   m_FormName="引入完毕!";
	else
		m_FormName="引入失败!";
	UpdateData(FALSE);
	
}

BOOL CTaiChuQuanInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_PowerForm.InsertString(0,""+g_strCompanyName+"除权数据格式");
	m_PowerForm.InsertString(1,"分析家除权数据格式");
	m_PowerForm.SetCurSel(0);
	m_InFile="C:\\dest.pwr";
	UpdateData(FALSE);
	return TRUE;  
}

void CTaiChuQuanInDlg::OnSelchangeGeshi() 
{

	switch( m_PowerForm.GetCurSel() )
	{
		case 0:
			m_InFile="C:\\dest.pwr";
			m_FormName="必须是"+g_strCompanyName+"格式的数据!";
		
			UpdateData(FALSE);
			break;
		case 1:
			m_InFile="C:\\split.pwr";
			m_FormName="必须是分析家格式的数据!";
		
			UpdateData(FALSE);
			break;
	}
	
}

void CTaiChuQuanInDlg::OnRead() 
{
	
    CString DefExt,FileName,Filter;
	if(m_PowerForm.GetCurSel()==0)
	{
	    DefExt="*.pwr";
	    FileName="dest.pwr";
	    Filter="split file (*.pwr)|*.pwr||";
	}
	else
	{
	    DefExt="*.pwr";
	    FileName="split.pwr";
	    Filter="split file (*.pwr)|*.pwr||";
	}
	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bOpenFileDialog.DoModal() ==IDOK )
		m_InFile=bOpenFileDialog.GetPathName();

	UpdateData(FALSE);

	
}

BOOL CTaiChuQuanInDlg::InstallPowerData(CString FilePath)
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
    if(nFlag!=FILEID22)
	{
		AfxMessageBox("不是"+g_strCompanyName+"除权数据格式!");
		InFile.Close();
		return FALSE;
	}
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
			
				
			
			::ZeroMemory(pSplit,80*sizeof(Split));
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
