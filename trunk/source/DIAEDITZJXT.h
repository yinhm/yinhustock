#include "CFormularContent.H"

#if !defined(AFX_DIAEDITZJXT_H__482F9B45_D07B_11D2_908D_0000E8593F1A__INCLUDED_)
#define AFX_DIAEDITZJXT_H__482F9B45_D07B_11D2_908D_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIAEDITZJXT.h : header file
//
#include "resource.h"


class CDialogEDITZJXT : public CDialog
{

public:
	float StringToFloat(CString str);
	void ShowData(CString &str,float fdata);
	CDialogEDITZJXT(CWnd* pParent = NULL);  
	
	CFormularContent*  jishunow;                 
	int  errorposition;              
	CString errormessage;               
	CString functionstr;               
	int item;                           
	RECT datarect;                     
	int numPara;                         
	CString inputpre;                     
	

// Dialog Data
	//{{AFX_DATA(CDialogEDITZJXT)
	enum { IDD = IDD_EDITZJXT };
	CEdit	m_namecor;
	CButtonST	m_hfqscor;
	CEdit	m_sellinputcor;
	CEdit	m_passwordcor;
	CEdit	m_inputcor;
	CEdit	m_edit45cor;
	CEdit	m_edit44cor;
	CEdit	m_edit43cor;
	CEdit	m_edit42cor;
	CEdit	m_edit41cor;
	CEdit	m_edit35cor;
	CEdit	m_edit34cor;
	CEdit	m_edit33cor;
	CEdit	m_edit32cor;
	CEdit	m_edit31cor;
	CEdit	m_edit25cor;
	CEdit	m_edit24cor;
	CEdit	m_edit23cor;
	CEdit	m_edit22cor;
	CEdit	m_edit21cor;
	CEdit	m_edit15cor;
	CEdit	m_edit14cor;
	CEdit	m_edit13cor;
	CEdit	m_edit12cor;
	CEdit	m_edit11cor;
	CEdit	m_buyinputcor;
	CString	m_buyinput;
	BOOL	m_checkmm;
	CString	m_edit11;
	CString	m_edit12;
	CString	m_edit13;
	CString	m_edit14;
	CString	m_edit15;
	CString	m_edit21;
	CString	m_edit22;
	CString	m_edit23;
	CString	m_edit24;
	CString	m_edit25;
	CString	m_edit31;
	CString	m_edit32;
	CString	m_edit33;
	CString	m_edit34;
	CString	m_edit35;
	CString	m_edit41;
	CString	m_edit42;
	CString	m_edit43;
	CString	m_edit44;
	CString	m_edit45;
	CString	m_explainbrief;
	CString	m_input;
	CString	m_name;
	CString	m_password;
	CString	m_sellinput;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogEDITZJXT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:

	
	//{{AFX_MSG(CDialogEDITZJXT)
	afx_msg void OnPaint();
	afx_msg void OnCheckmm();
	afx_msg void OnCsgs();
	afx_msg void OnDyhs();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnHfqs();
	afx_msg void OnMyhelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAEDITZJXT_H__482F9B45_D07B_11D2_908D_0000E8593F1A__INCLUDED_)
