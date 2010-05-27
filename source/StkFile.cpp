
#include "stdafx.h"
#include "StkFile.h"

#include <io.h>

CStkFile::CStkFile()
{
	m_bShareMem = FALSE;
	m_sNameShareMem = _T("");

	m_hFile = (HANDLE)hFileNull;
	m_hFileMap = NULL;
	m_lpvFileBegin = NULL;
	m_lpvFileEnd = NULL;
	m_lpvFileCurrent = NULL;
	m_nLenFile = 0;

	m_nAddReMap = 1024;
	m_bCloseOnDelete = FALSE;

	m_pSymbolToPos = NULL;
	MaxNumStock = 4096;
}

CStkFile::~CStkFile()
{
	if (m_hFile != (HANDLE)hFileNull && m_bCloseOnDelete)
		Close();

	if (m_pSymbolToPos != NULL)
		delete m_pSymbolToPos;
}

void CStkFile::SeekToBegin()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	m_lpvFileCurrent = m_lpvFileBegin;
	ASSERT(m_lpvFileCurrent > 0);
	if (m_lpvFileCurrent <= 0)
	{
		AfxMessageBox("文件指针不能为负数!");
	}
}

DWORD CStkFile::SeekToEnd()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	int dwRtn = 0;
	ASSERT(m_lpvFileCurrent > 0 && m_lpvFileCurrent <= m_lpvFileEnd);
	dwRtn = m_lpvFileEnd - m_lpvFileBegin;
	if (dwRtn < 0)
	{
		AfxMessageBox("SeekToEnd 返回的文件长度不能为负数!");
	}

	m_lpvFileCurrent = m_lpvFileEnd;
	if (m_lpvFileCurrent <= 0)
	{
		AfxMessageBox("文件指针不能为负数!");
	}

	return dwRtn;
}

LONG CStkFile::Seek(LONG lOff, UINT nFrom)
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	ASSERT(m_bShareMem == TRUE || m_hFile != (HANDLE)hFileNull);
	ASSERT(nFrom == begin || nFrom == end || nFrom == current);
	ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);

	switch (nFrom)
	{
	case begin:
		m_lpvFileCurrent = m_lpvFileBegin + lOff;
		break;
	case end:
		m_lpvFileCurrent = m_lpvFileEnd + lOff;
		break;
	case current:
		m_lpvFileCurrent = m_lpvFileCurrent + lOff;
		break;
	}

	ASSERT(m_lpvFileCurrent > 0);
	if (m_lpvFileCurrent <= 0)
	{
		AfxMessageBox("文件指针不能为负数!");
	}

	int nRtn = m_lpvFileCurrent - m_lpvFileBegin;
	ASSERT(nRtn >= 0);

	return nRtn;
}

BYTE* CStkFile::GetFileBeginPointer()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	return m_lpvFileBegin;
}

BYTE* CStkFile::GetFileCurrentPointer()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	ASSERT(m_lpvFileEnd - m_lpvFileCurrent >= 0);
	return m_lpvFileCurrent;
}

extern BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
extern void AFXAPI AfxGetRoot(LPCTSTR lpszPath, CString& strRoot);

BOOL CStkFile::Open(LPCTSTR lpszFileName, UINT nFileLength, CString strShareName)
{
	if (lpszFileName == NULL)
		return FALSE;

	m_hFile = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, nFileLength, NULL);
	if (m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		CloseHandle(m_hFile);
		m_hFile = NULL;
		m_hFileMap = NULL;
		return FALSE;
	}

	m_lpvFileBegin = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (m_lpvFileBegin == NULL)
	{
		ASSERT(FALSE);
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		m_hFile = NULL;
		m_hFileMap = NULL;
		return FALSE;
	}

	return TRUE;
}

