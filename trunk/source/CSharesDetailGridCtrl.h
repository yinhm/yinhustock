
#if !defined(AFX_STOCKDETAILGRIDCTRL_H__D99F64A5_B550_11D4_882E_00400552E583__INCLUDED_)
#define AFX_STOCKDETAILGRIDCTRL_H__D99F64A5_B550_11D4_882E_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCtrl.h"

class CSharesDetailGridCtrl : public CGridCtrl  
{
	friend class CSharesRealTimeDetail;

public:
	BOOL m_bIsF1;
	CTaiShanKlineShowView *m_pView;
	CSharesDetailGridCtrl();
	virtual ~CSharesDetailGridCtrl();
	BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
    BOOL SetRowCount(int nRows = 10);
	float GetItemTextV(int nRow,int nCol);
	void SetDisplayMode(BOOL IsF1){m_bIsF1=IsF1;}
	void SetViewPoint(CTaiShanKlineShowView *pView){m_pView=pView;}
    void GetCurrentDate(time_t t,CString &strDate) const;


};

#endif // !defined(AFX_STOCKDETAILGRIDCTRL_H__D99F64A5_B550_11D4_882E_00400552E583__INCLUDED_)
