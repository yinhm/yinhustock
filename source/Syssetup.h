#if !defined(AFX_SYSSETUP_H__3603CBA2_A124_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_SYSSETUP_H__3603CBA2_A124_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Syssetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSYSSETUP dialog

class CSYSSETUP : public CPropertyPage
{
	DECLARE_DYNCREATE(CSYSSETUP)

// Construction
public:
	CSYSSETUP();
	~CSYSSETUP();
    CTaiShanDoc *pDoc;

// Dialog Data
	//{{AFX_DATA(CSYSSETUP)
	enum { IDD = IDD_SZ_SYSTEMINFO };
	BOOL	m_AlarmSound;
	BOOL	m_AutoOrganizeData;
	BOOL	m_autoclose;
	BOOL	m_autoday;
	BOOL	m_showxline;
	BOOL	m_showyline;
	BOOL	m_autosavezb;
	BOOL	m_cjmxyester;
	BOOL	m_tjxgpower;
	BOOL	m_showinfohq;
	BOOL	m_startalert;
	BOOL	m_volpower;
	BOOL	m_backpower;
	BOOL	m_autominute;
	int		m_kline;
	int		m_fourgraph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSYSSETUP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSYSSETUP)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETUP_H__3603CBA2_A124_11D4_9426_0080C8E20736__INCLUDED_)
