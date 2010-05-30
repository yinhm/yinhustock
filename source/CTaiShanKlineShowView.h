
#if !defined(AFX_VWBASEVIEW_H__D69209B5_A1E6_11D1_B031_000021002C16__INCLUDED_)
#define AFX_VWBASEVIEW_H__D69209B5_A1E6_11D1_B031_000021002C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFormularCompute.h"
#include "StructKlineView.h"
#include "CTaiShanDoc.h"
#include "MyTipWin.h"
#include "CTaiKlineDialogCross.h"
#include "CTaiKlineShowText.h"	
#include "LBsInfoRS.h"
#include "CTaiKlineDlgChangeIndexParam.h"
#include "StructTaiShares.h"
#include "CTaiKlineDlgDealHistory.h"
#include "MSFlexGrid.h"
#include "CTaiScreensIndicate.h"

#define FIGUER_ALL_NUM 5

class LBsInfoRS;
class CTaiKlineShowKline;
class CTaiKlineMin1;
class CTaiKlineDialogCross;
class CTaiKlineDrawing;
class CTaiKlineShowText;
class CTaiKlineDialogKLineMode;

class CSheetScrollBar;

class CTaiKlineWideNet;

class CTaiKlineDC : public CDC
{
public:


	CTaiKlineDC(CWnd* pWnd,CBitmap* pBitmap,CRect* r = NULL) : CDC()
	{
		ASSERT(pWnd != NULL);

		m_pDC = new CClientDC(pWnd);
		m_pOldBitmap = NULL;
		if(r==NULL)
			pWnd->GetClientRect (m_rect);
		else
			m_rect = *r;

		this->CreateCompatibleDC(m_pDC);
		m_pOldBitmap = SelectObject(pBitmap);

	}


	virtual ~CTaiKlineDC()
	{

		::BitBlt(m_pDC->m_hDC,m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			this->m_hDC, m_rect.left, m_rect.top, SRCCOPY);


		this->SelectObject(m_pOldBitmap);
		delete m_pDC;
	}


	CTaiKlineDC* operator->() {return this;}


	operator CTaiKlineDC*() {return this;}

private:
	CBitmap  m_bitmap;     
	CBitmap* m_pOldBitmap;  
	CDC*     m_pDC;        
	CRect    m_rect;       
	BOOL     m_bMemDC;     
};


struct PAGE_DATA_INFO
{
	int footBegin;
	int footEnd;
	int klineNumDefault;

	int nStockBegin;
	int nStock;
	int nTotalStock;


	int kLineType;		

	SUB_FIGUER_INIT_INFO	m_infoInit;	


	int tabNum;			
	int infoFiguer;	
	int m_bMultiFiguer;

	bool bBaseInfo;
	BYTE isShowShiZi;

	char symbol[8];
	int m_stkKind;
	PAGE_DATA_INFO()
	{
		memset(symbol,0,8);
		m_stkKind = 0;
	};
};
struct KLINE_EXT_PER
{

	int m_nCountKlineSelf;
	Kline* m_pKlineSelf;
	KLINE_EXT_PER()
	{
		m_nCountKlineSelf = 0;
		m_pKlineSelf = 0;
	};
};

class CTaiTestRichEditCtrl;
class CTaiKlineFileKLine;
class CTaiKlineDlgDealHistory;
class CTaiKlineDialogShiDuanTJ;
class CTaiKlineDlgZhibiaoTongji;

typedef CTypedPtrMap<CMapStringToPtr,CString,KLINE_EXT_PER*> CMapStringToKlineExt;

class CTaiShanKlineShowView : public CView
{
protected:
	CTaiShanKlineShowView();
	DECLARE_DYNCREATE(CTaiShanKlineShowView)


public:
	void DoHistoryAStep() ;
	float CaclMid(int i);
	void RemoveHs(int flag);
	int FindTimeK(int keyVlu);
	int ReadKline5Min(CString fName,int stkKind, Kline *&pKline,int nRead=-1);
	int ReadKLine(CString fName,int stkKind,Kline*& pKline,int nRead=-1);



public:



