
#include "stdafx.h"
#include "StkReceiver.h"
#include "StkDatabase.h"

#include "StkDriver.h"
#include "FoxDriver.h"

CStkReceiver* CStkReceiver::s_pInstance = NULL;

CStkReceiver::CStkReceiver()
{
	m_hWnd = NULL;
	m_hSendWnd = NULL;

	m_pDriver = NULL;
}

CStkReceiver::~CStkReceiver()
{
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
	HWND hWnd = CreateReceiverWnd();
	//CreateFoxMemoryShare();
	//CopyProcessInfo();

	if (m_pDriver != NULL)
	{
		delete m_pDriver;
		m_pDriver = NULL;
	}

	m_pDriver = new CFoxDriver;
	if (!m_pDriver) return FALSE;

	m_pDriver->Start(hWnd);

	//if (m_pMapping)
	{
		//CFile file;
		//file.Open("E:\\Stock.dat", CFile::modeCreate | CFile::modeReadWrite);
		//file.Write(m_pMapping, 0x1000);
		//file.Close();
	}

	//FOX_SHAREINFO* pSi = (FOX_SHAREINFO*)m_pMapping;
	//int* n = new int;
	//*n = 1;
	//COPYDATASTRUCT data;
	//data.cbData = 4;
	//data.dwData = 1;
	//data.lpData = n;

	//::SendMessage(pSi->hReceiverWnd, WM_COPYDATA, 1, (LPARAM)&data);

	//delete n;

	return TRUE;
}

BOOL CStkReceiver::StopEngine()
{
	//int n = 3;
	//COPYDATASTRUCT data;
	//data.cbData = 4;
	//data.dwData = 1;
	//data.lpData = &n;

	//FOX_SHAREINFO* pSi = (FOX_SHAREINFO*)m_pMapping;
	//BOOL bRet = ::SendMessage(pSi->hReceiverWnd, WM_COPYDATA, 1, (LPARAM)&data);

	return TRUE;
}

CStkDriver* CStkReceiver::GetStkDriver()
{
	if (m_pDriver == NULL)
	{
		m_pDriver = new CFoxDriver;
	}

	return m_pDriver;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_APP_STKRECEIVER_TSDATA)
	{
		//TSKReceiver()->OnStkReceiverTsdata(wParam, lParam);
	}
	else if (message == WM_COPYDATA)
	{
		TSKReceiver()->GetStkDriver()->ProcessData(wParam, lParam);

		return 0;
	}
	else
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
