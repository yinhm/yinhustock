#if !defined(AFX_MANAGERCHOOSE_H__BD1C43A3_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERCHOOSE_H__BD1C43A3_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_ADDSTOCK WM_USER+1200


class CSuperviseChoose : public CDialog
{

public:
	CSuperviseChoose(CWnd* pParent = NULL); 
public:
	CImageList * m_pImageList; 
	CImageList * m_pImageListSmall;
    CTaiShanDoc *m_pDoc;


	//{{AFX_DATA(CSuperviseChoose)
	enum { IDD = IDD_CHOOSE_MANAGER };
	CButtonST	m_ok;
	CButtonST	m_btn2;
	CButtonST	m_btn1;
	CListCtrl	m_MyList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSuperviseChoose)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
   BOOL InsertStockToList(char *name,char *code,int nKind);
   void FillToListCtrlHead();

	//{{AFX_MSG(CSuperviseChoose)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAddStockToChoose();
	afx_msg void OnDeleteStockFromChoose();
	//}}AFX_MSG
	afx_msg LRESULT OnAddStockCode(WPARAM wParam, LPARAM lParam); 

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGERCHOOSE_H__BD1C43A3_5D50_11D4_8829_00400552E583__INCLUDED_)
