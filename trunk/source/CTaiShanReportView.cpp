
#include "stdafx.h"
#include "afxwin.h"
#include <share.h>
#include  <io.h>

#include "CTaiShanApp.h"

#include "PageWnd.h"	
#include "ProgressDialog.h"
#include "SheetScrollBar.h"
#include "ColumnSelect.h" 
#include "SelectStock.h"
#include "SortIndex.h"
#include "alcindex.h"
#include "CTaiShanDoc.h"
#include "ChildFrm.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "keybrodeangel.h"

//#include "Companyinfoview.h"
#include "GridCtrl.h"
#include "CTaiShanKlineShowView.h"
#include "MainFrm.h"
#include "NineShowView.h"

//#include "sheetbhadd.h"
#include "DiaTest.h"
#include "CTaiKlineFileHS.h"
#include "HistoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW

#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define BOTTOM_HIGHT 16
#define ID_CGRIDLIST  5001
#define ID_HSCROLLBAR 5002
#define ID_GPHQXS     5003


#define MOUSE_ALL  7001
#define MOUSE_COL  7002
#define MOUSE_ROW  7003
#define MOUSE_CELLS 7004
#define MOUSE_NOTHINGS 7005
#define SYSTEMHEAD     1000
#define MAINPAGE     6


BOOL g_bDestroyedView = FALSE;

void Swap(STOCKDATASHOW &m_pData1,STOCKDATASHOW &m_pData2)
{
     STOCKDATASHOW pData;
	 memcpy(&pData,&m_pData1,sizeof(STOCKDATASHOW));
	 memcpy(&m_pData1,&m_pData2,sizeof(STOCKDATASHOW));
	 memcpy(&m_pData2,&pData,sizeof(STOCKDATASHOW));
}


BOOL QuickSort(STOCKDATASHOW *m_pStockData,BOOL bAscending,int low,int high)
{
    int lo = low;
    int hi = high;
    STOCKDATASHOW pivot;

    if( hi <= lo ) return FALSE;

	int mid=(low +high)/2;
	pivot=m_pStockData[mid];

    while( lo <= hi )
    {
       
        if( bAscending )
		{
            while (lo < high && m_pStockData[lo].ItemData  <pivot.ItemData )
			{
 				++lo;
			}
		}
        else
		{
            while ((lo < high  && m_pStockData[lo].ItemData > pivot.ItemData))
			{
 				++lo;
			}
		
		}

        
        if( bAscending )
		{
			while (hi > low && m_pStockData[hi].ItemData >pivot.ItemData )
			{
 				--hi;
			}
		}
        else
		{
            while (hi > low && m_pStockData[hi].ItemData <pivot.ItemData)
			{
                --hi;
			}
		}
   
        if (lo <= hi)
        {
         
            if ( m_pStockData[lo].ItemData != m_pStockData[hi].ItemData  )
            {
				Swap(m_pStockData[lo],m_pStockData[hi]);
            }

            ++lo;
            --hi;
        }
    }

   
    if( low < hi )
        QuickSort(m_pStockData, bAscending, low, hi);

   
    if( lo < high )
        QuickSort(m_pStockData, bAscending, lo, high);

    return TRUE;
}
BOOL QuickSortString(STOCKDATASHOW *m_pStockData,BOOL bAscending,int low,int high)
{
    int lo = low;
    int hi = high;
    STOCKDATASHOW pivot;

    if( hi <= lo ) return FALSE;

	int mid=(low +high)/2;
	pivot=m_pStockData[mid];

    while( lo <= hi )
    {
       
        if( bAscending )
		{
            while (lo < high && strcmp(m_pStockData[lo].StockId , pivot.StockId)<0 )
			{
 				++lo;
			}
		}
        else
		{
            while ((lo < high  && strcmp(m_pStockData[lo].StockId, pivot.StockId)>0))
			{
 				++lo;
			}
		
		}

      
        if( bAscending )
		{
			while (hi > low && strcmp(m_pStockData[hi].StockId, pivot.StockId)>0 )
			{
 				--hi;
			}
		}
        else
		{
            while (hi > low && strcmp(m_pStockData[hi].StockId, pivot.StockId)<0)
			{
                --hi;
			}
		}
        
        if (lo <= hi)
        {
            
            if ( m_pStockData[lo].StockId != m_pStockData[hi].StockId  )
            {
				Swap(m_pStockData[lo],m_pStockData[hi]);
            }

            ++lo;
            --hi;
        }
    }

    
    if( low < hi )
        QuickSortString(m_pStockData, bAscending, low, hi);

    if( lo < high )
        QuickSortString(m_pStockData, bAscending, lo, high);

    return TRUE;
}



IMPLEMENT_DYNCREATE(CTaiShanReportView, CView)

BEGIN_MESSAGE_MAP(CTaiShanReportView, CView)
	//{{AFX_MSG_MAP(CTaiShanReportView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_DATAMANAGE, OnViewDatamanage)
	ON_COMMAND(ID_VIEW_INVESTMANAGE, OnViewInvestmanage)
	ON_COMMAND(ID_ADD_CHOOSE, OnAddChoose)
	ON_COMMAND(ID_DELETE_STOCKTYPE, OnDeleteStocktype)
	ON_COMMAND(ID_OPEN_DOCUMENT, OnOpenDocument)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_LOOP_DISPLAY, OnLoopDisplay)
	ON_UPDATE_COMMAND_UI(ID_LOOP_DISPLAY, OnUpdateLoopDisplay)
	ON_COMMAND(ID_FONT_CHANGE, OnFontChange)
	ON_COMMAND(ID_REAL_TIME, OnRealTime)
	ON_COMMAND(ID_COMPANYINFO, OnCompanyinfo)
	ON_COMMAND(ID_FILE_OPEN1, OnFileOpen1)
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_SAVE_STOCKTYPE, OnSaveStocktype)
	ON_COMMAND(ID_CHANGETYPE, OnChangetype)
	ON_COMMAND(ID_OPEN_STOCK, OnOpenStock)
	ON_COMMAND(ID_STOCK_TYPE1, OnStockType1)
	ON_COMMAND(ID_STOCK_TYPE2, OnStockType2)
	ON_COMMAND(ID_STOCK_TYPE3, OnStockType3)
	ON_COMMAND(ID_STOCK_TYPE4, OnStockType4)
	ON_COMMAND(ID_NINE_SHOW, OnOpenNineShow)
	ON_WM_HELPINFO()
	ON_WM_RBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_STOCK_TYPE2, OnUpdateStockType2)
	ON_UPDATE_COMMAND_UI(ID_STOCK_TYPE3, OnUpdateStockType3)
	ON_UPDATE_COMMAND_UI(ID_STOCK_TYPE4, OnUpdateStockType4)
	ON_COMMAND(ID_HISTORY_OPEN, OnHistoryOpen)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_OPEN, OnUpdateHistoryOpen)
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	
    ON_COMMAND(ID_SWITCH_FINANCE, SwitchFinance)
 	ON_COMMAND_RANGE(ID_MENU_TYPE1,ID_MENU_TYPE5,OnDisplayStockTypeChange)
 	ON_UPDATE_COMMAND_UI_RANGE(ID_MENU_TYPE1,ID_MENU_TYPE5,OnUpdateDisplayStockType)
    ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(WM_USER_HSCROLL,OnSetScrollBarPos)
    ON_MESSAGE(WM_USER_HOTKEYPROGRESS ,HotKeyProgress)
    ON_MESSAGE(WM_USER_CHANGECOLOR,OnGridColorChange)
    ON_MESSAGE(WM_USER_FROM_KLINE,OnKlineToThis)
    ON_MESSAGE(WM_USER_CHANGESTOCK,OnStockChange)
    ON_MESSAGE(WM_USER_COLUMNSELECT,OnColumnChange)
    ON_MESSAGE(WM_USER_JISHUCOLUMNSELECT,OnIndexColumnChange)

END_MESSAGE_MAP()



CTaiShanReportView::CTaiShanReportView()
{

	m_pHistoryDlg = NULL;
    m_pGrid = new CGridCtrl;
	m_hScrollBar = new CSheetScrollBar;
    m_PageWnd=new CPageWnd;
	m_nBarlength = 200;
	m_bListMode=TRUE;
	m_nCols=0;
	m_nRows=0;
    m_VisibleColumn=0;
	m_TimeCount=0;
    m_LoopDisplay=FALSE;
	m_pColumnSelect=NULL;
 

    m_pPopMenu=NULL;
	for(int i=0;i<68;i++)
	{
		m_TitleMenuDef[i].m_nString=IDS_STOCKNAME +i;
        m_TitleMenuDef[i].m_nID=ID_STOCKNAME+i;
	}
    m_VisibleRowCount=0;
	m_nFirst = 1;
	m_pColumnSelect=NULL;
#ifdef WIDE_NET_VERSION
	m_nMinuteCount=0;
	n_nSendStockCount=0;
	m_pTransmitStockAttribute=NULL;
	m_SocketID.hWnd=NULL;
	m_SocketID.iFlag=-1;
#endif
}

CTaiShanReportView::~CTaiShanReportView()
{
#ifdef WIDE_NET_VERSION
	if(m_pTransmitStockAttribute)
	{
 		GlobalUnlock((HGLOBAL)m_pTransmitStockAttribute);     
		GlobalFree( (HGLOBAL)m_pTransmitStockAttribute);
	}
#endif
	if (m_pGrid)      
		delete m_pGrid;

	if(m_hScrollBar)
		delete m_hScrollBar;

	if(m_PageWnd)
		delete m_PageWnd;
	if(m_pPopMenu)
		delete m_pPopMenu;
	if(m_pColumnSelect)
		::SendMessage(m_pColumnSelect->m_hWnd,WM_CLOSE,0,0);
}
LRESULT CTaiShanReportView::OnSetScrollBarPos(WPARAM wParam, LPARAM lParam)
{
    int nCurPos;

	CCellRange m_VisibleCellRange=m_pGrid->GetVisibleNonFixedCellRange(); 
	int m_VisibleColumn=m_VisibleCellRange.GetMaxCol()-m_VisibleCellRange.GetMinCol();
	
	nCurPos = m_hScrollBar->GetScrollPos();


	BOOL m_IsLeft;
	m_IsLeft=(BOOL )wParam;
	if(m_IsLeft)
       nCurPos +=1;
	else
       nCurPos -=1;

    m_hScrollBar->SetScrollPos(nCurPos,true);

	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
	long m_CurrRow=m_pGrid->GetCurrentRow();
    m_TopLeftCell.row= m_CurrRow;
	m_pGrid->SetFocusCell(m_TopLeftCell);

	return 0;
}

BOOL CTaiShanReportView::PreCreateWindow(CREATESTRUCT& cs)
{
	

	return CView::PreCreateWindow(cs);
}


void CTaiShanReportView::OnDraw(CDC* pDC)
{

}
LRESULT CTaiShanReportView::OnGridColorChange(WPARAM wParam, LPARAM lParam)
{
	CTaiShanDoc* pDoc = GetDocument();
	m_pGrid->SetGridColor(pDoc->m_colorArray[18+wParam],pDoc->m_colorArray[17],pDoc->m_colorArray[16]);

    m_pGrid->SetFixedBkColor(pDoc->m_colorArray[18+wParam]);
    m_pGrid->SetBackgroundClr(pDoc->m_colorArray[18+wParam]);

	CFont Font;
	
	LOGFONT *lf;
	lf=m_pGrid->GetItemFont(1,1);
	if(strcmp(lf->lfFaceName,pDoc->m_fontstr[1].lfFaceName)!=0||lf->lfHeight !=pDoc->m_fontstr[1].lfHeight
		||lf->lfWidth !=pDoc->m_fontstr[1].lfWidth || lf->lfWeight !=pDoc->m_fontstr[1].lfWeight)
	{
		Font.CreateFontIndirect(&pDoc->m_fontstr[1]);
		m_pGrid->SetFont(&Font);
 		m_pGrid->AutoSize();
		Font.DeleteObject();
	}
	for(int i=0;i<MAXCOLUME;i++)
	{
		char str[50];
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			break;
		switch(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i])
		{
			case ID_ZJLS:     
					sprintf(str,"资金(%d分)",pDoc->m_syshqset.capital);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,ID_ZJLS);
					break;
			case ID_VOLBIFM:    
					sprintf(str,"量比(%d分)",pDoc->m_syshqset.quangtity);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,ID_VOLBIFM);
					break;
			case ID_HLPFM:     
					sprintf(str,"震幅(%d分)",pDoc->m_syshqset.swing);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,ID_HLPFM);
					break;
			case ID_EXCHANGEFM:
					sprintf(str,"换手率(%d分)",pDoc->m_syshqset.changehand);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,ID_EXCHANGEFM);
					break;
			case ID_ADVSP:     
					sprintf(str,"涨速(%d分)",pDoc->m_syshqset.upspeed);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,ID_ADVSP);
					break;
		}
	}
    m_pGrid->Invalidate();                       

	m_PageWnd->RedrawWindow();

	return 0;
}



BOOL CTaiShanReportView::OnPreparePrinting(CPrintInfo* pInfo)
{

	return DoPreparePrinting(pInfo);
}

void CTaiShanReportView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}

void CTaiShanReportView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}



#ifdef _DEBUG
void CTaiShanReportView::AssertValid() const
{
	CView::AssertValid();
}

void CTaiShanReportView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTaiShanDoc* CTaiShanReportView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaiShanDoc)));
	return (CTaiShanDoc*)m_pDocument;
}
#endif 


int CTaiShanReportView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

    SetWindowText("动态显示牌");
	
 	DWORD dwStyle;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE;
	dwStyle &= ~WS_HSCROLL;
	m_pGrid->Create(CRect(0,0,10,10),this,ID_CGRIDLIST,dwStyle);
    
    m_hScrollBar->Create( SBS_HORZ | WS_CHILD | WS_VISIBLE,CRect(20,20,100,100),this,ID_HSCROLLBAR);
	m_hScrollBar->SetScrollRange(0,500,TRUE);

	m_PageWnd->Create(_T(" "),WS_CHILD | WS_VISIBLE, 
		CRect(100,100,200,200), this);
 
	m_pGrid->SetListMode(m_bListMode);

	m_pGrid->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));

 	m_PageWnd->m_scrollbar = (CSheetScrollBar *)GetDlgItem(ID_HSCROLLBAR);
	m_pGrid->SetFocus();
  m_pPopMenu=new CMenu;
  m_pPopMenu->LoadMenu(IDR_MENU_DISPLAY);


  SetWindowText("动态显示牌");
	return 0;

}


void CTaiShanReportView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	

	if(cx == 0 || cy == 0)
		return;
	
	
	GetClientRect(m_barRect);
	
    m_pGrid->MoveWindow(0,0,cx,cy-BOTTOM_HIGHT,TRUE);


	int bar_lengh = 250;   
	ShowWindow(SW_SHOW);
    m_hScrollBar->m_bRefrash=TRUE;
	m_hScrollBar->MoveWindow(cx-bar_lengh,cy-BOTTOM_HIGHT,bar_lengh,BOTTOM_HIGHT,TRUE);
	ShowWindow(SW_SHOW);
	
	int x,y;
	x  = cx - bar_lengh;
	y  = cy - BOTTOM_HIGHT;
    m_PageWnd->m_bRefrash=TRUE;
	m_PageWnd->MoveWindow(0,y,x,BOTTOM_HIGHT,true);
	ShowWindow(SW_SHOW);
	
	m_barplace.x = cx - bar_lengh;
	m_barplace.y = cy - BOTTOM_HIGHT;

	m_PageWnd->m_barstartplace.y = cy-16;
	m_PageWnd->m_barendplace.x = cx;
    m_PageWnd->m_barendplace.y = cy;
    
    CCellRange VisibleCells = m_pGrid->GetVisibleNonFixedCellRange();
        
    if(VisibleCells.GetMaxRow()<0||VisibleCells.GetMinRow ()<0)
      return ;
	m_VisibleRowCount=VisibleCells.GetMaxRow()   - VisibleCells.GetMinRow ()  + 1; 


	CCellRange m_VisibleCellRange=m_pGrid->GetVisibleNonFixedCellRange(); 
	m_VisibleColumn=m_VisibleCellRange.GetMaxCol()-m_VisibleCellRange.GetMinCol();
    

	int pos=m_hScrollBar->GetScrollPos();
	for(int i=0;i<pos;i++)
		m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);
	int nMax=m_nCols -  m_nFixCols + 1;
	m_hScrollBar->SetScrollRange( 0, nMax, TRUE);
    m_hScrollBar->SetScrollPos(0,true);
	m_pGrid->SetFocus();
}


void CTaiShanReportView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

    int nCurPos;
	int nMax,nMin;
	int i=0;

	CClientDC dc(this);
	


	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();


	CCellRange m_VisibleCellRange=m_pGrid->GetVisibleNonFixedCellRange(); 
	int m_VisibleColumn=m_VisibleCellRange.GetMaxCol()-m_VisibleCellRange.GetMinCol() +1 ;
	

	nCurPos = pScrollBar->GetScrollPos();
	pScrollBar->GetScrollRange(&nMin,&nMax);

	switch(nSBCode)
	{
	case SB_LINERIGHT:
			nCurPos += 1;
			if(nCurPos >nMax||m_TopLeftCell.col==m_nCols-1)
				return;
			m_pGrid->SendMessage(WM_HSCROLL,SB_LINERIGHT,0);
			break;
	case SB_LINELEFT:
			nCurPos -= 1;
			if(nCurPos<0)
				return;
			m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);
			break;
	case SB_PAGERIGHT:
		{
				if(nCurPos +m_VisibleColumn >= nMax )
				{
					m_VisibleColumn=nMax - nCurPos;
					nCurPos=nMax;
				}
				else
					nCurPos +=m_VisibleColumn;
				if(m_VisibleColumn <=0)
					return;
				int index=0;
				for(i=0;i< m_VisibleColumn;i++)
				{
					CCellID m_TopLeft=m_pGrid->GetTopleftNonFixedCell();
					if(m_TopLeft.col!=m_nCols-1)
					{
					  m_pGrid->SendMessage(WM_HSCROLL,SB_LINERIGHT,0);
					  index++;
					}
				}
				nCurPos -=m_VisibleColumn -index;
				break;
		}
	case SB_PAGELEFT:
		if(nCurPos - m_VisibleColumn <= nMin )
		{
            m_VisibleColumn=nCurPos - nMin;
		    nCurPos=nMin;
		}
		else
			nCurPos -=m_VisibleColumn;
		for(i=0;i< m_VisibleColumn;i++)
		{
			   m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);
        }
		break;
	case SB_THUMBTRACK:

		i = nPos - nCurPos;
		nCurPos = nPos;
		if(i < 0)
		{
		  for(int j = 0;j > i;j--)
		     m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);
		}
		if(i > 0)
		{
          int index=0;
		  for(int j = 0;j < i;j++)
		  {
	        CCellID m_TopLeft=m_pGrid->GetTopleftNonFixedCell();
			if(m_TopLeft.col!=m_nCols-1)
			{
               m_pGrid->SendMessage(WM_HSCROLL,SB_LINERIGHT,0);
			   index++;
			}
		  }
          nCurPos -=i-index;
		}
		else
			m_pGrid->SendMessage(WM_HSCROLL,SB_THUMBTRACK,0);
		break;
	default:
		break;
	}