BOOL CStkFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd, CFileException* pException)
{
	ASSERT(AfxIsValidString(lpszFileName));
	ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));

	nOpenFlags &= ~(UINT)typeBinary;

	m_bCloseOnDelete = FALSE;
	m_hFile = (HANDLE)hFileNull;
	m_strFileName.Empty();

	TCHAR szTemp[_MAX_PATH];
	AfxFullPath(szTemp, lpszFileName);
	m_strFileName = szTemp;

	ASSERT(sizeof(HANDLE) == sizeof(UINT));
	ASSERT(shareCompat == 0);


	int nFileLen = 0;
	if (_access(lpszFileName, 06) == -1)
	{
		nFileLen = 16;
	}


	ASSERT((modeRead | modeWrite | modeReadWrite) == 3);

	HANDLE hFile = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		if (pException != NULL)
		{
			pException->m_lOsError = GetLastError();
			pException->m_cause = CFileException::OsErrorToException(pException->m_lOsError);
			pException->m_strFileName = lpszFileName;
		}

		return FALSE;
	}

	m_hFile = (HANDLE)hFile;
	m_bCloseOnDelete = TRUE;
	m_nLenFile = GetFileSize((HANDLE)m_hFile, NULL);


	DWORD flProtect = 0;
	flProtect = PAGE_READWRITE;

	if (m_sNameShareMem == "")
	{
		m_hFileMap = CreateFileMapping((HANDLE)m_hFile, NULL, flProtect, 0, nFileLen + m_nLenFile + nAddToFileEnd, NULL);
	}
	else
	{
		m_hFileMap = CreateFileMapping((HANDLE)m_hFile, NULL, flProtect, 0, nFileLen + m_nLenFile + nAddToFileEnd, m_sNameShareMem);
	}
	if (m_hFileMap == NULL)
	{
		CString ss = m_strFileName;
		ss = "文件" + ss;
		AfxMessageBox("遇到错误! hFileMap 是零。请关闭程序后删除此文件！");
		CloseHandle(m_hFileMap);
		return FALSE;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
	}


	m_lpvFileBegin = (BYTE*)MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, nFileLen + m_nLenFile + nAddToFileEnd);
	ASSERT(m_lpvFileBegin != NULL);

	m_lpvFileCurrent = m_lpvFileBegin;

	DWORD dwError;
	if (m_nLenFile == -1 && (dwError = GetLastError()) != NO_ERROR)
	{
		AfxMessageBox("GetFileSize 出错!");
		m_nLenFile = 0;
		m_lpvFileEnd = m_lpvFileBegin;
		return FALSE;
	}

	m_lpvFileEnd = m_lpvFileBegin + m_nLenFile + nAddToFileEnd;

	return TRUE;
}

BOOL CStkFile::OpenShare(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd, CString sShareName, CFileException* pException)
{
	m_bShareMem = TRUE;

	ASSERT(AfxIsValidString(lpszFileName));
	ASSERT(pException == NULL || AfxIsValidAddress(pException, sizeof(CFileException)));


	nOpenFlags &= ~(UINT)typeBinary;

	m_bCloseOnDelete = FALSE;
	m_hFile = (HANDLE)hFileNull;
	m_strFileName.Empty();

	TCHAR szTemp[_MAX_PATH];
	AfxFullPath(szTemp, lpszFileName);
	m_strFileName = szTemp;


	ASSERT(sizeof(HANDLE) == sizeof(UINT));
	ASSERT(shareCompat == 0);


	CString sFileN = lpszFileName;
	int nFileLen = 0;
	if (sFileN != "")
	{
		CFile fl;
		if (fl.Open(m_strFileName, CFile::shareDenyNone | CFile::modeReadWrite) == FALSE)
		{
			return FALSE;
		}

		nFileLen = fl.GetLength();
		if (nFileLen <= 0)
		{
			nFileLen = 16;
		}

		fl.Close();
	}


	m_bCloseOnDelete = TRUE;
	m_nLenFile = nFileLen;


	DWORD flProtect = 0;
	flProtect = PAGE_READWRITE;

	m_sNameShareMem = sShareName;
	m_hFileMap = CreateFileMapping((HANDLE)m_hFile, NULL, flProtect, 0, m_nLenFile + nAddToFileEnd, m_sNameShareMem);
	if (m_hFileMap == NULL)
	{
		AfxMessageBox("Error! hFileMap is Null.");
		CloseHandle(m_hFileMap);
		return FALSE;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
	}


	m_lpvFileBegin = (BYTE*)MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, m_nLenFile + nAddToFileEnd);
	ASSERT(m_lpvFileBegin != NULL);

	m_lpvFileCurrent = m_lpvFileBegin;

	DWORD dwError;
	if (m_nLenFile == -1 && (dwError = GetLastError()) != NO_ERROR)
	{
		AfxMessageBox("GetFileSize 出错!");
		m_nLenFile = 0;
		m_lpvFileEnd = m_lpvFileBegin;
		return FALSE;
	}

	m_lpvFileEnd = m_lpvFileBegin + m_nLenFile + nAddToFileEnd;

	if (sFileN == "")
	{
		m_nLenFile = nAddToFileEnd;
	}

	return TRUE;
}


UINT CStkFile::Read(void* lpBuf, UINT nCount)
{
	ASSERT(m_bShareMem == TRUE || m_hFile != (HANDLE)hFileNull);
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (nCount == 0)
		return 0;

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount));

	DWORD dwRead = nCount;
	if (m_lpvFileCurrent + nCount > m_lpvFileEnd)
		return 0;

	ASSERT(m_lpvFileEnd - m_lpvFileCurrent - dwRead >= 0);
	if (m_lpvFileEnd - m_lpvFileCurrent - dwRead < 0)
		return 0;

	memcpy(lpBuf, m_lpvFileCurrent, dwRead);

	m_lpvFileCurrent = m_lpvFileCurrent + dwRead;

	return (UINT)dwRead;
}

void CStkFile::Write(const void* lpBuf, UINT nCount)
{
	ASSERT(m_bShareMem == TRUE || m_hFile != (HANDLE)hFileNull);
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	if (nCount == 0)
		return;

	if (ReMap(nCount) == FALSE)
		return;

	ASSERT(lpBuf != NULL);
	ASSERT(AfxIsValidAddress(lpBuf, nCount, FALSE));

	memcpy(m_lpvFileCurrent, lpBuf, nCount);
	m_lpvFileCurrent += nCount;

	if (m_lpvFileCurrent > m_lpvFileEnd)
		m_lpvFileEnd = m_lpvFileCurrent;
}

