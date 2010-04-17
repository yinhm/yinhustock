#if !defined(AFX_LSHOWTEXT_H__04AD9D05_BA81_11D2_91DF_0080C8E1242B__INCLUDED_)
#define AFX_LSHOWTEXT_H__04AD9D05_BA81_11D2_91DF_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineShowText.h : header file
//
#include "resource.h"

class CTaiShanKlineShowView;

class CTaiKlineShowText : public CDialog
{

public:
	CTaiShanKlineShowView* pView;
	bool m_bMove;
	bool m_bOk;
	HWND hWndParent;
	BOOL bEnableParent;
	void ShowWin();
	int x;
	int y;
	CTaiKlineShowText(CWnd* pParent = NULL);  


	//{{AFX_DATA(CTaiKlineShowText)
	enum { IDD = IDD_SHOW_TEXT };
	CString	m_editText;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CTaiKlineShowText)
	public:
	virtual int DoModal();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CTaiKlineShowText)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit2();
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnCancelMode();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnActivateApp(BOOL bActive, DWORD hTask);
	//}}AFX_MSG
	afx_msg LRESULT ShowMyWin(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSHOWTEXT_H__04AD9D05_BA81_11D2_91DF_0080C8E1242B__INCLUDED_)
