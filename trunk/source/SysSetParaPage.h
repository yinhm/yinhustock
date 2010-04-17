#if !defined(AFX_SYSSETPARAPAGE_H__C39D49C2_FA4C_11D3_BB59_0000E8593F1A__INCLUDED_)
#define AFX_SYSSETPARAPAGE_H__C39D49C2_FA4C_11D3_BB59_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSetParaPage.h : header file
//
#include "FloatEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetParameterPage dialog

class CTaiSysSetParameterPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiSysSetParameterPage)

// Construction
public:
	CTaiSysSetParameterPage();
	~CTaiSysSetParameterPage();
	CTaiShanDoc *pDoc;

// Dialog Data
	//{{AFX_DATA(CTaiSysSetParameterPage)
	enum { IDD = IDD_SET_PARAM };
	CEdit	m_ltp_rateC;
	CEdit	m_cjl_rateC;
	CFloatEdit	m_nBlockT;
	CFloatEdit	m_nRealT;
	CFloatEdit	m_nTimespan;
	CFloatEdit	m_nUpSpeed;
	CFloatEdit	m_nSwing;
	CFloatEdit	m_nQuantity;
	CFloatEdit	m_nDaysOfMIn;
	CFloatEdit	m_nCountOf;
	CFloatEdit	m_nDaysOfDay;
	CFloatEdit	m_nChangePage;
	CFloatEdit	m_nChange;
	CFloatEdit	m_nCapital;
	CFloatEdit	m_notice;
	CFloatEdit	m_nacouny;
	CFloatEdit	m_nper;
	CFloatEdit	m_n;
	CFloatEdit	m_nDaysOfMany;
	float	m_fBuyRate;
	int		m_nCapitalTime;
	int		m_nChangeHandTime;
	int		m_nChangPageTime;
	int		m_nDaysOfDayKline;
	int		m_nCountOfHistoryFile;
	int		m_nDaysOfMin5Kline;
	int		m_nDaysOfManyKline;
	int		m_nMidWay;
	int		m_nNoFinishedTest;
	int		m_nQuantityTime;
	int		m_fSellRate;
	int		m_nSwingTime;
	int		m_nUpSpeedTime;
	int		m_nTimespanAlert;
	int		m_noticesave;
	float	m_nVol;
	float	m_nPer;
	float	m_nAcount;
	int		m_nFormat;
	CString	m_nAddress;
	int		m_nRealTime;
	int		m_nBlockTime;
	CString	m_nKswtAddr;
	float	m_cjl_rateV;
	float	m_ltp_rateV;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTaiSysSetParameterPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTaiSysSetParameterPage)
	afx_msg void OnSelchangeCombo1();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETPARAPAGE_H__C39D49C2_FA4C_11D3_BB59_0000E8593F1A__INCLUDED_)
