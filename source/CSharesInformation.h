
#if !defined(AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StkFile.h"
#include "CSharesBaseInfo.h"

#define STOCKTYPE  13
#define ADDCOUNT   10

class CSharesInformation : public CStkFile
{
public:
	CSharesInformation();
	virtual ~CSharesInformation();

protected:

	BYTE* m_pbData;

	REALDATA* m_RealFileHead;
	CReportData* m_pMapData;
	STOCKDATASHOW* m_pData[STOCKTYPE];
	DWORD m_dwStockMaxCount[STOCKTYPE];
	DWORD* m_pdwStockCurrentCount;

	Rsdn1* Nidx[3];
	Tidxd* Tidx[3];

	int m_countUp[4][6];
	int m_countDown[4][6];
	int m_countEqual[4][6];
	float m_indexAmount[4][6];

	char m_sPath[255];


	CSharesBaseInfo m_StockBaseInfo;


protected:
	BOOL SavePosToFile();
	void SavePosToFile(int StockType);

public:
	BOOL ClearRealDataMinute();


public:
	long  GetTotalStockTypeCount() { return STOCKTYPE;}  
	long  GetStockTypeCount(DWORD StockType) { return m_pdwStockCurrentCount[StockType];}  
	BOOL  GetStockTypeMaxCount(DWORD StockType) { return m_dwStockMaxCount[StockType];}   
	long  GetCount();   
	BOOL  GetStockItem(DWORD StockType,DWORD nPos,PCdat1 &pStockData);   

	BOOL InsertItem(char *StockId ,PCdat1 &pStockData,DWORD StockType); 
	BOOL Lookup(char *StockId,PCdat1 &pStockData,int nKind);  
	int   GetStockPos(int StockType,char *StockId); 


public:
	BOOL InitRealTimeData(CString strPath = _T(""));

	void CalcIndexBuyAndSell();  
	void GetIndexRsdn(PRsdn1 &pNidx) { pNidx=&Nidx[0]; }                          
	void GetIndexTidxd(PTidxd &pTidx ){ pTidx=&Tidx[0]; }                        
	REALDATA *GetRealDataHead() {return m_RealFileHead ;}   
	BOOL SetRealDataHead(PREALDATA &pRealData);              
	void SaveRealDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);                                                                
public:

private:

private:

	BOOL InsertItemPoint(CReportData *pStockData );            

public:
	float GetValueUpDown(int isDown,int whick_stk,int nKind);
	void CalcUpDown(int which_stk);
	static bool IsZhiShu(int kind);
	void AddStocksIfEmpty();
	static CString Symbol4To6(CString sIn);
	static DWORD GetStockKind(int MarketType,char *strLabel);
	static WORD GetStockMarket(int nKind);
	BOOL AddOneStockInfo(CString strStockCode,CString strStockName,
		CString strStockPyjc,int nKind);               
	BOOL MOdifyOneStockInfo(CString strStockCode,CString strStockName,
		CString strStockPyjc,int nKind);               
	BOOL RemoveAllStockCjmxTj();                   

public:
	int GetChuQuanInfo(CString strStockCode, int nKind, PSplit& pSplit);
	BOOL AddChuQuanInfo(CString strStockCode, int nKind, Split* pSplit);
	BOOL ModifyChuQuanInfo(CString strStockCode, int nWhichItem, Split* pSplit, int nKind);
	BOOL DeleteChuQuanInfo(CString strStockCode, int nWhichItem, int nKind);

	BOOL ExportChuQuanInfo(CString strStockCode, PSplit& pSplit, int& nChuquanTotalTimes, int nKind);

	BOOL RemoveStockInfo();                                  
	BOOL RemoveStockCjmxTj(char *code,int nKind,BOOL IsAdd);            
	BOOL InsertItemCorrect(char *StockId ,PCdat1 pStockData, DWORD StockType); 



	BOOL DeleteAllStockFromStockType();

#ifdef WIDE_NET_VERSION
	BOOL ClearAllRealTimeMarketData();
#endif

protected:
	void ClearUpDown(int nMarket);

protected:
	BOOL InitRealTimeDataEmpty();
	BOOL InitRealTimeDataExist();
	BOOL InitEmptyDatabase();
	BOOL SetMemroyALLOCSize(DWORD StockType, unsigned int nSize);

public:
	BOOL DeleteOneStockInfo(CString strStockCode, int nKind);
	BOOL RemoveKey(char* StockId, DWORD StockType);
	void MoveStock(CString strStockCode, int nKind, int nNewKind);

public:
	BOOL RecvStockDataForType(PSTOCKDATASHOW& p, BYTE StockType);



public:
	/* 初始化财务资料，将财务资料连接到 Report
	*/
	void InitBaseInfoData();

public:
	/* 插入财务项目
	*/
	BOOL AddBaseinfoPoint(CString strStockCode, int nKind, PBASEINFO& pBaseinfo);
	BOOL LookupBase(CString strStockCode, int nKind, PBASEINFO& m_pStock);

public:
	void ReadAllBaseInfo();
	void ReadBaseInfoData(PCdat1& pStockData);
	BOOL ImportCaiwuInfo(BASEINFO* pBaseinfo);
	BOOL ImportChuQuanInfo(CString strStockCode, Split* pSplit, int nChuquanTotalTimes, int nKind);
};

inline BOOL CSharesInformation::LookupBase(CString strStockCode, int nKind, PBASEINFO& m_pStock)
{
	WORD wMarket = GetStockMarket(nKind);

	char szStock[STKLABEL_LEN] = "";
	memcpy(szStock, &wMarket, sizeof(WORD));
	strcat(szStock, strStockCode);

	CString strSymbol(szStock);

	return m_StockBaseInfo.Lookup(strSymbol, m_pStock);
}

inline void CSharesInformation::SaveRealDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress,dwNumberOfBytesToFlush);
}
inline BOOL CSharesInformation::SetRealDataHead(PREALDATA &pRealData)
{
	if(m_RealFileHead==NULL)
		return FALSE;
	pRealData=m_RealFileHead;
	SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPE); 
	return TRUE;
}

inline long  CSharesInformation::GetCount()
{
	long temp=0;
	for(int i=0;i<STOCKTYPE;i++)
		temp+=m_pdwStockCurrentCount[i];
	return temp;
}
inline BOOL CSharesInformation::GetStockItem(DWORD StockType,DWORD nPos,PCdat1 &pStockData)
{
	if(nPos < m_dwStockMaxCount[StockType])
	{
		pStockData=m_pData[StockType][nPos].pItem; 
		return TRUE;
	}
	else
	{
		pStockData=NULL;
		return FALSE;
	}
}  

#endif // !defined(AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
