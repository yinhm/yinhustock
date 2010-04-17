
#if !defined(AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_)
#define AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSharesBigBillData  
{
public:
	CSharesBigBillData();
	virtual ~CSharesBigBillData();
    HANDLE m_hFile,m_hFileMap;               
	BIG_LISTHEAD *m_pBigBillHead;                    
	BYTE  *m_pbData;                          
	BIG_LIST  *m_pMapData;                        
	
public:
	BOOL InitRealTimeHsData();              
    BOOL InitRealTimeHsDataEmpty();
    BOOL InitRealTimeHsDataExist();

	BOOL AddRealTimeHsData(PBIG_LIST & pCjHs);  
	BOOL ClearRealTimeHsData();             
	BOOL SetBigDataForSetup(float *pBigData,float *pBigMaxData,float *CjmxType);  
	BOOL SetBigDataForSetupGB(float *pBigData,float *pBigMaxData,float *CjmxType);
	BOOL CalcBigBillData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport); 
	BOOL CalcBigBillDataForType(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport);
    BOOL CalcBigBillBigData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport); 
	BOOL CalcBigBillDataForTypeGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB); 
    BOOL CalcBigBillBigDataGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB); 
	void CalcRealTimeHsBigData(BIG_LIST *pCjHs);    
	BOOL GetBigBillData(int index,PBIG_LIST &pCjmx);   
	void GetParamData(float *pBigData,float *pBigMaxData,float *CjmxType);   
    void GetParamDataGB(float *pBigData,float *pBigMaxData,float *CjmxType);  
	void SetIsCalcForBig(BOOL *IsCalc);
	void SetBigBillBigType(BOOL IsGB){m_pBigBillHead->IsGbjg=IsGB;}


	BOOL CalcBigBillHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData,int nTotalCount); 
    void CalcBigBillHistoryDataForType(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1);
    void CalcBigBillBigHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1);
	void CalcBigBillHistoryDataForTypeGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB);
	void CalcBigBillBigHistoryDataGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB);


};

#endif // !defined(AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_)
