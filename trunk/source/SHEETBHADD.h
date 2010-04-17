#if !defined(AFX_SHEETBHADD_H__9B88A302_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
#define AFX_SHEETBHADD_H__9B88A302_B596_11D1_83E9_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETBHADD.h : header file

class CSHEETBHADD : public CDialog
{

public:
	CSHEETBHADD(CWnd* pParent = NULL); 


	BOOL m_OpenKind;

	//{{AFX_DATA(CSHEETBHADD)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_cBKNAME;
	CStatic	m_Static;
	CString	m_bkname;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CSHEETBHADD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CSHEETBHADD)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETBHADD_H__9B88A302_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
