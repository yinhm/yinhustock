#if !defined(AFX_TECHDLGIndex_TONGJI_H__D984BABD_F2FF_46CF_B439_A963978AA92D__INCLUDED_)
#define AFX_TECHDLGIndex_TONGJI_H__D984BABD_F2FF_46CF_B439_A963978AA92D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgZhibiaoTongji.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgZhibiaoTongji dialog
class CTaiShanKlineShowView;
class CTaiKlineShowKline;
class CTaiKlineDlgZhibiaoTongji : public CDialog
{
// Construction
public:
	int m_nKlineKind2 ;
	float TongJi(int nFlags);
	CTaiKlineDlgZhibiaoTongji(CWnd* pParent = NULL);   // standard constructor

	int m_nSon;

	CTaiShanKlineShowView* m_pView;
// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgZhibiaoTongji)
	enum { IDD = IDD_DIALOG1 };
	CDateTimeCtrl	m_DateTime2;
	CDateTimeCtrl	m_DateTime;
	CComboBox	m_conZhibiao;
	CTime	m_tmB;
	CTime	m_tmE;
	DWORD	m_num;
	float	m_fTotal;
	float	m_fAverage;
	float	m_fHigh;
	float	m_fLow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgZhibiaoTongji)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgZhibiaoTongji)
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeDatetimepicker3(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGIndex_TONGJI_H__D984BABD_F2FF_46CF_B439_A963978AA92D__INCLUDED_)
