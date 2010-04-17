// InvestLogin.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "InvestLogin.h"
#include "EditAccount.h"
#include "ManageInvest.h"
#include "direct.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


CInvestLogin::CInvestLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CInvestLogin::IDD, pParent)
{
	

	m_bZoom=true;
}


void CInvestLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvestLogin)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SETTING, m_setting);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_editAccount);
	DDX_Control(pDX, IDC_DELETE_ACCOUNT, m_delete);
	DDX_Control(pDX, IDC_ADD_ACCOUNT, m_add);
	DDX_Control(pDX, IDC_USERLIST, m_UserCtrlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvestLogin, CDialog)
	//{{AFX_MSG_MAP(CInvestLogin)
	ON_BN_CLICKED(IDC_SETTING, OnSetting)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
	ON_BN_CLICKED(IDC_ADD_ACCOUNT, OnAddAccount)
	ON_BN_CLICKED(IDC_EDIT_ACCOUNT, OnEditAccount)
	ON_BN_CLICKED(IDC_DELETE_ACCOUNT, OnDeleteAccount)
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, OnDblclkUserlist)
	ON_NOTIFY(LVN_KEYDOWN, IDC_USERLIST, OnKeydownUserlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CInvestLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	



	this->ReadRegInfo();
	

	CRect rect;
	ListView_SetExtendedListViewStyle(  m_UserCtrlList.m_hWnd, LVS_EX_FULLROWSELECT);
	m_UserCtrlList.GetClientRect(&rect);
	m_UserCtrlList.InsertColumn(0,"号",LVCFMT_CENTER,rect.Width()/4);
	m_UserCtrlList.InsertColumn(1,"用",LVCFMT_LEFT,rect.Width()/2);
	m_UserCtrlList.InsertColumn(2,"间",LVCFMT_LEFT,rect.Width()/4);

	this->Refresh();

	GetWindowRect(&rect);
	MoveWindow(rect.left,rect.top,rect.Width(), rect.Height()/2 );

	return TRUE;  
}


void CInvestLogin::ReadRegInfo()
{
    CString szKey=_T("SOFTWARE\\WsStock\\Ver2.0\\Invest");
	
	HKEY hResult;

	if( ERROR_SUCCESS != RegOpenKey(HKEY_LOCAL_MACHINE, (LPCTSTR)szKey,&hResult ) )
		return;

	BYTE Byte[4096];
	DWORD cbData=4096;

	if( RegQueryValueEx(hResult,(LPCTSTR)"Account", 0, 0,  &Byte[0], &cbData ) != ERROR_SUCCESS )
	{
		RegCloseKey(hResult);
		return;
	}
	RegCloseKey(hResult);
	
	Account* pAccount=new Account[ cbData/sizeof(Account)];
	memcpy( pAccount,(Account*)Byte,cbData );
	for( int i=0;i<int( cbData/sizeof(Account) );i++ )
		m_arrAccount.Add( *(pAccount+i) );
	delete []pAccount;

	for(int i=0;i<m_arrAccount.GetSize();i++)
	{
		for( int j=0;j<16;j++)
			m_arrAccount[i].chPassword[j]=~m_arrAccount[i].chPassword[j];
	}


}

void CInvestLogin::WriteRegInfo()
{
	HKEY hKey;
    CString szKey=_T("SOFTWARE\\WsStock\\Ver2.0\\Invest");
	DWORD dwDisposition;
    RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		szKey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS|KEY_CREATE_SUB_KEY,
		NULL,
		&hKey,
		&dwDisposition
		);
	


	for( int i=0;i<m_arrAccount.GetSize();i++)
	{
		for( int j=0;j<16;j++)
			m_arrAccount[i].chPassword[j]=~m_arrAccount[i].chPassword[j];
	}

	RegSetValueEx(
		hKey,
		(LPCTSTR)"Account",
		0,
		REG_BINARY,
		(BYTE*)m_arrAccount.GetData(),
		sizeof(Account)*m_arrAccount.GetSize()
		);

	for(int i=0;i<m_arrAccount.GetSize();i++)
	{
		for(int j=0;j<16;j++)
			m_arrAccount[i].chPassword[j]=~m_arrAccount[i].chPassword[j];
	}

}

void CInvestLogin::OnSetting() 
{
	CRect rect;
	GetWindowRect(&rect);

	if( m_bZoom )
	{
		GetDlgItem(IDC_SETTING)->SetWindowText("<<设置");
		MoveWindow(rect.left,rect.top,rect.Width(), rect.Height()*2 );
		m_bZoom=FALSE;
	}
	else
	{
		GetDlgItem(IDC_SETTING)->SetWindowText("设置>>");
		MoveWindow(rect.left,rect.top,rect.Width(), rect.Height()/2 );
		m_bZoom=TRUE;
	}
	
	

}

void CInvestLogin::OnClose() 
{

	CDialog::OnClose();
}

void CInvestLogin::OnSelchangeCombo() 
{
	int nCurSel=((CComboBox*)GetDlgItem(IDC_COMBO))->GetCurSel();
	if( m_arrAccount[nCurSel].bRemember )
	{
		GetDlgItem(IDC_PASSWORD)->SetWindowText( m_arrAccount[nCurSel].chPassword );
		((CButton*)GetDlgItem(IDC_CHECK))->SetCheck(1);
	}
	else
	{
		GetDlgItem(IDC_PASSWORD)->SetWindowText("");
		((CButton*)GetDlgItem(IDC_CHECK))->SetCheck(0);
	}
}

