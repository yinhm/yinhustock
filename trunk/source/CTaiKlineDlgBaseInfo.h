#if !defined(AFX_LDLGBASEINFO_H__FDA57761_1E46_11D3_91F3_0080C8E1242B__INCLUDED_)
#define AFX_LDLGBASEINFO_H__FDA57761_1E46_11D3_91F3_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgBaseInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgBaseInfo dialog

class CTaiShanKlineShowView;
class CTaiKlineDlgBaseInfo : public CDialog
{
// Construction
public:
	CTaiKlineDlgBaseInfo(CWnd* pParent = NULL);   // standard constructor
	CTaiShanKlineShowView * pView;

	COleDateTime m_time_ole;
	CDateTimeCtrl m_DateTime;
// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgBaseInfo)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_cBrief;
	CEdit	m_cContent;
	CString	m_content;
	CString	m_brief;
	CString	m_symbol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgBaseInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:


	//{{AFX_MSG(CTaiKlineDlgBaseInfo)
	afx_msg void OnChangeEditBrief();
	afx_msg void OnChangeEditContentTotal();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDLGBASEINFO_H__FDA57761_1E46_11D3_91F3_0080C8E1242B__INCLUDED_)
