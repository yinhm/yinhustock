// Serial.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "Serial.h"
#include "secretdrv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Serial dialog


Serial::Serial(CWnd* pParent /*=NULL*/)
	: CDialog(Serial::IDD, pParent)
{
	//{{AFX_DATA_INIT(Serial)
	m_Serial = _T("");
	//}}AFX_DATA_INIT
}


void Serial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Serial)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_Serial);
	DDV_MaxChars(pDX, m_Serial, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Serial, CDialog)
	//{{AFX_MSG_MAP(Serial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Serial message handlers

void Serial::OnOK() 
{
	// TODO: Add extra v  alidation here
	int key=0;
	char desecret[20];
	CString temp;
	CSecretDrv *pSecretDrv;

	pSecretDrv=new CSecretDrv;
    
	this->UpdateData(TRUE);
	temp=Number;
    key=atoi(temp.Right(2));
	pSecretDrv->secret(desecret,Number,key);
	if(this->m_Serial!=desecret)
	{
		AfxMessageBox("输入序列号不正确！ 请重输入序列号。 ");
	    this->m_edit .SetFocus(); 
		this->m_edit .SetSel(m_Serial.GetLength()); 
		return;
	}
	HKEY hKey;
	CString	 strKey=_T("SOFTWARE\\TL\\Skyer");
	if(ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE,(LPCTSTR)strKey,&hKey))
	{
			 int cc=RegSetValueEx(
					 hKey,
					 "Serial",
					 0,
					 REG_SZ,
					 (BYTE *)desecret ,
					 (strlen(desecret) + 1 )*sizeof(TCHAR));
	         RegCloseKey(hKey);
	}    

	delete pSecretDrv;

	
	CDialog::OnOK();
}

BOOL Serial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_edit .SetFocus(); 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
