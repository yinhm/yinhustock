
#if !defined(AFX_MODIFYSTOCKTYPE_H__BD1C43A2_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MODIFYSTOCKTYPE_H__BD1C43A2_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_ADDSTOCK WM_USER+1200

class CModifyStockType : public CDialog
{

public:
	CModifyStockType(CWnd* pParent = NULL);   
	CModifyStockType(PSTOCKTYPEINFO &pStockTypeInfo,BOOL IsAddStockType=TRUE,CWnd* pParent = NULL);   // standard constructor
	enum {ZGB=0,LTG=1,OTHER=2	 };

public:
	CImageList * m_pImageList;
	CImageList * m_pImageListSmall;
	CTaiShanDoc *m_pDoc;
	STOCKTYPEINFO *m_pStockTypeInfo;
	BOOL m_bIsAddNewStockType;
	float m_fTotalRight;
	int  m_nTotalCount;
	BOOL m_bIsChanged;
	BOOL m_bIsCalc;

public:
	void FillToListCtrlHead();
	void FillToListCtrlItem(CReportData *pCdat);
	void RefreshShowData();
	void CheckStockCount();





	//{{AFX_DATA(CModifyStockType)
	enum { IDD = IDD_MODIFY_STOCKTYPE };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CButtonST	m_refresh;
	CButtonST	m_delete;
	CButtonST	m_add;
	CListCtrl	m_MyList;
	CString m_sStockTypeCode;
	CString m_sStockTypeName;
	CProgressCtrl	m_Progress;
	int  m_nRightType;
	//}}AFX_DATA

protected:
	//{{AFX_VIRTUAL(CModifyStockType)
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CModifyStockType)
	afx_msg void OnAddStock();
	afx_msg void OnDeleteStock();
	afx_msg void OnChangeStockTypeRight();
	afx_msg void OnUpdateOk();
	afx_msg void OnRefreshHistoryData();
	afx_msg void OnChangedName();
	afx_msg LRESULT OnAddStockCode(WPARAM wParam, LPARAM lParam); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MODIFYSTOCKTYPE_H__BD1C43A2_5D50_11D4_8829_00400552E583__INCLUDED_)
