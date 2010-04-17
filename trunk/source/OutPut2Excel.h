#if !defined(AFX_OUTPUT2EXCEL_H__C89800A1_3053_11D3_BF3B_0000E86564F7__INCLUDED_)
#define AFX_OUTPUT2EXCEL_H__C89800A1_3053_11D3_BF3B_0000E86564F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutPut2Excel.h : header file
//

#include "afxdb.h"
/////////////////////////////////////////////////////////////////////////////
// COutPut2Excel dialog

class COutPut2Excel : public CPropertyPage
{
	DECLARE_DYNCREATE(COutPut2Excel)

// Construction
public:
	COutPut2Excel();
	~COutPut2Excel();

	void InsertExcel(CDatabase& database,CString StockSymbol, TAI_SHAN_DATA& tianlong);
	void CreateExcel(CDatabase& database);
	void Put2Excel(CString filename,int length);


// Dialog Data
	//{{AFX_DATA(COutPut2Excel)
	enum { IDD = IDD_OUTPUT2EXCEL };
	CProgressCtrl	m_Progress;
	CString	m_StockMarket;
	CTime	m_BeginDate;
	CTime	m_EndDate;
	CString	m_OutFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COutPut2Excel)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COutPut2Excel)
	afx_msg void OnSearch();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateEdit1();
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropdownDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepicker2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDropdownDatetimepicker2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboMarket();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUT2EXCEL_H__C89800A1_3053_11D3_BF3B_0000E86564F7__INCLUDED_)
