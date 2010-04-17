// LKeyBoardDial.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "LKeyBoardDial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LKeyBoardDial dialog


LKeyBoardDial::LKeyBoardDial(CWnd* pParent /*=NULL*/)
	: CDialog(LKeyBoardDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(LKeyBoardDial)
	
	//}}AFX_DATA_INIT
}


void LKeyBoardDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LKeyBoardDial)
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LKeyBoardDial, CDialog)
	//{{AFX_MSG_MAP(LKeyBoardDial)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void LKeyBoardDial::OnPaint() 
{
	CPaintDC dc(this); 
	
	
}
