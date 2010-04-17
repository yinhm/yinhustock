// HistoryDlg.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "HistoryDlg.h"
#include "CTaiShanDoc.h"
#include "CTaiShanReportView.h"
#include "MainFrm.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineTransferKline.h"
#include "HistorySelDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoryDlg dialog
extern	UINT ID_ARRAY[];



CHistoryDlg::CHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryDlg::IDD, pParent)
{
	pView = (CTaiShanReportView*)pParent;

	pView->m_pHistoryDlg = this;

	//{{AFX_DATA_INIT(CHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistoryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CHistoryDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTORY_HIDE, OnHistoryHide)
	ON_COMMAND(ID_HISTORY_GOHEAD, OnHistoryGohead)
	ON_COMMAND(ID_HISTORY_GOBACK, OnHistoryGoback)
	ON_COMMAND(ID_HISTORY_CLOSE, OnHistoryClose)
	ON_COMMAND(ID_HISTORY_A_STEP, OnHistoryAStep)
	ON_COMMAND(ID_HISTORY_SWITCH, OnHistorySwitch)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CHistoryDlg::OnOk() 
{

	
}

void CHistoryDlg::OnCancel() 
{
	CDialog::OnCancel();
}

int CHistoryDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if ( !m_wndToolBar.CreateEx(this) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;   
	}
    
	CImageList  imgList;          
	CString     str;
	const int nButton = 5;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(40, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);


	imgList.Create(IDB_51, 22, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&imgList);
	imgList.Detach();
	imgList.Create(IDB_52, 22, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&imgList);
	imgList.Detach();

	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons(NULL, nButton);


	CString StringArray[]={"同步","切换","向前","向后","关闭"};
	for( int k=0;k<nButton;k++)
	{
		m_wndToolBar.SetButtonInfo(k, ID_ARRAY[k], TBSTYLE_BUTTON, k);
		str = (StringArray[k]);
		m_wndToolBar.SetButtonText(k, str);
		m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[k],TBSTATE_ENABLED );

	}


	CRect rectToolBar(0,0,40,45);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(27,21));
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY| CBRS_SIZE_DYNAMIC);
	

	if(!this->m_wndToolTip.Create(this,TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE))
	{
	  return -1;
	}
    m_wndToolTip.AddTool(&m_wndToolBar,LPSTR_TEXTCALLBACK);


	if(pView !=NULL)
	{
		CRect r;
		pView->GetWindowRect (r );
		CRect rcWindow;
		GetWindowRect(rcWindow);
		CRect rc;
		GetClientRect(rc);
		r.bottom-=30;
		r.top = r.bottom -43-(rcWindow.Height ()-rc.Height ());

		r.right = r.left + 40*5+6;
		MoveWindow(r, FALSE);
	}



	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	if(pView !=NULL) pView->SetFocus ();

    m_wndToolTip.Activate(TRUE);	

	
	pView->SetHistoryColor(true);


	return 0;
}
BOOL CHistoryDlg::OnToolTipNotify(UINT id,NMHDR *pNMHDR,LRESULT *pResult)
{
  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
  UINT nID = pNMHDR->idFrom;

  if(pTTT->uFlags & TTF_IDISHWND)
  {
	  nID = ::GetDlgCtrlID((HWND)nID);
	  if(nID == 0)
		  return FALSE;
  }
  switch(nID)
  {
     case  ID_HISTORY_OPEN:
	      pTTT->lpszText = "打开历史回忆";
	      break;
 	 case  ID_HISTORY_A_STEP:
          pTTT->lpszText = "同步历史回忆";
	      break;
     case ID_HISTORY_SWITCH:
		  pTTT->lpszText = "切换到指定日期";
	      break;
	 case ID_HISTORY_GOHEAD:
		   pTTT->lpszText = "向前";
	      break;
	 case ID_HISTORY_GOBACK:
		  pTTT->lpszText = "向后";
	      break;
	 case ID_HISTORY_HIDE:
		  pTTT->lpszText = "隐藏";
	      break;
     case ID_HISTORY_CLOSE:
          pTTT->lpszText = "关闭历史回忆";
	      break;
  }
  return TRUE;
} 


void CHistoryDlg::OnClose() 
{
	DestroyWindow();

}
void CHistoryDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
}
void CHistoryDlg::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
}

void CHistoryDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
}


