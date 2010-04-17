#if !defined(AFX_SETPARAM_H__EC8E1885_2D61_11D4_8829_00400552E583__INCLUDED_)
#define AFX_SETPARAM_H__EC8E1885_2D61_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetParam.h : header file

class SetParam : public CDialog
{

public:
	float nBig[4];
	float nBigMax[4];
	float nParam[10];
	float nBigGB[4];
	float nBigMaxGB[4];
	float nParamGB[10];
	BOOL IsGb;
	BOOL IsGbParam;
public:
	SetParam(CWnd* pParent = NULL);   
	SetParam(float *big,float *bigMax,float *param,float *bigGB,float *bigMaxGB,float *paramGB,BOOL isgb,CWnd* pParent = NULL);   // standard constructor
    BOOL CheckParam();

	//{{AFX_DATA(SetParam)
	enum { IDD = IDD_SET2_PARAM };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	float	m_Big1;
	float	m_Big2;
	float	m_Big3;
	float	m_Big4;
	float	m_BigMax1;
	float	m_BigMax2;
	float	m_BigMax3;
	float	m_BigMax4;
	float	m_Param0;
	float	m_Param1;
	float	m_Param2;
	float	m_Param3;
	float	m_Param4;
	float	m_Param6;
	float	m_Param5;
	float	m_Param7;
	float	m_Param8;
	int		m_Radio;
	int		m_Radio1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(SetParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(SetParam)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPARAM_H__EC8E1885_2D61_11D4_8829_00400552E583__INCLUDED_)
