#if !defined(AFX_LKEYBOARDDIAL_H__47E83165_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
#define AFX_LKEYBOARDDIAL_H__47E83165_C70C_11D2_91D8_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class LKeyBoardDial : public CDialog
{

public:
	LKeyBoardDial(CWnd* pParent = NULL);   


	//{{AFX_DATA(LKeyBoardDial)
	enum { IDD = IDD_DIALOG_KEYB };
	
	//}}AFX_DATA

	//{{AFX_VIRTUAL(LKeyBoardDial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(LKeyBoardDial)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LKEYBOARDDIAL_H__47E83165_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
