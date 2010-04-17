#if !defined(AFX_TECHDLGINDICATOROFTEN_H__982D8D82_88B4_11D4_9711_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGINDICATOROFTEN_H__982D8D82_88B4_11D4_9711_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgIndicatorOften.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgIndicatorOften dialog
class 	CTaiShanDoc;

class CTaiKlineDlgIndicatorOften : public CDialog
{
// Construction
public:
	CTaiShanDoc*	pDoc;//
	int* m_pIndex;
	CTypedPtrMap<CMapStringToPtr,CString,int*> m_map;

	CTaiKlineDlgIndicatorOften(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgIndicatorOften)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_listselect;
	CListCtrl	m_listall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgIndicatorOften)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgIndicatorOften)
	afx_msg void OnAdd();
	afx_msg void OnDel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListselect(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateImageList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGINDICATOROFTEN_H__982D8D82_88B4_11D4_9711_0080C8D6450E__INCLUDED_)
