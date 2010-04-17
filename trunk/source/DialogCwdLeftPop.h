#if !defined(AFX_DIALOGCWDLEFTPOP_H__4E3C5A95_34E4_4CA1_ABB9_A863860CAAF1__INCLUDED_)
#define AFX_DIALOGCWDLEFTPOP_H__4E3C5A95_34E4_4CA1_ABB9_A863860CAAF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogCwdLeftPop.h : header file
//
#include "CTaiKlineTreeCtl.h"


class CFormularContent;
class CDialogCwdLeftPop : public CDialog
{

public:
	void RefreshJishuTreeCtrl();
	void InsertData();
	CDialogCwdLeftPop(CWnd* pParent = NULL);  
	CWnd* m_pParent;


	//{{AFX_DATA(CDialogCwdLeftPop)
	enum { IDD = IDD_CWD_LEFT_POP };
	CTaiKlineTreeCtl	m_treeFormularCtrl;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogCwdLeftPop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL
protected:
	CFormularContent*	m_pIndex; 
	int nKind;        
	HTREEITEM m_hItem;


protected:


	//{{AFX_MSG(CDialogCwdLeftPop)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCWDLEFTPOP_H__4E3C5A95_34E4_4CA1_ABB9_A863860CAAF1__INCLUDED_)
