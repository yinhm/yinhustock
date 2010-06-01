
#include "grid.h"
#if !defined(AFX_WSSTOCK2000_H__61A31CE9_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_WSSTOCK2000_H__61A31CE9_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"  


//#define WM_USER_CUOHE (WM_USER + 1)
//#define WM_USER_CLEAR (WM_USER + 2)
//#define WM_USER_CUOHEDIALOG (WM_USER + 3)
#define WM_USER_HOTKEYPROGRESS (WM_USER + 4)
#define WM_USER_HSCROLL (WM_USER + 5)
#define WM_USER_ALERT   (WM_USER + 6)
#define WM_USER_TJXG   (WM_USER + 7)
#define WM_USER_CHANGECOLOR  (WM_USER+8)
#define WM_USER_CHANGEPOS    (WM_USER+9)
#define WM_USER_CHANGESTOCK  (WM_USER+10)
#define WM_USER_FROM_KLINE	WM_USER+100
#define WM_USER_TELL_CHANGE	WM_USER+101
#define WM_USER_DRAGDROP	WM_USER+102
#define WM_USER_COLUMNSELECT WM_USER+11
#define WM_USER_JISHUCOLUMNSELECT WM_USER+12
#define WM_USER_NINESHOW_ROW_CHANGED WM_USER+13
#define WM_USER_ADDSTOCK WM_USER+1200
#define WM_USER_ADDDAYLINE WM_USER+1201

#define WM_USER_RECVSTOCKCODE WM_USER+1202
#define WM_USER_TRANSMITINFO  WM_USER+1203

#define WM_USERKEYDOWN WM_USER+200
#define WM_KEYBOARDGENIUS  WM_USER+201 
#define WM_REFRESHNINESHOW WM_USER+202 
#define WM_CHANGEALLCOLOR  WM_USER+203 
#define WM_USER_DrawKlineWindow  WM_USER+301 

#define BACKGROUNDCOLOR  RGB(63,63,63)

void DoHtmlHelp(CWnd* pWnd,int nID = 0);

class CLongString
{
public:
	static void FloatArrayToExcelString(CLongString & longStr,float* pFloat,int nTot,int nBegin);
	static CString FloatToString(float fVal);
	int GetLength();
	int m_nMax;
	int m_nCurrentNum;
	int m_nAdd;
	char* m_pData;
	void operator+(CString s);
	CLongString();
	virtual ~CLongString();
};

#define FILE_PATH_HSH_SH "data\\historysh\\"
#define FILE_PATH_HSH_SZ "data\\historysz\\"


class CTaiShanApp : public CWinApp
{
public:


	virtual void SwitchToReportView( LPCSTR lpszLabel, WORD wMarket);


	virtual void SwitchToKlineView( LPCSTR lpszLabel, WORD wMarket);


	virtual void SwitchToF10View( LPCSTR lpszLabel, WORD wMarket);

	virtual void  OnKeyboardGeniusCmd( LPCSTR lpszCmd );

public:
	static void DispatchMessageEachTime();

	CTaiShanApp();
	HANDLE hAppMutex;
	CMultiDocTemplate* pDocTemplate;

	CMultiDocTemplate* pDrawTemplate;
	CMultiDocTemplate *m_pMyNewTemplate;

	CMultiDocTemplate* m_pBroadcastDocTemplate;

	CMultiDocTemplate* pNineShowTemplate;
	int Line_mode;
	int m_key;
	CTime t;
	BOOL m_bAppAuthorized;

	//{{AFX_VIRTUAL(CTaiShanApp)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual int Run();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTaiShanApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFilePrintSetup();
	afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WSSTOCK2000_H__61A31CE9_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
