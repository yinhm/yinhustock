#if !defined(AFX_EDITINVEST_H__EADB9E4E_AC95_431D_9569_FE5B72117ED6__INCLUDED_)
#define AFX_EDITINVEST_H__EADB9E4E_AC95_431D_9569_FE5B72117ED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FloatEdit.h"

class CEditInvest : public CDialog
{

public:
	int m_nKind;
	double m_RefFee;
	double m_RefPrice;
	double m_RefShares;
	unsigned m_SelectTab;
	unsigned m_Operation;
	CTime m_RefTime;
	int m_RefIn;
	CString m_Name;

	BOOL m_bInit;

	double m_ShRate[5][5]; 
	double m_SzRate[5][5]; 
	double m_CyRate[5][5]; 


	CEditInvest(CWnd* pParent = NULL); 


	//{{AFX_DATA(CEditInvest)
	enum { IDD = IDD_EDIT_INV2 };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CEdit	m_SymbolControl;
	CDateTimeCtrl	m_DateTimeCtrl;
	CFloatEdit	m_Value;
	CFloatEdit	m_Shares;
	CFloatEdit	m_Fee;
	CFloatEdit	m_Price;
	CTabCtrl	m_TabCtrl;
	CString	m_Symbol;
	CTime	m_time;
	int		m_In;
	float	m_PositiveFee;
	float	m_PositivePrice;
	float	m_PositiveShares;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CEditInvest)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:

	
	//{{AFX_MSG(CEditInvest)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnChangeEditShares();
	afx_msg void OnChangeEditPrice();
	afx_msg void OnChangeEditSymbol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITINVEST_H__EADB9E4E_AC95_431D_9569_FE5B72117ED6__INCLUDED_)
