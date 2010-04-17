#if !defined(AFX_FLOATEDIT_H__17464189_3A34_4D68_AE01_AF7AF060E0B2__INCLUDED_)
#define AFX_FLOATEDIT_H__17464189_3A34_4D68_AE01_AF7AF060E0B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFloatEdit : public CEdit
{

public:
	CFloatEdit();


public:


public:


	//{{AFX_VIRTUAL(CFloatEdit)
	//}}AFX_VIRTUAL


public:
	virtual ~CFloatEdit();

	
protected:
	//{{AFX_MSG(CFloatEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATEDIT_H__17464189_3A34_4D68_AE01_AF7AF060E0B2__INCLUDED_)
