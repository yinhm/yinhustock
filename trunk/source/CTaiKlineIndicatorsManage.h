#if !defined(AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineIndicatorsManage.h : header file


#include "CTaiKlineTreeCtl.h"

class CFormularContent;
class CTaiKlineIndicatorsManage : public CDialog
{

public:
	CTaiKlineIndicatorsManage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineIndicatorsManage)
	enum { IDD = IDD_MANAGE_GONGSHI };
	CButtonST	m_often;
	CButtonST	m_conModify;
	CButtonST	m_conAdd;
	CButtonST	m_conDelete;
	CButtonST	m_conRead;
	CButtonST	m_conSave;
	CTaiKlineTreeCtl	m_treeFormularCtrl;	
	CString	m_editexplain;
	CString	m_tabedit;
	CButtonST	m_btOK;
	CButtonST	m_btCancel;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CTaiKlineIndicatorsManage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineIndicatorsManage)
	afx_msg void OnAddFormula();	
	afx_msg void OnModifyFormula();	
	afx_msg void OnDeleteFormula();	
	afx_msg void OnSave();		
	afx_msg void OnRead();			
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnOften();			
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeTabSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nKindRoot;			
	CFormularContent *	m_pIndex;		
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_)
