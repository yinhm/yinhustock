// MyChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "MyChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiTestChildFrame

IMPLEMENT_DYNCREATE(CTaiTestChildFrame, CMDIChildWnd)

CTaiTestChildFrame::CTaiTestChildFrame()
{
}

CTaiTestChildFrame::~CTaiTestChildFrame()
{
}


BEGIN_MESSAGE_MAP(CTaiTestChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTaiTestChildFrame)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiTestChildFrame message handlers
