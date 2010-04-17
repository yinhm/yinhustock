#if !defined(AFX_XGNSELECTTIME_H__3A881A41_90CA_11D6_968D_D9E1D3061874__INCLUDED_)
#define AFX_XGNSELECTTIME_H__3A881A41_90CA_11D6_968D_D9E1D3061874__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XgnSelectTime.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XgnSelectTime dialog

class XgnSelectTime : public CDialog
{
// Construction
public:
	XgnSelectTime(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(XgnSelectTime)
	enum { IDD = IDD_XGN_SELECT_TIME };
	CDateTimeCtrl	m_conTm;
	CTime	m_tmSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XgnSelectTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(XgnSelectTime)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XGNSELECTTIME_H__3A881A41_90CA_11D6_968D_D9E1D3061874__INCLUDED_)
