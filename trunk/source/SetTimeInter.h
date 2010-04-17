#if !defined(AFX_SETTIMEINTER_H__3DAA8689_FBAF_11D2_A6A2_0080C87F62E5__INCLUDED_)
#define AFX_SETTIMEINTER_H__3DAA8689_FBAF_11D2_A6A2_0080C87F62E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetTimeInter.h : header file

class CSetTimeInter : public CDialog
{

public:
	CSetTimeInter(CWnd* pParent = NULL); 


	//{{AFX_DATA(CSetTimeInter)
	enum { IDD = IDD_TIMESETDLG };
	CSpinButtonCtrl	m_TimeSpin;
	int		m_TimeInter;
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CSetTimeInter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CSetTimeInter)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTIMEINTER_H__3DAA8689_FBAF_11D2_A6A2_0080C87F62E5__INCLUDED_)
