
#pragma once

#define WM_APP_STKRECEIVER_TSDATA	WM_APP + 5
#define WM_APP_STKRECEIVER_SCDATA	WM_APP + 6

typedef struct tagFOX_DATA1
{
	int n1;
	int n2;
	int n3;
} FOX_DATA1;

class CStkDriver;

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

protected:
	CStkDriver* m_pDriver;

public:
	HWND CreateReceiverWnd();
	void SetReceiverWnd(HWND hWnd) { m_hWnd = hWnd; };

public:
	BOOL StartEngine();
	BOOL StopEngine();

public:
	CStkDriver* GetStkDriver();
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