///////////
    pScrollBar->SetScrollPos(nCurPos,true);


	long m_CurrRow=m_pGrid->GetCurrentRow();
	m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
    m_TopLeftCell.row= m_CurrRow;
	m_pGrid->SetFocusCell(m_TopLeftCell);

 	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CTaiShanReportView::InitAllTitle()
{
    m_nStartColumn=1;
	m_nRows = 450;
	m_nFixRows = 1;
	m_nFixCols = 3;
   	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	memcpy(&pDoc->m_nColumnIndex[0],&pDoc->m_SystemInitData.ColumnIndex[0],MAXCOLUME*sizeof(int));

	if(pDoc->m_nColumnIndex[0][1]==-1)
	{
		for(int i=0;i<MAXCOLUME;i++)
            pDoc->m_nColumnIndex[0][i] =-1;
		for(int i=0;i<INITCOLUME;i++)
		   pDoc->m_nColumnIndex[0][i]=i+ID_ROWNUM;
	}
	for(int i=0;i<3;i++)
       pDoc->m_nColumnIndex[1][i]=i+ID_ROWNUM;
	for(int i=3;i<13;i++)
       pDoc->m_nColumnIndex[1][i]=i+ID_ZGB -3;

	for(int i=0;i<3;i++)
       pDoc->m_nColumnIndex[2][i]=i+ID_ROWNUM;
	for(int i=3;i<16;i++)
       pDoc->m_nColumnIndex[2][i]=i+ID_ZZC-3;

	for(int i=0;i<3;i++)
       pDoc->m_nColumnIndex[3][i]=i+ID_ROWNUM;
	for(int i=3;i<10;i++)
       pDoc->m_nColumnIndex[3][i]=i+ID_ZYYWSR;

	for(int i=0;i<MAXCOLUME;i++)
	{
		CString str;
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			break;
		switch(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i])
		{
		case ID_ROWNUM:									
				str.Format("  ");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_STOCKID:                                      

				str.Format("代  码");
			    m_nColumnTitleArray.Add(str);
			    break;
		case ID_STOCKNAME:                                  
				str.Format("证券名称");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_YSTC:                              
				str.Format("昨收");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_OPEN:                         
				str.Format("今开");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HIGH:                            
				str.Format("最高");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LOW:                               
				str.Format("最低");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_NEW:                             
				str.Format("最新");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_TOTV:                         
				str.Format("总手");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_NOWV:                                
				str.Format("现手");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_TOTP:                                
			    str.Format("总额");
			m_nColumnTitleArray.Add(str);
			    break;
		case ID_AVGP:                              
				str.Format("均价");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ADDE:                                
				str.Format("涨跌");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HLP:                               
				str.Format("震幅");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_RDR:                                
				str.Format("涨幅");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BSSPBAS:                           
				str.Format("委比");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_DIFBS:                             
				str.Format("委差");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_VOLBI:                             
				str.Format("量比");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ACCB:                            
				str.Format("委买");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ACCS:                               //委卖
				str.Format("委卖");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BUY:                                //委买价
				str.Format("委买价");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_SEL:                                //委卖价
				str.Format("委卖价");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_RVOL:                               //内盘
				str.Format("内盘");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_DVOL:                              //外盘
				str.Format("外盘");
				m_nColumnTitleArray.Add(str);
			    break;
        case ID_EXCHANGE:
				str.Format("换手率");
				m_nColumnTitleArray.Add(str);
			    break;
        case ID_SYL:
				str.Format("市盈率");
				m_nColumnTitleArray.Add(str);
			    break; 
		case ID_ZGB:        //总股本(万股)
				str.Format("总股本(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GJG:        //国家股(万股)
				str.Format("国家股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
				str.Format("发起人法人股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_FRG:        //法人股(万股)
				str.Format("法人股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZGG:        //职工股(万股)
				str.Format("职工股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GZAG:       //公众A股(万股)
				str.Format("公众A股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LTAG:       //流通A股(万股)
				str.Format("流通A股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HG:         //Ｈ股(万股)
				str.Format("Ｈ股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BG:         //B股(万股)
				str.Format("B股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZPG:        //转配股(万股)
				str.Format("转配股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;

		case ID_ZZC:        //总资产(万元)
				str.Format("总资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LDZC:       //流动资产(万元)
				str.Format("流动资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_CQTZ:       //长期投资(万元)
				str.Format("长期投资(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GDZC:       //固定资产(万元)
				str.Format("固定资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_WXZC:       //无形资产(万元)
				str.Format("无形资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LDFZ:       //流动负债(万元)
				str.Format("流动负债(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_CQFZ:       //长期负债(万元)
				str.Format("长期负债(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GGQY:       //股东权益(万元)
				str.Format("股东权益(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
				str.Format("资本公积金(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
				str.Format("盈余公积金(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGJZ:       //每股净值(元)
				str.Format("每股净值(元)"); 
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
				str.Format("股东权益比率(%s)","%");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGGJJ:      //每股公积金(元)
				str.Format("每股公积金(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
				str.Format("主营业务收入(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
				str.Format("主营业务利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
				str.Format("其它业务利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LRZE:       //利润总额(万元)
				str.Format("利润总额(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_JLR:        //净利润(万元)
				str.Format("净利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_WFPLR:      //未分配利润(万元)
				str.Format("未分配利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGSY:       //每股收益(元)
				str.Format("每股收益(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
				str.Format("净资产收益率(%s)","%");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
				str.Format("每股未分配利润(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGJZC:      //每股净资产(元)
				str.Format("每股净资产(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZJLS:        //资金(%d分)
				str.Format("资金(%d分)",pDoc->m_syshqset.capital);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_VOLBIFM:    //量比(%d分)
				str.Format("量比(%d分)",pDoc->m_syshqset.quangtity);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HLPFM:      //震幅(%d分)
				str.Format("震幅(%d分)",pDoc->m_syshqset.swing);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_EXCHANGEFM:  //换手率(%d分)
				str.Format("换手率(%d分)",pDoc->m_syshqset.changehand);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ADVSP:       //涨速(%d分)
				str.Format("涨速(%d分)",pDoc->m_syshqset.upspeed);
				m_nColumnTitleArray.Add(str);
			    break;

		case ID_BUYP1:                               //
                m_nColumnTitleArray.Add(g_sBuySell[0]);break;
		case ID_BUYP2:                               //
                m_nColumnTitleArray.Add(g_sBuySell[1]);break;
		case ID_BUYP3:                               //
                m_nColumnTitleArray.Add(g_sBuySell[2]);break;
		case ID_BUYV1:                               //
                m_nColumnTitleArray.Add(g_sBuySell[3]);break;
		case ID_BUYV2:                               //
                m_nColumnTitleArray.Add(g_sBuySell[4]);break;
		case ID_BUYV3:                                //
                m_nColumnTitleArray.Add(g_sBuySell[5]);break;
		case ID_SELP1:                               //
                m_nColumnTitleArray.Add(g_sBuySell[6]);break;
		case ID_SELP2:                               //
                m_nColumnTitleArray.Add(g_sBuySell[7]);break;
		case ID_SELP3:                               //
                m_nColumnTitleArray.Add(g_sBuySell[8]);break;
		case ID_SELV1:                               //
                m_nColumnTitleArray.Add(g_sBuySell[9]);break;
		case ID_SELV2:                               //
                m_nColumnTitleArray.Add(g_sBuySell[10]);break;
		case ID_SELV3:                               //
                m_nColumnTitleArray.Add(g_sBuySell[11]);break;

		case ID_DELITEM:   
				str.Format("删除表项");
				m_nColumnTitleArray.Add(str);
			    break;
		default:
			    UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i];
				if(nID>=9000&&nID<9010)
				{
					TECHNIQUE_COLUMN *pIndexIndex;
					if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						return;
					str.Format("%s(%s)",pIndexIndex->nIndex.sIndex_name,pIndexIndex->nIndex.SubLineType);
					m_nColumnTitleArray.Add(str);
					break;
				}
			    break;
		}
	}

	m_nCols=m_nColumnTitleArray.GetSize(); 
    
    m_nRows=0;
    for(int i=0;i<8;i++)
      if(pDoc->m_sharesInformation.GetStockTypeCount(i)>m_nRows)
		  m_nRows=pDoc->m_sharesInformation.GetStockTypeCount(i);

	m_nRows +=5 ;
    
	if(m_nRows <25)
		m_nRows=450;


}



BOOL CTaiShanReportView::InitGridContent()
{
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    m_pGrid->SetMainDocument(pDoc);
	if(m_nRows <25)
       m_nRows=450;

	try {
 		m_pGrid->SetRowCount(m_nRows);
		m_pGrid->SetColumnCount(m_nCols);
		m_pGrid->SetFixedRowCount(m_nFixRows);
		m_pGrid->SetFixedColumnCount(m_nFixCols);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
   	m_pGrid->SetGridColor(pDoc->m_colorArray[18],pDoc->m_colorArray[17],pDoc->m_colorArray[16]);
//20000615
    m_pGrid->SetFixedBkColor(pDoc->m_colorArray[18]);


//20000615
	CFont Font;
	Font.CreateFontIndirect(&pDoc->m_fontstr[1]);
	m_pGrid->SetFont(&Font);
	Font.DeleteObject();
    char str[40];
	
	for(int i=0;i<MAXCOLUME;i++)
	{
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			break;
		int nID = pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i];
		if(nID>=ID_BUYP1 &&nID<=ID_SELV3)
		{
			strcpy(str ,g_sBuySell[nID-ID_BUYP1]);
			m_nColumnTitleArray.Add(str);
			m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str,nID);
		}
		else
		{
		switch(nID)
		{
		case ID_ROWNUM:										
				strcpy(str,"  ");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ROWNUM);
			    break;
		case ID_STOCKID:                                   
				strcpy(str,"代  码");
                m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_STOCKID);
			    break;
		case ID_STOCKNAME:                                    
				strcpy(str,"证券名称");
                m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_STOCKNAME);
			    break;
		case ID_YSTC:                             
				strcpy(str,"昨收");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_YSTC);
			    break;
		case ID_OPEN:                             
				strcpy(str,"今开");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_OPEN);
			    break;
		case ID_HIGH:                               
				strcpy(str,"最高");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HIGH);
			    break;
		case ID_LOW:                                
				strcpy(str,"最低");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LOW);
			    break;
		case ID_NEW:                               
				strcpy(str,"最新");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_NEW);
			    break;
		case ID_TOTV:                               
				strcpy(str,"总手");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_TOTV);
			    break;
		case ID_NOWV:                               
				strcpy(str,"现手");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_NOWV);
			    break;
		case ID_TOTP:                               
			    strcpy(str,"总额");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_TOTP);
			    break;
		case ID_AVGP:                              
				strcpy(str,"均价");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_AVGP);
			    break;
		case ID_ADDE:                              
				strcpy(str,"涨跌");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ADDE);
			    break;
		case ID_HLP:                             
				strcpy(str,"震幅");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HLP);
			    break;
		case ID_RDR:                               
				strcpy(str,"涨幅");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_RDR);
			    break;
		case ID_BSSPBAS:                          
				strcpy(str,"委比");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_BSSPBAS);
			    break;
		case ID_DIFBS:                            
				strcpy(str,"委差");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_DIFBS);
			    break;
		case ID_VOLBI:                           
				strcpy(str,"量比");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_VOLBI);
			    break;
		case ID_ACCB:                            
				strcpy(str,"委买");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ACCB);
			    break;
		case ID_ACCS:                             
				strcpy(str,"委卖");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ACCS);
			    break;
		case ID_BUY:                              
				strcpy(str,"委买价");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_BUY);
			    break;
		case ID_SEL:                              
				strcpy(str,"委卖价");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_SEL);
			    break;
		case ID_RVOL:                              
				strcpy(str,"内盘");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_RVOL);
			    break;
		case ID_DVOL:                              
				strcpy(str,"外盘");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_DVOL);
			    break;
        case ID_EXCHANGE:
				strcpy(str,"换手率");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_EXCHANGE);
			    break;
        case ID_SYL:
				strcpy(str,"市盈率");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_SYL);
			    break;


		case ID_ZGB:        //总股本(万股)
				strcpy(str,"总股本(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZGB);
			    break;
		case ID_GJG:        //国家股(万股)
				strcpy(str,"国家股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GJG);
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
				strcpy(str,"发起人法人股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_FQRFRG);
			    break;
		case ID_FRG:        //法人股(万股)
				strcpy(str,"法人股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_FRG);
			    break;
		case ID_ZGG:        //职工股(万股)
				strcpy(str,"职工股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZGG);
			    break;
		case ID_GZAG:       //公众A股(万股)
				strcpy(str,"公众A股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GZAG);
			    break;
		case ID_LTAG:       //流通A股(万股)
				strcpy(str,"流通A股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LTAG);
			    break;
		case ID_HG:         //Ｈ股(万股)
				strcpy(str,"Ｈ股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HG);
			    break;
		case ID_BG:         //B股(万股)
				strcpy(str,"B股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_BG);
			    break;
		case ID_ZPG:        //转配股(万股)
				strcpy(str,"转配股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZPG);
			    break;

		case ID_ZZC:        //总资产(万元)
				strcpy(str,"总资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZZC);
			    break;
		case ID_LDZC:       //流动资产(万元)
				strcpy(str,"流动资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LDZC);
			    break;
		case ID_CQTZ:       //长期投资(万元)
				strcpy(str,"长期投资(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_CQTZ);
			    break;
		case ID_GDZC:       //固定资产(万元)
				strcpy(str,"固定资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GDZC);
			    break;
		case ID_WXZC:       //无形资产(万元)
				strcpy(str,"无形资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_WXZC);
			    break;
		case ID_LDFZ:       //流动负债(万元)
				strcpy(str,"流动负债(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LDFZ);
			    break;
		case ID_CQFZ:       //长期负债(万元)
				strcpy(str,"长期负债(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_CQFZ);
			    break;
		case ID_GGQY:       //股东权益(万元)
				strcpy(str,"股东权益(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GGQY);
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
				strcpy(str,"资本公积金(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZBGJJ);
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
				strcpy(str,"盈余公积金(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_YYGJJ);
			    break;
		case ID_MGJZ:       //每股净值(元)
				strcpy(str,"每股净值(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGJZ);
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
				strcpy(str,"股东权益比率(%)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GDQYBL);
			    break;
		case ID_MGGJJ:      //每股公积金(元)
				strcpy(str,"每股公积金(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGGJJ);
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
				strcpy(str,"主营业务收入(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZYYWSR);
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
				strcpy(str,"主营业务利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZYYWLR);
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
				strcpy(str,"其它业务利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_QTYWLR);
			    break;
		case ID_LRZE:       //利润总额(万元)
				strcpy(str,"利润总额(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LRZE);
			    break;
		case ID_JLR:        //净利润(万元)
				strcpy(str,"净利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_JLR);
			    break;
		case ID_WFPLR:      //未分配利润(万元)
				strcpy(str,"未分配利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_WFPLR);
			    break;
		case ID_MGSY:       //每股收益(元)
				strcpy(str,"每股收益(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGSY);
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
				strcpy(str,"净资产收益率(%)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_JZCSYL);
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
				strcpy(str,"每股未分配利润(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGWFPLR);
			    break;
		case ID_MGJZC:      //每股净资产(元)
				strcpy(str,"每股净资产(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGJZC);
			    break;
		case ID_ZJLS:        //资金(%d分)
				sprintf(str,"资金(%d分)",pDoc->m_syshqset.capital);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZJLS);
			    break;
		case ID_VOLBIFM:    //量比(%d分)
				sprintf(str,"量比(%d分)",pDoc->m_syshqset.quangtity);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_VOLBIFM);
			    break;
		case ID_HLPFM:      //震幅(%d分)
				sprintf(str,"震幅(%d分)",pDoc->m_syshqset.swing);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HLPFM);
			    break;
		case ID_EXCHANGEFM:  //换手率(%d分)
				sprintf(str,"换手率(%d分)",pDoc->m_syshqset.changehand);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_EXCHANGEFM);
			    break;
		case ID_ADVSP:       //涨速(%d分)
				sprintf(str,"涨速(%d分)",pDoc->m_syshqset.upspeed);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ADVSP);
			    break;

		default:
			{
				UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i];
				if(nID>=9000&&nID<9010)
				{
					TECHNIQUE_COLUMN *pIndexIndex;
					if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						continue;
					sprintf(str,"%s(%s)",pIndexIndex->nIndex.sIndex_name,pIndexIndex->nIndex.SubLineType);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,nID);
				}
			}

			    break;
		}
		}
	}
    ChangeToPage(pDoc->m_nCurrentPage);
    m_PageWnd->m_nActivePage=pDoc->m_nCurrentPage;
    m_PageWnd->SetPageTitle(pDoc->m_SystemInitData.StockTypeName,STKTYPEPAGE);
    m_PageWnd->SetPageTitle(pDoc->m_SystemInitData.ScreenStockName,TJXGPAGE);

    m_PageWnd->m_bRefrash=TRUE;
    m_PageWnd->Invalidate();
    EndWaitCursor();
	SetCursor(LoadCursor(  NULL, IDC_ARROW )); 
 	return TRUE;
}

UINT CTaiShanReportView::GetActivePage()
{
	return m_PageWnd->m_nActivePage;
}



void CTaiShanReportView::RefreshData()
{

    m_pGrid->Invalidate();                          
}
void CTaiShanReportView::LoopDisplayOnTimer()
{

	::SendMessage(m_pGrid->m_hWnd,WM_KEYDOWN,VK_NEXT,0);
    
}
BOOL CTaiShanReportView::SetShowData(CTaiShanDoc *pDoc,int StockCount)
{
	HGLOBAL	hMem;
	LPVOID hp;
	STOCKDATASHOW  *ptempStockShow;
     if(!pDoc->m_pStockDataShow)
	 {
         hMem=GlobalAlloc( GPTR, (StockCount)* sizeof( STOCKDATASHOW) );
	     hp=GlobalLock(hMem);
		 if( hp )
		 {
            pDoc->m_pStockDataShow=(STOCKDATASHOW *)hp;
		    pDoc->m_nShowMaxCount = StockCount;
			return TRUE;
		 }
		 else
		 {
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		 }
	 }
	 else
	 {
		 if(pDoc->m_nShowMaxCount >= StockCount)
			 return TRUE;
		 else
		 {
			 hMem=GlobalAlloc( GPTR, (StockCount)* sizeof( STOCKDATASHOW) );
			 hp=GlobalLock(hMem);
			 if( hp )
			 {
				ptempStockShow=(STOCKDATASHOW *)hp;

 				GlobalUnlock((HGLOBAL)pDoc->m_pStockDataShow);        
				GlobalFree( (HGLOBAL)pDoc->m_pStockDataShow);
                pDoc->m_pStockDataShow=ptempStockShow;
                pDoc->m_nShowMaxCount=StockCount;
				return TRUE;
			 }
			 else
			 {
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return FALSE;
			 }
		 }
	 }
}
void CTaiShanReportView::LoadShowData(int nKind)
{
	CTaiShanDoc* pDoc = GetDocument();
	int nType;
	nType = GetStockTypeFromPageWnd(m_PageWnd->m_nActivePage);
	if(nType == -1) return;

    if(nKind!=nType)
		return;
    SetShowData(pDoc,pDoc->m_sharesInformation.GetStockTypeCount(nType));
	pDoc->m_sharesInformation.RecvStockDataForType(pDoc->m_pStockDataShow,nType);

	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, pDoc->m_sharesInformation.GetStockTypeCount(nType),this);

}

BOOL CTaiShanReportView::ChangeToTjxg(CTaiShanDoc* pDoc,CString m_StockType)
{
	BLOCKSTOCK * blocknow;
	BOOL m_finddone=FALSE;
	pDoc->m_nCurrentPage=TJXGPAGE;
	POSITION pos=pDoc->m_Tjxgblocklist.GetHeadPosition();
    while(pos)
	{
	    blocknow=pDoc->m_Tjxgblocklist.GetNext(pos);
		if(blocknow->blockname== m_StockType )
		{
             m_finddone=TRUE;
			 break;
		}
	}
    if(!m_finddone)                                 
	{
	   m_pGrid->SetRealRow(1);
	   m_pGrid->Invalidate();                          
	   m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
       
   	   m_PageWnd->m_bMove = true;
       m_PageWnd->m_nActivePage=TJXGPAGE;
       m_PageWnd->m_bRefrash=TRUE;
	   m_PageWnd->SetPageTitle("条件选股",TJXGPAGE);
       m_PageWnd->Invalidate();
       strcpy(pDoc->m_SystemInitData.ScreenStockName,"条件选股");
	   return FALSE ;   
	}
    
    strcpy(pDoc->m_SystemInitData.ScreenStockName,m_StockType.GetBuffer(0));
    m_PageWnd->SetPageTitle(m_StockType.GetBuffer(0),TJXGPAGE);
    pDoc->m_StockTypeMap[2].RemoveAll();
    int index=0;
    int DisplayCount=blocknow->stocklist.GetCount();
    SetShowData(pDoc,DisplayCount);

	for(int row=1 ;row <=  (int)DisplayCount  ;row++)       
	{    
		 CReportData *Cdat;
		 CString m_zqdm;
		 SharesSymbol *m_sharesymbol;
   	     POSITION pos =blocknow->stocklist.FindIndex( row - 1 ); 
		 if(pos==NULL)
			 break;
         m_sharesymbol= blocknow->stocklist.GetAt(pos);
		 m_zqdm=m_sharesymbol->sharesymbol ;
	 	 if (pDoc->m_sharesInformation.Lookup(m_sharesymbol->sharesymbol , Cdat,m_sharesymbol->nKind) == TRUE)     //检测该股票是否已记录在内存模板中
		 {
             pDoc->m_pStockDataShow[index].pItem=Cdat;

             strcpy(pDoc->m_pStockDataShow[index].StockId,Cdat->id);
             
			 CString m_zqdmkind=pDoc->GetStockKindString(m_sharesymbol->nKind);
			 m_zqdmkind+=m_zqdm;
             pDoc->m_StockTypeMap[2][m_zqdmkind]=m_zqdm;
			 index++;
		 }
	}
	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, index,this);

    m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
	m_pGrid->SetRealRow(index+1); 
    m_pGrid->Invalidate();                        

	UINT TotRow=0;
    if(DisplayCount > m_nRows)
	{
		TotRow=DisplayCount +5;
 		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return FALSE;
		}
		m_nRows=TotRow;
	}
    m_PageWnd->m_bRefrash=TRUE;
    m_PageWnd->SetPageTitle(m_StockType,TJXGPAGE);
    m_PageWnd->Invalidate();

     pDoc->m_nPreDisplayRow=DisplayCount ;       
     return TRUE;
}

void CTaiShanReportView::ChangeToTjxg(CTaiShanDoc* pDoc)
{
    int DisplayCount=pDoc->m_ScreenList.GetSize();
    m_PageWnd->m_nActivePage=TJXGPAGE;
    m_PageWnd->m_bRefrash=TRUE;
    m_PageWnd->Invalidate();
    int index=0;        
    SetShowData(pDoc,DisplayCount);

    pDoc->m_StockTypeMap[2].RemoveAll();

	for(int row=0 ;row <DisplayCount  ;row++)     
	{    
		 CReportData *Cdat;
		 SharesSymbol m_StockSymbol;
         m_StockSymbol=pDoc->m_ScreenList.GetAt(row) ;
	 	 if (pDoc->m_sharesInformation.Lookup(m_StockSymbol.sharesymbol, Cdat,m_StockSymbol.nKind) == TRUE)     //检测该股票是否已记录在内存模板中
		 {
			 
             pDoc->m_pStockDataShow[index].pItem=Cdat;

             strcpy(pDoc->m_pStockDataShow[index ].StockId,Cdat->id); 
 
			 CString m_zqdm=m_StockSymbol.sharesymbol;
			 CString m_zqdmkind=pDoc->GetStockKindString(m_StockSymbol.nKind);
			 m_zqdmkind+=m_zqdm;
		     pDoc->m_StockTypeMap[2][m_zqdmkind]=m_zqdm;
             index++; 
		 }
	}
	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, index,this);

    m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
	m_pGrid->SetRealRow(index+1); 

    m_pGrid->Invalidate();                      
    
	m_PageWnd->SetPageTitle("条件选股",TJXGPAGE);
	strcpy(pDoc->m_SystemInitData.ScreenStockName,"条件选股");
    m_PageWnd->Invalidate();
    

	UINT TotRow=0;
    if(DisplayCount > m_nRows)
	{
		TotRow=DisplayCount +5;
 		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		m_nRows=TotRow;
	}

    pDoc->m_nPreDisplayRow=DisplayCount;       
}
void CTaiShanReportView::ChangeToTjxg(CTaiShanDoc* pDoc,BOOL IsNewTjxg)
{
	POSITION pos;
    m_PageWnd->m_nActivePage=TJXGPAGE;
    m_PageWnd->m_bRefrash=TRUE;
    m_PageWnd->Invalidate();
    int index=0;        
    int DisplayCount=pDoc->m_StockTypeMap[2].GetCount();
    SetShowData(pDoc,DisplayCount);

	pos=pDoc->m_StockTypeMap[2].GetStartPosition( );
	while(pos)
	{    
		 CReportData *Cdat;
		 CString m_zqdm,m_zqdmkind;
         pDoc->m_StockTypeMap[2].GetNextAssoc( pos,m_zqdmkind,m_zqdm) ;
         int nKind=pDoc->GetStockKind(m_zqdmkind.Left(2));
	 	 if (pDoc->m_sharesInformation.Lookup(m_zqdm.GetBuffer(0) , Cdat,nKind) == TRUE)     
		 {
             pDoc->m_pStockDataShow[index].pItem=Cdat;

             strcpy(pDoc->m_pStockDataShow[index ].StockId,Cdat->id); 
             index++; 
		 }
	}
	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, index,this);

    m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
	m_pGrid->SetRealRow(index+1);

    m_pGrid->Invalidate();                  
    
	m_PageWnd->SetPageTitle("条件选股",TJXGPAGE);
	strcpy(pDoc->m_SystemInitData.ScreenStockName,"条件选股");
    m_PageWnd->Invalidate();
    

	UINT TotRow=0;
    if(DisplayCount > m_nRows)
	{
		TotRow=DisplayCount +5;
 		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		m_nRows=TotRow;
	}

    pDoc->m_nPreDisplayRow=DisplayCount;       
}



BOOL CTaiShanReportView::ChangeToStockType(CTaiShanDoc* pDoc,CString m_StockType) 
{


	BOOL m_finddone=FALSE;
	pDoc->m_nCurrentPage=5;
    SymbolKindArr m_StockCodeArray;
	if(pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(m_StockCodeArray,m_StockType.GetBuffer(0)))          
	{
        m_finddone=TRUE;
	}
    if(!m_finddone)                            
	{
	   m_pGrid->SetRealRow(1); 
	   m_pGrid->Invalidate();                           
	   m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
       
   	   m_PageWnd->m_bMove = true;
       m_PageWnd->m_nActivePage=STKTYPEPAGE;
       m_PageWnd->m_bRefrash=TRUE;
	   m_PageWnd->SetPageTitle("板块股票",STKTYPEPAGE);
       m_PageWnd->Invalidate();
       strcpy(pDoc->m_SystemInitData.StockTypeName,"板块股票");
	   return FALSE ;   
	}
    
    strcpy(pDoc->m_SystemInitData.StockTypeName,m_StockType.GetBuffer(0));
    m_PageWnd->SetPageTitle(m_StockType.GetBuffer(0),STKTYPEPAGE);

    pDoc->m_StockTypeMap[1].RemoveAll();

    int DisplayCount=m_StockCodeArray.GetSize();
	int index=0;
    SetShowData(pDoc,DisplayCount);

	for(int row=0 ;row <  (int)DisplayCount  ;row++)          
	{    
		 CReportData *Cdat;
		 CString m_zqdm; 
		 CString m_zqdmkind;
		 SymbolKind m_SymbolKind;
		 m_SymbolKind=m_StockCodeArray.GetAt(row) ;
	 	 if (pDoc->m_sharesInformation.Lookup(m_SymbolKind.m_chSymbol , Cdat,m_SymbolKind.m_nSymbolKind) == TRUE)     //检测该股票是否已记录在内存模板中
		 {
			 if(Cdat->pBaseInfo==NULL)
				 continue;

		
             pDoc->m_pStockDataShow[index ].pItem=Cdat;

             strcpy(pDoc->m_pStockDataShow[index ].StockId,Cdat->id); 
             m_zqdm=m_SymbolKind.m_chSymbol;
			 m_zqdmkind=pDoc->GetStockKindString(m_SymbolKind.m_nSymbolKind);
			 m_zqdmkind+=m_zqdm;
             pDoc->m_StockTypeMap[1][m_zqdmkind]=m_zqdm;
			 index++;
		 }
	}
	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, index,this);

    m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
	m_pGrid->SetRealRow(index+1); 
    m_pGrid->Invalidate();                          

	UINT TotRow=0;
    if(DisplayCount > m_nRows)
	{
		TotRow=DisplayCount +5;
 		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return FALSE;
		}
		m_nRows=TotRow;
	}

     pDoc->m_nPreDisplayRow=DisplayCount ; 
	 return TRUE;
}

BOOL CTaiShanReportView::PreTranslateMessage(MSG* pMsg) 
{

	CRect rc;
	GetClientRect(&rc);
	if(m_pColumnSelect&&(pMsg->message==WM_LBUTTONDOWN||pMsg->message==WM_RBUTTONDOWN))
	{
		::SendMessage(m_pColumnSelect->m_hWnd,WM_CLOSE,0,0);
        m_pColumnSelect=NULL;
	}

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN  || 
		pMsg->wParam == VK_PRIOR || pMsg->wParam ==  VK_NEXT))
	{
		  m_pGrid->SetFocus();	
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F3)
	{
	  CTaiShanDoc *pDoc = GetDocument();
	  ::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_FROM_KLINE,0,3);	
	}
	
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4)
	{
	  CTaiShanDoc *pDoc = GetDocument();
	  ::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_FROM_KLINE,0,4);	
	}
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
	        CTaiShanDoc *pDoc = GetDocument();
			int l_TotalRows;
			int ss=m_pGrid->GetSortColumn();
			if(m_pGrid->GetSortColumn()!=-1)
			{
				int StockType=0;
				StockType = GetStockTypeFromPageWnd(m_PageWnd->m_nActivePage);
				if(StockType>=0)
				{
					l_TotalRows=pDoc->m_sharesInformation.GetStockTypeCount(StockType);
				}
				else
				{
					switch(m_PageWnd->m_nActivePage)
					{
						case CHOOSEPAGE:
									  l_TotalRows=pDoc->m_StockTypeMap[0].GetCount();
									  break;
						case STKTYPEPAGE:
									  l_TotalRows=pDoc->m_StockTypeMap[1].GetCount();
									  break;
						case TJXGPAGE:
									  l_TotalRows=pDoc->m_StockTypeMap[2].GetCount();
									  break;
					}
				}
				 m_pGrid->SetSortColumn(-1 ,TRUE);
				 m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
                 QuickSortString(pDoc->m_pStockDataShow,TRUE,0,l_TotalRows - 1);
			 	 m_pGrid->ClearSortColumn();
                 m_pGrid->SetFocusRow(1); 
                 m_pGrid->Invalidate();                            
  	             m_pGrid->SetMouseState(MOUSE_NOTHINGS);
			}
	        return true;
		}
	if(pMsg->message==WM_RBUTTONUP)
	{
	
		CPoint m_pos;
	    CTaiShanDoc *pDoc = GetDocument();
		m_pos=pMsg->pt;
		ScreenToClient(&m_pos);
        m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);
        if(m_CellTitleCommand.row == 0 && ((m_CellTitleCommand.col > m_pGrid->GetFixedColumnCount() - 1)|| m_CellTitleCommand.col==-1))
		{
		#ifndef OEM
			int nPos;
			CRect nRect;
			BOOL IsDelete=FALSE;
			GetWindowRect(nRect ); 
			if(pDoc->m_SystemInitData.ColumnType!=0)
				return TRUE;
			if(pMsg->pt.x +155 > nRect.right )
               nPos=pMsg->pt.x -155;
			else
               nPos=pMsg->pt.x ; 
           if(m_CellTitleCommand.col==m_pGrid->GetColumnCount() -1 )
		   {
			   IsDelete=TRUE;
		   }
           CString Index; 
		   BOOL IsIndex=FALSE;
           if(pDoc->m_nColumnIndex[0][m_CellTitleCommand.col]>=9000&&pDoc->m_nColumnIndex[0][m_CellTitleCommand.col]<9010)
		   {
			   TECHNIQUE_COLUMN *pIndexIndex;
			   if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(pDoc->m_nColumnIndex[0][m_CellTitleCommand.col],pIndexIndex))
					return TRUE;
			   Index=pIndexIndex->nIndex.sIndex_name;
			   IsIndex=TRUE;
		   }


		 
		   {
			   CColumnSelect dlg(pDoc->m_nColumnIndex[0][m_CellTitleCommand.col],Index,IsDelete,IsIndex,nPos,pMsg->pt.y,this );
			   dlg.ShowMenu();

		   }
#endif
			return TRUE;
		}
		else if(m_CellTitleCommand.row > 0 && m_pos.y < rc.bottom - 16 )
		{
		#ifndef OEM
			CMenu * pMenuContext=m_pPopMenu->GetSubMenu(0);
			CMenu menu;
			CString strMenuText;
			pMenuContext->GetMenuString(5,strMenuText,MF_BYPOSITION);
			if(strMenuText==_T("加入到板块"))
			{ 
               pMenuContext->DeleteMenu(5,MF_BYPOSITION);
			}
			pMenuContext->GetMenuString(ID_DELETE_TJXGTYPES,strMenuText,MF_BYCOMMAND);
			if(strMenuText==_T("删除条件板块"))
			{ 
               pMenuContext->DeleteMenu(ID_DELETE_TJXGTYPES,MF_BYCOMMAND);
			}
			menu.CreatePopupMenu();
			pMenuContext->InsertMenu(5,MF_POPUP | MF_STRING|MF_BYPOSITION, (UINT)menu.m_hMenu, "加入到板块");
            if(m_PageWnd->m_nActivePage==6)
			  pMenuContext->AppendMenu(MF_STRING|MF_BYPOSITION, ID_DELETE_TJXGTYPES, "删除条件板块");


		
			pMenuContext->GetMenuString(ID_MENU_TYPE2, strMenuText, MF_BYCOMMAND);
			if (strMenuText != _T("股本构成") ) {
				pMenuContext->AppendMenu(MF_SEPARATOR);
				pMenuContext->AppendMenu(MF_STRING|MF_BYPOSITION, ID_MENU_TYPE1, "行情显示");
				pMenuContext->AppendMenu(MF_STRING|MF_BYPOSITION, ID_MENU_TYPE2, "股本构成");
				pMenuContext->AppendMenu(MF_STRING|MF_BYPOSITION, ID_MENU_TYPE3, "资产负债");
				pMenuContext->AppendMenu(MF_STRING|MF_BYPOSITION, ID_MENU_TYPE4, "利润指标");
			}

			CStringArray m_StockTypeNameArray;
			int index=9900;
			pDoc->m_ManagerStockTypeData.GetAllStockTypeName(m_StockTypeNameArray);
			int l_nCount=m_StockTypeNameArray.GetSize();
			int CurrentPos=-1;
			for(int i=0;i<l_nCount;i++,index++)
			{
			    CString l_sStockType=m_StockTypeNameArray.GetAt(i);
				if(i%8==0 && i!=0)
					menu.AppendMenu(MF_STRING|MF_MENUBARBREAK ,
						 index, l_sStockType);
                else
			        menu.AppendMenu(MF_STRING  ,index ,l_sStockType );
				if(	pDoc->m_nCurrentPage==5&&pDoc->m_SystemInitData.StockTypeName==l_sStockType)
					CurrentPos=i;
			}
            menu.EnableMenuItem( CurrentPos,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
            
			PopMenuItemInit(pMenuContext,m_CellTitleCommand.row);
			if(strlen(m_pGrid->GetItemText(m_CellTitleCommand.row,1))==4||
				strlen(m_pGrid->GetItemText(m_CellTitleCommand.row,1))==6)
			{
				m_pGrid->OnLButtonDown(1, m_pos);
			}
	        pMenuContext->TrackPopupMenu( TPM_LEFTALIGN|TPM_RIGHTBUTTON, pMsg->pt.x, pMsg->pt.y, this, NULL );
	#endif
			return TRUE;
		}
	}
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		CPoint m_pos;
	    CTaiShanDoc *pDoc = GetDocument();
	    int nRow=m_pGrid->GetCurrentRow();
		int l_TotalRows;
		m_pos=pMsg->pt;
		ScreenToClient(&m_pos);
        m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);

		if(m_CellTitleCommand.row==0&&m_CellTitleCommand.col==1)
		{
			int ss=m_pGrid->GetSortColumn();
			if(m_pGrid->GetSortColumn()!=-1)
			{
				l_TotalRows = GetTotalRowsLmb(m_PageWnd->m_nActivePage);

				m_pGrid->SetSortColumn(-1 ,TRUE);
				 m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
                 QuickSortString(pDoc->m_pStockDataShow,TRUE,0,l_TotalRows - 1);
			 	 m_pGrid->ClearSortColumn();
                 m_pGrid->SetFocusRow(1); 
                 m_pGrid->Invalidate();                         
  	             m_pGrid->SetMouseState(MOUSE_NOTHINGS);
			}
	        return true;
		}
	}

	if(pMsg->message==WM_LBUTTONDBLCLK||pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
	{
	    CPoint m_pos;
		CString stock_code,stock_name;
		CTaiShanDoc *pDoc = GetDocument();
			
		if(pMsg->message==WM_LBUTTONDBLCLK)
		{
			m_pos=pMsg->pt;
			ScreenToClient(&m_pos);
			m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);
			if(m_CellTitleCommand.row == 0)
			   return FALSE;
		}
		else 
		{
            m_CellTitleCommand.row=m_pGrid->GetCurrentRow();
			if(m_CellTitleCommand.row == 0)
			   return FALSE;
            m_CellTitleCommand.col=2;
		}

		if(m_CellTitleCommand.row > 0 && m_pos.y < rc.bottom - 16||
			m_CellTitleCommand.row > 0&&pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		{
			m_LoopDisplay=FALSE;
			CReportData *pCdat;
			if(!this->GetCdatPointForRow(m_CellTitleCommand.row,pCdat))
				return TRUE;
			stock_code =pCdat->id;
			stock_name =pCdat->name;
            if(stock_code.Compare("") == 0)
			{
				MessageBox("请选择某一股票进入K线!","操作提示",MB_OK);
				return FALSE;
			}
			pDoc->m_sharesSymbol = stock_code;
			pDoc->m_stkKind=pCdat->kind;
			pDoc->m_GetFocus = true;
			CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
			pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),pDoc->m_stkKind*10);
		}
	}
	if(pMsg->message==WM_CHAR)
	{
		if((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z'))
		{
				CTaiKeyBoardAngelDlg m_diakeybrode;
				if(pMsg->wParam>='a'&&pMsg->wParam<='z')
					pMsg->wParam-=32;
				char input=pMsg->wParam;
				m_diakeybrode.input=input;
				if(pMsg->wParam>='0'&&pMsg->wParam<='9')
					m_diakeybrode.ischar=false;
				else
					m_diakeybrode.ischar=true;

			m_diakeybrode.DoModal();
			if(m_diakeybrode.isresultok)
			{
			   char result[10];		   
			   int nKind;
			   strcpy(result,m_diakeybrode.result);
			   nKind=m_diakeybrode.m_stkKind;
			   if(strlen(result) == 2&&!m_diakeybrode.ischar)
			   {
				   if(strcmp(result,"10") == 0)
				 {
					   OnCompanyinfo();
				 }
			   }
			   
			   if(strlen(result)==6||strlen(result)==4)
			   {
          		   BeginWaitCursor();
				   ScrollToSelectStock(result,nKind);
				   EndWaitCursor();
			   }
			   else if(strlen(result)==2&&!m_diakeybrode.ischar)
			   {
          		   BeginWaitCursor();

				   HotKeyDownProgress(result);
				   EndWaitCursor();

	
				   return true;
			   }

				CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
				long m_CurrRow=m_pGrid->GetCurrentRow();
				m_TopLeftCell.row= m_CurrRow;
				m_pGrid->SetFocusCell(m_TopLeftCell);
				m_pGrid->SetFocus();
			}
			return true;
		}
		else if(pMsg->wParam=='W' || pMsg->wParam=='w')
		{
			if(::GetKeyState(VK_CONTROL)&0x8000)
			{
				SwitchFinance() ;
				return true;
			}
		}
	}

    if(pMsg->message ==WM_KEYDOWN&&pMsg->wParam ==VK_RIGHT)
	{
        OnHScroll(SB_LINERIGHT, 0, m_hScrollBar);
		return TRUE;
	}else if(pMsg->message ==WM_KEYDOWN&&pMsg->wParam ==VK_LEFT)
	{
        OnHScroll(SB_LINELEFT, 0, m_hScrollBar);
		return TRUE;
	}
	return CView::PreTranslateMessage(pMsg);
}
LRESULT CTaiShanReportView::OnColumnChange(WPARAM wParam, LPARAM lParam)
{
	UINT nId=(long)wParam;
	if(m_pColumnSelect)
	{
		::SendMessage(m_pColumnSelect->m_hWnd,WM_CLOSE,0,0);
        m_pColumnSelect=NULL;
	}
	DoUpdateColumn(nId);

	return 0;

}
LRESULT CTaiShanReportView::OnIndexColumnChange(WPARAM wParam, LPARAM lParam)
{
	UINT nId=(long)wParam;
	char Index[50];
    if(nId==98887)
        strcpy(Index,(char *)lParam);
    CTaiShanDoc *pDoc = GetDocument();
	if(m_pColumnSelect)
	{
		m_pColumnSelect->ShowWindow( SW_HIDE );
	}

	switch(nId)
	{
		case 98887:
			{
				TECHNIQUE_COLUMN *pIndexIndex;
				IndexDataInfo nIndex_Save;
				BOOL isInit=FALSE;
				UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col];
				if(nID>=9000)
				{
                    if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						break;
					if(strcmp(pIndexIndex->nIndex.sIndex_name,Index )==0 )
					{
						  memcpy(&nIndex_Save,&pIndexIndex->nIndex,106);
                          isInit=TRUE;
					}
					else
					{
                          nIndex_Save.iKind=-1;                     
						  strcpy(nIndex_Save.sIndex_name,Index);            
					}
				}
				else
				{
					nID=pDoc->m_pTechniqueColumn->FindSolt(pIndexIndex);
					if(nID==-1)
						break;
                    nIndex_Save.iKind=-1;                    
				    strcpy(nIndex_Save.sIndex_name,Index)          ;         
				}
	            SortIndex nSortIndex(&nIndex_Save,nID,isInit);
				if(nSortIndex.DoModal()==IDOK)
				{
					memcpy(&pIndexIndex->nIndex ,&nSortIndex.m_nIndexSave,106);
					strcpy(pIndexIndex->nIndex.SubLineType,nSortIndex.m_sLTypeName.GetBuffer(0) );
					pIndexIndex->IsUse =TRUE;
					pIndexIndex->nID=nID;
					this->SendMessage(WM_USER_COLUMNSELECT,nID,0);       
				}
			}
			break;
		case 98888:
			{
				TECHNIQUE_COLUMN *pIndexIndex;
				IndexDataInfo nIndex_Save;
				BOOL isInit=FALSE;
				UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col];
				if(nID>=9000)
				{
                    if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						break;
		    	    memcpy(&nIndex_Save,&pIndexIndex->nIndex,106);
                    isInit=TRUE;
				}
	            SortIndex nSortIndex(&nIndex_Save,nID,isInit,FALSE);
				if(nSortIndex.DoModal()==IDOK)
				{
				    memcpy(&pIndexIndex->nIndex ,&nSortIndex.m_nIndexSave,106);
					strcpy(pIndexIndex->nIndex.SubLineType,nSortIndex.m_sLTypeName.GetBuffer(0) );
					pIndexIndex->IsUse =TRUE;
					pIndexIndex->nID=nID;
					this->SendMessage(WM_USER_COLUMNSELECT,nID,0);       
				}
			}
			break;
		case 98889:
				TECHNIQUE_COLUMN *pIndexIndex;
				UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col];
				if(nID>=9000)
				{
                    if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						break;
				}
	            CCalcIndex nCalcIndex(&pIndexIndex->nIndex ,nID);
				nCalcIndex.DoModal();
			    break;
	}
	if(m_pColumnSelect)
	{
		::SendMessage(m_pColumnSelect->m_hWnd,WM_CLOSE,0,0);
        m_pColumnSelect=NULL;
	}

	SortDataForColumn();

	return 0;
}


void CTaiShanReportView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}



BOOL CTaiShanReportView::OnCmdMsg(UINT nID, int nCode, void* pExtra,
							AFX_CMDHANDLERINFO* pHandlerInfo)
{
	
	if (pHandlerInfo == NULL)
	{
	
			if (nCode == CN_COMMAND)
			{
				if(nID==ID_DELETE_TJXGTYPES)
					OnDeleteTjxgStockType();
				if(nID>=9900&&nID<=9999)
				{
	              OnAddStocktype(nID -9900);
				}
				for (int i = 68-1; i >= 0; i--)
				{
					if (nID == m_TitleMenuDef[i].m_nID)
					{
						DoUpdateColumn(nID);
						return TRUE;
					}
				}
				if(nID>179 && nID<1000)
				{
					CColumnSelect dlg;
					int nId = dlg.GetId(nID);
					if(nId<9000)
					  ::SendMessage(m_hWnd,WM_USER_COLUMNSELECT ,nId,0);       
					else
					{
						CTaiShanDoc *pDoc = CMainFrame::m_taiShanDoc ;
						int num=pDoc->m_formuar_index.GetSize();
						if(nId-9000>=0 &&nId-9000<num)
						{
							::SendMessage(m_hWnd, WM_USER_JISHUCOLUMNSELECT ,98887,(long )((pDoc->m_formuar_index.ElementAt(nId-9000)->name.GetBuffer (0) )));       
						}
					}
				}


			}
	}
	
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CTaiShanReportView::DoUpdateColumn(UINT nID)
{

	BOOL DeleteDone=FALSE;
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(nID==ID_DEFAULT)
	{
		DefalutColumnSelect();
		pDoc->m_pTechniqueColumn->SetAllColumnNotUse();
	}
	else if(nID==ID_DELITEM)       
	{
	  int rtn = MessageBox("是否确实删除这一列数据?","警告",MB_YESNO|MB_ICONWARNING);
	  if(rtn!=6)
		 return;
	  m_nColumnTitleArray.RemoveAt(m_CellTitleCommand.col  ,  1 );
	  m_pGrid->DeleteColumn(m_CellTitleCommand.col);
      if(m_CellTitleCommand.col==m_pGrid->GetSortColumn())
         m_pGrid->SetSortColumn(-1 ,TRUE); 

      m_pGrid->Invalidate();
      m_nCols=m_pGrid->GetColumnCount();
	  if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col]>=9000)
	  {
         pDoc->m_pTechniqueColumn->SetColumnNotUse(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col]);
	  }
      pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col]=-1 ;                   
	}
	else 	
	{
#ifdef WIDE_NET_VERSION
		if(nID==ID_BSSPBAS||nID==ID_DIFBS||nID==ID_VOLBI||nID==ID_ACCB||nID==ID_BUY||nID==ID_SEL)
		{
		    MessageBox("宽带版客户端中将不提供该列显示!","警告",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		if(nID==ID_ZJLS||nID==ID_VOLBIFM||nID==ID_HLPFM ||nID==ID_EXCHANGEFM||nID==ID_ADVSP)
		{
		    MessageBox("宽带版客户端中将不提供分时数据统计列显示!","警告",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
#endif
		if(m_CellTitleCommand.col>0) 
		{
           DeleteDone=TRUE; 
		   if(nID<9000&&pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col]>=9000)
		   {
                pDoc->m_pTechniqueColumn->SetColumnNotUse(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col]);
		   }
		}
		else
		{
			if(m_nCols >=MAXCOLUME   )
				return;
            m_CellTitleCommand.col=m_nCols ;
		}

		try
		{
        pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][m_CellTitleCommand.col ]=nID;
		if(DeleteDone==TRUE)
              UpdateColumn(m_CellTitleCommand.col,nID);                               
		else
			  AddColumn(m_CellTitleCommand.col,nID);
		}
		catch(...)
		{
		}
		
	}

	m_CellTitleCommand.col=-1;



	int pos=m_hScrollBar->GetScrollPos();
	int nMax=m_nCols - m_nFixCols +1;
	if(pos > nMax)
		pos=nMax;
	m_hScrollBar->SetScrollRange( 0, nMax, TRUE);
    m_hScrollBar->SetScrollPos(pos,true);
	if(nID==ID_DELITEM)           
	  m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);


	memcpy(&pDoc->m_SystemInitData.ColumnIndex[0],pDoc->m_nColumnIndex[0],sizeof(int)*MAXCOLUME);
}
void  CTaiShanReportView::AddColumn(UINT m_Column,UINT nID)
{
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    CString str;
	m_Column++;
	try { m_pGrid->SetColumnCount(m_Column); }
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
	m_Column--;
	if(nID>=ID_BUYP1 &&nID<=ID_SELV3)
	{
		str = g_sBuySell[nID-ID_BUYP1];
		m_nColumnTitleArray.Add(str);
        m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
			str.GetBuffer(0),nID);
	}
	else
	{
	switch(nID)
	{
		case ID_STOCKID:                                    

			    str.Format("代  码");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0) ,ID_STOCKID);
			    break;
		case ID_STOCKNAME:                                    
				str.Format("证券名称");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_STOCKNAME);
			    break;
		case ID_YSTC:                               
				str.Format("昨收");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_YSTC);
			    break;
		case ID_OPEN:                             
				str.Format("今开");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_OPEN);
			    break;
		case ID_HIGH:                               
				str.Format("最高");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HIGH);
			    break;
		case ID_LOW:                                
				str.Format("最低");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LOW);
			    break;
		case ID_NEW:                                
				str.Format("最新");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_NEW);
			    break;
		case ID_TOTV:                               
				str.Format("总手");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_TOTV);
			    break;
		case ID_NOWV:                                
				str.Format("现手");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_NOWV);
			    break;
		case ID_TOTP:                               
			    str.Format("总额");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_TOTP);
			    break;
		case ID_AVGP:                               
				str.Format("均价");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_AVGP);
			    break;
		case ID_ADDE:                               
				str.Format("涨跌");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ADDE);
			    break;
		case ID_HLP:                             
				str.Format("震幅");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HLP);
			    break;
		case ID_RDR:                               
				str.Format("涨幅");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_RDR);
			    break;
		case ID_BSSPBAS:                           
				str.Format("委比");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_BSSPBAS);
			    break;
		case ID_DIFBS:                            
				str.Format("委差");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_DIFBS);
			    break;
		case ID_VOLBI:                              //量比
				str.Format("量比");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_VOLBI);
			    break;
		case ID_ACCB:                               //委买
				str.Format("委买");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ACCB);
			    break;
		case ID_ACCS:                               //委卖
				str.Format("委卖");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ACCS);
			    break;
		case ID_BUY:                                //委买价
				str.Format("委买价");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_BUY);
			    break;
		case ID_SEL:                                //委卖价
				str.Format("委卖价");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_SEL);
			    break;
		case ID_RVOL:                               //内盘
				str.Format("内盘");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_RVOL);
			    break;
		case ID_DVOL:                              //外盘
				str.Format("外盘");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_DVOL);
			    break;
        case ID_EXCHANGE:
				str.Format("换手率");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_EXCHANGE);
			    break;
        case ID_SYL:
				str.Format("市盈率");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_SYL);
			    break;
		case ID_ZGB:        //总股本(万股)
				str.Format("总股本(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZGB);
			    break;
		case ID_GJG:        //国家股(万股)
				str.Format("国家股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GJG);
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
				str.Format("发起人法人股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_FQRFRG);
			    break;
		case ID_FRG:        //法人股(万股)
				str.Format("法人股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_FRG);
			    break;
		case ID_ZGG:        //职工股(万股)
				str.Format("职工股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZGG);
			    break;
		case ID_GZAG:       //公众A股(万股)
				str.Format("公众A股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GZAG);
			    break;
		case ID_LTAG:       //流通A股(万股)
				str.Format("流通A股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LTAG);
			    break;
		case ID_HG:         //Ｈ股(万股)
				str.Format("Ｈ股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HG);
			    break;
		case ID_BG:         //B股(万股)
				str.Format("B股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_BG);
			    break;
		case ID_ZPG:        //转配股(万股)
				str.Format("转配股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZPG);
			    break;

		case ID_ZZC:        //总资产(万元)
				str.Format("总资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZZC);
			    break;
		case ID_LDZC:       //流动资产(万元)
				str.Format("流动资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LDZC);
			    break;
		case ID_CQTZ:       //长期投资(万元)
				str.Format("长期投资(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_CQTZ);
			    break;
		case ID_GDZC:       //固定资产(万元)
				str.Format("固定资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GDZC);
			    break;
		case ID_WXZC:       //无形资产(万元)
				str.Format("无形资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_WXZC);
			    break;
		case ID_LDFZ:       //流动负债(万元)
				str.Format("流动负债(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LDFZ);
			    break;
		case ID_CQFZ:       //长期负债(万元)
				str.Format("长期负债(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_CQFZ);
			    break;
		case ID_GGQY:       //股东权益(万元)
				str.Format("股东权益(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GGQY);
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
				str.Format("资本公积金(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZBGJJ);
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
				str.Format("盈余公积金(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_YYGJJ);
			    break;
		case ID_MGJZ:       //每股净值(元)
				str.Format("每股净值(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGJZ);
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
				str.Format("股东权益比率(%)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GDQYBL);
			    break;
		case ID_MGGJJ:      //每股公积金(元)
				str.Format("每股公积金(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGGJJ);
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
				str.Format("主营业务收入(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZYYWSR);
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
				str.Format("主营业务利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZYYWLR);
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
				str.Format("其它业务利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_QTYWLR);
			    break;
		case ID_LRZE:       //利润总额(万元)
				str.Format("利润总额(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LRZE);
			    break;
		case ID_JLR:        //净利润(万元)
				str.Format("净利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_JLR);
			    break;
		case ID_WFPLR:      //未分配利润(万元)
				str.Format("未分配利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_WFPLR);
			    break;
		case ID_MGSY:       //每股收益(元)
				str.Format("每股收益(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGSY);
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
				str.Format("净资产收益率(%)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_JZCSYL);
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
				str.Format("每股未分配利润(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGWFPLR);
			    break;
		case ID_MGJZC:      //每股净资产(元)
				str.Format("每股净资产(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGJZC);
			    break;
		case ID_ZJLS:        //资金(%d分)
				str.Format("资金(%d分)",pDoc->m_syshqset.capital);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZJLS);
			    break;
		case ID_VOLBIFM:    //量比(%d分)
				str.Format("量比(%d分)",pDoc->m_syshqset.quangtity);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_VOLBIFM);
			    break;
		case ID_HLPFM:      //震幅(%d分)
				str.Format("震幅(%d分)",pDoc->m_syshqset.swing);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HLPFM);
			    break;
		case ID_EXCHANGEFM:  //换手率(%d分)
				str.Format("换手率(%d分)",pDoc->m_syshqset.changehand);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_EXCHANGEFM);
			    break;
		case ID_ADVSP:       //涨速(%d分)
				str.Format("涨速(%d分)",pDoc->m_syshqset.upspeed);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ADVSP);
			    break;
		default:
			{
				if(nID>=9000&&nID<9010)
				{
					TECHNIQUE_COLUMN *pIndexIndex;
					if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						return;
					str.Format("%s(%s)",pIndexIndex->nIndex.sIndex_name,pIndexIndex->nIndex.SubLineType);
					m_nColumnTitleArray.Add(str);
					m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str.GetBuffer(0),nID);
				}
			}
            break;
	}
	}
   if(m_pGrid->GetCurrentRow()>0)
   {
	   int nState = m_pGrid->GetItemState(m_pGrid->GetCurrentRow(), m_Column);

		
	   m_pGrid->SetItemState(m_pGrid->GetCurrentRow(), m_Column, nState | GVIS_SELECTED);
   }

    m_pGrid->Invalidate();                         
   m_nCols=m_pGrid->GetColumnCount();
}
void  CTaiShanReportView::UpdateColumn(UINT m_Column,UINT nID)
{
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    CString str;
	if(nID>=ID_BUYP1 &&nID<=ID_SELV3)
	{
		str = g_sBuySell[nID-ID_BUYP1];
		m_nColumnTitleArray.Add(str);
        m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
			str.GetBuffer(0),nID);
	}
	else
	{
	switch(nID)
	{
		case ID_STOCKID:                                        //代码
			    str.Format("代  码");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0) ,ID_STOCKID);
			    break;
		case ID_STOCKNAME:                                      //名称
				str.Format("证券名称");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_STOCKNAME);
			    break;
		case ID_YSTC:                                //昨收
				str.Format("昨收");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_YSTC);
			    break;
		case ID_OPEN:                                //今开
				str.Format("今开");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_OPEN);
			    break;
		case ID_HIGH:                                //最高
				str.Format("最高");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HIGH);
			    break;
		case ID_LOW:                                 //最低
				str.Format("最低");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LOW);
			    break;
		case ID_NEW:                                 //最新
				str.Format("最新");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_NEW);
			    break;
		case ID_TOTV:                                //总手
				str.Format("总手");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_TOTV);
			    break;
		case ID_NOWV:                                //现手
				str.Format("现手");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_NOWV);
			    break;
		case ID_TOTP:                                //总额
			    str.Format("总额");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_TOTP);
			    break;
		case ID_AVGP:                                //均价
				str.Format("均价");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_AVGP);
			    break;
		case ID_ADDE:                                //涨跌
				str.Format("涨跌");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ADDE);
			    break;
		case ID_HLP:                                 //震幅
				str.Format("震幅");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HLP);
			    break;
		case ID_RDR:                                 //涨幅
				str.Format("涨幅");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_RDR);
			    break;
		case ID_BSSPBAS:                            //委比
				str.Format("委比");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_BSSPBAS);
			    break;
		case ID_DIFBS:                              //委差
				str.Format("委差");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_DIFBS);
			    break;
		case ID_VOLBI:                              //量比
				str.Format("量比");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_VOLBI);
			    break;
		case ID_ACCB:                               //委买
				str.Format("委买");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ACCB);
			    break;
		case ID_ACCS:                               //委卖
				str.Format("委卖");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ACCS);
			    break;
		case ID_BUY:                                //委买价
				str.Format("委买价");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_BUY);
			    break;
		case ID_SEL:                                //委卖价
				str.Format("委卖价");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_SEL);
			    break;
		case ID_RVOL:                               //内盘
				str.Format("内盘");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_RVOL);
			    break;
		case ID_DVOL:                              //外盘
				str.Format("外盘");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_DVOL);
			    break;
        case ID_EXCHANGE:
				str.Format("换手率");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_EXCHANGE);
			    break;
        case ID_SYL:
				str.Format("市盈率");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_SYL);
			    break;
		case ID_ZGB:        //总股本(万股)
				str.Format("总股本(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZGB);
			    break;
		case ID_GJG:        //国家股(万股)
				str.Format("国家股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GJG);
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
				str.Format("发起人法人股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_FQRFRG);
			    break;
		case ID_FRG:        //法人股(万股)
				str.Format("法人股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_FRG);
			    break;
		case ID_ZGG:        //职工股(万股)
				str.Format("职工股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZGG);
			    break;
		case ID_GZAG:       //公众A股(万股)
				str.Format("公众A股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GZAG);
			    break;
		case ID_LTAG:       //流通A股(万股)
				str.Format("流通A股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LTAG);
			    break;
		case ID_HG:         //Ｈ股(万股)
				str.Format("Ｈ股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HG);
			    break;
		case ID_BG:         //B股(万股)
				str.Format("B股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_BG);
			    break;
		case ID_ZPG:        //转配股(万股)
				str.Format("转配股(万股)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZPG);
			    break;

		case ID_ZZC:        //总资产(万元)
				str.Format("总资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZZC);
			    break;
		case ID_LDZC:       //流动资产(万元)
				str.Format("流动资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LDZC);
			    break;
		case ID_CQTZ:       //长期投资(万元)
				str.Format("长期投资(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_CQTZ);
			    break;
		case ID_GDZC:       //固定资产(万元)
				str.Format("固定资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GDZC);
			    break;
		case ID_WXZC:       //无形资产(万元)
				str.Format("无形资产(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_WXZC);
			    break;
		case ID_LDFZ:       //流动负债(万元)
				str.Format("流动负债(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LDFZ);
			    break;
		case ID_CQFZ:       //长期负债(万元)
				str.Format("长期负债(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_CQFZ);
			    break;
		case ID_GGQY:       //股东权益(万元)
				str.Format("股东权益(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GGQY);
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
				str.Format("资本公积金(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZBGJJ);
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
				str.Format("盈余公积金(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_YYGJJ);
			    break;
		case ID_MGJZ:       //每股净值(元)
				str.Format("每股净值(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGJZ);
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
				str.Format("股东权益比率(%)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_GDQYBL);
			    break;
		case ID_MGGJJ:      //每股公积金(元)
				str.Format("每股公积金(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGGJJ);
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
				str.Format("主营业务收入(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZYYWSR);
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
				str.Format("主营业务利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZYYWLR);
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
				str.Format("其它业务利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_QTYWLR);
			    break;
		case ID_LRZE:       //利润总额(万元)
				str.Format("利润总额(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_LRZE);
			    break;
		case ID_JLR:        //净利润(万元)
				str.Format("净利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_JLR);
			    break;
		case ID_WFPLR:      //未分配利润(万元)
				str.Format("未分配利润(万元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_WFPLR);
			    break;
		case ID_MGSY:       //每股收益(元)
				str.Format("每股收益(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGSY);
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
				str.Format("净资产收益率(%)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_JZCSYL);
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
				str.Format("每股未分配利润(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGWFPLR);
			    break;
		case ID_MGJZC:      //每股净资产(元)
				str.Format("每股净资产(元)");
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_MGJZC);
			    break;
		case ID_ZJLS:        //资金(%d分)
				str.Format("资金(%d分)",pDoc->m_syshqset.capital);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ZJLS);
			    break;
		case ID_VOLBIFM:    //量比(%d分)
				str.Format("量比(%d分)",pDoc->m_syshqset.quangtity);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_VOLBIFM);
			    break;
		case ID_HLPFM:      //震幅(%d分)
				str.Format("震幅(%d分)",pDoc->m_syshqset.swing);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_HLPFM);
			    break;
		case ID_EXCHANGEFM:  //换手率(%d分)
				str.Format("换手率(%d分)",pDoc->m_syshqset.changehand);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_EXCHANGEFM);
			    break;
		case ID_ADVSP:       //涨速(%d分)
				str.Format("涨速(%d分)",pDoc->m_syshqset.upspeed);
				m_nColumnTitleArray.Add(str);
                m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str.GetBuffer(0),ID_ADVSP);
			    break;
		default:
			{
				if(nID>=9000&&nID<9010)
				{
					TECHNIQUE_COLUMN *pIndexIndex;
					if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						return;
					str.Format("%s(%s)",pIndexIndex->nIndex.sIndex_name,pIndexIndex->nIndex.SubLineType);
					m_nColumnTitleArray.Add(str);
					m_pGrid->SetColumnInfo(m_Column,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str.GetBuffer(0),nID);
				}
			}
			break;
	 }
	 }
    m_pGrid->Invalidate();                   

                   
   m_nCols=m_pGrid->GetColumnCount();
}



void CTaiShanReportView::UpdateColumnData(CTaiShanDoc* pDoc,int m_Column,int FirstRow,int EndRow)
{
}


int CTaiShanReportView::InitGridCtrl()
{
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    pDoc->m_WsStock2000View =this;
	InitAllTitle();
	if(!InitGridContent())return -1;


    m_pGrid->SetItemState(1,1, m_pGrid->GetItemState(1,1) | GVIS_READONLY);
 	m_pGrid->AutoSize();

	m_pGrid->SetColumnWidth(0,35);

	m_pGrid->SetRowHeight(0, (int)1.2*m_pGrid->GetRowHeight(0));
 	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetRowResize(FALSE);

    m_pGrid->SetFocus();
    m_pGrid->SetFocusRow(1); 
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);

	CCellRange VisibleCells = m_pGrid->GetVisibleNonFixedCellRange();
    m_VisibleRowCount=VisibleCells.GetMaxRow()   - VisibleCells.GetMinRow ()  + 1; 

    SetTimer( ID_GPHQXS, 1 * 1000, NULL );        

	pDoc->m_nPreDisplayRow = GetTotalRowsLmb(pDoc->m_nCurrentPage);

	GetColumnWith();
	return 0;
}


void CTaiShanReportView::AddGridRow(UINT nPage )
{
	int TotRow=0;
	UINT nType;
	CTaiShanDoc* pDoc = GetDocument();

    for(int i=0;i<8;i++)
      if(pDoc->m_sharesInformation.GetStockTypeCount(i)>TotRow)
		  TotRow=pDoc->m_sharesInformation.GetStockTypeCount(i);

	  nType = GetStockTypeFromPageWnd(m_PageWnd->m_nActivePage);
	  if(nType ==-1)
		  return;

	  if(TotRow < m_nRows-1) 
	{
		if(nPage==nType)    
		{
	       m_pGrid->SetRealRow(pDoc->m_sharesInformation.GetStockTypeCount(nPage)+1); // add by xqk 1999/4/21
		}
		return ;
	}
    TotRow=TotRow+10;
 	try { m_pGrid->SetRowCount(TotRow); }
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}
    m_nRows=TotRow;
	if(nPage==nType)    
	{
       m_pGrid->SetRealRow(pDoc->m_sharesInformation.GetStockTypeCount(nType)+1); // add by xqk 1999/4/21
	}
}

void CTaiShanReportView::OnTimer(UINT nIDEvent) 
{

	CTaiShanDoc* pDoc = GetDocument();
	if(nIDEvent == 899)	
	{
		KillTimer(899);
		PostMessage(WM_KEYDOWN, VK_RETURN, 0);
	}
	else if(nIDEvent==ID_GPHQXS) 
	{
	   if(m_TimeCount%3==0)
	      RefreshData();	

	   {

			DWORD dwTick = GetTickCount();
			int nColSel = m_pGrid->GetSortColumn();
			if ( nColSel > 0 )
			{
				IndexDataInfo			nIndexSave;
				TECHNIQUE_COLUMN	*pIndexIndex;
				UINT nID=m_pGrid->GetColumnId(nColSel);		


		   }
		}
		if(m_TimeCount%5==0)
	   {
		   pDoc->OnCalcHqDataProgress();

	      if(pDoc->m_NineShowView!=NULL)
		  {
			 ::SendMessage (pDoc->m_NineShowView->m_hWnd,WM_REFRESHNINESHOW,0,0);	   
		  }
	   }
	   if(m_TimeCount%pDoc->m_SystemInitData.RealTime==0&&m_pGrid->GetSortColumn()>0)
	   {
		   if(m_PageWnd->m_nActivePage<MAINPAGE)
		   	 SortDataForColumn();		
		   else
		   	 SortDataForPage8();
	   }
	   if(m_TimeCount%pDoc->m_systemdata.changepage ==0&&m_LoopDisplay)
	      LoopDisplayOnTimer();
       m_TimeCount++;
	   if(m_TimeCount==100)
		   m_TimeCount=0;
	   
	}	


	CView::OnTimer(nIDEvent);
}
void CTaiShanReportView::SortDataForPage8()
{
    CTaiShanDoc* pDoc = GetDocument();
	int index=0;
	int DisplayCount=0;
	DisplayCount=pDoc->m_StockTypeMap[m_PageWnd->m_nActivePage - MAINPAGE ].GetCount();
    UINT  nID=0; 
    nID=m_pGrid->GetColumnId(m_pGrid->GetSortColumn());
	if(m_pGrid->GetSortColumn()!=-1)
	{
		for(int row=0 ;row <  DisplayCount  ;row++)  
		{    
		     CReportData *Cdat;
			 Cdat = pDoc->m_pStockDataShow[row ].pItem;
			 switch(nID)
			 {
					case ID_STOCKNAME:     					
					     break;
					case ID_YSTC:                            
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->ystc   ;
					     break;
					case ID_OPEN:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->opnp   ;
					     break;
					case ID_HIGH:                               
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->higp   ;
					     break;
					case ID_LOW:                               
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->lowp   ;
					     break;
					case ID_NEW:                               
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->nowp   ;
					     break;
					case ID_TOTV:                               
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->totv  ;
					     break;

					case ID_BUYP1:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->pbuy1 ;break;
					case ID_BUYP2:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->pbuy2 ;break;
					case ID_BUYP3:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->pbuy3 ;break;
					case ID_BUYV1:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vbuy1 ;break;
					case ID_BUYV2:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vbuy2 ;break;
					case ID_BUYV3:                                //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vbuy3 ;break;
					case ID_SELP1:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->psel1 ;break;
					case ID_SELP2:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->psel2 ;break;
					case ID_SELP3:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->psel3 ;break;
					case ID_SELV1:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vsel1 ;break;
					case ID_SELV2:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vsel2 ;break;
					case ID_SELV3:                               //
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vsel3 ;break;

					case ID_NOWV:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->nowv  ;
					     break;
					case ID_TOTP:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->totp  ;
					     break;
					case ID_AVGP:                               
						  if(Cdat->totv!=0)
					         pDoc->m_pStockDataShow[row ].ItemData =Cdat->totp /Cdat->totv  ;
						  else
					         pDoc->m_pStockDataShow[row ].ItemData =0  ;
                          break;						 
					case ID_ADDE:                               
						 if(Cdat->ystc !=0&&Cdat->nowp!=0)
						 {
					         pDoc->m_pStockDataShow[row ].ItemData =Cdat->nowp - Cdat->ystc  ;
						 }
						 else
					         pDoc->m_pStockDataShow[row ].ItemData =0;
                          break;						 
					case ID_HLP:                                 
						  if(Cdat->ystc !=0)
						  {
							 pDoc->m_pStockDataShow[row ].ItemData=(Cdat->higp - Cdat->lowp) /Cdat->ystc ;
						  }
						  else
							 pDoc->m_pStockDataShow[row ].ItemData=0; 
                          break;						 
					case ID_RDR:                                 
						 if(Cdat->ystc !=0&&Cdat->nowp!=0)
						 {
						   pDoc->m_pStockDataShow[row ].ItemData=(Cdat->nowp - Cdat->ystc )/ Cdat->ystc ;
						 }
						 else
						   pDoc->m_pStockDataShow[row ].ItemData=0;
                          break;						 
					case ID_BSSPBAS:            					
						{
						  float temp;
						  if (Cdat->accb+Cdat->accs>0)
							if(Cdat->accs==0&&Cdat->accb!=0)
							   temp=100;
							else if(Cdat->accs!=0&&Cdat->accb==0)
							   temp=-100;
							else
							   temp=(float)((Cdat->accb-Cdat->accs)/(Cdat->accb+Cdat->accs)*100.0);     //计算委比
						   else
					 		   temp=0;

						   pDoc->m_pStockDataShow[row ].ItemData=temp;
						}
                          break;						 
					case ID_DIFBS:                             
						   pDoc->m_pStockDataShow[row ].ItemData=Cdat->accb - Cdat->accs;
                          break;						 
					case ID_VOLBI:                             
						  if(pDoc->m_nANT[0] !=0 && Cdat->volume5!=0 )
						  {
						     pDoc->m_pStockDataShow[row ].ItemData=(Cdat->totv / pDoc->m_nANT[0]) / (Cdat->volume5  );
						  }
						  else
							  pDoc->m_pStockDataShow[row ].ItemData=0;
                          break;						 
					case ID_ACCB:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->accb  ;
					     break;
					case ID_ACCS:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->accs  ;
					     break;
					case ID_BUY:                               
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->pbuy1  ;
					     break;
					case ID_SEL:                                
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->psel1  ;
					     break;
					case ID_RVOL:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->dvol  ;
					     break;
					case ID_DVOL:                            
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->rvol  ;
					     break;
				case ID_EXCHANGE:
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						  if(IsBg( Cdat->kind))
							 if(Cdat->pBaseInfo->Bg!=0) 
								pDoc->m_pStockDataShow[row ].ItemData=Cdat->totv/Cdat->pBaseInfo->Bg;
							 else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
						  else
							  if(Cdat->pBaseInfo->ltAg!=0)
								pDoc->m_pStockDataShow[row ].ItemData=Cdat->totv/Cdat->pBaseInfo->ltAg;
							  else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  }
					  break;
				case ID_SYL:
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 if(Cdat->pBaseInfo->mgsy!=0)
						   pDoc->m_pStockDataShow[row ].ItemData=Cdat->nowp/Cdat->pBaseInfo->mgsy; 
						 else
						   pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  }
					  break;
				case ID_ZGB:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zgb; 
					  }
						break;
				case ID_GJG:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gjg; 
					  }
						break;
				case ID_FQRFRG:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->fqrfrg; 
					  }
						break;
				case ID_FRG:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->frg; 
					  }
						break;
				case ID_ZGG:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zgg; 
					  }
						break;
				case ID_GZAG:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gzAg; 
					  }
						break;
				case ID_LTAG:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ltAg; 
					  }
						break;
				case ID_HG:        
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->Hg; 
					  }
						break;
				case ID_BG:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->Bg; 
					  }
						break;
				case ID_ZPG:        
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zpg; 
					  }
						break;

				case ID_ZZC:      
					  if(Cdat->pBaseInfo==NULL)
						pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zzc; 
					  }
						break;
				case ID_LDZC:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ldzc; 
					  }
						break;
				case ID_CQTZ:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->cqtz; 
					  }
						break;
				case ID_GDZC:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gdzc; 
					  }
						break;
				case ID_WXZC:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->wxzc; 
					  }
						break;
				case ID_LDFZ:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ldfz; 
					  }
						break;
				case ID_CQFZ:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->cqfz; 
					  }
						break;
				case ID_GGQY:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ggqy; 
					  }
						break;
				case ID_ZBGJJ:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zbgjj; 
					  }
						break;
				case ID_YYGJJ:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->yygjj; 
					  }
						break;
				case ID_MGJZ:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgjz; 
					  }
						break;
				case ID_GDQYBL:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gdqybl; 
					  }
						break;
				case ID_MGGJJ:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mggjj; 
					  }
						break;
				case ID_ZYYWSR:  
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zyywsr; 
					  }
						break;
				case ID_ZYYWLR:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zyywlr; 
					  }
						break;
				case ID_QTYWLR:  
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->qtywlr; 
					  }
						break;
				case ID_LRZE:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->lrze; 
					  }
						break;
				case ID_JLR:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->jlr; 
					  }
						break;
				case ID_WFPLR:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->wfplr; 
					  }
						break;
				case ID_MGSY:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgsy; 
					  }
						break;
				case ID_JZCSYL:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->jzcsyl; 
					  }
						break;
				case ID_MGWFPLR:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgwfplr; 
					  }
						break;
				case ID_MGJZC: 
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgjzc; 
					  }
						break;
				case ID_ZJLS:      
					  if(pDoc->m_nANT[0]>pDoc->m_syshqset.capital)
					  {
						int i=pDoc->m_nANT[0];
						while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.capital)
						{
							i--;
						}
						pDoc->m_pStockDataShow[row ].ItemData=Cdat->m_Kdata1[i].Amount - Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.capital ].Amount;
					  }
					  else
					  {
						int i=pDoc->m_nANT[0];
						while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.capital)
						{
							i--;
						}
						pDoc->m_pStockDataShow[row ].ItemData=Cdat->m_Kdata1[i].Amount;
					  }
					  break;
				case ID_VOLBIFM:    
					if(pDoc->m_nANT[0] !=0 && Cdat->volume5!=0 )
					{
						int i=pDoc->m_nANT[0];
						while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.quangtity)
						{
						   i--;
						}
						 if(pDoc->m_nANT[0] >pDoc->m_syshqset.quangtity)
							pDoc->m_pStockDataShow[row ].ItemData=((Cdat->m_Kdata1[i].Volume - Cdat->m_Kdata1[pDoc->m_nANT[0] -pDoc->m_syshqset.quangtity ].Volume) ) / (Cdat->volume5  );
						 else
							pDoc->m_pStockDataShow[row ].ItemData=((Cdat->m_Kdata1[i].Volume ) ) / (Cdat->volume5  );
					}
					else
					    pDoc->m_pStockDataShow[row ].ItemData=0.0f ;
					break;
				case ID_HLPFM:      
					{
						float high=0,low=0,totp=0;
						int minute=pDoc->m_syshqset.swing;
						if(pDoc->m_nANT[0] < minute)
							minute=pDoc->m_nANT[0];
						for(int i=0;i<minute;i++)
						{   
							if(Cdat->m_Kdata1[pDoc->m_nANT[0]-i].Price==0)
								continue;
							if(high==0&&low==0)
							{
								high=low=Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price;
								continue;
							}
							if(Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price > high  )
								high=Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price;
							else if(Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price <low)
								low=Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price;
						}
						if(Cdat->ystc==0)
						  pDoc->m_pStockDataShow[row ].ItemData=0;
						else
						  pDoc->m_pStockDataShow[row ].ItemData=(high-low)/(Cdat->ystc);
					}
					break;
				case ID_EXCHANGEFM:  
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
 						  int i=pDoc->m_nANT[0];
						  while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.changehand)
						  {
				 			  i--;
						  }
						  if(IsBg( Cdat->kind))
							 if(Cdat->pBaseInfo->Bg!=0) 
							 {
								 if(pDoc->m_nANT[0] > pDoc->m_syshqset.changehand)
								   pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume - Cdat->m_Kdata1[pDoc->m_nANT[0]-pDoc->m_syshqset.changehand].Volume)/Cdat->pBaseInfo->Bg;
								 else
								   pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume)/Cdat->pBaseInfo->Bg;
							 }
							 else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
						  else
							  if(Cdat->pBaseInfo->ltAg!=0)
							  {
								 if(pDoc->m_nANT[0] > pDoc->m_syshqset.changehand)
									pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume - Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.changehand].Volume)/Cdat->pBaseInfo->ltAg;
								 else
									pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume)/Cdat->pBaseInfo->ltAg;
							  }
							  else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  }
					  break;
				case ID_ADVSP:     
					{
						float m_temp=0;
					if( pDoc->m_syshqset.upspeed  <pDoc->m_nANT[0])
						if( Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.upspeed].Price!=0)
						{
							float m_nowp=0;
							int i=pDoc->m_nANT[0];
							while( Cdat->m_Kdata1[i].Price==0&&i!=0)
							{
								  i--;
							}
							m_nowp=Cdat->m_Kdata1[i].Price;
							m_temp= 100*(m_nowp - 
								Cdat->m_Kdata1[pDoc->m_nANT[0]  - pDoc->m_syshqset.upspeed].Price)
								/Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.upspeed ].Price;
						}   
						else
							m_temp=0.0f;
					else
						if(Cdat->opnp !=0)
						{
							float m_nowp=0;
							int i=pDoc->m_nANT[0];
							while( Cdat->m_Kdata1[i].Price==0&&i!=0)
							{
								  i--;
							}
							m_nowp=Cdat->m_Kdata1[i].Price;

							m_temp= 100*(m_nowp - Cdat->opnp )/Cdat->opnp ;
						}
						else
							m_temp=0.0f;
					  pDoc->m_pStockDataShow[row ].ItemData=m_temp;
					}
					break;
				default:
					if(nID>=9000&&nID<9010)
					{
						   pDoc->m_pStockDataShow[row ].ItemData=Cdat->Index[nID -9000];
					}

			 }
		}
	    QuickSort(pDoc->m_pStockDataShow,m_pGrid->GetSortAscend(),0,DisplayCount - 1 );
        m_pGrid->Invalidate();
	 }

}
void CTaiShanReportView::SortDataForColumn()
{
    CTaiShanDoc* pDoc = GetDocument();
	int index=0;
	int DisplayCount=0;
	int nType=GetStockTypeFromPageWnd(m_PageWnd->m_nActivePage);
	if(nType<0)
	{
		ASSERT(FALSE);
		nType = 0;
	}


	DisplayCount=GetPageRows();


    UINT  nID=0; 
    nID=m_pGrid->GetColumnId(m_pGrid->GetSortColumn());


	if(m_pGrid->GetSortColumn()!=-1)
	{
		for(int row=0 ;row <  DisplayCount  ;row++)     
		{    
		     CReportData *Cdat;
			 Cdat = pDoc->m_pStockDataShow[row ].pItem;
			 if(Cdat==NULL)
				 continue;
			 switch(nID)
			 {
					case ID_STOCKNAME:     					
					     break;
					case ID_YSTC:                           
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->ystc   ;
					     break;
					case ID_OPEN:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->opnp   ;
					     break;
					case ID_HIGH:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->higp   ;
					     break;
					case ID_LOW:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->lowp   ;
					     break;
					case ID_NEW:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->nowp   ;
					     break;
					case ID_TOTV:                            
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->totv  ;
					     break;
					case ID_BUYP1:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->pbuy1 ;break;
					case ID_BUYP2:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->pbuy2 ;break;
					case ID_BUYP3:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->pbuy3 ;break;
					case ID_BUYV1:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vbuy1 ;break;
					case ID_BUYV2:                             
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vbuy2 ;break;
					case ID_BUYV3:                             
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vbuy3 ;break;
					case ID_SELP1:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->psel1 ;break;
					case ID_SELP2:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->psel2 ;break;
					case ID_SELP3:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->psel3 ;break;
					case ID_SELV1:                             
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vsel1 ;break;
					case ID_SELV2:                              
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vsel2 ;break;
					case ID_SELV3:                            
							pDoc->m_pStockDataShow[row ].ItemData = Cdat->vsel3 ;break;
					case ID_NOWV:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->nowv  ;
					     break;
					case ID_TOTP:                           
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->totp  ;
					     break;
					case ID_AVGP:                              
						  if(Cdat->totv!=0)
					         pDoc->m_pStockDataShow[row ].ItemData =Cdat->totp /Cdat->totv  ;
						  else
					         pDoc->m_pStockDataShow[row ].ItemData =0  ;
                          break;						 
					case ID_ADDE:                               
						 if(Cdat->ystc !=0&&Cdat->nowp!=0)
						 {
					         pDoc->m_pStockDataShow[row ].ItemData =Cdat->nowp - Cdat->ystc  ;
						 }
						 else
					         pDoc->m_pStockDataShow[row ].ItemData =0;
                          break;						 
					case ID_HLP:                                
						  if(Cdat->ystc !=0)
						  {
							 pDoc->m_pStockDataShow[row ].ItemData=(Cdat->higp - Cdat->lowp) /Cdat->ystc ;
						  }
						  else
							 pDoc->m_pStockDataShow[row ].ItemData=0; 
                          break;						 
					case ID_RDR:                              
						 if(Cdat->ystc !=0&&Cdat->nowp!=0)
						 {
						   pDoc->m_pStockDataShow[row ].ItemData=(Cdat->nowp - Cdat->ystc )/ Cdat->ystc ;
						 }
						 else
						   pDoc->m_pStockDataShow[row ].ItemData=0;
                          break;						 
					case ID_BSSPBAS:            					
						{
						  float temp;
						  if (Cdat->accb+Cdat->accs>0)
							if(Cdat->accs==0&&Cdat->accb!=0)
							   temp=100;
							else if(Cdat->accs!=0&&Cdat->accb==0)
							   temp=-100;
							else
							   temp=(float)((Cdat->accb-Cdat->accs)/(Cdat->accb+Cdat->accs)*100.0);     //计算委比
						   else
					 		   temp=0;

						   pDoc->m_pStockDataShow[row ].ItemData=temp;
						}
                          break;						 
					case ID_DIFBS:                            
						   pDoc->m_pStockDataShow[row ].ItemData=Cdat->accb - Cdat->accs;
                          break;						 
					case ID_VOLBI:                             
						  if(pDoc->m_nANT[0] !=0 && Cdat->volume5!=0 )
						  {
						     pDoc->m_pStockDataShow[row ].ItemData=(Cdat->totv / pDoc->m_nANT[0]) / (Cdat->volume5  );
						  }
						  else
							  pDoc->m_pStockDataShow[row ].ItemData=0;
                          break;						 
					case ID_ACCB:                              
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->accb  ;
					     break;
					case ID_ACCS:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->accs  ;
					     break;
					case ID_BUY:                             
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->pbuy1  ;
					     break;
					case ID_SEL:                            
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->psel1  ;
					     break;
					case ID_RVOL:                        
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->dvol  ;
					     break;
					case ID_DVOL:                           
					     pDoc->m_pStockDataShow[row ].ItemData =Cdat->rvol  ;
					     break;
				case ID_EXCHANGE:
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						  if(IsBg( Cdat->kind))
							 if(Cdat->pBaseInfo->Bg!=0) 
								pDoc->m_pStockDataShow[row ].ItemData=Cdat->totv/Cdat->pBaseInfo->Bg;
							 else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
						  else
							  if(Cdat->pBaseInfo->ltAg!=0)
								pDoc->m_pStockDataShow[row ].ItemData=Cdat->totv/Cdat->pBaseInfo->ltAg;
							  else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  }
					  break;
				case ID_SYL:
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 if(Cdat->pBaseInfo->mgsy!=0)
						   pDoc->m_pStockDataShow[row ].ItemData=Cdat->nowp/Cdat->pBaseInfo->mgsy; 
						 else
						   pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  }
					  break;
				case ID_ZGB:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zgb; 
					  }
						break;
				case ID_GJG:        
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gjg; 
					  }
						break;
				case ID_FQRFRG:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->fqrfrg; 
					  }
						break;
				case ID_FRG:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->frg; 
					  }
						break;
				case ID_ZGG:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zgg; 
					  }
						break;
				case ID_GZAG:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gzAg; 
					  }
						break;
				case ID_LTAG:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ltAg; 
					  }
						break;
				case ID_HG:         
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->Hg; 
					  }
						break;
				case ID_BG:        
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->Bg; 
					  }
						break;
				case ID_ZPG:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zpg; 
					  }
						break;

				case ID_ZZC:     
					  if(Cdat->pBaseInfo==NULL)
						pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zzc; 
					  }
						break;
				case ID_LDZC:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ldzc; 
					  }
						break;
				case ID_CQTZ:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->cqtz; 
					  }
						break;
				case ID_GDZC:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gdzc; 
					  }
						break;
				case ID_WXZC:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->wxzc; 
					  }
						break;
				case ID_LDFZ:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ldfz; 
					  }
						break;
				case ID_CQFZ:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->cqfz; 
					  }
						break;
				case ID_GGQY:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->ggqy; 
					  }
						break;
				case ID_ZBGJJ:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zbgjj; 
					  }
						break;
				case ID_YYGJJ:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->yygjj; 
					  }
						break;
				case ID_MGJZ:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgjz; 
					  }
						break;
				case ID_GDQYBL:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->gdqybl; 
					  }
						break;
				case ID_MGGJJ:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mggjj; 
					  }
						break;
				case ID_ZYYWSR:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zyywsr; 
					  }
						break;
				case ID_ZYYWLR:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->zyywlr; 
					  }
						break;
				case ID_QTYWLR:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->qtywlr; 
					  }
						break;
				case ID_LRZE:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->lrze; 
					  }
						break;
				case ID_JLR:       
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->jlr; 
					  }
						break;
				case ID_WFPLR:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->wfplr; 
					  }
						break;
				case ID_MGSY:      
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgsy; 
					  }
						break;
				case ID_JZCSYL:    
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->jzcsyl; 
					  }
						break;
				case ID_MGWFPLR:   
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgwfplr; 
					  }
						break;
				case ID_MGJZC:     
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
						 pDoc->m_pStockDataShow[row ].ItemData=Cdat->pBaseInfo->mgjzc; 
					  }
						break;
				case ID_ZJLS:      
					  if(pDoc->m_nANT[0]>pDoc->m_syshqset.capital)
					  {
						int i=pDoc->m_nANT[0];
						while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.capital)
						{
							i--;
						}
						pDoc->m_pStockDataShow[row ].ItemData=Cdat->m_Kdata1[i].Amount - Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.capital ].Amount;
					  }
					  else
					  {
						int i=pDoc->m_nANT[0];
						while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.capital)
						{
							i--;
						}
						pDoc->m_pStockDataShow[row ].ItemData=Cdat->m_Kdata1[i].Amount;
					  }
					  break;
				case ID_VOLBIFM:   
					if(pDoc->m_nANT[0] !=0 && Cdat->volume5!=0 )
					{
						int i=pDoc->m_nANT[0];
						while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.quangtity)
						{
						   i--;
						}
						 if(pDoc->m_nANT[0] > pDoc->m_syshqset.quangtity)
							pDoc->m_pStockDataShow[row ].ItemData=((Cdat->m_Kdata1[i].Volume - Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.quangtity ].Volume) ) / (Cdat->volume5  );
						 else
							pDoc->m_pStockDataShow[row ].ItemData=((Cdat->m_Kdata1[i].Volume ) ) / (Cdat->volume5  );
					}
					else
					pDoc->m_pStockDataShow[row ].ItemData=0.0f ;

					break;
				case ID_HLPFM:      
					{
						float high=0,low=0,totp=0;
						int minute=pDoc->m_syshqset.swing;
						if(pDoc->m_nANT[0] < minute)
							minute=pDoc->m_nANT[0];
						for(int i=0;i<minute;i++)
						{   
							if(Cdat->m_Kdata1[pDoc->m_nANT[0]-i].Price==0)
								continue;
							if(high==0&&low==0)
							{
								high=low=Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price;
								continue;
							}
							if(Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price > high  )
								high=Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price;
							else if(Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price <low)
								low=Cdat->m_Kdata1[pDoc->m_nANT[0] -i].Price;
						}
						if(Cdat->ystc==0)
						  pDoc->m_pStockDataShow[row ].ItemData=0;
						else
						  pDoc->m_pStockDataShow[row ].ItemData=(high-low)/(Cdat->ystc);
					}
					break;
				case ID_EXCHANGEFM:  
					  if(Cdat->pBaseInfo==NULL)
						 pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  else
					  {
 						  int i=pDoc->m_nANT[0];
						  while( Cdat->m_Kdata1[i].Price==0&&i!=0&&pDoc->m_nANT[0]-i<pDoc->m_syshqset.changehand)
						  {
				 			  i--;
						  }
						  if(IsBg( Cdat->kind))
							 if(Cdat->pBaseInfo->Bg!=0) 
							 {
								 if(pDoc->m_nANT[0] > pDoc->m_syshqset.changehand)
								   pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume - Cdat->m_Kdata1[pDoc->m_nANT[0]-pDoc->m_syshqset.changehand].Volume)/Cdat->pBaseInfo->Bg;
								 else
								   pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume)/Cdat->pBaseInfo->Bg;
							 }
							 else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
						  else
							  if(Cdat->pBaseInfo->ltAg!=0)
							  {
								 if(pDoc->m_nANT[0] >pDoc->m_syshqset.changehand)
									pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume - Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.changehand].Volume)/Cdat->pBaseInfo->ltAg;
								 else
									pDoc->m_pStockDataShow[row ].ItemData=(Cdat->m_Kdata1[i].Volume)/Cdat->pBaseInfo->ltAg;
							  }
							  else
								pDoc->m_pStockDataShow[row ].ItemData=0.0f;
					  }
					  break;
				case ID_ADVSP:    
					{
						float m_temp=0;
					if( pDoc->m_syshqset.upspeed  <pDoc->m_nANT[0])
						if( Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.upspeed ].Price!=0)
						{
							float m_nowp=0;
							int i=pDoc->m_nANT[0];
							while( Cdat->m_Kdata1[i].Price==0&&i!=0)
							{
								  i--;
							}
							m_nowp=Cdat->m_Kdata1[i].Price;
							m_temp= 100*(m_nowp - 
							Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.upspeed ].Price)/Cdat->m_Kdata1[pDoc->m_nANT[0] - pDoc->m_syshqset.upspeed ].Price;
						}   
						else
							m_temp=0.0f;
					else
						if(Cdat->opnp !=0)
						{
							float m_nowp=0;
							int i=pDoc->m_nANT[0];
							while( Cdat->m_Kdata1[i].Price==0&&i!=0)
							{
								  i--;
							}
							m_nowp=Cdat->m_Kdata1[i].Price;

							m_temp= 100*(m_nowp - Cdat->opnp )/Cdat->opnp ;
						}
						else
							m_temp=0.0f;
					  pDoc->m_pStockDataShow[row ].ItemData=m_temp;
					}
					break;
				default:
					if(nID>=9000&&nID<9010)
					{
						   pDoc->m_pStockDataShow[row ].ItemData=Cdat->Index[nID -9000] ;
					}
                     
			 }
		}
	    QuickSort(pDoc->m_pStockDataShow,m_pGrid->GetSortAscend(),0,DisplayCount - 1 );
        m_pGrid->Invalidate();
	 }
}


