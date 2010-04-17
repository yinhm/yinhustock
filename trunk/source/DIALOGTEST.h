#if !defined(AFX_DIALOGTEST_H__1BEB0B21_C0E8_11D2_91D7_0080C8E1242B__INCLUDED_)
#define AFX_DIALOGTEST_H__1BEB0B21_C0E8_11D2_91D7_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIALOGTEST.h : header file
//
#include "resource.h"


class CDialogLOGTEST : public CDialog
{

public:
	CDialogLOGTEST(CWnd* pParent = NULL);  


	//{{AFX_DATA(CDialogLOGTEST)
	enum { IDD = IDD_CSmy };
	
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogLOGTEST)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDialogLOGTEST)
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTEST_H__1BEB0B21_C0E8_11D2_91D7_0080C8E1242B__INCLUDED_)
