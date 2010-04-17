#if !defined(AFX_DIAGETUPDATE_H__33C1A4E1_FA51_11D3_941E_0080C8E20736__INCLUDED_)
#define AFX_DIAGETUPDATE_H__33C1A4E1_FA51_11D3_941E_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaGetUpdate.h : header file


class CDiaGetUpdate : public CDialog
{

public:
	CDiaGetUpdate(CWnd* pParent = NULL);


	//{{AFX_DATA(CDiaGetUpdate)
	enum { IDD = IDD_UPDATEFILE };
	int		m_days;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CDiaGetUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CDiaGetUpdate)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAGETUPDATE_H__33C1A4E1_FA51_11D3_941E_0080C8E20736__INCLUDED_)
