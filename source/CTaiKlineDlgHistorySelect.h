#if !defined(AFX_LMBDLGHISTORYSELECT_H__A3A59949_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
#define AFX_LMBDLGHISTORYSELECT_H__A3A59949_FF03_11D3_96E9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgHistorySelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgHistorySelect dialog
class CTaiKlineFileHS;

class CTaiKlineHistorySelect : public CObject
{
public:
	int FilterFileName(CTime& begin,CTime& end);
	bool OpenAll();
	CTaiKlineHistorySelect();

	CStringArray m_fileNameArray;

	CTypedPtrMap<CMapStringToPtr,CString,CTaiKlineFileHS*> m_fileHsShArray;
	CTypedPtrMap<CMapStringToPtr,CString,CTaiKlineFileHS*> m_fileHsSzArray;

	virtual ~CTaiKlineHistorySelect();
private:
	void CloseAll();
}
;

class CTaiKlineDlgHistorySelect : public CDialog
{
// Construction
public:
	static void GetFileNameArray(CStringArray& sArry);
	

	CString m_fileName;
	CTaiKlineDlgHistorySelect(CWnd* pParent = NULL); 

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgHistorySelect)
	enum { IDD = IDD_HISTORY_SELECT };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CListCtrl	m_fileNameList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgHistorySelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgHistorySelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBDLGHISTORYSELECT_H__A3A59949_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
