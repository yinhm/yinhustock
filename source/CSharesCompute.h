#if !defined(AFX_STOCKDATACALC_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKDATACALC_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StructKlineView.h"

#define MAX_COUNT_CDAT1    30

class CTaiShanDoc;
class CSharesCompute  
{
public:
	CSharesCompute();
	virtual ~CSharesCompute();
private:
	CTaiShanDoc   *m_MainDocument;
public:
	void AddDataCdat1(CReportData* p);
	SymbolKindArr m_dataArr[2];
	static CString GetIndexSymbol(int nKind);
	static int GetMarketKind(int nKind);
	void SetMainDocument(CTaiShanDoc *m_MainDoc ) { m_MainDocument = m_MainDoc;}   
	void chk_rdp(CReportData  *Cdat,float NewPrice);                                    
	void UpdateHs(CReportData  *Cdat, short Curr_Min);                                   
	void SendCuoHeDataToDialog(CReportData *Cdat);                                 
	short GetStockMinute(time_t time,int mode);                               
    void StockDataPowerUpdate(POWER_TOTAL_STRUCTEx * m_GpPower);                     

#ifdef WIDE_NET_VERSION
	void WideStockDataUpdate(RCV_WIDOFEREPORT_STRUCTEx* m_GpHqReport);                                 
	void WideStockItemUpdate(CReportData  *Cdat,RCV_WIDOFEREPORT_STRUCTEx* m_GpHqReport);       
    void WideStockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute);                            
#else
	void StockDataUpdate(RCV_REPORT_STRUCTEx* m_GpHqReport);                                
	void StockItemUpdate(CReportData  *Cdat,RCV_REPORT_STRUCTEx* m_GpHqReport);    
    void StockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute);                         

#endif	

public:
    int CheckStockDaytime(DAY_TOTAL_STRUCTEx* m_GpDay, int index);
	void StockDataDayUpdate(DAY_TOTAL_STRUCTEx* m_GpDay);
};

#endif // !defined(AFX_STOCKDATACalc_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