void CInvestLogin::OnAddAccount() 
{

	CEditAccount EditDlg;
	EditDlg.m_Operate=0;

	if( EditDlg.DoModal()==IDOK )
	{
	
		CString str;
	
		for( int i=0;i<m_arrAccount.GetSize();i++)
		{
			str=m_arrAccount[i].chName;
			
			if(	str.CompareNoCase(EditDlg.m_szName) == 0 )
			{
				AfxMessageBox("该帐户名已经存在!");
				return;
			}
		}
	
		
	
		m_szFileName="Data/Account/";
		m_szFileName+=EditDlg.m_szName;
		m_szFileName+=".acc";
		if( _access("Data/Account",0)==-1 )
			_mkdir("Data/Account");

		CFile UserFile(m_szFileName,CFile::modeWrite|CFile::modeCreate);

		strcpy(m_Account.chPassword,EditDlg.m_szPassword );
		for( int j=0;j<16;j++)
		m_Account.chPassword[j]=~m_Account.chPassword[j];
		m_Account.uRecords=0;
		UserFile.Write(&m_Account,sizeof(m_Account));

		UserFile.Close();

	
		
		Account acnt;
		acnt.bRemember=0;
		strcpy(acnt.chName,(LPCTSTR)EditDlg.m_szName);

		strcpy(acnt.chPassword,(LPCTSTR)EditDlg.m_szPassword);
		CTime tm=CTime::GetCurrentTime();
		acnt.uTime=tm.GetYear()*10000+tm.GetMonth()*100+tm.GetDay();
		m_arrAccount.Add(acnt);
		WriteRegInfo();
		this->Refresh();
	}
}

void CInvestLogin::OnEditAccount() 
{
	CEditAccount EditDlg;
	EditDlg.m_Operate=1;

	POSITION pos = m_UserCtrlList.GetFirstSelectedItemPosition();
	
	int nItem;
	if(pos)
	{
		nItem = m_UserCtrlList.GetNextSelectedItem(pos);
		EditDlg.m_szName=m_UserCtrlList.GetItemText(nItem, 1 );
		EditDlg.m_szOldPassword=m_arrAccount[nItem].chPassword;
	}
	else
	{
		AfxMessageBox("没有选择被编辑的内容.",MB_ICONSTOP);
		return;
	}


	if( EditDlg.DoModal()==IDOK )
	{
		Account acnt;
		acnt.bRemember=0;
		strcpy(acnt.chName,(LPCTSTR)EditDlg.m_szName);
		strcpy(acnt.chPassword,(LPCTSTR)EditDlg.m_szPassword);
		CTime tm=CTime::GetCurrentTime();
		acnt.uTime=tm.GetYear()*10000+tm.GetMonth()*100+tm.GetDay();
		m_arrAccount[nItem]=acnt;
		WriteRegInfo();
		this->Refresh();
	}
	
	
}

void CInvestLogin::OnDeleteAccount() 
{
	POSITION pos = m_UserCtrlList.GetFirstSelectedItemPosition();
	
	if(pos)
	{
		int nItem = m_UserCtrlList.GetNextSelectedItem(pos);
		if( AfxMessageBox("真的要进行删除?",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
		{
			CString str=m_arrAccount[nItem].chName;
			str+=".acc";
			DeleteFile(str);

			m_arrAccount.RemoveAt(nItem);
			this->Refresh();
			WriteRegInfo();
		}
	}
	else
		AfxMessageBox("没有选择被删除的内容.",MB_ICONSTOP);
	
}

void CInvestLogin::Refresh()
{
	((CComboBox*)GetDlgItem(IDC_COMBO))->ResetContent();
	m_UserCtrlList.DeleteAllItems();

	char ch[20];
	for(  int j=0;j<m_arrAccount.GetSize();j++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO))->InsertString(j, m_arrAccount[j].chName );

		sprintf(ch,"%d",j+1);
		m_UserCtrlList.InsertItem(j,ch);
		m_UserCtrlList.SetItemText(j,1,m_arrAccount[j].chName);
		sprintf(ch,"%d",m_arrAccount[j].uTime);
		m_UserCtrlList.SetItemText(j,2,ch );
	}
}


void CInvestLogin::OnOK() 
{

	int nCurSel=((CComboBox*)GetDlgItem(IDC_COMBO))->GetCurSel();
	if(nCurSel < 0) return;

	CString str;
	GetDlgItem(IDC_PASSWORD)->GetWindowText( str );

	if( str.CompareNoCase(m_arrAccount[nCurSel].chPassword) != 0 )
	{
		AfxMessageBox("密码输入错误.",MB_ICONSTOP);
		return;
	}
	
	CDialog::OnOK();

	if(  ((CButton*)GetDlgItem(IDC_CHECK))->GetCheck() )
		m_arrAccount[nCurSel].bRemember=1;
	else
		m_arrAccount[nCurSel].bRemember=0;

	WriteRegInfo();


	CSuperviseInvest ManageInvest;
	if( _access("Data/Account",0)==-1 )
		_mkdir("Data/Account");
	ManageInvest.m_szFileName+="Data/Account/";
	ManageInvest.m_szFileName+=m_arrAccount[nCurSel].chName;
	ManageInvest.m_szFileName+=".acc";
	strcpy(ManageInvest.m_Password,str);
	ManageInvest.DoModal();
}

void CInvestLogin::OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{

	this->OnEditAccount();

	*pResult = 0;
}

void CInvestLogin::OnKeydownUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if( pLVKeyDow->wVKey==VK_DELETE)
		this->OnDeleteAccount();

	*pResult = 0;
}
