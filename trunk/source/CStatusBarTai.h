#if !defined(AFX_CStatusBar_sjl_H__6E6FB2C8_268B_11D3_842C_00A0C9422E56__INCLUDED_)
#define AFX_CStatusBar_sjl_H__6E6FB2C8_268B_11D3_842C_00A0C9422E56__INCLUDED_

#include "CoolPopupMenu.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ticker.h"


struct tagBIG_LIST
{
	char m_szStockName[10];
	char m_szReason[10];
	int  m_nAmount;
	float m_fPrice;
};


class CStatusBar_sjl : public CStatusBar
{

public:
	CStatusBar_sjl();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);


public:
	float m_fRedGreen_sh; 
	float m_fRedGreen_sz;
	float m_fRedGreen_cy;

	float m_fIndex_sh; 
	float m_fIndex_sz;
	float m_fIndex_cy;

	float m_fAD_sh; 
	float m_fAD_sz;
	float m_fAD_cy;

	float m_fVol_sh;
	float m_fVol_sz;
	float m_fVol_cy;

	float m_fAmount_sh;
	float m_fAmount_sz;
	float m_fAmount_cy;

	BOOL m_aRcvCount[4];
	int nCount;
	CRect m_aPaneRect[8];
	COLORREF bColor[10];

	tagBIG_LIST m_BigBill;

	BOOL  m_bHasAlarm;
	CString m_strAlarmReason;

	CTicker*	m_pTicker;


public:
	void SetRedGreen_sh(float fRedGreen,BOOL bRepaint = TRUE);
	void SetRedGreen_sz(float fRedGreen,BOOL bRepaint = TRUE);
	void SetRedGreen_cy(float fRedGreen,BOOL bRepaint = TRUE);

	void SetIndex_sh(float fIndex,BOOL bRepaint = TRUE);
	void SetIndex_sz(float fIndex,BOOL bRepaint = TRUE);
	void SetIndex_cy(float fIndex,BOOL bRepaint = TRUE);

	void SetAD_sh(float fAD,BOOL bRepaint = TRUE);
	void SetAD_sz(float fAD,BOOL bRepaint = TRUE);
	void SetAD_cy(float fAD,BOOL bRepaint = TRUE);

	void SetVol_sh(float fVol,BOOL bRepaint = TRUE);
	void SetVol_sz(float fVol,BOOL bRepaint = TRUE);
	void SetVol_cy(float fVol,BOOL bRepaint = TRUE);

	void SetAmount_sh(float fAmount,BOOL bRepaint = TRUE);
	void SetAmount_sz(float fAmount,BOOL bRepaint = TRUE);
	void SetAmount_cy(float fAmount,BOOL bRepaint = TRUE);

	void SetAlarm(CString& strReason);
	void SetRcvCount(int nCount,int nType,BOOL bRepaint = TRUE);


	//{{AFX_VIRTUAL(CStatusBar_sjl)
	//}}AFX_VIRTUAL


public:
	void DrawTime(CDC &dc, CRect rect);
	void DrawSzRed(CDC &dc, CRect rect);
	void DrawShRed(CDC &dc, CRect rect);
	void SetRollText(char* ch);
	void DrawMyXMZPane(CDC &dc, CRect rect);
	void PaintRect(BOOL bRepaint=TRUE);
	void SetBigBill(char *strStockName, char* strReason,int nAmount,float fPrice,BOOL bRepaint);
	virtual ~CStatusBar_sjl();
	CToolTipCtrl m_wndToolTip;
 
protected:
	void DrawSzInfoPane(CDC& dc,CRect rect);
	void DrawSignalPane(CDC& dc,CRect rect);
	void DrawCardPane(CDC& dc,CRect rect);
	void DrawAlarmPane(CDC& dc,CRect rect);
	void DrawShInfoPane(CDC& dc,CRect rect);
	void DrawBigBill(CDC& dc,CRect rect);
	//{{AFX_MSG(CStatusBar_sjl)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


protected:
	void DrawCyInfoPane(CDC& dc,CRect rect);
	void DrawMenuIconPane(CDC &dc, CRect rect);
	CCoolPopupMenu m_Menu;
	CImageList m_ImageList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CStatusBar_sjl_H__6E6FB2C8_268B_11D3_842C_00A0C9422E56__INCLUDED_)
