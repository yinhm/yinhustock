
#if !defined(AFX_CHILDFRM_H__1568198A_DAB3_11D1_BF39_0000E86564F7__INCLUDED_)
#define AFX_CHILDFRM_H__1568198A_DAB3_11D1_BF39_0000E86564F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNineShowFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CNineShowFrame)
public:
	CNineShowFrame();
	virtual void OnUpdateFrameTitle(BOOL nAddToTitle);

public:


public:

	//{{AFX_VIRTUAL(CNineShowFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

public:
	virtual ~CNineShowFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	//{{AFX_MSG(CNineShowFrame)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__1568198A_DAB3_11D1_BF39_0000E86564F7__INCLUDED_)
