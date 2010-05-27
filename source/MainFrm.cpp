// MainFrm.cpp : implementation of the CMainFrame class
//


#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "managerstocktype.h"
//#include "vwbase.h"
#include "CTaiShanKlineShowView.h"
#include "ChildFrm.h"
#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "MyTreeView.h"
#include "cellRange.h"
#include "shlwapi.h"
#include "DownLoadDlg.h"
#include "WideDownLoadDlg.h"
#include "WideDownLoadDlg.h"
#include "CTaiShanReportView.h"
#include "InfoView.h"
#include "MainFrm.h"
#include  "afxpriv.h"
#include  "DIALOGGSGL.h"
#include "BgridCtrl.h"
#include "CSharesBigBillDlg.h"
#include "CTaiScreen.h"
#include "CTaiKlineIndicatorsManage.h"
#include "CTaiScreenTest.h"
#include "CAlertSystem.h"
#include "CAlertPopDlg.h"
#include "RecvStockCode.h"
#include "NineShowFrm.h"
#include "GetSetReg.h"
//Added By CuiWei.2000-08-03
#include	"ManageData.h"
#include	"ImportData.h"       
#include	"ExportData.h"       
#include	"ClearData.h"        

#include	"SymbolContrast.h"   
#include	"ManageInvest.h"
#include	"CaiwuData.h"
#include	"ChuquanData.h"

#include    "investlogin.h"
//#include    "RiskIndex.h"

#include    "Option.h"
#include    "Sysface.h"
#include    "Syssetup.h"
#include    "SysSetParaPage.h"
#include    "direct.h"
#include "CTaiKlineDlgNeuralLearn.h"
#include "DlgMYXMZ.h"

//added by qyp 2001.10.9
#include "LogonDlg.h"
#include "DlgShowInformation.h"
#include "XgnExtDataDlg.h"
#include "CwdCloseWork.h"
//#include "DownFilesDlg.h"

//
//Added by Xia Qiankun 2000/5/11
//
//#include "LoginDlg.h"
//#include "InvestManageDlg.h"
//end

#include "DlgMYXMZ.h"

#define ID_SIZECHANGE   2
#define ID_GPSJCL   200
#define TJXG_ALERT  400   
#define MaxShareMem   (1024*1024)

#include "StkReceiver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef WIDE_NET_VERSION
CClientTransmitManageWnd* CMainFrame::m_pClientTransmitManageWnd;

BOOL CMainFrame::m_bLogonSuccess;
#endif
CTaiShanDoc * CMainFrame::m_taiShanDoc = NULL;
CTaiScreen *CMainFrame::dialogtjxg;
CTaiKlineDlgNeuralLearn *CMainFrame::m_pDlgNL;
CTaiScreenTest *CMainFrame::dialogtjxgmmtest;

bool m_gbDestroyed = false;
IMPLEMENT_DYNCREATE(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(DA_SERVICE_MSG_EXT,OnExtService)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_VIEW_COMPANYINFO, OnViewCompanyinfo)
	ON_COMMAND(ID_VIEW_GGXX, OnViewGgxx)
	ON_COMMAND(ID_FULLSCREEN, OnFullscreen)
	ON_COMMAND(ID_NOMALSCREEN, OnNomalscreen)
	ON_WM_GETMINMAXINFO()
	ON_WM_NCPAINT()
	ON_COMMAND(ID_TOOL_SJGL, OnToolSjgl)
	ON_COMMAND(ID_TOOL_GSGL, OnToolGsgl)
	ON_COMMAND(ID_TJXG_TJXG, OnTjxgTjxg)
	ON_COMMAND(ID_VIEW_XX, OnViewXx)
	ON_COMMAND(ID_STOP_CUOHE, OnStopCuohe)
	ON_COMMAND(ID_TOOL_TZGL, OnToolTzgl)
	ON_COMMAND(ID_RISKINDEX, OnToolRiskIndex)
	ON_UPDATE_COMMAND_UI(ID_FULLSCREEN, OnUpdateFullscreen)
	ON_UPDATE_COMMAND_UI(ID_NOMALSCREEN, OnUpdateNomalscreen)
	ON_COMMAND(ID_STOCK_BIG, OnOpenBigBill)
	ON_COMMAND(ID_F3, OnF3)
	ON_COMMAND(ID_F4, OnF4)
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_TOPIC, OnHelpTopic)
	ON_UPDATE_COMMAND_UI(ID_TOOL_GSGL, OnUpdateToolGsgl)
	ON_UPDATE_COMMAND_UI(ID_BASEINFO_GRID, OnUpdateBaseinfoGrid)
	ON_UPDATE_COMMAND_UI(ID_CUOHE_OPEN, OnUpdateCuoheOpen)
	ON_COMMAND(ID_GRID, OnGrid)
	ON_UPDATE_COMMAND_UI(ID_GRID, OnUpdateGrid)
	ON_UPDATE_COMMAND_UI(ID_F3, OnUpdateF3)
	ON_UPDATE_COMMAND_UI(ID_F4, OnUpdateF4)
	ON_COMMAND(ID_WINDOW_NEW, OnWindowNew)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, OnUpdateWindowNew)
	ON_COMMAND(ID_TJXG_ALARM, OnTjxgAlarm)
	ON_COMMAND(ID_TOOL_BLOCK_MANA, OnToolBlockMana)
	ON_COMMAND(ID_DOWNLOAD_20, OnDownload20)
	ON_COMMAND(ID_CLOSE_RECEIVER, OnCloseReceiver)
	ON_UPDATE_COMMAND_UI(ID_CLOSE_RECEIVER, OnUpdateCloseReceiver)

	ON_COMMAND(ID_TJXG_MM_TEST, OnTjxgMmTest)
	ON_COMMAND(ID_TOOL_NNET_TRAIN, OnToolNnetTrain)
	ON_COMMAND(ID_TOOL_ZDWT, OnToolAutoSubmit)
	ON_COMMAND(ID_DAY_LINE, OnDayLine)
	ON_COMMAND(ID_TIMESHARING, OnMin1Line)
	ON_UPDATE_COMMAND_UI(ID_STOCK_BIG, OnUpdateStockBig)
	ON_UPDATE_COMMAND_UI(ID_TJXG_ALARM, OnUpdateTjxgAlarm)
	ON_COMMAND(ID_F6,OnF6)
	ON_WM_PAINT()
	ON_COMMAND(IDM_SORT_INDEX, OnSortIndex)
	ON_COMMAND(ID_XGN_EXT_DATA, OnXgnExtData)
	ON_COMMAND(ID_CLEAR_REALDATA, OnClearRealdata)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND(ID_CALC, OnCalc)
	ON_COMMAND(ID_COMPANYINFO, OnCompanyinfo)
	ON_COMMAND(IDM_GDFX_REGISTE, OnGdfxRegister)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_USER_TJXG,OnTjxg)
	ON_MESSAGE(WM_USER_DrawKlineWindow,DrawKlineWindow)
#ifdef WIDE_NET_VERSION
	ON_COMMAND(ID_LOGIN_SERVER, OnLoginServer)
	ON_COMMAND(ID_RECV_STOCKCODE, OnRecvStockcode)
#endif

	ON_MESSAGE(Gp_Msg_StkData, OnStkDataOK)
END_MESSAGE_MAP()

static UINT palette[] =
{

	ID_PALETTEERASE,
	ID_PALETTEPEN,
	ID_PALETTESELECT,
	ID_PALETTEBRUSH,
	ID_PALETTESPRAY,
	ID_PALETTEPAINT,
	ID_PALETTELINE,
	ID_PALETTEEYEDROP,
	ID_PALETTEMAG,
	ID_PALETTERECT,
	ID_PALETTEROUND,
	ID_PALETTEOVAL
};



