#if !defined(AFX_TECHDLGCANUSEDPERIOD_H__E8B2AFE2_71B9_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGCANUSEDPERIOD_H__E8B2AFE2_71B9_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgCanUsedPeriod.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgCanUsedPeriod dialog

class CTaiKlineDlgCanUsedPeriod : public CDialog
{
// Construction
public:
	void InputString(BYTE *pByte);
	void OutSetting(BYTE* pByte);
	CTaiKlineDlgCanUsedPeriod(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgCanUsedPeriod)
	enum { IDD = IDD_CAN_USED };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	BOOL	m_bHs;
	BOOL	m_b1Min;
	BOOL	m_b5Min;
	BOOL	m_b15Min;
	BOOL	m_b30Min;
	BOOL	m_b60Min;
	BOOL	m_bDay;
	BOOL	m_bWeek;
	BOOL	m_bMonth;
	BOOL	m_bYear;
	BOOL	m_bMany;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgCanUsedPeriod)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

// Implementation
protected:


	//{{AFX_MSG(CTaiKlineDlgCanUsedPeriod)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGCANUSEDPERIOD_H__E8B2AFE2_71B9_11D4_970C_0080C8D6450E__INCLUDED_)
