#if !defined(AFX_BGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_)
#define AFX_BGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

class CSharesCjMxTj;
class CBGridCtrl : public CGridCtrl
{

	friend class CSharesBigBillDlg;

public:
	CBGridCtrl();


public:


public:

	//{{AFX_VIRTUAL(CBGridCtrl)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//}}AFX_VIRTUAL

public:
	virtual ~CBGridCtrl();
	BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
    BOOL SetRowCount(int nRows = 10);
	float GetItemTextV(int nRow,int nCol);
	BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);
	void QuickSortString();
	CString GetTimeGrid(time_t ntime)const;







	
protected:
	//{{AFX_MSG(CBGridCtrl)
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_)
