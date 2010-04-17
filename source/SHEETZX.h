#include "CTaiShanDoc.h"

#if !defined(AFX_SHEETZX_H__9A5A8B47_BA79_11D2_9086_0000E8593F1A__INCLUDED_)
#define AFX_SHEETZX_H__9A5A8B47_BA79_11D2_9086_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000
// SHEETZX.h : header file
//
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetSelfBlockPage dialog

class CTaiSysSetSelfBlockPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiSysSetSelfBlockPage)

// Construction
public:
	CTaiSysSetSelfBlockPage();
	~CTaiSysSetSelfBlockPage();

	CMapStringToString m_MapChoose;

// Dialog Data
	//{{AFX_DATA(CTaiSysSetSelfBlockPage)
	enum { IDD = IDD_SHEETZX };
	CButtonST	m_btnAdd;
	CButtonST	m_btnDelete;
	CListCtrl	m_ctrlChoosedStock;
	CListCtrl	m_ctrlStockName;
	CString	m_strChooseStatus;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTaiSysSetSelfBlockPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString GetStockCode(int nIndex);
	void InitListCtrl();
	CTaiShanDoc * GetDocument();
	void DoKeyShow();
	void updatechooselist(int defitem);  //这个函数用来对自选股列表重新显示

	// Generated message map functions
	//{{AFX_MSG(CTaiSysSetSelfBlockPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList3(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char keyinput;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETZX_H__9A5A8B47_BA79_11D2_9086_0000E8593F1A__INCLUDED_)
