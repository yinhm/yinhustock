#if !defined(AFX_SCREENSTOCKTEST_H__62BF6202_63BF_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_SCREENSTOCKTEST_H__62BF6202_63BF_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreenTest.h : header file
//
#include "FloatEdit.h"

#include "CTaiScreenParent.h"

class CTaiScreenTest : public CTaiScreenParent
{

public:
	int GetIndexKline(Kline*& pKline);
	int GetPeriodTest();
	void EnableControl(bool bEnable = true);
	bool m_bExecuteCalc;
	void RefreshJishuTreeCtrl();
	CTaiScreenTest(CWnd* pParent = NULL);   


	//{{AFX_DATA(CTaiScreenTest)
	enum { IDD = IDD_6_TJXG_TEST };
	CButtonST	m_cancel;
	CButtonST	m_ok;
	CButtonST	m_button1;
	CButtonST	m_deleteStocks;
	CButtonST	m_addStocks;
	CFloatEdit	m_floatEdit2;
	CFloatEdit	m_floatEdit1;
	CButtonST	m_buttonExpl;
	CProgressCtrl	m_progress;
	CListCtrl	m_liststockrange;
	int		m_stock_number;
	CTime	m_starttime;
	CTime	m_endtime;
	float	m_mbly;
	int		m_mbzq;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CTaiScreenTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:
	void NotifySubclass();


	//{{AFX_MSG(CTaiScreenTest)
	afx_msg void OnStockTest();
	virtual BOOL OnInitDialog();
	afx_msg void OnAddStocksParent();
	afx_msg void OnDeleteStocks();
	afx_msg void OnButton2();
	afx_msg void OnStopLose();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnKeydownListAddStocks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENSTOCKTEST_H__62BF6202_63BF_11D4_970B_0080C8D6450E__INCLUDED_)
