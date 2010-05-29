// CTaiShanApp.cpp : Defines the class behaviors for the application.
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ChooseReceiveDataForm.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "CTaiShanKlineShowView.h"
//#include "CompanyInfoView.h"
#include "MyTreeView.h"
#include "MyRichView.h"
#include "InfoView.h"
////////////////////
#include "NineShowView.h"
#include "NineShowFrm.h"
#include "MySplash.h"
#include "GuiZhouTvView.h"
#include "UserRegPart2.h"
#include "CTaiKlineFileKLine.h"
#include "FyRegister.h"

#include <afxpriv.h>


#include "CaiwuData.h"
#include  "CTaiChuQuanInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef __cplusplus
extern "C" {
extern long ReadDog( void );
extern long WriteDog( void );
extern short DogAddr,DogBytes;
extern void * DogData;
}
#else
extern long ReadDog( void );
extern long WriteDog( void );
extern short DogAddr,DogBytes;
extern void * DogData;
#endif
extern HANDLE hDevice;

int CTaiShanApp::m_gMessageID = 0;
bool CTaiShanApp::m_gbUseExe = false;
bool CTaiShanApp::m_gbDoInitate = true;

void DoHtmlHelp(CWnd* pWnd,int nID)
{
	if(pWnd)

	::ShellExecute( pWnd->m_hWnd, "open", "", NULL, NULL, SW_SHOWNORMAL);
};

BEGIN_MESSAGE_MAP(CTaiShanApp, CWinApp)
	//{{AFX_MSG_MAP(CTaiShanApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	//}}AFX_MSG_MAP
	
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)

	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()



CTaiShanApp::CTaiShanApp()
{
	
    Line_mode = 5;
	m_key = 0;
	m_bAppAuthorized=TRUE;
	m_gbUseExe = false;
}




CTaiShanApp theApp;



STARTUPINFO startup;
PROCESS_INFORMATION process;

CString g_strCompanyName = "";

BOOL CTaiShanApp::InitInstance()
{
	AfxEnableControlContainer();

	m_gMessageID = ::RegisterWindowMessage("WsSendMessageHqData");
	CFileFind fnd;
	if(S_OK != ::CoInitialize (NULL))
		return FALSE;

	ReadDiskIDPartCwd();
	if(!FyRegister::IsValidUser())
		return FALSE;

   memset( &startup, 0, sizeof( startup ) );
   startup.cb = sizeof( startup );
   memset( &process, 0, sizeof( process ) );
	if(fnd.FindFile ("WsSendMessageShare.exe"))
	{
		m_gbUseExe = true;
	}

	hAppMutex=::CreateMutex(NULL,TRUE,m_pszExeName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CWnd *pPrevWnd = CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while(pPrevWnd)
		{
			if(::GetProp(pPrevWnd->GetSafeHwnd(),m_pszExeName))
			{
			  if(pPrevWnd->IsIconic())
				  pPrevWnd->ShowWindow(SW_RESTORE);
			  pPrevWnd->SetForegroundWindow();
			  pPrevWnd->GetLastActivePopup()->SetForegroundWindow();
			  return false;
			} 
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		return false;
	}


	int nResult;
	m_bAppAuthorized=TRUE;



	// 设置当前文件夹
	TCHAR pszPath[MAX_PATH];
	::GetModuleFileName(NULL, pszPath, MAX_PATH);

	CString strPath = pszPath;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	::SetCurrentDirectory(strPath);

	::GetCurrentDirectory(MAX_PATH, pszPath);



    CTaiTestSplash *m_splash;
	BOOL SplashOpen=FALSE;
	m_splash = new CTaiTestSplash;
	SplashOpen=m_splash->Create();
	if( SplashOpen )
		m_splash->ShowWindow(SW_SHOW);
	DWORD Currenttime=GetTickCount();
	BeginWaitCursor();

#ifdef TEST_USER1
	t = CTime::GetCurrentTime();
    CTime t2 = g_timeUseEnd;
	if(t >= t2)
	{

	}
	else
	{
	}
#endif

	AfxInitRichEdit( );
#ifdef _AFXDLL
	Enable3dControls();		
#else
	Enable3dControlsStatic();	
#endif

	
	UpdateTheOEMInfo();	

	SetRegistryKey("");
	LoadStdProfileSettings();  


#ifdef WIDE_NET_VERSION
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME_WIDE,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), 
		RUNTIME_CLASS(CTaiShanReportView));
	AddDocTemplate(pDocTemplate);


    pDrawTemplate = new CMultiDocTemplate(
		IDR_VWBASETYPE_WIDE,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), 
		RUNTIME_CLASS(CTaiShanKlineShowView));
