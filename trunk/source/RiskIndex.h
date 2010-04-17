//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_RISKINDEX_H__8E0814E1_F2E5_4A3A_AC08_D41054FEF4AD__INCLUDED_)
#define AFX_RISKINDEX_H__8E0814E1_F2E5_4A3A_AC08_D41054FEF4AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RiskIndex.h : header file
//
#include "PlayGif.h"
/////////////////////////////////////////////////////////////////////////////
// CRiskIndex dialog
struct StockRisk
{
	char chSymbol[8];
	char chName[16];
	double Weight;//风险方向上，投资的权重
	double Beta;  //个股的Beta值
};

class CRiskIndex : public CDialog
{
// Construction
public:
	double GetRiskIndex(CTime& nStart, CTime& nEnd);
	CRiskIndex(CWnd* pParent = NULL);   // standard constructor
	~CRiskIndex();

// Dialog Data
	//{{AFX_DATA(CRiskIndex)
	enum { IDD = IDD_RISKINDEX };
	CTabCtrl	m_TabCtrl;
	CTime	m_StartDate;
	CTime	m_EndDate;
	//}}AFX_DATA
	CArray<StockRisk,StockRisk&> m_StockRiskArray;
	double m_dRiskIndex;
	SymbolKindArr m_StockChoose;
	CPlayGif *m_Gif;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiskIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OutputToList(double dRiskIndex);
	void QuickSort(CArray<StockRisk,StockRisk&>& StockRiskArray,int low, int high,bool bAscend);
	int  GetOutstandingMatrix(int nDay,double* &pOutstandingMat);
	int  GetPriceMatrix( CTime& nStart,CTime& nEnd,double* &pPriceMat);
	BOOL PreProcessData(double *pPriceMat,double *pOutstandingMat,int nStock, int nDay,int nCriterion);
	double GetMaxEigVal(double *pCovMat,int nDay, int nStock);
	void GetCovMat(double *pPriceMat,int nDay,int nStock,double *&dCovMat);
	double GetMarketPortfolioStd(double *pPriceMat, double *pOutstandingMat,int nDay, int nStock);
	// Generated message map functions
	//{{AFX_MSG(CRiskIndex)
	afx_msg void OnCalc();
	afx_msg void OnAddstock();
	virtual BOOL OnInitDialog();
	afx_msg void OnSeldefine();
	afx_msg void OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RISKINDEX_H__8E0814E1_F2E5_4A3A_AC08_D41054FEF4AD__INCLUDED_)
