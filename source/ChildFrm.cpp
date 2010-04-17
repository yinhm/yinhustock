// ChildFrm.cpp : implementation of the CChildFrame class
// Tel:13366898744

#include "stdafx.h"
#include "resource.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "CTaiShanApp.h"
#include "MySplitter.h"
#include "CompanyInfoView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "CTaiShanKlineShowView.h"
#include "MyTreeView.h"
#include "MyRichView.h"
#include "CTaiKlineDialogCross.h"
#include "CTaiKlineDialogKLineMode.h"
#include "CTaiKlineDlgChangeIndexParam.h"

/////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
class CTaiShanReportView;//对话框基类
/////////////////////////////////////////////////////////////////////////////
// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_USER_CHANGEPOS,OnSetSplitterPos)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


CChildFrame::CChildFrame()
{

	m_wndSplitter=NULL;
	m_pContext=NULL;
	m_CurrentDoc = NULL;
}

CChildFrame::~CChildFrame()
{//从该类继承

	
	if(m_wndSplitter)  delete m_wndSplitter;

	if(m_pContext)  delete m_pContext;
    


}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{//从该类继承

	cs.lpszName = "123";
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}



#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}
void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif 

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

		m_pContext=new CCreateContext;
		memcpy(m_pContext,pContext,sizeof(CCreateContext));
		return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::AddCuoHeView()
{//从该类继承
   }
void CChildFrame::DeleteCuoHeView()
{ //为该类的对象
}

void CChildFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	static int IsTheFirstTime = 0;
    if(IsTheFirstTime == 0)
	{
		CMenu* mnu;
	    mnu=GetSystemMenu(false);
	    mnu->EnableMenuItem( SC_CLOSE,MF_GRAYED|MF_DISABLED);
	    SendMessage(WM_INITMENU,(UINT)mnu->m_hMenu ,0);
		this->MDIMaximize( );
	    IsTheFirstTime++;
	} 
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{//为该类的对象
	
	CMDIChildWnd::OnSize(nType, cx, cy);
	if(cx==0||cy==0)
		return ;
	if(m_wndSplitter==NULL)
		return ;
	int m_width=285+18;
	if(m_width > cx)
       m_width=2 * cx /3;
	m_wndSplitter->SetColumnInfo( 0, cx - m_width , 0  );
	m_wndSplitter->RecalcLayout( );

	
}


void CChildFrame::CreateF9() 
{
  
	m_SplitterWnd = new CTaiTestSplitter;
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	m_CurrentDoc = (CTaiShanDoc*)pActiveWnd->GetActiveDocument();

	CView* pOldActiveView = GetActiveView();
   

	if (!m_SplitterWnd->CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return ;
	}
	CRect rc;
	GetClientRect(&rc);//静态变量

	if (!m_SplitterWnd->CreateView(0, 0,
		RUNTIME_CLASS(CTaiTestTreeView), CSize(rc.Width()/4, rc.Height()), m_pContext))
	{
		TRACE0("Failed to create first pane\n");
		return ;
	}

	

	if (!m_SplitterWnd->CreateView(0, 1,
		RUNTIME_CLASS(CTaiTestRichView), CSize(0, 0), m_pContext))
	{
		TRACE0("Failed to create second pane\n");
		return ;
	}
	
	CTaiTestRichView *m_pRichView;
	m_pRichView = (CTaiTestRichView *)m_SplitterWnd->GetPane(0,1);
	m_pRichView->m_bF9ORF10 = TRUE;
	CHARFORMAT m_charformat;
	m_charformat.cbSize = 60;
    m_charformat.dwMask = 3892314127;
	m_charformat.dwEffects = 0;
	m_charformat.yHeight = 240;
	m_charformat.yOffset = 0;
	m_charformat.bCharSet = 134;
    m_charformat.bPitchAndFamily = 2;
	m_charformat.crTextColor = RGB(0 ,0,0);
	lstrcpy(m_charformat.szFaceName,"宋体");
	m_pRichView->GetRichEditCtrl().SetDefaultCharFormat(m_charformat);

	CTaiTestTreeView *m_pTreeView;
	m_pTreeView = (CTaiTestTreeView *)m_SplitterWnd->GetPane(0,0);
	m_pTreeView->m_bF9ORF10 = TRUE; 
	m_pTreeView->ShowAll();
    

	m_CurrentDoc->m_taiViewF9 = m_pTreeView;

	OnSize(SIZE_RESTORED,0,0);
    pOldActiveView->DestroyWindow();
	OnUpdateFrameTitle(TRUE);
}   

