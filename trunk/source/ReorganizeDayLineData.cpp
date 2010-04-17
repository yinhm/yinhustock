// ReorganizeDayLineData.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ReorganizeDayLineData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReorganizeDayLineData dialog


CReorganizeDayLineData::CReorganizeDayLineData(CWnd* pParent /*=NULL*/)
	: CDialog(CReorganizeDayLineData::IDD, pParent)
{
	
}


void CReorganizeDayLineData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReorganizeDayLineData)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReorganizeDayLineData, CDialog)
	//{{AFX_MSG_MAP(CReorganizeDayLineData)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReorganizeDayLineData message handlers
