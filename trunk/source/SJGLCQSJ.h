#if !defined(AFX_SJGLCQSJ_H__804D1367_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
#define AFX_SJGLCQSJ_H__804D1367_B12B_11D1_83E6_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SJGLCQSJ.h : header file
//

#include "resource.h"
#include "CTaiShanDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanPage dialog

class CTaiChuQuanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiChuQuanPage)

// Construction
public:
	int GetChuQuanInfo(CString strStockCode,PSplit &pSplit);
	CTaiChuQuanPage();
	~CTaiChuQuanPage();

// Dialog Data
	//{{AFX_DATA(CTaiChuQuanPage)
	enum { IDD = IDD_SJGLCQSJ };
	CListCtrl	m_ctrlChuQuan;
	CString		m_strStaticName;
	CString		m_strStockCode;
	//}}AFX_DATA

private:	
	int m_nChuQuanKind[80];

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTaiChuQuanPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation


protected:
	void EnableButton(BOOL bEnable);
	void DeleteChuQuanInfo(CString strStockCode, int nWhichItem);
	void ModifyChuQuanInfo(CString strStockCode, int nWhichItem,POWER *pPower);
	void AddChuQuanInfo(CString strStockCode,POWER *pPower);
	void InsertItem(int nItem,CString strNo,POWER nPower);
	CTaiShanDoc * GetDocument();

	BOOL IsAlreadyChuQuan(int nSeconds);

	// Generated message map functions
	//{{AFX_MSG(CTaiChuQuanPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditKey();
	afx_msg void OnDeleteInfo();
	afx_msg void OnAddItem();
	afx_msg void OnModifyCq();
	afx_msg void OnButtonLeadout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SJGLCQSJ_H__804D1367_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
