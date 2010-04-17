// ClientSocketManageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ClientTransmitManageWnd.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "share.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientTransmitManageWnd

CClientTransmitManageWnd::CClientTransmitManageWnd()
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW);
	CRect rect(0,0,0,0);
	CreateEx(0, lpszClassName, "MarketDataMainWindow", WS_OVERLAPPEDWINDOW, rect, NULL, 0);
	ShowWindow(SW_HIDE);

	m_iNextID=0;
	for(int i=0;i<50;i++)
	{
		m_bIDStatus[i]=FALSE;
		m_transArray[i][0].bIsNull=TRUE;
		m_transArray[i][1].bIsNull=TRUE;
	}
	m_pMainFrame=NULL;
}

CClientTransmitManageWnd::CClientTransmitManageWnd(CMDIFrameWnd* pMainFrame)
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW);
	CRect rect(0,0,0,0);
	CreateEx(0, lpszClassName, "MarketDataMainWindow", WS_OVERLAPPEDWINDOW, rect, NULL, 0);
	ShowWindow(SW_HIDE);

	m_iNextID=0;
	for(int i=0;i<50;i++)
	{
		m_bIDStatus[i]=FALSE;
		m_transArray[i][0].bIsNull=TRUE;
		m_transArray[i][1].bIsNull=TRUE;
	}
	m_pMainFrame=pMainFrame;
}

CClientTransmitManageWnd::~CClientTransmitManageWnd()
{
}


BEGIN_MESSAGE_MAP(CClientTransmitManageWnd, CWnd)
	//{{AFX_MSG_MAP(CClientTransmitManageWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SOCKET, OnStkDataOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////


int CClientTransmitManageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	Stock_Init(this->GetSafeHwnd(),WM_SOCKET,0);
	return 0;
}

void CClientTransmitManageWnd::PostNcDestroy() 
{

	Stock_Quit(this->GetSafeHwnd());
	CWnd::PostNcDestroy();
}

SOCKET_ID CClientTransmitManageWnd::RegisterID(HWND hWnd)

{
	SOCKET_ID sock_id;
	sock_id.hWnd=hWnd;
	sock_id.iFlag=m_iNextID;
	m_bIDStatus[m_iNextID]=TRUE;
	AdjustNextIDNumber();
	return sock_id;
}

void CClientTransmitManageWnd::ReleaseID(SOCKET_ID socket_id)

{
	m_bIDStatus[socket_id.iFlag]=FALSE;
}

LONG CClientTransmitManageWnd::OnStkDataOK(UINT wParam,LONG lParam)
{
	RCV_COMMINTERFACE_STRUCTEx  *pCommInterface;  
	SOCKET_ID sock_id;
	DWORD dwTransEnvironment;
	pCommInterface=(RCV_COMMINTERFACE_STRUCTEx*)lParam;
	dwTransEnvironment= pCommInterface->m_dwTransEnvironment;
	sock_id=*(SOCKET_ID*)dwTransEnvironment;


	StkDataManage(wParam,lParam);

	if(!m_bIDStatus[sock_id.iFlag])
	{
		m_transArray[sock_id.iFlag][0].bIsNull=TRUE;
		m_transArray[sock_id.iFlag][1].bIsNull=TRUE;
	}
	else
	{
		m_transArray[sock_id.iFlag][0].bIsNull=TRUE;
		CView* pView;
		pView=GetCurrentActiveView();
		if(pView->GetSafeHwnd()==sock_id.hWnd)
		{
			if(!m_transArray[sock_id.iFlag][1].bIsNull)
			{
				m_transArray[sock_id.iFlag][0].action=m_transArray[sock_id.iFlag][1].action;
				m_transArray[sock_id.iFlag][0].sock_id=m_transArray[sock_id.iFlag][1].sock_id;
				m_transArray[sock_id.iFlag][0].transPackage=m_transArray[sock_id.iFlag][1].transPackage;
				m_transArray[sock_id.iFlag][0].bIsNull=FALSE;
				m_transArray[sock_id.iFlag][1].bIsNull=TRUE;

				GetMarketData(m_transArray[sock_id.iFlag][0]);
			}
		}
		else
		{
		
			for(int i=0;i<50;i++)
			{
				if(m_bIDStatus[i] && m_transArray[i][1].bIsNull==FALSE && m_transArray[i][1].sock_id.hWnd==pView->GetSafeHwnd())
				{					
					m_transArray[i][0].action=m_transArray[i][1].action;
					m_transArray[i][0].sock_id=m_transArray[i][1].sock_id;
					m_transArray[i][0].transPackage=m_transArray[i][1].transPackage;
					m_transArray[i][0].bIsNull=FALSE;
					m_transArray[i][1].bIsNull=TRUE;
					
					GetMarketData(m_transArray[i][0]);
					break;					
				}
			}
		}
	}

	return 0L;
}

CView* CClientTransmitManageWnd::GetCurrentActiveView()
{
	ASSERT(m_pMainFrame);
	CMDIChildWnd *pChild = 
		(CMDIChildWnd *)m_pMainFrame->GetActiveFrame();
	
	CView *pView = (CView *) pChild->GetActiveView();	

	return pView;
}

void CClientTransmitManageWnd::AdjustNextIDNumber()
{
	BOOL bOK=FALSE;
	for(int i=0;i<50;i++)
	{
		if(m_iNextID++>=50)
			m_iNextID=0;
		if(m_bIDStatus[m_iNextID]==FALSE)
		{
			bOK=TRUE;
			break;
		}
	}
	if(!bOK)
		TRACE("Error:The ID is full!");
}

