#if !defined(AFX_DIAGSGLMM_H__26B92444_BCC2_11D1_83EE_0000E8593F1A__INCLUDED_)
#define AFX_DIAGSGLMM_H__26B92444_BCC2_11D1_83EE_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIAGSGLMM.h : header file
//
#include "resource.h"


class CDialogGSGLMM : public CDialog
{

public:
	CDialogGSGLMM(CWnd* pParent = NULL);  
	CString   password;
	


	//{{AFX_DATA(CDialogGSGLMM)
	enum { IDD = IDD_DIAGSGLMM };
	CButtonST	m_cancel;
	CButtonST	m_ok;
	CEdit	m_editcor;
	CString	m_input;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogGSGLMM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDialogGSGLMM)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGSGLMM_H__26B92444_BCC2_11D1_83EE_0000E8593F1A__INCLUDED_)