LRESULT CMainFrame::OnTjxg(WPARAM wParam, LPARAM lParam)
{

	if(!m_taiShanDoc)
		return 0;
	int length=dialogtjxg->m_resultarray.GetSize();
	m_taiShanDoc->m_StockTypeMap[2].RemoveAll();
	m_taiShanDoc->m_ScreenList.RemoveAll();
	if(length>0)
	{
		for(int i=0;i<length;i++)
		{
			SymbolKind m_SymbolKind;
			CString m_zqdm,m_zqdmkind;
			m_SymbolKind = dialogtjxg->m_resultarray.GetAt(i);
			m_zqdm=m_SymbolKind.m_chSymbol;
			m_zqdmkind=m_taiShanDoc->GetStockKindString(m_SymbolKind.m_nSymbolKind);
			m_zqdmkind+=m_zqdm;
			m_taiShanDoc->m_StockTypeMap[2][m_zqdmkind]=m_zqdm;
			SharesSymbol m_StockSymbol;
			strcpy(m_StockSymbol.sharesymbol,m_SymbolKind.m_chSymbol);
			m_StockSymbol.nKind=m_SymbolKind.m_nSymbolKind;  
			m_taiShanDoc->m_ScreenList.Add(m_StockSymbol);
		}
	}

	POSITION pos = m_taiShanDoc->GetFirstViewPosition(); 
	while (pos != NULL) 
	{
		CView* pView = m_taiShanDoc->GetNextView(pos);
		if(pView->IsKindOf( RUNTIME_CLASS(CTaiShanReportView)))
		{
			CTaiShanReportView *m_View;
			m_View=(CTaiShanReportView *)pView;
			m_View->GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
			m_View->ChangeToPage(TJXGPAGE,TRUE); 

		}
	} 

	return 0;
}

CMainFrame::CMainFrame():m_dlgLeftPop(this)
{
	m_fDLL=FALSE;
	m_pDlgNL = NULL;
	dialogtjxgmmtest = NULL;
	dialogtjxg = NULL;
	m_pDlgAlarmPop = NULL;
	m_pDlgAlarm = NULL;
	m_bAlarmExec = false;
	m_taiShanDoc=NULL;
	m_MDIChildWnd=NULL;

	m_bRunFlag=FALSE;	
	m_EnableF10 = true;
	m_EnableF9 = true;
	m_bFullScreen = FALSE;
	m_BaseInfo = true;

	InformationLoop=0;
	m_drawLineToolBar = new CToolBar;
	m_F9ORF10 = FALSE;

	nLKind[0]=HS_KLINE;
	nLKind[1]=MINUTE1_KLINE;
	nLKind[2]=MIN5_KLINE;
	nLKind[3]=MIN15_KLINE;
	nLKind[4]=MIN30_KLINE;
	nLKind[5]=MIN60_KLINE;
	nLKind[6]=DAY_KLINE;
	nLKind[7]=WEEK_KLINE;
	nLKind[8]=MONTH_KLINE;
	nLKind[9]=YEAR_KLINE ;
	nLKind[10]=MANY_DAY_KLINE;

	CAlertSystem::LoadFile();

#ifdef WIDE_NET_VERSION
	m_pClientTransmitManageWnd=new CClientTransmitManageWnd((CMDIFrameWnd*)this);
	m_bLogonInterface=FALSE;
	m_bLogonSuccess=FALSE;
#endif

	m_pViewMYXMZ = NULL;
}

CMainFrame::~CMainFrame()
{
#ifdef WIDE_NET_VERSION
	delete m_pClientTransmitManageWnd;
#endif

	if (m_pViewMYXMZ) delete m_pViewMYXMZ;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(CTaiShanApp::m_gbUseExe == true)
	{
		if(!m_memFile.OpenShare ("",CFile::modeReadWrite
			|CFile::modeCreate|CFile::shareDenyNone
			|CFile::modeNoTruncate,MaxShareMem,"Ws_SendMessageDrv"  ))
		{
			ASSERT(FALSE);
			AfxMessageBox("不能共享内存！");
		}
	}
	CreateToolDrawLine();


	if(!m_dlgLeftPop.Create(m_dlgLeftPop.IDD,this))
	{
		TRACE0("Failed to create Dialog\n");
		return -1;      
	}

	if(!m_TestDialogbar.Create(IDD_DIALOGBAR,this))
	{
		TRACE0("Failed to create Dialog bar\n");
		return -1;   
	}

	CRect Rect;
	GetClientRect(&Rect);
	m_TestDialogbar.SetWindowPos(NULL,1,0,Rect.Width( )-4,55,SWP_NOMOVE);


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;    
	}

	// Initialize the command bars
	//if (!InitCommandBars())
	//	return -1;

	// Get a pointer to the command bars object.
	//CXTPCommandBars* pCommandBars = GetCommandBars();
	//if (pCommandBars == NULL)
	//{
	//	TRACE0("Failed to create command bars object.\n");
	//	return -1;      // fail to create
	//}

	// Add the menu bar
	//CXTPToolBar* pMenuBar = pCommandBars->SetMenu(_T("Menu Bar"), IDR_MAINFRAME);
	//if (pMenuBar == NULL)
	//{
	//	TRACE0("Failed to create menu bar.\n");
	//	return -1;      // fail to create
	//}

	TSKReceiver()->StartEngine();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif 

void CMainFrame::OnDestroy() 
{
	::RemoveProp(GetSafeHwnd(),AfxGetApp()->m_pszExeName);

	m_gbDestroyed = true;

	if(m_pDlgAlarmPop != NULL)
		m_pDlgAlarmPop->DestroyWindow();
	if(m_pDlgAlarm != NULL)
		m_pDlgAlarm->DestroyWindow();
	if(this->dialogtjxg != NULL)
	{
		dialogtjxg->DestroyWindow();
		dialogtjxg = NULL;
	}
	if(dialogtjxgmmtest != NULL)
	{
		dialogtjxgmmtest->DestroyWindow();
		dialogtjxgmmtest = NULL;
	}
	if(this->m_pDlgNL != NULL)
	{
		m_pDlgNL->DestroyWindow();
		m_pDlgNL = NULL;
	}

	CAlertSystem::SaveFile();

	m_bAlarmExec = false;

	CMDIFrameWnd::OnDestroy();

}



void CMainFrame::OnTimer(UINT nIDEvent) 
{

	CMDIFrameWnd::OnTimer(nIDEvent);

	if(nIDEvent == TJXG_ALERT)
		CAlertSystem::CaclAlarm(this); 

}

static int flagmy=0;


void CMainFrame::Drawline() 
{
	if(flagmy==0)
	{
		flagmy=1;
		CPoint pt(GetSystemMetrics(SM_CXSCREEN) -400,
			GetSystemMetrics(SM_CYSCREEN) / 4);
		FloatControlBar(m_drawLineToolBar, pt);

		ShowControlBar(m_drawLineToolBar, TRUE, FALSE);
		RecalcLayout();
	}
	else
	{
		flagmy=0; 
		BOOL bVisible = ((m_drawLineToolBar->GetStyle() & WS_VISIBLE) != 0);
		ShowControlBar(m_drawLineToolBar, TRUE, FALSE);
		RecalcLayout();
	}


}

int CMainFrame::CreateToolDrawLine()
{

	static int first = 0;
	if(first == 1)
		return 0;
	if (!m_drawLineToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE |CBRS_SIZE_FIXED |
		CBRS_TOP | CBRS_TOOLTIPS)||
		!m_drawLineToolBar->LoadToolBar(IDR_DRAWLINE)
		)
	{
		TRACE0("Failed to create toolbar\n");
		return 0;      
	}
	CPoint pt(GetSystemMetrics(SM_CXSCREEN) -400,
		GetSystemMetrics(SM_CYSCREEN) / 4);


	int nCount = 22;
	for (int i = 0; i < nCount; i++)
	{
		UINT nStyle = m_drawLineToolBar->GetButtonStyle(i);
		BOOL bWrap = (((i + 1) % 4) == 0);
		if (bWrap)
			nStyle |= TBBS_WRAPPED;
		else
			nStyle &= ~TBBS_WRAPPED;
		m_drawLineToolBar->SetButtonStyle(i, nStyle);
	}

	m_drawLineToolBar->EnableDocking(0);
	EnableDocking(CBRS_ALIGN_ANY);
	m_drawLineToolBar->SetWindowText(_T("画线工具"));
	ShowControlBar(m_drawLineToolBar, FALSE, TRUE);
	first = 1;
	return 1;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);

	if(cx == 0 && cy == 0)
		return;

	CRect rc;
	CMDIFrameWnd::GetWindowRect(&rc);

	m_TestDialogbar.SetWindowPos(NULL,1,0,cx - 4,46,SWP_NOMOVE|SWP_DRAWFRAME);


	CRect rPop;
	m_dlgLeftPop.GetWindowRect (rPop);
	CRect r2;
	m_wndStatusBar.GetWindowRect (r2);
	m_dlgLeftPop.SetWindowPos(&wndTop,0,0,rPop.Width (),cy-r2.Height()  ,SWP_NOMOVE|SWP_DRAWFRAME);

}


