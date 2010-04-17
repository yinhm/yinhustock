#if !defined(AFX_SYMBOLCONTRAST_H__A02AA9C2_35E7_4A57_A494_69EAB93D9228__INCLUDED_)
#define AFX_SYMBOLCONTRAST_H__A02AA9C2_35E7_4A57_A494_69EAB93D9228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolContrast.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSymbolContrast dialog

class CSymbolContrast : public CPropertyPage
{
	DECLARE_DYNCREATE(CSymbolContrast)

// Construction
public:
	BOOL ConDeleteOneStockInfo(CString strCode,CString strName,CString strHypy,int nKind);
	CSymbolContrast();
	~CSymbolContrast();
	CTaiShanDoc *pDoc;

// Dialog Data
	//{{AFX_DATA(CSymbolContrast)
	enum { IDD = IDD_CONTRAST_SYMBOL };
	CButtonST	m_button22;
	CButtonST	m_button12;
	CButtonST	m_button4;
	CListCtrl	m_ctrlSymbolCompare;
	CTabCtrl	m_Tabctrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSymbolContrast)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL ModifyOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc,int nKind);
	void InitListCtrl(BOOL bShanghai);
	BOOL AddOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc,int nKind);
	// Generated message map functions
	//{{AFX_MSG(CSymbolContrast)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangezqsc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddNew();
	afx_msg void OnEdit();
	afx_msg void OnDelete();
	afx_msg void OnKeydown1000(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bModify;
	BOOL m_bDelete;
	BOOL m_bAdd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLCONTRAST_H__A02AA9C2_35E7_4A57_A494_69EAB93D9228__INCLUDED_)
