#if !defined(AFX_ALERTPOPDLG_H__FD452F62_6C45_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTPOPDLG_H__FD452F62_6C45_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAlertPopDlg : public CDialog
{

public:
	CAlertPopDlg(CWnd* pParent = NULL);   


	//{{AFX_DATA(CAlertPopDlg)
	enum { IDD = IDD_1UPALERT };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CButtonST	m_bt1;
	CListBox	m_conInfo;
	CString	m_sInfo;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CAlertPopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CAlertPopDlg)
	afx_msg void OnToAlarmSystem();
	afx_msg void OnToKlineFiguer();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTPOPDLG_H__FD452F62_6C45_11D4_970C_0080C8D6450E__INCLUDED_)
