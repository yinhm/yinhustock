
#pragma once

#include "StructKlineView.h"
#include "FoxDriver.h"

typedef map<std::string, int> MapIndex;

#define MAX_STOCK_NUM			4096
#define MAX_BLOCK_USE			16

typedef struct tagTSK_TICKINDEX
{
	char stockCode[8];
	int tickCount;
	int numReserved;
	WORD symBlock[MAX_BLOCK_USE];
} TSK_TICKINDEX;
#define TSK_TICKINDEX_SIZE		sizeof(TSK_TICKINDEX)

class CTaiShanDoc;
class CProgressDialog;

class CTaiKlineFileHS : public CStkFile  
{
public:
	static TRADE_DETAIL_H_PER* GetAtBS(CBuySellList* pBuySellList, int nFoot, int nCount);
	static void RemoveHs(CBuySellList& buySellList);


public:
	static int GetCountPre(CString symbol, int stkKind);

public:
	static bool IsNeedHsHistData(CString symbol,int stkKind,CString sDate);

	static int GetMinuteSecond(int timet);
	static CTaiKlineFileHS* BillOpenFileHs(CString strTime,bool bShangHai=true,bool bToday=false);
	static bool BillTransferDataEx(CTaiKlineFileHS* pFileHs,char *pStockCode, CBuySellList *pBuySellList, KlineEx *&pKlineEx);
	static bool TransferDataEx(CBuySellList* pBuySellList,KlineEx*& pKlineEx);
	static void DoCloseWorkHs(CProgressDialog* pDlg = NULL);
	enum{
		BUYVOL=0,SELLVOL=1,ISBUYORDER=2,
		ASKPRICE=3,ASKVOL=4,BIDPRICE=5,BIDVOL=6
	};
	static void TransferHs(CBuySellList* pBuySellList,ARRAY_BE& pp,int nMax,int nKindIn,int nOther, KlineEx *pKlineEx = NULL);
	CTaiShanDoc * pDoc;
	int ReadHS(CString symbol,CBuySellList& buySellList,bool bClearAll = true);	




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

	// 转换分笔数据
	static void Cdat1ToHs(CReportData* pCdat, TRADE_DETAIL_H_PER* pHs, bool bToHs = true, TRADE_DETAIL_H_PER* pHsPre = NULL);



public:
	static int WriteHsDataWideNet(FOX_TICK* pData, int nPacketNum, int nRequest);
	static void WriteHsToFileWideNet(CString symbol, int nMarket, CString sDate, FOX_TICK* pData, int nRecord, int nRequest);

protected:
	void WriteHeaderInfo();

public:
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd = 0, CFileException* pException = NULL);

public:
	// 增加一条交易分笔记录，建议在写入之前进行数据验证(时间和交易量)
	BOOL WriteHS(TSK_TICKINDEX* pTickIndex, TRADE_DETAIL_H_PER* pHs);

	// 从第一条位置开始写和分笔记录，覆盖以前数据
	BOOL WriteHS2(CString symbol, CBuySellList& buySellList);

	// 保存最新行情到分笔文件，建议此功能移动类外
	BOOL WriteHS(CReportData* pCdat, BOOL bFirstOne = FALSE);



protected:
	MapIndex m_mapIndex;
	BOOL m_bIndex;						// 是否已经索引标志

public:
	// 转换分笔数据到分时
	static int TransferHsToMin1(CBuySellList& buySellList, TRADE_DETAIL_H_PER* pHs, int nTotal = 240);

protected:
	void AddIndexToMap();
	void DeleteMap();
	void LookupIndex(std::string symbol, int& nIndex);

protected:
	// 获取指定证券的头部数据块，不存在的增加
	int GetTickIndex(std::string symbol, TSK_TICKINDEX* pTickIndex);
	BOOL SetTickIndex(int nIndex, TSK_TICKINDEX* pTickIndex);

public:
	// 获取指定证券的分笔数
	int GetDataCount(std::string symbol);

public:
	// 清除数据头部块
	void ZeroHsCountEach();

public:
	// 读取分笔数据
	int ReadHS2(CString symbol, CBuySellList& buySellList, BOOL bClearAll);

	// 保存分笔数据
	int WriteHsToFile(CString symbol, FOX_TICK* pData, int nRecord);
};
