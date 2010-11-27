
#pragma once

#include "StkDriver.h"

#define FOX_DATAOFFSET 0x0124

enum
{
	foxModeReceive		= 1,
	foxModeDriver		= 2,
	foxModeHub			= 3,
};

enum
{
	FOX_CMD_ACCOUNT		= 301,			// 帐号信息
	FOX_CMD_REPORT		= 302,
	FOX_CMD_FILE		= 303,
	FOX_CMD_CODELIST	= 0x3003,		// 代码表
	FOX_CMD_FINANCE		= 0xA00B,		// 财务资料
};

#pragma pack(1)

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

// 飞狐代码表
typedef struct tagFOX_CODELIST
{
	WORD	m_wMarket;
	char	m_szLabel[STKLABEL_LEN];
	time_t	time;
} FOX_CODELIST;

// 分笔数据结构
typedef union tagFOX_TICK
{
	struct
	{
		time_t	m_time;						// UCT
		float	m_fNewPrice;				// 最新价
		float	m_fVolume;					// 成交量
		float	m_fAmount;					// 成交额
		BYTE	byte[36];					// 未使用
	};
	RCV_EKE_HEADEx m_head;
} FOX_TICK;

#pragma pack()



class CStkReceiver;

class CFoxDriver : public CStkDriver
{
public:
	CFoxDriver();
	virtual ~CFoxDriver();

protected:
	HANDLE m_hMapping;
	LPVOID m_pMapping;

	FOX_SHAREINFO m_foxInfo;
	BOOL m_bHub;
	HWND h1;
	HWND h2;

protected:
	BOOL CreateFoxMemoryShare(DWORD dwPid = -1);
	void CloseFoxMemoryShare();
	BOOL CopyProcessInfo(HWND hWnd, BOOL bReceive = TRUE, BOOL bHub = FALSE);

protected:
	void RequestTick(WORD wMarket, LPTSTR szCode);
	void RequestKline(WORD wMarket, LPTSTR szCode, int nType = 5);

public:
	virtual void Start(HWND hWnd, BOOL bReceive = TRUE, DWORD dwPid = -1, BOOL bHub = false);
	virtual void Stop();
	virtual void RequestData(WORD wMarket, LPTSTR szCode, int nType = 1);
	virtual void ProcessData(HWND hWnd, WPARAM wParam, LPARAM lParam);
};
