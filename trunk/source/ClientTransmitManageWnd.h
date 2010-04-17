#if !defined(AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_)
#define AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocketManageWnd.h : header file
//

#define	WM_SOCKET		WM_USER+10	
#define MSG_LOGON		WM_USER+20	
#define MSG_MYTREEVIEW		WM_USER+30	
#define	Gp_Msg_StkData	WM_APP + 1
/////////////////////////////////////////////////////////////////////////////
// CClientTransmitManageWnd window

typedef struct _SOCKET_ID
{
	HWND	hWnd;	
	int		iFlag;	
}SOCKET_ID;

typedef struct _TRANSENVIRONMENT
{
		TRANS_PACKAGE_STRUCTEx transPackage;
		int action;							
		SOCKET_ID sock_id;				
		BOOL bIsNull;						
}TRANSENVIRONMENT;

//#include "MainFrm.h"

class CClientTransmitManageWnd : public CWnd 
{

public:

	CClientTransmitManageWnd();
	CClientTransmitManageWnd(CMDIFrameWnd* pMainFrame);
	virtual ~CClientTransmitManageWnd();

	void GetMarketData(TRANS_PACKAGE_STRUCTEx* pTransPackage,int action,SOCKET_ID sock_id,BOOL AutoUnregister=FALSE);
	void ReleaseID(SOCKET_ID socket_id);
	SOCKET_ID RegisterID(HWND hWnd);

	CMDIFrameWnd* m_pMainFrame;


public:


public:


	//{{AFX_VIRTUAL(CClientTransmitManageWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	int WINAPI Stock_Quit(HWND hWnd);
	int WINAPI Stock_Init(HWND hWnd,UINT Msg,int nWorkMode);

	int WINAPI InitNetworkAndClientLogin(char* szServerIp,char* ServerPort,char* useName,char* usePass);

	// Generated message map functions
protected:
	//{{AFX_MSG(CClientTransmitManageWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnStkDataOK(UINT wParam,LONG lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void ChangeFormat(RCV_BASEINFO_STRUCTEx* pBaseInfo,BASEINFO* baseinfo);
	void StkDataManage(UINT wParam,LONG lParam);
	BOOL GetMarketData(TRANSENVIRONMENT transEnvironment);
	int WINAPI GetMarketData(TRANS_PACKAGE_STRUCTEx *pTransPackage ,int action , DWORD dwTransEnvironment);
	void AdjustNextIDNumber();
	CView* GetCurrentActiveView();
	
	BOOL m_bIDStatus[50];
						
	int m_iNextID;		
						
	TRANSENVIRONMENT m_transArray[50][2];
						
	TRANSENVIRONMENT m_transUnending;
							

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKETMANAGEWND_H__F3F0A262_E27F_406C_9B33_1D6E8950628E__INCLUDED_)
