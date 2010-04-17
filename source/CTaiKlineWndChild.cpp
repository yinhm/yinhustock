// CTaiKlineWndChild.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineWndChild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CTaiKlineWndChild, CMDIChildWnd)

CTaiKlineWndChild::CTaiKlineWndChild()
{
	m_pContext = NULL;
}

CTaiKlineWndChild::~CTaiKlineWndChild()
{
	if(m_pContext)  delete m_pContext;
}


BEGIN_MESSAGE_MAP(CTaiKlineWndChild, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTaiKlineWndChild)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CTaiKlineWndChild::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

	
		m_pContext=new CCreateContext;
		memcpy(m_pContext,pContext,sizeof(CCreateContext));
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
