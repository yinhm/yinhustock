// MyRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "MyRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiTestRichEditCtrl

CTaiTestRichEditCtrl::CTaiTestRichEditCtrl()
{
}

CTaiTestRichEditCtrl::~CTaiTestRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaiTestRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CTaiTestRichEditCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiTestRichEditCtrl message handlers

BOOL CTaiTestRichEditCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message==WM_KEYDOWN || pMsg->message==WM_KEYUP)
	{
		
			return TRUE;
	}
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}
