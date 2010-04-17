// FtpGetSend.h: interface for the FtpGetSend class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPGETSEND_H__2A5CD434_E987_4FAA_BF9F_9433D6842070__INCLUDED_)
#define AFX_FTPGETSEND_H__2A5CD434_E987_4FAA_BF9F_9433D6842070__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxinet.h>
#define WM_SETDIRECTORYDISPLAY WM_USER+1212
#define WM_SETFILE				 WM_USER+1213
#define WM_SETREDRAWFLAG		 WM_USER+1214
#define WM_RECORDFTPINFO		 WM_USER+1215
#define WM_REDISPLAYFILE		 WM_USER+1216
#define WM_SETDIR				 WM_USER+1217
#include "FtpParaClass.h"

class AFX_EXT_CLASS FtpGetSend : public CWinThread
{
DECLARE_DYNCREATE(FtpGetSend)
protected:
FtpGetSend();


public:
	static BOOL IsFree;
	static CWnd* pFtpInfoView;
	static CWnd* pLocView;
	static CWnd* pServView;
	static char szAppName[256];
	static char szFtpName[256];
	static char szFtpUser[20];
	static char szFtpPassword[20];
	static char szFtpDirectory[MAX_PATH];
	static char szLocDirectory[MAX_PATH];
	static int nPort;
	static int nCount;

	enum
	{
	FILEICON,
	DIRECTORYICON,
	LOCFILE,
	SERVFILE,
	};


public:
	static CString GetFileNameOnly(CString sFileFullPath);

	static void PrepareFileSend(FtpParaClass::FILE_COUNT_INFO* pInfo,int nItem,CString sLocalCurrentDirectory,CString sServerCurrentDirectory);

	static UINT RenameFileThread(LPVOID lParam);
	static UINT DeleteFileThread(LPVOID lParam);
	static UINT ReceiveFileThread(LPVOID lParam);
	static UINT SendFileThread(LPVOID lParam);
	static UINT FindLocFileThread(LPVOID lParam);
	static UINT FindServFileThread(LPVOID lParam);
	static UINT CreateDirectorServThread(LPVOID lParam);

	static BOOL BeginAThread(AFX_THREADPROC pFunc,LPVOID lParam);
	static int OpenInternet(HINTERNET& hInetSession,HINTERNET& hFtpConn);

private:
	static BOOL PostMessageToInfoWnd( UINT message, WPARAM wParam = 0, LPARAM lParam = 0 );

	static BOOL SendMessageToLocalWnd( UINT message, WPARAM wParam = 0, LPARAM lParam = 0 );

protected:
	static BOOL PreDeleteFile(HINTERNET hInetSession,CString& strFtp,CString& OldDir);
	static BOOL PreReceiveFile(HINTERNET hInetSession,CString& str,CString& strFtp,LPVOID pInf);
	static BOOL PreSendFile(CFtpConnection* lpFtp,CString& str,CString& strFtp,LPVOID pInf);


	//{{AFX_VIRTUAL(FtpGetSend)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL


protected:
	virtual ~FtpGetSend();


//{{AFX_MSG(FtpGetSend)

//}}AFX_MSG

DECLARE_MESSAGE_MAP()
};

#endif 





