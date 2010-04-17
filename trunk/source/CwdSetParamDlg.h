#if !defined(AFX_CWDSETPARAMDLG_H__3073A620_01AB_47ED_9C73_08C8DFAF7224__INCLUDED_)
#define AFX_CWDSETPARAMDLG_H__3073A620_01AB_47ED_9C73_08C8DFAF7224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FloatEdit.h"

class CFxjSetParamDlg : public CDialog
{

public:
	void SetSelEdit();
	void SetText(CString s,int nMin=-1000,int nMax = 1000);
	CString GetText();
	CFxjSetParamDlg(CWnd* pParent = NULL);  


	//{{AFX_DATA(CFxjSetParamDlg)
	enum { IDD = IDD_CWD_EDIT_PARAM };
	CSpinButtonCtrl	m_spin;
	CFloatEdit	m_edit;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CFxjSetParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:

	
	//{{AFX_MSG(CFxjSetParamDlg)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWDSETPARAMDLG_H__3073A620_01AB_47ED_9C73_08C8DFAF7224__INCLUDED_)
