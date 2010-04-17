#if !defined(AFX_CATALOGSTATIC_H__5959A89F_851F_455A_B4F2_1157A817DBF2__INCLUDED_)
#define AFX_CATALOGSTATIC_H__5959A89F_851F_455A_B4F2_1157A817DBF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CatalogStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCatalogStatic window

class CCatalogStatic : public CStatic
{

public:
	CCatalogStatic();
	virtual ~CCatalogStatic();


public:


public:



	//{{AFX_VIRTUAL(CCatalogStatic)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


protected:
	void PositionWindow();
	void SetDefaultCursor();

public:
	BOOL	 m_bVisited;					

protected:	
	COLORREF m_crLinkColor;				
	COLORREF m_crVisitedColor;			
	COLORREF m_crHoverColor;			
	BOOL	 m_bOverControl;				
	BOOL	 m_bUnderline;				
	BOOL	 m_bAdjustToFit;				
	CFont	 m_Font;					
	HCURSOR  m_hCursor;					


protected:
	//{{AFX_MSG(CCatalogStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATALOGSTATIC_H__5959A89F_851F_455A_B4F2_1157A817DBF2__INCLUDED_)