void CClientTransmitManageWnd::GetMarketData(TRANS_PACKAGE_STRUCTEx* pTransPackage,int action,SOCKET_ID sock_id,BOOL AutoUnregister)

{
	BOOL rsp;
	if(action==1 || action==-1)
	{
		m_transUnending.transPackage=*pTransPackage;
		m_transUnending.action=action;
		m_transUnending.sock_id=sock_id;
		m_transUnending.bIsNull=FALSE;
		rsp=GetMarketData(m_transUnending);
		if(!rsp)
			TRACE("Error:GetMarketData() failed!");
	}
	else
	{
		if(!m_transArray[sock_id.iFlag][0].bIsNull)
		{
			m_transArray[sock_id.iFlag][1].transPackage=*pTransPackage;
			m_transArray[sock_id.iFlag][1].action=action;
			m_transArray[sock_id.iFlag][1].sock_id=sock_id;
			m_transArray[sock_id.iFlag][1].bIsNull=FALSE;
		}
		else
		{
			m_transArray[sock_id.iFlag][0].transPackage=*pTransPackage;
			m_transArray[sock_id.iFlag][0].action=action;
			m_transArray[sock_id.iFlag][0].sock_id=sock_id;
			m_transArray[sock_id.iFlag][0].bIsNull=FALSE;

			rsp=GetMarketData(m_transArray[sock_id.iFlag][0]);
			if(!rsp)
				TRACE("Error:GetMarketData() failed!");
		}
	}	
	if(AutoUnregister)
		ReleaseID(sock_id);
}


BOOL CClientTransmitManageWnd::GetMarketData(TRANSENVIRONMENT transEnvironment)
{
	int rsp=GetMarketData(&(transEnvironment.transPackage),transEnvironment.action,DWORD(&(transEnvironment.sock_id)));
	if(rsp==-1)
		return FALSE;
	return TRUE;
}

int WINAPI CClientTransmitManageWnd::GetMarketData(TRANS_PACKAGE_STRUCTEx *pTransPackage, int action, DWORD dwTransEnvironment)

{
	return 0;
}

int WINAPI CClientTransmitManageWnd::Stock_Init(HWND hWnd, UINT Msg, int nWorkMode)
{
	return 0;
}

int WINAPI CClientTransmitManageWnd::Stock_Quit(HWND hWnd)
{
	return 0;
}

int WINAPI CClientTransmitManageWnd::InitNetworkAndClientLogin(char *szServerIp, char *ServerPort, char *useName, char *usePass)
{
	return 0;
}

void CClientTransmitManageWnd::StkDataManage(UINT wParam, LONG lParam)
{
}


void CClientTransmitManageWnd::ChangeFormat(RCV_BASEINFO_STRUCTEx* pBaseInfo, BASEINFO* baseinfo)
{
	strcpy(baseinfo->Symbol,pBaseInfo->m_szLabel);  

	baseinfo->zgb=pBaseInfo->m_fZgb;     
	baseinfo->gjg=pBaseInfo->m_fGjg;     
	baseinfo->fqrfrg=pBaseInfo->m_fFqrfrg; 
	baseinfo->frg=pBaseInfo->m_fFrg;    
	baseinfo->zgg=pBaseInfo->m_fZgg;     
	baseinfo->gzAg=pBaseInfo->m_fGzAg;    
	baseinfo->ltAg=pBaseInfo->m_fLtAg;   
	baseinfo->Hg=pBaseInfo->m_fHg;      
	baseinfo->Bg=pBaseInfo->m_fBg;   
	baseinfo->zpg=pBaseInfo->m_fZpg;     

	baseinfo->zzc=pBaseInfo->m_fZzc;   
	baseinfo->ldzc=pBaseInfo->m_fLdzc;    
	baseinfo->cqtz=pBaseInfo->m_fCqtz;    
	baseinfo->gdzc=pBaseInfo->m_fGdzc;   
	baseinfo->wxzc=pBaseInfo->m_fWxzc;   
	baseinfo->ldfz=pBaseInfo->m_fLdfz;   
	baseinfo->cqfz=pBaseInfo->m_fCqfz;   
	baseinfo->ggqy=pBaseInfo->m_fGgqy;    
	baseinfo->zbgjj=pBaseInfo->m_fZbgjj; 
	baseinfo->yygjj=pBaseInfo->m_fYygjj; 
	baseinfo->mgjz=pBaseInfo->m_fMgjz;   
	baseinfo->gdqybl=pBaseInfo->m_fGdqybl; 
	baseinfo->mggjj=pBaseInfo->m_fMggjj;  

	baseinfo->zyywsr=pBaseInfo->m_fZyywsr; 
	baseinfo->zyywlr=pBaseInfo->m_fZyywlr;  
	baseinfo->qtywlr=pBaseInfo->m_fQtywlr;  
	baseinfo->lrze=pBaseInfo->m_fLrze;   
	baseinfo->jlr=pBaseInfo->m_fJlr;    
	baseinfo->wfplr=pBaseInfo->m_fWfplr;   
	baseinfo->mgsy=pBaseInfo->m_fMgsy;    
	baseinfo->jzcsyl=pBaseInfo->m_fJzcsyl; 
	baseinfo->mgwfplr=pBaseInfo->m_fMgwfplr;

	baseinfo->mgjzc=pBaseInfo->m_fMgjzc; 


}
