#if !defined(AFX_SERIAL_H__27282D95_4E49_11D3_9200_00400552E583__INCLUDED_)
#define AFX_SERIAL_H__27282D95_4E49_11D3_9200_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Serial.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Serial dialog

class Serial : public CDialog
{
// Construction
public:
	Serial(CWnd* pParent = NULL);   // standard constructor
	char Number[10];

// Dialog Data
	//{{AFX_DATA(Serial)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_edit;
	CString	m_Serial;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Serial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Serial)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIAL_H__27282D95_4E49_11D3_9200_00400552E583__INCLUDED_)
