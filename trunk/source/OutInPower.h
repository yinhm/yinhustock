#if !defined(AFX_OUTINPOWER_H__F74E7581_1E44_11D3_BF3B_0000E86564F7__INCLUDED_)
#define AFX_OUTINPOWER_H__F74E7581_1E44_11D3_BF3B_0000E86564F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutInPower.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanOutInDlg dialog

class CTaiChuQuanOutInDlg : public CDialog
{
// Construction
public:
	CTaiChuQuanOutInDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiChuQuanOutInDlg)
	enum { IDD = IDD_OUTINPOWER };
	CComboBox	m_PowerForm;
	CProgressCtrl	m_Progress;
	CString	m_OutFile;
	CString	m_InFile;
	CString	m_FormName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiChuQuanOutInDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CTaiShanDoc * GetDocument();

	// Generated message map functions
	//{{AFX_MSG(CTaiChuQuanOutInDlg)
	afx_msg void OnOut();
	afx_msg void OnIn();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSave();
	afx_msg void OnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//分析家导出的除权数据结构
	typedef struct tagFXJPOWER
	{
		int   nTime;
		float fGive;
		float fAlloc;
		float fAllocPrice;
		float fDividend;
	}FXJPOWER;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTINPOWER_H__F74E7581_1E44_11D3_BF3B_0000E86564F7__INCLUDED_)
