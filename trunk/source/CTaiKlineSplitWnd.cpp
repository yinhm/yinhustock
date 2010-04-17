// CTaiKlineSplitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineSplitWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTaiKlineSplitWnd, CSplitterWnd)

CTaiKlineSplitWnd::CTaiKlineSplitWnd()
{
}

CTaiKlineSplitWnd::~CTaiKlineSplitWnd()
{
}


BEGIN_MESSAGE_MAP(CTaiKlineSplitWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CTaiKlineSplitWnd)
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CWnd* CTaiKlineSplitWnd::GetActivePane(int *row, int *col)
{
	CWnd* pW = CSplitterWnd::GetActivePane(row, col);
	return pW;

}

void CTaiKlineSplitWnd::OnDestroy() 
{
	CSplitterWnd::OnDestroy();
	

	
}

void CTaiKlineSplitWnd::PostNcDestroy() 
{

	delete this;
	CSplitterWnd::PostNcDestroy();
}

void CTaiKlineSplitWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CSplitterWnd::OnActivate(nState, pWndOther, bMinimized);
	

	
}
