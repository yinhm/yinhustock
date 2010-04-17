// LogonDlg.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "LogonDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog


CLogonDlg::CLogonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogonDlg)
	m_checkAutoLogon = FALSE;
	m_editPassword = _T("");
	m_editUserName = _T("");
	m_checkSavePassword = FALSE;
	m_comboServer = _T("");
	//}}AFX_DATA_INIT
	
	m_LoginAuth.bAuthorizationState=FALSE;
	m_LoginAuth.dwAuthorizationPurview=0;
	m_hEvent.ResetEvent();

	m_bTempSavePassword=TRUE;
	m_bLogonSuccess=FALSE;
	for(int i=0;i<5;i++)
	{
		m_strServer[i]="";
	}
}


void CLogonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogonDlg)
	DDX_Control(pDX, IDC_COMBO_SERVER, m_comboCtrServer);
	DDX_Check(pDX, IDC_CHECK_AUTOLOGON, m_checkAutoLogon);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_editPassword);
	DDV_MaxChars(pDX, m_editPassword, 49);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_editUserName);
	DDV_MaxChars(pDX, m_editUserName, 19);
	DDX_Check(pDX, IDC_CHECK_SAVEPASSWORD, m_checkSavePassword);
	DDX_CBString(pDX, IDC_COMBO_SERVER, m_comboServer);
	DDV_MaxChars(pDX, m_comboServer, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogonDlg, CDialog)
	//{{AFX_MSG_MAP(CLogonDlg)
	ON_WM_CREATE()
	ON_MESSAGE(MSG_LOGON,OnMsgLogon)
	ON_BN_CLICKED(IDC_CHECK_AUTOLOGON, OnCheckAutologon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


int CLogonDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetLogonInformationFromReg();

	if(!m_bAutoLogon)
		return 0;
	if(ValidateUser(m_sAutoLogonServer,m_sAutoLogonUser,m_sAutoLogonPassword))
	{
		m_bLogonSuccess=TRUE;
		return -1;
	}
	else 
		return 0;	
	return 0;
}

BOOL CLogonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	if(m_bAutoLogon)
	{
		m_checkSavePassword=TRUE;
		((CButton*)GetDlgItem(IDC_CHECK_SAVEPASSWORD))->EnableWindow(FALSE);
		m_checkAutoLogon=TRUE;
	}
	else if(m_bSavePassword)
	{
		m_checkSavePassword=TRUE;
	}
	m_editUserName=m_sAutoLogonUser;
	if(m_bSavePassword)
		m_editPassword=m_sAutoLogonPassword;
	m_comboServer=m_sAutoLogonServer;
	if(m_strServer[0]!="")
		m_comboCtrServer.AddString(m_strServer[0]);
	if(m_strServer[1]!="")
		m_comboCtrServer.AddString(m_strServer[1]);
	if(m_strServer[2]!="")
		m_comboCtrServer.AddString(m_strServer[2]);
	if(m_strServer[3]!="")
		m_comboCtrServer.AddString(m_strServer[3]);
	if(m_strServer[4]!="")
		m_comboCtrServer.AddString(m_strServer[4]);
	UpdateData(FALSE);

	return TRUE; 
}

