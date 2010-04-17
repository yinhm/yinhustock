#if !defined(AFX_MYRICHEDITCTRL_H__0D24BE3F_54E2_4AE1_BFD4_F0AEEB477CF3__INCLUDED_)
#define AFX_MYRICHEDITCTRL_H__0D24BE3F_54E2_4AE1_BFD4_F0AEEB477CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiTestRichEditCtrl : public CRichEditCtrl
{

public:
	CTaiTestRichEditCtrl();


public:


public:


	//{{AFX_VIRTUAL(CTaiTestRichEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


public:
	virtual ~CTaiTestRichEditCtrl();


protected:
	//{{AFX_MSG(CTaiTestRichEditCtrl)
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRICHEDITCTRL_H__0D24BE3F_54E2_4AE1_BFD4_F0AEEB477CF3__INCLUDED_)
