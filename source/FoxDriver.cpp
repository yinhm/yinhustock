
#include "stdafx.h"
#include "FoxDriver.h"

#include "StkReceiver.h"
#include "StkDatabase.h"

CFoxDriver::CFoxDriver()
{
	m_hMapping = NULL;
	m_pMapping = NULL;
}

CFoxDriver::~CFoxDriver()
{
	CloseFoxMemoryShare();
}

BOOL CFoxDriver::CreateFoxMemoryShare(DWORD dwPid)
{
	if (m_pMapping)
	{
		UnmapViewOfFile(m_pMapping);
	}

	if (m_hMapping)
	{
		CloseHandle(m_hMapping);
	}

	DWORD dwProcessId = GetCurrentProcessId();
	if (dwPid != -1)
	{
		dwProcessId = dwPid;
	}

	char szShareName[64];
	sprintf(szShareName, _T("PatiosoftShareMem%d"), dwProcessId);

	m_hMapping = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 0x1000, szShareName);
	if (m_hMapping == NULL)
	{
		return FALSE;
	}

	m_pMapping = MapViewOfFile(m_hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (m_pMapping == NULL)
	{
		return FALSE;
	}

	return TRUE;
}

void CFoxDriver::CloseFoxMemoryShare()
{
	if (m_pMapping)
	{
		UnmapViewOfFile(m_pMapping);
		m_pMapping = NULL;
	}

	if (m_hMapping)
	{
		CloseHandle(m_hMapping);
		m_hMapping = NULL;
	}
}

BOOL CFoxDriver::CopyProcessInfo(HWND hWnd)
{
	if (m_pMapping == NULL)
	{
		return FALSE;
	}

	//if (hWnd != NULL)
	//{
	//	m_hSendWnd = hWnd;
	//}

	FOX_SHAREINFO foxSi;
	memset(&foxSi, 0, sizeof(foxSi));

	foxSi.stime = time(&foxSi.stime);
	foxSi.dwFoxPid = GetCurrentProcessId();
	foxSi.hReceiverWnd = hWnd;

	memcpy(m_pMapping, (void*)&foxSi, sizeof(foxSi));

	//FOX_SHAREINFO* pSi = (FOX_SHAREINFO*)m_pMapping;
	//pSi->dtime = time(&pSi->dtime);
	//pSi->dwDriverPid = GetCurrentProcessId();
	//pSi->hDrvierWnd = m_hWnd;

	return TRUE;
}

void CFoxDriver::Start(HWND hWnd, BOOL bReceive, DWORD dwPid)
{
	if (CreateFoxMemoryShare(dwPid))
	{
		CopyProcessInfo(hWnd);
	}
}

