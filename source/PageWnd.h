// by ÓáÃ÷Â¼
#if !defined(AFX_MYWND_H__DCABFF9D_9ABF_11D1_8C3A_0000E823CF90__INCLUDED_)
#define AFX_MYWND_H__DCABFF9D_9ABF_11D1_8C3A_0000E823CF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SHOWTYPE  (WM_USER + 100)
#define MAX_PAGE 7

struct DynaMenuDef {
   CString 	m_nString;				
   UINT     m_nID;                   
};

typedef CTypedPtrList<CPtrList, DynaMenuDef*> m_DynaMenuDef;
class CSheetScrollBar;
class CPageWnd : public CWnd
{

public:
	CPageWnd();


public:
	CSheetScrollBar * m_scrollbar; 
	int m_nActivePage; 
	UINT m_nBarLen;
	bool m_bMove;
	CFont m_Font;



public:

	int m_nMovebar;
    CPoint m_barstartplace; 
	CPoint m_barendplace;  
	int m_nBarlength;
   	BOOL m_bRefrash; 




public:

	//{{AFX_VIRTUAL(CPageWnd)
	public:
	//}}AFX_VIRTUAL


public:	
	void DoRButtonDown(UINT nFlags, CPoint point);
	
	void ReplacePage(int nPage,CString ReplaceTitle);
	
	void ShowAllPage(CDC *pDC);
	CString GetPageTitle(int nPage); 
	int GetActivePage(); 
    void CPageWnd::SetPageTitle(CString m_title,int index);
	virtual ~CPageWnd();


protected:

	void DrawPageBox(CDC *pDC,int x,int cx,BOOL bActive);
	CScrollBar  m_PageBtn;

	//{{AFX_MSG(CPageWnd)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	
	void InitAllPara();

	void ShowPageTitle(CDC *pDC,CString &str,LPRECT lpRect,UINT nFormat,int nPage);
	CStringArray m_strPageTitle;
	
	int m_nBeginX;
  
	int m_nFirstPage;

	int m_nEndPage;
  
	int PageWidth[MAX_PAGE];

	m_DynaMenuDef m_StockType;
	CMenu *m_TypeMenu;
	void InitTypeMenu(int mode);
	void ExecuteMenu(int mode);
    BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
					AFX_CMDHANDLERINFO* pHandlerInfo);

public:
	virtual BOOL Create(LPCTSTR szTitle, LONG style, const RECT& rect, CWnd* parent);

protected:
	BOOL DoLButtonDown(UINT nFlags, CPoint points);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
}; 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWND_H__DCABFF9D_9ABF_11D1_8C3A_0000E823CF90__INCLUDED_)
