#if !defined(AFX_ALERTSET_H__F218F662_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTSET_H__F218F662_69E0_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CTaiScreenParent.h"
#include "CAlertSystem.h"

class CAlertSet : public CTaiScreenParent
{

public:
	void NotifySubclass();
	int m_nItem;
	void GetItemData(ALERT_SET_ITEM &itemData);
	void SetInitData(ALERT_SET_ITEM& itemData);
	BYTE m_btAddtional[4];

	CAlertSet(CWnd* pParent = NULL);   


	//{{AFX_DATA(CAlertSet)
	enum { IDD = IDD_TJXG_ALARM_SET };
	CButtonST	m_settingAdded;
	CButtonST	m_buttonExpl;
	CListCtrl	m_liststockrange;
	CString	m_sTitle;
	CButtonST m_btAddS;
	CButtonST m_btDeleteS;
	CButtonST m_btOK;
	CButtonST m_btCancel;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CAlertSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CAlertSet)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDeleteStocks2();
	afx_msg void OnAddStocksParent();
	afx_msg void OnSettingAdded();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnKeydownStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTSET_H__F218F662_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
