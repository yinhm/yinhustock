
#pragma once

#include "StructKlineView.h"

typedef map<std::string, int> MapIndex;

typedef struct TSK_FILEHEADER
{
	char byte[16];
	int stocksum;
	int blocksum;
	int flag;
	WORD w1;
	WORD stockmax;
};

#define MAX_STOCK_NUM			4096
#define MAX_BLOCK_USE			26

typedef struct TSK_KLINEINDEX
{
	char stockCode[8];				// 证券代码
	int klineCount;					// 记录总数
	WORD symBlock[MAX_BLOCK_USE];	// 记录块
};
#define TSK_KLINEINDEX_SIZE		sizeof(TSK_KLINEINDEX)

//#define DAY_KLINE	0
#define MIN5_KLINE	1

class CTaiKlineFileKLine : public CStkFile  
{
public:


	static bool IsDayKline(int nKlineType);
	static int ReadKLineAny(CString symbol,int stkKind, Kline *&pKline, int nRead, int nKlineType,CBuySellList* pBuySellList = NULL, int nAdd = 48,bool bToday=true,CTime * tmEnd = NULL,CTime *tmBegin = NULL);
	static bool IsNeedHs(int nKlineType);
	static bool OutputCloseVal(SymbolKindArr &strSymbolArray, int& nDays,CTime& tmStart,CTime &tmEnd,double*& pdbl,Kline*& pKlineIndex);
	static int ReadKlineTimeSpan(CString symbol,int stkKind,Kline*& pKline,CTime &timeB, CTime &timeE, int nKlineType,CBuySellList* pBuySellList = NULL,int nAddBlank=48);
	static void ReorgnizeFile(bool bSh ,bool bSz,bool bDayKline = true);
	int ReadKlinePeriod(int nIndex, Kline *&kline, CTime& timeStart, CTime& timeEnd,int nAddBlank=0 );
	int ReadKlineAccordingTime(CString symbol, Kline *&kline, CTime& time,int nCount = 2,int nAddBlank = 48);

	static int LookTwoPath(time_t& tmt ,Kline* pKline,int nMax,bool& bAdd,bool bDay);
	int TimeToFoot(TSK_KLINEINDEX* pKlineSmallHead,CTime& tm,BOOL& bAdd/*[in]*/);
	enum InstallMode {
		overWriteAll =    0x00,
		insert =  0x01,
		timeSpan =    0x02,
		};
	int ReadKlinePeriod(CString symbol, Kline *&kline, CTime& timeStart, CTime& timeEnd,bool istotaldata = false,int nAddBlank = 0);//读取一段时间的K线数据


	static int RegularTime(int time,bool bDay);


private:
	void AddSmallHeadBlock();

	int CreateOrMoveSmallBlock(TSK_KLINEINDEX* pKlineSmallHead,int& nBlock);

public:
	CTaiKlineFileKLine();
	CTaiKlineFileKLine(int nKind, int nMarket);
	virtual ~CTaiKlineFileKLine();

public:
	static CTaiKlineFileKLine* m_fileDaySh;
	static CTaiKlineFileKLine* m_fileDaySz;
	static CTaiKlineFileKLine* m_fileMin5Sh;
	static CTaiKlineFileKLine* m_fileMin5Sz;

protected:
	int m_kindKline;
	int m_nMarket;
	BOOL m_bFirstWrite;

public:
	static BOOL OpenAll();
	static void CloseAll();

	static CTaiKlineFileKLine* GetFilePointer(CString symbol, int stkKind, BOOL bDayKline);



	// delete the functions
	static int GetFirstKline(CString sSymbol,int stkKind,bool bDay/*=true*/,int nFoot = -1 ,float * pClose = NULL);
	static int GetKlineCount(CString symbol, int stkKind, BOOL bDayKline);
	static float GetLastClose(CString symbol, int stkKind);
	static int ReadKLineS(CString symbol, int stkKind, Kline*& pKline, int nRead = -1, int nAddBlank = 0, BOOL bDay = TRUE);
	static BOOL WriteKLineS(CString symbol, int stkKind, Kline* pKline, int nWrite, int nType = 0, BOOL bDay = TRUE);
	static void ZeroKlineData(CString symbol, int stkKind, BOOL bDay);

public:
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd = 0, CFileException* pException = NULL);

protected:
	void WriteHeaderInfo();

	/*	计算存储KLine所需要的块数
	*	DayKline / FixedKlinePerBlock + 1
	*/
	int CalcMaxCountBlock();
	int AddNewStockToFile(std::string symbol, TSK_KLINEINDEX*& pKlineSmallHead);

public:

	BOOL WriteKlinePeriod(CString symbol, Kline* pKline, int nKline, CTime& timeStart, CTime& timeEnd);
	BOOL WriteKLineToRepair(CString symbol, Kline* pKline, int nWrite);



protected:
	TSK_FILEHEADER* m_pHeader;

	MapIndex	m_mapIndex;
	BOOL	m_bIndex;

public:
	// 检验主动买量数据值
	static void TestKlineVolpositive(Kline* pKline, int nCount);

public:
	virtual WORD GetMaxNumStock();
	virtual void SetMaxNumStock(WORD wStock);
	virtual int GetStockNumber();
	virtual void SetStockNumber(int nStock);
	virtual int GetSmallBlockCount();
	virtual void SetSmallBlockCount(int nBlock);



protected:
	void AddIndexToMap();
	void DeleteMap();

public:
	void LookupIndex(std::string symbol, int& nIndex);

public:
	int GetKlineIndex(std::string symbol, TSK_KLINEINDEX* pKlineIndex);
	BOOL SetKlineIndex(int nIndex, TSK_KLINEINDEX* pKlineIndex);
	CString GetSymbol(int nIndex);

public:
	int ReadKLine(int nIndex, Kline*& pKline, int nRead);
	int ReadKLine(std::string symbol, Kline*& pKline, int nRead = -1, int nAddBlank = 0);
	BOOL WriteKLine(std::string symbol, Kline* pKline, int nWrite, int nType);
	void DeleteKlineData(std::string symbol, int nFoot, int nCount = 1);
};
