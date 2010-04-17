// mainFrmD.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "mainFrmD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////



mainFrmD::mainFrmD(CWnd* pParent /*=NULL*/)
	: CDialog(mainFrmD::IDD, pParent)
{
	
}


void mainFrmD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
}


BEGIN_MESSAGE_MAP(mainFrmD, CDialog)
	//{{AFX_MSG_MAP(mainFrmD)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mainFrmD message handlers

int mainFrmD::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	


#include "stdafx.h"



	
	return 0;
}