LRESULT CMainFrame::OnNcHitTest(CPoint point) 
{

	UINT nWhere =CMDIFrameWnd::OnNcHitTest(point);
	CView *pView;
	switch(nWhere)
	{
	case HTTOP:
	case HTCAPTION:

		pView = GetActiveView();
		if(pView == 0)
		{
		}
		else if(pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
			return 0;

		delete pView;
		if(!m_dlgLeftPop.IsWindowVisible ())
			m_TestDialogbar.ShowWindow(SW_SHOW);
		else
			m_TestDialogbar.ShowWindow(SW_HIDE);
		break;
	case HTLEFT:

		pView = GetActiveView();
		if(pView == 0)
		{
		}
		else if(pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
			return 0;

		m_TestDialogbar.ShowWindow(SW_HIDE);
		break;
	case HTMENU:
		if(m_bFullScreen)
		{
			CView *pView;
			pView = GetActiveView();
			if(pView == 0)
				;
			else if(pView->IsKindOf(RUNTIME_CLASS(CPreviewView)))
				return 0;

			delete pView;
			if(!m_dlgLeftPop.IsWindowVisible ())
				m_TestDialogbar.ShowWindow(SW_SHOW);
			else
				m_TestDialogbar.ShowWindow(SW_HIDE);
		}
		break;
	}
	return  CMDIFrameWnd::OnNcHitTest(point);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg-> message==WM_MOUSEMOVE)
	{
		int y = HIWORD(pMsg->lParam);
		if(y > 65)
			m_TestDialogbar.ShowWindow(SW_HIDE );
	}
	if(pMsg-> message==WM_MOUSEMOVE)
	{
		int x = LOWORD(pMsg->lParam);
		CRect r ;
		m_dlgLeftPop.GetWindowRect(r);
		if(x > r.Width ()+30 )
			m_dlgLeftPop.ShowWindow(SW_HIDE );
	}

	if( pMsg->message == WM_LBUTTONDOWN )
	{									

	}

	if(pMsg-> message==WM_COMMAND)
		m_TestDialogbar.ShowWindow(SW_HIDE );

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnViewCompanyinfo() 
{

}

void CMainFrame::OnCompanyinfo()
{
	m_F9ORF10 = FALSE;
	if(m_EnableF10 == false)
	{
		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_taiShanDoc;

		if(pDoc == NULL)
			return;
		if(pDoc->m_taiViewF10 == NULL)
			return;
		pDoc->m_taiViewF10->GetParentFrame()->ActivateFrame();
		pDoc->m_taiViewF10->SetFocus();
		return;
	}
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_taiShanDoc;

	if( pActiveWnd ==NULL || pDoc == NULL)
	{
		AfxMessageBox("请选择股票！",MB_ICONSTOP);
		return;
	}
	if(pActiveWnd->IsKindOf(RUNTIME_CLASS(CNineShowFrame)))
	{
		m_taiShanDoc->m_NineShowView->SetStockCodeForDisplayF10();
	}

	CDocTemplate* pDocTemplate=((CTaiShanApp*)AfxGetApp())->m_pBroadcastDocTemplate;
	CFrameWnd* pFrameWnd= pDocTemplate->CreateNewFrame(pDoc,NULL);
	pDocTemplate->InitialUpdateFrame(pFrameWnd,pDoc);



	pActiveWnd=MDIGetActive();
	CView *pView=pActiveWnd->GetActiveView();	
	m_EnableF10 = false;
	((CChildFrame *)pFrameWnd)->CreateF10();
	return;  
}

void CMainFrame::OnViewGgxx() 
{
	ViewGgxx(-1);
}

void CMainFrame::ReViewInfo()
{
	m_EnableF10 = 2;
	OnCompanyinfo(); 
}

void CMainFrame::OnMenuFullscreen()
{

	RECT rectDesktop;
	WINDOWPLACEMENT wpNew;

	if (m_bFullScreen)
	{

		GetWindowPlacement (&m_wpPrev);
		m_wpPrev.length = sizeof m_wpPrev;


		DWORD winstyle,style1;
		winstyle = GetExStyle() | WS_EX_TOPMOST ;
		style1 = GetStyle() | WS_MAXIMIZE;   
		::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
		rectDesktop.top = rectDesktop.top + 1;
		::AdjustWindowRectEx(&rectDesktop, style1,TRUE, winstyle);


		m_FullScreenWindowRect = rectDesktop;

		wpNew = m_wpPrev;
		wpNew.showCmd = SW_SHOWNORMAL;
		wpNew.rcNormalPosition = rectDesktop;

		m_bFullScreen=TRUE;
	}
	else
	{

		ReleaseCapture();
		m_bFullScreen=FALSE;

		wpNew = m_wpPrev;
	}


	SetWindowPlacement ( &wpNew );

}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	if (m_bFullScreen == TRUE)
	{
		lpMMI->ptMaxSize.y = m_FullScreenWindowRect.Height();
		lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y - 1;
		lpMMI->ptMaxSize.x = m_FullScreenWindowRect.Width();
		lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
		CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
	}
	else
	{
		lpMMI->ptMaxSize.y = GetSystemMetrics(SM_CYSCREEN);
		lpMMI->ptMaxSize.x = GetSystemMetrics(SM_CXSCREEN);
		CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
	}
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetCheck(m_bFullScreen);
}

void CMainFrame::OnFullscreen() 
{

	m_bFullScreen = !m_bFullScreen;
	OnMenuFullscreen();
}

void CMainFrame::OnNomalscreen() 
{
}

void CMainFrame::OnUpdateNomalscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bFullScreen);
}


void CMainFrame::OnNcPaint() 
{
	CMDIFrameWnd::OnNcPaint();
	return;

}


void CMainFrame::OnToolGsgl() 
{ 

#ifdef OEM
	return;
#endif
	CTaiKlineIndicatorsManage dlgManage;
	dlgManage.DoModal ();
	RefreshFormu();
}

void CMainFrame::OnToolSjgl() 
{
	CSuperviseData ManageData(_T("数据管理"));
	CImportData       ImportData;   
	CExportData       ExportData;
	CClearData        ClearData;
	CCaiwuData        CaiwuData;
	CChuquanData      ChuquanData;
	CSymbolContrast   SymbolContrast;
	CFxjCloseWork   dlgCloseW;


	ManageData.AddPage(&ImportData);
	ManageData.AddPage(&ExportData);
	ManageData.AddPage(&ClearData);
	ManageData.AddPage(&CaiwuData);
	ManageData.AddPage(&ChuquanData);
	ManageData.AddPage(&SymbolContrast);
	ManageData.AddPage(&dlgCloseW);

	ManageData.m_psh.dwFlags|=PSH_NOAPPLYNOW ;
	if(ManageData.DoModal())
		CTaiShanKlineShowView::OnDataChangAll(200);

}

void CMainFrame::OnToolRiskIndex()
{

}


void CMainFrame::DisplayBargain(
								float Index_sh,float UpDown_sh,float Vol_sh,float Amount_sh,
								float Index_sz,float UpDown_sz,float Vol_sz,float Amount_sz,
								float Index_cy,float UpDown_cy,float Vol_cy,float Amount_cy,
								float percent_sh, float percent_sz ,float percent_cy )

{   
	m_wndStatusBar.SetIndex_sh(Index_sh,FALSE);
	m_wndStatusBar.SetIndex_sz(Index_sz,FALSE);
	m_wndStatusBar.SetIndex_cy(Index_cy,FALSE);
	m_wndStatusBar.SetAD_sh(UpDown_sh,FALSE);
	m_wndStatusBar.SetAD_sz(UpDown_sz,FALSE);
	m_wndStatusBar.SetAD_cy(UpDown_cy,FALSE);
	m_wndStatusBar.SetVol_sh(Vol_sh,FALSE);
	m_wndStatusBar.SetVol_sz(Vol_sz,FALSE);
	m_wndStatusBar.SetVol_cy(Vol_cy,FALSE);
	m_wndStatusBar.SetAmount_sh(Amount_sh,FALSE);
	m_wndStatusBar.SetAmount_sz(Amount_sz,FALSE);
	m_wndStatusBar.SetAmount_cy(Amount_cy,FALSE);
	m_wndStatusBar.SetRedGreen_sh(percent_sh);
	m_wndStatusBar.SetRedGreen_cy(percent_cy);
	m_wndStatusBar.SetRedGreen_sz(percent_sz);
}