void CLogonDlg::OnOK() 
{

	UpdateData();
	if(ValidateUser(m_comboServer,m_editUserName,m_editPassword))
	{
		m_bLogonSuccess=TRUE;
		HKEY hKey;
		DWORD u32Disposition;
		DWORD u32cbData, u32Type;
		
		RegCreateKeyEx(
			HKEY_LOCAL_MACHINE,
			"SOFTWARE\\WsStock\\Ver2.0\\Auto Logon\\",0, 
			NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hKey, &u32Disposition);
		
		RegSetValueEx(hKey, "Auto Logon", 0, REG_BINARY, (CONST BYTE*)&m_checkAutoLogon, sizeof(m_checkAutoLogon));
		RegSetValueEx(hKey, "Save Password", 0, REG_BINARY, (CONST BYTE*)&m_checkSavePassword, sizeof(m_checkSavePassword));
		char* sUser=m_editUserName.GetBuffer(0);
		RegSetValueEx(hKey, "User", 0, REG_SZ,(BYTE*)sUser,m_editUserName.GetLength()+1);		
		if(m_checkSavePassword)
		{
			char* sPassword=m_editPassword.GetBuffer(0);
			RegSetValueEx(hKey, "Password", 0, REG_SZ,(BYTE*)sPassword,m_editPassword.GetLength()+1);
		}

		char* sLastServer=m_comboServer.GetBuffer(0);
		RegSetValueEx(hKey, "Last Server", 0, REG_SZ,(BYTE*)sLastServer,m_comboServer.GetLength()+1);

		if(m_comboServer!=m_sAutoLogonServer)
		{
			char* sServer=m_sAutoLogonServer.GetBuffer(0);
			char sTempServer[50];
			u32cbData = sizeof(sTempServer);
			if ( RegQueryValueEx(hKey, "Server1", 0, &u32Type, (LPBYTE)&sTempServer, &u32cbData) != ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, "Server1", 0, REG_SZ,(BYTE*)sServer,m_comboServer.GetLength()+1);				
			}
			else if ( RegQueryValueEx(hKey, "Server2", 0, &u32Type, (LPBYTE)&sTempServer, &u32cbData) != ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, "Server2", 0, REG_SZ,(BYTE*)sServer,m_comboServer.GetLength()+1);				
			}
			else if ( RegQueryValueEx(hKey, "Server3", 0, &u32Type, (LPBYTE)&sTempServer, &u32cbData) != ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, "Server3", 0, REG_SZ,(BYTE*)sServer,m_comboServer.GetLength()+1);				
			}
			else if ( RegQueryValueEx(hKey, "Server4", 0, &u32Type, (LPBYTE)&sTempServer, &u32cbData) != ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, "Server4", 0, REG_SZ,(BYTE*)sServer,m_comboServer.GetLength()+1);				
			}
			else if ( RegQueryValueEx(hKey, "Server5", 0, &u32Type, (LPBYTE)&sTempServer, &u32cbData) != ERROR_SUCCESS)
			{
				RegSetValueEx(hKey, "Server5", 0, REG_SZ,(BYTE*)sServer,m_comboServer.GetLength()+1);				
			}
			else
			{
				RegSetValueEx(hKey, "Server1", 0, REG_SZ,(BYTE*)sServer,m_comboServer.GetLength()+1);				
			}			
		}

		RegCloseKey(hKey);
		CDialog::OnOK();
	}
}

void CLogonDlg::OnCancel() 
{

	
	CDialog::OnCancel();
}

void CLogonDlg::GetLogonInformationFromReg()
{
	HKEY hKey;
	DWORD u32Disposition;
	DWORD u32cbData, u32Type;
	
	RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		"SOFTWARE\\WsStock\\Ver2.0\\Auto Logon\\",0, 
		NULL, REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS, NULL, &hKey, &u32Disposition);

	u32cbData = sizeof(m_bAutoLogon);
	if ( RegQueryValueEx(hKey, "Auto Logon", 0, &u32Type, (LPBYTE)&m_bAutoLogon, &u32cbData) != ERROR_SUCCESS)
	{
		m_bAutoLogon=false;
	}
	u32cbData = sizeof(m_bSavePassword);
	if ( RegQueryValueEx(hKey, "Save Password", 0, &u32Type, (LPBYTE)&m_bSavePassword, &u32cbData) != ERROR_SUCCESS)
	{
		m_bSavePassword=false;
	}
	char sUser[20];
	u32cbData = sizeof(sUser);
	if ( RegQueryValueEx(hKey, "User", 0, &u32Type, (LPBYTE)&sUser, &u32cbData) != ERROR_SUCCESS)
	{
		sUser[0]='\0';
	}
	m_sAutoLogonUser.Format("%s",sUser);
	char sPassword[50];
	u32cbData = sizeof(sPassword);
	if ( RegQueryValueEx(hKey, "Password", 0, &u32Type, (LPBYTE)&sPassword, &u32cbData) != ERROR_SUCCESS)
	{
		sPassword[0]='\0';
	}
	m_sAutoLogonPassword.Format("%s",sPassword);
	char sServer[50];
	u32cbData = sizeof(sServer);
	if ( RegQueryValueEx(hKey, "Last Server", 0, &u32Type, (LPBYTE)&sServer, &u32cbData) != ERROR_SUCCESS)
	{
		sServer[0]='\0';
	}
	m_sAutoLogonServer.Format("%s",sServer);

	char sServer1[50];
	u32cbData = sizeof(sServer1);
	if ( RegQueryValueEx(hKey, "Server1", 0, &u32Type, (LPBYTE)&sServer1, &u32cbData) == ERROR_SUCCESS)
	{
		m_strServer[0].Format("%s",sServer1);
	}
	char sServer2[50];
	if ( RegQueryValueEx(hKey, "Server2", 0, &u32Type, (LPBYTE)&sServer2, &u32cbData) == ERROR_SUCCESS)
	{
		m_strServer[1].Format("%s",sServer2);
	}
	char sServer3[50];
	if ( RegQueryValueEx(hKey, "Server3", 0, &u32Type, (LPBYTE)&sServer3, &u32cbData) == ERROR_SUCCESS)
	{
		m_strServer[2].Format("%s",sServer3);
	}
	char sServer4[50];
	if ( RegQueryValueEx(hKey, "Server4", 0, &u32Type, (LPBYTE)&sServer4, &u32cbData) == ERROR_SUCCESS)
	{
		m_strServer[3].Format("%s",sServer4);
	}
	char sServer5[50];
	if ( RegQueryValueEx(hKey, "Server5", 0, &u32Type, (LPBYTE)&sServer5, &u32cbData) == ERROR_SUCCESS)
	{
		m_strServer[4].Format("%s",sServer5);
	}
	

	RegCloseKey(hKey);
}

