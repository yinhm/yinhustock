
#if !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StkFile.h"
#include <afxcmn.h>

#define ADDSIZES	50
#define CHOOSEPOS	251

class CSuperviseSharesBlockData : public CStkFile
{
public:
	CSuperviseSharesBlockData();
	virtual ~CSuperviseSharesBlockData();
	enum {ZGB=0,LTG=1,OTHER=2	 };     

private:
	CTaiShanDoc   *m_pDoc;      

protected:
	STOCKTYPEHEAD* m_pStockTypeHead;		// 
	STOCKTYPEINFO* m_pStockTypeInfo;		// 
	STOCK_TYPE_INFO* m_pStockInfo;			// 
	STOCK_POINT_INFO* m_pStockPoint;		// 

protected:
	BOOL InitStockTypeDataEmpty(CString strPath);       
	BOOL InitStockTypeDataExist(CString strPath);    
	BOOL AddStockTypeDataSize(int AddSize = ADDSIZES);
	BOOL SetMemroyALLOCSize(unsigned int nSize);  
	BOOL InsertItemPoint(STOCK_TYPE_INFO* m_pStk);  
	BOOL SavePosToFile();                             
	void SaveDataToFile(LPCVOID lpBaseAddress, DWORD dwNumberOfBytesToFlush);

public:
	BOOL InitStockTypeData(CString strPath);
	void GetStockTypeHeadPoint(PSTOCKTYPEHEAD& pStockTypeHead) { pStockTypeHead = m_pStockTypeHead; }

	// 将股票连接到 Report->pStockTypeInfo
	BOOL InitStockPoint();

	// 插入板块指数到 Report (STKTYPE)
	BOOL InitStockTypePoint();

	void CalcAllStockTypeData();           
	void CalcRealTimeStockTypeIndex();      
	void CalcMinuteRealTimeIndex();    



public:
	// 在指定位置插入板块
	BOOL InsertStockType(PSTOCKTYPEINFO& m_pStktype, int m_iPos);
	BOOL DeleteStockType(char* m_pszStockTypeCode);

	// 获取指定板块的证券列表
	BOOL GetStockFromStockType(SymbolKindArr& m_StockCodeArray, char* m_pszStockTypeCode);
	BOOL GetStockFromStockTypeName(SymbolKindArr& m_StockCodeArray, char* m_pszStockTypeName);

	// 获取指定板块的证券数
	int GetStockCountFromType(char* m_pszCode);

	// 取得有效板块个数
	int GetStockTypeCounts();

	// 在板块中插入删除股票
	BOOL InsertStockToType(PCdat1& m_pCdat, char* m_szStockTypeCode);       
	BOOL DeleteStockFromType(char* m_pszStockCode, int nKind, char* m_szStockTypeCode);    
	BYTE GetStockTypeNumber(char* m_szStockTypeCode);

	// 检测版块名和代码是否重复
	BOOL CheckBlockName(char* BlockName, char* BlockCode);

	// 获取板块空白插入位置
	int GetInsertStockTypePos();

	// 获取所有板块名称和代码
	void GetAllStockTypeCode(CStringArray& m_StockTypeCodeArray);
	void GetAllStockTypeName(CStringArray& m_StockTypeNameArray);

	// 获取板块信息
	BOOL GetStockTypePoint(PSTOCKTYPEINFO& m_pStktype, char* m_pszStockTypeCode);
	BOOL GetStockTypePointFromName(PSTOCKTYPEINFO& m_pStktype, char* m_pszStockTypeName);

	// 导入股票板块
	BOOL ImportStockTypeData(CString m_szPathStr, CString m_szStockNameStr, PSTOCKTYPEINFO& m_pStktype, int nRightType = LTG);

public:
	static bool CheckKind(int i);
	void  CheckStockCount(int AddStockCount);


	void RefreshHistoryDayLineData(char *pszStockTypeCode, CProgressCtrl *p=NULL);
	BOOL GetStockTypeStartDate(char * pszStockTypeCode,SymbolKindArr &StockCodeArray,time_t &t);

	BOOL InsertStockItemCorrect(char *m_szStockId ,PSTOCK_TYPE_INFO m_pStock) ;     
	long CorrectKline(Kline *pKline,long LineCount);
	float GetStockBlockRight(char *StockTypeCode);

public:
	BOOL InsertStockItem(char* m_szStockId, int nKind, PSTOCK_TYPE_INFO& m_pStock);
	BOOL DeleteStockItem(char* m_szStockId, int nKind);  
	BOOL RemoveDeletedStock();
	BOOL Lookup(char* m_szStockId, int nKind, PSTOCK_TYPE_INFO& m_pStock);

public:
	BOOL GetChooseStockCode(SymbolKindArr& m_StockCodeArray);
	int GetChooseStockCounts();
	BOOL InsertStockToChoose(char* m_szStockId, int nKind);
	BOOL DeleteStockFromChoose(char* m_szStockId, int nKind);
};

inline void CSuperviseSharesBlockData::SaveDataToFile(LPCVOID lpBaseAddress, DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush);
}

inline BYTE CSuperviseSharesBlockData::GetStockTypeNumber(char* m_szStockTypeCode)
{
	char mtemp[3];
	memcpy(mtemp, m_szStockTypeCode + 4, 2);
	mtemp[2] = '\0';
	return (BYTE)atoi(mtemp);
}

#endif // !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
