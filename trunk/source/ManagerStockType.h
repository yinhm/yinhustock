#if !defined(AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSuperviseSharesBlock : public CDialog
{

public:
	CSuperviseSharesBlock(CWnd* pParent = NULL);   
	enum {ZGB=0,LTG=1,OTHER=2	 };     

public:
	CImageList * m_pImageList;   
	CImageList * m_pImageListSmall; 
    CTaiShanDoc *m_pDoc;        
	BOOL m_bIsCalc;


    void FillToListCtrlHead();      
    void FillToListCtrlItem(STOCKTYPEINFO *pStockType);   
	void RefreshShowData();  

	//{{AFX_DATA(CSuperviseSharesBlock)
	enum { IDD = IDD_MANAGER_STOCKTYPE };
	CButtonST	m_ok;
	CButtonST	m_updatestocktype;
	CButtonST	m_refreshday;
	CButtonST	m_refresh;
	CButtonST	m_managerchoose;
	CButtonST	m_delete;
	CButtonST	m_add;
	CProgressCtrl	m_Progress;
	CListCtrl	m_MyList;                
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSuperviseSharesBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CSuperviseSharesBlock)
	virtual BOOL OnInitDialog();           
	afx_msg void OnImportStockType();     
	afx_msg void OnAddStockType();        
	afx_msg void OnUpdateStockType();      
	afx_msg void OnManagerChoose();       
	afx_msg void OnDeletestocktype();    
	afx_msg void OnRefresh();            
	afx_msg void OnExport();
	afx_msg void OnRefreshDayLine();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_)
