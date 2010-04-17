// clntsock.h : interface of the CClientSocket class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __CLNTSOCK_H__
#define __CLNTSOCK_H__

class CMsg;
//class CServerDoc;

class CClientSocket : public CSocket
{
	DECLARE_DYNAMIC(CClientSocket);
private:
	CClientSocket(const CClientSocket& rSrc);      
	void operator=(const CClientSocket& rSrc);  


public:
	CClientSocket();


public:
	int m_nMsgCount;
	CSocketFile* m_pFile;
	CArchive* m_pArchiveIn;
	CArchive* m_pArchiveOut;

	BOOL IsAborted() { return m_pArchiveOut == NULL; }


public:
	void Init();
	void Abort();
	void SendMsg(CMsg* pMsg);
	void ReceiveMsg(CMsg* pMsg);


protected:
	virtual void OnReceive(int nErrorCode);


public:
	static bool Certificate(LPCTSTR lpszAddress, UINT nPort,BYTE* pByte = 0);
	bool ConnectSocket(LPCTSTR lpszAddress, UINT nPort);
	virtual ~CClientSocket();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

#endif // __CLNTSOCK_H__
