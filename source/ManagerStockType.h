
#if !defined(AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSuperviseSharesBlock : public CDialog
{
public:
	CSuperviseSharesBlock(CWnd* pParent = NULL);

protected:
	enum { IDD = IDD_MANAGER_STOCKTYPE };

	CButtonST m_ok;
	CButtonST m_updatestocktype;
	CButtonST m_refreshday;
	CButtonST m_refresh;
	CButtonST m_managerchoose;
	CButtonST m_delete;
	CButtonST m_add;

	CListCtrl m_MyList;
	CProgressCtrl m_Progress;

	enum { ZGB = 0, LTG = 1, OTHER = 2 };

	CTaiShanDoc* m_pDoc;
	CImageList* m_pImageList;
	CImageList* m_pImageListSmall;

	BOOL m_bIsCalc;

protected:
	void FillToListCtrlHead();
	void RefreshShowData();

public:
	void FillToListCtrlItem(STOCKTYPEINFO* pStockType);



	//{{AFX_DATA(CSuperviseSharesBlock)
	//}}AFX_DATA

protected:
	//{{AFX_VIRTUAL(CSuperviseSharesBlock)
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_VIRTUAL

protected:
	afx_msg void OnClose();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	//{{AFX_MSG(CSuperviseSharesBlock)
	afx_msg void OnImportStockType();
	afx_msg void OnAddStockType();
	afx_msg void OnUpdateStockType();
	afx_msg void OnManagerChoose();
	afx_msg void OnDeletestocktype();
	afx_msg void OnRefresh();
	afx_msg void OnExport();
	afx_msg void OnRefreshDayLine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_)
