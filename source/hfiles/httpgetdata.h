// HttpGetData.h: interface for the HttpGetData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPGETDATA_H__C15F39ED_9792_4B5A_A408_DFB6A0792B82__INCLUDED_)
#define AFX_HTTPGETDATA_H__C15F39ED_9792_4B5A_A408_DFB6A0792B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxinet.h>
#include <iostream>
#include <stdlib.h>

class CHtmlGetSession : public CInternetSession
{
public:
	CHtmlGetSession(LPCTSTR pszAppName, int nMethod);
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus,
		LPVOID lpvStatusInfomration, DWORD dwStatusInformationLen);
};


class CHtmlGetException : public CException
{
	DECLARE_DYNCREATE(CHtmlGetException)

public:
	CHtmlGetException(int nCode = 0);
	~CHtmlGetException() { }
	static void ThrowTearException(int nCode);

	int m_nErrorCode;
};

class AFX_EXT_CLASS HttpGetData  
{
public:
	CString GetHtmlCode(CString url);
	HttpGetData();
	virtual ~HttpGetData();

};

#endif // !defined(AFX_HTTPGETDATA_H__C15F39ED_9792_4B5A_A408_DFB6A0792B82__INCLUDED_)
