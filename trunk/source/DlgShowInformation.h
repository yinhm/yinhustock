#if !defined(AFX_DLGSHOWDATA_INFO_H__033C6932_15D7_4E42_A488_6B73C6A6201C__INCLUDED_)
#define AFX_DLGSHOWDATA_INFO_H__033C6932_15D7_4E42_A488_6B73C6A6201C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShowInformation.h : header file
//
#include "HtmlShow.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogShowInformation dialog

class CDialogShowInformation : public CDialog
{

public:
	CDialogShowInformation(CWnd* pParent = NULL); 
	CHtmlShow m_htmlShow;
	CString m_sHtml;


	//{{AFX_DATA(CDialogShowInformation)
	enum { IDD = IDD_DIALOG_SHOW_HTML };
		
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogShowInformation)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDialogShowInformation)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHOWDATA_INFO_H__033C6932_15D7_4E42_A488_6B73C6A6201C__INCLUDED_)
