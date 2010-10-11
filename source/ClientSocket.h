
#pragma once

#include "TCPSocketAsync.h"

class CClientSocket : public CTCPSocketAsync
{
public:
	CClientSocket();
	virtual ~CClientSocket();

protected:
	NO_OnSocketTimeout
	NO_OnSocketConnect
	NO_OnSocketAccept
	//NO_OnSocketClose
	NO_OnSocketOOB
	NO_OnSocketWrite
	//NO_OnSocketReceive

protected:
	virtual BOOL OnSocketClose(int iErrorCode);
	virtual BOOL OnSocketReceive(int iErrorCode);
};
