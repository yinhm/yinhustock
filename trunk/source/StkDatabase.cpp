
#include "stdafx.h"
#include "StkDatabase.h"

//#include "StkReportFile.h"
#include "CTaiKlineFileKLine.h"
#include "CSharesBaseInfo.h"
#include "StkBlockManager.h"			// 板块管理

#include "StkBlockDzh.h"
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

	CStringArray block;
	CStkBlockDzh dzhBlock;
	dzhBlock.EnumAllType(block);

	CStringArray arrBlock;
	dzhBlock.EnumBlockNames(_T("行业"), arrBlock);

	int nSize = arrBlock.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		CString strBlock = arrBlock.GetAt(i);

		CStringArray arrStock;
		dzhBlock.ReadSymbols(strBlock, arrStock);

		//if (GetBlockManager()->CheckBlockName(strBlock.GetBuffer(0), NULL) == FALSE)
		//{
		//	PSTOCKTYPEINFO pBlock;

		//	int nPos = GetBlockManager()->GetInsertStockTypePos();
		//	GetBlockManager()->InsertStockType(pBlock, nPos);
		//}
	}

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

CStkBlockManager* CStkDatabase::GetBlockManager()
{
	CStkBlockManager* pManager = NULL;

	CTaiShanDoc* pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	pManager = &pDoc->m_ManagerStockTypeData;

	return pManager;
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

void CStkDatabase::GetStockSymbol(SymbolKind& symbol, char* szStock, int nKind)
{

}

void CStkDatabase::AddStockSymbol(WORD wMarket, char *szLabel)
{
	CString strSymbol = _T("");
	memcpy(strSymbol.GetBuffer(0), &wMarket, sizeof(WORD));

	BOOL bFind = FALSE;

	//for (int j = 0; j < m_strArrayKind[nKind].GetSize(); j++)
	//{
	//	if (m_strArrayKind[nKind][j] == strName)
	//	{
	//		bFind = TRUE;
	//		break;
	//	}
	//}

	//if (bFind == FALSE)
	//{
	//	if (m_strArrayKind[nKind].GetSize() <= 0)
	//	{
	//		m_strArrayKind[nKind].Add(strName);
	//		return;
	//	}

	//	for (int j = 0; j < m_strArrayKind[nKind].GetSize(); j++)
	//	{
	//		int k = strName.Compare(m_strArrayKind[nKind][j]);
	//		if (k < 0)
	//		{
	//			m_strArrayKind[nKind].InsertAt(j, strName);
	//			return;
	//		}
	//	}

	//	m_strArrayKind[nKind].Add(strName);
	//}
}

void CStkDatabase::ProcessReport(RCV_REPORT_STRUCTEx* pReport, int nCount)
{
	CTaiShanDoc* pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	if (pDoc == NULL)
		return;

	BYTE* pBuffBase = (BYTE*)pReport;
	int nBuffSize = pReport->m_cbSize;

	for (int i = 0; i < nCount; i++)
	{
		RCV_REPORT_STRUCTEx* pBuffer = (RCV_REPORT_STRUCTEx*)(pBuffBase + nBuffSize * i);
		pDoc->m_sharesCompute.StockDataUpdate(pBuffer);
	}
}

void CStkDatabase::ProcessHistory(RCV_HISTORY_STRUCTEx* pHistory, int nCount)
{
	CTaiShanDoc* pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	if (pDoc == NULL)
	{
		return;
	}

	pDoc->m_sharesCompute.StockDataDayUpdate(pHistory, nCount);
}
