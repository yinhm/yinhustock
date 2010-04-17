#if !defined(AFX_BJDLGCHENGBENSETTING_H__4C7C9E8D_A7FE_4E79_B991_8D84721B6643__INCLUDED_)
#define AFX_BJDLGCHENGBENSETTING_H__4C7C9E8D_A7FE_4E79_B991_8D84721B6643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiKlineMoveChenben;

class BjDlgChengBenSetting : public CDialog
{

public:
	BjDlgChengBenSetting(CWnd* pParent = NULL);   
	virtual ~BjDlgChengBenSetting();

	CBrush* m_brush;

	CTaiKlineMoveChenben * m_pChengBen;


	//{{AFX_DATA(BjDlgChengBenSetting)
	enum { IDD = IDD_BJ_CHENGBEN_SETTING };
	CStatic	m_color;
	CComboBox	m_comb;
	CTabCtrl	m_tab;
	UINT	m_nDays;
	//}}AFX_DATA




	//{{AFX_VIRTUAL(BjDlgChengBenSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(BjDlgChengBenSetting)
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BJDLGCHENGBENSETTING_H__4C7C9E8D_A7FE_4E79_B991_8D84721B6643__INCLUDED_)
