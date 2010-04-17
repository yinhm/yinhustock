// FyRegister.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "FyRegister.h"
#include "FyAddMoney.h"
#include "HttpGetData.h"
#include "GetSetReg.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "UserRegPart2.h"
#include "secretdrvhyt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////



FyRegister::FyRegister(CWnd* pParent /*=NULL*/)
	: CDialog(FyRegister::IDD, pParent)
{
	//{{AFX_DATA_INIT(FyRegister)
	m_sMachineID = _T("");
	m_sEmail = _T("");
	m_sID = _T("");
	m_sPhone = _T("");
	m_sAddress = _T("");
	m_sPower = _T("");
	//}}AFX_DATA_INIT

	m_sMachineID = g_chMachineID;
}


void FyRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FyRegister)
	DDX_Text(pDX, IDC_EDIT_DISKID, m_sMachineID);
	DDV_MaxChars(pDX, m_sMachineID, 32);
	DDX_Text(pDX, IDC_EDIT10, m_sEmail);
	DDX_Text(pDX, IDC_EDIT7, m_sID);
	DDV_MaxChars(pDX, m_sID, 16);
	DDX_Text(pDX, IDC_EDIT8, m_sPhone);
	DDX_Text(pDX, IDC_EDIT9, m_sAddress);
	DDX_Text(pDX, IDC_EDIT_POWER, m_sPower);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FyRegister, CDialog)
	//{{AFX_MSG_MAP(FyRegister)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonHand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL FyRegister::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	FyAddMoney::ReadSettings(m_sPower, m_sID);
	UpdateData(FALSE);
	
		
	return TRUE; 
}

void FyRegister::OnOK() 
{
	if(!UpdateData()) return;
	if(m_sMachineID == ""
		||m_sID == ""
		||m_sPhone == ""
		||m_sAddress == ""
		||m_sEmail == ""
		)
	{
		AfxMessageBox("£¡");
		return;
	}
	CString s = FyAddMoney::GetUrl();
	s.TrimRight ("\\");
	s.TrimRight ("/");
	s+="/ServerLogOnManage/RegisteResult.asp";

	CSecretDrv2 secret;
	bool bSuccess;
	CString strTemp = secret.DesecretSerialID(m_sID, bSuccess);

	CString url = s;
	url+="?suser=";
	url+=strTemp;
	url+="&cardid=";


	url+=strTemp ;

	CString sMachine = g_chMachineID;
	url+="&spassword=";
	url+=sMachine;

	url+="&phone=";
	url+=m_sPhone;

	url+="&address=";
	url+=m_sAddress;

	url+="&zone=";
	url+=m_sEmail;

	int nErrorCode;
	s = FyAddMoney::GetReturnCode(url,true, nErrorCode);

	if(nErrorCode == 0)
	{
		CString strCreditNew;
		CSecretDrv2 secret;
		if (!secret.CreateImpowerID(strTemp,m_sMachineID,strCreditNew))
			return ;
		strCreditNew.MakeUpper();

	
		FyAddMoney::SaveSettings(strCreditNew, m_sID);
	
		CString ss ;
		ss.Format ("¡£",strCreditNew);
		AfxMessageBox(ss);
		CDialog::OnOK();
	}
}

void FyRegister::OnCancel() 
{

	
	CDialog::OnCancel();
}

void FyRegister::OnButtonHand() 
{
	if(!UpdateData()) return;

	BOOL b = IsCreditUserPart2(this->m_sID ,this->m_sPower ,this->m_sMachineID );
	if(!b)
	{
	
		FyAddMoney::SaveSettings(m_sPower, m_sID);
	
		AfxMessageBox("£¡");
		CDialog::OnOK();
	}
	else
	{
		
		FyAddMoney::SaveSettings(m_sPower, m_sID);
	
		AfxMessageBox("£¡");
		CDialog::OnOK();
	}
}

bool FyRegister::IsValidUser()
{
	CString strSerial;
	CString strCredit;
	CString strMachine = g_chMachineID;
	FyAddMoney::ReadSettings(strCredit, strSerial);

	return true;
}
