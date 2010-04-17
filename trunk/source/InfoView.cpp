// InfoView.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "InfoView.h"
#include "MainFrm.h"
#include "KEYBRODEANGEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoView
IMPLEMENT_DYNCREATE(CInfoView, CRichEditView)
CInfoView::CInfoView()
{
}

CInfoView::~CInfoView()
{
  pDoc->m_sharesSymbol = m_OldStockSymbol;
  if(pDoc->m_InfoView)
	  pDoc->m_InfoView = NULL;
  ((CMainFrame *)AfxGetMainWnd())->m_EnableF10 = 0;
}


BEGIN_MESSAGE_MAP(CInfoView, CRichEditView)
	//{{AFX_MSG_MAP(CInfoView)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////


int CInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
    
	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -16;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	CFont m_Font;
	strcpy(m_lf.lfFaceName,"ËÎÌו");
	VERIFY(m_Font.CreateFontIndirect(&m_lf));
	SetFont(&m_Font,TRUE);

	((CMainFrame *)AfxGetMainWnd())->m_EnableF10 = 0;
	return 0;
}

void CInfoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	if(nChar == VK_ESCAPE)
	{
	  GetParent()->SendMessage(WM_CLOSE);
	  return;
	}
	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInfoView::OnFilePrint() 
{

    CRichEditView::OnFilePrint();
}


BOOL CInfoView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	
	CRect pagerect = GetMargins();
	SetMargins(CRect(1440,1440,10,10));
	WrapChanged();
	pagerect = GetMargins();
	return DoPreparePrinting(pInfo);
	
}

void CInfoView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	
	CRichEditView::OnBeginPrinting(pDC, pInfo);
}

void CInfoView::OnFilePrintPreview() 
{

	CRichEditView::OnFilePrintPreview();
}

BOOL CInfoView::PreTranslateMessage(MSG* pMsg) 
{
	
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

	    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();

		if(m_diakeybrode.isresultok == false || m_diakeybrode.isresultstock == false  )
		{
		 return TRUE;
		}
		pDoc->m_sharesSymbol = m_diakeybrode.result;
		((CMainFrame *)pMainWnd)->ReViewInfo();
		
		return TRUE;

	}
	return CRichEditView::PreTranslateMessage(pMsg);
}

void CInfoView::OnInitialUpdate() 
{
	CRichEditView::OnInitialUpdate();
}
