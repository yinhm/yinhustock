#if !defined(AFX_MYRICHVIEW_H__5E8F69E1_EC9E_11D1_88EB_0080C8E20736__INCLUDED_)
#define AFX_MYRICHVIEW_H__5E8F69E1_EC9E_11D1_88EB_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxrich.h>

class CTaiTestRichView : public CRichEditView
{

public:
	CTaiTestRichView();
    DECLARE_DYNCREATE(CTaiTestRichView)
    BOOL m_bF9ORF10;

public:
   CRect m_rectMargin;

public:

	//{{AFX_VIRTUAL(CTaiTestRichView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_VIRTUAL

public:
	virtual ~CTaiTestRichView();

protected:
	//{{AFX_MSG(CTaiTestRichView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFilePrint();
	afx_msg void OnRichCopy();
	afx_msg void OnNewF10();
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnUpdateNewF10(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CTaiShanApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYRICHVIEW_H__5E8F69E1_EC9E_11D1_88EB_0080C8E20736__INCLUDED_)