	//{{AFX_VIRTUAL(CTaiShanKlineShowView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL



public:
	void ShowKlineMode(CFormularContent *pJishu);
	void DoF4();
	void ShowMark(CFormularContent* pJishu = NULL);
	void WideNetRedraw(WPARAM wp,LPARAM lp);
	int m_stkKind;
	int GetDataKind();
	CString GetMultiSymbol(int iSymbol,int &stkKind);
	void YHParam();
	void HistoryAStep(int nFoot);
	static float GetCapital(CReportData* pdt);
	static bool CheckDiskFreeSpace();
	static bool CheckStockSymbol(CString s);
	void InvertFocusRect2(CDC* pDC);
	void ShowDlgCross(BOOL bShow = TRUE);
	int m_nCurrentKlineKind;
	static int IsIndexStock3(CString s);
	static int IsIndexStock(CString s);
	void HistoryHide();
	static void OnDataChangAll(int nParam);
	void ClearKlineExt();
	void SetKlineExt(Kline* pKlineIn,int nKline=1,int nFootWrite = -1);
	static CTypedPtrMap<CMapStringToPtr,CString,KLINE_EXT_PER*> m_klineExtMap;
	BASEINFO* GetBaseinfoPointer();
	bool GetPriceAve(float &f1,float totv = 0,float totp = 0);
	void RedrawWindowNoCacl();
	static DWORD GetTimeSpan(CString s);
	void OutDataExcel(CMSFlexGrid* grid,int nFiguer);

	CTaiKlineWideNet * m_pWideNet;
	CTaiScreensIndicate m_screenStockShow;
	void GetCurrClientRect(CRect& rt);
	CTaiShanDoc* GetDocument();
	void SetPageData();
	CTaiKlineDialogShiDuanTJ* m_dlgFenshiTj;
	CTaiKlineDialogShiDuanTJ* m_dlgKlineTj;
	CTaiKlineDlgZhibiaoTongji* m_dlgZhibiao;
	CString m_symbol9[9];
	int m_stkKind9[9];
	int m_nKline9[9];



	CTaiKlineDlgDealHistory* m_pDlgDealHistory;
	CArray<PAGE_DATA_INFO,PAGE_DATA_INFO>	m_pageInfoArray;
	int m_nPageCurrent;
	CBuySellList m_hs ;
	CFJList	m_fenjia;
	Kline* m_pkline;	
	bool m_bAddKline;
	CString m_sharesSymbol;
	int m_nCountKline;
	int m_nCountKlineFile;
	int m_nCountKlineToday;
	SUB_FIGUER_INIT_INFO	m_infoInit;	
	bool bExtend;
	int lineTypeDrawed;
	int m_nOldnFgr;

	bool IsBarShow;
	bool m_bKD;

	int m_xPrinter;
	int m_yPrinter;
	int m_cxExt;
	int m_cyExt;

	int m_nBeginHS;
	bool bTrackCurcorBgn;
	int m_minGridX;
	int m_minGrid;
	int m_heightCaption ;
	int m_midLen        ;	
	int m_rightLength	  ;
	bool m_GetFocus;  

	CTaiKlineDlgChangeIndexParam* m_pChangeIndexParam;


	int m_nSharesBegin;
	int m_nShares;
	int m_nTotalStock;
	int m_AutoChange;

	bool m_bKeyUpDown;
	bool m_bBaseInfo;
	bool m_bDoPower;
	CTaiKlineDialogKLineMode* pKlineMode;
	LBsInfoRS m_bs;
	BYTE m_isShowCross;
	CPoint m_toScreen;
	int m_bMultiFiguer;
	CTaiKlineDialogCross* m_dlgShowCross;
	float m_currentValue;
	CTaiTestTipWin* m_MyToolTip;
	CRect m_rectHitPos;
	int m_nOldHitPos;
	int m_nOldHitY;
	CTaiKlineDrawing* pDrawLine;
	int m_hit;			
	int m_tjxgshow;               
	int m_infoFiguer;	
	int m_nMaxY;			
	int m_nMaxX;				
	CPoint m_pointMove;			

	CBitmap	m_bit;				
	CDC*    dc;
	int m_tabNum;				
	CScrollBar* m_scrollBar;		

	RECTFENSHI m_rtMin1;		
	RECTKLINE m_rtKlineFiguer;

	CTaiTestRichEditCtrl*  m_pRichEdit;	



	void DrawRightSheet(CDC* pDC,int Num,bool bShowScroll=true);
	void DrawRightSheet(CDC* pDC,CPoint pt);

	void ClearBack(CDC* pDC,CRect& rt,bool bHist=false);

	BOOL DoOnCommand(WPARAM wParam, LPARAM lParam);


	bool CreateSplitMy(int bVert=1,int * pInt = NULL);

	void AddNewStock(CString& symbol,int nStkKind);


	void SetConstant();

	void ReadDataMultiFiguer(int nItem);

	void CaclStockPos();

	void ShowAll(CString sharesymbol,bool bCaclStockPos = true,bool bInitFoot = true);    



	void ReDrawSheet(BOOL UpOrDown);

	void OnPageDown(bool bDown,int nPer=1);


