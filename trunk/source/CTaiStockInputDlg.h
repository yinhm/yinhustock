#if !defined(AFX_SJLSTOCKINPUTDLG_H__B444AAE1_EE80_11D3_BB59_0000E8593F1A__INCLUDED_)
#define AFX_SJLSTOCKINPUTDLG_H__B444AAE1_EE80_11D3_BB59_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiShanDoc;

class CTaiStockInputDlg : public CDialog
{

public:
	int nKind;
	int nMarketType;
	void SetStockInfo(CString Code,CString Name,CString Pyjc);
	void SetEditFlag(BOOL bEdit);
	BOOL GetStockType();
	void SetStockType(BOOL bShangHai);
	CTaiStockInputDlg(CWnd* pParent = NULL);  


	//{{AFX_DATA(CTaiStockInputDlg)
	enum { IDD = IDD_SharesSymbol_INPUT };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CString	m_strStockCode;
	CString	m_strStockName;
	CString	m_strStockPyjc;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CTaiStockInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL


protected:
	CTaiShanDoc * GetDocument();


	//{{AFX_MSG(CTaiStockInputDlg)
	afx_msg void OnChangeStockcode();
	afx_msg void OnChangeStockname();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_StockPyjc;
	CString m_StockName;
	CString m_StockSymbol;
	BOOL m_bEditFlag;
	BOOL m_bStockType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SJLSTOCKINPUTDLG_H__B444AAE1_EE80_11D3_BB59_0000E8593F1A__INCLUDED_)
