#if !defined(AFX_TECHDLGPARAMEXPLAIN_H__5ECE54C2_78D3_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGPARAMEXPLAIN_H__5ECE54C2_78D3_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgParamExplain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgParamExplain dialog

class CTaiKlineDlgParamExplain : public CDialog
{
// Construction
public:
	CTaiKlineDlgParamExplain(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgParamExplain)
	enum { IDD = IDD_8_PARAM_EXPLAIN };
	CString	m_sExplain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgParamExplain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgParamExplain)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGPARAMEXPLAIN_H__5ECE54C2_78D3_11D4_970C_0080C8D6450E__INCLUDED_)
