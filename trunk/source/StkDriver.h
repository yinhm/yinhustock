
#if !defined(AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CStkDriver  
{
public:
	CStkDriver();
	virtual ~CStkDriver();

protected:
	HINSTANCE m_hInstance;

protected:
	int (WINAPI* m_pfnStock_Init)(HWND hWnd, UINT Msg, int nWorkMode);
	int (WINAPI* m_pfnStock_Quit)(HWND hWnd);
	int (WINAPI* m_pfnGetTotalNumber)();
	int	(WINAPI* m_pfnSetupReceiver)(BOOL bSetup);
	DWORD (WINAPI* m_pfnGetStockDrvInfo)(int nInfo, void* pBuf);

protected:
	void GetAdress();

public:
	int Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode);
	int Stock_Quit(HWND hWnd);
	int GetTotalNumber();
	int SetupReceiver(BOOL bSetup);
	DWORD GetStockDrvInfo(int nInfo, void* pBuf);
};

#endif 
