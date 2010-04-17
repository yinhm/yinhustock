#if !defined(AFX_DIASTOCKDETAIL_H__1901EDC1_0784_11D2_88EE_0080C8E20736__INCLUDED_)
#define AFX_DIASTOCKDETAIL_H__1901EDC1_0784_11D2_88EE_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaStockdetail.h : header file
//
#include "CTaiShanKlineShowView.h"
#include "CSharesDetailList.h"


class CDiaStockdetail : public CDialog
{

public:
	CDiaStockdetail(CWnd* pParent = NULL,CTaiShanKlineShowView *m_pVwbaseView = NULL,bool IsF1 = true);   // standard constructor
    void ShowStockDetail();
	void ShowStockDetailF2();
	int iRootItem;
	int iRootItem1;
	int num;
	int prenum;
	int prenumF2;
	int first;
	bool m_IsF1;

	//{{AFX_DATA(CDiaStockdetail)
	enum { IDD = IDD_STOCKDETAIL };
	CSharesDetailList	m_list;
	CTaiShanKlineShowView *m_pView;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDiaStockdetail)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL


protected:

	
	//{{AFX_MSG(CDiaStockdetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIASTOCKDETAIL_H__1901EDC1_0784_11D2_88EE_0080C8E20736__INCLUDED_)
