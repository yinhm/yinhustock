#if !defined(AFX_SHEETWG_H__72D05F61_BA0F_11D2_9086_0000E8593F1A__INCLUDED_)
#define AFX_SHEETWG_H__72D05F61_BA0F_11D2_9086_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETWG.h : header file
#include "resource.h"
#include "CTaiShanDoc.h"

class CTaiSysSetInterfacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTaiSysSetInterfacePage)

public:
	CTaiSysSetInterfacePage();
	~CTaiSysSetInterfacePage();

	RECT     m_rectsample;                 
	LOGFONT  m_logfont,fontdefault;       
	COLORREF cr[systemcolorlength];
	LOGFONT  fr[fontstructlength];

	int     iLastComItem;
	COLORREF crbak[systemcolorlength];
	LOGFONT  frbak[fontstructlength];

	//{{AFX_DATA(CTaiSysSetInterfacePage)
	enum { IDD = IDD_SHEETWG };
	CButtonST	m_setfont;
	CComboBox	m_sheetwgcom1;
	CListBox	m_sheetwglist1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTaiSysSetInterfacePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL


protected:

	//{{AFX_MSG(CTaiSysSetInterfacePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSetcolor();
	afx_msg void OnSetfont();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETWG_H__72D05F61_BA0F_11D2_9086_0000E8593F1A__INCLUDED_)
