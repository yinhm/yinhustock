#if !defined(AFX_TOOLBAREX_H__78326745_9BDA_11D1_B032_000021002C16__INCLUDED_)
#define AFX_TOOLBAREX_H__78326745_9BDA_11D1_B032_000021002C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx window

class CToolBarEx : public CTabCtrl
{
// Construction
public:
	CToolBarEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarEx)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBarEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarEx)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBAREX_H__78326745_9BDA_11D1_B032_000021002C16__INCLUDED_)
