#if !defined(AFX_LDIALOGCROSS_H__B52EF805_BF0D_11D2_91DD_0080C8E1242B__INCLUDED_)
#define AFX_LDIALOGCROSS_H__B52EF805_BF0D_11D2_91DD_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDialogCross.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogCross dialog
#include "resource.h"
class CTaiShanKlineShowView;
class CTaiKlineDialogCross : public CDialog
{
// Construction
public:
	int m_nHeight;
	CBitmap	m_bit;		
	bool m_bSaveBackground;
	CTaiShanKlineShowView* pView;
	bool m_isFirst;
	CTaiKlineDialogCross(CWnd* pParent = NULL);   
	CFont ft;


	//{{AFX_DATA(CTaiKlineDialogCross)
	enum { IDD = IDD_DIALOG1 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDialogCross)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	void SetColor(CDC* pDC,float fVal);

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDialogCross)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDIALOGCROSS_H__B52EF805_BF0D_11D2_91DD_0080C8E1242B__INCLUDED_)
