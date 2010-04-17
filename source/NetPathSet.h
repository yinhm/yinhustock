#if !defined(AFX_NETPATHSET_H__DA870FB9_5E1A_4E1E_8F94_972534718885__INCLUDED_)
#define AFX_NETPATHSET_H__DA870FB9_5E1A_4E1E_8F94_972534718885__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetPathSet.h : header file
//
#include "FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CNetPathSet dialog

class CNetPathSet : public CPropertyPage
{
// Construction
public:
	void SaveDataToReg();
	CNetPathSet();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetPathSet)
	enum { IDD = IDD_DIALOG1 };
	CFloatEdit	m_f2;
	CFloatEdit	m_f1;
	CFloatEdit	m_f0;
	CString	m_serverPath;
	CString	m_sDataBakPath;
	DWORD	m_nMax0;
	DWORD	m_nMax1;
	DWORD	m_nMax2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetPathSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetPathSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonSetServerPath();
	afx_msg void OnButtonBakPathSet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETPATHSET_H__DA870FB9_5E1A_4E1E_8F94_972534718885__INCLUDED_)
