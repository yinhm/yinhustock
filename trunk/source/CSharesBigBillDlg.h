#if !defined(AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_)
#define AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_
// by 俞明录
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MOUSE_ALL  7001
#define MOUSE_COL  7002
#define MOUSE_ROW  7003
#define MOUSE_CELLS 7004
#define MOUSE_NOTHINGS 7005

class BCridCtrl;
class CSharesBigBillDlg : public CDialog
{

public:
	CSharesBigBillDlg(CWnd* pParent = NULL);  
	~CSharesBigBillDlg();

public:
	CBGridCtrl *m_pGrid;  
 	int     m_nCols; 
	int		m_nRows; 
	int		m_nFixCols;   
	int		m_nFixRows;   
	int     m_nBigBillCount; 
	int     m_nRealRow;   

	BOOL	m_bEditable;  
	BOOL	m_bListMode;  
	BOOL	m_bSelectable;
	BOOL	m_bReadOnly;   
	CRect   m_barRect;
	int     m_nStartColumn;  
	int     m_nVisibleRowCount;  
	CStringArray m_ColumnTitleArray; 
	CCellID m_CellTitleCommand;        //
	CImageList * m_pImageList;    
	CImageList * m_pImageListSmall; 
	CCellRange VisibleCells;
    int m_VisibleRowCount;
	BOOL m_bIsCalc;




    CTaiShanDoc *m_pDoc;
	void InitAllTitle();    
    int InitGridCtrl();   
	BOOL InitGridContent();  
	void FillToListCtrlHead(); 
	void FillToListCtrlItem(char *Id,int nParam,BOOL StockType); 
	void DisplayInfomation();  
	void InitBigBillData();       
	void AddGridRow(int CurrentRow );  
	void EnableVisibleChildren();     
	void ExpandDialog(BOOL nDisplayTj,BOOL nDisplayAll); 
	void GetBigBillDataToday();
    void GetBigBillDataHistory();
	void SetGridRealRowData();
	void InitBigBillFileData();
    LRESULT OnGridColorChange(WPARAM wParam, LPARAM lParam);


	//{{AFX_DATA(CSharesBigBillDlg)
	enum { IDD = IDD_DIALOG6 };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CButtonST	m_setup;
	CButtonST	m_delete;
	CButtonST	m_calc;
	CButtonST	m_add;
	CEdit	m_edit;
	CDateTimeCtrl	m_EndCtrl;
	CDateTimeCtrl	m_StartCtrl;
	CListCtrl	m_MyList;
	BOOL	m_Check1;
	BOOL	m_Check2;
	BOOL	m_Check3;
	BOOL	m_Check4;
	int		m_Radio1;
	CTime	m_Start;
	CTime	m_End;
	BOOL	m_check5;
	BOOL	m_check6;
	BOOL	m_stop;
	BOOL	m_bSaveData;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSharesBigBillDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CSharesBigBillDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDelete();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCalc();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnSetup();
	afx_msg void OnCheck1();
	virtual void OnCancel();
	virtual void OnAdd();
	afx_msg void OnStop();
	afx_msg void OnSaveBigBillData();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAddStock(WPARAM wParam, LPARAM lParam); //增加股票至LISTCTRL

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_)
