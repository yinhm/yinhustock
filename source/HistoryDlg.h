#if !defined(AFX_HISTORYDLG_H__7E61FC06_C873_11D1_968C_8B4A225F8378__INCLUDED_)
#define AFX_HISTORYDLG_H__7E61FC06_C873_11D1_968C_8B4A225F8378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HistoryFile.h"

class CTaiShanReportView;
class CHistoryDlg : public CDialog,public CHistoryFile
{

public:
	static void DoHistoryOne(STOCKDATASHOW *pData, int nFoot,CTaiShanReportView* pViewIn);
	static void DoHistory(STOCKDATASHOW *pData, int nCount,CTaiShanReportView* pViewIn);
	static void Open(CTaiShanReportView* pViewIn);
	CTaiShanReportView* pView ;
	CToolBar    m_wndToolBar;
	CToolTipCtrl m_wndToolTip; 
	CHistoryDlg(CWnd* pParent = NULL);  


	//{{AFX_DATA(CHistoryDlg)
	enum { IDD = IDD_DEAL_HISTORY };
	
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);  
	virtual BOOL OnToolTipNotify(UINT id,NMHDR *pTTTStruct,LRESULT *pResult);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CHistoryDlg)
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistoryHide();
	afx_msg void OnHistoryGohead();
	afx_msg void OnHistoryGoback();
	afx_msg void OnHistoryClose();
	afx_msg void OnHistoryAStep();
	afx_msg void OnHistorySwitch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool OnDoGo(bool bGoHead);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYDLG_H__7E61FC06_C873_11D1_968C_8B4A225F8378__INCLUDED_)
