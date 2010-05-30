
#pragma once

#include "CTaiKlineFileHS.h"

//class CStkReportFile;
class CTaiKlineFileKLine;
class CSharesBaseInfo;

class CStkDatabase
{
public:
	CStkDatabase();
	virtual ~CStkDatabase();

public:
	static CStkDatabase* s_pInstance;

protected:
	CString m_strPath;
	//CStkReportFile* m_pStkReport;
	CSharesBaseInfo* m_pFinance;

public:
	BOOL InitInstance();

public:
	CString GetAppPath();

public:
	//CStkReportFile* GetReportFile(WORD wMarket);
	CTaiKlineFileKLine* GetKLineFile(WORD wMarket);
	CTaiKlineFileHS* GetTickFile(WORD wMarket);
	CTaiKlineFileHS* GetTickFile(CString strSymbol, int nKind);
	CSharesBaseInfo* GetBaseInfoFile(WORD wMarket);

public:
	CString GetStockSymbol(char* szStock, int nKind);
	void GetStockSymbol(SymbolKind& symbol, char* szSymbol);
};

CStkDatabase* TSKDatabase();

inline CStkDatabase* TSKDatabase()
{
	if (CStkDatabase::s_pInstance == NULL)
	{
		static CStkDatabase sInstance;
		return &sInstance;
	}

	return CStkDatabase::s_pInstance;
}
