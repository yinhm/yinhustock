// DlgShowInformation.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DlgShowInformation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogShowInformation::CDialogShowInformation(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogShowInformation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogShowInformation)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sHtml = "";
}


void CDialogShowInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogShowInformation)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogShowInformation, CDialog)
	//{{AFX_MSG_MAP(CDialogShowInformation)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CDialogShowInformation::OnClose() 
{

	;
	EndDialog(IDOK);

}

BOOL CDialogShowInformation::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect r;
	GetClientRect(r);
	r.bottom -= 50;
	m_htmlShow.Create(this,r);

	if(m_sHtml!="")
		m_htmlShow.Navigate (m_sHtml.GetBuffer (0));

	
	
	return TRUE;  
	           
}

void CDialogShowInformation::OnFinalRelease() 
{

	
	CDialog::OnFinalRelease();
}

void CDialogShowInformation::OnDestroy() 
{
	CDialog::OnDestroy();
	

	
}

void CDialogShowInformation::PostNcDestroy() 
{

	
	CDialog::PostNcDestroy();
}

void CDialogShowInformation::OnOk() 
{

	OnClose() ;
}

void CDialogShowInformation::OnCancel() 
{

	
	OnClose() ;
}
