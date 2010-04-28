// STKDRV.cpp: implementation of the CSTKDRV class.
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "STKDRV.h"
#include "GetSetReg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSTKDRV::CSTKDRV()
{
	m_hSTKDrv = NULL;

	m_pfnGetStockDrvInfo	= NULL;
	m_pfnGetTotalNumber		= NULL;
	m_pfnSetupReceiver		= NULL;
	m_pfnStock_Init			= NULL;
	m_pfnStock_Quit			= NULL;

	m_pfnSetNewsPath		= NULL;
	m_pfnSetExtMsg			= NULL;
	m_pfnGetKData			= NULL;
	m_pfnQueryKData			= NULL;
	m_pfnGetMinData			= NULL;
	m_pfnQueryMinData		= NULL;

	GetAdress();
}

CSTKDRV::~CSTKDRV()
{
	if (m_hSTKDrv)
	{

		m_hSTKDrv = NULL;
	}
}

void CSTKDRV::GetAdress()
{
	if (CTaiShanApp::m_gbUseExe == TRUE)
		return;

	CFileFind flfd;
	CString m_CurrentWorkDirectory;
	GetCurrentDirectory(MAX_PATH, m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	m_CurrentWorkDirectory.ReleaseBuffer();
	m_CurrentWorkDirectory.TrimRight("\\");
	m_CurrentWorkDirectory.TrimRight("/");
	m_CurrentWorkDirectory +="\\";

	if (flfd.FindFile(/*m_CurrentWorkDirectory + "\\System\\Stock.dll"*/"E:\\NetStock\\JStockclt\\Stock.dll"))
	{
		m_hSTKDrv = LoadLibrary(/*m_CurrentWorkDirectory + "\\System\\Stock.dll"*/"E:\\NetStock\\JStockclt\\Stock.dll");
	}
	else
	{
		GetSetReg reg;
		char ch[512];
		DWORD ncb = sizeof(ch);
		DWORD typ = REG_SZ;
		if (reg.GetValue("software\\stockdrv", "stockdrv", typ, (unsigned char *)ch, ncb) == ERROR_SUCCESS)
		{
			ch[511] = 0;
			m_hSTKDrv = LoadLibrary(ch);
		}
		else
		{
			m_hSTKDrv = LoadLibrary("Stock.dll");
		}
	}

	if (!m_hSTKDrv)
	{
		LoadLibrary("Stock.dll");
	}
	if (!m_hSTKDrv)
	{
		return;
	}

	m_pfnGetStockDrvInfo = (DWORD (WINAPI*)(int, void*))GetProcAddress(m_hSTKDrv, "GetStockDrvInfo");
	m_pfnGetTotalNumber = (int (WINAPI*)())GetProcAddress(m_hSTKDrv, "GetTotalNumber");
	m_pfnSetupReceiver = (int (WINAPI*)(BOOL))GetProcAddress(m_hSTKDrv, "SetupReceiver");
	m_pfnStock_Init = (int (WINAPI*)(HWND, UINT, int))GetProcAddress(m_hSTKDrv, "Stock_Init");
	m_pfnStock_Quit = (int (WINAPI*)(HWND))GetProcAddress(m_hSTKDrv, "Stock_Quit");

	m_pfnSetNewsPath = (void (WINAPI*)(LPCTSTR))GetProcAddress(m_hSTKDrv, "SetNewsPath");
	m_pfnSetExtMsg = (int (WINAPI*)(UINT msg))GetProcAddress(m_hSTKDrv, "SetExtMsg");
	m_pfnGetKData = (int (WINAPI*)(LPCSTR scode, time_t mt))GetProcAddress(m_hSTKDrv, "GetKData");
	m_pfnQueryKData = (int (WINAPI*)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv, "QueryKData");
	m_pfnGetMinData = (int (WINAPI*)(LPCSTR scode, time_t mt))GetProcAddress(m_hSTKDrv, "GetMinData");
	m_pfnQueryMinData = (int (WINAPI*)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv, "QueryMinData");
}

int CSTKDRV::Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode)
{
	if(CTaiShanApp::m_gbUseExe ==true)
		return 1;

	if( !m_pfnStock_Init )
		return -1;
	CString m_CurrentWorkDirectory;
	::GetCurrentDirectory(MAX_PATH,m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	m_CurrentWorkDirectory.ReleaseBuffer();
	m_CurrentWorkDirectory.TrimRight ("\\");
	m_CurrentWorkDirectory.TrimRight ("/");
	m_CurrentWorkDirectory+="\\";
	int n = ( (*m_pfnStock_Init)(hWnd,uMsg,nWorkMode)); 
	SetNewsPath(m_CurrentWorkDirectory+"news");
	return n;
}

int CSTKDRV::Stock_Quit(HWND hWnd)
{
	if(CTaiShanApp::m_gbUseExe ==true)
		return 1;
	if( !m_pfnStock_Quit )
		return -1;
	return( (*m_pfnStock_Quit)(hWnd));
}

int CSTKDRV::GetTotalNumber()
{

	if( !m_pfnGetTotalNumber )
		return -1;
	return( (*m_pfnGetTotalNumber)());
}

int CSTKDRV::SetupReceiver(BOOL bSetup)
{
	if(CTaiShanApp::m_gbUseExe ==true)
		return 1;
	if( !m_pfnSetupReceiver )
		return -1;
	return( (*m_pfnSetupReceiver)(bSetup));
}

DWORD CSTKDRV::GetStockDrvInfo(int nInfo, void *pBuf)
{

	if( !m_pfnGetStockDrvInfo ) 
		return -1;
	return( (*m_pfnGetStockDrvInfo)(nInfo,pBuf));
}
void CSTKDRV::SetNewsPath(LPCTSTR lpszNewsPath)
{
	if(CTaiShanApp::m_gbUseExe ==true)
		return ;
	if( !m_pfnSetNewsPath )
		return ;

	try
	{
		(*m_pfnSetNewsPath)(lpszNewsPath);
	}
	catch(...)
	{
	}
}
int CSTKDRV::SetExtMsg(UINT msg)
{
	if( !m_pfnSetExtMsg )
		return 0;
	return( (*m_pfnSetExtMsg)(msg));
}
int CSTKDRV::GetKData(LPCSTR scode, time_t mt)
{
	if( !m_pfnGetKData )
		return 0;
	return( (*m_pfnGetKData)(scode, mt));
}
int CSTKDRV::QueryKData(LPCSTR scode, int nMarket, time_t mt)
{
	if( !m_pfnQueryKData )
		return 0;
	return( (*m_pfnQueryKData)(scode, nMarket, mt));
}
int CSTKDRV::GetMinData(LPCSTR scode, time_t mt)
{
	if( !m_pfnGetMinData )
		return 0;
	return( (*m_pfnGetMinData)(scode, mt));
}
int CSTKDRV::QueryMinData(LPCSTR scode, int nMarket, time_t mt)
{
	if( !m_pfnQueryMinData )
		return 0;
	return( (*m_pfnQueryMinData)(scode, nMarket, mt));
}