void CChildFrame::CreateF10() 
{//应用程序类
  
	m_SplitterWnd = new CTaiTestSplitter;
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	m_CurrentDoc = (CTaiShanDoc*)pActiveWnd->GetActiveDocument();

	CView* pOldActiveView = GetActiveView();
   
	
	if (!m_SplitterWnd->CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return ;
	}
	CRect rc;
	GetClientRect(&rc);

	if (!m_SplitterWnd->CreateView(0, 0,
		RUNTIME_CLASS(CTaiTestTreeView), CSize(rc.Width()/4 , rc.Height()), m_pContext))
	{
		TRACE0("Failed to create first pane\n");
		return ;
	}

	if (!m_SplitterWnd->CreateView(0, 1,
		RUNTIME_CLASS(CTaiTestRichView), CSize(0, 0), m_pContext))
	{
		TRACE0("Failed to create second pane\n");
		return ;
	}
	CTaiTestRichView *m_pRichView;//子窗口框架
	m_pRichView = (CTaiTestRichView *)m_SplitterWnd->GetPane(0,1);
    m_pRichView->m_bF9ORF10 = FALSE;
	CHARFORMAT m_charformat;
	m_charformat.cbSize = 60;
    m_charformat.dwMask = 3892314127;
	m_charformat.dwEffects = 0;
	m_charformat.yHeight = 240;
	m_charformat.yOffset = 0;
	m_charformat.bCharSet = 134;
    m_charformat.bPitchAndFamily = 2;
	m_charformat.crTextColor = RGB(0 ,0,0);
	lstrcpy(m_charformat.szFaceName,"宋体");
	m_pRichView->GetRichEditCtrl().SetDefaultCharFormat(m_charformat);

	CTaiTestTreeView *m_pTreeView;
	m_pTreeView = (CTaiTestTreeView *)m_SplitterWnd->GetPane(0,0);
	m_pTreeView->m_bF9ORF10 = FALSE;
	m_pTreeView->ShowAllF10();
    
	m_CurrentDoc->m_taiViewF10 = m_pTreeView;
	OnSize(SIZE_RESTORED,0,0);
    pOldActiveView->DestroyWindow();
	SetWindowText("上市公司基本资料");
    GetMDIFrame()->OnUpdateFrameTitle(TRUE);
}

void CChildFrame::OnClose() 
{
	CView *pView = GetActiveView();
	if(pView == NULL)
	{
	  CMDIChildWnd::OnClose(); 
	}
	else if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
	  return;
	}
	else if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
	    CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	    CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc *pDoc = (CTaiShanDoc*)pActiveWnd->GetActiveDocument(); 
		POSITION pos = pDoc->GetFirstViewPosition();
		CView *pView;
	    while(pos != NULL)
		{
			pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
				{
					((CTaiShanReportView *)pView)->m_nFirst = 1;
				  
				    break;
				}
			}
	    CMDIChildWnd::OnClose();     
	}
	else
	{
	 CMDIChildWnd::OnClose();
	}
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{//基本资料数据结构类实现
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
 
    CView *pView = GetActiveView();
	if(pView == NULL)
		return;
	
	if(bActivate == TRUE && pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{

	  ((CTaiShanReportView *)pView)->ActiveGrid();
	  return;
	}
	  
}
LRESULT CChildFrame::OnSetSplitterPos(WPARAM wParam, LPARAM lParam)
{
	long Width=(long)lParam;
	CRect rc;
	GetClientRect(&rc);
	if(rc.right < Width)
       Width=rc.right;  
	m_wndSplitter->SetColumnInfo( 0, rc.right - Width , 0  );
	m_wndSplitter->RecalcLayout( );
	((CTaiShanReportView*)m_wndSplitter->GetPane(0,0))->SetFocus();

	return 0;
}

void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{//基本资料

  CTaiShanDoc* pDoc = ((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;
  CView* pView = GetActiveView();
  CWnd *pWnd = GetWindow(GW_CHILD);
  
  if(bAddToTitle && pDoc != NULL)
  {
	  CString strCurCaption,strWindowText,strNewCaption;
	  if(pView == NULL)
		 return;
	
	  if(pView->IsKindOf(RUNTIME_CLASS(CTaiTestRichView)))
	  {//基本资料数据结构类实现
	    if(((CTaiTestRichView *)pView)->m_bF9ORF10 == TRUE) 
		   SetWindowText("公告信息");
		if(((CTaiTestRichView *)pView)->m_bF9ORF10 == FALSE) 
           SetWindowText("上市公司基本资料");
		GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
		return;
	  }
	  if(pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	  {
        GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
		return;
	  }
	  if(!pView->IsKindOf(RUNTIME_CLASS(CInfoView)))
	  {
	   GetWindowText(strCurCaption);
	   GetActiveView()->GetWindowText(strWindowText);
	   const CString&strDocTitle = pDoc->GetTitle();
	   strNewCaption = strDocTitle;
	   if(m_nWindow > 0)
	   {
	    strNewCaption += ":";
		strNewCaption += strWindowText;
	   }
	   if(strNewCaption != strCurCaption)
		SetWindowText(strNewCaption);
	  }
  }
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{//基本资料数据结构类实现
    CView *pView = GetActiveView();
	if(pView == NULL)
	{
		CMDIChildWnd::OnSetFocus(pOldWnd);	
		return;
	}
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
	  ((CTaiShanReportView *)GetActiveView())->ActiveGrid();
	  return;
	}
	CMDIChildWnd::OnSetFocus(pOldWnd);	
}
