// GuiZhouTvView.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "GuiZhouTvView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGuiZhouTvView

IMPLEMENT_DYNCREATE(CGuiZhouTvView, CEditView)

CGuiZhouTvView::CGuiZhouTvView()
{
}

CGuiZhouTvView::~CGuiZhouTvView()
{
}


BEGIN_MESSAGE_MAP(CGuiZhouTvView, CEditView)
	//{{AFX_MSG_MAP(CGuiZhouTvView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CGuiZhouTvView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
}

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CGuiZhouTvView::AssertValid() const
{
	CEditView::AssertValid();
}

void CGuiZhouTvView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif 

/////////////////////////////////////////////////////////////////////////////


BOOL CGuiZhouTvView::ShowInfo(CString strFile)
{
	
	CEdit& pEditCtrl=GetEditCtrl();	
	pEditCtrl.SetReadOnly(TRUE);
	pEditCtrl.SetSel(0,0);
	pEditCtrl.ReplaceSel("Edit ”Õº");
	return TRUE;
}
