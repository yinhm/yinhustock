
#include "stdafx.h"
#include "CTaiKlineMemFile.h"

//#include <winnetwk.h>
//#include <shlobj.h>
//#include <shellapi.h>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTaiKlineMemFile::CTaiKlineMemFile()
{
}

CTaiKlineMemFile::~CTaiKlineMemFile()
{
}

CTaiKlineMemFile::CTaiKlineMemFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{
	ASSERT(AfxIsValidString(lpszFileName));

	CFileException e;
	if (!Open(lpszFileName, nOpenFlags, 0,&e))
		AfxThrowFileException(e.m_cause, e.m_lOsError, e.m_strFileName);
}

extern BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
extern void AFXAPI AfxGetRoot(LPCTSTR lpszPath, CString& strRoot);
