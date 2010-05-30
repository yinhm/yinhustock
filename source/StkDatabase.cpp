
#include "stdafx.h"
#include "StkDatabase.h"

//#include "StkReportFile.h"
#include "CTaiKlineFileKLine.h"
#include "CSharesBaseInfo.h"

#include "mainfrm.h"
#include "CTaiShanDoc.h"

CStkDatabase* CStkDatabase::s_pInstance = NULL;

CStkDatabase::CStkDatabase()
{
	m_strPath = _T("");
	//m_pStkReport = NULL;
	m_pFinance = NULL;
}

CStkDatabase::~CStkDatabase()
{
	//if (m_pStkReport) m_pStkReport->Close();
	if (m_pFinance) m_pFinance->Close();
}

BOOL CStkDatabase::InitInstance()
{
	GetAppPath();

	CreateDirectory(GetAppPath() + _T("\\Data"), NULL);

	return TRUE;
}

CString CStkDatabase::GetAppPath()
{
	if (!m_strPath.IsEmpty())
		return m_strPath;

	TCHAR szFilePath[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];

	if (GetModuleFileName(NULL, szFilePath, _MAX_PATH) == NULL)
		return _T("");

	_tsplitpath_s(szFilePath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFileName, _MAX_FNAME, szExt, _MAX_EXT);
	m_strPath = szDrive;
	m_strPath += szDir;

	return m_strPath;
}

//CStkReportFile* CStkDatabase::GetReportFile(WORD wMarket)
//{
//	CString strFile = GetAppPath() + _T("\\Data\\Report.dat");
//	if (!m_pStkReport)
//	{
//		m_pStkReport = new CStkReportFile();
//		m_pStkReport->Open(strFile, NULL, 0);
//	}
//
//	return m_pStkReport;
//}

CTaiKlineFileKLine* CStkDatabase::GetKLineFile(WORD wMarket)
{
	CTaiKlineFileKLine* pFile = NULL;

	if (wMarket == SH_MARKET_EX)
	{
		pFile = CTaiKlineFileKLine::m_fileDaySh;
	}
	else if (wMarket == SZ_MARKET_EX)
	{
		pFile = CTaiKlineFileKLine::m_fileDaySz;
	}

	return pFile;
}

CTaiKlineFileHS* CStkDatabase::GetTickFile(WORD wMarket)
{
	CTaiKlineFileHS* pFile = CTaiKlineFileHS::GetFilePointer2(wMarket);
	ASSERT(pFile);
	if (pFile == NULL)
	{
		CTaiKlineFileHS::m_fileHsSh;
	}

	return pFile;
}

CTaiKlineFileHS* CStkDatabase::GetTickFile(CString strSymbol, int nKind)
{
	WORD wMarket = CSharesInformation::GetStockMarket(nKind);
	return GetTickFile(wMarket);
}

CSharesBaseInfo* CStkDatabase::GetBaseInfoFile(WORD wMarket)
{
	CSharesBaseInfo* pFile = NULL;

	if (wMarket == SH_MARKET_EX || wMarket == SZ_MARKET_EX)
	{
		pFile = m_pFinance;
	}

	return pFile;
}

CString CStkDatabase::GetStockSymbol(char* szStock, int nKind)
{
	char szSymbol[10];

	WORD wMarket = CSharesInformation::GetStockMarket(nKind);
	memcpy(&szSymbol[0], &wMarket, sizeof(WORD));
	szSymbol[2] = '\0';
	strcat_s(szSymbol, szStock);

	return CString(szSymbol);
}

void CStkDatabase::GetStockSymbol(SymbolKind& symbol, char* szSymbol)
{
	strcpy(symbol.m_szSymbol, szSymbol);
	memcpy(&symbol.m_wMarket, szSymbol, sizeof(WORD));

	CString strSymbol(szSymbol);
	strcpy(symbol.m_chSymbol, strSymbol.Right(strSymbol.GetLength() - 2));

	symbol.m_nSymbolKind = CSharesInformation::GetStockKind(symbol.m_wMarket, symbol.m_chSymbol);
}
