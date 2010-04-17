#if !defined(AFX_LDIALOGSHIDUANTJ_H__467F7901_A7A4_11D2_91E0_0080C8E1242B__INCLUDED_)
#define AFX_LDIALOGSHIDUANTJ_H__467F7901_A7A4_11D2_91E0_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDialogShiDuanTJ.h : header file
//
class	CTaiShanKlineShowView;

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogShiDuanTJ dialog

class CTaiKlineDialogShiDuanTJ : public CDialog
{
// Construction
private:
	int m_nTimeStyle;	
public:
	int m_nKlineKind2;
	CTaiShanKlineShowView* pView;
	void OnDTNotify(NMHDR *pHdr, LRESULT *pRes);
	void OnDTNotify2(NMHDR *pHdr, LRESULT *pRes);
	void CreateTimeCtl();
	CDateTimeCtrl m_DateTime;
	CDateTimeCtrl m_DateTime2;
	int m_foot;
	CTime m_tmB;
	CTime m_tmE;
	CString m_begin;
	CString m_end;


	CTaiKlineDialogShiDuanTJ(CWnd* pParent = NULL,int nTimeStyle=0);   // standard constructor


	//{{AFX_DATA(CTaiKlineDialogShiDuanTJ)
	enum { IDD = IDD_DIALOG_SHIDUAN_TJ };
	CButtonST	m_ok;
	CString	m_chengBenTJ;
	CString	m_close;
	CString	m_high;
	CString	m_junjia;
	CString	m_low;
	CString	m_open;
	CString	m_num;
	CString	m_vol;
	CString	m_zhDie;
	CString	m_zhDieFu;
	CString	m_amount;
	CString	m_huanShou;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDialogShiDuanTJ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDialogShiDuanTJ)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDIALOGSHIDUANTJ_H__467F7901_A7A4_11D2_91E0_0080C8E1242B__INCLUDED_)
