#if !defined(AFX_SORTINDEX_H__110C7021_1503_11D4_8829_00400552E583__INCLUDED_)
#define AFX_SORTINDEX_H__110C7021_1503_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortIndex.h : header file

#include "CTaiScreenParent.h"

class SortIndex : public CTaiScreenParent
{

public:
	SortIndex(CWnd* pParent = NULL);  
	SortIndex(IndexDataInfo *pIndexSave,UINT nID,BOOL IsInit,BOOL IsChanged=TRUE,CWnd* pParent = NULL);   // standard constructor
	~SortIndex();
	void ExpandDialog(int nResourceID,BOOL bExpand); 
	void EnableVisibleChildren();                 
	void InitTechniqueIndex(CFormularContent*	pIndex,BOOL IsInit=FALSE); 
	void FillTechniqueToListCtrl();                             
	void CalcTechniqueIndexData();                            


	IndexDataInfo m_nIndexSave;                                  
	CString m_sLTypeName;                               
	UINT m_nColumnId;                                
	BOOL m_bIsInit;                                   
	CImageList * m_pImageList;                         
	CImageList * m_pImageListSmall;                  
	BOOL m_bIsChangeTechnique;                 
	BOOL m_bIsCalc;


	//{{AFX_DATA(SortIndex)
	enum { IDD = IDD_INDEXSORT };
	CProgressCtrl	m_Progress;
	CListCtrl	m_TechniqueList;
	CEdit	m_HelpCtl;

	CString	m_periodstr;
	CButtonST	m_btOK;
	CButtonST	m_btCancel;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(SortIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:


	//{{AFX_MSG(SortIndex)
	virtual BOOL OnInitDialog();
	afx_msg void OnTechnique();
	virtual void OnOK();
	afx_msg void OnClick(NMHDR* wParam, LRESULT* lParam);
	afx_msg void OnClose();
	virtual void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTINDEX_H__110C7021_1503_11D4_8829_00400552E583__INCLUDED_)
