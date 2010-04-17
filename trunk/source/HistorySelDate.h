#if !defined(AFX_HISTORYSELDATE_H__549C22E1_4FB2_11D6_968C_E8A724F73E7B__INCLUDED_)
#define AFX_HISTORYSELDATE_H__549C22E1_4FB2_11D6_968C_E8A724F73E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHistorySelDate : public CDialog
{

public:
	CHistorySelDate(CWnd* pParent = NULL); 


	//{{AFX_DATA(CHistorySelDate)
	enum { IDD = IDD_DIALOG_SELECT_DATE };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CTime	m_tm;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CHistorySelDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CHistorySelDate)
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYSELDATE_H__549C22E1_4FB2_11D6_968C_E8A724F73E7B__INCLUDED_)
