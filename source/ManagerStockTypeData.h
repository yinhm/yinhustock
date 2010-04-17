
#if !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define CHOOSEPOS   101
#define ADDSIZES  50
#include "AFXCMN.H"
class CSuperviseSharesBlockData  
{
public:
	CSuperviseSharesBlockData();
	virtual ~CSuperviseSharesBlockData();
	enum {ZGB=0,LTG=1,OTHER=2	 };     

private:
    HANDLE m_hFile,m_hFileMap;         
	BYTE  *m_pbtData;                   
    STOCKTYPEHEAD *m_pStockTypeHead;   
    STOCKTYPEINFO *m_pStockTypeInfo;     
    STOCK_TYPE_INFO *m_pStockInfo;       
	STOCK_POINT_INFO  *m_pStockPoint;

private:
	CTaiShanDoc   *m_pDoc;      


private:
	BOOL InitStockTypeDataExist(CString path);    
    BOOL InitStockTypeDataEmpty(CString path);       
	BOOL AddStockTypeDataSize(int AddSize=ADDSIZES);                  

    BOOL SetMemroyALLOCSize(unsigned int nSize);  
	void SaveDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);                       //保存内存数据到文件
    BOOL InsertItemPoint(STOCK_TYPE_INFO *m_pStk);  
	BOOL SavePosToFile();                             

public:
	void GetStockTypeHeadPoint(PSTOCKTYPEHEAD &pStockTypeHead){pStockTypeHead=m_pStockTypeHead;}//返回板块结构指针
    BOOL InitStockTypeData(CString path);    
	BOOL InitStockPoint();                  
	BOOL InitStockTypePoint();               

	void CalcAllStockTypeData();           
	void CalcRealTimeStockTypeIndex();      
	void CalcMinuteRealTimeIndex();    



public:
	BOOL InsertStockType(PSTOCKTYPEINFO &m_pStktype,int m_iPos);               
	int  GetStockCountFromType(char *m_pszCode);                                 
	BOOL DeleteStockType(char *m_pszStockTypeCode);                                
	BOOL GetStockTypePoint(PSTOCKTYPEINFO &m_pStktype,char *m_pszStockTypeCode); 
    int  GetInsertStockTypePos();                                              
	BOOL GetStockFromStockType(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeCode);         
	BOOL GetStockFromStockTypeName(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeName);          
	int GetStockTypeCounts();        

	BOOL InsertStockToType(PCdat1 &m_pCdat,char *m_szStockTypeCode);       
	BOOL DeleteStockFromType(char *m_pszStockCode,int nKind,char *m_szStockTypeCode);    
    BYTE GetStockTypeNumber(char *m_szStockTypeCode);                        

	void GetAllStockTypeCode(CStringArray &m_StockTypeCodeArray);             
	void GetAllStockTypeName(CStringArray &m_StockTypeNameArray);              
	BOOL GetStockTypePointFromName(PSTOCKTYPEINFO &m_pStktype,char *m_pszStockTypeName); 

    BOOL ImportStockTypeData(CString m_szPathStr,CString m_szStockNameStr,PSTOCKTYPEINFO &m_pStktype,int nRightType=LTG);     

public:
	static bool CheckKind(int i);
 	BOOL  Lookup(char *m_szStockId,int nKind,PSTOCK_TYPE_INFO &m_pStock);                
    BOOL  InsertStockItem(char *m_szStockId ,int nKind,PSTOCK_TYPE_INFO &m_pStock);      
	BOOL  DeleteStockItem(char *m_szStockId,int nKind);  
	BOOL  RemoveDeletedStock();                 
	void  CheckStockCount(int AddStockCount);

    BOOL  InsertStockToChoose(char *m_szStockId ,int nKind);    
	BOOL  DeleteStockFromChoose(char *m_szStockId,int nKind); 
	BOOL  GetChooseStockCode(SymbolKindArr &m_StockCodeArray);
	int   GetChooseStockCounts();


    void RefreshHistoryDayLineData(char *pszStockTypeCode, CProgressCtrl *p=NULL);
	BOOL GetStockTypeStartDate(char * pszStockTypeCode,SymbolKindArr &StockCodeArray,time_t &t);

    BOOL InsertStockItemCorrect(char *m_szStockId ,PSTOCK_TYPE_INFO m_pStock) ;     
    long CorrectKline(Kline *pKline,long LineCount);
	float GetStockBlockRight(char *StockTypeCode);
    BOOL CheckBlockName(char *BlockName,char *BlockCode);


};
inline void CSuperviseSharesBlockData::SaveDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress,dwNumberOfBytesToFlush);
}
inline BYTE CSuperviseSharesBlockData::GetStockTypeNumber(char *m_szStockTypeCode)                          
{
	char mtemp[3];
	memcpy(mtemp,m_szStockTypeCode+4,2);
	mtemp[2]='\0';
	return ( BYTE )atoi(mtemp);
}

#endif // !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
