#if !defined(AFX_WIDEDOWNLOADTIMESET_H__061132A5_4E5A_40AE_A74C_A9CEE991D4D0__INCLUDED_)
#define AFX_WIDEDOWNLOADTIMESET_H__061132A5_4E5A_40AE_A74C_A9CEE991D4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WideDownLoadTimeSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownLoadNetTimeSet dialog

class CDownLoadNetTimeSet : public CDialog
{
// Construction
public:
	CDownLoadNetTimeSet(CWnd* pParent = NULL);   // standard constructor
	CDownLoadNetTimeSet(time_t StartTime,time_t EndTime,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDownLoadNetTimeSet)
	enum { IDD = IDD_WIDE_SETTIME };
	CTime	m_Start;
	CTime	m_End;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadNetTimeSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownLoadNetTimeSet)
		// NOTE: the ClassWizard will add member functions here
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIDEDOWNLOADTIMESET_H__061132A5_4E5A_40AE_A74C_A9CEE991D4D0__INCLUDED_)
