#if !defined(AFX_STOCKDETAILLIST_H__7E188421_08E3_11D2_88EE_0080C8E20736__INCLUDED_)
#define AFX_STOCKDETAILLIST_H__7E188421_08E3_11D2_88EE_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSharesDetailList.h : header file
//
#include "CTaiShanDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CSharesDetailList window

class CSharesDetailList : public CListCtrl
{

public:
	CSharesDetailList();


public:


public:
    int   RedOrGreen[2000];         
		                    
	int num;
	bool   m_IsF1;
    CTaiShanDoc* pDoc;

	//{{AFX_VIRTUAL(CSharesDetailList)
	//}}AFX_VIRTUAL


public:
	virtual ~CSharesDetailList();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );


protected:
	//{{AFX_MSG(CSharesDetailList)
	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKDETAILLIST_H__7E188421_08E3_11D2_88EE_0080C8E20736__INCLUDED_)
