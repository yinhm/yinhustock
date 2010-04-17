#if !defined(AFX_LMBMOVECHENBEN_H__45E004E6_9E5E_11D3_96DF_0080C8E1242B__INCLUDED_)
#define AFX_LMBMOVECHENBEN_H__45E004E6_9E5E_11D3_96DF_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineMoveChenben.h : header file
//
#include "BjChenBenSetting.h"




class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiKlineShowKline;

class CTaiKlineMoveChenben : public CObject
{
	DECLARE_DYNCREATE(CTaiKlineMoveChenben)

	CTaiKlineMoveChenben();         


public:
	

	CArray<BjChenBenSetting,BjChenBenSetting&> m_chengBenBeforArr;
	CArray<BjChenBenSetting,BjChenBenSetting&> m_chengBenInsideArr;
	void WriteOutSettings();
	void ReadInSettings();


	int m_nShowKind;

	static void GetMoveCBPerFoot2(Kline* pKline,float* cnp,float ltp,float fMax,float fMin,int nFoot ,float fRate,int n,float fRuler);
	CString m_symbol;
	int m_stkKind;
	bool m_bDayKline;
	Kline* m_pKlineOther;
	int m_numkline;
	CTaiShanDoc* pDoc ;		
	CTaiShanKlineShowView* pView ;	
	CTaiKlineShowKline* pDoK ;
	CTaiKlineMoveChenben(CTaiKlineShowKline* pDoKline,CTaiShanKlineShowView* pViewIn,CTaiShanDoc* pDocIn);           // protected constructor used by dynamic creation

	virtual ~CTaiKlineMoveChenben();

public:
	void ShowFxjMoveCB(CDC *pDC);
	void ShowCommerceCB(CDC* pDC,int nShowVirtualLine = 0);
	static int GetRuler(float fMax, float fMin,float& fRuler);
	static void GetMoveCBPerFoot(Kline* pKline,float* cnp,float ltp,float fMax,float fMin,int nFoot = 0,float fRate=1);
	static void GetMaxMin(Kline* pKline,int nKline,float& fMax,float& fMin);
	void BuySellFiguer(CDC* pDC);
	void ShowMoveCB(CDC* pDC,int nShowVirtualLine = 0);
	

	//{{AFX_VIRTUAL(CTaiKlineMoveChenben)
	//}}AFX_VIRTUAL


protected:
	void ShowVol(CDC *pDC,int nShowVirtualLine);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBMOVECHENBEN_H__45E004E6_9E5E_11D3_96DF_0080C8E1242B__INCLUDED_)
