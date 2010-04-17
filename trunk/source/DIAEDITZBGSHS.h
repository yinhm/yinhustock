#if !defined(AFX_DIAEDITZBGSHS_H__D7507DE6_BBE0_11D1_83EE_0000E8593F1A__INCLUDED_)
#define AFX_DIAEDITZBGSHS_H__D7507DE6_BBE0_11D1_83EE_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIAEDITZBGSHS.h : header file
//
const  int FUNCNUM=57;


#include "resource.h"
struct dataFunc
{
	char s[512];
	int nFlag;
	char s2[512];
};
class CDialogEDITZBGSHS : public CDialog
{

public:
	CDialogEDITZBGSHS(CWnd* pParent = NULL);  


	HTREEITEM  m_selecteditem;
	CString    m_selectedstr;



	//{{AFX_DATA(CDialogEDITZBGSHS)
	enum { IDD = IDD_EDIT_HS };
	CButtonST	m_ok;
	CTreeCtrl	m_tree;
	CString	m_sExplain;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogEDITZBGSHS)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDialogEDITZBGSHS)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturnTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAEDITZBGSHS_H__D7507DE6_BBE0_11D1_83EE_0000E8593F1A__INCLUDED_)
