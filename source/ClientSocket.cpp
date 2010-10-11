
#include "stdafx.h"
#include "ClientSocket.h"

#include "Response.h"
using namespace StockMarket;

#define CClientSocket_Class "CClientSocket"

CClientSocket::CClientSocket() : CTCPSocketAsync(FALSE)
{
}

CClientSocket::~CClientSocket()
{
}

BOOL CClientSocket::OnSocketClose(int iErrorCode)
{
	return TRUE;
}

BOOL CClientSocket::OnSocketReceive(int iErrorCode)
{
	char buffer[2048];
	int receive;

	receive = Receive(buffer, sizeof(buffer));

	ProcessResponsePacket((const uchar*)buffer, receive, false);

	return true;
}
