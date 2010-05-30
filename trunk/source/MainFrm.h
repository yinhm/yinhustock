
#if !defined(AFX_MAINFRM_H__61A31CED_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_MAINFRM_H__61A31CED_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CTaiShanKlineShowView.h"
#include "TestDialogbar.h"
#include "CTaiScreen.h"
#include "CTaiScreenTest.h"
#include "CStatusBarTai.h"
#include "CTaiKlineMemFile.h"
#include "DialogCwdLeftPop.h"
#include "ClientTransmitManageWnd.h"

class CTaiShanDoc ;
class CMDIChildWnd ;

#define	 Gp_Msg_StkData			WM_APP + 1
class CAlertSystem;
class CAlertPopDlg;
class CTaiKlineDlgNeuralLearn;

class CDialogMYXMZ;

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)
public:
	CMainFrame();

	CTaiToolBar m_TestDialogbar; 
	CDialogCwdLeftPop m_dlgLeftPop;

#ifdef WIDE_NET_VERSION
	static CClientTransmitManageWnd* m_pClientTransmitManageWnd;

	static BOOL m_bLogonSuccess;
#else
	CSTKDRV	gSTOCKDLL;
#endif
private:
#ifdef WIDE_NET_VERSION
	BOOL m_bLogonInterface;
#endif


public:
	CAlertPopDlg* m_pDlgAlarmPop;
	CAlertSystem* m_pDlgAlarm;
	bool m_bAlarmExec;
	static CTaiShanDoc *m_taiShanDoc;   
	CMDIChildWnd *m_MDIChildWnd;          
	CTaiShanKlineShowView* pVwbaseView;
	int nLKind[11];


	CToolBar* m_drawLineToolBar; 

	void        Drawline();

	int         m_EnableF10;           


	BOOL		m_EnableF9;             

	BOOL        m_F9ORF10;                  

	bool        m_BaseInfo;                
	BOOL        m_StopCuoHe;                  

	IndexDataInfo m_index_save;

	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
public:
	afx_msg void OnCompanyinfo();

public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void ViewGgxx(int  nID = -1);
	void RefreshFormu();
	CString GetCurrentSelectStockCode( WORD & wMarket );
	CTaiScreen* ShowTjxg();


	void TestHQ();
	static void ActiveVwbase(CTaiShanKlineShowView* pView);
	CStatusBar_sjl  m_wndStatusBar;
	bool bAlarm;
	void DisplayBargain(
		float Index_sh,float UpDown_sh,float Vol_sh,float Amount_sh,
		float Index_sz,float UpDown_sz,float Vol_sz,float Amount_sz,
		float Index_cy,float UpDown_cy,float Vol_cy,float Amount_cy,
		float percent_sh, float percent_sz,float percent_cy);
	void SetStatusBar();	
	void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	void OnMenuFullscreen();
	CRect m_FullScreenWindowRect;
	BOOL m_bFullScreen;		
	void ReViewInfo(void);	
	WINDOWPLACEMENT m_wpPrev;      
	int CreateToolDrawLine();


	static CTaiScreen *dialogtjxg;
	static CTaiScreenTest *dialogtjxgmmtest;
	static CTaiKlineDlgNeuralLearn* m_pDlgNL;

protected:
	CStringArray m_symbol_array;
	IndexDataInfo* pIndex_save;
	int *nDays;


protected:

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg long OnExtService(UINT wFileType, LONG lPara)	;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnViewCompanyinfo();
	afx_msg void OnViewGgxx();
	afx_msg void OnFullscreen();
	afx_msg void OnNomalscreen();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnNcPaint();
	afx_msg void OnToolSjgl();
	afx_msg void OnToolGsgl();
	afx_msg void OnTjxgTjxg();
	afx_msg void OnViewXx();
	afx_msg void OnStopCuohe();
	afx_msg void OnToolTzgl();
	afx_msg void OnToolRiskIndex();
	afx_msg void OnUpdateFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNomalscreen(CCmdUI* pCmdUI);
	afx_msg void OnOpenBigBill();
	afx_msg void OnF3();
	afx_msg void OnF4();
	afx_msg void OnClose();
	afx_msg void OnHelpTopic();
	afx_msg void OnUpdateToolGsgl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBaseinfoGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCuoheOpen(CCmdUI* pCmdUI);
	afx_msg void OnGrid();
	afx_msg void OnUpdateGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateF3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateF4(CCmdUI* pCmdUI);
	afx_msg void OnWindowNew();
	afx_msg void OnUpdateWindowNew(CCmdUI* pCmdUI);
	afx_msg void OnTjxgAlarm();
	afx_msg void OnToolBlockMana();
	afx_msg void OnDownload20();
	afx_msg void OnCloseReceiver();
	afx_msg void OnUpdateCloseReceiver(CCmdUI* pCmdUI);
	afx_msg void OnTjxgMmTest();
	afx_msg void OnToolNnetTrain();
	afx_msg void OnToolAutoSubmit();
	afx_msg void OnDayLine();
	afx_msg void OnMin1Line();
	afx_msg void OnUpdateStockBig(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTjxgAlarm(CCmdUI* pCmdUI);
	afx_msg void OnF6();		
	afx_msg void OnPaint();
	afx_msg void OnSortIndex();
	afx_msg void OnXgnExtData();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnCalc();
	afx_msg void OnGdfxRegister();
	//}}AFX_MSG
	afx_msg LRESULT DisplayAlarmForMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTjxg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT DrawKlineWindow(WPARAM wParam, LPARAM lParam);
#ifdef WIDE_NET_VERSION
	afx_msg void OnLoginServer();
	afx_msg void OnRecvStockcode();
#endif

protected:
	CDialogMYXMZ*	m_pViewMYXMZ;

public:
	void ShowMYXMZ();

protected:
	//CTabControl m_wndTabs;
	CXTPTabClientWnd m_MTIClientWnd;

public:
	void HqStock_Init();
	void HqStock_Quit();

protected:
	afx_msg void OnClearRealdata();
	afx_msg long OnStkDataOK(UINT wFileType, LONG lPara);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MAINFRM_H__61A31CED_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
