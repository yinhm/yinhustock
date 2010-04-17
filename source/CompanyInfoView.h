#if !defined(AFX_COMPANYINFOVIEW_H__C25532C1_DF56_11D1_88E7_0080C8E20736__INCLUDED_)
#define AFX_COMPANYINFOVIEW_H__C25532C1_DF56_11D1_88E7_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCompanyInfoView : public CEditView
{
protected:
	CCompanyInfoView();           
	DECLARE_DYNCREATE(CCompanyInfoView)


public:

protected:
    CFont m_Font;

public:


	//{{AFX_VIRTUAL(CCompanyInfoView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);    
	//}}AFX_VIRTUAL


protected:
	virtual ~CCompanyInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	//{{AFX_MSG(CCompanyInfoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPANYINFOVIEW_H__C25532C1_DF56_11D1_88E7_0080C8E20736__INCLUDED_)
