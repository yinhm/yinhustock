// STKDRV.h: interface for the CSTKDRV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CSTKDRV  
{
public:
	CSTKDRV();
	virtual ~CSTKDRV();
public:
	DWORD GetStockDrvInfo(int nInfo,void * pBuf);
	int SetupReceiver(BOOL bSetup);
	int GetTotalNumber();
	int Stock_Quit(HWND hWnd);
	int Stock_Init(HWND hWnd,UINT uMsg,int nWorkMode);
	void SetNewsPath(LPCTSTR lpszNewsPath);
	int SetExtMsg(UINT msg);
	int GetKData(LPCSTR scode, time_t mt);
	int QueryKData(LPCSTR scode, int nMarket, time_t mt);
	int GetMinData(LPCSTR scode, time_t mt);
	int QueryMinData(LPCSTR scode, int nMarket, time_t mt);

	int (WINAPI *	m_pfnStock_Init)(HWND hWnd,UINT Msg,int nWorkMode);
	int (WINAPI *	m_pfnStock_Quit)(HWND hWnd);
	int (WINAPI *	m_pfnGetTotalNumber)();													
	int	(WINAPI *	m_pfnSetupReceiver)(BOOL bSetup);
	DWORD (WINAPI * m_pfnGetStockDrvInfo)(int nInfo,void * pBuf);
	void	(WINAPI *	m_pfnSetNewsPath)(LPCTSTR lpszNewsPath);

	int (WINAPI *	m_pfnSetExtMsg)(UINT msg);
	int (WINAPI *	m_pfnGetKData)(LPCSTR scode, time_t mt);
	int (WINAPI *	m_pfnQueryKData)(LPCSTR scode, int nMarket, time_t mt);
	int  (WINAPI *	m_pfnGetMinData)(LPCSTR scode, time_t mt);
	int  (WINAPI *	m_pfnQueryMinData)(LPCSTR scode, int nMarket, time_t mt);


private:
	void GetAdress();
	HINSTANCE	m_hSTKDrv;
};

#endif 
