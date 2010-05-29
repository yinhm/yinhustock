
#include "stdafx.h"
#include "StkReceiver.h"

CStkReceiver* CStkReceiver::s_pInstance = NULL;

CStkReceiver::CStkReceiver()
{
	m_hWnd = NULL;
	m_hSendWnd = NULL;

	m_hMapping = NULL;
	m_pMapping = NULL;
}

CStkReceiver::~CStkReceiver()
{
}

BOOL CStkReceiver::CreateFoxMemoryShare(DWORD dwPid)
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

	CHAR szShareName[64];
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

void CStkReceiver::CloseFoxMemoryShare()
{
	if (m_pMapping)
	{
		UnmapViewOfFile(m_pMapping);
	}

	if (m_hMapping)
	{
		CloseHandle(m_hMapping);
	}
}

BOOL CStkReceiver::CopyProcessInfo()
{
	if (m_pMapping == NULL)
	{
		return FALSE;
	}

	if (m_hSendWnd == NULL)
	{
		m_hSendWnd = m_hWnd;
	}

	//FOX_SHAREINFO foxSi;
	//memset(&foxSi, 0, sizeof(foxSi));
	//foxSi.stime = time(&foxSi.stime);
	//foxSi.dwFoxPid = GetCurrentProcessId();
	//foxSi.hReceiverWnd = m_hSendWnd;

	//memcpy(m_pMapping, (void*)&foxSi, sizeof(foxSi));

	FOX_SHAREINFO* pSi = (FOX_SHAREINFO*)m_pMapping;
	pSi->dtime = time(&pSi->dtime);
	pSi->dwDriverPid = GetCurrentProcessId();
	pSi->hDrvierWnd = m_hWnd;

	return TRUE;
}

HWND CStkReceiver::CreateReceiverWnd()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= /*hInstance*/NULL;
	wcex.hIcon			= /*LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32))*/NULL;
	wcex.hCursor		= /*LoadCursor(NULL, IDC_ARROW)*/NULL;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= /*MAKEINTRESOURCE(IDC_WIN32)*/NULL;
	wcex.lpszClassName	= /*szWindowClass*/_T("CStkReceiverWnd");
	wcex.hIconSm		= /*LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL))*/NULL;

	RegisterClassEx(&wcex);

	HWND hWnd;
	hWnd = CreateWindow(_T("CStkReceiverWnd"), _T(""), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, /*hInstance*/NULL, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	m_hWnd = hWnd;

	ShowWindow(hWnd, /*nCmdShow*/SW_HIDE);
	UpdateWindow(hWnd);

	return hWnd;
}

BOOL CStkReceiver::StartEngine()
{
	CreateReceiverWnd();
	CreateFoxMemoryShare(3552);
	CopyProcessInfo();

	if (m_pMapping)
	{
		//CFile file;
		//file.Open("E:\\Stock.dat", CFile::modeCreate | CFile::modeReadWrite);
		//file.Write(m_pMapping, 0x1000);
		//file.Close();
	}

	FOX_SHAREINFO* pSi = (FOX_SHAREINFO*)m_pMapping;
	int* n = new int;
	*n = 1;
	COPYDATASTRUCT data;
	data.cbData = 4;
	data.dwData = 1;
	data.lpData = n;

	::SendMessage(pSi->hReceiverWnd, WM_COPYDATA, 1, (LPARAM)&data);

	delete n;

	return TRUE;
}

BOOL CStkReceiver::StopEngine()
{
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_APP_STKRECEIVER_TSDATA)
	{
		//TSKReceiver()->OnStkReceiverTsdata(wParam, lParam);
	}
	else if (message == WM_COPYDATA)
	{
		COPYDATASTRUCT* pData = (COPYDATASTRUCT*)lParam;

		CFile file;
		char filename[64];
		time_t t;
		sprintf(filename, "e:\\%d.dat", time(&t));
		file.Open(filename, CFile::modeCreate | CFile::modeReadWrite);
		file.Write(pData->lpData, pData->cbData);
		file.Close();

		return 1;
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
