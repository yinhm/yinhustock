#if !defined(AFX_GUIZHOUTVVIEW_H__5F4F0AC5_4F2F_4393_BBD5_173D2CB37139__INCLUDED_)
#define AFX_GUIZHOUTVVIEW_H__5F4F0AC5_4F2F_4393_BBD5_173D2CB37139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuiZhouTvView : public CEditView
{
protected:
	CGuiZhouTvView();         
	DECLARE_DYNCREATE(CGuiZhouTvView)


public:


public:
	BOOL ShowInfo(CString strFile);


	//{{AFX_VIRTUAL(CGuiZhouTvView)
	protected:
	virtual void OnDraw(CDC* pDC);      
	//}}AFX_VIRTUAL


protected:
	virtual ~CGuiZhouTvView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	//{{AFX_MSG(CGuiZhouTvView)
		
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUIZHOUTVVIEW_H__5F4F0AC5_4F2F_4393_BBD5_173D2CB37139__INCLUDED_)
