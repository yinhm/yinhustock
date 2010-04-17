#if !defined(AFX_MAINFRMD_H__FA099AA6_9AD5_11D1_B032_000021002C16__INCLUDED_)
#define AFX_MAINFRMD_H__FA099AA6_9AD5_11D1_B032_000021002C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class mainFrmD : public CDialog
{

public:
	mainFrmD(CWnd* pParent = NULL);  


	//{{AFX_DATA(mainFrmD)
	enum { IDD = IDD_DIALOGMAINFRAME };

	//}}AFX_DATA



	//{{AFX_VIRTUAL(mainFrmD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(mainFrmD)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMD_H__FA099AA6_9AD5_11D1_B032_000021002C16__INCLUDED_)
