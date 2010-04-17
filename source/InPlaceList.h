#if !defined(AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// InPlaceList.h : header file
//
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// 
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then acknowledgement must be made to the author of this file 
// (in whatever form you wish).
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////

#define IDC_COMBOEDIT 1001


class CComboEdit : public CEdit
{

public:
	CComboEdit();


public:


public:

	//{{AFX_VIRTUAL(CComboEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


public:
	virtual ~CComboEdit();


protected:
	//{{AFX_MSG(CComboEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


class CInPlaceList : public CComboBox
{
    friend class CComboEdit;


public:
	CInPlaceList(CWnd* pParent,      
                 CRect& rect,          
                 DWORD dwStyle,       
                 UINT nID,            
                 int nRow, int nColumn,
				 CStringArray& Items,   
                 CString sInitText,     
				 UINT nFirstChar);   


public:
   CComboEdit m_edit;


public:

	//{{AFX_VIRTUAL(CInPlaceList)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


public:
	virtual ~CInPlaceList();
protected:
    void EndEdit();


protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int		m_nNumLines;
	CString m_sInitText;
	int		m_nRow;
	int		m_nCol;
 	UINT    m_nLastChar; 
	BOOL	m_bExitOnArrows; 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACELIST_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
