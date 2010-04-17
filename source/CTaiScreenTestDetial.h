//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SCREENSTOCKTESTDETIAL_H__0D839826_62CE_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_SCREENSTOCKTESTDETIAL_H__0D839826_62CE_11D4_970B_0080C8D6450E__INCLUDED_

#include "CTaiShanDoc.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreenTestDetial.h : header file



class CTaiScreenTestDetial : public CDialog
{

public:
	CString m_stockname;
	SCREEN_DETAIL detail;
	void SetDetail(SCREEN_RESULT *chooseresult,CString stockname);
	SCREEN_RESULT *m_chooseresult;
	CTaiScreenTestDetial(CWnd* pParent = NULL);  


	//{{AFX_DATA(CTaiScreenTestDetial)
	enum { IDD = IDD_DIATJXGDETAIL };
	CButtonST	m_ok;
	CMSFlexGrid	m_flexgrid;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CTaiScreenTestDetial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CTaiScreenTestDetial)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENSTOCKTESTDETIAL_H__0D839826_62CE_11D4_970B_0080C8D6450E__INCLUDED_)
