#if !defined(AFX_INFOVIEW_H__040F8FA5_ED6A_11D1_88EB_0080C8E20736__INCLUDED_)
#define AFX_INFOVIEW_H__040F8FA5_ED6A_11D1_88EB_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxrich.h>

class CTaiShanDoc;
class CInfoView : public CRichEditView
{

public:
	CInfoView();
	DECLARE_DYNCREATE(CInfoView)


public:


public:


	//{{AFX_VIRTUAL(CInfoView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL


public:
	virtual ~CInfoView();
	CString m_OldStockSymbol;
	CTaiShanDoc* pDoc;


protected:
	//{{AFX_MSG(CInfoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOVIEW_H__040F8FA5_ED6A_11D1_88EB_0080C8E20736__INCLUDED_)
