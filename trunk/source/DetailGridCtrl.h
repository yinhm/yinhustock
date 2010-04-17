
#if !defined(AFX_DETAILGRIDCTRL_H__D99F64A2_B550_11D4_882E_00400552E583__INCLUDED_)
#define AFX_DETAILGRIDCTRL_H__D99F64A2_B550_11D4_882E_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

class CDetailGridCtrl : public CGridCtrl  
{
	friend class CRealTimeDetailList;
public:
	BOOL m_bIsF1;
	CDetailGridCtrl();
	virtual ~CDetailGridCtrl();
	BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
    BOOL SetRowCount(int nRows = 10);
	float GetItemTextV(int nRow,int nCol);
	void SetDisplayMode(BOOL IsF1){m_bIsF1=IsF1;}
	void GetCurrentDate(int bMenutes,CString &strDate) const;
};

#endif // !defined(AFX_DETAILGRIDCTRL_H__D99F64A2_B550_11D4_882E_00400552E583__INCLUDED_)