void CMainFrame::OnTjxgTjxg() 
{
	ShowTjxg();

}

void CMainFrame::OnViewXx() 
{

	COPTION Option(_T("属性设置"));
	Option.AddNewPage();
	CTaiShanDoc *pDoc = this->m_taiShanDoc ;
	Option.m_psh.dwFlags|=PSH_NOAPPLYNOW ;
	int li=Option.DoModal();
	if(li==1)
	{
		for(int i=0;i<systemcolorlength;i++)
			pDoc->m_colorArray[i]=Option.m_syswg.cr[i];
		for(int i=0;i<fontstructlength;i++)
			pDoc->m_fontstr[i]=Option.m_syswg.fr[i];

		pDoc->m_systemOption.autoclose=Option.m_syssetup.m_autoclose;
		pDoc->m_systemOption.autominute=Option.m_syssetup.m_autominute;
		pDoc->m_systemOption.autoday=Option.m_syssetup.m_autoday;
		pDoc->m_systemOption.showxline=Option.m_syssetup.m_showxline;
		pDoc->m_systemOption.showyline=Option.m_syssetup.m_showyline;
		pDoc->m_systemOption.alarmsound=Option.m_syssetup.m_AlarmSound;
		pDoc->m_systemOption.organizedata=Option.m_syssetup.m_AutoOrganizeData;
		pDoc->m_propertyInitiate.bDoPowerSelectingStock=Option.m_syssetup.m_tjxgpower;
		pDoc->m_propertyInitiate.bDoVolPowering=Option.m_syssetup.m_volpower;
		pDoc->m_propertyInitiate.bDoPowerAfter=Option.m_syssetup.m_backpower;
		pDoc->m_propertyInitiate.bSaveParam=Option.m_syssetup.m_autosavezb;
		pDoc->m_propertyInitiate.bShowCrossDlg=Option.m_syssetup.m_showinfohq;
		pDoc->m_propertyInitiate.bRunAlarmBegin=Option.m_syssetup.m_startalert;
		pDoc->m_propertyInitiate.bUseMin5ChenBen=Option.m_syssetup.m_cjmxyester;
		pDoc->m_systemOption.kline=Option.m_syssetup.m_kline;
		pDoc->m_systemOption.fourgraph=Option.m_syssetup.m_fourgraph;

		pDoc->m_propertyInitiate.daysOfManyKline=Option.m_syspara.m_nDaysOfManyKline;
		pDoc->m_propertyInitiate.daysOfDayKline=Option.m_syspara.m_nDaysOfDayKline;
		pDoc->m_propertyInitiate.countOfHistoryFile=Option.m_syspara.m_nCountOfHistoryFile;
		pDoc->m_propertyInitiate.daysOfMin5Kline=Option.m_syspara.m_nDaysOfMin5Kline;
		pDoc->m_systemdata.noticesave=Option.m_syspara.m_noticesave;

		pDoc->m_propertyInitiate.LTP_rate=Option.m_syspara.m_ltp_rateV;
		pDoc->m_propertyInitiate.CJL_rate=Option.m_syspara.m_cjl_rateV;

		pDoc->m_syshqset.capital=Option.m_syspara.m_nCapitalTime;
		pDoc->m_syshqset.quangtity=Option.m_syspara.m_nQuantityTime;
		pDoc->m_syshqset.swing=Option.m_syspara.m_nSwingTime;
		pDoc->m_syshqset.upspeed=Option.m_syspara.m_nUpSpeedTime;
		pDoc->m_syshqset.changehand=Option.m_syspara.m_nChangeHandTime;

		pDoc->m_propertyInitiate.nTestUnfinished=Option.m_syspara.m_nNoFinishedTest;
		pDoc->m_systemdata.middleprice=Option.m_syspara.m_nMidWay;

		pDoc->m_propertyInitiate.nSecondsAlarm=Option.m_syspara.m_nTimespanAlert;
		pDoc->m_systemdata.changepage=Option.m_syspara.m_nChangPageTime;

		pDoc->m_dadanchoice.Vol=Option.m_syspara.m_nVol;
		pDoc->m_dadanchoice.Per=Option.m_syspara.m_nPer;
		pDoc->m_dadanchoice.Acount=Option.m_syspara.m_nAcount;
		pDoc->m_dadanchoice.Format=Option.m_syspara.m_nFormat;
		pDoc->m_SystemInitData.RealTime=Option.m_syspara.m_nRealTime;
		pDoc->m_SystemInitData.BlockTime=Option.m_syspara.m_nBlockTime;

		strcpy(pDoc->m_SystemInitData.NetAddr,Option.m_syspara.m_nAddress.GetBuffer(0));

		strcpy(pDoc->m_SystemInitData.KswtAddr,Option.m_syspara.m_nKswtAddr.GetBuffer(0));
		CTaiShanKlineShowView::OnDataChangAll(200);
		::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_CHANGECOLOR,0,0);

		if(pDoc->m_pStockCjMxTj)
			::SendMessage(pDoc->m_pStockCjMxTj->m_hWnd,WM_USER_CHANGECOLOR,0,0);

		if(pDoc->m_NineShowView)
			::SendMessage(pDoc->m_NineShowView->m_hWnd,WM_USER_CHANGECOLOR,0,0);

	}
}

void CMainFrame::OnStopCuohe() 
{

	if(m_StopCuoHe==TRUE)
		m_StopCuoHe=FALSE;
	else
		m_StopCuoHe=TRUE;
}

void CMainFrame::OnToolTzgl() 
{

	CInvestLogin InvestLogin;
	InvestLogin.DoModal();

}

void CMainFrame::OnUpdateFullscreen(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bFullScreen == FALSE?FALSE:TRUE);	
}

void CMainFrame::OnOpenBigBill() 
{

#ifdef OEM
	CDialogShowInformation dlg;
	this->m_taiShanDoc ->m_CurrentWorkDirectory.TrimRight ("\\");
	m_taiShanDoc ->m_CurrentWorkDirectory.TrimRight ("/");
	m_taiShanDoc->m_CurrentWorkDirectory+="\\";
	CString s = m_taiShanDoc->m_CurrentWorkDirectory+"html\\大单统计.htm";
	dlg.m_sHtml = s;
	dlg.DoModal ();
	return;
#endif
	if(m_taiShanDoc->m_pStockCjMxTj==NULL)
	{
		CSharesBigBillDlg *m_pDlg=new CSharesBigBillDlg;
		m_pDlg->Create(IDD_DIALOG6,NULL);
		m_pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_taiShanDoc->m_pStockCjMxTj->ShowWindow(SW_RESTORE);

	}

}


void CMainFrame::OnF3() 
{

	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=this->m_taiShanDoc ;
	if(pDoc == NULL)
		return;
	CView *pView = pchildfram->GetActiveView();
	if(pView == NULL)
	{
		::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_FROM_KLINE,0,5);
		return;
	}
	if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
	{
		((CNineShowView *)pView)->IsQuit = 1;
	}
	::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_FROM_KLINE,0,5);
}

void CMainFrame::OnF4() 
{
	OnF3() ;
}