#else
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), 
		RUNTIME_CLASS(CTaiShanReportView));
	AddDocTemplate(pDocTemplate);


    pDrawTemplate = new CMultiDocTemplate(
		IDR_NEWTYPE,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CTaiShanKlineShowView));
#endif
	

    m_pBroadcastDocTemplate = new CMultiDocTemplate(
		IDR_BASEINFO,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame), 
		RUNTIME_CLASS(CTaiTestRichView)); 

    
	pNineShowTemplate = new CMultiDocTemplate(
		IDR_NINE_VIEW,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CNineShowFrame),
		RUNTIME_CLASS(CNineShowView));

	m_pMyNewTemplate=new CMultiDocTemplate(
		IDI_ICON1,
		RUNTIME_CLASS(CTaiShanDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(CGuiZhouTvView));

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;


	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CString sCommand = this->m_lpCmdLine;
	if(sCommand.GetLength ()>0)
		m_gbDoInitate = false;

	
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_nCmdShow |= SW_SHOWMAXIMIZED  ;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();	
	Currenttime=GetTickCount() - Currenttime;
    if(Currenttime<8000)
	{
		if(SplashOpen)
		{
			try
			{
				m_splash->SetWindowPos(&(CWnd::wndTop   ),0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
			}
			catch(...)
			{
			}
		}
		else
			delete m_splash;
	}
    ::SetProp(pMainFrame->GetSafeHwnd(),m_pszExeName,(HANDLE)1);
	EndWaitCursor(); 

	pMainFrame->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);

	if(m_gbUseExe == true)
	{
		BOOL tSuccess = CreateProcess( _T("WsSendMessageShare.exe"), NULL, NULL, NULL,
						FALSE, 0, NULL, NULL, &startup, &process );
	}
	return TRUE;
}




class CAboutDlg : public CDialog
{
public:
	CAboutDlg();


	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStatic	m_static_Icon;
	CString	m_strCredit;
	CString	m_strSerial;
	CString	m_strCompany;
	CString	m_strUser;
	CString	m_strPName;
	//}}AFX_DATA

	
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strCredit = _T("");
	m_strSerial = _T("");
	m_strCompany = _T("");
	m_strUser = _T("");
	m_strPName = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STA_ICON, m_static_Icon);
	DDX_Text(pDX, IDC_STATIC_COMPANY, m_strCompany);
	DDX_Text(pDX, IDC_STATIC_USER, m_strUser);
	DDX_Text(pDX, IDC_STATIC_PNAME, m_strPName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()


void CTaiShanApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTaiShanApp * pApp = (CTaiShanApp*)AfxGetApp();
	
	
	CTaiShanDoc *pDoc = CMainFrame::m_taiShanDoc;
	

	return TRUE; 
	              
}


int CTaiShanApp::ExitInstance() 
{
	if(pDrawTemplate)
		delete pDrawTemplate;

    ReleaseMutex(hAppMutex);




	return CWinApp::ExitInstance();
}