void CTaiShanReportView::OnDestroy() 
{
	if(this->m_pHistoryDlg ) m_pHistoryDlg->DestroyWindow();

	g_bDestroyedView = TRUE;
	CView::OnDestroy();

   	SetColumnWidth();
#ifdef WIDE_NET_VERSION
	CMainFrame * pMainFrm=((CMainFrame *)(AfxGetApp()->m_pMainWnd));
	if(pMainFrm->m_pClientTransmitManageWnd)
	{
		if(m_SocketID.iFlag!=-1)
		{
             pMainFrm->m_pClientTransmitManageWnd->ReleaseID(this->m_SocketID);
             m_SocketID.iFlag=-1;
		}
	}
#endif	
}



void CTaiShanReportView::PopMenuItemInit(CMenu *pMenuContext,UINT nRow)
{
	CString zqdm;
	zqdm=m_pGrid->GetItemText(nRow, 1);
	CTaiShanDoc* pDoc = GetDocument();

	if(zqdm.GetLength ()<4)
	{
		 pMenuContext->EnableMenuItem( ID_OPEN_STOCK,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		 pMenuContext->EnableMenuItem( ID_OPEN_DOCUMENT,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
	}
	else
	{
		 pMenuContext->EnableMenuItem( ID_OPEN_STOCK,MF_BYCOMMAND|MF_ENABLED);
		 pMenuContext->EnableMenuItem( ID_OPEN_DOCUMENT,MF_BYCOMMAND|MF_ENABLED );
	}
    
	CString stock_code;

	stock_code = m_pGrid->GetItemText(m_CellTitleCommand.row,1);
	switch(m_PageWnd->m_nActivePage)
	{
	case SHPAGE: 
	case SZPAGE:
    case SZEBPAGE:
			 if(stock_code.Compare("") == NULL)
			 {
				 pMenuContext->EnableMenuItem( ID_ADD_CHOOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			     pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			     pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			 }
			 else
			 {
				 pMenuContext->EnableMenuItem( ID_ADD_CHOOSE,MF_BYCOMMAND|MF_ENABLED);
                 switch(m_PageWnd->m_nActivePage)
				 {
					case SHPAGE:    if(pDoc->m_nSharesSh==0)
									{
			                           pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
				                       pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
									}
						            else
									{
				                       pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_ENABLED );
				                       pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_ENABLED);
									}
						         break;
					case SZPAGE:  if(pDoc->m_nSharesSz==0)
									{
			                           pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
				                       pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
									}
						            else
									{
				                       pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_ENABLED );
				                       pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_ENABLED);
									}
						         break;
					case SZEBPAGE:  if(pDoc->m_nSharesSzEb==0)
									{
			                           pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
				                       pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
									}
						            else
									{
				                       pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_ENABLED );
				                       pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_ENABLED);
									}
						         break;
				 }
			 }
		     pMenuContext->EnableMenuItem( ID_DELETE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		     pMenuContext->EnableMenuItem( ID_SAVE_TJXG,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		     pMenuContext->EnableMenuItem( ID_SAVE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
             break;
	case STKINDEXPAGE:
		     if(stock_code.Compare("") == NULL)
			    pMenuContext->EnableMenuItem( ID_ADD_CHOOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			 else
			    pMenuContext->EnableMenuItem( ID_ADD_CHOOSE,MF_BYCOMMAND|MF_ENABLED);
			 pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			 pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		     pMenuContext->EnableMenuItem( ID_DELETE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		     pMenuContext->EnableMenuItem( ID_SAVE_TJXG,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		     pMenuContext->EnableMenuItem( ID_SAVE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
             break;
	case CHOOSEPAGE:
	case STKTYPEPAGE:
		  	 if(stock_code.Compare("") == NULL)
			 {
			      pMenuContext->EnableMenuItem( ID_DELETE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
			     pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			 }
			 else
			 {
			      pMenuContext->EnableMenuItem( ID_DELETE_STOCKTYPE,MF_BYCOMMAND|MF_ENABLED );
				 pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_ENABLED);
			 }     
		     pMenuContext->EnableMenuItem( ID_ADD_CHOOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		     pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_ENABLED);
		    
		     pMenuContext->EnableMenuItem( ID_SAVE_TJXG,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
		     pMenuContext->EnableMenuItem( ID_SAVE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
             break;
	case TJXGPAGE:
		     if(stock_code.Compare("") == NULL)
			 {
			     pMenuContext->EnableMenuItem( ID_DELETE_STOCKTYPE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED );
			     pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
			 }
			 else
			 {
			      pMenuContext->EnableMenuItem( ID_DELETE_STOCKTYPE,MF_BYCOMMAND|MF_ENABLED );
				 pMenuContext->EnableMenuItem( ID_COMPANYINFO,MF_BYCOMMAND|MF_ENABLED);
			 } 
		     pMenuContext->EnableMenuItem( ID_ADD_CHOOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);
		     pMenuContext->EnableMenuItem( 5,MF_BYPOSITION|MF_ENABLED );
		     pMenuContext->EnableMenuItem( ID_SAVE_TJXG,MF_BYCOMMAND|MF_ENABLED );
		     pMenuContext->EnableMenuItem( ID_SAVE_STOCKTYPE,MF_BYCOMMAND|MF_ENABLED );
		    
             break;

	}
	pMenuContext->CheckMenuItem( ID_STOCK_TYPE1,MF_BYCOMMAND|MF_UNCHECKED);
	pMenuContext->CheckMenuItem( ID_STOCK_TYPE2,MF_BYCOMMAND|MF_UNCHECKED);
	pMenuContext->CheckMenuItem( ID_STOCK_TYPE3,MF_BYCOMMAND|MF_UNCHECKED);
	pMenuContext->CheckMenuItem( ID_STOCK_TYPE4,MF_BYCOMMAND|MF_UNCHECKED);
	switch(pDoc->m_SystemInitData.ColumnType)
	{
	   case 0:
					 pMenuContext->CheckMenuItem( ID_STOCK_TYPE1, MF_BYCOMMAND|MF_CHECKED);
					 break;
	   case 1:
					 pMenuContext->CheckMenuItem( ID_STOCK_TYPE2, MF_BYCOMMAND|MF_CHECKED);
					 break;
	   case 2:
					 pMenuContext->CheckMenuItem( ID_STOCK_TYPE3, MF_BYCOMMAND|MF_CHECKED);
					 break;
	   case 3:
					 pMenuContext->CheckMenuItem( ID_STOCK_TYPE4, MF_BYCOMMAND|MF_CHECKED);
					 break;
	}
}
void CTaiShanReportView::AddChoose(char *StockCode,int nKind) 
{
	CTaiShanDoc* pDoc = GetDocument();
	CString m_zqdm=StockCode;
	CString m_olddm;
    CString m_zqdmkind=pDoc->GetStockKindString(nKind);
	m_zqdmkind+=m_zqdm;
	if(pDoc->m_StockTypeMap[0].Lookup(m_zqdmkind,m_olddm)==TRUE)
		return;
	CReportData *Cdat;
	if(pDoc->m_sharesInformation.Lookup(StockCode,Cdat,nKind)==TRUE)
	{
		pDoc->m_ManagerStockTypeData.InsertStockToChoose(Cdat->id,Cdat->kind);
		pDoc->m_StockTypeMap[0][m_zqdmkind]=m_zqdm;
	}
}
void CTaiShanReportView::OnAddChoose() 
{

    CString m_zqdm;
	CTaiShanDoc* pDoc = GetDocument();
	CReportData *pCdat;
	if(!GetCdatPointForRow(m_CellTitleCommand.row,pCdat))
		return;
    AddChoose(pCdat->id,pCdat->kind) ;
}

void CTaiShanReportView::OnAddStocktype(UINT nID) 
{

 	CStringArray m_StockTypeNameArray;
 	SymbolKindArr m_StockCodeArray;
	CString m_zqdm;
	STOCKTYPEINFO *m_pStktype;
 	CTaiShanDoc* pDoc = GetDocument();
	CReportData *pCdat;
	if(this->GetCdatPointForRow(m_CellTitleCommand.row,pCdat)==FALSE)
		return;
	m_zqdm=pCdat->id;
	pDoc->m_ManagerStockTypeData.GetAllStockTypeName(m_StockTypeNameArray);
	CString strStockTypeName=m_StockTypeNameArray.GetAt(nID);
    pDoc->m_ManagerStockTypeData.GetStockTypePointFromName(m_pStktype,strStockTypeName.GetBuffer(0)); //从板块名称返回板块结构指针
	pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(m_StockCodeArray,strStockTypeName.GetBuffer(0));          
	int StockCount=m_StockCodeArray.GetSize();
	for(int i=0;i<StockCount;i++)
	{
		SymbolKind m_SymbolKind=m_StockCodeArray.GetAt(i);
        CString strStockName=m_SymbolKind.m_chSymbol;
		if(strStockName==m_zqdm)
		{
			return;
		}
	}
	
    if(pCdat->pBaseInfo!=NULL&&pCdat->pBaseInfo->zgb!=0)
    {
		if(!pDoc->m_ManagerStockTypeData.InsertStockToType(pCdat,m_pStktype->m_szCode))
			return;
    }
    else
    {
		 MessageBox("在将股票加入板块时,该股票财务数据不能为空!","窗口操作提示",MB_OK);
		 return;
    }
    if(m_pStktype->m_szName==pDoc->m_SystemInitData.StockTypeName)
	{
		CString m_zqdmkind=pDoc->GetStockKindString(pCdat->kind);
		m_zqdmkind+=m_zqdm;
        pDoc->m_StockTypeMap[1][m_zqdmkind]=m_zqdm;
	}
 	CProgressDialog *dlg;
	dlg=new CProgressDialog;
	dlg->Create(IDD_DIALOG_PROGRESS,NULL);
    dlg->ShowWindow(SW_SHOW); 
	dlg->m_DialogDown =FALSE;
	CWnd *pWnd;
	pWnd=dlg->GetDlgItem(IDC_ZQMC) ;
	if(pWnd)
		pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_ZQDM) ;
	if(pWnd)
		pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_DISPLAY_INFO) ;
	pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_COMP) ;
	pWnd->ShowWindow(SW_HIDE);
	CButton *m_pButton;
	m_pButton =(CButton *)dlg->GetDlgItem(IDOK); 
    m_pButton->EnableWindow(FALSE) ;
	dlg->SetWindowText(_T("历史日线数据刷新"));
    CProgressCtrl *pProgressCtrl;
	pProgressCtrl =(CProgressCtrl *)dlg->GetDlgItem(IDC_PROGRESS); 
    pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData(m_pStktype->m_szCode,pProgressCtrl);
    m_pButton->EnableWindow(TRUE) ;
	dlg->m_DialogDown =TRUE;
	dlg->ShowWindow(SW_HIDE);
	dlg->OnClose() ;
}

void CTaiShanReportView::OnDeleteStocktype() 
{
	
	CString m_zqdm;
	CString m_zqdmkind;
 	CTaiShanDoc* pDoc = GetDocument();
	CReportData *pCdat=NULL;
	if(!this->GetCdatPointForRow(m_CellTitleCommand.row,pCdat))
		return;
	m_zqdm=pCdat->id;
	m_zqdmkind=pDoc->GetStockKindString(pCdat->kind);
	m_zqdmkind+=m_zqdm;
	int ls_Count=0;
    BLOCKSTOCK * blocknow;
	BOOL m_finddone=FALSE;
	POSITION pos,position;
	FILE *fp;
    char choosehead[]={65,79,88,14};
	int length=0;
	char  blank[2]={' ',' '};         
    switch(m_PageWnd->m_nActivePage)
	{
	case CHOOSEPAGE:
                pDoc->m_ManagerStockTypeData.DeleteStockFromChoose(pCdat->id,pCdat->kind);
				length=pDoc->m_ManagerStockTypeData.GetChooseStockCounts();
 	            m_pGrid->SetRealRow(length+1);
				pDoc->m_StockTypeMap[0].RemoveKey(m_zqdmkind);
		        break;
	case STKTYPEPAGE:                                             
		{
			    STOCKTYPEINFO *m_pStktype;
		        pDoc->m_ManagerStockTypeData.GetStockTypePointFromName(m_pStktype,pDoc->m_SystemInitData.StockTypeName); //从板块名称返回板块结构指针
                length=pDoc->m_ManagerStockTypeData.GetStockCountFromType(m_pStktype->m_szCode);
	            pDoc->m_ManagerStockTypeData.DeleteStockFromType(m_zqdm.GetBuffer(0),pCdat->kind,m_pStktype->m_szCode);      //删除股票从某一板块
                int length1=pDoc->m_ManagerStockTypeData.GetStockCountFromType(m_pStktype->m_szCode);
				pDoc->m_StockTypeMap[1].RemoveKey(m_zqdmkind);

				if(length!=length1)
				{
 	                m_pGrid->SetRealRow(length1+1);
 					CProgressDialog *dlg;
					dlg=new CProgressDialog;
					dlg->Create(IDD_DIALOG_PROGRESS,NULL);
					dlg->ShowWindow(SW_SHOW); 
					dlg->m_DialogDown =FALSE;
					CWnd *pWnd;
					pWnd=dlg->GetDlgItem(IDC_ZQMC) ;
					pWnd->ShowWindow(SW_HIDE);
					pWnd=dlg->GetDlgItem(IDC_ZQDM) ;
					pWnd->ShowWindow(SW_HIDE);
					pWnd=dlg->GetDlgItem(IDC_DISPLAY_INFO) ;
					pWnd->ShowWindow(SW_HIDE);
					pWnd=dlg->GetDlgItem(IDC_COMP) ;
					pWnd->ShowWindow(SW_HIDE);
					CButton *m_pButton;
					m_pButton =(CButton *)dlg->GetDlgItem(IDOK); 
					m_pButton->EnableWindow(FALSE) ;
					dlg->SetWindowText(_T("历史日线数据刷新"));
					CProgressCtrl *pProgressCtrl;
					pProgressCtrl =(CProgressCtrl *)dlg->GetDlgItem(IDC_PROGRESS); 
					pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData(m_pStktype->m_szCode,pProgressCtrl);
					m_pButton->EnableWindow(TRUE) ;
					dlg->m_DialogDown =TRUE;
					dlg->ShowWindow(SW_HIDE);
					dlg->OnClose() ;

				}
		}       
		        break;
	case TJXGPAGE:   if(strcmp(pDoc->m_SystemInitData.ScreenStockName,"条件选股")==0)
			   {
				   ls_Count=pDoc->m_ScreenList.GetSize();
				   for(int i=0;i<ls_Count;i++)
				   {
					   CString zqdm;
                       SharesSymbol m_StockSymbol;
                       m_StockSymbol=pDoc->m_ScreenList.GetAt(i);
					   zqdm=m_StockSymbol.sharesymbol;
					   if(zqdm== m_zqdm )
					   {
							 pDoc->m_ScreenList.RemoveAt(i);
							 pDoc->m_StockTypeMap[2].RemoveKey(m_zqdmkind );     
                             length=pDoc->m_StockTypeMap[2].GetCount();
						     m_pGrid->SetRealRow(length+1); 
					   }
				   }
			   }
		       else
			   {
				   pos=pDoc->m_Tjxgblocklist.GetHeadPosition();   
				   while(pos)
				   {
						blocknow=pDoc->m_Tjxgblocklist.GetNext(pos);  
						if(blocknow->blockname== pDoc->m_SystemInitData.ScreenStockName )  
						{
							 m_finddone=TRUE;
							 ls_Count=blocknow->stocklist.GetCount(); 

							 pos=blocknow->stocklist.GetHeadPosition();
							 while(pos)                         
							 {
								SharesSymbol *symbol;
								position=pos;
								symbol=blocknow->stocklist.GetNext(pos);
								if(strcmp(symbol->sharesymbol, m_zqdm.GetBuffer(0))==0 )
								{
									 blocknow->stocklist.RemoveAt( position );  
									 pDoc->m_StockTypeMap[2].RemoveKey(m_zqdmkind );      

									 CString FileName;
									 FileName.Format("板块数据//%s",pDoc->m_SystemInitData.ScreenStockName); 
									 fp=_fsopen(FileName.GetBuffer(0),"w+b",SH_DENYNO);
									 if(fp==NULL)
										 return;
									 length=blocknow->stocklist.GetCount();
									 fwrite(&length,1,4,fp);

									 pos= blocknow->stocklist.GetHeadPosition();  
									 while(pos)
									 {
										 symbol= blocknow->stocklist.GetNext(pos);
										 fwrite(symbol->sharesymbol,1,6,fp);
										 fwrite(&blank,1,2,fp);
									 }
									 fclose(fp);
									 m_pGrid->SetRealRow(length);
								}
							}
							break;
						}
					}
					if(!m_finddone)                                     
					{
						return;
					}
			   }
		       break;
	}
    memcpy(&pDoc->m_pStockDataShow[m_CellTitleCommand.row -1],&pDoc->m_pStockDataShow[m_CellTitleCommand.row],
		(pDoc->m_nShowMaxCount - m_CellTitleCommand.row)*sizeof(STOCKDATASHOW));
    m_pGrid->Invalidate();
}
void CTaiShanReportView::OnOpenDocument() 
{

	
}

void CTaiShanReportView::OnFontChange() 
{

	LOGFONT lf;
	m_pGrid->GetFont()->GetLogFont(&lf);

	CFontDialog dlg(&lf);
	if (dlg.DoModal() == IDOK) 
	{
		dlg.GetCurrentFont(&lf);

		CFont Font;
		Font.CreateFontIndirect(&lf);
		m_pGrid->SetFont(&Font);
 		m_pGrid->AutoSize();
		Font.DeleteObject();
	}	
}



long CTaiShanReportView::FindStockInNameSymbor(UINT nPage,char *zqdm)
{
    CTaiShanDoc* pDoc = GetDocument();
	int low=0;
	int high=pDoc->m_sharesInformation.GetStockTypeCount(nPage) ;
	int mid=0;
	CReportData *Cdat;
    while(low <= high)
	{
		 mid=(low+high)/2;
		 pDoc->m_sharesInformation.GetStockItem(nPage,mid,Cdat);
		 if(strcmp(Cdat->id , zqdm)>0) high=mid -1;
         else if(strcmp(Cdat->id , zqdm)< 0 ) low=mid +1;
		 else 
		 {
			 return mid ;
		 }
	}
	return -1;
}
void CTaiShanReportView::ScrollToSelectStock(char *zqdm,int nkind)
{
   CTaiShanDoc* pDoc = GetDocument();
   CString m_zqdm=zqdm;
   CString m_findzqdm=zqdm;
   CString m_zqdmkind=pDoc->GetStockKindString(nkind);
   m_zqdmkind+=m_zqdm;
   int StockType=GetStockTypeFromPageWnd(m_PageWnd->m_nActivePage);
   if(StockType<0)
	   StockType = 0;

   if(m_PageWnd->m_nActivePage>=MAINPAGE&&pDoc->m_StockTypeMap[m_PageWnd->m_nActivePage - MAINPAGE ].Lookup(m_zqdmkind,m_findzqdm)) //本页查找股票代码
   {
	   int temp=pDoc->m_StockTypeMap[m_PageWnd->m_nActivePage - MAINPAGE ].GetCount();
	   for(int i=1 ;i<=temp;i++)
	   {
            if(m_findzqdm==m_pGrid->GetItemText(i, 1))
			{
               m_pGrid->GotoLine(i,TRUE);
               m_pGrid->Invalidate();                        
	           pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
			}
	   }
   }
   else if(nkind==StockType&&m_PageWnd->m_nActivePage<CHOOSEPAGE)
   {
	   int temp=pDoc->m_sharesInformation.GetStockTypeCount(StockType);
	   for(int i=1 ;i<=temp;i++)
	   {
            if(m_findzqdm==m_pGrid->GetItemText(i, 1))
			{
               m_pGrid->GotoLine(i,TRUE);
               m_pGrid->Invalidate();                          
	           pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
			}
	   }
   }
   else
   {
	   switch(nkind)
	   {
		   case 0:
					 m_PageWnd->m_nActivePage=SHPAGE;
					 pDoc->m_nSharesSh=0;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,0,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 1:
					 m_PageWnd->m_nActivePage=SHPAGE;
					 pDoc->m_nSharesSh=1;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,1,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 2:
					 m_PageWnd->m_nActivePage=SHPAGE;
					 pDoc->m_nSharesSh=2;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,2,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 3:
					 m_PageWnd->m_nActivePage=SZPAGE;
					 pDoc->m_nSharesSz=0;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,3,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 4:
					 m_PageWnd->m_nActivePage=SZPAGE;
					 pDoc->m_nSharesSz=1;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,4,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 5:
					 m_PageWnd->m_nActivePage=SZPAGE;
					 pDoc->m_nSharesSz=2;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,5,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 6:
					 m_PageWnd->m_nActivePage=SHPAGE;
					 pDoc->m_nSharesSh=3;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,6,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 7:
					 m_PageWnd->m_nActivePage=SZPAGE;
					 pDoc->m_nSharesSz=3;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,7,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 8:
					 m_PageWnd->m_nActivePage=SZEBPAGE;
					 pDoc->m_nSharesSz=0;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,8,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 9:
					 m_PageWnd->m_nActivePage=SZEBPAGE;
					 pDoc->m_nSharesSz=1;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,9,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
		   case 10:
					 m_PageWnd->m_nActivePage=STKINDEXPAGE;
					 m_PageWnd->m_bRefrash=TRUE;
					 m_PageWnd->Invalidate();
					 ScrollToPageOfStock(pDoc,10,m_findzqdm);
					 pDoc->m_nCurrentPage=m_PageWnd->m_nActivePage;
					 break;
	   }
   }
   m_pGrid->SetMouseState(MOUSE_NOTHINGS);
}
void CTaiShanReportView::ScrollToPageOfStock(CTaiShanDoc* pDoc,int nPage,CString m_findzqdm)
{
	UINT m_pos;
    m_pos=FindStockInNameSymbor(nPage, m_findzqdm.GetBuffer(0));
	int index=0;
	UINT DisplayCount=0;
    m_pGrid->SetSortColumn(-1 ,TRUE);



	DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nPage);
    SetShowData(pDoc,pDoc->m_sharesInformation.GetStockTypeCount(nPage));


	m_pGrid->SetRealRow(DisplayCount+1);

   	pDoc->m_sharesInformation.RecvStockDataForType(pDoc->m_pStockDataShow,nPage);

    CCellRange VisibleCells = m_pGrid->GetVisibleNonFixedCellRange();
        
	m_VisibleRowCount=VisibleCells.GetMaxRow() - VisibleCells.GetMinRow ()  + 1; 

    UINT CurrCount,mode,indexT; 

    if( DisplayCount <= m_VisibleRowCount )
	{
		mode=1;
        CurrCount = DisplayCount;
		indexT=1;
        m_pGrid->GotoLine(indexT,FALSE );
	}
	else
	{
        CurrCount= m_VisibleRowCount;
		mode=0;
		if(m_pos - index < m_VisibleRowCount  - 1)
		{
		   indexT=1;
           m_pGrid->GotoLine(indexT ,FALSE);

		}
		else if( DisplayCount - ( m_pos - index ) > m_VisibleRowCount )
		{
			indexT=m_pos - index;
            m_pGrid->GotoLine(indexT + 1,FALSE);
		}
		else
		{
			indexT=DisplayCount - m_VisibleRowCount + 1;
            m_pGrid->GotoLine(indexT + 1,FALSE);
		}
	}
    m_pGrid->SetFocusRow( m_pos - index + 1);
    m_pGrid->Invalidate();                       
    pDoc->m_nPreDisplayRow=DisplayCount;       

}
LRESULT CTaiShanReportView::HotKeyProgress(WPARAM wParam, LPARAM lParam)
{
	 char HotKey[10];
	 _itoa( (int)lParam, HotKey, 10 );
     
	 HotKeyDownProgress(HotKey);

	 return 0;
}

void CTaiShanReportView::HotKeyDownProgress(char  *HotKey)
{
   CTaiShanDoc* pDoc = GetDocument();
   int m_key;
   int nStockType=0;
   int DisplayCount;
   CCellID cell;
   m_key=atoi(HotKey);
   int currcol=0;
   switch(m_key)
   {
	  
	   case 61:  
	   case 62:  
	   case 63:  
	   case 64:  
	   case 65:  
	   case 66:  
       case 69: 
		          nStockType=m_key -60;
		          if(nStockType > 2&&nStockType!=9 )
					  nStockType++;
		          
		          DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
                  pDoc->m_nPreDisplayRow=DisplayCount;       
                  SortStockIndex(nStockType,ID_RDR);

				  SetTypeToDocumentLmb( nStockType);

                  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();

                  m_pGrid->SetScrollPos32(SB_VERT, 0, FALSE);
				  m_pGrid->SetSortColumn(GetSortColumn(ID_RDR) ,FALSE);


                  m_pGrid->SetHotKeySelect(GetSortColumn(ID_RDR));
	              m_pGrid->SetRealRow(DisplayCount+1); 
                  
				  currcol=GetSortColumn(ID_RDR);
				  m_pGrid->EnsureVisible(0, currcol);

				  m_pGrid->Invalidate();

				  break;
	   case 11:  
	   case 12:  
	   case 13:  
	   case 14:  
	   case 15:  
	   case 16:  
		          nStockType=m_key -10;
		          if(nStockType > 2 )
					  nStockType++;
		          
		          DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
                  pDoc->m_nPreDisplayRow=DisplayCount;       
                  SortStockIndex(nStockType,ID_TOTV);

				  SetTypeToDocumentLmb(nStockType);
                  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();
				  m_pGrid->SetSortColumn(GetSortColumn(ID_TOTV) ,FALSE);


                  m_pGrid->SetHotKeySelect(GetSortColumn(ID_TOTV));

	              m_pGrid->SetRealRow(DisplayCount+1); 
                  m_pGrid->SetScrollPos32(SB_VERT, 0, FALSE);
				  currcol=GetSortColumn(ID_TOTV);
				  m_pGrid->EnsureVisible(0, currcol);
				  m_pGrid->Invalidate();
				  break;
	   case 21:  
	   case 22:  
	   case 23:  
	   case 24:  
	   case 25:  
	   case 26:  
		          nStockType=m_key -20;
		          if(nStockType > 2 )
					  nStockType++;
		          
		          DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
                  pDoc->m_nPreDisplayRow=DisplayCount;       
                  SortStockIndex(nStockType,ID_TOTP);

				  SetTypeToDocumentLmb( nStockType);

				  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();
				  m_pGrid->SetSortColumn(GetSortColumn(ID_TOTP) ,FALSE);


                  m_pGrid->SetHotKeySelect(GetSortColumn(ID_TOTP));

				  m_pGrid->SetRealRow(DisplayCount+1); 
                  m_pGrid->SetScrollPos32(SB_VERT, 0, FALSE);
				  currcol=GetSortColumn(ID_TOTP);
				  m_pGrid->EnsureVisible(0, currcol);
				  m_pGrid->Invalidate();
				  break;
	   case 31:  
	   case 32:  
	   case 33:  
	   case 34:  
	   case 35:  
	   case 36:  
		          nStockType=m_key -30;
		          if(nStockType > 2 )
					  nStockType++;
		          
		          DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
                  pDoc->m_nPreDisplayRow=DisplayCount;       
                  SortStockIndex(nStockType,ID_VOLBI);

  				  SetTypeToDocumentLmb( nStockType);

                  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();
				  m_pGrid->SetSortColumn(GetSortColumn(ID_VOLBI) ,FALSE);
      

                  m_pGrid->SetHotKeySelect(GetSortColumn(ID_VOLBI));

				  currcol=GetSortColumn(ID_VOLBI);
	              m_pGrid->SetRealRow(DisplayCount+1);
                  m_pGrid->SetScrollPos32(SB_VERT, 0, FALSE);
				  m_pGrid->EnsureVisible(0, currcol);
				  m_pGrid->Invalidate();

				  break;
	   case 41:  
	   case 42:  
	   case 43:  
	   case 44:  
	   case 45:  
	   case 46:  
		          nStockType=m_key -40;
		          if(nStockType > 2 )
					  nStockType++;
		          
		          DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
                  pDoc->m_nPreDisplayRow=DisplayCount;       
                  SortStockIndex(nStockType,ID_HLP);

  				  SetTypeToDocumentLmb( nStockType);

                  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();
				  m_pGrid->SetSortColumn(GetSortColumn(ID_HLP) ,FALSE);


                  m_pGrid->SetHotKeySelect(GetSortColumn(ID_HLP));
				  currcol=GetSortColumn(ID_HLP);
				  
	              m_pGrid->SetRealRow(DisplayCount+1); 
                  m_pGrid->SetScrollPos32(SB_VERT, 0, FALSE);
				  m_pGrid->EnsureVisible(0, currcol);

				  m_pGrid->Invalidate();
				  break;
	   case 51:  
	   case 52:  
	   case 53:  
	   case 54:  
	   case 55:  
	   case 56:  
		          nStockType=m_key -50;
		          if(nStockType > 2 )
					  nStockType++;
		          
		          DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
                  pDoc->m_nPreDisplayRow=DisplayCount;       
                  SortStockIndex(nStockType,ID_BSSPBAS);

				  SetTypeToDocumentLmb( nStockType);

                  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();
				  m_pGrid->SetSortColumn(GetSortColumn(ID_BSSPBAS) ,FALSE);


                  m_pGrid->SetHotKeySelect(GetSortColumn(ID_BSSPBAS));
				  

				  currcol=GetSortColumn(ID_BSSPBAS);
				  m_pGrid->SetRealRow(DisplayCount+1); 
                  m_pGrid->SetScrollPos32(SB_VERT, 0, FALSE);
				  m_pGrid->EnsureVisible(0, currcol);

				  m_pGrid->Invalidate();
				  break;
	   case 6:
				  m_pGrid->SetSortColumn(-1 ,TRUE);

                  m_PageWnd->m_nActivePage=CHOOSEPAGE;
                  m_PageWnd->m_bRefrash=TRUE;
                  m_PageWnd->Invalidate();
				  ChangeToPage(CHOOSEPAGE);					  
		          break;
	   case 90:
       case 91:
       case 92:
       case 93:
       case 94:
       case 95:
       case 96:
       case 97:
       case 98:
	   case 99:
		   {
                  break;
		   
		   }
       case 71:
       case 72:
       case 73:
       case 74:
		   {
			   ((CMainFrame*)AfxGetMainWnd())->ViewGgxx (m_key-71);

		   }
				  	
                  break;
	   
	   case 03:
       case 04:
       case 05:
       case 07:
		   {
		    CString stock_code;
		    if(m_key == 3)
			{
				stock_code = CSharesCompute::GetIndexSymbol(0);
			    pDoc->m_stkKind = SHZS;
			}
			else if(m_key >= 5)
			{
				static bool bSh = true;
				if(bSh )
				{
					stock_code = CSharesCompute::GetIndexSymbol(0);
					pDoc->m_stkKind = SHZS;
				}
				else
				{
					stock_code = CSharesCompute::GetIndexSymbol(1);
					pDoc->m_stkKind = SZZS;
				}
				bSh=!bSh;
			}
			else
			{
				stock_code = CSharesCompute::GetIndexSymbol(1);
				pDoc->m_stkKind = SZZS;
			}
			pDoc->m_sharesSymbol = stock_code;
			pDoc->m_GetFocus = true;
			CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
			pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(1+pDoc->m_stkKind*10));
			return;

		   }
		break;
	
        case 81:
        case 82:
        case 83:
        case 84:
        case 85:
        case 86:
        case 87:
        case 89:
            CWnd *pWnd=GetParent();
			if(pWnd!=NULL)
               pWnd->ShowWindow(SW_SHOWMAXIMIZED);   
			if(pDoc->m_NineShowView != NULL)
			{
			  CNineShowView *pView = pDoc->m_NineShowView;
			  pView->IsQuit = 1;
	          pView->GetParent()->SendMessage(WM_CLOSE);
			}
			CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		   	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
			
			CTaiShanDoc* pDoc=(CTaiShanDoc*)GetDocument();
			CDocTemplate* pTemplate= ((CTaiShanApp*)AfxGetApp())->pNineShowTemplate;
			CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDoc,pFrameWnd);
			pTemplate->InitialUpdateFrame(pFrame,pDoc);
		    pFrameWnd = ((CFrameWnd*)pMainWnd)->GetActiveFrame();
			CView *pView=pFrameWnd->GetActiveView();
		
			pDoc->m_NineShowView=(CNineShowView *)pView;
			if(m_key!=89)
			   pDoc->m_NineShowView->FirstMenu = m_key - 81;
			else
               pDoc->m_NineShowView->FirstMenu = 7;
			pDoc->m_NineShowView->SendMessage(WM_KEYBOARDGENIUS,m_key,0);


			pDoc->m_NineShowView->SetFocus();
			return;
   }
	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
	long m_CurrRow=m_pGrid->GetCurrentRow();
    m_TopLeftCell.row= m_CurrRow;
	m_pGrid->SetFocusCell(m_TopLeftCell);

}
UINT CTaiShanReportView::GetSortColumn(UINT nID)
{

    CTaiShanDoc* pDoc = GetDocument();
    for(int i=0;i<MAXCOLUME;i++)
	    if((UINT)pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==nID)
			return i ;

	return -1;
}
void CTaiShanReportView::DisplaySortDataInPage(SORTSTRUCT *m_pStockData,int DisplayCount)
{
}
void CTaiShanReportView::SortStockIndex(UINT nPage,UINT nID)
{
 	CTaiShanDoc* pDoc = GetDocument();
    int index=0;
	int DisplayCount=0;

	int nType = GetStockTypeFromPageWnd(pDoc->m_nCurrentPage);

	if((int )nPage!=nType)
	{
        SetShowData(pDoc,pDoc->m_sharesInformation.GetStockTypeCount(nPage));
	    pDoc->m_sharesInformation.RecvStockDataForType(pDoc->m_pStockDataShow,nPage);

		if(nPage==6||nPage<3)
          pDoc->m_nCurrentPage=SHPAGE;
		else if(nPage==7||(nPage>=3&&nPage<8))
          pDoc->m_nCurrentPage=SZPAGE;
		else if(nPage>=8&&nPage<10)
          pDoc->m_nCurrentPage=SZEBPAGE;
	}
    DisplayCount=pDoc->m_sharesInformation.GetStockTypeCount(nPage);
	for(int row=1 ;row <=  DisplayCount  ;row++)        
	{    
		     CReportData *Cdat;
			 Cdat = pDoc->m_pStockDataShow[row -1].pItem;
			 if(Cdat==NULL)
				 continue;
			 if(pDoc->m_pStockDataShow[row -1].pItem==NULL)
			 {
			    pDoc->m_pStockDataShow[row -1].ItemData =Cdat->totv  ;
				continue;
			 }
			 switch(nID)
			 {
				case ID_BUYP1:                             
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->pbuy1 ;break;
				case ID_BUYP2:                           
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->pbuy2 ;break;
				case ID_BUYP3:                              
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->pbuy3 ;break;
				case ID_BUYV1:                          
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->vbuy1 ;break;
				case ID_BUYV2:                           
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->vbuy2 ;break;
				case ID_BUYV3:                              
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->vbuy3 ;break;
				case ID_SELP1:                            
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->psel1 ;break;
				case ID_SELP2:                             
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->psel2 ;break;
				case ID_SELP3:                             
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->psel3 ;break;
				case ID_SELV1:                            
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->vsel1 ;break;
				case ID_SELV2:                             
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->vsel2 ;break;
				case ID_SELV3:                            
						pDoc->m_pStockDataShow[row -1].ItemData = Cdat->vsel3 ;break;
				case ID_TOTV:                             
					  pDoc->m_pStockDataShow[row -1].ItemData =Cdat->totv  ;
					break;
				case ID_TOTP:                              
					  pDoc->m_pStockDataShow[row -1].ItemData= Cdat->totp  /10000 ;
					break;
				case ID_ADDE:                               
                      pDoc->m_pStockDataShow[row -1].ItemData=Cdat->nowp - Cdat->ystc;
					break;
				case ID_HLP:                               
					if(Cdat->ystc !=0)
					   pDoc->m_pStockDataShow[row -1].ItemData=(Cdat->higp - Cdat->lowp)/Cdat->ystc ;
					else
                       pDoc->m_pStockDataShow[row -1].ItemData=0;
					break;
				case ID_RDR:                               
					if(Cdat->ystc!=0)
					{
					  pDoc->m_pStockDataShow[row -1].ItemData=100 * (Cdat->nowp - Cdat->ystc )/ Cdat->ystc;
					  if(pDoc->m_pStockDataShow[row -1].ItemData==-100)
                         pDoc->m_pStockDataShow[row -1].ItemData=0;
					}
					else
                      pDoc->m_pStockDataShow[row -1].ItemData=0;
					break;
				case ID_BSSPBAS:                     
					{
					  float temp;
					  if (Cdat->accb+Cdat->accs>0)
						if(Cdat->accs==0&&Cdat->accb!=0)
						   temp=100;
						else if(Cdat->accs!=0&&Cdat->accb==0)
						   temp=-100;
						else
						   temp=(float)((Cdat->accb-Cdat->accs)/(Cdat->accb+Cdat->accs)*100.0);     //计算委比
					   else
					 	   temp=0;

					   pDoc->m_pStockDataShow[row -1].ItemData=temp;
					}
					break;
				case ID_DIFBS:                           
                    pDoc->m_pStockDataShow[row -1].ItemData=Cdat->accb - Cdat->accs;
					break;
				case ID_VOLBI:                             
					if(pDoc->m_nANT[0] !=0 && Cdat->volume5!=0 )
					    pDoc->m_pStockDataShow[row -1].ItemData=Cdat->totv / pDoc->m_nANT[0] / Cdat->volume5;
					else
						pDoc->m_pStockDataShow[row -1].ItemData=0;
					break;
				case ID_ADVSP:                           
					break;

			 }
	}
  QuickSort(pDoc->m_pStockDataShow,FALSE,0,DisplayCount-1);
}

void CTaiShanReportView::OnViewDatamanage() 
{
	
}

void CTaiShanReportView::OnViewInvestmanage() 
{
	
}



LRESULT CTaiShanReportView::OnStockChange(WPARAM wParam, LPARAM lParam)
{
    ChangeToPage(m_PageWnd->m_nActivePage);
	return 0;
}


void CTaiShanReportView::OnLoopDisplay() 
{

    if(m_LoopDisplay==FALSE)
		m_LoopDisplay=TRUE;
	else
		m_LoopDisplay=FALSE;
}

void CTaiShanReportView::OnUpdateLoopDisplay(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(m_LoopDisplay);
}


void CTaiShanReportView::OnDayLine() 
{
	CString stock_code,stock_name;
    CReportData *pCdat;
	long row = m_pGrid->GetCurrentRow();
	
	m_LoopDisplay=FALSE;
	if(!this->GetCdatPointForRow(row,pCdat))
		return;
	stock_code = pCdat->id;
	stock_name = pCdat->name;
    if(stock_code.Compare("") == 0)
		return;
			
	CTaiShanDoc *pDoc = GetDocument();		
	
	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_stkKind=pCdat->kind;

	pDoc->m_GetFocus = true;
	CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
	pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(pDoc->m_stkKind*10+2));
	return ;
}

