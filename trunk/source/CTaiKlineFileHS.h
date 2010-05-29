// CTaiKlineFileHS.h: interface for the CTaiKlineFileHS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LMBFILEHS_H__5655C041_C739_11D3_96E5_0080C8E1242B__INCLUDED_)
#define AFX_LMBFILEHS_H__5655C041_C739_11D3_96E5_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CTaiKlineMemFile.h"
#include "StructKlineView.h"

struct HSSMALLHEAD
{
	char	StockSign[8];
	int		numHS;
	int		numReserved;
	WORD	symBlock[16];
};

class	CTaiShanDoc;
class   CProgressDialog;
class CTaiKlineFileHS : public CTaiKlineMemFile  
{
public:
	int WriteHsToFile(CString symbol, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord);
	static void WriteHsToFileWideNet(CString symbol,int nMarket, CString sDate,RCV_DISPBARGAINING_STRUCTEx *pData,  int nRecord,int nRequest);
	static bool WriteHsDataWideNet(RCV_DISPBARGAINING_STRUCTEx* pData,int nPacketNum,int nRequest);//for qyp
	static bool IsNeedHsHistData(CString symbol,int stkKind,CString sDate);

	static int GetCountPre(CString symbol,int stkKind);
	void DeleteMap();
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
	int GetHsSmallHeader(CString symbol,HSSMALLHEAD* pHsSmallHead);
	int ReadHS(CString symbol,CBuySellList& buySellList,bool bClearAll = true);	
	int ReadHSPeriod(CString symbol, CBuySellList& buySellList, CTime timeStart, CTime timeEnd,bool bClearAll = true);//读取一段时间的K线数据

	bool WriteHS(CReportData* pCdat,bool bFirstOne = false);
	void ClearHS();	
	static int TransferHsToMin1(CBuySellList& buySellList,TRADE_DETAIL_H_PER* pHs,int nTotal = 240);//transfer hs to 1 minute line

	bool SetHsSmallHeader(int nIndex,HSSMALLHEAD* pHsSmallHead);

private:
	void AddSmallHeadBlock();
	int ReadIndexHS(CString symbol,CBuySellList& buySellList,bool bClearAll);
	static void WriteHistoryHsFile(bool bSh,CProgressDialog* pDlg = NULL);
	void AddNewStockToFile(CString symbol,HSSMALLHEAD*& pHsSmallHead);
	int CreateOrMoveSmallBlock(HSSMALLHEAD* pHsSmallHead,int& nBlock);

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
	void AddIndexToMap();
	void WriteHeaderInfo();

public:
	int ReadHS2(CString symbol, CBuySellList& buySellList, BOOL bClearAll);
	BOOL WriteHS(HSSMALLHEAD* pHsSmallHead, TRADE_DETAIL_H_PER* pHs);
	BOOL WriteHS2(CString symbol, CBuySellList& buySellList);
	void ZeroHsCountEach();

public:
	int GetDataCount(CString symbol);
};

#endif // !defined(AFX_LMBFILEHS_H__5655C041_C739_11D3_96E5_0080C8E1242B__INCLUDED_)
