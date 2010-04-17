// MyRichView.cpp : implementation file
//

#include "stdafx.h"
#include <afxdlgs.h>
#include "CTaiShanApp.h"
#include <share.h>
#include  <io.h>
#include "MyRichView.h"
#include "MytreeView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanApp.h"
#include "resource.h"
#include "KEYBRODEANGEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiTestRichView
IMPLEMENT_DYNCREATE(CTaiTestRichView, CRichEditView)
CTaiTestRichView::CTaiTestRichView()
{
	m_rectMargin.SetRect(1800, 1440, 1800, 1440);
    m_bF9ORF10 = TRUE;
}

CTaiTestRichView::~CTaiTestRichView()
{
}


BEGIN_MESSAGE_MAP(CTaiTestRichView, CRichEditView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CTaiTestRichView)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_RICH_COPY, OnRichCopy)
	ON_COMMAND(ID_NEW_F10,OnNewF10)
	ON_UPDATE_COMMAND_UI(ID_NEW_F10, OnUpdateNewF10)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiTestRichView message handlers

int CTaiTestRichView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	CFont m_Font;
	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -16;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");
	VERIFY(m_Font.CreateFontIndirect(&m_lf));
	SetFont(&m_Font,TRUE);
	return 0;
}



void CTaiTestRichView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if(nChar == VK_ESCAPE)
	{
	  GetParent()->GetParent()->SendMessage(WM_CLOSE);
	  return;
	}
	if(nChar == VK_TAB)
	{
	   CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	   CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	  
	   CTaiTestTreeView *p_Info =(CTaiTestTreeView *)(((CChildFrame *)pActiveWnd)->m_SplitterWnd)->GetPane(0,0);
	   ((CChildFrame *)pActiveWnd)->SetActiveView(p_Info,TRUE);
	   p_Info->ChangeView = false;
	   p_Info->SetFocus();
	  return;
	}
	
	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTaiTestRichView::OnPreparePrinting(CPrintInfo* pInfo) 
{

	CRect pagerect = GetMargins();
	SetMargins(CRect(1440,1000,10,10));
	WrapChanged();
	pagerect = GetMargins();
	return DoPreparePrinting(pInfo);
}


void CTaiTestRichView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{

	CRichEditView::OnBeginPrinting(pDC, pInfo);
}

void CTaiTestRichView::OnFilePrint() 
{


	CRichEditView::OnFilePrint();

}

void CTaiTestRichView::OnContextMenu(CWnd*, CPoint point)
{


	{
		if (point.x == -1 && point.y == -1){
	
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_COPYDAATA1));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}


void CTaiTestRichView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   	long startChar,endChar;
	this->GetRichEditCtrl().GetSel(startChar,endChar);
	ClientToScreen(&point);
	SendMessage(WM_CONTEXTMENU,WPARAM(this->m_hWnd),MAKELPARAM(point.x,point.y));
}

void CTaiTestRichView::OnRichCopy() 
{
   SendMessage(WM_COPY);	
}


BOOL CTaiTestRichView::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_F1)	
	{
	DoHtmlHelp(this);return TRUE;


	}
	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
			CTaiKeyBoardAngelDlg m_diakeybrode;
			if(pMsg->wParam>='a'&&pMsg->wParam<='z')
				pMsg->wParam-=32;
			char input=pMsg->wParam;
			m_diakeybrode.input=input;
			if(pMsg->wParam>='0'&&pMsg->wParam<='9')
				m_diakeybrode.ischar=false;
			else
				m_diakeybrode.ischar=true;

		m_diakeybrode.DoModal();
	    
		
		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
		if(m_diakeybrode.isresultok == false || m_diakeybrode.isresultstock == false  )
		{
		 return TRUE;
		}
		pDoc->m_sharesSymbol = m_diakeybrode.result;
        pDoc->m_stkKind=  m_diakeybrode.m_stkKind;
		CString temp_str;
		char currdir[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH,currdir);
		if(m_diakeybrode.m_stkKind==SHAG||m_diakeybrode.m_stkKind==SHBG||m_diakeybrode.m_stkKind==SHZQ)
		{
		    temp_str.Format("%s\\%s%s.TXT",currdir,g_sF10sh,pDoc->m_sharesSymbol);
		}
		else if(m_diakeybrode.m_stkKind==SZAG||m_diakeybrode.m_stkKind==SZBG||m_diakeybrode.m_stkKind==SZZQ)
		{
		   temp_str.Format("%s\\%s%s.TXT",currdir,g_sF10sz,pDoc->m_sharesSymbol);
		}
		if(_access(temp_str.GetBuffer(0),0)==-1)   
		{
            AfxMessageBox("该股票文件公告信息不存在!");
			return TRUE;
		}
        CFile file(temp_str,CFile::modeRead);
		char* broadcastmessage=new char[file.GetLength()];		
		file.Read(broadcastmessage,file.GetLength() );
		file.Close();
		GetRichEditCtrl().ShowWindow(SW_HIDE);
		GetRichEditCtrl().SetReadOnly(TRUE);

		GetRichEditCtrl().SetSel(0,-1);
		GetRichEditCtrl().ReplaceSel(broadcastmessage);
		GetRichEditCtrl().SetSel(0,0);
		GetRichEditCtrl().ShowWindow(SW_SHOW);
		GetRichEditCtrl().GetLimitText();

		pDoc->SetModifiedFlag(FALSE);
	    UpdateData(FALSE);

		CView* pView;
		POSITION pos = pDoc->GetFirstViewPosition();   
		while (pos != NULL)   
		{
		  pView = pDoc->GetNextView(pos);
		  if(pView->IsKindOf(RUNTIME_CLASS(CTaiTestTreeView)))
		  {
			  ((CTaiTestTreeView *)(pView))->FindStockBaseInfoPos(pDoc->m_sharesSymbol.GetBuffer(0));
		  }
		} 


        delete[] broadcastmessage;
		SetFocus();
		return TRUE;
	}
	return CRichEditView::PreTranslateMessage(pMsg);
}

void CTaiTestRichView::OnNewF10() 
{
    CTaiShanDoc *pDoc = ((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;
	if(pDoc == NULL)
		return;
	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();   
	while (pos != NULL)   
	{
      pView = pDoc->GetNextView(pos);
      if(pView->IsKindOf(RUNTIME_CLASS(CTaiTestTreeView)))
	  {
		  if(this->m_bF9ORF10 == ((CTaiTestTreeView *)(pView))->m_bF9ORF10)
		  {
			  ((CTaiTestTreeView *)(pView))->OnGetUpdate();
			  break;
		  }
	  }
	} 
	return;
}


void CTaiTestRichView::OnUpdateNewF10(CCmdUI* pCmdUI) 
{
  if(m_bF9ORF10)
	  pCmdUI->Enable(FALSE);
  else
	  pCmdUI->Enable();
}