	void WriteKLine5min(int nFoot);


	void OnPrintKLine(CDC* pDC);


	void WriteKLine(int nFoot,bool bOne,bool bDelete = false);
	void OnMenu1fs();

	void OnDayKline();

	void ShowKlineBig(bool bBig=true);

	void ShowTransferText(CDC* pDC);
	void CopyCaption();
	void ShowCross(bool bKey=false,int x=-1,int y=-1);
	void ClearCross();
	void CopyRect(CRect r);

	void ShowShiZi();
	void ShowShiZi(int x,int y,int flag=0);
	void ShowMyMenu(UINT id,int subM=0);

	int TransferX(int x);
	void TrackCursorEnd(CPoint point,BOOL flag);
	void TrackCursorGoOn(CPoint p);
	void TrackCursorBgn(CPoint point);
	void OnSizeMy(int cx, int cy);


	void SetPictYPos(int which,int y);


	void DrawRightBoxDapan(CDC* pDC,bool bClearAll=true);
	void DrawRightText(CDC *pDC,float vl,int x, int yNum, int flag,int floatBit=2);


	void DrawRightbox(CDC* pDC,bool bClearAll=true);
	void CalcCursorShape(CPoint pt);

	void DrawRulorBoxY(int y);	
	void DrawRulorBoxX(int x,bool bKey=false);	
	void DrawFenShi(CDC* pDC);	

	void CaclRulorMovable(CPoint point);	

	void DrawRectPer(CDC* pDC);		
	void InitRect(int cx, int cy);	

	void DrawLineCurve(CDC* pDC);
	void BuySell();
	void DrawZhibiao(CString ZhibiaoName,int nFiguer = -1);   

