
#pragma once

#include "StructKlineView.h"

typedef map<std::string, int> MapIndex;

#define MAX_STOCK_NUM			4096
#define MAX_BLOCK_USE			16

typedef struct TSK_TICKINDEX
{
	char stockCode[8];
	int tickCount;
	int numReserved;
	WORD symBlock[MAX_BLOCK_USE];
};
#define TSK_TICKINDEX_SIZE		sizeof(TSK_TICKINDEX)

class CTaiShanDoc;
class CProgressDialog;

class CTaiKlineFileHS : public CStkFile  
{
public:
	static int GetCountPre(CString symbol, int stkKind);

public:
	int WriteHsToFile(CString symbol, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord);
	static void WriteHsToFileWideNet(CString symbol,int nMarket, CString sDate,RCV_DISPBARGAINING_STRUCTEx *pData,  int nRecord,int nRequest);
	static bool WriteHsDataWideNet(RCV_DISPBARGAINING_STRUCTEx* pData,int nPacketNum,int nRequest);//for qyp
	static bool IsNeedHsHistData(CString symbol,int stkKind,CString sDate);

	static int GetMinuteSecond(int timet);
	bool m_bToday ;
	static CTaiKlineFileHS* BillOpenFileHs(CString strTime,bool bShangHai=true,bool bToday=false);
	static bool BillTransferDataEx(CTaiKlineFileHS* pFileHs,char *pStockCode, CBuySellList *pBuySellList, KlineEx *&pKlineEx);
	static bool TransferDataEx(CBuySellList* pBuySellList,KlineEx*& pKlineEx);
	static void DoCloseWorkHs(CProgressDialog* pDlg = NULL);
	static TRADE_DETAIL_H_PER* GetAtBS(CBuySellList *pBuySellList,int nFoot,int nCount);
	enum{
		BUYVOL=0,SELLVOL=1,ISBUYORDER=2,
		ASKPRICE=3,ASKVOL=4,BIDPRICE=5,BIDVOL=6
	};
	static void TransferHs(CBuySellList* pBuySellList,ARRAY_BE& pp,int nMax,int nKindIn,int nOther, KlineEx *pKlineEx = NULL);
	CTaiShanDoc * pDoc;
	static void Cdat1ToHs(CReportData* pCdat,TRADE_DETAIL_H_PER* pHs,bool bToHs = true, TRADE_DETAIL_H_PER *pHsPre = NULL);
	int ReadHS(CString symbol,CBuySellList& buySellList,bool bClearAll = true);	
	int ReadHSPeriod(CString symbol, CBuySellList& buySellList, CTime timeStart, CTime timeEnd,bool bClearAll = true);//读取一段时间的K线数据

	static int TransferHsToMin1(CBuySellList& buySellList,TRADE_DETAIL_H_PER* pHs,int nTotal = 240);//transfer hs to 1 minute line


private:
	void AddSmallHeadBlock();
	int ReadIndexHS(CString symbol,CBuySellList& buySellList,bool bClearAll);
	static void WriteHistoryHsFile(bool bSh,CProgressDialog* pDlg = NULL);
	void AddNewStockToFile(CString symbol,TSK_TICKINDEX*& pTickIndex);
	int CreateOrMoveSmallBlock(TSK_TICKINDEX* pTickIndex,int& nBlock);

public:
	CTaiKlineFileHS();
	CTaiKlineFileHS(int nMarket, BOOL bToday = TRUE);
	virtual ~CTaiKlineFileHS();

public:
	static CTaiKlineFileHS*	m_fileHsSh;
	static CTaiKlineFileHS*	m_fileHsSz;

protected:
	int m_nMarket;

public:
	static BOOL OpenAll();
	static void CloseAll();

	static CTaiKlineFileHS* GetFilePointer(CString symbol, int stkKind);
	static CTaiKlineFileHS* GetFilePointer2(int nMarket);
	static int GetDataCountAll(CString symbol, int stkKind);
	static void RemoveHs(CBuySellList& buySellList);

public:
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd = 0, CFileException* pException = NULL);

protected:
	void WriteHeaderInfo();

public:
	BOOL WriteHS(TSK_TICKINDEX* pTickIndex, TRADE_DETAIL_H_PER* pHs);
	BOOL WriteHS2(CString symbol, CBuySellList& buySellList);


protected:
	MapIndex m_mapIndex;
	BOOL m_bIndex;

protected:
	void AddIndexToMap();
	void DeleteMap();
	void LookupIndex(std::string symbol, int& nIndex);

protected:
	int GetTickIndex(std::string symbol, TSK_TICKINDEX* pTickIndex);
	BOOL SetTickIndex(int nIndex, TSK_TICKINDEX* pTickIndex);

public:
	int GetDataCount(std::string symbol);

public:
	void ZeroHsCountEach();

public:
	BOOL WriteHS(CReportData* pCdat, BOOL bFirstOne = FALSE);
	int ReadHS2(CString symbol, CBuySellList& buySellList, BOOL bClearAll);
};
