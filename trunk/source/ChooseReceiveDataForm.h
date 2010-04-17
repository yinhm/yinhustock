#if !defined(AFX_CHOOSERECEIVEDATAFORM_H__A6939792_882B_4728_B60C_C5E95832D8B5__INCLUDED_)
#define AFX_CHOOSERECEIVEDATAFORM_H__A6939792_882B_4728_B60C_C5E95832D8B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChooseReceiveDataForm : public CDialog
{

public:
	int nWhich;
	CChooseReceiveDataForm(CWnd* pParent = NULL);  


	//{{AFX_DATA(CChooseReceiveDataForm)
	enum { IDD = IDD_DIALOG13 };
	BOOL	m_ShowDlg;
	int		m_OldData;

	//}}AFX_DATA



	//{{AFX_VIRTUAL(CChooseReceiveDataForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CChooseReceiveDataForm)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHOOSERECEIVEDATAFORM_H__A6939792_882B_4728_B60C_C5E95832D8B5__INCLUDED_)
