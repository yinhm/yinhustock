#if !defined(AFX_SELECTSTOCKHSDETAIL2_H__47966218_8900_43DC_A526_C0C743111B8B__INCLUDED_)
#define AFX_SELECTSTOCKHSDETAIL2_H__47966218_8900_43DC_A526_C0C743111B8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStockHsDetail2.h : header file

#include "SelectStock.h"

class CSelectStockHsDetail2 : public SelectStock
{

public:
	void GetSymbolArray(SymbolKindArr &sSymbolArray);
	BOOL QuickSort(int *nItem,int low,int high);
	CSelectStockHsDetail2(CWnd* pParent = NULL);   
	CStringArray m_FileArray;


	//{{AFX_DATA(CSelectStockHsDetail2)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_SelectedStockList;
	
	//}}AFX_DATA
	//{{AFX_VIRTUAL(CSelectStockHsDetail2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:


	//{{AFX_MSG(CSelectStockHsDetail2)
	afx_msg void OnDel();
	afx_msg void OnAdd2();
	afx_msg void OnChangeday();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSTOCKHSDETAIL2_H__47966218_8900_43DC_A526_C0C743111B8B__INCLUDED_)
