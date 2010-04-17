#if !defined(AFX_LMBSETNAMETREEGROUP_H__7E6C73EF_FB18_11D3_96E9_0080C8E1242B__INCLUDED_)
#define AFX_LMBSETNAMETREEGROUP_H__7E6C73EF_FB18_11D3_96E9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineSetNameTreeGroup.h : header file


class CTaiKlineSetNameTreeGroup : public CDialog
{

public:
	void DoMoveWindow();
	CTaiKlineSetNameTreeGroup(CWnd* pParent = NULL);   
	bool m_bShowFromMouse;

// Dialog Data
	//{{AFX_DATA(CTaiKlineSetNameTreeGroup)
	enum { IDD = IDD_SET_JISHU_NAME };
	CEdit	m_conEdit;
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineSetNameTreeGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CTaiKlineSetNameTreeGroup)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBSETNAMETREEGROUP_H__7E6C73EF_FB18_11D3_96E9_0080C8E1242B__INCLUDED_)
