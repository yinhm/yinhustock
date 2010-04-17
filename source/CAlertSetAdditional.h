#if !defined(AFX_ALERTSETADDITIONAL_H__F218F664_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTSETADDITIONAL_H__F218F664_69E0_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAlertSetAdditional : public CDialog
{

public:
	CAlertSetAdditional(CWnd* pParent = NULL);  


	//{{AFX_DATA(CAlertSetAdditional)
	enum { IDD = IDD_ALARM_ADDED_CASE };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	BOOL	m_bSound;
	BOOL	m_bContinueWatch;
	BOOL	m_bMessageBox;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CAlertSetAdditional)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CAlertSetAdditional)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTSETADDITIONAL_H__F218F664_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