void CTaiShanReportView::OnRealTime() 
{
	CString stock_code,stock_name;
    CReportData *pCdat;
	long row = m_pGrid->GetCurrentRow();
	
	m_LoopDisplay=FALSE;
	if(!this->GetCdatPointForRow(row,pCdat))
		return;
	stock_code = pCdat->id;
	stock_name = pCdat->name;
	if(stock_code.Compare("") == 0)
		return;
	CTaiShanDoc *pDoc = GetDocument();		
	
	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_stkKind=pCdat->kind;
	pDoc->m_GetFocus = true;
	CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
	pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),(LPARAM)(pDoc->m_stkKind*10+1));
}


void CTaiShanReportView::OnCompanyinfo() 
{
	CTaiShanDoc* pDoc = (CTaiShanDoc *)GetDocument();
	CReportData *pCdat;
	long row = m_pGrid->GetCurrentRow();
	m_LoopDisplay=FALSE;
	if(!this->GetCdatPointForRow(row,pCdat))
		return;
	pDoc->m_sharesSymbol = pCdat->id;
	pDoc->m_stkKind=pCdat->kind;
	CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
	((CMainFrame *)pMainWnd)->OnCompanyinfo();

}

void CTaiShanReportView::OnFileOpen1()
{
	
	SelectStock pDlg(this,TRUE,FALSE,TRUE);
	if(pDlg.DoModal()==IDOK)
	{
       ScrollToSelectStock(pDlg.m_strStockCode.GetBuffer(0),pDlg.m_stkKind) ;
	}
}
void CTaiShanReportView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CView::OnActivate(nState, pWndOther, bMinimized);     
    if(nState == WA_ACTIVE)
	{
	  m_pGrid->SetFocus();
	}
	
}

