#if !defined(AFX_DAYLINESELECT_H__B46517A2_88B1_11D4_882A_00400552E583__INCLUDED_)
#define AFX_DAYLINESELECT_H__B46517A2_88B1_11D4_882A_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDayLineSelect : public CDialog
{

public:
	CImageList * m_pImageListSmall;

	CDayLineSelect(CWnd* pParent = NULL);  
	CDayLineSelect(UINT uiSelectDayLineType,CWnd* pParent = NULL);  
	void InsertDayLineToList(BOOL IsMonthLine);
	void FillToListCtrlHead();
    BOOL InsertDateToList(char *szData);


public:
	UINT m_uiSelectDayLineType;

	//{{AFX_DATA(CDayLineSelect)
	enum { IDD = IDD_SELECT_DAYLINE };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	int  m_nDayLineType;
	CListCtrl	m_MyList;
	CTime	m_DayLineTime;
	CMonthCalCtrl	m_MonthCalCtrl;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDayLineSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDayLineSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDayLineType();
	afx_msg void OnAddDownLoadData();


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAYLINESELECT_H__B46517A2_88B1_11D4_882A_00400552E583__INCLUDED_)