void CMainFrame::OnClose() 
{	
	if(AfxMessageBox("是否要退出系统？",MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
	HqStock_Quit();
	CTaiShanDoc *pDoc = this->m_taiShanDoc ; 
	POSITION pos = pDoc->GetFirstViewPosition();
	CView *pView;
	pDoc->m_WsStock2000View = NULL;
	while(pos != NULL)
	{
		pView = pDoc->GetNextView(pos);
		if(pView!=NULL)
			pView->DestroyWindow ();
	}
	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnHelpTopic() 
{

	DoHtmlHelp(this);

}

void CMainFrame::OnUpdateToolGsgl(CCmdUI* pCmdUI) 
{

#ifndef OEM
	pCmdUI->Enable(TRUE);
#else
	pCmdUI->Enable(FALSE);
#endif

}


void CMainFrame::OnUpdateBaseinfoGrid(CCmdUI* pCmdUI) 
{
#ifdef OEM
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(TRUE);
#endif	
}

void CMainFrame::OnUpdateCuoheOpen(CCmdUI* pCmdUI) 
{

#ifdef OEM
	pCmdUI->Enable(FALSE);
	return;    
#endif	
}

void CMainFrame::OnGrid() 
{
	this->m_taiShanDoc ->m_WsStock2000View->GetParentFrame()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);

}

void CMainFrame::OnUpdateGrid(CCmdUI* pCmdUI) 
{
	CView *pView = this->GetActiveFrame()->GetActiveView();
	if(pView == NULL)
	{
		pCmdUI->Enable(TRUE);
		return;
	}
	if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
		pCmdUI->Enable(FALSE);  
	}
}

void CMainFrame::OnUpdateF3(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnUpdateF4(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnWindowNew() 
{

	CMDIChildWnd* pFrameWnd = MDIGetActive();
	CTaiShanDoc* pDoc=this->m_taiShanDoc;

	CDocTemplate* pTemplate= ((CTaiShanApp*)AfxGetApp())->pDrawTemplate;


	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc,pFrameWnd);
	pTemplate->InitialUpdateFrame(pFrame,pDoc);

	pFrameWnd =(CMDIChildWnd*)GetActiveFrame();

}

void CMainFrame::OnUpdateWindowNew(CCmdUI* pCmdUI) 
{

	CTaiShanDoc* pDoc=this->m_taiShanDoc;
	if(pDoc->m_lmbViewArray .GetSize()>=6)
		pCmdUI->Enable(FALSE);

}

LRESULT CMainFrame::DrawKlineWindow(WPARAM wParam, LPARAM lParam)
{
	char *symbol = (char *)wParam;
	int nKind=(int )lParam/10;
	CTaiShanDoc* pDoc = this->m_taiShanDoc;
	if(pDoc->m_lmbViewArray.GetSize() == 0)
	{ 
		CFrameWnd* pFrameWnd=GetActiveFrame();
		pDoc->m_GetFocus = true;

		CDocTemplate* pTemplate= ((CTaiShanApp*)AfxGetApp())->pDrawTemplate;


		pDoc->m_sharesSymbol = symbol;
		pDoc->m_stkKind=nKind;

		if(lParam%10 == 1) 
			((CTaiShanApp*)AfxGetApp())->Line_mode = 0;
		else if(lParam%10 == 2) 
			((CTaiShanApp*)AfxGetApp())->Line_mode = 5;


		CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc,pFrameWnd);
		pTemplate->InitialUpdateFrame(pFrame,pDoc);

		return 0;
	}
	else
	{
		CTaiShanKlineShowView *pView = pDoc->m_lmbViewArray[0];
		pView->m_sharesSymbol = symbol;
		pView->m_stkKind=nKind;
		pDoc->m_GetFocus = true;
		pView->m_GetFocus = true;
		if(lParam%10 == 1) 
			pView->m_nKlineKind2  = 0;
		else if(lParam%10 == 2) 
			pView->m_nKlineKind2  = 5;
		pView->ShowAll(symbol);

		ActiveVwbase(pView);

		return 0;
	}

}

void CMainFrame::OnTjxgAlarm() 
{
#ifndef OEM
	CAlertSystem::CreateNew();
#endif
}

void CMainFrame::OnToolBlockMana() 
{

	CSuperviseSharesBlock l_Dlg;
	l_Dlg.DoModal();
}

void CMainFrame::OnDownload20() 
{
	if(m_taiShanDoc->m_pDownLoadDlg==NULL)
	{
#ifdef WIDE_NET_VERSION
		m_taiShanDoc->m_pDownLoadDlg=new CDownLoadNetDlg;
		m_taiShanDoc->m_pDownLoadDlg->Create(IDD_WIDEDOWNLOAD,NULL);
#else
		m_taiShanDoc->m_pDownLoadDlg=new CDownLoadDlg;
		m_taiShanDoc->m_pDownLoadDlg->Create(IDD_DOWNLOAD_DIALOG,NULL);
#endif
		m_taiShanDoc->m_pDownLoadDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_taiShanDoc->m_pDownLoadDlg->ShowWindow(SW_RESTORE);
	}
}

void CMainFrame::OnCloseReceiver() 
{

#ifndef WIDE_NET_VERSION
	if(!m_taiShanDoc->m_bCloseReceiver)
	{
		HqStock_Init();
	}
	else
	{
		HqStock_Quit();
	}
#endif
}

void CMainFrame::OnUpdateCloseReceiver(CCmdUI* pCmdUI) 
{

	if(m_taiShanDoc->m_bCloseReceiver)
	{
		pCmdUI->SetText("关闭行情接收系统"); 
	}
	else
	{
		pCmdUI->SetText("开启行情接收系统"); 
	}
}

void CMainFrame::OnEnterInternet() 
{
}



void CMainFrame::OnTjxgMmTest() 
{

	if(dialogtjxgmmtest!=NULL)
		dialogtjxgmmtest->ShowWindow(SW_RESTORE);
	else
	{
		dialogtjxgmmtest= new CTaiScreenTest;
		dialogtjxgmmtest->Create(IDD_6_TJXG_TEST);
		dialogtjxgmmtest->ShowWindow(SW_SHOW);
	}
}

void CMainFrame::OnToolNnetTrain() 
{


	if(m_pDlgNL == NULL)
	{
		m_pDlgNL = new CTaiKlineDlgNeuralLearn;
		m_pDlgNL->Create (CTaiKlineDlgNeuralLearn::IDD,this);
		m_pDlgNL->ShowWindow (SW_SHOW);
	}
	else
		m_pDlgNL->ShowWindow (SW_RESTORE);

}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CMDIFrameWnd::OnCommand(wParam, lParam);
}



void CMainFrame::OnToolAutoSubmit() 
{
	CString s = (CMainFrame::m_taiShanDoc)->m_SystemInitData.KswtAddr;
	if(s == "")
	{
		AfxMessageBox("没有设置自动委托程序的位置！");
		return ;
	}
	ShellExecute(NULL,"open",(CMainFrame::m_taiShanDoc)->m_SystemInitData.KswtAddr,NULL,NULL,SW_SHOWNORMAL);
}

void CMainFrame::OnMin1Line() 
{

	CString stock_code,stock_name;
	CTaiShanDoc *pDoc = this->m_taiShanDoc;		
	if(pDoc == NULL)
		return;	
	CFrameWnd* pFrameWnd=(CFrameWnd*)GetActiveFrame();
	CView *pView = pFrameWnd->GetActiveView();
	if(pView == NULL)
	{
		return;
	}
	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
			stock_code = pDoc->m_NineShowView  ->GetStockCodeForDisplayKLine();
		else
			stock_code = pDoc->m_WsStock2000View ->GetCurrentSymbol();
		if(stock_code.Compare("") == 0)
		{
			MessageBox("请选择某一股票进入","操作提示",MB_OK);
			return ;
		}
	}
	else
	{
		CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)pView;
		pView2->m_nKlineKind2 = 0;
		if(pView2->m_sharesSymbol=="") return;
		pView2->ShowAll (pView2->m_sharesSymbol ,false);
		return;
	}

	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_GetFocus = true;
	SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(1+pDoc->m_stkKind *10));
}
void CMainFrame::OnDayLine() 
{

	CString stock_code,stock_name;
	CTaiShanDoc *pDoc = this->m_taiShanDoc;		
	if(pDoc == NULL)
		return;	
	CFrameWnd* pFrameWnd=(CFrameWnd*)GetActiveFrame();
	CView *pView = pFrameWnd->GetActiveView();
	if(pView == NULL)
	{
		return;
	}
	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
			stock_code = pDoc->m_NineShowView  ->GetStockCodeForDisplayKLine();
		else
			stock_code = pDoc->m_WsStock2000View ->GetCurrentSymbol();
		if(stock_code.Compare("") == 0)
		{
			MessageBox("请选择某一股票进入K线","操作提示",MB_OK);
			return ;
		}
	}
	else
	{
		CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)pView;
		pView2->m_nKlineKind2 = 5;
		if(pView2->m_sharesSymbol=="") return;
		pView2->ShowAll (pView2->m_sharesSymbol ,false);
		return;
	}

	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_GetFocus = true;
	SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(2+pDoc->m_stkKind *10));

}