void CTaiShanReportView::ActiveGrid()
{
   m_pGrid->SetFocus();
}

void CTaiShanReportView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	SetWindowText("动态显示牌");
	HICON hIcon;
	CTaiShanApp *pApp=(CTaiShanApp*)AfxGetApp();
	hIcon=pApp->LoadIcon(IDR_WSSTOCTYPE);
	GetParentFrame()->SetIcon(  hIcon, FALSE );

}

LRESULT CTaiShanReportView::OnKlineToThis(WPARAM wParam, LPARAM lParam)
{
	 char HotKey[10];
	 _itoa( (int)lParam, HotKey, 10 );
	 HotKeyDownProgress(HotKey);

	 return 0;
}


void CTaiShanReportView::OnOpenStock() 
{

	SetTimer(899,100,NULL);
}

void CTaiShanReportView::OnSaveStocktype() 
{
    CTaiShanDoc* pDoc = GetDocument();
    BOOL IsAdd=TRUE;    
	CString filename;
	//CSHEETBHADD dlg;
	//dlg.m_OpenKind =TRUE;
	//if(dlg.DoModal()!=IDOK) 
 //     return ;
	//filename=dlg.m_bkname; 
	ASSERT(FALSE);
	if(filename=="条件选股")
	{
		AfxMessageBox("条件选股为系统采用专用名字，故不能采用！请选择其它名字！");
		return;
	}

    BLOCKSTOCK *block;
	POSITION pos=pDoc->m_Tjxgblocklist.GetHeadPosition();  
	while(pos)
	{
        POSITION prepos=pos;                      
	    block=pDoc->m_Tjxgblocklist.GetNext(pos);    
		if(strcmp(block->blockname,filename)==0)     
		{
			block->stocklist.RemoveAll();           

			IsAdd=FALSE;     
			break;
		}
	}


	if(pDoc->m_Tjxgblocklist.GetCount()>=80 )
	{
		AfxMessageBox("系统提供最大条件选股数为80，请删除不用条件选股后再增加！");
		return;
	}
    char choosehead[]={65,79,88,14};
	int length;
	char  blank[2]={' ',' '};            


    FILE *fp;
	if((fp=fopen("板块数据/条件选股名.DAT","r+b"))!=NULL)
	{
      if(IsAdd)
	  {
        fseek(fp,4,SEEK_SET);
		fread(&length,4,1,fp);
		length++;
        fseek(fp,4,SEEK_SET);
		fwrite(&length,4,1,fp);

		fseek(fp,0,SEEK_END);
		char file[13];
		strcpy(file,filename.GetBuffer(0));
		file[12]='\0';
		fwrite(file,1,12,fp);  
		fwrite(&blank,2,1,fp);
		fclose(fp);
		 block=new BLOCKSTOCK(filename);
	  }
	  else
	  {
		 fclose(fp);

	    block->blockname=filename;
	  }
		CString PathFile=_T("板块数据//")+filename;
        FILE *fp1;
		fp1=fopen(PathFile.GetBuffer(0),"w+b" );
		int stocklength=pDoc->m_StockTypeMap[2].GetCount();
		fwrite(&stocklength,1,4,fp1);
	    POSITION pos=pDoc->m_StockTypeMap[2].GetStartPosition( );  
		while(pos)
		{
			CString zqdm;
			CString zqdmkind;
			int nKind;
			pDoc->m_StockTypeMap[2].GetNextAssoc(pos,zqdmkind,zqdm);
            nKind=pDoc->GetStockKind(zqdmkind.Left(2));  
			fwrite(zqdm.GetBuffer(6),1,6,fp1);
			fwrite(&nKind,4,1,fp);
			fwrite(&blank,1,2,fp);

	        SharesSymbol *symbol=new SharesSymbol;
			strcpy(symbol->sharesymbol ,zqdm.GetBuffer (0));
			block->stocklist.AddTail(symbol);
		}
		if(IsAdd)
		   pDoc->m_Tjxgblocklist.AddTail(block);
        fclose(fp1);
	    m_PageWnd->SetPageTitle(filename,TJXGPAGE);
	    strcpy(pDoc->m_SystemInitData.ScreenStockName,filename);
        m_PageWnd->Invalidate();
	}
}

