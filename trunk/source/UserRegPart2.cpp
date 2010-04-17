#include "stdafx.h"
#include "UserRegPart2.h"
#include "secretdrvhyt.h"
#include "GetHardDiskID1.h"

CString strKey = _T("SOFTWARE\\158Stock\\Ver1.0");
CString strItem1 = _T("id");
CString strItem2 = _T("power");
CString strItem3 = _T("Pos");
HANDLE hDevice;
char g_chMachineID[16];


int IsUserRegisteredPart2(CTime &t)
{

	return 0;
}

bool GetDiskIDPart2(CString &s, const char* IDE, int &nFlag, CTime &t)
{
	BOOL bRet;
	DWORD cbRet;
	int nTryTimes=12;
	char buf[524]; 
	int i,j;
	char pBuf[3];
	unsigned char nDevice;

	ASSERT(strlen(IDE)==2);
	strcpy(pBuf,IDE);

	
	for (i=0;i<nTryTimes;i++)
	{
		buf[0] = 1;
		bRet = DeviceIoControl(hDevice,	(DWORD)0x222001, buf, sizeof(buf), NULL, 0, &cbRet, NULL);
		if (bRet)
			break;
	}
	if (i==nTryTimes)
		return false;


	buf[0]=(pBuf[0]=='0') ? 0x00 : 0x01;
	buf[1]=(pBuf[1]=='0') ? 0x00 : 0x01;
	for (i=0;i<nTryTimes;i++)
	{
		bRet = DeviceIoControl(hDevice, (DWORD)0x222000, buf, sizeof(buf),buf,sizeof(buf),&cbRet,NULL);
		if (bRet && cbRet)
			break;
	}

	if (i==nTryTimes)
		return false;

	for (i=0;i<524;i++)
		buf[i]=~buf[i];

	nDevice=(unsigned char)(buf[0]);
	if ((nDevice & 0x40)!=0x40)	
	{
		return false;
	}
	else if((nDevice & 0x80)==0x80)
	{
		return false;
	}

	char str[61];
	for(i=20;i<40;i++) 
		if(i%2==0)
			str[i-20]=buf[i+1];
		else 
			str[i-20]=buf[i-1];

	for (i=54; i<92; i++)
		if (i%2==0)
			str[i-34]=buf[i+1];
		else
			str[i-34]=buf[i-1];
	str[60]=0;

	
	j=0;
	for (i=0; i<60; i++)
	{
		if ((str[i]>='a' && str[i]<='z') || (str[i]>='A' && str[i]<='Z') || (str[i]>='0' && str[i]<='9'))
		{
			str[j++] = str[i];
		}
	}


	if (j<8)
	{
		for (i=j; i<8; i++)
			str[i]='0';
	}
	str[8]=0;
	for(i = 0;i<8;i++)
	{
		BYTE nn = str[i];
		nn = nn%10;
		str[i] = nn + '0';
	}


	s=str;		

	
	int nYear,nMonth,nDay,nHour,nMinute,nSecond;
	nSecond=((buf[512] & 0xF0) >> 4) * 10 + (buf[512] & 0x0F); 
	nMinute=((buf[514] & 0xF0) >> 4) * 10 + (buf[514] & 0x0F); 
	nHour=((buf[516] & 0xF0) >> 4) * 10 + (buf[516] & 0x0F); 
	nDay=((buf[519] & 0xF0) >> 4) * 10 + (buf[519] & 0x0F); 
	nMonth = ((buf[520] & 0xF0) >> 4) * 10 + (buf[520] & 0x0F); 
	nYear=(((buf[522] & 0xF0) >> 4) * 10 + (buf[522] & 0x0F)) * 100 + ((buf[521] & 0xF0) >> 4) * 10 + (buf[521] & 0x0F); 
	try
	{
		CTime t1=CTime::GetCurrentTime();
		t=CTime(t1.GetYear(), nMonth, nDay, nHour, nMinute, nSecond);
		if (t<0)
			t=CTime::GetCurrentTime();
	}
	catch(...)
	{
		t=CTime::GetCurrentTime();
	}
	return true;
}


long ReadDiskIDPart2(const char* IDE, CString &strMachine, int &nFlag, int nDelay, CTime &t)
{

	int nTryTimes=10;
	CString strT1=_T("");
	CString strT2=_T("");
	int nSameTimes=0;
	int nMaxSameTimes=1;
	bool bResult;
	int rtn = 0;
	
	if (strcmp(IDE,"22")==0)
	{
		strMachine = "20020101";
		rtn = 1;
		goto END_READ;
	}
	else
	{
		int i;
		for (i=0;i<nTryTimes;i++)
		{
			bResult=GetDiskIDPart2(strT2,IDE,nFlag,t);
			if (bResult)
			{
				if (strT1 != strT2) 
				{
					nSameTimes=0;
					strT1=strT2;
				}
				else if (++nSameTimes > nMaxSameTimes)
					break;
			}	
			else
			{
				if (i<(nTryTimes-1))
				{
					if (nDelay>0)
						Sleep(nDelay * 1000);
				}
			}
		}
		if (i<nTryTimes)
		{
			strMachine = strT1;
			rtn = 1;
			goto END_READ;
		}
	}
	rtn = 0;
END_READ:

	strcpy(g_chMachineID,strMachine);
	return rtn;
}


BOOL IsCreditUserPart2(CString & strSerial,CString &strCredit,CString &strMachine)
{
	CString strCreditNew;
	CSecretDrv2 secret;
	bool bSuccess;
	if (strSerial.GetLength()!=16)
		return FALSE;
	if (strCredit.GetLength()!=8)
		return FALSE;
	CString strTemp = secret.DesecretSerialID(strSerial, bSuccess);
	if (!bSuccess) 
		return FALSE;
	if (!secret.CreateImpowerID(strTemp,strMachine,strCreditNew))
		return FALSE;
	strCreditNew.MakeUpper();
	strTemp=strCredit;
	strTemp.MakeUpper();
	if (strCreditNew==strTemp)
	{
		return TRUE;
	}
	else
	{

		return FALSE;
	}
}

BOOL OpenDiskVxdOrSys()
{

	DWORD dwVersion = GetVersion();
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));

	if (dwVersion < 0x80000000)               
	{
		hDevice = CreateFile("\\\\.\\LMBDRV", GENERIC_WRITE | GENERIC_READ,
						  FILE_SHARE_WRITE | FILE_SHARE_READ,
						  NULL, OPEN_EXISTING, 0, NULL);
	}
	else if (dwWindowsMajorVersion > 3)		
	{
		hDevice = CreateFile("\\\\.\\LMBVXD.VXD",0,0,NULL, 
					0,FILE_FLAG_DELETE_ON_CLOSE,NULL); 
	}
	else									
		return FALSE;

	if (hDevice == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

void CloseDiskVxdOrSys()
{
	CloseHandle(hDevice);
}
long ReadDiskIDPartCwd()
{
	
	CString sError;
	CString s = CGetHardDiskID::GetID(sError);

	strcpy(g_chMachineID,s);
	return 1;
}