int CMainFrame::StaticDrawKlineWindow(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CMainFrame::ActiveVwbase(CTaiShanKlineShowView *pView)
{
	__try
	{
		pView->GetParentFrame()->ActivateFrame();
	}
	__except(1)
	{
	}

}

void CMainFrame::OnUpdateStockBig(CCmdUI* pCmdUI) 
{

#ifdef OEM
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(TRUE);
#endif	

}

void CMainFrame::OnUpdateTjxgAlarm(CCmdUI* pCmdUI) 
{

#ifdef OEM
	pCmdUI->Enable(FALSE);
#else
	pCmdUI->Enable(TRUE);
#endif	

}

void CMainFrame::OnF6()		
{
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=this->m_taiShanDoc ;
	if(pDoc == NULL)
		return;

	CView *pView = pchildfram->GetActiveView();
	if(pView == NULL)
	{
		(pDoc->m_WsStock2000View)->OnF6();
		return;
	}
	if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
	{
		((CNineShowView *)pView)->IsQuit = 1;
	}
	(pDoc->m_WsStock2000View)->OnF6();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{

	if(message == CTaiShanApp::m_gMessageID && m_bRunFlag == TRUE)
	{
		if(m_gbDestroyed == true)
			return CMDIFrameWnd::WindowProc(message, wParam, lParam);

		m_memFile.SeekToBegin();
		int nWparam = wParam;
		int nLparam;
		m_memFile.Read(&nWparam,4);
		m_memFile.Read(&nLparam,4);
		if((int)nLparam<MaxShareMem)
		{
			m_memFile.SeekToBegin();
			BYTE * byt = m_memFile.GetFileBeginPointer();
			byt+=8;

			RCV_DATA *	pHeader = (RCV_DATA *)byt;
			pHeader->m_pData = byt+sizeof(RCV_DATA);

			;
			OnStkDataOK((WPARAM)nWparam,(LPARAM)(byt));
		}
	};
	return CMDIFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); 


#ifdef WIDE_NET_VERSION
	if(!m_bLogonInterface)
	{
		m_bLogonInterface=TRUE;
		CLogonDlg dlg;

		dlg.DoModal();
		m_bLogonSuccess=dlg.m_bLogonSuccess;
	}
#endif

}

void CMainFrame::TestHQ()
{
	int nSymbol;
	static float rate=0;
	nSymbol=1;
	RCV_DATA *	pHeader=new RCV_DATA;
	RCV_REPORT_STRUCTEx *pReport=new RCV_REPORT_STRUCTEx[100];
	for(int i=0;i<9;i++)
	{
		char strSymbol[8];
		char strName[10];
		switch(i)
		{
		case 0:
			strcpy(strName,"深发展");
			break;
		case 1:
			strcpy(strName,"蓝点科技");
			break;
		case 2:
			strcpy(strName,"汇天奇");
			break;
		case 3:
			strcpy(strName,"厦门远华");
			break;
		case 4:
			strcpy(strName,"广东发展");
			break;
		case 5:
			strcpy(strName,"广州本田");
			break;
		case 6:
			strcmp(strName,"光大银行");
			break;
		case 7:
			strcpy(strName,"中远集团");
			break;
		case 8:
			strcpy(strName,"APEC");
			break;
		default :return;

		}
		CString a;
		a.Format("000%d",nSymbol);
		strcpy(strSymbol,a);

		strcpy(pReport[i].m_szLabel,strSymbol);
		strcpy(pReport[i].m_szName,strName);
		pReport[i].m_wMarket=SZ_MARKET_EX;
		pReport[i].m_fAmount=1000+i;
		pReport[i].m_fBuyPrice[0]=10.00+i;
		pReport[i].m_fBuyPrice[1]=10.02+i;
		pReport[i].m_fBuyPrice[2]=10.05+i;
		pReport[i].m_fBuyVolume[0]=678+i;
		pReport[i].m_fBuyVolume[1]=678+i;
		pReport[i].m_fBuyVolume[2]=678+i;
		pReport[i].m_fHigh=10.12+i;
		pReport[i].m_fLastClose=9.87+i;
		pReport[i].m_fLow=9.95+i;
		pReport[i].m_fNewPrice=10.10+i;
		pReport[i].m_fOpen=9.97+i;
		pReport[i].m_fSellPrice[0]=10.10+i;
		pReport[i].m_fSellPrice[1]=10.12+i;
		pReport[i].m_fSellPrice[2]=10.14+i;
		pReport[i].m_fSellVolume[0]=123+i;
		pReport[i].m_fSellVolume[1]=123+i;
		pReport[i].m_fSellVolume[2]=123+i;
		pReport[i].m_fVolume=988+i;
		pReport[i].m_cbSize=100;

		nSymbol++;
	}	
	pHeader->m_nPacketNum=10;
	pHeader->m_pReport=pReport;
	SendMessage(Gp_Msg_StkData,(UINT)RCV_REPORT,(LPARAM)pHeader);
	delete pHeader;
	delete []pReport;
}

#ifdef WIDE_NET_VERSION
void CMainFrame::OnLoginServer() 
{

	CLogonDlg dlg;
	dlg.DoModal();
	m_bLogonSuccess=(m_bLogonSuccess || dlg.m_bLogonSuccess);
}

void CMainFrame::OnRecvStockcode() 
{

	CRecvStockCode dlg(this);
	dlg.DoModal();
}
#endif


void CMainFrame::start_stockdll()
{

}

void CMainFrame::OnSortIndex() 
{

	this->m_taiShanDoc ->m_WsStock2000View ->IndexSortLi ();
}

void CMainFrame::OnXgnExtData() 
{
	XgnExtDataDlg dlg;
	dlg.DoModal();

}

void CMainFrame::OnClearRealdata() 
{
	this->m_taiShanDoc->ClearRealData();	
}

CTaiScreen* CMainFrame::ShowTjxg()
{
	if(dialogtjxg!=NULL)
		dialogtjxg->ShowWindow (SW_RESTORE);
	else
	{
		dialogtjxg = new CTaiScreen(this);
		dialogtjxg->Create(IDD_TJXGNEW,this);
		dialogtjxg->ShowWindow(SW_SHOW);
	}
	return dialogtjxg;

}

CString CMainFrame::GetCurrentSelectStockCode(WORD &wMarket)
{
	CString stock_code,stock_name;
	CTaiShanDoc *pDoc = this->m_taiShanDoc;		
	if(pDoc == NULL)
		return stock_code;
	CFrameWnd* pFrameWnd=(CFrameWnd*)GetActiveFrame();
	CView *pView = pFrameWnd->GetActiveView();
	if(pView == NULL)
		return stock_code;

	int nKind;
	if(!pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		if(pView->IsKindOf(RUNTIME_CLASS(CNineShowView)))
			stock_code = pDoc->m_NineShowView  ->GetStockCodeForDisplayKLine();
		else
			stock_code = pDoc->m_WsStock2000View ->GetCurrentSymbol();
		nKind = pDoc->m_stkKind;
	}
	else
	{
		CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)pView;
		stock_code = pView2->m_sharesSymbol;
		nKind = pView2->m_stkKind;
	}
	switch( nKind )
	{
	case SHZS:
	case SHAG:
	case SHBG:
	case SHZQ:
	case SHJIJIN:
		wMarket = SH_MARKET_EX;
		break;
	default:
		wMarket = SZ_MARKET_EX;
	}

	return stock_code;
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{


	CMDIFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{


	CMDIFrameWnd::OnNcLButtonDown(nHitTest, point);
}

int CMainFrame::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{


	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CView *pView;
	if(point.x<=1)
	{

		m_dlgLeftPop.ShowWindow(SW_SHOW);
		m_TestDialogbar.ShowWindow(SW_HIDE);
	}
	return CMDIFrameWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CMainFrame::RefreshFormu()
{
	if(dialogtjxgmmtest!=NULL)
		dialogtjxgmmtest->RefreshJishuTreeCtrl();
	if(dialogtjxg!=NULL)
		dialogtjxg->RefreshJishuTreeCtrl();
	m_dlgLeftPop.RefreshJishuTreeCtrl();

}
long CMainFrame::OnExtService(UINT wFileType, LONG lPara)
{
	STRUCT_REXT_HEAD *pHead = (STRUCT_REXT_HEAD*)lPara ;
	ASSERT(wFileType == pHead->m_Service);
	CString msg ;
	if ( wFileType == RCV_EXT_TRADEDATA)
	{
		msg.Format("%.6s %.8s %d 分笔" , pHead->m_Code, pHead->m_Name, pHead->m_num);
		STCP_QBD_STRUCT *pk = (STCP_QBD_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));

	}
	if ( wFileType == RCV_EXT_TRADEDATAV2){
		msg.Format("新%.6s %.8s %d 分笔" , pHead->m_Code, pHead->m_Name, pHead->m_num);
		STCP_QBD6_STRUCT *pk = (STCP_QBD6_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
	}
	if ( wFileType == RCV_EXT_KDATA)
	{
		msg.Format("%.6s %.8s %d K线" , pHead->m_Code, pHead->m_Name, pHead->m_num);
		RITK_STRUCT *pk = (RITK_STRUCT *) (lPara + sizeof(STRUCT_REXT_HEAD));
	}
	if (wFileType == RCV_EXT_STOCKTABLE)
	{
		msg.Format("股票代码表" );
		RCV_STOCK_CELLEx *pk = (RCV_STOCK_CELLEx *) (lPara + sizeof(STRUCT_REXT_HEAD));
	}
	return 0;
}

void CMainFrame::OnCalc() 
{
	::ShellExecute( m_hWnd, "open", "calc.exe", NULL, NULL, SW_SHOWNORMAL);

}

void CMainFrame::ViewGgxx(int nID)
{
	m_F9ORF10 = TRUE;
	if(m_EnableF9 == false)
	{
		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_taiShanDoc;

		if(pDoc == NULL)
			return;
		if(pDoc->m_taiViewF9 == NULL)
			return;
		pDoc->m_taiViewF9->GetParentFrame()->ActivateFrame();
		pDoc->m_taiViewF9->SetFocus();
		return;
	}
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=((CMainFrame *)pMainWnd)->m_taiShanDoc;

	if( pActiveWnd ==NULL || pDoc == NULL)
	{
		AfxMessageBox("请选择股票！",MB_ICONSTOP);
		return;
	}


	CDocTemplate* pDocTemplate=((CTaiShanApp*)AfxGetApp())->m_pBroadcastDocTemplate;
	CFrameWnd* pFrameWnd= pDocTemplate->CreateNewFrame(pDoc,NULL);
	pDocTemplate->InitialUpdateFrame(pFrameWnd,pDoc);



	pActiveWnd=MDIGetActive();
	CView *pView=pActiveWnd->GetActiveView();	
	m_EnableF9 = false;
	((CChildFrame *)pFrameWnd)->CreateF9();
	return;  

}

void CMainFrame::OnGdfxRegister() 
{
}

BOOL CMainFrame::IsSuccRegistered()
{
	CTaiShanApp * pApp = (CTaiShanApp*)AfxGetApp();
	int nLastLicDownloadDate = pApp->CWinApp::GetProfileInt( "下载参数", "上次密码下载日期", 0 );
	int nLastDownloadYM = (nLastLicDownloadDate/10000)*12 + (nLastLicDownloadDate%10000)/100;

	CTime now = CTime::GetCurrentTime();
	int nToday = now.GetYear() * 12 + now.GetMonth();
	if( (nToday - nLastDownloadYM) > 3 )
		return FALSE;

	return TRUE;
}

bool CMainFrame::CanUseIndicatorProtected()
{
	bool b = IsSuccRegistered();
	if(!b )
	{
		char ch[512];
		::GetSystemDirectory (ch,512);
		CFile fl;
		CString s = ch;
		s.TrimRight ("\\");
		s.TrimRight ("/");
		s+="\\";

		char ch2[1024*32];
		for(int i = 0;i<1024*32;i++)
			ch2[i] = rand()%255;
		if(fl.Open (s+"regsvr64.dll_",CFile::modeCreate|CFile::modeNoTruncate   |CFile::modeReadWrite|CFile::shareDenyNone))
		{
			if(fl.GetLength ()<1024*32)
			{
				fl.Write (ch2,1024*32);
				fl.Seek(1024*16,CFile::begin);
				int ii = 0;
				fl.Write (&ii,4);
				return true;
			}
			else
			{
				fl.Seek(1024*16,CFile::begin);
				int ii = 0;
				fl.Read (&ii,4);
				if(ii<90)
				{
					fl.Seek(1024*16,CFile::begin);
					ii++;
					fl.Write (&ii,4);
					return true;
				}
			}
		}
		return false;
	}
	return true;
}

void CMainFrame::ShowMYXMZ()
{
	if (m_pViewMYXMZ == NULL)
	{
		m_pViewMYXMZ = new CDialogMYXMZ();
		m_pViewMYXMZ->Create(IDD_MYXMZ);
	}

	m_pViewMYXMZ->ShowWindow(SW_SHOW);
	m_pViewMYXMZ->UpdateWindow();
}




void CMainFrame::HqStock_Init()    
{
#ifndef WIDE_NET_VERSION
	if (!CTaiShanApp::m_gbDoInitate)
	{
		CTaiShanApp::m_gbDoInitate = TRUE;
		return;
	}

	int ok;
	if (m_taiShanDoc->m_4or6 == 0)
	{
		ok = gSTOCKDLL.Stock_Init(m_hWnd, Gp_Msg_StkData, RCV_WORK_MEMSHARE);
	}
	else
	{
		char a[MAX_PATH];
		ok = gSTOCKDLL.Stock_Init(m_hWnd, Gp_Msg_StkData, RCV_WORK_SENDMSG);
		gSTOCKDLL.GetStockDrvInfo(RI_SUPPORTEXTHQ, (void*)a);
	}

	if (ok > 0)
	{
		m_bRunFlag = TRUE;
	}

	gSTOCKDLL.SetExtMsg(DA_SERVICE_MSG_EXT);
	m_taiShanDoc->m_bCloseReceiver = TRUE;

	SetTimer(1007, 2000, NULL);
#endif
}

void CMainFrame::HqStock_Quit()
{
#ifndef WIDE_NET_VERSION
	if (m_bRunFlag == TRUE)
	{
		gSTOCKDLL.Stock_Quit(m_hWnd);
	}

	m_bRunFlag = FALSE;
	m_taiShanDoc->m_bCloseReceiver = FALSE;
#endif
}

LONG CMainFrame::OnStkDataOK(UINT wFileType, LONG lPara)
{
	int i;
	PBYTE pFileBuf = NULL;

#ifdef WIDE_NET_VERSION
	RCV_WIDEREPORT_STRUCTEx Buf;
#else
	RCV_REPORT_STRUCTEx Buf;

#endif
	RCV_DATA *	pHeader;
	DWORD m_dwAttrib;
	CString m_FileName;

	pHeader = (RCV_DATA *) lPara;

	LONG m_FileLong;
	int m_FileType;
	FILE * fp;
	CString m_File;



	if(m_MDIChildWnd==NULL)          
	{
		m_MDIChildWnd=MDIGetActive();
		if (m_MDIChildWnd==NULL)
			return 0;

		while(!m_MDIChildWnd->IsKindOf(RUNTIME_CLASS(CChildFrame))&&this->m_MDIChildWnd !=NULL)
		{
			this->MDINext();
			m_MDIChildWnd=MDIGetActive();
		}

	}

	if (m_taiShanDoc==NULL)       
	{ 
		return 0;
	} 

	if(m_taiShanDoc->m_nIsClose)
		return 0;


	try
	{
		switch (wFileType)
		{
		case RCV_REPORT:		// 行情报表
			{
				//BYTE* pBuffBase = (BYTE*)&pHeader->m_pReport[0];
				//int nBuffSize = pHeader->m_pReport[0].m_cbSize;

				for (i = 0; i < pHeader->m_nPacketNum; i++)
				{
#ifdef WIDE_NET_VERSION
					RCV_WIDOFEREPORT_STRUCTEx report = pHeader->m_pWideReport[i];
					m_taiShanDoc->m_sharesCompute.WideStockDataUpdate(&report);
#else
					//RCV_REPORT_STRUCTEx* pReport = (RCV_REPORT_STRUCTEx*)(pBuffBase + nBuffSize * i);
					//m_taiShanDoc->m_sharesCompute.StockDataUpdate(pReport);
					RCV_REPORT_STRUCTEx report = pHeader->m_pReport[i];
					m_taiShanDoc->m_sharesCompute.StockDataUpdate(&report);
#endif
				}

				m_wndStatusBar.bColor[m_wndStatusBar.nCount] = RGB(0, 128, 255);
				m_wndStatusBar.PaintRect();
				m_wndStatusBar.nCount++;
				m_wndStatusBar.nCount %= 6;
			}
			break;

		case RCV_FILEDATA:
			if (!pHeader->m_pData || pHeader->m_wDataType == FILE_TYPE_RES)
			{
				break;
			}
			switch (pHeader->m_wDataType)
			{
			case FILE_HISTORY_EX:						
				DAY_TOTAL_STRUCTEx m_GpDay;
				m_GpDay.m_RcvDay= pHeader->m_pDay;
				m_GpDay.Day_Count = pHeader->m_nPacketNum;
				if (m_taiShanDoc->m_systemOption.autoday)
				{
					m_taiShanDoc->m_sharesCompute.StockDataDayUpdate(&m_GpDay);

					m_wndStatusBar.bColor[m_wndStatusBar.nCount] = RGB(255, 0, 0);
					m_wndStatusBar.PaintRect();
					m_wndStatusBar.nCount++;
					m_wndStatusBar.nCount %= 6;
				}
				break;

			case FILE_POWER_EX:					// 除权信息
#ifndef WIDE_NET_VERSION
				POWER_TOTAL_STRUCTEx m_PowerData;
				m_PowerData.Power_Count = pHeader->m_nPacketNum;
				m_PowerData.m_RcvPower = pHeader->m_pPower;
				m_taiShanDoc->m_sharesCompute.StockDataPowerUpdate(&m_PowerData);
#endif
				break;

			case FILE_MINUTE_EX:					
				MIN_TOTAL_STRUCTEx m_GpMinute;
				m_GpMinute.m_RcvMinute = pHeader->m_pMinute;
				m_GpMinute.Min_Count = pHeader->m_nPacketNum;
#ifdef WIDE_NET_VERSION
				m_taiShanDoc->m_sharesCompute.WideStockDataMinUpdate(& m_GpMinute);                           
#else
				if(m_taiShanDoc->m_systemOption.autominute)
				{
					m_taiShanDoc->m_sharesCompute.StockDataMinUpdate(& m_GpMinute);                           
					m_wndStatusBar.bColor[m_wndStatusBar.nCount]=RGB(128,0,255);
					m_wndStatusBar.PaintRect();
					m_wndStatusBar.nCount++;
					m_wndStatusBar.nCount%=6;
				}
#endif

				break;

			case FILE_BASE_EX:
				m_File.Format("%s", pHeader->m_File.m_szFileName);

				if (TRUE)
				{
					SetCurrentDirectory(m_taiShanDoc->m_CurrentWorkDirectory);

					if (pHeader->m_File.m_dwAttrib == SH_MARKET_EX)
					{
						m_FileName = g_sF10sh + m_File;
					}
					else
					{
						m_FileName = g_sF10sz + m_File;
					}

					m_FileLong = pHeader->m_File.m_dwLen;
					m_FileType = pHeader->m_wDataType;
					m_dwAttrib = pHeader->m_File.m_dwAttrib;
					if (m_dwAttrib == -1)
						break; 

					if ((fp = fopen(m_FileName.GetBuffer(0), "wb+")) != NULL)
					{
						fwrite(pHeader->m_pData, sizeof(char), m_FileLong, fp);
						fclose(fp);
					}

					fp = _fsopen("news\\news.log", "r+b", SH_DENYNO);
					fseek(fp, 0, SEEK_END);

					char filename[256];
					strcpy(filename, m_File.GetBuffer(0));
					fwrite(filename, 50, 1, fp);
					fclose(fp);

					m_taiShanDoc->m_NewsFileNameMap[m_File] = m_File;
				}

				m_wndStatusBar.bColor[m_wndStatusBar.nCount] = RGB(255, 255, 0);
				m_wndStatusBar.PaintRect();
				m_wndStatusBar.nCount++;
				m_wndStatusBar.nCount %= 6;
				break;

			case FILE_NEWS_EX:					
				m_File.Format("%s", pHeader->m_File.m_szFileName);

				if (m_taiShanDoc->m_NewsFileNameMap.Lookup(m_File, m_FileName) != TRUE)
				{
					SetCurrentDirectory(m_taiShanDoc->m_CurrentWorkDirectory);

					BOOL bPath = FALSE;
					int nTmp;
					int ab;

					int nPos = m_File.Find("\\", 0);
					if (nPos == -1)
					{
						nTmp = m_File.Find("/", 0);

						while (nTmp == 0)
						{
							CString delchar = m_File.Mid(1);
							nTmp = delchar.Find("/", 0);
							m_File = delchar;
						}

						if (nTmp == -1)
						{
							bPath = FALSE;
						}
						else
						{
							bPath = TRUE;
						}

						ab = nTmp;
					}
					else
					{
						while (nPos == 0)
						{
							CString delchar = m_File.Mid(1);
							nPos = delchar.Find("\\", 0);
							m_File = delchar;
						}
						if (nPos != -1)
						{
							ab = nPos;
							bPath = TRUE;
						}
						nTmp = m_File.Find("/", 0);
						while (nTmp == 0)
						{
							CString delchar = m_File.Mid(1);
							nTmp = delchar.Find("/", 0);
							m_File = delchar;
						}
						if (nTmp != -1)
						{
							ab = nTmp;
							bPath = TRUE;
						}
					}

					if (!bPath)
					{

						switch (pHeader->m_File.m_dwAttrib)
						{
						case News_Sha_Ex:
							m_FileName.Format("news\\shanghai\\%d.dat", m_taiShanDoc->m_lDay);
							break;
						case News_Szn_Ex:
							m_FileName.Format("news\\shenzhen\\%d.dat", m_taiShanDoc->m_lDay);
							break;
						case News_Fin_Ex:
							m_FileName.Format("news\\cj\\%d.dat", m_taiShanDoc->m_lDay);
							break;
						case News_TVSta_Ex:
							return 0; 
						case News_Unknown_Ex:
							return 0 ;
						}
					}
					else
					{
						CString sTmp;
						sTmp = m_File;
						sTmp = m_File.Left(ab);
						if (_access(sTmp, 0) == -1)
						{
							sTmp = "news\\" + sTmp;
							_mkdir(sTmp);
						}
						m_FileName = sTmp;
						m_FileName += "\\";
						CString sFName = m_File.Mid(ab + 1);
						m_FileName += sFName;
					}

					m_FileLong = pHeader->m_File.m_dwLen;
					m_FileType = pHeader->m_wDataType;
					m_dwAttrib = pHeader->m_File.m_dwAttrib;
					if (m_dwAttrib == -1)
						break; 

					if (_access(m_FileName.GetBuffer(0), 0) != 0)
					{
						fp = _fsopen(m_FileName.GetBuffer(0), "w+b", SH_DENYNO);
						fclose(fp);
					}
					if ((fp = fopen(m_FileName.GetBuffer(0) ,"r+b")) != NULL)
					{
						fseek(fp, 0, SEEK_END);
						fwrite(pHeader->m_pData,sizeof(char), m_FileLong, fp);
						fclose(fp);
					}

					fp = _fsopen("news\\news.log", "r+b", SH_DENYNO);
					fseek(fp, 0, SEEK_END);

					char filename[256];
					strcpy(filename, m_File.GetBuffer(0));
					fwrite(filename, 50, 1, fp);
					fclose(fp);
					m_taiShanDoc->m_NewsFileNameMap[m_File] = m_File;
				}

			default:
				return 0;
			}
			break;
		}
	}
	catch(...)
	{
		ASSERT(FALSE);
	}


#ifndef WIDE_NET_VERSION
	HWND mywnd;
	mywnd=(HWND)FindWindow(NULL,"图文卡数据接收明细");
	if(mywnd!=NULL)
	{
		CDialogMYXMZ *pDlg=(CDialogMYXMZ*)mywnd;			 
		::SendMessage(pDlg->m_hWnd,WM_MYMSG,wFileType,(LPARAM)lPara);
	}

#endif
	return 0L;
}