DWORD CStkFile::GetLength() const
{
	DWORD dwLength = m_lpvFileEnd - m_lpvFileBegin;
	return dwLength;
}

void CStkFile::Close()
{
	if (m_hFile == (HANDLE)hFileNull)
		return;

	BOOL bError = FALSE;
	if (m_hFile != (HANDLE)hFileNull)
		bError = !CloseHandle((HANDLE)m_hFile);

	m_hFile = (HANDLE)hFileNull;
	m_bCloseOnDelete = FALSE;
	m_strFileName.Empty();

	if (m_lpvFileBegin != NULL)
		UnmapViewOfFile(m_lpvFileBegin);
	if (m_hFileMap != NULL)
		CloseHandle(m_hFileMap);

	if (bError)
		CFileException::ThrowOsError((LONG)::GetLastError());
}

void CStkFile::Flush()
{
}

int CStkFile::GetStockNumber()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	int nData;
	SeekToBegin();
	Read(&nData, 4);

	return nData;
}

void CStkFile::SetStockNumber(int nStock)
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return ;
	}

	SeekToBegin();
	Write(&nStock, 4);
}

WORD CStkFile::GetMaxNumStock()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	WORD wd;
	int n = 0;
	Seek(14, begin);
	Read(&wd, 2);

	if (wd < 4096 || wd > 4096 * 4)
	{
		wd = 4096;
		Seek(14, begin);
		Write(&wd, 2);
	}

	return wd;
}

void CStkFile::SetMaxNumStock(WORD nMaxNumStock)
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return ;
	}

	Seek(14, begin);
	Write(&nMaxNumStock, 2);
}

int CStkFile::GetSmallBlockCount()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	int nData;
	Seek(4, begin);
	Read(&nData,4);

	return nData;
}

void CStkFile::SetSmallBlockCount(int nBlock)
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	Seek(4, begin);
	Write(&nBlock, 4);
}

int CStkFile::GetID()
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	int nData = 0;
	SeekToBegin();
	Read(&nData, 4);
	Read(&nData, 4);
	Read(&nData, 4);

	return nData;
}

void CStkFile::ReMapFromBegin(int nCount)
{
	if (m_bShareMem == FALSE && m_hFile == (HANDLE)hFileNull || m_hFileMap == NULL)
	{
		ASSERT(FALSE);
		return;
	}

	SeekToBegin();
	ReMap(nCount);
}

BOOL CStkFile::ReMap(int nAdd)
{
	if (m_bShareMem == TRUE)
		return TRUE;

	ASSERT(nAdd >= 0);
	if ((DWORD)(m_lpvFileCurrent + nAdd - m_lpvFileBegin) <= m_nLenFile)
		return TRUE;

	int nLenFile = (int)(m_lpvFileCurrent - m_lpvFileBegin) + nAdd + m_nAddReMap;
	ASSERT(nLenFile >= 0);
	if (nLenFile < 0)
		return FALSE;

	ASSERT(m_bShareMem == TRUE || m_hFile != (HANDLE)hFileNull);

	if (m_lpvFileBegin != NULL)
	{
		UnmapViewOfFile(m_lpvFileBegin);
	}
	if (m_hFileMap != NULL)
	{
		CloseHandle(m_hFileMap);
	}

	if (m_sNameShareMem != "")
	{
		m_hFileMap = CreateFileMapping((HANDLE)m_hFile, NULL, PAGE_READWRITE, 0, nLenFile, m_sNameShareMem);
	}
	else
	{
		m_hFileMap = CreateFileMapping((HANDLE)m_hFile, NULL, PAGE_READWRITE, 0, nLenFile, NULL);
	}
	if (m_hFileMap == NULL)
	{
		CString ss = m_strFileName;
		ss = "文件" + ss;
		AfxMessageBox("遇到错误! hFileMap 是零。请关闭程序后删除此文件！");
		CloseHandle(m_hFileMap);
		return FALSE;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (m_bShareMem == FALSE)
		{
			AfxMessageBox("File already exists.");
			CloseHandle(m_hFileMap);
			m_hFileMap = NULL;
			return FALSE;
		}
	}

	int nCurrent = m_lpvFileCurrent - m_lpvFileBegin;
	ASSERT(nCurrent >= 0);

	BYTE* lpvFileBegin = (BYTE*)MapViewOfFile(m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, nLenFile);
	ASSERT(lpvFileBegin != NULL);
	if (lpvFileBegin == NULL)
		return FALSE;

	BOOL bCh = TRUE;
	if (m_lpvFileBegin == lpvFileBegin)
		bCh = FALSE;

	m_lpvFileBegin = lpvFileBegin;
	m_lpvFileCurrent = m_lpvFileBegin + nCurrent;

	m_nLenFile = nLenFile;
	m_lpvFileEnd = m_lpvFileBegin + m_nLenFile;

	return TRUE;
}
