#if !defined(AFX_LMBSETMAXMIN_H__2F230361_913E_11D3_9203_0080C8E1242B__INCLUDED_)
#define AFX_LMBSETMAXMIN_H__2F230361_913E_11D3_9203_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineSetMaxMin.h : header file

#include "FloatEdit.h"

class CTaiShanKlineShowView;
class CTaiKlineSetMaxMin : public CDialog
{

public:
	CTaiKlineSetMaxMin(CWnd* pParent = NULL);  
	CTaiShanKlineShowView* pView ;


	//{{AFX_DATA(CTaiKlineSetMaxMin)
	enum { IDD = IDD_DIALOG_SETMAXMIN_L };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CFloatEdit	m_conMin5;
	CFloatEdit	m_conMax5;
	CFloatEdit	m_conMin4;
	CFloatEdit	m_conMax4;
	CFloatEdit	m_conMin3;
	CFloatEdit	m_conMax3;
	CFloatEdit	m_conMin2;
	CFloatEdit	m_conMax2;
	CFloatEdit	m_conMin1;
	CFloatEdit	m_conMax1;
	float	m_fMax1;
	float	m_fMin1;
	float	m_fMax2;
	float	m_fMin2;
	float	m_fMax3;
	float	m_fMin3;
	float	m_fMax4;
	float	m_fMin4;
	float	m_fMax5;
	float	m_fMin5;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CTaiKlineSetMaxMin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CTaiKlineSetMaxMin)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBSETMAXMIN_H__2F230361_913E_11D3_9203_0080C8E1242B__INCLUDED_)
