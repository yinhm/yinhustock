// clntsock.cpp : implementation of the CClientSocket class


#include "stdafx.h"
#include "CTaiShanApp.h"
#include "clntsock.h"
//#include "srvrdoc.h"
#include "msg.h"
#include <Nb30.h>

/////////////////////////////////////////////////////////////////////////////

typedef struct _ASTAT_
{
      ADAPTER_STATUS adapt;
      NAME_BUFFER    NameBuff [30];
}ASTAT, * PASTAT;
ASTAT Adapter;

CClientSocket::CClientSocket()
{

	m_nMsgCount = 0;
	m_pFile = NULL;
	m_pArchiveIn = NULL;
	m_pArchiveOut = NULL;
}

/////////////////////////////////////////////////////////////////////////////


void CClientSocket::Init()
{
	m_pFile = new CSocketFile(this);
	m_pArchiveIn = new CArchive(m_pFile,CArchive::load);
	m_pArchiveOut = new CArchive(m_pFile,CArchive::store);
}

void CClientSocket::Abort()
{
	if (m_pArchiveOut != NULL)
	{
		m_pArchiveOut->Abort();
		delete m_pArchiveOut;
		m_pArchiveOut = NULL;
	}
}

void CClientSocket::SendMsg(CMsg* pMsg)
{
	if (m_pArchiveOut != NULL)
	{
		pMsg->Serialize(*m_pArchiveOut);
		m_pArchiveOut->Flush();
	}
}

void CClientSocket::ReceiveMsg(CMsg* pMsg)
{
	pMsg->Serialize(*m_pArchiveIn);
}

/////////////////////////////////////////////////////////////////////////////
// CClientSocket Overridable callbacks

void CClientSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);


}

/////////////////////////////////////////////////////////////////////////////
// CSocket Implementation

CClientSocket::~CClientSocket()
{
	if (m_pArchiveOut != NULL)
		delete m_pArchiveOut;

	if (m_pArchiveIn != NULL)
		delete m_pArchiveIn;

	if (m_pFile != NULL)
		delete m_pFile;
}

#ifdef _DEBUG
void CClientSocket::AssertValid() const
{
	CSocket::AssertValid();
}

void CClientSocket::Dump(CDumpContext& dc) const
{
	CSocket::Dump(dc);
}
#endif 

IMPLEMENT_DYNAMIC(CClientSocket, CSocket)

bool CClientSocket::ConnectSocket(LPCTSTR lpszAddress, UINT nPort)
{
	if (!Create())
	{
		AfxMessageBox("create faild");
		return false;
	}

	while (!Connect(lpszAddress, nPort ))
	{
		if (AfxMessageBox("retry?",MB_YESNO) == IDNO)
		{
			return false;
		}
	}

	Init();

	return true;


}

bool CClientSocket::Certificate(LPCTSTR lpszAddress, UINT nPort, BYTE *pByte)
{
	CClientSocket clt;
	if(!clt.ConnectSocket( lpszAddress,  nPort))
		return false;


	  char buf[20];
	  NCB Ncb;
      UCHAR uRetCode;
      char NetName[50];
      LANA_ENUM   lenum;
      int      i;

      memset( &Ncb, 0, sizeof(Ncb) );
      Ncb.ncb_command = NCBENUM;
      Ncb.ncb_buffer = (UCHAR *)&lenum;
      Ncb.ncb_length = sizeof(lenum);
      uRetCode = Netbios( &Ncb );


      for(i=0; i <lenum.length ;i++)
      {
          memset( &Ncb, 0, sizeof(Ncb) );
          Ncb.ncb_command = NCBRESET;
          Ncb.ncb_lana_num = lenum.lana[i];

          uRetCode = Netbios( &Ncb );
         

          memset( &Ncb, 0, sizeof (Ncb) );
          Ncb.ncb_command = NCBASTAT;
          Ncb.ncb_lana_num = lenum.lana[i];

          strcpy((char * )Ncb.ncb_callname,  "*               " );
          Ncb.ncb_buffer = (unsigned char *) &Adapter;
          Ncb.ncb_length = sizeof(Adapter);

          uRetCode = Netbios( &Ncb );
          if ( uRetCode == 0 )
          {
             wsprintf(buf,"%02x%02x%02x%02x%02x%02x",
				  Adapter.adapt.adapter_address[0],
                  Adapter.adapt.adapter_address[1],
                  Adapter.adapt.adapter_address[2],
                  Adapter.adapt.adapter_address[3],
                  Adapter.adapt.adapter_address[4],
                  Adapter.adapt.adapter_address[5] );
			 break;
          }
       }
	;

	CMsg msg;
	msg.m_bClose = TRUE;
	memcpy(msg.m_nData,buf,12); 
	clt.SendMsg (&msg);

	INFO_NETCARDID msg2;
	int nrtn = clt.Receive (&msg2,sizeof(msg2),0);
	if(nrtn == SOCKET_ERROR ||nrtn == 0 || nrtn!= sizeof(msg2))
	{
		AfxMessageBox("");
		return true;
	}
	if(msg2.m_nID ==0)
	{
		AfxMessageBox("");
		return false;
	}
	return true;


}
