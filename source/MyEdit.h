#if !defined(AFX_MYEDIT_H__DE50440F_0762_11D6_968A_D5E53096946C__INCLUDED_)
#define AFX_MYEDIT_H__DE50440F_0762_11D6_968A_D5E53096946C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoRichEditCtrl.h"
#include "CwdSetParamDlg.h"

class CFormularContent;
struct ParamBE
{
	int m_nParamB;
	int m_nParamCount;
	float fVal;
	ParamBE()
	{
		m_nParamB = 0;
		m_nParamCount = 0;
		fVal = 0;
	};

};

class CTaiTestEdit : public CAutoRichEditCtrl
{

public:
	CTaiTestEdit();

private:	
	CFont m_Font;
	int MoveOnWhichParam(CPoint & point);
	CEdit 	m_edit;
	CFxjSetParamDlg m_editDlg;
	bool m_bCreated;
	CFormularContent * m_jiShu;
	int m_nPaPre;
	
	ParamBE m_nParamInfo[8];


public:


public:


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiTestEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

public:
	void GetParamVal(float* fVal);
	void SetLJishu(CFormularContent* pJis);
	virtual ~CTaiTestEdit();


protected:
	//{{AFX_MSG(CTaiTestEdit)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetfocus();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__DE50440F_0762_11D6_968A_D5E53096946C__INCLUDED_)
