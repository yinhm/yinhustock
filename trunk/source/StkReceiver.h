
#pragma once

#define WM_APP_STKRECEIVER_TSDATA	WM_APP + 5
#define WM_APP_STKRECEIVER_SCDATA	WM_APP + 6

typedef struct tagFoxShareInfo
{
	BYTE b1[8];
	time_t stime;
	DWORD dwFoxPid;
	HWND hReceiverWnd;
	time_t dtime;
	DWORD dwDriverPid;
	HWND hDrvierWnd;
} FOX_SHAREINFO;

class CStkReceiver
{
public:
	CStkReceiver();
	virtual ~CStkReceiver();

public:
	static CStkReceiver* s_pInstance;

protected:
	HWND m_hWnd;
	HWND m_hSendWnd;

	HANDLE m_hMapping;
	LPVOID m_pMapping;

protected:
	BOOL CreateFoxMemoryShare(DWORD dwPid = -1);
	void CloseFoxMemoryShare();
	BOOL CopyProcessInfo();

public:
	HWND CreateReceiverWnd();
	void SetReceiverWnd(HWND hWnd) { m_hWnd = hWnd; };

public:
	BOOL StartEngine();
	BOOL StopEngine();
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CStkReceiver* TSKReceiver();

inline CStkReceiver* TSKReceiver()
{
	if (CStkReceiver::s_pInstance == NULL)
	{
		static CStkReceiver sInstance;
		return &sInstance;
	}

	return CStkReceiver::s_pInstance;
}