void CTaiShanApp::OnFilePrintSetup() 
{

	try
	{
		CWinApp::OnFilePrintSetup();
	}
	catch(...)
	{
	}
	
}
BOOL CTaiShanApp::OnIdle(LONG lCount)
{
	BOOL bMore = CWinApp::OnIdle(lCount);

	CTaiShanDoc* pDoc = (CTaiShanDoc*)(((CMainFrame*)m_pMainWnd)->m_taiShanDoc);
	if(pDoc==NULL)
	   return bMore;

	if ( pDoc->IdleProc( lCount ) )
	   bMore = TRUE;
	return bMore;
}

CLongString::CLongString()
{
	m_nMax = 4096;
	m_nCurrentNum =0;
	m_nAdd = 4096;
	m_pData = new char[4096];
	memset(m_pData,0,m_nMax);
}

CLongString::~CLongString()
{
	if(m_pData!=NULL)
		delete []m_pData;
}
void CLongString::operator+(CString s)
{
	int n = s.GetLength ();
	if(n+m_nCurrentNum>=m_nMax)
	{
		m_nMax = ((n+m_nCurrentNum)/m_nAdd+1)*m_nAdd;
		if(m_nMax<=0)
			m_nMax = 1;
		char* pch = new char[m_nMax];
		strcpy(pch,m_pData);

		delete []m_pData;
		m_pData = pch;
	}
	strcpy(m_pData+m_nCurrentNum,s);
	m_nCurrentNum+=n;
}

int CLongString::GetLength()
{
	return m_nCurrentNum;
}

CString CLongString::FloatToString(float fVal)
{
	CString s;
	s.Format ("%.3f",fVal);
	if(s.Find (".")>=0)
	{
		s.TrimRight ("0");
		s.TrimRight (".");
	}

	return s;
}

void CLongString::FloatArrayToExcelString(CLongString & longStr,float *pFloat, int nTot, int nBegin)
{
	CString s;
	for(int i=0;i<nTot;i++)
	{
		if(i<nBegin)
			s="    ";
		else
			s = FloatToString(pFloat[i]);
		s = s+"\t";
		longStr+s;
	}
	s="\n";
	longStr+s;

}

void CTaiShanApp::DispatchMessageEachTime()
{
		MSG message;
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			if (message.message != WM_KICKIDLE && !theApp.PreTranslateMessage(&message))
			{
				 TranslateMessage(&message);
				 DispatchMessage(&message);
			}
		}
}

int CTaiShanApp::Run() 
{
	
	ASSERT_VALID(this);

	
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;

	CFile file;
	file.Open ("reportError.txt",CFile::modeCreate|CFile::modeWrite);

	MSG m_msgCur;
	for (;;)
	{
	
		while (bIdle &&
			!::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE))
		{
		
			if (!OnIdle(lIdleCount++))
				bIdle = FALSE; 
		}

		
#ifndef _DEBUG
	try
	{
#endif 
		do
		{
		
			if (!PumpMessage())
				return ExitInstance();

		
			if (IsIdleMessage(&m_msgCur))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
#ifndef _DEBUG
		}


		catch(...)
		{
			CString s;
			int n = GetLastError();
			s.Format("%d\r\n",n);
			file.Write(s.GetBuffer(s.GetLength()),s.GetLength());
			if(!this->m_pMainWnd->IsWindowEnabled())
				m_pMainWnd->EnableWindow();
		}
#endif

	}

	ASSERT(FALSE);  

	

}

void CTaiShanApp::ToTerminateChildProcess()
{
	if(process.hProcess !=0)
	{
		::TerminateProcess (process.hProcess,9);
	}
}

CString CTaiShanApp::GetStkName( LPCSTR lpszLabel, WORD wMarket )
{
	CString s = "";
	int kind = CMainFrame::m_taiShanDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
			s = pStockData->name ;
	}
	return s;
}
 

