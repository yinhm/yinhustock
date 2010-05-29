
#if !defined(AFX_CHUQUANDATA_H__34161F42_8E06_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_CHUQUANDATA_H__34161F42_8E06_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChuquanData : public CPropertyPage
{
	DECLARE_DYNCREATE(CChuquanData)
public:
	CChuquanData();
	virtual ~CChuquanData();

protected:
	CTaiShanDoc* pDoc;
	int m_nKind;

public:
	void OutChuQuanData(CString FilePath);
	BOOL IsAlreadyChuQuan(int nSeconds);
	void InsertItem(int nItem,CString strNo,POWER& nPower);
	int GetChuQuanInfo(CString strStockCode,PSplit &pSplit);
	void EnableButton(BOOL bEnable);

	//{{AFX_DATA(CChuquanData)
	enum { IDD = IDD_POWER2 };
	CButtonST	m_button22;
	CButtonST	m_button12;
	CButtonST	m_button7;
	CButtonST	m_button6;
	CButtonST	m_button4;
	CListCtrl	m_ctrlChuQuan;
	CString	m_strStockCode;
	//}}AFX_DATA
private: 
	int m_nChuQuanKind[80];


	//{{AFX_VIRTUAL(CChuquanData)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:
	BOOL InstallPowerData(CString FilePath);
	void DeleteChuQuanInfo(CString strStockCode, int nWhichItem);
	void ModifyChuQuanInfo(CString strStockCode, int nWhichItem,POWER *pPower);
	void AddChuQuanInfo(CString strStockCode, POWER *pPower);

protected:
	//{{AFX_MSG(CChuquanData)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeStockSymbol();
	afx_msg void OnAddNew();
	afx_msg void OnModifyCq();
	afx_msg void OnDeleteInfo();
	afx_msg void OnImport();
	afx_msg void OnExport();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_CHUQUANDATA_H__34161F42_8E06_11D4_9426_0080C8E20736__INCLUDED_)