void CFoxDriver::ProcessData(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pData = (COPYDATASTRUCT*)lParam;

	FOX_DATA* pFoxHeader = (FOX_DATA*)pData->lpData;
	int nCount = pFoxHeader->m_nCount;
	BYTE* pBuffBase = (BYTE*)pData->lpData + FOX_DATAOFFSET;

	if (pFoxHeader->m_dwFlag == 1)
	{
		BYTE* pBuffer = new BYTE[1024];
		memset(pBuffer, 0, 1024);

		int nPos = 0;

		FOX_DATA f0;
		f0.m_dwFlag = 0x65;
		f0.m_dwType = 0xB4;
		f0.m_nCount = 2;

		memcpy(pBuffer, &f0, sizeof(f0));
		nPos += sizeof(f0);

		char szText[1024];

		memset(&szText[0], 0, sizeof(szText));
		sprintf(&szText[0], _T("www.tsking.cn"));
		memcpy(pBuffer + nPos, &szText[0], 20);
		nPos += 20;

		memset(&szText[0], 0, sizeof(szText));
		sprintf(&szText[0], _T("策略为王"));
		memcpy(pBuffer + nPos, &szText[0], 20);
		nPos += 20;

		memset(&szText[0], 0, sizeof(szText));
		sprintf(&szText[0], _T("71120"));
		memcpy(pBuffer + nPos, &szText[0], 40);
		nPos += 40;

		memset(&szText[0], 0, sizeof(szText));
		sprintf(&szText[0], _T("飞狐交易师"));
		memcpy(pBuffer + nPos, &szText[0], 92);
		nPos += 92;

		COPYDATASTRUCT data;
		data.cbData = nPos;
		data.dwData = 1;
		data.lpData = pBuffer;

		BOOL bRet;

		//HWND hwnd = TSKReceiver()->GetReceiverHwnd();
		//if (::IsWindow(hwnd))
		//{
		//	bRet = ::SendMessage(hwnd, WM_COPYDATA, 1, (LPARAM)&data);
		//}

		DWORD dwErr = ::GetLastError();

		FOX_DATA f1;
		f1.m_dwFlag = 0x66;
		f1.m_dwType = 0x23F;
		f1.m_nCount = 2;

		nPos = 0;

		int nMarket = 0;
		nPos = 283 * nMarket;

		sprintf(&szText[nPos], _T("SH"));
		nPos += 2;
		szText[nPos] = 0;

		nPos++;
		sprintf(&szText[nPos], _T("E:\\软件开发\\股票软件\\yinhustock\\temp\\Data\\Sh\\F10"));
		nPos += strlen("E:\\软件开发\\股票软件\\yinhustock\\temp\\Data\\Sh\\F10");
		szText[nPos] = 0;

		nMarket = 1;
		nPos = 283 * nMarket;

		sprintf(&szText[nPos], _T("SZ"));
		nPos += 2;
		szText[nPos] = 0;

		nPos++;
		sprintf(&szText[nPos], _T("E:\\软件开发\\股票软件\\yinhustock\\temp\\Data\\Sz\\F10"));
		nPos += strlen("E:\\软件开发\\股票软件\\yinhustock\\temp\\Data\\Sz\\F10");
		szText[nPos] = 0;

		nMarket = 2;
		nPos = 283 * nMarket;
		szText[nPos] = 0;

		nPos++;

		memcpy(pBuffer, &f1, sizeof(f1));
		memcpy(pBuffer + sizeof(f1), &szText[0], nPos);

		data.cbData = sizeof(f1) + nPos;
		data.dwData = 1;
		data.lpData = pBuffer;

		//if (::IsWindow(hwnd))
		//{
		//	bRet = ::SendMessage(hwnd, WM_COPYDATA, 1, (LPARAM)&data);
		//}
		dwErr = ::GetLastError();

		FOX_DATA f3;
		f3.m_dwFlag = 0x67;
		f3.m_dwType = 0x210;
		f3.m_nCount = 2;


		memcpy(pBuffer, &f3, sizeof(f3));

		nPos = 0;

		sprintf(szText, "E:\\软件开发\\股票软件\\yinhustock\\temp\\StkInfo");
		nPos += strlen("E:\\软件开发\\股票软件\\yinhustock\\temp\\StkInfo");
		szText[nPos] = 0;

		nPos = 260;

		sprintf(&szText[nPos], "E:\\软件开发\\股票软件\\yinhustock\\temp\\News");
		nPos += strlen("E:\\软件开发\\股票软件\\yinhustock\\temp\\News");
		szText[nPos] = 0;

		nPos = 520;

		memcpy(pBuffer + sizeof(f3), szText, 520);

		data.cbData = sizeof(f3) + 520;
		data.lpData = &pBuffer;

		//if (::IsWindow(hwnd))
		//{
		//	bRet = ::SendMessage(hwnd, WM_COPYDATA, 1, (LPARAM)&data);
		//}
		dwErr = ::GetLastError();


		CFile file;
		char filename[64];
		time_t t;
		sprintf(filename, "e:\\%d.dat", time(&t));
		file.Open(filename, CFile::modeCreate | CFile::modeReadWrite);
		file.Write(pBuffer, sizeof(f1) + nPos);
		file.Close();

		int n = 2;
		data.cbData = 1;
		data.lpData = &n;

		//if (::IsWindow(hwnd))
		//{
		//	bRet = ::SendMessage(hwnd, WM_COPYDATA, 1, (LPARAM)&data);
		//}
		dwErr = ::GetLastError();

		delete pBuffer;
	}
	else if (pFoxHeader->m_dwType == RCV_REPORT)
	{
		RCV_REPORT_STRUCTEx* pReport = (RCV_REPORT_STRUCTEx*)pBuffBase;
		TSKDatabase()->ProcessReport(pReport, nCount);
	}
	else if (pFoxHeader->m_dwType == FILE_HISTORY_EX)
	{
		RCV_HISTORY_STRUCTEx* pHistory = (RCV_HISTORY_STRUCTEx*)pBuffBase;
		TSKDatabase()->ProcessHistory(pHistory, nCount);
	}
	else
	{
		CFile file;
		char filename[64];
		time_t t;
		sprintf(filename, "D:\\StockData\\foxdata - %d.dat", time(&t));
		file.Open(filename, CFile::modeCreate | CFile::modeReadWrite);
		file.Write(pData->lpData, pData->cbData);
		file.Close();
	}
}
