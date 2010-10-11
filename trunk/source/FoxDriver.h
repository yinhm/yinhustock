
#pragma once

#include "StkDriver.h"

#define FOX_DATAOFFSET 0x0124

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

class CStkReceiver;

class CFoxDriver : public CStkDriver
{
public:
	CFoxDriver();
	virtual ~CFoxDriver();

protected:
	HANDLE m_hMapping;
	LPVOID m_pMapping;

protected:
	BOOL CreateFoxMemoryShare(DWORD dwPid = -1);
	void CloseFoxMemoryShare();
	BOOL CopyProcessInfo(HWND hWnd);

public:
	virtual void Start(HWND hWnd, BOOL bReceive = TRUE, DWORD dwPid = -1);
	virtual void ProcessData(WPARAM wParam, LPARAM lParam);
};