	void RefreshChenben(CTaiKlineDC* pMemdc);
	virtual ~CTaiShanKlineShowView();




#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


protected:
	COLORREF GetColor(CReportData* pp,bool isVol);
	void DrawUpDown(CDC* pDC);
	int PointToFiguer();
	//{{AFX_MSG(CTaiShanKlineShowView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnLineself();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKCacl();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnXianduan();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKlineMode();
	afx_msg void OnBsInfoShow();
	afx_msg void OnPowerDo();
	afx_msg void OnZhutuDiejia();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewGpHuanQuan();
	afx_msg void OnUpdateViewGphq(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewBuysell();
	afx_msg void OnRealTime();
	afx_msg void OnDayLine();
	afx_msg void OnUpdateBsInfoShow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewBuysell(CCmdUI* pCmdUI);
	afx_msg void OnViewSimple();
	afx_msg void OnViewPersent();
	afx_msg void OnViewDszb();
	afx_msg void OnUpdateViewDszb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSimple(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPersent(CCmdUI* pCmdUI);
	afx_msg void OnViewOne();
	afx_msg void OnUpdateViewOne(CCmdUI* pCmdUI);
	afx_msg void OnViewFourstock();
	afx_msg void OnUpdateViewFourstock(CCmdUI* pCmdUI);
	afx_msg void OnViewLine();
	afx_msg void OnUpdateViewLine(CCmdUI* pCmdUI);
	afx_msg void OnViewDayline();
	afx_msg void OnUpdateViewDayline(CCmdUI* pCmdUI);
	afx_msg void OnViewFifteenmin();
	afx_msg void OnUpdateViewFifteenmin(CCmdUI* pCmdUI);
	afx_msg void OnViewThirtymin();
	afx_msg void OnUpdateViewThirtymin(CCmdUI* pCmdUI);
	afx_msg void OnViewSixtymin();
	afx_msg void OnUpdateViewSixtymin(CCmdUI* pCmdUI);
	afx_msg void OnViewMonthline();
	afx_msg void OnUpdateViewMonthline(CCmdUI* pCmdUI);
	afx_msg void OnViewWeekline();
	afx_msg void OnUpdateViewWeekline(CCmdUI* pCmdUI);
	afx_msg void OnView5min();
	afx_msg void OnUpdateView5min(CCmdUI* pCmdUI);
	afx_msg void OnViewOneitem();
	afx_msg void OnUpdateViewOneitem(CCmdUI* pCmdUI);
	afx_msg void OnViewTwoitem();
	afx_msg void OnUpdateViewTwoitem(CCmdUI* pCmdUI);
	afx_msg void OnViewFouritem();
	afx_msg void OnUpdateViewFouritem(CCmdUI* pCmdUI);
	afx_msg void OnViewThreeitem();
	afx_msg void OnUpdateViewThreeitem(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZhutuDiejia(CCmdUI* pCmdUI);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnFileOpen1();
	afx_msg void OnTjxgTjxgts();
	afx_msg void OnUpdateTjxgTjxgts(CCmdUI* pCmdUI);
	afx_msg void OnToolCursor();
	afx_msg void OnUpdateToolCursor(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePowerDo(CCmdUI* pCmdUI);
	afx_msg void OnViewJszb();
	afx_msg void OnViewstockdetail();
	afx_msg void OnF2();
	afx_msg void OnViewYearkline();
	afx_msg void OnUpdateViewYearkline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKlineMd(CCmdUI* pCmdUI);
	afx_msg void OnToolDrawline();
	afx_msg void OnHldjLmb();
	afx_msg void OnUpdateHldjLmb(CCmdUI* pCmdUI);
	afx_msg void OnHedjLmb();
	afx_msg void OnUpdateHedjLmb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewJszb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolDrawline(CCmdUI* pCmdUI);
	afx_msg void OnCopyFile();
	afx_msg void OnLoopDisplay();
	afx_msg void OnUpdateLoopDisplay(CCmdUI* pCmdUI);
	afx_msg void OnDelZhutuL();
	afx_msg void OnUpdateDelZhutuL(CCmdUI* pCmdUI);
	afx_msg void OnQldjLmb();
	afx_msg void OnUpdateQldjLmb(CCmdUI* pCmdUI);
	afx_msg void OnQedjLmb();
	afx_msg void OnUpdateQedjLmb(CCmdUI* pCmdUI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKlineHs();
	afx_msg void OnUpdateKlineHs(CCmdUI* pCmdUI);
	afx_msg void OnKlineManyDay();
	afx_msg void OnUpdateKlineManyDay(CCmdUI* pCmdUI);
	afx_msg void OnKline1Minu();
	afx_msg void OnUpdateKline1Minu(CCmdUI* pCmdUI);
	afx_msg void OnLSplitV();
	afx_msg void OnUpdateLSplitV(CCmdUI* pCmdUI);
	afx_msg void OnLSplitH();
	afx_msg void OnLSplitDel();
	afx_msg void OnUpdateLSplitDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLSplitH(CCmdUI* pCmdUI);
	afx_msg void OnLSplit4();
	afx_msg void OnUpdateLSplit4(CCmdUI* pCmdUI);
	afx_msg void OnClassifyJishu();
	afx_msg void OnHistoryOpen();
	afx_msg void OnUpdateHistoryOpen(CCmdUI* pCmdUI);
	afx_msg void OnViewFiveitem();
	afx_msg void OnUpdateViewFiveitem(CCmdUI* pCmdUI);
	afx_msg void OnMenuitemDeleteK();
	afx_msg void OnUpdateMenuitemDeleteK(CCmdUI* pCmdUI);
	afx_msg void OnTjxgMmTestShow();
	afx_msg void OnUpdateTjxgMmTestShow(CCmdUI* pCmdUI);
	afx_msg void OnMoveChengben2();
	afx_msg void OnTimeAstep4();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnNineShow();
	afx_msg void OnMoveFig();
	afx_msg void OnUpdateMoveFig(CCmdUI* pCmdUI);
	afx_msg void OnSaveBitmap();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSwitchHistoryAcc();
	afx_msg void OnAstepHist();
	afx_msg void OnUpdateAstepHist(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchHistoryAcc(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void DoDrawLine(UINT nID);
	afx_msg void DoDrawLineUpdate(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDataChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDropDrag(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void HistOpen(CString * s = NULL);
	static void ViewSetFocus(CTaiShanKlineShowView* pView);
	BOOL m_bActived ;
	void DoUpdateViewItem(int nItem,CCmdUI* pCmdUI);
	void DoViewItem(int nItem);
	void SavePageFile();
	void OpenPageFile();

	void DrawFinace(CDC* pDC,int nBegin=0);
	void DrawPingCe(CDC* pDC,CRect &rc);
	void GetPageData(bool bMovePre = true);
	void CopyScrollBar(CDC* pMemDC);
	void KlineReadTransferData();

protected:
	BOOL m_bInited;

public:
	CTaiKlineMin1* pMin1Drawing;		// 
	CTaiKlineShowKline* pKlineDrawing;	// 

public:
	int m_nKlineKind2;					// 画线类型 分时、K线。。。

public:
	void ShowTextRect(int nItem, CDC* pDC);
};

#ifndef _DEBUG 
inline CTaiShanDoc* CTaiShanKlineShowView::GetDocument() { return (CTaiShanDoc*)m_pDocument; }
#endif

#endif // !defined(AFX_VWBASEVIEW_H__D69209B5_A1E6_11D1_B031_000021002C16__INCLUDED_)
