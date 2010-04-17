#if !defined(AFX_TECHDLGNNETEDITDATA_H__C9B1AE2C_6860_4654_9332_B776A979B1B5__INCLUDED_)
#define AFX_TECHDLGNNETEDITDATA_H__C9B1AE2C_6860_4654_9332_B776A979B1B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgNNetEditData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgNNetEditData dialog
class CTaiKlineDlgNeuralLearn;

class CTaiKlineDlgNNetEditData : public CDialog
{
// Construction
public:
	CTaiKlineDlgNNetEditData(CWnd* pParent = NULL);   // standard constructor
	CTaiKlineDlgNeuralLearn* m_pParent;

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgNNetEditData)
	enum { IDD = IDD_11_EDIT_DATA };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CListCtrl	m_conData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgNNetEditData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgNNetEditData)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGNNETEDITDATA_H__C9B1AE2C_6860_4654_9332_B776A979B1B5__INCLUDED_)
