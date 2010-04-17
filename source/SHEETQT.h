#if !defined(AFX_SHEETQT_H__9A5A8B49_BA79_11D2_9086_0000E8593F1A__INCLUDED_)
#define AFX_SHEETQT_H__9A5A8B49_BA79_11D2_9086_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETQT.h : header file
//
#include "resource.h"
#include <afxtempl.h>

class CTaiSysSetOthersPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiSysSetOthersPage)

public:
	CTaiSysSetOthersPage();
	~CTaiSysSetOthersPage();
	void UpdateImageList();
	int zbtotalnumber;					
	int index;							
	CArray<int,int> m_indexisoften;	          

	//{{AFX_DATA(CTaiSysSetOthersPage)
	enum { IDD = IDD_SHEETQT };
	CListCtrl	m_listselect;
	CListCtrl	m_listall;
	BOOL	m_autoclose;
	BOOL	m_autominute;
	BOOL    m_showxline;
	BOOL    m_showyline;
	int		m_maingraph;
	BOOL	m_autoday;
	int		m_fourgraph;
	int		m_kline;
	BOOL	m_AlarmSound;
	BOOL	m_AutoOrganizeData;
	BOOL	m_bAutoSaveParamOfJishu;
	BOOL	m_bHsTodayAddLast;
	BOOL	m_bDoPowerSelectingStock;
	BOOL	m_bShowInfoCrossing;
	BOOL	m_bStartAlertOnBegin;
	BOOL	m_bDoVolPowering;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTaiSysSetOthersPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CTaiSysSetOthersPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnDblclkListall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListselect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio4t();
	afx_msg void OnRadio9t();
	afx_msg void OnSetfocusListall(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusListselect(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETQT_H__9A5A8B49_BA79_11D2_9086_0000E8593F1A__INCLUDED_)
