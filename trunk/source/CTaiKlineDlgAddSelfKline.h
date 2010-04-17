//{{AFX_INCLUDES()
#include "dtpicker.h"
//}}AFX_INCLUDES
#if !defined(AFX_TECHDLGADDSELFKLINE_H__C54E43A4_67B1_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGADDSELFKLINE_H__C54E43A4_67B1_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgAddSelfKline.h : header file
//
#include "FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgAddSelfKline dialog
class CTaiShanKlineShowView;
class CTaiKlineDlgAddSelfKline : public CDialog
{
// Construction
public:
	void LoadData(int foot);
	CTaiKlineDlgAddSelfKline(CWnd* pParent = NULL);
	CTaiShanKlineShowView* pView;

	//{{AFX_DATA(CTaiKlineDlgAddSelfKline)
	enum { IDD = IDD_ADD_KLINE_SELF };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CFloatEdit	m_floatEdit7;
	CFloatEdit	m_floatEdit6;
	CFloatEdit	m_floatEdit9;
	CFloatEdit	m_floatEdit8;
	CFloatEdit	m_floatEdit5;
	CFloatEdit	m_floatEdit4;
	CFloatEdit	m_floatEdit3;
	CFloatEdit	m_floatEdit2;
	CFloatEdit	m_conAmount;
	CDateTimeCtrl	m_conDtTm;
	BOOL	m_bUseNNet;
	CTime	m_tmBegin;
	float	m_close;
	float	m_low;
	float	m_open;
	float	m_high;
	int		m_vol;
	CString	m_amountStr;
	float	m_fInnerPartVol;
	float	m_fOuterPartVol;
	int		m_nDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgAddSelfKline)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

// Implementation
protected:


	//{{AFX_MSG(CTaiKlineDlgAddSelfKline)
	afx_msg void OnAdd();
	afx_msg void OnUseNNet();
	virtual BOOL OnInitDialog();
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGADDSELFKLINE_H__C54E43A4_67B1_11D4_970B_0080C8D6450E__INCLUDED_)