BOOL CLogonDlg::ValidateUser(CString sServer,CString sUser,CString sPassword)
{

#ifdef WIDE_NET_VERSION
#endif
	SetCapture();
	BeginWaitCursor();
	DWORD rsp=WaitForSingleObject(m_hEvent,10000);
	EndWaitCursor();
	ReleaseCapture();
	if(rsp==WAIT_OBJECT_0 && m_LoginAuth.bAuthorizationState==2001)
	{
		m_hEvent.ResetEvent();
		return TRUE;				
	}
	if(rsp==WAIT_TIMEOUT)
	{
		MessageBox("连接超时,请检查网络状况.","登录结果",MB_ICONSTOP);
	}
	return FALSE;
}

LRESULT CLogonDlg::OnMsgLogon(UINT wParam,LONG lParam)
{
	m_hEvent.SetEvent();
	RCV_DATA* pHeader;
	pHeader=(RCV_DATA *)lParam;
	if(wParam!=RCV_FILEDATA || pHeader->m_wDataType!=FILE_LOGINAUTH_EX)
	{
		TRACE("Error when sending logon message!");
	}
	else
	{
		m_LoginAuth=*(pHeader->m_pLoginAuth);
		switch(m_LoginAuth.bAuthorizationState)
		{
		case 1001:
			MessageBox("不能连接到服务器","登录结果",MB_ICONSTOP);
			break;
		case 1002:
			MessageBox("登入失败,不能登入到服务器","登录结果",MB_ICONSTOP);
			break;
		case 1003:
			MessageBox("连接超时","登录结果",MB_ICONSTOP);
			break;
		case 1004:
			MessageBox("数据出错或数据格式不正确","登录结果",MB_ICONSTOP);
			break;
		case 2001:
			TRACE("连接成功");
			break;
		case 2002:
			MessageBox("服务器行情关闭","登录结果",MB_ICONSTOP);
			break;
		}
	}

	return 0;
}

void CLogonDlg::OnCheckAutologon() 
{

	UpdateData();
	if(m_checkAutoLogon)
	{
		m_bTempSavePassword=m_checkSavePassword;
		m_checkSavePassword=TRUE;
		((CButton*)GetDlgItem(IDC_CHECK_SAVEPASSWORD))->EnableWindow(FALSE);
	}
	else
	{		
		((CButton*)GetDlgItem(IDC_CHECK_SAVEPASSWORD))->EnableWindow(TRUE);
		if(!m_bTempSavePassword)
			m_checkSavePassword=FALSE;
		else 
			m_checkSavePassword=TRUE;
	}
	UpdateData(FALSE);
}
