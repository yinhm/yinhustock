
#if !defined(AFX_STOCK2000VIEW_H__13C4E5B0_D3B4_11D1_BF39_0000E86564F7__INCLUDED_)
#define AFX_STOCK2000VIEW_H__13C4E5B0_D3B4_11D1_BF39_0000E86564F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ColumnSelect.h"
#include "cellrange.h"


struct TitleMenuDef {
	UINT  m_nString;              
	UINT     m_nID;                 
};
class CPageWnd;
class CGridCtrl;
class CChildFrame;
class CSheetScrollBar;
class CColumnSelect;

class CHistoryDlg;
class CTaiShanReportView : public CView
{
protected: 
	CTaiShanReportView();
	DECLARE_DYNCREATE(CTaiShanReportView)


public:
	CTaiShanDoc* GetDocument();
	CChildFrame *m_ChildFrame;

public:



	//{{AFX_VIRTUAL(CTaiShanReportView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL


public:
	void InitAllTitle();
	void OnF6();	
	virtual ~CTaiShanReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
public:

	CHistoryDlg* m_pHistoryDlg;
	void OnDayLine();
	TitleMenuDef m_TitleMenuDef[68];  
	CMenu *m_pPopMenu;
	CColumnSelect *m_pColumnSelect;
	CCellID m_CellTitleCommand;        //
	int     m_nFirst;
	void RefreshData();            
	CGridCtrl *m_pGrid;  


protected:
	BOOL InitGridContent();
	CSheetScrollBar * m_hScrollBar; 
	CPageWnd *m_PageWnd; 
	//{{AFX_MSG(CTaiShanReportView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnViewDatamanage();
	afx_msg void OnViewInvestmanage();
	afx_msg void OnAddChoose();
	afx_msg void OnDeleteStocktype();
	afx_msg void OnOpenDocument();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLoopDisplay();
	afx_msg void OnUpdateLoopDisplay(CCmdUI* pCmdUI);
	afx_msg void OnFontChange();
	afx_msg void OnRealTime();
	afx_msg void OnCompanyinfo();
	afx_msg void OnFileOpen1();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSaveStocktype();
	afx_msg void OnChangetype();
	afx_msg void OnOpenStock() ;
	afx_msg void OnStockType1();
	afx_msg void OnStockType2();
	afx_msg void OnStockType3();
	afx_msg void OnStockType4();
	afx_msg void OnOpenNineShow();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateStockType2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStockType3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStockType4(CCmdUI* pCmdUI);
	afx_msg void OnHistoryOpen();
	afx_msg void OnUpdateHistoryOpen(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void SwitchFinance() ;
	//}}AFX_MSG
	afx_msg LRESULT OnSetScrollBarPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HotKeyProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKlineToThis(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStockChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnIndexColumnChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDisplayStockTypeChange(UINT nID);
	afx_msg void OnUpdateDisplayStockType(CCmdUI* pCmdUI);



	DECLARE_MESSAGE_MAP()
private:
	int     m_nStartColumn; 

	int     m_nCols; 
	int		m_nRows; 

	CImageList m_ImageList;   
	CStatic	m_GridRect;      
	int		m_nFixCols;      
	int		m_nFixRows;      

	POINT   m_barplace;    
	BOOL	m_bListMode;     
	int     m_nBarlength;      
	CRect   m_barRect;
	UINT    m_TimeCount;

	CStringArray m_nColumnTitleArray;
	UINT m_VisibleRowCount;                       
	UINT m_VisibleColumn;
	BOOL m_LoopDisplay;

public:
	void SetColumnWidth();
	void GetColumnWith();
	static bool IsBg(int kind);
	void SetTypeToDocumentLmb(int nStockType,bool bSetPageWnd = true);
	int GetTotalRowsLmb(int nActive);
	int GetStockTypeFromPageWnd(int nActive);
	void SetHistoryColor(bool bHistory);
	void IndexSortLi();
	CString GetCurrentSymbol();
	void ActiveGrid();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);  
	void DoUpdateColumn(UINT nID);                          
	int InitGridCtrl();                                      
	void ClearStockRow(long nRow);                         
	void AddGridRow(UINT nPage);                          
	void AddColumn(UINT m_Column,UINT nID);
	void UpdateColumnData(CTaiShanDoc* pDoc,int m_Column,int FirstRow,int EndRow);
	void UpdateColumn(UINT m_Column,UINT nID);
	BOOL ChangeToStockType(CTaiShanDoc* pDoc,CString m_StockType) ;
	void PopMenuItemInit(CMenu *pMenuContext,UINT nRow);
	UINT GetActivePage();
	long FindStockInNameSymbor(UINT nPage,char *zqdm);
	void ScrollToSelectStock(char *zqdm,int nKind);
	void HotKeyDownProgress(char  *HotKey);
	void SortStockIndex(UINT nPage,UINT nID);
	void DisplaySortDataInPage(SORTSTRUCT *m_pStockData,int DisplayCount);
	UINT GetSortColumn(UINT nID);
	void ScrollToPageOfStock(CTaiShanDoc* pDoc,int nPage,CString m_findzqdm);
	void DisplayDpStockRow(CTaiShanDoc* pDoc,CReportData *Cdat ,long nRow);
	void LoopDisplayOnTimer();
	void ChangeToTjxg(CTaiShanDoc* pDoc);
	BOOL ChangeToTjxg(CTaiShanDoc* pDoc,CString m_StockType );
	LRESULT OnGridColorChange(WPARAM wParam, LPARAM lParam);
	void SortDataForColumn();
	int  GetPageRows();
	BOOL GetStockCodeForRow(int nRows,char *Code,int &nKind);
	BOOL GetRowForStockCode(char * Code,int & nRow);
	void SortDataForPage8();
	void ChangedColumnType(int ColumnType);
	void DefalutColumnSelect();
	void OnAddStocktype(UINT nID);
	void OnDeleteTjxgStockType();
	static void OnSaveScreenStocktype(SymbolKindArr& StockCodeArray ,CString *StockName=NULL); 
	void ChangeToTjxg(CTaiShanDoc* pDoc,BOOL IsNewTjxg);
	BOOL GetCdatPointForRow(int nRow,PCdat1 &pStockData);
#ifdef WIDE_NET_VERSION
	TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
	int m_nMinuteCount;
	SOCKET_ID m_SocketID;
	int n_nSendStockCount;
	TransmitStockAttribute  *m_pTransmitStockAttribute;
	void SendRecvDataInfomation(ClientTransmitAttributes  TransmitType);
	BOOL SetSendStockCodeData(int SendStockCount);
#endif

public:
	BOOL SetShowData(CTaiShanDoc* pDoc, int StockCount);
	void LoadShowData(int nKind);

public:
	void ChangeToChoose(CTaiShanDoc* pDoc);
	void ChangeToPage(int nPage, BOOL NewTjxg = FALSE);

public:
	void AddChoose(char* StockCode, int nKind);
};

#ifndef _DEBUG
inline CTaiShanDoc* CTaiShanReportView::GetDocument() { return (CTaiShanDoc*)m_pDocument; }
#endif

#endif // !defined(AFX_STOCK2000VIEW_H__13C4E5B0_D3B4_11D1_BF39_0000E86564F7__INCLUDED_)
