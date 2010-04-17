#if !defined(AFX_LMBDLGJISHUCLASSIFY_H__D2047699_FB11_11D3_96E8_0080C8E1242B__INCLUDED_)
#define AFX_LMBDLGJISHUCLASSIFY_H__D2047699_FB11_11D3_96E8_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgJishuClassify.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgJishuClassify dialog
#include "CTaiKlineTreeCtl.h"

class CTaiKlineDlgJishuClassify : public CDialog
{
// Construction
public:
	CTaiKlineDlgJishuClassify(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgJishuClassify)
	enum { IDD = IDD_LMB_CLASSIFY_JISHU };
	CTaiKlineTreeCtl	m_treeFormularCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgJishuClassify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgJishuClassify)
	afx_msg void OnOk();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bInit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBDLGJISHUCLASSIFY_H__D2047699_FB11_11D3_96E8_0080C8E1242B__INCLUDED_)
