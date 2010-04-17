#if !defined(AFX_TJXGNEW_H__937DFD01_1696_11D4_941E_0080C8E20736__INCLUDED_)
#define AFX_TJXGNEW_H__937DFD01_1696_11D4_941E_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreen.h : header file

#include "CTaiScreenParent.h"

class CTaiScreen : public CTaiScreenParent
{

public:
	bool m_bExecuteCalc;
	void AddCComboBoxString(CComboBox& comboBox,bool bInserFirst= false);
	void RefreshJishuTreeCtrl();
	void ShowSetCompute(bool bShow = false);
	CTaiScreen(CWnd* pParent = NULL);  
    CArray<BOOL,BOOL> m_stockrange;

protected:
	bool m_bShowSetCompute;
	SymbolKindArr m_choose_result_save;
protected:
	int  FindStr(SymbolKindArr* array,SymbolKind& str);
// Dialog Data
	//{{AFX_DATA(CTaiScreen)
	enum { IDD = IDD_SHOWTJXG };
	CButtonST	m_button1;
	CButtonST	m_close1;
	CDateTimeCtrl	m_comTmBegin;
	CProgressCtrl	m_progress;
	CButtonST	m_buttonExpl;
	CDateTimeCtrl	m_conTmEnd;
	CListBox	m_aggregation_list;
	CComboBox	m_save_list;
	CListCtrl	m_liststockrange;
	int		m_choosed_number;
	int		m_selected_number;
	int		m_stock_number;
	int		m_aggregation2;
	int		m_operator;
	int		m_aggregation1;
	int		m_aggregation3;
	CTime	m_tmEnd;
	int		m_nKindScreen;
	CButtonST m_btExec;
	CButtonST m_btAddS;
	CButtonST m_btDeleteS;
	CTime	m_tmBegin;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTaiScreen)
	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:
	void NotifySubclass();


	//{{AFX_MSG(CTaiScreen)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChoosestock();
	virtual void OnCancel();
	afx_msg void OnEqual();
	afx_msg void OnToGrid();
	afx_msg void OnButton2();
	afx_msg void OnButton1();
	afx_msg void OnRadio1();
	afx_msg void OnRadioHistory();
	afx_msg void OnDeleteStocks2();
	afx_msg void OnAddStocksParent();
	afx_msg void OnButtonSaveResult();
	afx_msg void OnKeydownStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnClose1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool CalcEach(IndexDataInfo& index_save2,CString StockId,int stkKind, int nCount, Kline *pKline,CTime& tmB,CTime& tmE, CBuySellList *pList);
	void EnableControl(bool bEnable = true);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TJXGNEW_H__937DFD01_1696_11D4_941E_0080C8E20736__INCLUDED_)
