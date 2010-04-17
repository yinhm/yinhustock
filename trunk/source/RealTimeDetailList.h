#if !defined(AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_)
#define AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CRealTimeDetailList : public CDialog
{

public:
    CRealTimeDetailList(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,BOOL IsF1);
    CDetailGridCtrl *m_pGrid;
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
	CCellID m_CellTitleCommand;       
	CCellRange VisibleCells;
    int m_VisibleRowCount;
    CTaiShanDoc *m_pDoc;
	CTaiShanKlineShowView *m_pView;

    BOOL m_IsF1;
	BOOL m_bFirst;
	BOOL m_bFirstDisplay;

	void InitAllTitle();    
    int InitGridCtrl();   
	BOOL InitGridContent(); 
    void Show_1Min(BOOL IsFirstDisplay=FALSE);
    void Show_5Min(BOOL IsFirstDisplay=FALSE);

	//{{AFX_DATA(CRealTimeDetailList)
	enum { IDD = IDD_DETAIL_HS };
	
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CRealTimeDetailList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual void OnCancel();
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CRealTimeDetailList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_)
