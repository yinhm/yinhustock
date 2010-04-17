#if !defined(AFX_ALCINDEX_H__C7754381_1C1C_11D4_8829_00400552E583__INCLUDED_)
#define AFX_ALCINDEX_H__C7754381_1C1C_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CTaiScreenParent.h"



class CCalcIndex : public CTaiScreenParent
{

public:
	CCalcIndex(CWnd* pParent = NULL);  
	CCalcIndex(Index_Technique *pIndex,UINT nID,CWnd* pParent = NULL);   
    
	Index_Technique m_pIndexTechnique;
	UINT m_nColumnId;
	void CalcIndexIndexData();
	BOOL m_bIsCalc;



	//{{AFX_DATA(CCalcIndex)
	enum { IDD = IDD_COMPUTEINDEX };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA




	//{{AFX_VIRTUAL(CCalcIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	//}}AFX_VIRTUAL
	afx_msg LRESULT OnCalcIndex(WPARAM wParam, LPARAM lParam);



protected:


	//{{AFX_MSG(CCalcIndex)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALCINDEX_H__C7754381_1C1C_11D4_8829_00400552E583__INCLUDED_)
