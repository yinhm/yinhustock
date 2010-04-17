// GetSetReg.cpp: implementation of the GetSetReg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetSetReg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GetSetReg::GetSetReg()
{

}

GetSetReg::~GetSetReg()
{

}

int GetSetReg::SetValue(CString strKey, CString strValueName, DWORD dwType, BYTE *lpData, DWORD cbData)
{
	HKEY hKey;
	CString Number;
	char str[20];
	int ec=0;
	DWORD dwDisposition;


    ec = RegCreateKeyEx(
		 HKEY_LOCAL_MACHINE,
		 strKey,
		 0,
		 NULL,
		 REG_OPTION_NON_VOLATILE,
		 KEY_ALL_ACCESS,
		 NULL,
		 &hKey,
		 &dwDisposition);

	  if (ERROR_SUCCESS == ec)
	  {
	
		  ec = RegSetValueEx(
				 hKey,
				 strValueName,
				 0,
				 dwType,
				 (BYTE *)lpData,
				 cbData);
		  if(ec!=ERROR_SUCCESS)
		  {
			  ASSERT(FALSE);
			  return ec;
		  }
	  }
	  else
		  return ec;
	  if(RegCloseKey(hKey)!=ERROR_SUCCESS)
		  ASSERT(FALSE);
	  return ec;
}

int GetSetReg::GetValue(CString strKey, CString strValueName, DWORD& dwType, BYTE *lpData, DWORD& cbData)
{

	HKEY hKey;
	int ec=RegOpenKey(HKEY_LOCAL_MACHINE,(LPCTSTR)strKey,&hKey);
	if(ERROR_SUCCESS != ec)
	{

		return ec;
	}

	ec=RegQueryValueEx(hKey,(LPCTSTR)strValueName,0,&dwType,(unsigned char *)lpData,&cbData);

	//char ch[1024];
	//int nSize;
	//void* pVoid;
	//FormatMessage(  FORMAT_MESSAGE_FROM_SYSTEM,   
	//  (void*)pVoid,   
	//  ec, 
	//  0, 
	//  ch,    
	//  1024,        
	//  NULL  
	//  );
	//int nn = GetLastError();
	RegCloseKey(hKey);
	return ec;

}