void CTaiShanReportView::OnChangetype() 
{
  CTaiShanDoc *pDoc = GetDocument();
  int i = m_PageWnd->m_nActivePage;
  switch(m_PageWnd->m_nActivePage)
  {
	 case SHPAGE: if(pDoc->m_nSharesSh==3)
				  {
					  i++;
                      pDoc->m_nSharesSz=0; 
				  }
				  else
					 pDoc->m_nSharesSh++; 
		  break;
	 case SZPAGE: if(pDoc->m_nSharesSz==3)
				  {
					 i++;
                     pDoc->m_nSharesSzEb=0; 
				  }
				 else
					 pDoc->m_nSharesSz++; 
		  break;
	 case SZEBPAGE: if(pDoc->m_nSharesSzEb==1)
                     i++; 
				  else
					 pDoc->m_nSharesSzEb++;
            break;
     default:
		        i++;  
  }

  if(i >TJXGPAGE)
  {
	  pDoc->m_nSharesSh=0;
	  i = 0;
  }
  ChangeToPage(i);

  m_PageWnd->m_bMove = true;
  m_PageWnd->m_nActivePage = i;
  m_PageWnd->m_bRefrash=TRUE;
  m_PageWnd->Invalidate();
  return;
}
BOOL CTaiShanReportView::GetStockCodeForRow(int nRows,char *Code,int &nKind)
{
	CReportData *pCdat;
	if(!GetCdatPointForRow(nRows,pCdat))
		return FALSE;
	strcpy(Code,pCdat->id);
	nKind=pCdat->kind;
	return TRUE;
}
BOOL CTaiShanReportView::GetRowForStockCode(char * Code,int & nRow)
{
   CTaiShanDoc* pDoc = GetDocument();
   int nCount=GetTotalRowsLmb(m_PageWnd->m_nActivePage);

   for(int i=0 ;i<nCount;i++)
   {
      if(strcmp(pDoc->m_pStockDataShow[i].StockId,Code)==0)
	  {
          nRow=i;
	      return TRUE;
	  }
   }
   return FALSE;
}
void CTaiShanReportView::DefalutColumnSelect()
{
   	CTaiShanDoc* pDoc = GetDocument();
	for(int i=0;i<MAXCOLUME;i++)
	  pDoc->m_nColumnIndex[0][i]=-1;

	for(int i=0;i<INITCOLUME;i++)
	   pDoc->m_nColumnIndex[0][i]=i+ID_ROWNUM;

	memcpy(pDoc->m_SystemInitData.ColumnIndex,&pDoc->m_nColumnIndex[0],sizeof(int)*MAXCOLUME);
    ChangedColumnType(0);
}

