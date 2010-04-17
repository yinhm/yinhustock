#if !defined(AFX_DLGMYXMZ_H__FC62D2DB_5ABA_4506_9EF3_4B5E15FAD0D6__INCLUDED_)
#define AFX_DLGMYXMZ_H__FC62D2DB_5ABA_4506_9EF3_4B5E15FAD0D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMYXMZ.h : header file
//
#define WM_MYMSG (WM_USER+1121)


class CDialogMYXMZ : public CDialog
{

public:
	CImageList m_imagelist;
	CStringList m_strlistData;
	CDialogMYXMZ(CWnd* pParent = NULL);   


	//{{AFX_DATA(CDialogMYXMZ)
	enum { IDD = IDD_MYXMZ };
	CListCtrl	m_mylist;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDialogMYXMZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDialogMYXMZ)
	afx_msg LRESULT DealMsg(WPARAM wParam,LPARAM lParam);	
	//}}AFX_MSG
	
virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYXMZ_H__FC62D2DB_5ABA_4506_9EF3_4B5E15FAD0D6__INCLUDED_)
