//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_TRANSACTFEE_H__01CF505C_6D12_4A13_B9F3_6CE51522D6C8__INCLUDED_)
#define AFX_TRANSACTFEE_H__01CF505C_6D12_4A13_B9F3_6CE51522D6C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransactFee.h : header file
//

#include "floatedit.h"
/////////////////////////////////////////////////////////////////////////////
// CTransactFee dialog

class CTransactFee : public CDialog
{
// Construction
public:
	float GetCommission(SymbolKind StockSymbol);
	double m_ShRate[5][5]; 
	double m_SzRate[5][5]; 
	unsigned m_uRow;
	unsigned m_uCol;

	CTransactFee(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransactFee)
	enum { IDD = IDD_TRANSACT_RATE };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CFloatEdit	m_fRate;
	CTabCtrl	m_TabCtrl;
	CMSFlexGrid	m_MsFlexGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransactFee)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransactFee)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickMsflexgrid();
	afx_msg void OnKillfocusEditFee();
	virtual void OnOK();
	afx_msg void OnClickTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSACTFEE_H__01CF505C_6D12_4A13_B9F3_6CE51522D6C8__INCLUDED_)
