//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANAGEINVEST_H__E646B0F1_B52D_490A_BE68_4E2041A4BDC1__INCLUDED_)
#define AFX_MANAGEINVEST_H__E646B0F1_B52D_490A_BE68_4E2041A4BDC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct Invest
{
	unsigned uType;
	union
	{
		struct _Transact
		{
			unsigned uTime;
			int nWay;
			char chSymbol[8];
			int nKind;
			char chName[10];
			double dPrice;
			double dVolume;
			double dFee;
		}Transact;
		struct _Money
		{
			unsigned uTime;
			int nWay;
			double dAmount;
		}Money;
	};
};


struct AccountID
{
	char chName[16];
	char chPassword[16];
	unsigned uRecords;
	unsigned uUserType;
	char chRes[8];
};


class CSuperviseInvest : public CDialog
{

public:
	int m_nKind;
	static void AlarmCallInvest(SymbolKindArr & SymbolArray,CString AccountName,CString Password);
	double GetFeeRate(unsigned uKind,char chSymbol[]);
	double m_dLeftMoney;
	CString m_szFileName;
	AccountID m_Account;
	char m_Password[16];
	Invest* m_pInvest;
	double m_ShRate[5][5]; 
	double m_SzRate[5][5];

	void DisplayList( unsigned nTab);
	CSuperviseInvest(CWnd* pParent = NULL);

	//{{AFX_DATA(CSuperviseInvest)
	enum { IDD = IDD_MANAGEINVEST };
	CButtonST	m_cancel;
	CButtonST	m_fee;
	CButtonST	m_edit;
	CButtonST	m_delete;
	CButtonST	m_add;
	CTabCtrl	m_TabCtrl;
	CMSFlexGrid	m_MsFlexGrid;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSuperviseInvest)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CSuperviseInvest)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnAdd();
	afx_msg void OnFee();
	virtual void OnCancel();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnKeyDownMsflexgrid1(short FAR* KeyCode, short Shift);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEINVEST_H__E646B0F1_B52D_490A_BE68_4E2041A4BDC1__INCLUDED_)
