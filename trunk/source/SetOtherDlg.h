#if !defined(AFX_SETOTHERDLG_H__3DAA868B_FBAF_11D2_A6A2_0080C87F62E5__INCLUDED_)
#define AFX_SETOTHERDLG_H__3DAA868B_FBAF_11D2_A6A2_0080C87F62E5__INCLUDED_

#include "ColorCombox.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetOtherDlg.h : header file

class CSetOtherDlg : public CDialog
{

public:

	COLORREF m_BackColor;
	COLORREF m_TitleColor;
	COLORREF m_StockNameColor;
	COLORREF m_OtherDataColor;
	COLORREF m_StockUpColor;
	COLORREF m_StockDownColor;
	COLORREF m_StockStopColor;

	LOGFONT m_LogFont;
	CSetOtherDlg(CWnd* pParent = NULL);   


	//{{AFX_DATA(CSetOtherDlg)
	enum { IDD = IDD_OTHERSETDLG };

	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSetOtherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL


protected:

	GColorComboBox m_nColorSet[7];

	//{{AFX_MSG(CSetOtherDlg)
	afx_msg void OnSetfont();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetdefault();
	//}}AFX_MSG
    afx_msg void OnBackColorChange();
    afx_msg void OnTitleColorChange();
    afx_msg void OnStockNameColorChange();
    afx_msg void OnOtherDataColorChange();
    afx_msg void OnStockUpColorChange();
    afx_msg void OnStockDownColorChange();
    afx_msg void OnStockStopColorChange();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETOTHERDLG_H__3DAA868B_FBAF_11D2_A6A2_0080C87F62E5__INCLUDED_)
