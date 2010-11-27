
#include "stdafx.h"
#include "FoxDriver.h"

#include "StkReceiver.h"
#include "StkDatabase.h"

CFoxDriver::CFoxDriver()
{
	m_hMapping = NULL;
	m_pMapping = NULL;

	m_bHub = FALSE;
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

	m_hMapping = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1000, szShareName);
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

BOOL CFoxDriver::CopyProcessInfo(HWND hWnd, BOOL bReceive, BOOL bHub)
{
	if (m_pMapping == NULL)
	{
		return FALSE;
	}

	if (bReceive)
	{
		FOX_SHAREINFO foxSi;
		memset(&foxSi, 0, sizeof(foxSi));

		foxSi.stime = time(&foxSi.stime);
		foxSi.dwFoxPid = GetCurrentProcessId();
		foxSi.hReceiverWnd = hWnd;

		memcpy(m_pMapping, (void*)&foxSi, sizeof(foxSi));
	}
	else
	{
		FOX_SHAREINFO* pSi = (FOX_SHAREINFO*)m_pMapping;

		pSi->dtime = time(&pSi->dtime);
		pSi->dwDriverPid = ::GetCurrentProcessId();
		pSi->hDrvierWnd = hWnd;

		memcpy(&m_foxInfo, m_pMapping, sizeof(FOX_SHAREINFO));

		if (!bHub)
		{
			COPYDATASTRUCT* pData = new COPYDATASTRUCT;
			int data = 1;
			pData->cbData = sizeof(int);
			pData->lpData = &data;
			pData->dwData = 1;
			SendMessage(pSi->hReceiverWnd, WM_COPYDATA, (WPARAM)1, (LPARAM)pData);
		}
	}

	return TRUE;
}

