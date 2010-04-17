#if !defined(AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_)
#define AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineTransferKline.h : header file
//

#include "StructKlineView.h"


class CTaiShanDoc;
class CTaiKlineTransferKline : public CCmdTarget
{
	DECLARE_DYNCREATE(CTaiKlineTransferKline)


public:
	bool m_bCalPowerToday;
	CReportData* m_pReportData;
	CString m_symbolOld;
	int m_stkKindOld;
	bool m_bUseMinute1Data;
	Kline m_klineOld;
	CTaiShanDoc * pDoc;
	CTaiKlineTransferKline();           
	CTaiKlineTransferKline(CBuySellList* pBSList);       

	virtual ~CTaiKlineTransferKline();

public:
	static float GetVolHs(TRADE_DETAIL_H_PER* pHs,TRADE_DETAIL_H_PER* pHsPre,int& nFlag);
	static void ComputePowerToday(Kline *pKlineToday,int nKline,powerArr &splitArr);
	static void ComputePower(Kline *pKlineHistory, int nKlineInOut, int nTypeTarget, powerArr& splitArr);
	
	static int ReadPower(CTaiShanDoc * pDoc,CString symbol,int stkKind,powerArr& splitArr);
	
	int OneTo5(Kline *pKline,int nEach,CString symbol,int stkKind);
	
	int TransferToday(bool bAnother,CString symbol,int stkKind,Kline* pKline,int nKline);
	
	void InPutValue(Kline* pKline,int i, int& j, bool bFrst,int& nCountDay,int nKindStock = 2);
	
	bool TransferKlineToday(CString symbol,int stkKind,Kline*& pKline,int& nKlineInOut,int nTypeTarget);
	bool TransferKlineHistory(CString symbol,int stkKind,Kline*& pKlineHistory,int& nKlineInOut,int nTypeTarget,bool bToday = true,bool bSplit = false);
  

	int CaclKline1Minu(Kline*& pKline,CString name,int stkKind, int nMinu);
	
	
	//{{AFX_VIRTUAL(CTaiKlineTransferKline)
	//}}AFX_VIRTUAL


protected:
	CBuySellList* pBuySellList; 
	bool m_bRemoveBuySellList;

	int CaclKline1MinuIndex(Kline*& pKline,CString name,int stkKind, int nMinu);

	
	//{{AFX_MSG(CTaiKlineTransferKline)
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	static float CalcPowerAfterPer(float fIn, POWER &pwr);
	static float CalcPowerPer(float fIn,POWER& pwr);
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_)
