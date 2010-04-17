#if !defined(AFX_TESTDIALOGBAR_H__53274537_E9E9_11D2_88EF_0000E8A1A5C0__INCLUDED_)
#define AFX_TESTDIALOGBAR_H__53274537_E9E9_11D2_88EF_0000E8A1A5C0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TestDialogBar.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTaiToolBar dialog

class CTaiToolBar : public CDialog
{
// Construction
public:
	CTaiToolBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiToolBar)
	enum { IDD = IDD_DIALOGBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiToolBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnToolTipNotify(UINT id,NMHDR *pTTTStruct,LRESULT *pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiToolBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CToolBar    m_wndToolBar;
	CToolTipCtrl m_wndToolTip; 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDIALOGBAR_H__53274537_E9E9_11D2_88EF_0000E8A1A5C0__INCLUDED_)
