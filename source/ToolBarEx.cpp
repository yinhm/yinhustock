// ToolBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


CToolBarEx::CToolBarEx()
{
}

CToolBarEx::~CToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarEx, CTabCtrl)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CToolBarEx::OnPaint() 
{
	CPaintDC dc(this); 
	
	
}

BOOL CToolBarEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{// NOTE: the ClassWizard will add member functions here
	
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
