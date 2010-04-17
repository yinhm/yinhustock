#if !defined(AFX_LDLGCHANGEINDEXPARAM_H__EB53B3C1_84EA_11D1_91F3_0080C8E1242B__INCLUDED_)
#define AFX_LDLGCHANGEINDEXPARAM_H__EB53B3C1_84EA_11D1_91F3_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgChangeIndexParam.h : header file
//
#include "CFormularContent.h"
#include "FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgChangeIndexParam dialog

class CTaiShanKlineShowView;
class CTaiKlineDlgChangeIndexParam : public CDialog
{
// Construction
public:
	static void SetParamLjishu(CFormularContent* jishunow,int nKlineType,int nParam,BOOL bSaveAll,float f);
	static float StringToFloat(CString s);

	CTaiShanKlineShowView* pView;
	CTaiKlineDlgChangeIndexParam(CWnd* pParent = NULL);
	bool m_bInitialed;
	CFormularContent *m_pJishuNow;
	float m_paramDefault[8];
	int m_nHit;
	CPoint m_pt;
	CString m_name;
	void UpdateParamArea();


	//{{AFX_DATA(CTaiKlineDlgChangeIndexParam)
	enum { IDD = IDD_DIALOG_CHANGE_PARA };
	CButtonST	m_ok;
	CButtonST	m_set;
	CButtonST	m_restore;
	CFloatEdit	m_floatEditP32;
	CFloatEdit	m_floatEditN2;
	CFloatEdit	m_floatEditP5;
	CFloatEdit	m_floatEditLl2;
	CStatic	m_cstaticp2;
	CStatic	m_cstaticp1;
	CStatic	m_cstaticp4;
	CStatic	m_cstaticp3;
	CSpinButtonCtrl	m_cspin4;
	CSpinButtonCtrl	m_cspin3;
	CSpinButtonCtrl	m_cspin2;
	CSpinButtonCtrl	m_cspin1;
	CFloatEdit	m_controlp4;
	CFloatEdit	m_controlp3;
	CFloatEdit	m_controlp2;
	CFloatEdit	m_controlp1;
	BOOL	m_bSaveAllPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgChangeIndexParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgChangeIndexParam)
	afx_msg void OnButtonRestore();
	afx_msg void OnButtonSet();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditN();
	afx_msg void OnChangeEditLl2();
	afx_msg void OnChangeEditLl22();
	afx_msg void OnChangeEditP3();
	afx_msg void OnChangeEditP4();
	afx_msg void OnChangeEditN2();
	afx_msg void OnChangeEditP3P2();
	afx_msg void OnChangeEditLl3P5();
	afx_msg void OnCheckSaveParam();
	afx_msg void OnSetfocusEditLl();
	afx_msg void OnSetfocusEditLl2();
	afx_msg void OnSetfocusEditLl3P4();
	afx_msg void OnSetfocusEditLl3P5();
	afx_msg void OnSetfocusEditN();
	afx_msg void OnSetfocusEditN2();
	afx_msg void OnSetfocusEditP3P();
	afx_msg void OnSetfocusEditP3P2();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateParam(int nID,int nParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDLGCHANGEINDEXPARAM_H__EB53B3C1_84EA_11D1_91F3_0080C8E1242B__INCLUDED_)