void CTaiShanReportView::ChangedColumnType(int ColumnType)
{
   	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    pDoc->m_SystemInitData.ColumnType=ColumnType;
    m_nColumnTitleArray.RemoveAll( );
	for(int i=0;i<MAXCOLUME;i++)
	{
		CString str;
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			break;
		switch(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i])
		{
		case ID_ROWNUM:										
				str.Format("  ");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_STOCKID:                                    
				str.Format("代  码");
			    m_nColumnTitleArray.Add(str);
			    break;
		case ID_STOCKNAME:                                 
				str.Format("证券名称");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_YSTC:                             
				str.Format("昨收");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_OPEN:                             
				str.Format("今开");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HIGH:                              
				str.Format("最高");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LOW:                             
				str.Format("最低");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_NEW:                               
				str.Format("最新");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_TOTV:                             
				str.Format("总手");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BUYP1:                          
                m_nColumnTitleArray.Add(g_sBuySell[0]);break;
		case ID_BUYP2:                           
                m_nColumnTitleArray.Add(g_sBuySell[1]);break;
		case ID_BUYP3:                              
                m_nColumnTitleArray.Add(g_sBuySell[2]);break;
		case ID_BUYV1:                             
                m_nColumnTitleArray.Add(g_sBuySell[3]);break;
		case ID_BUYV2:                              
                m_nColumnTitleArray.Add(g_sBuySell[4]);break;
		case ID_BUYV3:                              
                m_nColumnTitleArray.Add(g_sBuySell[5]);break;
		case ID_SELP1:                              
                m_nColumnTitleArray.Add(g_sBuySell[6]);break;
		case ID_SELP2:                            
                m_nColumnTitleArray.Add(g_sBuySell[7]);break;
		case ID_SELP3:                            
                m_nColumnTitleArray.Add(g_sBuySell[8]);break;
		case ID_SELV1:                              
                m_nColumnTitleArray.Add(g_sBuySell[9]);break;
		case ID_SELV2:                              
                m_nColumnTitleArray.Add(g_sBuySell[10]);break;
		case ID_SELV3:                              
                m_nColumnTitleArray.Add(g_sBuySell[11]);break;
		case ID_NOWV:                              
				str.Format("现手");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_TOTP:                              
			    str.Format("总额");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_AVGP:                               
				str.Format("均价");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ADDE:                             
				str.Format("涨跌");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HLP:                               
				str.Format("震幅");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_RDR:                                 //涨幅
				str.Format("涨幅");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BSSPBAS:                            //委比
				str.Format("委比");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_DIFBS:                              //委差
				str.Format("委差");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_VOLBI:                              //量比
				str.Format("量比");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ACCB:                               //委买
				str.Format("委买");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ACCS:                               //委卖
				str.Format("委卖");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BUY:                                //委买价
				str.Format("委买价");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_SEL:                                //委卖价
				str.Format("委卖价");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_RVOL:                               //内盘
				str.Format("内盘");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_DVOL:                              //外盘
				str.Format("外盘");
				m_nColumnTitleArray.Add(str);
			    break;
        case ID_EXCHANGE:
				str.Format("换手率");
				m_nColumnTitleArray.Add(str);
			    break;
        case ID_SYL:
				str.Format("市盈率");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZGB:        //总股本(万股)
				str.Format("总股本(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GJG:        //国家股(万股)
				str.Format("国家股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
				str.Format("发起人法人股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_FRG:        //法人股(万股)
				str.Format("法人股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZGG:        //职工股(万股)
				str.Format("职工股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GZAG:       //公众A股(万股)
				str.Format("公众A股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LTAG:       //流通A股(万股)
				str.Format("流通A股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HG:         //Ｈ股(万股)
				str.Format("Ｈ股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_BG:         //B股(万股)
				str.Format("B股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZPG:        //转配股(万股)
				str.Format("转配股(万股)");
				m_nColumnTitleArray.Add(str);
			    break;

		case ID_ZZC:        //总资产(万元)
				str.Format("总资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LDZC:       //流动资产(万元)
				str.Format("流动资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_CQTZ:       //长期投资(万元)
				str.Format("长期投资(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GDZC:       //固定资产(万元)
				str.Format("固定资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_WXZC:       //无形资产(万元)
				str.Format("无形资产(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LDFZ:       //流动负债(万元)
				str.Format("流动负债(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_CQFZ:       //长期负债(万元)
				str.Format("长期负债(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GGQY:       //股东权益(万元)
				str.Format("股东权益(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
				str.Format("资本公积金(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
				str.Format("盈余公积金(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGJZ:       //每股净值(元)
				str.Format("每股净值(元)"); 
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
				str.Format("股东权益比率(%s)","%");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGGJJ:      //每股公积金(元)
				str.Format("每股公积金(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
				str.Format("主营业务收入(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
				str.Format("主营业务利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
				str.Format("其它业务利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_LRZE:       //利润总额(万元)
				str.Format("利润总额(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_JLR:        //净利润(万元)
				str.Format("净利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_WFPLR:      //未分配利润(万元)
				str.Format("未分配利润(万元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGSY:       //每股收益(元)
				str.Format("每股收益(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
				str.Format("净资产收益率(%s)","%");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
				str.Format("每股未分配利润(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_MGJZC:      //每股净资产(元)
				str.Format("每股净资产(元)");
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ZJLS:        //资金(%d分)
				str.Format("资金(%d分)",pDoc->m_syshqset.capital);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_VOLBIFM:    //量比(%d分)
				str.Format("量比(%d分)",pDoc->m_syshqset.quangtity);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_HLPFM:      //震幅(%d分)
				str.Format("震幅(%d分)",pDoc->m_syshqset.swing);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_EXCHANGEFM:  //换手率(%d分)
				str.Format("换手率(%d分)",pDoc->m_syshqset.changehand);
				m_nColumnTitleArray.Add(str);
			    break;
		case ID_ADVSP:       //涨速(%d分)
				str.Format("涨速(%d分)",pDoc->m_syshqset.upspeed);
				m_nColumnTitleArray.Add(str);
			    break;


		case ID_DELITEM:    //删除表项
				str.Format("删除表项");
				m_nColumnTitleArray.Add(str);
			    break;
		default:
			    UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i];
				if(nID>=9000&&nID<9010)
				{
					TECHNIQUE_COLUMN *pIndexIndex;
					if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						return;
					str.Format("%s(%s)",pIndexIndex->nIndex.sIndex_name,pIndexIndex->nIndex.SubLineType);
					m_nColumnTitleArray.Add(str);
					break;
				}
			    break;

			    break;
		}
	}

	m_nCols=m_nColumnTitleArray.GetSize();
	try {

		m_pGrid->SetColumnCount(m_nCols+1);
		GetColumnWith();
		m_pGrid->SetColumnCount(m_nCols);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return;
	}

	for(int i=0;i<MAXCOLUME;i++)
	{
        char str[40];
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			break;
		int nID = pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i];
		if(nID>=ID_BUYP1 &&nID<=ID_SELV3)
		{
				strcpy(str ,g_sBuySell[nID-ID_BUYP1]);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,nID);
		}
		else
		{
		switch(nID)
		{
		case ID_ROWNUM:										
				strcpy(str,"  ");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ROWNUM);
			    break;
		case ID_STOCKID:                                        //代码
				strcpy(str,"代  码");
                m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_STOCKID);
			    break;
		case ID_STOCKNAME:                                      //名称
				strcpy(str,"证券名称");
                m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_STOCKNAME);
			    break;
		case ID_YSTC:                                //昨收
				strcpy(str,"昨收");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_YSTC);
			    break;
		case ID_OPEN:                                //今开
				strcpy(str,"今开");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_OPEN);
			    break;
		case ID_HIGH:                                //最高
				strcpy(str,"最高");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HIGH);
			    break;
		case ID_LOW:                                 //最低
				strcpy(str,"最低");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LOW);
			    break;
		case ID_NEW:                                 //最新
				strcpy(str,"最新");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_NEW);
			    break;
		case ID_TOTV:                                //总手
				strcpy(str,"总手");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_TOTV);
			    break;
		case ID_NOWV:                                //现手
				strcpy(str,"现手");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_NOWV);
			    break;
		case ID_TOTP:                                //总额
			    strcpy(str,"总额");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_TOTP);
			    break;
		case ID_AVGP:                                //均价
				strcpy(str,"均价");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_AVGP);
			    break;
		case ID_ADDE:                                //涨跌
				strcpy(str,"涨跌");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ADDE);
			    break;
		case ID_HLP:                                 //震幅
				strcpy(str,"震幅");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HLP);
			    break;
		case ID_RDR:                                 //涨幅
				strcpy(str,"涨幅");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_RDR);
			    break;
		case ID_BSSPBAS:                            //委比
				strcpy(str,"委比");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_BSSPBAS);
			    break;
		case ID_DIFBS:                              //委差
				strcpy(str,"委差");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_DIFBS);
			    break;
		case ID_VOLBI:                              //量比
				strcpy(str,"量比");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_VOLBI);
			    break;
		case ID_ACCB:                               //委买
				strcpy(str,"委买");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ACCB);
			    break;
		case ID_ACCS:                               //委卖
				strcpy(str,"委卖");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ACCS);
			    break;
		case ID_BUY:                                //委买价
				strcpy(str,"委买价");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_BUY);
			    break;
		case ID_SEL:                                //委卖价
				strcpy(str,"委卖价");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_SEL);
			    break;
		case ID_RVOL:                               //内盘
				strcpy(str,"内盘");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_RVOL);
			    break;
		case ID_DVOL:                              //外盘
				strcpy(str,"外盘");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_DVOL);
			    break;
        case ID_EXCHANGE:
				strcpy(str,"换手率");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_EXCHANGE);
			    break;
        case ID_SYL:
				strcpy(str,"市盈率");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_SYL);
			    break;
		case ID_ZGB:        //总股本(万股)
				strcpy(str,"总股本(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZGB);
			    break;
		case ID_GJG:        //国家股(万股)
				strcpy(str,"国家股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GJG);
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
				strcpy(str,"发起人法人股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_FQRFRG);
			    break;
		case ID_FRG:        //法人股(万股)
				strcpy(str,"法人股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_FRG);
			    break;
		case ID_ZGG:        //职工股(万股)
				strcpy(str,"职工股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZGG);
			    break;
		case ID_GZAG:       //公众A股(万股)
				strcpy(str,"公众A股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GZAG);
			    break;
		case ID_LTAG:       //流通A股(万股)
				strcpy(str,"流通A股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LTAG);
			    break;
		case ID_HG:         //Ｈ股(万股)
				strcpy(str,"Ｈ股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HG);
			    break;
		case ID_BG:         //B股(万股)
				strcpy(str,"B股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_BG);
			    break;
		case ID_ZPG:        //转配股(万股)
				strcpy(str,"转配股(万股)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZPG);
			    break;

		case ID_ZZC:        //总资产(万元)
				strcpy(str,"总资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZZC);
			    break;
		case ID_LDZC:       //流动资产(万元)
				strcpy(str,"流动资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LDZC);
			    break;
		case ID_CQTZ:       //长期投资(万元)
				strcpy(str,"长期投资(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_CQTZ);
			    break;
		case ID_GDZC:       //固定资产(万元)
				strcpy(str,"固定资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GDZC);
			    break;
		case ID_WXZC:       //无形资产(万元)
				strcpy(str,"无形资产(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_WXZC);
			    break;
		case ID_LDFZ:       //流动负债(万元)
				strcpy(str,"流动负债(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LDFZ);
			    break;
		case ID_CQFZ:       //长期负债(万元)
				strcpy(str,"长期负债(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_CQFZ);
			    break;
		case ID_GGQY:       //股东权益(万元)
				strcpy(str,"股东权益(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GGQY);
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
				strcpy(str,"资本公积金(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZBGJJ);
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
				strcpy(str,"盈余公积金(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_YYGJJ);
			    break;
		case ID_MGJZ:       //每股净值(元)
				strcpy(str,"每股净值(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGJZ);
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
				strcpy(str,"股东权益比率(%)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_GDQYBL);
			    break;
		case ID_MGGJJ:      //每股公积金(元)
				strcpy(str,"每股公积金(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGGJJ);
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
				strcpy(str,"主营业务收入(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZYYWSR);
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
				strcpy(str,"主营业务利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZYYWLR);
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
				strcpy(str,"其它业务利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_QTYWLR);
			    break;
		case ID_LRZE:       //利润总额(万元)
				strcpy(str,"利润总额(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_LRZE);
			    break;
		case ID_JLR:        //净利润(万元)
				strcpy(str,"净利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_JLR);
			    break;
		case ID_WFPLR:      //未分配利润(万元)
				strcpy(str,"未分配利润(万元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_WFPLR);
			    break;
		case ID_MGSY:       //每股收益(元)
				strcpy(str,"每股收益(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGSY);
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
				strcpy(str,"净资产收益率(%)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_JZCSYL);
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
				strcpy(str,"每股未分配利润(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGWFPLR);
			    break;
		case ID_MGJZC:      //每股净资产(元)
				strcpy(str,"每股净资产(元)");
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_MGJZC);
			    break;
		case ID_ZJLS:        //资金(%d分)
				sprintf(str,"资金(%d分)",pDoc->m_syshqset.capital);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ZJLS);
			    break;
		case ID_VOLBIFM:    //量比(%d分)
				sprintf(str,"量比(%d分)",pDoc->m_syshqset.quangtity);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_VOLBIFM);
			    break;
		case ID_HLPFM:      //震幅(%d分)
				sprintf(str,"震幅(%d分)",pDoc->m_syshqset.swing);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_HLPFM);
			    break;
		case ID_EXCHANGEFM:  //换手率(%d分)
				sprintf(str,"换手率(%d分)",pDoc->m_syshqset.changehand);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_EXCHANGEFM);
			    break;
		case ID_ADVSP:       //涨速(%d分)
				sprintf(str,"涨速(%d分)",pDoc->m_syshqset.upspeed);
                m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
					str,ID_ADVSP);
			    break;

		default:
			{
				UINT nID=pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i];
				if(nID>=9000&&nID<9010)
				{
					TECHNIQUE_COLUMN *pIndexIndex;
					if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(nID,pIndexIndex))
						continue;
					sprintf(str,"%s(%s)",pIndexIndex->nIndex.sIndex_name,pIndexIndex->nIndex.SubLineType);
					m_pGrid->SetColumnInfo(i,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
						str,nID);
				}
			}

			    break;
		}
		}
	}
    m_pGrid->SetSortColumn(-1 ,TRUE);

	m_pGrid->ClearSortColumn();
    m_pGrid->SetFocus();

    m_pGrid->SetFocusRow(1); 
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);

	int pos=m_hScrollBar->GetScrollPos();
	for(int i=0;i<=pos;i++)
		m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);
	int nMax=m_nCols - m_nFixCols +1;
	m_hScrollBar->SetScrollRange( 0, nMax, TRUE);
    m_hScrollBar->SetScrollPos(0,true);



    m_pGrid->Invalidate();                      
 	return ;
}

void CTaiShanReportView::OnStockType1() 
{
	
    ChangedColumnType(0);
	
}
void CTaiShanReportView::OnStockType2() 
{

    ChangedColumnType(1);
}
void CTaiShanReportView::OnStockType3() 
{

    ChangedColumnType(2);
	
}
void CTaiShanReportView::OnStockType4() 
{

    ChangedColumnType(3);
}
void CTaiShanReportView::OnDisplayStockTypeChange(UINT nID)
{
	CTaiShanDoc* pDoc = GetDocument();
    int nDisplay=nID-ID_MENU_TYPE1;
	SetColumnWidth();
	switch(nDisplay)
	{
		case 0: ChangedColumnType(0);

	         		 break;  

#ifndef OEM
		case 1: ChangedColumnType(1);

	         		 break;      
		case 2: ChangedColumnType(2);

	         		 break;      
		case 3: ChangedColumnType(3);

	         		 break;      
#endif
		case 4:DefalutColumnSelect();
			   pDoc->m_pTechniqueColumn->SetAllColumnNotUse();


	         		 break;      
	}
}
void CTaiShanReportView::OnUpdateDisplayStockType(CCmdUI* pCmdUI)
{
	CTaiShanDoc* pDoc = GetDocument();
    int nDisplay=pCmdUI->m_nID-ID_MENU_TYPE1;
	if(pCmdUI==NULL)
		return;

#ifdef OEM
	if(nDisplay>0) 
	{
		pCmdUI->Enable (FALSE);
		return;
	}
#endif
	if(pDoc->m_SystemInitData.ColumnType==nDisplay)
	    pCmdUI->SetCheck(TRUE);
	else
	    pCmdUI->SetCheck(FALSE);
}
void CTaiShanReportView::OnOpenNineShow()
{
   HotKeyDownProgress("81");
}
void CTaiShanReportView::OnDeleteTjxgStockType()
{
    CTaiShanDoc* pDoc = GetDocument();
	BOOL IsDelete=FALSE;
	POSITION pos=pDoc->m_Tjxgblocklist.GetHeadPosition();  
	CString TjxgName;
	while(pos)
	{
	    BLOCKSTOCK *block;
        POSITION prepos=pos;
	    block=pDoc->m_Tjxgblocklist.GetNext(pos);

		if(strcmp(block->blockname,pDoc->m_SystemInitData.ScreenStockName)==0)
		{
			block->stocklist.RemoveAll();
			pDoc->m_Tjxgblocklist.RemoveAt(prepos); 
			IsDelete=TRUE;
			break;
		}
	}
	if(!IsDelete)
		return;
    char choosehead[]={65,79,88,14};
	int length;
	char  blank[2]={' ',' '};            
    FILE *fp;
	if((fp=fopen("板块数据/条件选股名.DAT","w+b"))!=NULL)
	{
        fseek(fp,0,SEEK_SET);
		fwrite(choosehead,1,4,fp);
        length=pDoc->m_Tjxgblocklist.GetCount();
		fwrite(&length,4,1,fp);
		POSITION pos=pDoc->m_Tjxgblocklist.GetHeadPosition();  
		int index=0;
		while(pos)
		{
			BLOCKSTOCK *block;
			block=pDoc->m_Tjxgblocklist.GetNext(pos);
			if(index==0)
				TjxgName=block->blockname;
			fwrite(block->blockname,1,12,fp);
			fwrite(blank,1,2,fp);
			index++;
		}
		fclose(fp);

	}
    ChangeToTjxg(pDoc,TjxgName);
}
void CTaiShanReportView::OnSaveScreenStocktype(SymbolKindArr &StockCodeArray ,CString *StockName) 
{
    CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
    BOOL IsAdd=TRUE;    
	CString filename;
	//CSHEETBHADD dlg;
	//if(StockName->GetLength()==0)
	//{
	//	dlg.m_OpenKind =TRUE;
	//	if(dlg.DoModal()!=IDOK) 
	//	  return ;
	//	filename=dlg.m_bkname;
	//}
	//else
	//{
	//	filename=StockName->GetBuffer(0);
	//}
	ASSERT(FALSE);
	if(filename=="条件选股")
	{
		AfxMessageBox("条件选股为系统采用专用名字，故不能采用！请选择其它名字！");
		return;
	}

    BLOCKSTOCK *block;
	POSITION pos=pDoc->m_Tjxgblocklist.GetHeadPosition();  
	while(pos)
	{
        POSITION prepos=pos;
	    block=pDoc->m_Tjxgblocklist.GetNext(pos);
		if(strcmp(block->blockname,filename)==0)
		{
			block->stocklist.RemoveAll();

			IsAdd=FALSE;
			break;
		}
	}


	if(pDoc->m_Tjxgblocklist.GetCount()>=80 )
	{
		AfxMessageBox("系统提供最大条件选股数为80，请删除不用条件选股后再增加！");
		return;
	}
    char choosehead[]={65,79,88,14};
	int length;
	char  blank[2]={' ',' '};            


    FILE *fp;
	if((fp=fopen("板块数据/条件选股名.DAT","r+b"))!=NULL)
	{
      if(IsAdd)
	  {
        fseek(fp,4,SEEK_SET);
		fread(&length,4,1,fp);
		length++;
        fseek(fp,4,SEEK_SET);
		fwrite(&length,4,1,fp);

		fseek(fp,0,SEEK_END);
		char file[13];
		strcpy(file,filename.GetBuffer(0));
		file[12]='\0';
		fwrite(file,1,12,fp);  
		fwrite(&blank,2,1,fp);
		fclose(fp);
		block=new BLOCKSTOCK(filename);
	  }
	  else
	  {
		fclose(fp);
		block->blockname=filename;
	  }
   
		CString PathFile=_T("板块数据//")+filename;
        FILE *fp1;
		fp1=fopen(PathFile.GetBuffer(0),"w+b" );
		int stocklength=StockCodeArray.GetSize();
		fwrite(&stocklength,1,4,fp1);
		for(int i=0;i<stocklength;i++)
		{
			SymbolKind m_SymbolKind=StockCodeArray.GetAt(i);
			fwrite(m_SymbolKind.m_chSymbol,1,6,fp1);
			fwrite(&m_SymbolKind.m_nSymbolKind,1,4,fp1);

			fwrite(&blank,1,2,fp);

	        SharesSymbol *symbol=new SharesSymbol;
			strcpy(symbol->sharesymbol ,m_SymbolKind.m_chSymbol);
            symbol->nKind=m_SymbolKind.m_nSymbolKind;
			block->stocklist.AddTail(symbol);
		}
		if(IsAdd)
		  pDoc->m_Tjxgblocklist.AddTail(block);
        fclose(fp1);
	}
}

CString CTaiShanReportView::GetCurrentSymbol()
{
	CString stock_code,stock_name;

	long row = m_pGrid->GetCurrentRow();
	CReportData *pCdat;
	m_LoopDisplay=FALSE;
	if(!this->GetCdatPointForRow(row,pCdat))
		return "";
	if(pCdat!=NULL)
	{
		stock_code = pCdat->id;
		stock_name = pCdat->name;
	}
    if(stock_code.Compare("") == 0)
		return stock_code;
			
	CTaiShanDoc *pDoc = GetDocument();		
	
	pDoc->m_sharesSymbol = stock_code;
	pDoc->m_stkKind=pCdat->kind;
	pDoc->m_GetFocus = true;
	return stock_code;
}

BOOL CTaiShanReportView::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;


}

void CTaiShanReportView::OnRButtonUp(UINT nFlags, CPoint point) 
{

	
	CView::OnRButtonUp(nFlags, point);
}

void CTaiShanReportView::OnF6()	
{
	char* sKey="06";
	GetParentFrame()->ActivateFrame();
	HotKeyDownProgress(sKey);
}
int CTaiShanReportView::GetPageRows()
{
	return this->m_pGrid->GetRealRow()-1;
}
BOOL CTaiShanReportView::GetCdatPointForRow(int nRow,PCdat1 &pStockData)
{
	int cc=GetPageRows();
    if(nRow>GetPageRows())
	{
		pStockData=NULL;
		return FALSE;
	}
 	CTaiShanDoc *pDoc = GetDocument();		
    pStockData=pDoc->m_pStockDataShow[nRow-1].pItem;
	return TRUE;
}
#ifdef WIDE_NET_VERSION
void CTaiShanReportView::SendRecvDataInfomation(ClientTransmitAttributes  TransmitType)
{
 	CTaiShanDoc *pDoc = GetDocument();		
    int nPage=m_PageWnd->m_nActivePage;
	int nKind=0;
	if(nPage==STKINDEXPAGE)
	{
		return;
	}


	nKind = GetStockTypeFromPageWnd(nPage);
	if(nKind>=0)
	{
		m_TransPackageStruct.m_TransmitType=TransmitType;
        m_TransPackageStruct.m_dwTransmitStockType=nKind; 
        m_TransPackageStruct.m_dwTransmitStockCount=0;
		m_TransPackageStruct.m_pTransmitStockAttribute=NULL;
		m_TransPackageStruct.m_dwTransmitStockTypeCounts=-1;
	}
	if(nPage==CHOOSEPAGE||nPage==STKTYPEPAGE||nPage==TJXGPAGE)                      
	{
 		CTaiShanDoc *pDoc = GetDocument();		
		m_TransPackageStruct.m_TransmitType=TransmitType;
        m_TransPackageStruct.m_dwTransmitStockType=-1; 
		m_TransPackageStruct.m_dwTransmitStockTypeCounts=-1;
        m_TransPackageStruct.m_dwTransmitStockCount=GetPageRows();
		if(SetSendStockCodeData(m_TransPackageStruct.m_dwTransmitStockCount))
		{
			for(DWORD i=0;i<m_TransPackageStruct.m_dwTransmitStockCount;i++)
			{
		         CReportData *pCdat=pDoc->m_pStockDataShow[i].pItem;
                 m_pTransmitStockAttribute[i].m_dwTransmitCounts=-1;
                 strcpy(m_pTransmitStockAttribute[i].m_szTransmitStockLabel,pCdat->id); 
			}
		}
		m_TransPackageStruct.m_pTransmitStockAttribute=m_pTransmitStockAttribute;
	}
	CMainFrame * pMainFrm=((CMainFrame *)(AfxGetApp()->m_pMainWnd));
	if(pMainFrm!=NULL&&pMainFrm->m_pClientTransmitManageWnd)
	{
		if(m_SocketID.iFlag==-1)
		{
             m_SocketID=pMainFrm->m_pClientTransmitManageWnd->RegisterID(this->m_hWnd);
		}
		if(m_SocketID.iFlag!=-1)
		{
             pMainFrm->m_pClientTransmitManageWnd->GetMarketData(&m_TransPackageStruct,1,m_SocketID);
		}
	}
}
BOOL CTaiShanReportView::SetSendStockCodeData(int SendStockCount)
{
	HGLOBAL	hMem;
	LPVOID hp;
	TransmitStockAttribute  *pTransmitStockAttribute;
     if(!m_pTransmitStockAttribute)
	 {
         hMem=GlobalAlloc( GPTR, (SendStockCount)* sizeof( TransmitStockAttribute) );
	     hp=GlobalLock(hMem);
		 if( hp )
		 {
            m_pTransmitStockAttribute=(TransmitStockAttribute *)hp;
		    n_nSendStockCount = SendStockCount;
			return TRUE;
		 }
		 else
		 {
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		 }
	 }
	 else
	 {
		 if(n_nSendStockCount >= SendStockCount)
			 return TRUE;
		 else
		 {
			 hMem=GlobalAlloc( GPTR, (SendStockCount)* sizeof( TransmitStockAttribute) );
			 hp=GlobalLock(hMem);
			 if( hp )
			 {
				pTransmitStockAttribute=(TransmitStockAttribute *)hp;
 				GlobalUnlock((HGLOBAL)m_pTransmitStockAttribute);      
				GlobalFree( (HGLOBAL)m_pTransmitStockAttribute);
                m_pTransmitStockAttribute=pTransmitStockAttribute;
		        n_nSendStockCount = SendStockCount;
				return TRUE;
			 }
			 else
			 {
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return FALSE;
			 }
		 }
	 }
}
#endif

void CTaiShanReportView::OnUpdateStockType2(CCmdUI* pCmdUI) 
{

#ifdef OEM
	pCmdUI->Enable (FALSE);
#endif
	
}

void CTaiShanReportView::OnUpdateStockType3(CCmdUI* pCmdUI) 
{
	
#ifdef OEM
	pCmdUI->Enable (FALSE);
#endif
	
}

void CTaiShanReportView::OnUpdateStockType4(CCmdUI* pCmdUI) 
{


#ifdef OEM
	pCmdUI->Enable (FALSE);
#endif
}

void CTaiShanReportView::IndexSortLi()
{
   	CFrameWnd* pFrameWnd=this->GetParentFrame ();
	pFrameWnd->ActivateFrame ();


	CTaiShanDoc *pDoc = GetDocument();

    CCellID m_CellTitleCommand2;
	m_CellTitleCommand2.col = -1;
	int nCol = m_pGrid->GetColumnCount ();
	for(int i = m_pGrid->GetFixedColumnCount();i<nCol;i++)
	{
       if(pDoc->m_nColumnIndex[0][i]>=9000&&pDoc->m_nColumnIndex[0][i]<9010)
	   {
		   m_CellTitleCommand2.col = i;
		   break;
	   }

	}
	if(m_CellTitleCommand2.col == -1)
	{
		m_CellTitleCommand2.col = m_pGrid->GetFixedColumnCount();
	}
	
	{
	#ifndef OEM

		CRect nRect;
		BOOL IsDelete=FALSE;
		GetWindowRect(nRect ); 
		if(pDoc->m_SystemInitData.ColumnType!=0)
			return ;

       if(m_CellTitleCommand2.col==m_pGrid->GetColumnCount() -1 )
	   {
		   IsDelete=TRUE;
	   }
       CString Index; 
	   BOOL IsIndex=FALSE;

	   {
       if(pDoc->m_nColumnIndex[0][m_CellTitleCommand2.col]>=9000&&pDoc->m_nColumnIndex[0][m_CellTitleCommand2.col]<9010)
	   {
		   TECHNIQUE_COLUMN *pIndexIndex;
		   if(!pDoc->m_pTechniqueColumn->GetTechniqueFromId(pDoc->m_nColumnIndex[0][m_CellTitleCommand2.col],pIndexIndex))
				return ;
		   Index=pIndexIndex->nIndex.sIndex_name;
		   IsIndex=TRUE;
	   }
	   }

	   int nId = 98888;
	   int n2 = 0;

	   CPoint point;
	   m_pGrid->OnLeftButton(m_pGrid->GetPoint(m_CellTitleCommand2.col));
	   m_CellTitleCommand.col = m_CellTitleCommand2.col;

	   if(IsIndex == FALSE)
	   {
		   nId = 98887;
		   CString ss = "";
		   {
				int num=pDoc->m_formuar_index.GetSize();
				for(int i=0;i<num;i++)
				{
					if(pDoc->m_formuar_index.ElementAt(i)->isOfen )
					{
						ss=pDoc->m_formuar_index.ElementAt(i)->name;  
						break;
					}
				}
		   }
		   if(ss != "")
		   {
				::SendMessage(m_hWnd, WM_USER_JISHUCOLUMNSELECT ,nId,(LPARAM)(ss.GetBuffer (0)));       
		   }
	   }
	   else
		   ::SendMessage(m_hWnd, WM_USER_JISHUCOLUMNSELECT ,nId,n2);       


	   {
	   }

#endif
	   }

}

void CTaiShanReportView::OnHistoryOpen() 
{

	if(this->m_pHistoryDlg == NULL) CHistoryDlg::Open(this);
}

void CTaiShanReportView::OnUpdateHistoryOpen(CCmdUI* pCmdUI) 
{
	
}

void CTaiShanReportView::SetHistoryColor(bool bHistory)
{


	CTaiShanDoc* pDoc = GetDocument();
	if(bHistory == false)
	{
		m_pGrid->SetGridColor(pDoc->m_colorArray[18],pDoc->m_colorArray[17],pDoc->m_colorArray[16]);
		m_pGrid->SetFixedBkColor(pDoc->m_colorArray[18]);
	    m_pGrid->SetBackgroundClr(pDoc->m_colorArray[18]);
	}
	else
	{
	
		m_pGrid->SetGridColor(pDoc->m_colorArray[19],pDoc->m_colorArray[17],pDoc->m_colorArray[16]);
		m_pGrid->SetFixedBkColor(pDoc->m_colorArray[19]);
	    m_pGrid->SetBackgroundClr(pDoc->m_colorArray[19]);
	}
    m_pGrid->Invalidate();                         

}


BOOL CTaiShanReportView::OnEraseBkgnd(CDC* pDC) 
{

	
	return TRUE;
}

int CTaiShanReportView::GetStockTypeFromPageWnd(int nActive)
{
	CTaiShanDoc* pDoc = GetDocument();
	int nType = -1;
	switch(nActive)
	{
		case SHPAGE:
			 switch(pDoc->m_nSharesSh)
			 {
				 case 0:  nType=SHZS;
						  break;
				 case 1:  nType=SHAG;
						  break; 
				 case 2:  nType=SHBG;
						  break;
				 case 3:  nType=SHZQ;
						  break;
				 case 4:  nType=SHJIJIN;
						  break;
			 }
			 break;
		case SZPAGE:
			 switch(pDoc->m_nSharesSz)
			 {
				 case 0:  nType=SZZS;
						  break;
				 case 1:  nType=SZAG;
						  break; 
				 case 2:  nType=SZBG;
						  break;
				 case 3:  nType=SZZQ;
						  break;
				 case 4:  nType=SZJIJIN;
						  break;
			 }
			 break;
		case SZEBPAGE:
			 switch(pDoc->m_nSharesSzEb)
			 {
				 case 0:  nType=EBZS;
						  break;
				 case 1:  nType=EBAG;
						  break; 
			 }
			 break;
		 case STKINDEXPAGE:
             nType=STKTYPE;
			 break;
	}

	return nType;
}

int CTaiShanReportView::GetTotalRowsLmb(int nActive)
{
	CTaiShanDoc* pDoc = GetDocument();
	int StockType=0;
	int l_TotalRows = 0;
	StockType = GetStockTypeFromPageWnd(nActive);
	if(StockType>=0)
	{
		l_TotalRows=pDoc->m_sharesInformation.GetStockTypeCount(StockType);
	}
	else
	{
		switch(nActive)
		{
			case CHOOSEPAGE:
						  l_TotalRows=pDoc->m_StockTypeMap[0].GetCount();
						  break;
			case STKTYPEPAGE:
						  l_TotalRows=pDoc->m_StockTypeMap[1].GetCount();
						  break;
			case TJXGPAGE:
						  l_TotalRows=pDoc->m_StockTypeMap[2].GetCount();
						  break;
		}
	}
	return l_TotalRows;

}

void CTaiShanReportView::SetTypeToDocumentLmb(int nStockType, bool bSetPageWnd)
{
	CTaiShanDoc* pDoc = GetDocument();
	  switch(nStockType)
	  {
	  case SHZS:
	  case SHAG:
	  case SHBG:
		pDoc->m_nSharesSh=nStockType;
		m_PageWnd->m_nActivePage=SHPAGE;
		break;
	  case SHZQ:
		pDoc->m_nSharesSh=3;
		m_PageWnd->m_nActivePage=SHPAGE;
		break;
	  case SHJIJIN:
		pDoc->m_nSharesSh=4;
		m_PageWnd->m_nActivePage=SHPAGE;
		break;
	  case SZZS:
	  case SZAG:
	  case SZBG:
		pDoc->m_nSharesSh=nStockType-3;
		m_PageWnd->m_nActivePage=SZPAGE;
		break;
	  case SZZQ:
		pDoc->m_nSharesSh=3;
		m_PageWnd->m_nActivePage=SZPAGE;
		break;
	  case SZJIJIN:
		pDoc->m_nSharesSh=4;
		m_PageWnd->m_nActivePage=SZPAGE;
		break;
      case EBZS:
      case EBAG:
		 pDoc->m_nSharesSzEb=nStockType-EBZS;
		 m_PageWnd->m_nActivePage=SZEBPAGE;
		break;
	  }

}
void CTaiShanReportView::SwitchFinance() 
{
	static int nID = 0;
	nID++;
	nID%=4;
	OnDisplayStockTypeChange(nID+ID_MENU_TYPE1);
}

bool CTaiShanReportView::IsBg(int kind)
{
	  if(kind==SHBG|| kind==SZBG)
		  return true;
	  return false;

}

void CTaiShanReportView::GetColumnWith()
{
   	CTaiShanDoc* pDoc = GetDocument();
	for(int i=0;i<MAXCOLUME;i++)
	{
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			continue;

		if(pDoc->m_SystemInitData.ColumnType==0)
	        m_pGrid->SetColumnWidth(i,pDoc->m_SystemInitData.ColumnWidth[i]);
		else
	        m_pGrid->SetColumnWidth(i,pDoc->m_nColumnWidth[i]);
	}     

}

void CTaiShanReportView::SetColumnWidth()
{
   	CTaiShanDoc* pDoc = GetDocument();
	for(int i=0;i<MAXCOLUME;i++)
	{
		int cc=0;
		if(pDoc->m_nColumnIndex[pDoc->m_SystemInitData.ColumnType][i]==-1)
			continue;
		if(pDoc->m_SystemInitData.ColumnType==0)
			pDoc->m_SystemInitData.ColumnWidth[i]=m_pGrid->GetColumnWidth(i);
		else
			pDoc->m_nColumnWidth[i]=m_pGrid->GetColumnWidth(i);
	}

}

void CTaiShanReportView::ChangeToChoose(CTaiShanDoc* pDoc)
{
	SymbolKindArr l_StockArray;
	pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
	int DisplayCount = l_StockArray.GetSize();

	SetShowData(pDoc, DisplayCount);
	pDoc->m_StockTypeMap[0].RemoveAll();


	int index = 0;
	for (int row = 0; row < (int)DisplayCount; row++)
	{
		CReportData* Cdat;
		SymbolKind l_SymbolKind = l_StockArray.GetAt(row);
		if (pDoc->m_sharesInformation.Lookup(l_SymbolKind.m_chSymbol, Cdat, l_SymbolKind.m_nSymbolKind) == TRUE)
		{
			CString Code = Cdat->id;
			CString zqdmkind = pDoc->GetStockKindString(l_SymbolKind.m_nSymbolKind);
			zqdmkind += Code;
			pDoc->m_pStockDataShow[index].pItem = Cdat;
			strcpy(pDoc->m_pStockDataShow[index].StockId, Cdat->id);
			pDoc->m_StockTypeMap[0][zqdmkind] = Code;
			index++;
		}
	}

	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, index, this);

	m_pGrid->SetRealRow(index + 1);
	m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
	m_pGrid->Invalidate();

	UINT TotRow = 0;
	if (DisplayCount > m_nRows)
	{
		TotRow = DisplayCount + 5;

		try
		{
			m_pGrid->SetRowCount(TotRow);
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}

		m_nRows = TotRow;
	}

	pDoc->m_nPreDisplayRow = DisplayCount;
}

void CTaiShanReportView::ChangeToPage(int nPage,BOOL NewTjxg)
{
	if (nPage > TJXGPAGE)
	{
		ASSERT(FALSE);
		nPage = TJXGPAGE;
	}

	m_LoopDisplay = FALSE;
	CTaiShanDoc* pDoc = GetDocument();

	m_pGrid->SetSortColumn(-1 ,TRUE);
	m_pGrid->ClearSortColumn();
	m_pGrid->SetFocus();
	m_pGrid->SetFocusRow(1); 
	m_pGrid->SetMouseState(MOUSE_NOTHINGS);

	pDoc->m_nCurrentPage = nPage;
	m_PageWnd->m_nActivePage = nPage;


	int nType;
	nType = GetStockTypeFromPageWnd(nPage);
	if (nType >= 0)
	{
		SetShowData(pDoc, pDoc->m_sharesInformation.GetStockTypeCount(nType));
	}

	if (nPage == CHOOSEPAGE)
	{
		ChangeToChoose(pDoc);
		return;
	}

	if (nPage == STKTYPEPAGE)
	{
		ChangeToStockType(pDoc, pDoc->m_SystemInitData.StockTypeName);
		return;
	}

	if (nPage == TJXGPAGE)
	{
		if (NewTjxg)
		{
			ChangeToTjxg(pDoc);
		}
		else if (strcmp(pDoc->m_SystemInitData.ScreenStockName, "条件选股") == 0)
		{
			ChangeToTjxg(pDoc, TRUE);
		}
		else
		{
			ChangeToTjxg(pDoc, pDoc->m_SystemInitData.ScreenStockName);
		}
		return;
	}


	m_pGrid->SetRealRow(pDoc->m_sharesInformation.GetStockTypeCount(nType) + 1);
	m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);

	pDoc->m_sharesInformation.RecvStockDataForType(pDoc->m_pStockDataShow, nType);

	CHistoryDlg::DoHistory(pDoc->m_pStockDataShow, pDoc->m_sharesInformation.GetStockTypeCount(nType), this);

	m_pGrid->Invalidate();
	pDoc->m_nPreDisplayRow = pDoc->m_sharesInformation.GetStockTypeCount(nType);
}
