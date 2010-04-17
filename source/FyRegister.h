#if !defined(AFX_FYREGISTER_H__AA9D2BA1_D444_11D6_968F_AE9E83D8F170__INCLUDED_)
#define AFX_FYREGISTER_H__AA9D2BA1_D444_11D6_968F_AE9E83D8F170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FyRegister : public CDialog
{

public:
	static bool IsValidUser();
	FyRegister(CWnd* pParent = NULL);  


	//{{AFX_DATA(FyRegister)
	enum { IDD = IDD_FY_REGISTER };
	CString	m_sMachineID;
	CString	m_sEmail;
	CString	m_sID;
	CString	m_sPhone;
	CString	m_sAddress;
	CString	m_sPower;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(FyRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:

	
	//{{AFX_MSG(FyRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonHand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FYREGISTER_H__AA9D2BA1_D444_11D6_968F_AE9E83D8F170__INCLUDED_)
