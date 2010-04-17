// FyAddMoney.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "FyAddMoney.h"
#include "HttpGetData.h"
#include "GetSetReg.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "FyRegister.h"
#include "UserRegPart2.h"
#include "secretdrvhyt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define G_NDAYS 15
#define G_NTIMES 50
//#define Test_FuYou
static CString g_sFileWriteTime = "regsvr64.dll_";
/////////////////////////////////////////////////////////////////////////////

int FyAddMoney::g_nUseTimes = -1;


FyAddMoney::FyAddMoney(CWnd* pParent /*=NULL*/)
	: CDialog(FyAddMoney::IDD, pParent)
{
	//{{AFX_DATA_INIT(FyAddMoney)
	m_sUser = _T("");
	m_sID = _T("");
	//}}AFX_DATA_INIT
}


void FyAddMoney::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FyAddMoney)
	DDX_Text(pDX, IDC_EDIT1, m_sUser);
	DDV_MaxChars(pDX, m_sUser, 32);
	DDX_Text(pDX, IDC_EDIT7, m_sID);
	DDV_MaxChars(pDX, m_sID, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FyAddMoney, CDialog)
	//{{AFX_MSG_MAP(FyAddMoney)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void FyAddMoney::OnCancel() 
{

	CDialog::OnCancel();
}

void FyAddMoney::OnOK() 
{
	if(!UpdateData()) return;
	if(m_sUser == ""
		||m_sID == ""
		)
	{
		AfxMessageBox("£¡");
		return;
	}

	SaveSettings(m_sUser, m_sID);
	ShowTime(m_sID);
	CDialog::OnOK();

}

CString FyAddMoney::GetReturnCode(CString sUrl,bool bPrompt, int &nErrorCode)
{
	CString sError = "ÎÞ";
	/*
	HttpGetData htt;
	CString s = htt.GetHtmlCode( sUrl);

	nErrorCode = -1;
	if(!s.IsEmpty())
	{
		CString ss = "Zhan Lue Jia:Error";
		CFile fl;
		if(fl.Open ("c:\\httpfile.txt",CFile::modeCreate|CFile::modeWrite))
			fl.Write (s.GetBuffer (0),s.GetLength ());
		fl.Close ();
		int nf = s.Find(ss,0);
		if(nf>=0)
		{
			CString s2 = s.Right (s.GetLength ()-nf-ss.GetLength ());
			if(s2.GetLength ()>0)
			{
				CString st = s2.Left(2);
				int nn = atoi(st);
				nErrorCode = nn;
				switch(nn)
				{
				case 0:
					{
					sError = "£¡";
					}
					break;
				case 1:
					sError = "£¡";
					break;
				case 2:
					sError = "£¡";
					break;
				case 3:
					sError = "£¡";
					break;
				case 4:
					sError = "£¡";
					break;
				case 5:
					sError = "¡£";
					break;
				case 6:
					sError = "£¡";
					break;
				case 7:
					sError = "£¡";
					break;
				case 8:
					sError = "£¡";
					break;
				case 9:
					sError = "";
					break;
				case 10:
					sError = "";
					break;
				}
			}
		}
		if(bPrompt && nErrorCode!=0) AfxMessageBox(sError);
		if(nErrorCode!=0)
		{
		}
	}
	//*/

	return sError;
}

void FyAddMoney::SaveSettings(CString sUser, CString sID)
{
	GetSetReg reg;
	char ch[512];
	strcpy(ch,sUser);
	DWORD ncb = 512;
	DWORD typ = REG_SZ;
	int n = reg.SetValue("software\\DlsRegister", "RegisterID", typ, (unsigned char *)ch,ncb);
	if(n == ERROR_SUCCESS)
	{
	}
	strcpy(ch,sID);
	ncb = 512;
	n = reg.SetValue("software\\DlsRegister", "ProductID", typ, (unsigned char *)ch,ncb);
	if(n == ERROR_SUCCESS)
	{
	}
}

void FyAddMoney::ReadSettings(CString &sUser, CString &sID)
{
	sUser="";
	sID = "";
	GetSetReg reg;
	char ch[512];
	DWORD ncb = 512;
	DWORD typ = REG_SZ;
	int n = reg.GetValue("software\\DlsRegister", "RegisterID", typ, (unsigned char *)ch,ncb);
	if(n == ERROR_SUCCESS)
	{
		sUser = ch;
	}
	ncb = 512;
	n = reg.GetValue("software\\DlsRegister", "ProductID", typ, (unsigned char *)ch,ncb);
	if(n == ERROR_SUCCESS)
	{
		sID = ch;
	}
}

bool FyAddMoney::ToLogOn()
{
				return false;
}

bool FyAddMoney::ToLogOn2()
{
			return false;
}


BOOL FyAddMoney::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	ReadSettings(m_sUser, m_sID);
	UpdateData(FALSE);
	
	return TRUE;  
}

CString FyAddMoney::GetUrl()
{
	CString sUrl="lmb";
	GetSetReg reg;
	char ch[512];
	CFile fl;
	if(fl.Open("RegistServerIP.txt",CFile::modeRead))
	{
		int nLen = fl.GetLength ();
		if(nLen>512) nLen = 512;
		fl.Read (ch,nLen);
		ch[511] = 0;
		sUrl = ch;
		sUrl.MakeLower ();
	}
	{
		if(sUrl.Left (5)!= "http:")
		{
			sUrl.TrimLeft ("\\");
			sUrl.TrimLeft ("/");
			sUrl = "http://"+sUrl;
		}
	}
	return sUrl;

}

void FyAddMoney::SetUrl(char *chUrl)
{
	GetSetReg reg;
	char ch[512];
	strcpy(ch,chUrl);
	DWORD ncb = sizeof(ch);
	DWORD typ = REG_SZ;
	int n = reg.SetValue("software\\DlsRegister", "Url", typ, (unsigned char *)ch,ncb);
	if(n == ERROR_SUCCESS)
	{
	}

}

void FyAddMoney::ShowTime(CString sID)
{

}
