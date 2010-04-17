#if !defined(AFX_LMBSERVERPATHSET_H__77566FCF_0482_11D4_96EA_0080C8E1242B__INCLUDED_)
#define AFX_LMBSERVERPATHSET_H__77566FCF_0482_11D4_96EA_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LmbServerPathSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LmbServerPathSet dialog

class LmbServerPathSet : public CDialog
{

public:
	bool TestPath(bool bMessage=false);
	LmbServerPathSet(CWnd* pParent = NULL); 


	//{{AFX_DATA(LmbServerPathSet)
	enum { IDD = IDD_DIALOG61 };
	CString	m_serverPath;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(LmbServerPathSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(LmbServerPathSet)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBSERVERPATHSET_H__77566FCF_0482_11D4_96EA_0080C8E1242B__INCLUDED_)
