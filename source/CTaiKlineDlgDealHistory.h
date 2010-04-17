#if !defined(AFX_LMBDLGDEALHISTORY_H__A3A59948_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
#define AFX_LMBDLGDEALHISTORY_H__A3A59948_FF03_11D3_96E9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgDealHistory.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgDealHistory dialog
class CTaiKlineFileHS;
class CTaiKlineMin1;
class CTaiShanKlineShowView;
class CTaiKlineDlgDealHistory : public CDialog
{
// Construction
public:
	void SetCheck1(int n);
	CTaiShanKlineShowView * pView;
	static bool GetBuySellInfoPerFoot(CString symbol,CReportData* pDat, int nFoot, CBuySellList& buySellList, bool bIndex);
	
//	bool GetBuySellInfoMin1(CString symbol,CReportData* pDat, int nFoot, CBuySellList& buySellList, bool bIndex);
	bool GetBuySellInfo(CString symbol,int stkKind,CReportData* pDat,int nFoot, CBuySellList& buySellList,bool bIndex);
	
	bool HistoryAStep(int time);
	

	static bool HistoryOpen(CTaiShanKlineShowView *pWnd,CString * fileName = NULL);
	void OpenFile();

	bool OnDoGo(bool bGoHead = true,bool bDoReal = true);
	
	bool m_bHistorySynchrostep;
	static void StringToTime(CTime& tm,CString s);
	
	bool InitDoFenshi(CTaiKlineMin1* pdoFen,CString symbol,int stkKind);
	

	CReportData m_S0;
	CReportData m_S1;
	Tidxd* m_Tidx[2];
	Rsdn1* m_Nidx[2];

	CString m_fileName;
	CString m_oldSymbol;
	int m_oldStkKind;
	CTaiKlineFileHS*	m_fileHsSh;
	CTaiKlineFileHS*	m_fileHsSz;
	bool	m_bShowHistory;

	CTaiKlineDlgDealHistory(CWnd* pParent = NULL);   

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgDealHistory)
	enum { IDD = IDD_DEAL_HISTORY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgDealHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnToolTipNotify(UINT id,NMHDR *pTTTStruct,LRESULT *pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgDealHistory)
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
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CToolBar    m_wndToolBar;
	CToolTipCtrl m_wndToolTip; 
	CTaiKlineFileHS* GetFilePointer (CString symbol,int stkKind);
	void GetCdat1Data(CString symbol,int stkKind, CReportData* pDat,bool bIndex = true,CTaiKlineMin1 *pdoFen = NULL);
	
	void OnFileChanged();

	void PrepareData(CTaiKlineMin1 *pdoFen,CString symbol,int stkKind);
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBDLGDEALHISTORY_H__A3A59948_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
