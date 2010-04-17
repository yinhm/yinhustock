#if !defined(AFX_SHEETBKCYBK_H__9B88A303_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
#define AFX_SHEETBKCYBK_H__9B88A303_B596_11D1_83E9_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiCommonBlockSetDlg : public CDialog
{

public:
	CTaiCommonBlockSetDlg(CWnd* pParent = NULL);   
	CStringArray m_strName;

	//{{AFX_DATA(CTaiCommonBlockSetDlg)
	enum { IDD = IDD_DIALOG1 };
	int		m_nIndex;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CTaiCommonBlockSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CTaiCommonBlockSetDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnChangeChoice(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETBKCYBK_H__9B88A303_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
