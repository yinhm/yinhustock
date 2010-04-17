#if !defined(AFX_DIALOGGSSM_H__97D83A24_C579_11D1_83F0_0000E8593F1A__INCLUDED_)
#define AFX_DIALOGGSSM_H__97D83A24_C579_11D1_83F0_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIALOGGSSM.h : header file
//
#include "resource.h"


class CDialogLOGGSSM : public CDialog
{

public:
	bool m_bReadOnly;
	CDialogLOGGSSM(CWnd* pParent = NULL); 


	//{{AFX_DATA(CDialogLOGGSSM)
	enum { IDD = IDD_DIALOGGSZS };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CString	m_edit;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CDialogLOGGSSM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:

	
	//{{AFX_MSG(CDialogLOGGSSM)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGGSSM_H__97D83A24_C579_11D1_83F0_0000E8593F1A__INCLUDED_)
