#if !defined(AFX_MYMAINTB_H__4F771783_BC0C_11D2_9086_0000E8593F1A__INCLUDED_)
#define AFX_MYMAINTB_H__4F771783_BC0C_11D2_9086_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMYMAINTB : public CToolBarCtrl
{

public:
	CMYMAINTB();


public:


	int        buttonnumber;
	TBBUTTON   *m_tbbutton;


public:


	//{{AFX_VIRTUAL(CMYMAINTB)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL


public:
	virtual ~CMYMAINTB();


protected:
	//{{AFX_MSG(CMYMAINTB)
		
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMAINTB_H__4F771783_BC0C_11D2_9086_0000E8593F1A__INCLUDED_)
