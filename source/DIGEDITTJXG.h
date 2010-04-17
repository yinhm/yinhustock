#include "CTaiShanDoc.h"
#if !defined(AFX_DIGEDITTJXG_H__482F9B46_D07B_11D2_908D_0000E8593F1A__INCLUDED_)
#define AFX_DIGEDITTJXG_H__482F9B46_D07B_11D2_908D_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIGEDITTJXG.h : header file
//
#include "resource.h"


class CDIGEDITTJXG : public CDialog
{

public:
	BOOL CheckParmNameInput(CString parname,CString & errmessage);
	BOOL CheckParmInput(CString parminput);
	float StringToFloat(CString str);
	void ShowData(CString &str,float fdata);
	CDIGEDITTJXG(CWnd* pParent = NULL);  
	CFormularContent*  jishunow;                
	int  errorposition;                 
	CString errormessage;               
	CString functionstr;             
	int item;                           
	bool iskline;                      
	RECT datarect;                    
	int numPara;                         

	int  addpos;
	
// Dialog Data
	//{{AFX_DATA(CDIGEDITTJXG)
	enum { IDD = IDD_EDIT_TJXGGS };
	CEdit	m_namecor;
	CEdit	m_passwordcor;
	CButtonST	m_hfqs;
	CEdit	m_inputcor;
	CEdit	m_edit44cor;
	CEdit	m_edit43cor;
	CEdit	m_edit42cor;
	CEdit	m_edit41cor;
	CEdit	m_edit34cor;
	CEdit	m_edit33cor;
	CEdit	m_edit32cor;
	CEdit	m_edit31cor;
	CEdit	m_edit24cor;
	CEdit	m_edit23cor;
	CEdit	m_edit22cor;
	CEdit	m_edit21cor;
	CEdit	m_edit14cor;
	CEdit	m_edit13cor;
	CEdit	m_edit12cor;
	CEdit	m_edit11cor;
	BOOL	m_checkmm;
	CString	m_edit11;
	CString	m_edit12;
	CString	m_edit13;
	CString	m_edit14;
	CString	m_edit21;
	CString	m_edit22;
	CString	m_edit23;
	CString	m_edit24;
	CString	m_edit31;
	CString	m_edit33;
	CString	m_edit32;
	CString	m_edit34;
	CString	m_edit41;
	CString	m_edit42;
	CString	m_edit43;
	CString	m_edit44;
	CString	m_explainbrief;
	CString	m_myhelp;
	CString	m_name;
	CString	m_input;
	CString	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDIGEDITTJXG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDIGEDITTJXG)
	afx_msg void OnCsgs();
	virtual void OnOK();
	afx_msg void OnDyhs();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCheckmm();
	afx_msg void OnHfqs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGEDITTJXG_H__482F9B46_D07B_11D2_908D_0000E8593F1A__INCLUDED_)
