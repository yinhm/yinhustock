#if !defined(AFX_SJLCHUQUANINDLG_H__5A9C8081_8D90_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_SJLCHUQUANINDLG_H__5A9C8081_8D90_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

	typedef struct tagFXJPOWER
	{
		int   nTime;
		float fGive;
		float fAlloc;
		float fAllocPrice;
		float fDividend;
	}FXJPOWER;

class CTaiChuQuanInDlg : public CDialog
{

public:
	static BOOL InstallPowerData(CString FilePath);

	CTaiChuQuanInDlg(CWnd* pParent = NULL);  
    CTaiShanDoc* pDoc;

	//{{AFX_DATA(CTaiChuQuanInDlg)
	enum { IDD = IDD_CHUQUANIMPORT };
	CButtonST	m_cancel;
	CButtonST	m_read;
	CButtonST	m_import;
	CComboBox	m_PowerForm;
	CString	m_InFile;
	CString	m_FormName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTaiChuQuanInDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CTaiChuQuanInDlg)
	afx_msg void OnImport();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGeshi();
	afx_msg void OnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SJLCHUQUANINDLG_H__5A9C8081_8D90_11D4_9426_0080C8E20736__INCLUDED_)