void CHistoryDlg::OnHistoryHide() 
{

	DestroyWindow();
}


void CHistoryDlg::OnHistoryGohead() 
{
	OnDoGo(true);
}


void CHistoryDlg::OnHistoryGoback() 
{
	OnDoGo(false);
}

void CHistoryDlg::OnHistoryClose() 
{
	DestroyWindow();
}

void CHistoryDlg::OnHistoryAStep() 
{
	pView -> SetFocus();
	int nState = m_wndToolBar.GetToolBarCtrl().GetState(ID_HISTORY_A_STEP);
	m_wndToolBar.GetToolBarCtrl().SetState( ID_HISTORY_A_STEP,nState&~TBSTATE_ENABLED);//TBSTATE_CHECKED   

}


void CHistoryDlg::OnHistorySwitch() 
{
	CHistorySelDate dlg;
	if(m_strHistoryDate == "")
		dlg.m_tm = CTime::GetCurrentTime ();
	else
	{
		dlg.m_tm = CTime(atoi(m_strHistoryDate.Left (4)),
			atoi(m_strHistoryDate.Mid(4,2)),atoi(m_strHistoryDate.Right (2)),0,0,0);
	}
	if(dlg.DoModal () == IDOK)
	{
		MoveToDate(dlg.m_tm.Format ("%Y%m%d"));
		
		pView->SendMessage(WM_USER_CHANGESTOCK);
	
		pView->RedrawWindow();
		SetWindowText("历史回忆("+ m_strHistoryDate+")");

	}
	pView->SetFocus();
}


bool CHistoryDlg::OnDoGo(bool bGoHead)
{
	GetDateNavigated(bGoHead);

	pView->SendMessage(WM_USER_CHANGESTOCK);
	
	pView->RedrawWindow();
	SetWindowText("历史回忆("+ m_strHistoryDate+")");
	pView->SetFocus();

	return true;
}

void CHistoryDlg::PostNcDestroy() 
{

	pView->m_pHistoryDlg = NULL;

	pView->SendMessage(WM_USER_CHANGESTOCK);


	pView->SetHistoryColor(false);


	delete (CHistoryDlg*)this;

	CDialog::PostNcDestroy();
}

void CHistoryDlg::Open(CTaiShanReportView* pViewIn)
{
	CHistorySelDate dlg;
	dlg.m_tm = CTime::GetCurrentTime ();
	if(dlg.DoModal () != IDOK)
	{
		return;
	}

	if(pViewIn->m_pHistoryDlg == NULL)
	{
		CHistoryDlg* pDlg = new CHistoryDlg(pViewIn);
		pDlg->Create(IDD_DEAL_HISTORY ,pViewIn);
	}
	for( int k=1;k<5;k++)
	{
		pViewIn->m_pHistoryDlg->m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[k],TBSTATE_ENABLED );
	}

	pViewIn->m_pHistoryDlg->MoveToDate(dlg.m_tm.Format ("%Y%m%d"));
	//notify pview here
	pViewIn->SendMessage(WM_USER_CHANGESTOCK);
	//end
	pViewIn->RedrawWindow();
	pViewIn->m_pHistoryDlg->SetWindowText("历史回忆("+ pViewIn->m_pHistoryDlg->m_strHistoryDate+")");

	pViewIn->m_pHistoryDlg->ShowWindow (SW_SHOW);
	pViewIn->SetFocus();

}

void CHistoryDlg::DoHistory(STOCKDATASHOW *pData, int nCount,CTaiShanReportView* pViewIn)
{
	if(pViewIn->m_pHistoryDlg == NULL) return;
	for(int i = 0;i<nCount;i++)
	{
		if(i == nCount -1)
			i = nCount -1;
		pViewIn->m_pHistoryDlg->WriteOne(pData[i].pItem ,i);
	}
	for(int i = 0;i<nCount;i++)
	{
		pData[i].pItem = pViewIn->m_pHistoryDlg->GetOne(i);
	}
}
void CHistoryDlg::DoHistoryOne(STOCKDATASHOW *pData, int nFoot,CTaiShanReportView* pViewIn)
{
	return;
	if(pViewIn->m_pHistoryDlg == NULL) return;
	pViewIn->m_pHistoryDlg->WriteOne(pData[nFoot].pItem ,nFoot);
	pData[nFoot].pItem = pViewIn->m_pHistoryDlg->GetOne(nFoot);
}