void* CTaiShanApp::GetLineDayData( LPCSTR lpszLabel, WORD wMarket, CTime tm, BOOL bDoPowerSplit )
{
	Kline* pKline = NULL;
	int kind = CMainFrame::m_taiShanDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			CTaiKlineFileKLine * pFile = CTaiKlineFileKLine::GetFilePointer(lpszLabel,kind,true);
			Kline* pKline2 = NULL;
			int n = pFile->ReadKlineAccordingTime(lpszLabel, pKline2, tm ,2,0);
			if(n>0)
			{
				static Kline retval;
				pKline = &retval;
				pKline[0] = pKline2[n-1];
			}
			if(pKline2)
				delete [] pKline2;
		}
	}
	return pKline;
}
 

void * CTaiShanApp::GetBaseInfoData( LPCSTR lpszLabel, WORD wMarket)
{
	BASEINFO* pBase = NULL;
	int kind = CMainFrame::m_taiShanDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			pBase = pStockData->pBaseInfo;
		}
	}
	return pBase;
}
 

void CTaiShanApp::SwitchToReportView( LPCSTR lpszLabel, WORD wMarket)
{
	CMainFrame::m_taiShanDoc ->m_WsStock2000View->GetParentFrame()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
	if( NULL == lpszLabel || 0 == strlen(lpszLabel) )
		return;				
	int kind = CMainFrame::m_taiShanDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{			
			CMainFrame::m_taiShanDoc ->m_WsStock2000View->ScrollToSelectStock((char*)lpszLabel,kind);
		}
	}
}
 

void CTaiShanApp::SwitchToKlineView( LPCSTR lpszLabel, WORD wMarket)
{
	int kind = CMainFrame::m_taiShanDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			CMainFrame::m_taiShanDoc->m_sharesSymbol = lpszLabel;
			CMainFrame::m_taiShanDoc->m_stkKind = kind;
			CMainFrame::m_taiShanDoc->m_GetFocus = true;
			AfxGetMainWnd()->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)lpszLabel,(LPARAM)(2+CMainFrame::m_taiShanDoc->m_stkKind *10));
		}
	}
}
 

void CTaiShanApp::SwitchToF10View( LPCSTR lpszLabel, WORD wMarket)
{
	int kind = CMainFrame::m_taiShanDoc ->m_sharesInformation .GetStockKind(wMarket,(char*)lpszLabel);
	if(kind>=0)
	{
		PCdat1 pStockData = NULL;
		if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup ((char*)lpszLabel,pStockData,kind))
		{
			CMainFrame::m_taiShanDoc->m_sharesSymbol = lpszLabel;
			CMainFrame::m_taiShanDoc->m_stkKind = kind;
			CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
			((CMainFrame *)pMainWnd)->OnCompanyinfo();
		}
	}
}
 

void  CTaiShanApp::OnKeyboardGeniusCmd( LPCSTR lpszCmd )
{
	if(CMainFrame::m_taiShanDoc ->m_WsStock2000View)
	{
		CMainFrame::m_taiShanDoc ->m_WsStock2000View->HotKeyDownProgress((char*)lpszCmd);
	}
}


COLORREF CTaiShanApp::GetColorScheme( int nColorIndex )
{
	COLORREF colorRetVal;

	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc;
	if( NULL == pDoc )
		return RGB(0,0,0);
	switch( nColorIndex )
	{
	case 0:		
		colorRetVal = pDoc->m_colorArray[18];
		break;
	case 1:		
		colorRetVal = pDoc->m_colorArray[17];
		break;
	case 2:		
		colorRetVal = pDoc->m_colorArray[13];
		break;
	case 3:		
		colorRetVal = pDoc->m_colorArray[15];
		break;
	case 4:		
		colorRetVal = pDoc->m_colorArray[14];
		break;
	case 5:			
		colorRetVal = pDoc->m_colorArray[16];
		break;

	default:
		ASSERT( FALSE );
	}
	
	return colorRetVal;
}


BOOL CTaiShanApp::GetFontScheme( int nFontIndex, LOGFONT & OutLogFont )
{
	return FALSE;
}


void CTaiShanApp::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
}

void CTaiShanApp::UpdateTheOEMInfo()
{
}
