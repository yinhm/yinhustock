// CTaiKlineFileKLine.h: interface for the CTaiKlineFileKLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LMBFILEKLINE_H__8E7B3BE7_C684_11D3_96E4_0080C8E1242B__INCLUDED_)
#define AFX_LMBFILEKLINE_H__8E7B3BE7_C684_11D3_96E4_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StructKlineView.h"
#include "CTaiKlineMemFile.h"

#define DAY_KLINE	0
#define MIN5_KLINE	1

#define MAX_BLOCK_USE	26

typedef struct _KLINE_SMALL_HEAD2
{
	char StockSign[8];
	int numKln;
	WORD symBlock[MAX_BLOCK_USE];
} KLINE_SMALLHEAD;

#define KLINEHEAD			16
#define KLINESMALLHEAD		4096 * 64 + 16
#define FixedKlinePerBlock	240
#define KlineByteEach		32

class CTaiKlineFileKLine : public CTaiKlineMemFile  
{
public:


	static void TestKlineVolpositive(Kline* pKline,int nCount);
	static CString CheckSymbolValid(CString symbol);
	static bool IsDayKline(int nKlineType);
	static int GetFirstKline(CString sSymbol,int stkKind,bool bDay/*=true*/,int nFoot = -1 ,float * pClose = NULL);
	static int ReadKLineAny(CString symbol,int stkKind, Kline *&pKline, int nRead, int nKlineType,CBuySellList* pBuySellList = NULL, int nAdd = 48,bool bToday=true,CTime * tmEnd = NULL,CTime *tmBegin = NULL);
	static bool IsNeedHs(int nKlineType);
	static bool OutputCloseVal(SymbolKindArr &strSymbolArray, int& nDays,CTime& tmStart,CTime &tmEnd,double*& pdbl,Kline*& pKlineIndex);
	static int ReadKlineTimeSpan(CString symbol,int stkKind,Kline*& pKline,CTime &timeB, CTime &timeE, int nKlineType,CBuySellList* pBuySellList = NULL,int nAddBlank=48);
	static void ReorgnizeFile(bool bSh ,bool bSz,bool bDayKline = true);
	int ReadKlinePeriod(int nIndex, Kline *&kline, CTime& timeStart, CTime& timeEnd,int nAddBlank=0 );
	int ReadKlineAccordingTime(CString symbol, Kline *&kline, CTime& time,int nCount = 2,int nAddBlank = 48);

	bool WriteKLineToRepair(CString symbol,Kline* pKline,int nWrite);
	static int LookTwoPath(time_t& tmt ,Kline* pKline,int nMax,bool& bAdd,bool bDay);
	int TimeToFoot(KLINE_SMALLHEAD* pKlineSmallHead,CTime& tm,bool& bAdd/*[in]*/);
	enum InstallMode {
		overWriteAll =    0x00,
		insert =  0x01,
		timeSpan =    0x02,
		};
	int ReadKlinePeriod(CString symbol, Kline *&kline, CTime& timeStart, CTime& timeEnd,bool istotaldata = false,int nAddBlank = 0);//读取一段时间的K线数据
	bool WriteKLine(CString symbol,Kline* pKline,int nWrite,int nType);	
	bool WriteKlinePeriod(CString symbol, Kline *pKline,int nKline, CTime& timeStart, CTime& timeEnd);//写一段时间的K线数据


	static int RegularTime(int time,bool bDay);
protected:


	//{{AFX_VIRTUAL(CTaiKlineMemFile)
	//}}AFX_VIRTUAL


private:
	void AddSmallHeadBlock();

	int CreateOrMoveSmallBlock(KLINE_SMALLHEAD* pKlineSmallHead,int& nBlock);

public:
	CTaiKlineFileKLine();
	CTaiKlineFileKLine(int nKind, int nMarket);
	virtual ~CTaiKlineFileKLine();

public:
	static CTaiKlineFileKLine* m_fileDaySh;
	static CTaiKlineFileKLine* m_fileDaySz;
	static CTaiKlineFileKLine* m_fileMin5Sh;
	static CTaiKlineFileKLine* m_fileMin5Sz;

public:
	int m_kindKline;
	int m_nMarket;
	BOOL m_bFirstWrite;

public:
	static BOOL OpenAll();
	static void CloseAll();

	static CTaiKlineFileKLine* GetFilePointer(CString symbol, int stkKind, BOOL bDayKline);

	static int GetKlineCount(CString symbol, int stkKind, BOOL bDayKline);
	static float GetLastClose(CString symbol, int stkKind);

	static int ReadKLineS(CString symbol, int stkKind, Kline*& pKline, int nRead = -1, int nAddBlank = 0, BOOL bDay = TRUE);
	static BOOL WriteKLineS(CString symbol, int stkKind, Kline* pKline, int nWrite, int nType = 0, BOOL bDay = TRUE);
	static void ZeroKlineData(CString symbol, int stkKind, BOOL bDay);

public:
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd = 0, CFileException* pException = NULL);

public:
	void AddIndexToMap();
	void DeleteMap();
	void LookUpIndexFromMap(CString symbol, int& nIndex);

protected:
	void WriteHeaderInfo();

	/*	计算存储KLine所需要的块数
	*	DayKline / FixedKlinePerBlock + 1
	*/
	int CalcMaxCountBlock();
	int AddNewStockToFile(CString symbol, KLINE_SMALLHEAD*& pKlineSmallHead);

public:
	int GetKlineSmallHeader(CString symbol, KLINE_SMALLHEAD* pKlineSmallHead);
	BOOL SetKlineSmallHeader(int nIndex, KLINE_SMALLHEAD* pKlineSmallHead);
	CString GetSymbol(int nIndex);

public:
	int ReadKLine(int nIndex, Kline*& pKline, int nRead);
	int ReadKLine(CString symbol, Kline*& pKline, int nRead = -1, int nAddBlank = 0);
	void DeleteKlineData(CString symbol, int nFoot, int nCount = 1);
};

#endif // !defined(AFX_LMBFILEKLINE_H__8E7B3BE7_C684_11D3_96E4_0080C8E1242B__INCLUDED_)
