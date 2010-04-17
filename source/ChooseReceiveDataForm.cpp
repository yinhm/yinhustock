// ChooseReceiveDataForm.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ChooseReceiveDataForm.h"
#include "GetSetReg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

CChooseReceiveDataForm::CChooseReceiveDataForm(CWnd* pParent /*=NULL*/)
	: CDialog(CChooseReceiveDataForm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChooseReceiveDataForm)
	m_ShowDlg = FALSE;
	m_OldData = -1;
	//}}AFX_DATA_INIT
}


void CChooseReceiveDataForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseReceiveDataForm)
	DDX_Check(pDX, IDC_CHECK1, m_ShowDlg);
	DDX_Radio(pDX, IDC_RADIO1, m_OldData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChooseReceiveDataForm, CDialog)
	//{{AFX_MSG_MAP(CChooseReceiveDataForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CChooseReceiveDataForm::OnInitDialog() 
{// NOTE: the ClassWizard will add member functions here
	CDialog::OnInitDialog();
	
	
	this->m_OldData=0;
	
	switch(nWhich)
	{
	case 0:
		this->m_OldData=0;
		break;
	case 1:
		this->m_OldData=1;
		break;
	case 2:
		this->m_OldData=1;
		break;
	}
	UpdateData(FALSE);
	return TRUE;  
}

void CChooseReceiveDataForm::OnOK() //Éý¼¶
{//Default FontName
	
	UpdateData(TRUE);
	GetSetReg reg;
	char ch[512];
	memset(ch,0,512);
	DWORD ncb ;
	DWORD typ = REG_SZ;
	if(this->m_ShowDlg==1)
		strcpy(ch,"OK");
	else
		strcpy(ch,"NO");
	ncb = sizeof(ch);
	reg.SetValue ("software\\WsStock\\ReceiveDataForm","NotShow",typ,(unsigned char *)ch,ncb);
	memset(ch,0,512);
	if(this->m_OldData==0)
		strcpy(ch,"OLD");
	else if(this->m_OldData==1)
		strcpy(ch,"NEW");
	ncb = sizeof(ch);
	reg.SetValue ("software\\WsStock\\ReceiveDataForm","SelectForm",typ,(unsigned char *)ch,ncb);
	CDialog::OnOK();
}
