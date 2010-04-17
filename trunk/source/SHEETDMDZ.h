//
// Code Contradistingush header file
//
////////////////////////////////////////////////
// All file are recoded by Xia Qiankun on 2000/2
//
////////////////////////////////////////////////////

#ifndef _CSJLCODECONTRADISTINGUISHPAGE_H_
#define _CSJLCODECONTRADISTINGUISHPAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETDMDZ.h : header file
//
#include "resource.h"

class CTaiShanDoc;

class CTaiCodeContradistinguishPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiCodeContradistinguishPage)

public:
	CTaiCodeContradistinguishPage();
	~CTaiCodeContradistinguishPage();
protected:

	//{{AFX_DATA(CTaiCodeContradistinguishPage)
	enum { IDD = IDD_DMDZ };
	CEdit	m_EditPhonetic;
	CEdit	m_EditCode;
	CEdit	m_EditName;
	int		m_RadioSh;
	CListCtrl m_ctrlSymbolCompare;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CTaiCodeContradistinguishPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:
	BOOL DeleteOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc);
	BOOL ModifyOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc);
	BOOL AddOneStockInfo(CString strStockCode,CString strStockName,CString strStockPyjc);
	void StockTypeUpdate(CReportData  *Cdat);
	CTaiShanDoc * GetDocument();
	void InitListCtrl();

	//{{AFX_MSG(CTaiCodeContradistinguishPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadiosz();
	afx_msg void OnRadiosh();
	afx_msg void OnModify();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnChangeName();
	afx_msg void OnClickSymbolcompare(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKeydownSymbolcompare(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bDelete;
	BOOL m_bModify;
	BOOL m_bAdd;
	int m_nOldSel;
	CBrush m_SelfBrush;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // end of _CSJLCODECONTRADISTINGUISHPAGE_H_