#if !defined(AFX_TECHDLGSTOPLOSE_H__26076B3B_535C_402F_85AD_5302F4149043__INCLUDED_)
#define AFX_TECHDLGSTOPLOSE_H__26076B3B_535C_402F_85AD_5302F4149043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgStopLose.h : header file
//
#include  "CTaiScreenParent.h"
#include "FloatEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgStopLose dialog

class CTaiKlineDlgStopLose : public CDialog
{
// Construction
public:
	void GetAdditional(ADDITIONAL_BUYSELL &bsAdditional);
	void SetAdditional(ADDITIONAL_BUYSELL& bsAdditional);
	CTaiKlineDlgStopLose(CWnd* pParent = NULL);   // standard constructor
    CTaiScreenParent *GetInfo;
// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgStopLose)
	enum { IDD = IDD_STOP_LOSE_CASE };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CFloatEdit	m_fInitF;
	CFloatEdit	m_fMaxD;
	CFloatEdit	m_fMaxW;
	CFloatEdit	m_fMaxL;
	CTabCtrl	m_nTabCtrl;
	BOOL	m_bLose;
	BOOL	m_bWin;
	BOOL	m_bMaxDays;
	BOOL	m_bUseInitFinace;
	BOOL	m_bUsestractFee;
	BOOL	m_bAddPreProfit;
	float	m_fMaxLose;
	float	m_fMaxWin;
	float	m_fInitFinace;
	int		m_Enterpoint;
	int		m_fMaxDays;
	//}}AFX_DATA
    COLORREF m_ColorPoint;
	RECT     m_rectsample;                   
	ADDITIONAL_BUYSELL buySellAdditional;
   

	//{{AFX_VIRTUAL(CTaiKlineDlgStopLose)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgStopLose)
	virtual BOOL OnInitDialog();
	afx_msg void OnLose();
	afx_msg void OnWin();
	afx_msg void OnMayDays();
	afx_msg void OnUseInitFinace();
	virtual void OnOK();
	afx_msg void OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSetColor();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGSTOPLOSE_H__26076B3B_535C_402F_85AD_5302F4149043__INCLUDED_)