void CFoxDriver::RequestTick(WORD wMarket, LPTSTR szCode)
{
	char nBuffer[360];
	memset(nBuffer, 0, sizeof(nBuffer));

	FOX_DATA* pData = (FOX_DATA*)nBuffer;
	pData->m_dwFlag = 0x00CA;
	pData->m_dwType = 1;

	int nData = 0x0100;
	memcpy(nBuffer + 0x0C, &nData, sizeof(nData));
	nData = 0x0010CD35;
	memcpy(nBuffer + 0x10, &nData, sizeof(nData));
	nData = 0x00000001;
	memcpy(nBuffer + 0x18, &nData, sizeof(nData));
	nData = 0x00000001;
	memset(nBuffer + 0x1C, 0xFF, 0x18);
	memcpy(nBuffer + 0x34, &nData, sizeof(nData));
	memcpy(nBuffer + 0x38, &wMarket, sizeof(WORD));
	strcpy(nBuffer + 0x3A, szCode);
	//nData = 0x02DD8848;
	//memcpy(nBuffer + 0x0164, &nData, sizeof(nData));

	COPYDATASTRUCT cds;
	memset(&cds, 0, sizeof(cds));
	cds.dwData = 0;
	cds.cbData = sizeof(nBuffer);
	cds.lpData = nBuffer;

	CFile file;
	if (file.Open(_T("D:\\StockData\\Fox_Request_Tick"), CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(nBuffer, sizeof(nBuffer));
		file.Close();
	}

	if (IsWindow(m_foxInfo.hDrvierWnd))
	{
		SendMessage(m_foxInfo.hDrvierWnd, WM_COPYDATA, 0, (LPARAM)&cds);
	}
}

void CFoxDriver::RequestKline(WORD wMarket, LPTSTR szCode, int nType)
{
	char nBuffer[56];
	memset(nBuffer, 0, sizeof(nBuffer));

	FOX_DATA* pData = (FOX_DATA*)nBuffer;
	pData->m_dwFlag = 0x00CA;

	DWORD dwType;
	if (nType == 1)
	{
		dwType = 0x0A;		// 1 分钟
	}
	else if (nType == 2)
	{
		dwType = 0x03;		// 5 分钟
	}
	else if (nType == 5)
	{
		dwType = 0x04;		// 日线
	}
	else
	{
		return;
	}

	pData->m_dwType = dwType;

	int nData = 0x0200;
	memcpy(nBuffer + 0x0C, &nData, sizeof(nData));
	memcpy(nBuffer + 0x10, &wMarket, sizeof(WORD));
	strcpy(nBuffer + 0x12, szCode);

	COPYDATASTRUCT cds;
	memset(&cds, 0, sizeof(cds));
	cds.dwData = 0;
	cds.cbData = sizeof(nBuffer);
	cds.lpData = nBuffer;

	CFile file;
	if (file.Open(_T("D:\\StockData\\Fox_Request_Kline"), CFile::modeCreate | CFile::modeWrite))
	{
		file.Write(nBuffer, sizeof(nBuffer));
		file.Close();
	}

	if (IsWindow(m_foxInfo.hDrvierWnd))
	{
		SendMessage(m_foxInfo.hDrvierWnd, WM_COPYDATA, 0, (LPARAM)&cds);
	}
}

void CFoxDriver::Start(HWND hWnd, BOOL bReceive, DWORD dwPid, BOOL bHub)
{
	if (CreateFoxMemoryShare(dwPid))
	{
		CopyProcessInfo(hWnd, bReceive, bHub);
		h1 = hWnd;

		if (bReceive && !bHub)
		{
			Sleep(1000);
			CString str;
			str.Format(_T("%d"), ::GetCurrentProcessId());
			ShellExecute(NULL, NULL, _T("System\\ftstkdrv.exe"), str, NULL, SW_SHOWNORMAL);
		}
		else if (bHub)
		{
			HWND hWnd = TSKReceiver()->CreateReceiverWnd();
			CreateFoxMemoryShare();
			CopyProcessInfo(hWnd);
			h2 = hWnd;

			Sleep(1000);
			CString str;
			str.Format(_T("%d"), ::GetCurrentProcessId());
			ShellExecute(NULL, NULL, _T("D:\\证券股票\\千钧茶舍专用\\System\\ftstkdrv.exe"), str, NULL, SW_SHOWNORMAL);
		}

		if (!IsWindow(m_foxInfo.hReceiverWnd))
		{
			bHub = FALSE;
			bReceive = TRUE;
		}

		m_bHub = bHub;
	}
}

void CFoxDriver::Stop()
{
	if (m_pMapping == NULL)
		return;

	HWND hwnd = ((FOX_SHAREINFO*)m_pMapping)->hDrvierWnd;

	int nData = 3;

	COPYDATASTRUCT data;
	data.cbData = 4;
	data.dwData = 0;
	data.lpData = &nData;

	if (::IsWindow(hwnd))
	{
		::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
	}
}

void CFoxDriver::RequestData(WORD wMarket, LPTSTR szCode, int nType)
{
	if (!::IsWindow(m_foxInfo.hDrvierWnd))
		return;

	switch (nType)
	{
	case 0:
		RequestTick(wMarket, szCode);
		break;
	case 1:
	case 2:
	case 5:
		RequestTick(wMarket, szCode);
		RequestKline(wMarket, szCode, nType);
		break;
	default:
		break;
	}
}

void CFoxDriver::ProcessData(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pData = (COPYDATASTRUCT*)lParam;

	FOX_DATA* pFoxHeader = (FOX_DATA*)pData->lpData;
	int nCount = pFoxHeader->m_nCount;
	BYTE* pBuffBase = (BYTE*)pData->lpData + FOX_DATAOFFSET;


	CFile fileLog;

	static UINT nNum = 0;
	time_t t;
	char filename[256];
	sprintf(filename, "D:\\StockData\\FoxData\\%d - %04d - %04x - %04x - %d", time(&t), nNum++, pFoxHeader->m_dwFlag, pFoxHeader->m_dwType, pData->cbData);

	BOOL bPacketLog = TRUE;


	// 接收程序已经启动
	if (pFoxHeader->m_dwFlag == 1 && !m_bHub)
	{
		CFile file;
		int lenght;
		BYTE* pBuffer = NULL;

		COPYDATASTRUCT data;
		data.dwData = 0;

		// 保存接收程序信息
		FOX_SHAREINFO* pInfo = (FOX_SHAREINFO*)m_pMapping;
		m_foxInfo.dtime = pInfo->dtime;
		m_foxInfo.dwDriverPid = pInfo->dwDriverPid;
		m_foxInfo.hDrvierWnd = pInfo->hDrvierWnd;

		HWND hwnd = m_foxInfo.hDrvierWnd;



		//
		FOX_DATA* pFoxHeader;

		lenght = 184;
		pBuffer = new BYTE[lenght];
		memset(pBuffer, 0, lenght);

		pFoxHeader = (FOX_DATA*)pBuffer;
		pFoxHeader->m_dwFlag = 0x65;
		pFoxHeader->m_dwType = 0x80;
		pFoxHeader->m_nCount = 2;

		CString strName;
		strName.LoadString(IDR_MAINFRAME);
		strcpy((char*)pBuffer + 0x5C, strName.GetBuffer(0));

		data.cbData = lenght;
		data.lpData = pBuffer;

		if (::IsWindow(hwnd))
		{
			::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		}
		delete pBuffer;		//*/

		//file.Open(_T("D:\\StockData\\fox - 1289045593 - 0065 - 00b4 - 184.dat"), CFile::modeRead);
		//lenght = file.GetLength();
		//pBuffer = new BYTE[lenght];
		//file.Read(pBuffer, lenght);
		//file.Close();

		//data.cbData = lenght;
		//data.lpData = pBuffer;

		//if (::IsWindow(hwnd))
		//{
		//	::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		//}
		//delete pBuffer;



		//file.Open(_T("D:\\StockData\\fox - 1289045599 - 0066 - 023f - 579.dat"), CFile::modeRead);
		//lenght = file.GetLength();
		//pBuffer = new BYTE[lenght];
		//file.Read(pBuffer, lenght);
		//file.Close();

		//data.cbData = lenght;
		//data.lpData = pBuffer;

		//if (::IsWindow(hwnd))
		//{
		//	::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		//}
		//delete pBuffer;



		//file.Open(_T("D:\\StockData\\fox - 1289045603 - 0067 - 0210 - 532.dat"), CFile::modeRead);
		//lenght = file.GetLength();
		//pBuffer = new BYTE[lenght];
		//file.Read(pBuffer, lenght);
		//file.Close();

		//data.cbData = lenght;
		//data.lpData = pBuffer;

		//if (::IsWindow(hwnd))
		//{
		//	::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		//}
		//delete pBuffer;



		int nData = 2;
		data.cbData = 4;
		data.lpData = &nData;

		if (::IsWindow(hwnd))
		{
			::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		}

		bPacketLog = FALSE;
	}
	else if (pFoxHeader->m_dwFlag == 2 && !m_bHub)
	{
		CFile file;
		int len;
		char* buffer;

		COPYDATASTRUCT data;
		data.dwData = 0;
		HWND hwnd = ((FOX_SHAREINFO*)m_pMapping)->hReceiverWnd;

		//file.Open("D:\\StockData\\foxdata - 1 - 1288194121 - 56.dat", CFile::modeRead);
		//len = file.GetLength();
		//buffer = new char[len];
		//file.Read(buffer, len);
		//file.Close();

		//data.cbData = file.GetLength();
		//data.lpData = buffer;
		//if (::IsWindow(hwnd))
		//{
		//	::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		//}
		//delete buffer;

		//file.Open("D:\\StockData\\foxdata - 1 - 1288194123 - 569506.dat", CFile::modeRead);
		//len = file.GetLength();
		//buffer = new char[len];
		//file.Read(buffer, len);
		//file.Close();

		//data.cbData = file.GetLength();
		//data.lpData = buffer;
		//if (::IsWindow(hwnd))
		//{
		//	::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		//}
		//delete buffer;

		//file.Open("D:\\StockData\\foxdata - 1 - 1288194244 - 288.dat", CFile::modeRead);
		//len = file.GetLength();
		//buffer = new char[len];
		//file.Read(buffer, len);
		//file.Close();

		//data.cbData = file.GetLength();
		//data.lpData = buffer;
		//if (::IsWindow(hwnd))
		//{
		//	::SendMessage(hwnd, WM_COPYDATA, 0, (LPARAM)&data);
		//}
		//delete buffer;

		bPacketLog = FALSE;
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_ACCOUNT)
	{
		bPacketLog = TRUE;
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_FILE && pFoxHeader->m_dwType == FOX_CMD_CODELIST)	// 代码表
	{
		FOX_CODELIST* pList = (FOX_CODELIST*)pBuffBase;

		bPacketLog = FALSE;
		TRACE3(_T("CodeList: %d %s %d\r\n"), pList->m_wMarket, pList->m_szLabel, nCount);

		CString str;
		str.Format(_T("%s.1"), filename);
		ofstream ofs(str.GetBuffer(0), ios::out | ios::binary);
		for (int i = 0; i < nCount; i++)
		{
			str.Format("%s %d\r\n", (char*)&pList[i].m_wMarket, pList[i].time);
			ofs.write(str.GetBuffer(0), str.GetLength());
		}
		ofs.close();
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_REPORT && pFoxHeader->m_dwType == RCV_REPORT)
	{
		RCV_REPORT_STRUCTEx* pReport = (RCV_REPORT_STRUCTEx*)pBuffBase;
		TSKDatabase()->ProcessReport(pReport, nCount);

		bPacketLog = FALSE;
		TRACE3(_T("Report: %d %s %d\r\n"), pReport->m_wMarket, pReport->m_szLabel, nCount);
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_FILE && pFoxHeader->m_dwType == FILE_HISTORY_EX)
	{
		RCV_HISTORY_STRUCTEx* pHistory = (RCV_HISTORY_STRUCTEx*)pBuffBase;
		TSKDatabase()->ProcessHistory(pHistory, nCount);

		bPacketLog = FALSE;
		TRACE3(_T("History: %d %s %d\r\n"), pHistory->m_head.m_wMarket, pHistory->m_head.m_szLabel, nCount);
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_FILE && pFoxHeader->m_dwType == FILE_POWER_EX)
	{
		RCV_POWER_STRUCTEx* pPower = (RCV_POWER_STRUCTEx*)pBuffBase;
		TSKDatabase()->ProcessPower(pPower, nCount);

		bPacketLog = FALSE;
		TRACE3(_T("Power: %d %s %d\r\n"), pPower->m_head.m_wMarket, pPower->m_head.m_szLabel, nCount);
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_FILE && pFoxHeader->m_dwType == 83)
	{
		FOX_TICK* pTick = (FOX_TICK*)pBuffBase;
		TSKDatabase()->ProcessTick(pTick, nCount);

		bPacketLog = FALSE;
		TRACE3(_T("Tick: %d %s %d\r\n"), pTick->m_head.m_wMarket, pTick->m_head.m_szLabel, nCount);

		//CString str;
		//str.Format(_T("%s.1"), filename);
		//ofstream ofs(str.GetBuffer(0), ios::out | ios::binary);
		//for (int i = 0; i < nCount; i++)
		//{
		//	if (pTick[i].m_head.m_dwHeadTag == 0xFFFFFFFF)
		//	{
		//		ofs.write(pTick->m_head.m_szLabel, strlen(pTick->m_head.m_szLabel));
		//		ofs.write("\r\n", 2);
		//		continue;
		//	}

		//	tm* t;
		//	t = localtime(&pTick[i].m_time);
		//	char strtime[64];
		//	strftime(strtime, 64, "%Y-%m-%d %H:%M:%S", t);

		//	str.Format("%s\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\r\n", strtime, pTick[i].m_fNewPrice, pTick[i].m_fVolume, pTick[i].m_fVolume, pTick[i].byte[0], pTick[i].byte[1]);
		//	ofs.write(str.GetBuffer(0), str.GetLength());
		//}
		//ofs.close();
	}
	else if (pFoxHeader->m_dwFlag == FOX_CMD_FILE && pFoxHeader->m_dwType == FOX_CMD_FINANCE)
	{
		bPacketLog = FALSE;
		//TRACE3(_T("Power: %d %s %d\r\n"), pPower->m_head.m_wMarket, pPower->m_head.m_szLabel, nCount);

		//typedef struct
		//{
		//	time_t time;
		//	float f[56];
		//} ccc;
		//ccc* c = (ccc*)pBuffBase;

		//CString str;
		//str.Format(_T("%s.1"), filename);
		//ofstream ofs(str.GetBuffer(0), ios::out | ios::binary);
		//for (int j = 0; j < nCount; j++)
		//{
		//	if (c->time == 0xFFFFFFFF)
		//	{
		//		ofs.write(((char*)c)+6, 10);
		//		c++;
		//		continue;
		//	}

		//	ofs.write("\r\n", 5);

		//	for (int i = 0; i < 56; i++)
		//	{
		//		char mmm[200];
		//		sprintf(mmm, "%f\r\n", c->f[i]);
		//		ofs.write((char*)mmm, strlen(mmm));
		//	}

		//	ofs.write("---\r\n", 5);

		//	c++;
		//}
		//ofs.close();
	}
	else
	{
		bPacketLog = TRUE;

		TRACE3(_T("unkow: %04x %04x %d\r\n"), pFoxHeader->m_dwFlag, pFoxHeader->m_dwType, pFoxHeader->m_nCount);
	}

	if (m_bHub)
	{
		FOX_SHAREINFO* pInfo = (FOX_SHAREINFO*)m_pMapping;

		// 接收程序启动
		if (pFoxHeader->m_dwFlag == 1)
		{
			// 保存接收程序信息
			m_foxInfo.dtime = pInfo->dtime;
			m_foxInfo.dwDriverPid = pInfo->dwDriverPid;
			m_foxInfo.hDrvierWnd = pInfo->hDrvierWnd;
		}

		// 接收程序发来信息，转发到股票软件
		if (hWnd == h2)
		{
			if (::IsWindow(m_foxInfo.hReceiverWnd))
			{
				::SendMessage(m_foxInfo.hReceiverWnd, WM_COPYDATA, 0, lParam);
			}
		}

		// 股票软件发来信息，转发到接收程序
		if (hWnd == h1)
		{
			if (::IsWindow(m_foxInfo.hDrvierWnd))
			{
				::SendMessage(m_foxInfo.hDrvierWnd, WM_COPYDATA, 0, lParam);
			}
		}
	}

	if (bPacketLog)
	{
		CString str;
		str.Format(_T("%s.0"), filename);
		fileLog.Open(str, CFile::modeCreate | CFile::modeReadWrite);
		fileLog.Write(pData->lpData, pData->cbData);
		fileLog.Close();
	}
}
