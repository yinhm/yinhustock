// CompanyInfoView.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "resource.h"
#include "CompanyInfoView.h"
#include "MainFrm.h"
#include "KEYBRODEANGEL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompanyInfoView

IMPLEMENT_DYNCREATE(CCompanyInfoView, CEditView)

CCompanyInfoView::CCompanyInfoView()
{
}

CCompanyInfoView::~CCompanyInfoView()
{
  
   ((CMainFrame *)AfxGetMainWnd())->m_EnableF10 = 0;
}


BEGIN_MESSAGE_MAP(CCompanyInfoView, CEditView)
	//{{AFX_MSG_MAP(CCompanyInfoView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CCompanyInfoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

}

/////////////////////////////////////////////////////////////////////////////
// CCompanyInfoView diagnostics

#ifdef _DEBUG
void CCompanyInfoView::AssertValid() const
{// serious allocation failure checking
	CEditView::AssertValid();
}

void CCompanyInfoView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif 

/////////////////////////////////////////////////////////////////////////////


int CCompanyInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ // standard constructor
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	CFont m_Font;
	LOGFONT m_lf;
	CFontDialog zjh;
	if(zjh.DoModal() == IDOK)
	{
	   zjh.GetCurrentFont(&m_lf);
       memset(&m_lf,0,sizeof(LOGFONT));
	   VERIFY(m_Font.CreateFontIndirect(&m_lf));
	   SetFont(&m_Font,TRUE); 
	}

	return 0;
}

void CCompanyInfoView::OnFilePrintPreview() 
{// NOTE: the ClassWizard will add member functions here
  
	CView::OnFilePrintPreview();   
	
}


BOOL CCompanyInfoView::PreTranslateMessage(MSG* pMsg) 
{// NOTE: the ClassWizard will add member functions here

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
		((CMainFrame *)pMainWnd)->ReViewInfo();
		
		return TRUE;

	}
	return CEditView::PreTranslateMessage(pMsg);
}

void CCompanyInfoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{// NOTE: the ClassWizard will add member functions here

	if(nChar == VK_ESCAPE)
	{
	  GetParent()->SendMessage(WM_CLOSE);
	  return;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCompanyInfoView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
	
}
