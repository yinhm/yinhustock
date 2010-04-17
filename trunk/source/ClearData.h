#if !defined(AFX_CLEARDATA_H__9F87680D_1383_4800_AA57_00156A61E657__INCLUDED_)
#define AFX_CLEARDATA_H__9F87680D_1383_4800_AA57_00156A61E657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// by ÓáÃ÷Â¼

class CClearData : public CPropertyPage
{
	DECLARE_DYNCREATE(CClearData)


public:
	int m_nKind;
	
	void Clear(int nStock,bool bShanghai,bool bDayKline);
	BOOL m_Shanghai,m_Shenzhen;
	bool m_warning;
	int m_nFlag;
	CTaiShanDoc* pDoc;
	CArray<BOOL,BOOL> m_stockrange;
	CClearData();
	~CClearData();


	//{{AFX_DATA(CClearData)
	enum { IDD = IDD_CLEAR_DATA };
	CButtonST	m_cancel;
	CButtonST	m_button1;
	CListCtrl	m_listfw;
	CProgressCtrl	m_progress;
	BOOL	m_Invacode;
	BOOL	m_Price;
	BOOL	m_Invadate;
	BOOL	m_Continuous;
	BOOL	m_Chaos;
	BOOL	m_Date;
	BOOL	m_Cjiaoliang;
	CTime	m_clear_start;
	CTime	m_clear_end;
	CString	m_Smbl;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CClearData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:

	//{{AFX_MSG(CClearData)
	virtual BOOL OnInitDialog();
	afx_msg void Onzdsd();
	afx_msg void OnClearData();
	afx_msg void OnSelchangezlfs();
	afx_msg void OnClearOneStock();
	afx_msg void OnChangeStockSymbol();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnButton4To6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLEARDATA_H__9F87680D_1383_4800_AA57_00156A61E657__INCLUDED_)
