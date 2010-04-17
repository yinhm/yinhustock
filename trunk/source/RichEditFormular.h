#if !defined(AFX_RICHEDITFORMULAR_H__1A1A72C1_0498_11D7_9690_D53117962B72__INCLUDED_)
#define AFX_RICHEDITFORMULAR_H__1A1A72C1_0498_11D7_9690_D53117962B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RichEditFormular.h : header file
#include "MyTipWin.h"
#include "AutoRichEditCtrl.h"


class CRichEditFormular : public CAutoRichEditCtrl
{

public:
	CRichEditFormular();
	CString GetWordByOffset(int nOffset);


public:
	CTaiTestTipWin m_TitleTip;


public:

	//{{AFX_VIRTUAL(CRichEditFormular)
	//}}AFX_VIRTUAL


public:
	virtual ~CRichEditFormular();


protected:
	//{{AFX_MSG(CRichEditFormular)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RICHEDITFORMULAR_H__1A1A72C1_0498_11D7_9690_D53117962B72__INCLUDED_)
