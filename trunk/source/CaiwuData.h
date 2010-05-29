
#if !defined(AFX_CAIWUDATA_H__34161F41_8E06_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_CAIWUDATA_H__34161F41_8E06_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCaiwuData : public CPropertyPage
{
	DECLARE_DYNCREATE(CCaiwuData)
public:
	CCaiwuData();
	virtual ~CCaiwuData();

protected:
	CTaiShanDoc* pDoc;
	int m_nKind;

	//{{AFX_DATA(CCaiwuData)
	enum { IDD = IDD_CAIWUINOUT };

	CButtonST	m_zdtq;
	CButtonST	m_button6;
	CButtonST	m_button5;
	CProgressCtrl	m_progress;
	CListCtrl	m_listcw;
	CString	m_ShowStatus;
	//}}AFX_DATA

public:
	static BOOL ImportCwData(CString FileName);
	void OutCaiWuData(CString PathFileName);

	//{{AFX_VIRTUAL(CCaiwuData)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

protected:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//{{AFX_MSG(CCaiwuData)
	afx_msg void OnChangeStockSymbol();
	afx_msg void OnExport();
	afx_msg void OnImport();
	afx_msg void OnZdtq();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CAIWUDATA_H__34161F41_8E06_11D4_9426_0080C8E20736__INCLUDED_)
