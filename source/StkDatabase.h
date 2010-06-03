
#pragma once

#include "CTaiKlineFileHS.h"

// 分析家除权结构
typedef struct tagFXJ_POWER
{
	int		nTime;
	float	fGive;
	float	fAlloc;
	float	fAllocPrice;
	float	fDividend;
} FXJ_POWER;

typedef struct tagStockSymbol
{
	WORD wMarket;
	char szLabel[8];
	char szSymbol[10];
}  StockSymbol;

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
	void GetStockSymbol(SymbolKind& symbol, char* szStock, int nKind);

public:
	void AddStockSymbol(WORD wMarket, char* szLabel);
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
