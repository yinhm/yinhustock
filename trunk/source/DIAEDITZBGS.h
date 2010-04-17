#include "CTaiShanDoc.h"
#if !defined(AFX_DIAEDITZBGS_H__482F9B44_D07B_11D2_908D_0000E8593F1A__INCLUDED_)
#define AFX_DIAEDITZBGS_H__482F9B44_D07B_11D2_908D_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIAEDITZBGS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEDITZBGS dialog


class CDialogEDITZBGS : public CDialog
{

public:
	BOOL CheckParmNameInput(CString parname,CString & errormessage);
	BOOL CheckParmInput(CString parminput);
	void ShowData(CString &str,float fdata);
	float StringToFloat(CString str);
	CString FloatToString(float input);
	CDialogEDITZBGS(CWnd* pParent = NULL);   
	int item ;                         
	BYTE   numPara;                       
	CFormularContent*  jishunow;                
	RECT datarect;                      
    int  errorposition;                
	CString errormessage;             
	CString functionstr;
	CString help;

	int   addpos;                    


	//{{AFX_DATA(CDialogEDITZBGS)
	enum { IDD = IDD_EDIT_ZBGS };
	CEdit	m_namecor;
	CButtonST	m_hfqs;
	CButtonST	m_ztdj;
	CEdit	m_edit41cor;
	CEdit	m_edit31cor;
	CEdit	m_edit21cor;
	CEdit	m_edit11cor;
	CEdit	m_inputcor;
	CEdit	m_edit24cor;
	CEdit	m_edit44cor;
	CEdit	m_edit43cor;
	CEdit	m_edit42cor;
	CEdit	m_edit34cor;
	CEdit	m_edit33cor;
	CEdit	m_edit32cor;
	CEdit	m_edit23cor;
	CEdit	m_edit22cor;
	CEdit	m_edit14cor;
	CEdit	m_edit13cor;
	CEdit	m_edit12cor;
	CEdit	m_passwordcor;
	CEdit	m_xlinecor;
	CString	m_name;
	CString	m_password;
	CString	m_input;
	CString	m_edit11;
	CString	m_edit21;
	CString	m_edit31;
	CString	m_edit41;
	CString	m_edit12;
	CString	m_edit13;
	CString	m_edit14;
	CString	m_edit22;
	CString	m_edit23;
	CString	m_edit24;
	CString	m_edit32;
	CString	m_edit33;
	CString	m_edit34;
	CString	m_edit42;
	CString	m_edit43;
	CString	m_edit44;
	CString	m_xline;
	CString	m_explainbrief;
	BOOL	m_checkmm;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogEDITZBGS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDialogEDITZBGS)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckmm();
	virtual void OnOK();
	afx_msg void OnCsgs();
	afx_msg void OnPaint();
	afx_msg void OnDyhs();
	afx_msg void OnHfqs();
	afx_msg void OnMyhelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAEDITZBGS_H__482F9B44_D07B_11D2_908D_0000E8593F1A__INCLUDED_)
