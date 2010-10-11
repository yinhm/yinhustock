
#include "stdafx.h"
//#include "CTaiShanApp.h"
#include "StkDriver.h"

//#include "GetSetReg.h"

CStkDriver::CStkDriver()
{
	m_hInstance = NULL;

	FreeAddress();
}

CStkDriver::~CStkDriver()
{
	UnloadDriver();

	if (m_hInstance)
	{
		m_hInstance = NULL;
	}
}
//
//void CStkDriver::GetAdress()
//{
	//CFileFind flfd;
	//CString m_CurrentWorkDirectory;
	//GetCurrentDirectory(MAX_PATH, m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	//m_CurrentWorkDirectory.ReleaseBuffer();
	//m_CurrentWorkDirectory.TrimRight("\\");
	//m_CurrentWorkDirectory.TrimRight("/");
	//m_CurrentWorkDirectory +="\\";

	//if (flfd.FindFile(/*m_CurrentWorkDirectory + "\\System\\Stock.dll"*/"E:\\NetStock\\JStockclt\\Stock.dll"))
	//{
	//	m_hInstance = LoadLibrary(/*m_CurrentWorkDirectory + "\\System\\Stock.dll"*/"E:\\NetStock\\JStockclt\\Stock.dll");
	//}
	//else
	//{
	//	GetSetReg reg;
	//	char ch[512];
	//	DWORD ncb = sizeof(ch);
	//	DWORD typ = REG_SZ;
	//	if (reg.GetValue("software\\stockdrv", "stockdrv", typ, (unsigned char *)ch, ncb) == ERROR_SUCCESS)
	//	{
	//		ch[511] = 0;
	//		m_hInstance = LoadLibrary(ch);
	//	}
	//	else
	//	{
	//		m_hInstance = LoadLibrary("Stock.dll");
	//	}
	//}
//
//	if (!m_hInstance)
//	{
//		LoadLibrary("Stock.dll");
//	}
//}

void CStkDriver::GetAddress()
{
	if (!m_hInstance)
	{
		return;
	}

	m_pfnStock_Init = (int (WINAPI*)(HWND, UINT, int))GetProcAddress(m_hInstance, "Stock_Init");
	m_pfnStock_Quit = (int (WINAPI*)(HWND))GetProcAddress(m_hInstance, "Stock_Quit");
	m_pfnGetTotalNumber = (int (WINAPI*)())GetProcAddress(m_hInstance, "GetTotalNumber");
	m_pfnSetupReceiver = (int (WINAPI*)(BOOL))GetProcAddress(m_hInstance, "SetupReceiver");
	m_pfnGetStockDrvInfo = (DWORD (WINAPI*)(int, void*))GetProcAddress(m_hInstance, "GetStockDrvInfo");
}

void CStkDriver::FreeAddress()
{
	m_pfnStock_Init = NULL;
	m_pfnStock_Quit = NULL;
	m_pfnGetTotalNumber = NULL;
	m_pfnSetupReceiver = NULL;
	m_pfnGetStockDrvInfo = NULL;
}

BOOL CStkDriver::LoadDriver(LPCTSTR lpszDriver)
{
	CString	strLibrary = lpszDriver;
	if (strLibrary.GetLength() <= 0)
	{
		return FALSE;
	}

	m_hInstance = LoadLibrary(strLibrary);
	if (!m_hInstance)
	{
		return FALSE;
	}

	GetAddress();

	return TRUE;
}

void CStkDriver::UnloadDriver()
{
	if (m_hInstance)
	{
		BOOL nResult = FreeLibrary(m_hInstance);
		m_hInstance = NULL;
	}

	FreeAddress();
}

int CStkDriver::Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode)
{
	if (!m_pfnStock_Init)
		return -1;

	return ((*m_pfnStock_Init)(hWnd, uMsg, nWorkMode));
}

int CStkDriver::Stock_Quit(HWND hWnd)
{
	if (!m_pfnStock_Quit)
		return -1;

	return ((*m_pfnStock_Quit)(hWnd));
}

int CStkDriver::GetTotalNumber()
{
	if (!m_pfnGetTotalNumber)
		return -1;

	return ((*m_pfnGetTotalNumber)());
}

int CStkDriver::SetupReceiver(BOOL bSetup)
{
	if (!m_pfnSetupReceiver)
		return -1;

	return ((*m_pfnSetupReceiver)(bSetup));
}

DWORD CStkDriver::GetStockDrvInfo(int nInfo, void *pBuf)
{
	if (!m_pfnGetStockDrvInfo)
		return -1;

	return ((*m_pfnGetStockDrvInfo)(nInfo, pBuf));
}

void CStkDriver::Start(HWND hWnd, BOOL bReceive, DWORD dwPid)
{
}

void CStkDriver::ProcessData(WPARAM wParam, LPARAM lParam)
{
}
