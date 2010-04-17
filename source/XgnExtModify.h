#if !defined(AFX_XGNEXTMODIFY_H__81F4EFC1_808F_11D6_968D_D4F1A393C675__INCLUDED_)
#define AFX_XGNEXTMODIFY_H__81F4EFC1_808F_11D6_968D_D4F1A393C675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XgnExtModify.h : header file
//
#include "CTaiScreenParent.h"
#include "indicatorextendData.h"
#include "FileExtendData.h"
#include "FileInOutData.h"
#include "FtpGetSend.h"
#include "FtpParaClass.h"
#include "HttpGetData.h"


/////////////////////////////////////////////////////////////////////////////

class XgnExtDataDlg;
class XgnExtModify : public CTaiScreenParent
{

public:
	XgnExtModify(CWnd* pParent = NULL);   

	IndicatorInfo m_infoData;
	XgnExtDataDlg * m_pParent;
	int m_nSelInfo;

// Dialog Data
	//{{AFX_DATA(XgnExtModify)
	enum { IDD = IDD_XGN_MODIFY_EXT };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CComboBox	m_combTarget;
	CButtonST	m_buttonExpl;
	CString	m_sExtName;
	UINT	m_nID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XgnExtModify)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(XgnExtModify)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonExplain();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboPeriod();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XGNEXTMODIFY_H__81F4EFC1_808F_11D6_968D_D4F1A393C675__INCLUDED_)
