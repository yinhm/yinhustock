//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#include "CTaiScreenTest.h"
#if !defined(AFX_SCREENSTOCKTESTBUYSELLRESULT_H__C4102B02_63A3_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_SCREENSTOCKTESTBUYSELLRESULT_H__C4102B02_63A3_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreenTestBuySellResult.h : header file
//
struct PROFIT_RESULT{
    CString StockName;
	float profit[10];
	int index;
};


class CTaiScreenTestBuySellResult : public CDialog
{

public:
	Kline* m_pKline;
	int m_nKline;

	void Quick_Sort(int first,int last,int index);
	void SortItem(int index);
	void ShowResult();
	CTaiScreenTestBuySellResult(CWnd* pParent = NULL);  
    CTaiScreenTest *GetParenInfo;
	CTaiShanDoc* pDoc;
	SCREEN_RESULT *chooseresult;
	CArray<PROFIT_RESULT*,PROFIT_RESULT*&> m_prosult;
	PROFIT_RESULT *profitresult;
	bool isaccelerate[10];
	BOOL m_bRecursion;
	RECT     m_rectsample;
	// Dialog Data
	//{{AFX_DATA(CTaiScreenTestBuySellResult)
	enum { IDD = IDD_7_TRADE_DETAIL_RESULT };
	CButtonST	m_ok;
	CTabCtrl	m_bTab;
	CMSFlexGrid	m_grid;
	CString	m_sResult;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTaiScreenTestBuySellResult)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:
	int m_SelNum;


	//{{AFX_MSG(CTaiScreenTestBuySellResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMsflexgrid1();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnDestroy();
	afx_msg void OnRowColChangeMsflexgrid1();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnOK();
	afx_msg void OnClose();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENSTOCKTESTBUYSELLRESULT_H__C4102B02_63A3_11D4_970B_0080C8D6450E__INCLUDED_)
