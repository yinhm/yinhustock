// CSharesRealTimeDetail.cpp : implementation file
// by 俞明录

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "CTaiKlineDoFenshi.h"
#include "CSharesDetailGridCtrl.h"
#include "CTaiShanDoc.h"

#include "CSharesRealTimeDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ID_GRID_DIALOG 1999
/////////////////////////////////////////////////////////////////////////////
// CSharesRealTimeDetail dialog


CSharesRealTimeDetail::CSharesRealTimeDetail(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,BOOL IsF1)
	: CDialog(CSharesRealTimeDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSharesRealTimeDetail)
	
	//}}AFX_DATA_INIT
	m_pGrid=NULL;
	m_pView = m_pVwbaseView;
	m_bListMode=TRUE;
	m_nBigBillCount=0;
	m_nRealRow=0;
	m_IsF1=IsF1;
    m_bFirst=TRUE;

}


void CSharesRealTimeDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSharesRealTimeDetail)

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSharesRealTimeDetail, CDialog)
	//{{AFX_MSG_MAP(CSharesRealTimeDetail)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSharesRealTimeDetail message handlers

BOOL CSharesRealTimeDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

 	DWORD dwStyle=0;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE|WS_BORDER ;
	dwStyle &= ~WS_HSCROLL;

	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
	m_pGrid =new CSharesDetailGridCtrl;
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->Create(CRect(0,0,10,10),this,ID_GRID_DIALOG,dwStyle);
  
	m_pGrid->SetListMode(m_bListMode);
	m_pGrid->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_pGrid->SetMainDocument(m_pDoc);

	InitAllTitle();
	InitGridContent();
	int m_Width=0;
	int m_Height=0;
    m_pGrid->SetItemState(1,1, m_pGrid->GetItemState(1,1) | GVIS_READONLY);
	m_pGrid->SetRowHeight(0, int(1.0*m_pGrid->GetRowHeight(0)));
    m_pGrid->AutoSize();
	for(int i=0;i<m_pGrid->GetColumnCount();i++)
	{
		if(m_IsF1)
           m_pGrid->SetColumnWidth(i,100);
		else
           m_pGrid->SetColumnWidth(i,200);
	}
	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetRowResize(FALSE);
	m_pGrid->SetColumnResize(FALSE);
    m_Width=0;
	m_Height=0;
	for(int i=0;i<m_pGrid->GetColumnCount();i++)
        m_Width +=m_pGrid->GetColumnWidth(i);
    m_Width +=18;
    m_Height += int(m_pGrid->GetRowHeight(0)*1.2);
    for(int i=1;i<=20;i++)
        m_Height +=m_pGrid->GetRowHeight(i);

	this->MoveWindow(0,0 ,m_Width+13,m_Height +25,TRUE) ;
    this->CenterWindow();
    this->ShowWindow (SW_SHOW);

    m_pGrid->MoveWindow(3,1,m_Width+3,m_Height-2, TRUE);
	m_pGrid->ShowWindow(SW_SHOW);


    m_pGrid->SetFocus();
    m_pGrid->SetFocusRow(1); 
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);
    m_bFirstDisplay=TRUE;

	((CTaiShanKlineShowView*)m_pView)->pMin1Drawing->InitHs();
    if(m_IsF1 == true)
	  this->ShowStockDetail(m_bFirstDisplay);	
	else
      this->ShowStockDetailF2(m_bFirstDisplay);	
    m_pGrid->SetDisplayMode(m_IsF1);
    m_pGrid->SetViewPoint(m_pView);
    m_bFirstDisplay=FALSE;
	SetTimer(1,3000,NULL);

	
	return TRUE;  
}

void CSharesRealTimeDetail::InitAllTitle()
{
	if(m_IsF1)
 	  m_nRows = 250;
	else
 	  m_nRows = 50;
	m_nFixRows = 1;
	m_nFixCols = 1;
	CString str;
	
    if(m_pDoc->m_sharesSymbol != CSharesCompute::GetIndexSymbol(0) && m_pDoc->m_sharesSymbol != CSharesCompute::GetIndexSymbol(1))
	{
	    if(m_IsF1 == true)
		  SetWindowText("股票成交分笔明细表");
		else
		  SetWindowText("股票成交分价明细表");
        m_ColumnTitleArray.RemoveAll();
		if(m_IsF1)
		{
			str.Format("交易时间");
			m_ColumnTitleArray.Add(str);
			str.Format("成交价格");
			m_ColumnTitleArray.Add(str);
			str.Format("涨跌值");
			m_ColumnTitleArray.Add(str);
			str.Format("成交现手");
			m_ColumnTitleArray.Add(str);
		}else 
		{
			str.Format("成交价格");
			m_ColumnTitleArray.Add(str);
			str.Format("成交手数");
			m_ColumnTitleArray.Add(str);
		}
		m_bFirst=FALSE;
	}
	m_nCols=m_ColumnTitleArray.GetSize(); 
	return ; 
}
int CSharesRealTimeDetail::InitGridCtrl()   
{
	InitAllTitle();
	InitGridContent();
	int m_Width=0;
	int m_Height=0;
	for(int i=0;i<m_pGrid->GetColumnCount();i++)
	{
		if(m_IsF1)
           m_pGrid->SetColumnWidth(i,100);
		else
           m_pGrid->SetColumnWidth(i,200);
	}
    m_pGrid->SetFocus();
    m_pGrid->SetFocusRow(1); 
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);

    m_pGrid->Invalidate();                        
	return 0;
}
BOOL CSharesRealTimeDetail::InitGridContent() 
{
    m_pGrid->SetMainDocument(m_pDoc);
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
   	m_pGrid->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);
    m_pGrid->SetFixedBkColor(m_pDoc->m_colorArray[18]);
    
	CFont Font;
	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -12;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");

	Font.CreateFontIndirect(&m_lf);
	m_pGrid->SetFont(&Font);
	Font.DeleteObject();


	int ColumnCounts=m_ColumnTitleArray.GetSize();
    for(int i=0;i<m_ColumnTitleArray.GetSize();i++)
	{
		CString str=m_ColumnTitleArray.GetAt(i);
        m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),i);
	}
 	return TRUE;
}

void CSharesRealTimeDetail::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CSharesRealTimeDetail::PreTranslateMessage(MSG* pMsg) 
{

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_F2)	
	{
	  if(m_IsF1 == true)
	  {
		  m_IsF1 = false;
		  SetWindowText("股票成交分价明细表");
		  InitGridCtrl();
          m_pGrid->SetDisplayMode(m_IsF1);
  	      ((CTaiShanKlineShowView*)m_pView)->pMin1Drawing->InitHs();
		  m_bFirstDisplay=TRUE;
	      m_nRealRow=0;
		  ShowStockDetailF2(m_bFirstDisplay);
		  m_bFirstDisplay=FALSE;
	  }
	  else
	  {
		  m_IsF1 = true;
		  SetWindowText("股票成交分笔明细表");
		  this->m_bFirst = 0;
		  InitGridCtrl();
  	      ((CTaiShanKlineShowView*)m_pView)->pMin1Drawing->InitHs();
          m_pGrid->SetDisplayMode(m_IsF1);
	      m_bFirstDisplay=TRUE;
	      m_nRealRow=0;
	      ShowStockDetail(m_bFirstDisplay);
		  m_bFirstDisplay=FALSE;

	  }
	  return TRUE;
	}	
	
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		CPoint m_pos;
		m_pos=pMsg->pt;
		ScreenToClient(&m_pos);
		m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);
 		if(m_CellTitleCommand.row==0)
		{
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSharesRealTimeDetail::OnTimer(UINT nIDEvent) 
{
	
  	((CTaiShanKlineShowView*)m_pView)->pMin1Drawing->InitHs();
    if(m_IsF1 == true)
	  this->ShowStockDetail(FALSE);	
	else
      this->ShowStockDetailF2(FALSE);
	m_pGrid->Invalidate();
	
	CDialog::OnTimer(nIDEvent);
}
void CSharesRealTimeDetail::ShowStockDetail(BOOL bFirstDisplay)
{
	int CurrentRow=m_pView->m_hs.GetCount();
	if((m_pDoc->m_sharesSymbol.GetAt(1) == 'A') | (m_pDoc->m_sharesSymbol.GetAt(1) == 'B') | (m_pDoc->m_sharesSymbol.GetAt(1)=='C'))
		return;
	if(CurrentRow == 0)
		return;

	int TotRow=CurrentRow;
	int lAddRows=CurrentRow -m_nRealRow;
    m_nRealRow=CurrentRow;
	if( CurrentRow >= m_nRows) 
	{
		TotRow=TotRow+10;
 		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		m_nRows=TotRow;
	}
    m_pGrid->SetRealRow(CurrentRow+1); // add 1999/4/21
	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
    if(bFirstDisplay)
	{
	    m_pGrid->PostMessage(WM_KEYDOWN,VK_END,0);
		return;
	}
	if(m_nRealRow==0&&CurrentRow>m_VisibleRowCount+m_TopLeftCell.row-2)
	{
		int ls_row=m_pGrid->GetCurrentRow();
		m_pGrid->SetFocusRow(ls_row+lAddRows);
		for(int i=0;i<lAddRows-m_VisibleRowCount;i++)
		   m_pGrid->SendMessage(WM_VSCROLL,SB_LINEDOWN,0);
	}else if(CurrentRow>m_VisibleRowCount+m_TopLeftCell.row-2)
	{
		int ls_row=m_pGrid->GetCurrentRow();
		m_pGrid->SetFocusRow(ls_row+lAddRows);
		for(int i=0;i<lAddRows;i++)
		   m_pGrid->SendMessage(WM_VSCROLL,SB_LINEDOWN,0);
	}

}
void CSharesRealTimeDetail::ShowStockDetailF2(BOOL bFirstDisplay)
{
	int CurrentRow=m_pView->m_fenjia.GetCount();
	if((m_pDoc->m_sharesSymbol.GetAt(1) == 'A') | (m_pDoc->m_sharesSymbol.GetAt(1) == 'B') | (m_pDoc->m_sharesSymbol.GetAt(1)=='C'))
		return;
	if(CurrentRow == 0)
		return;

	int TotRow=CurrentRow;
	int lAddRows=CurrentRow -m_nRealRow;
    m_nRealRow=CurrentRow;
	if( CurrentRow >= m_nRows) 
	{
		TotRow=TotRow+10;
 		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		m_nRows=TotRow;
	}
    m_pGrid->SetRealRow(CurrentRow+1); // add 1999/4/21
	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
    if(bFirstDisplay)
	{
	    m_pGrid->PostMessage(WM_KEYDOWN,VK_END,0);
		return;
	}
	if(m_nRealRow==0&&CurrentRow>m_VisibleRowCount+m_TopLeftCell.row-2)
	{
		int ls_row=m_pGrid->GetCurrentRow();
		m_pGrid->SetFocusRow(ls_row+lAddRows);
		for(int i=0;i<lAddRows-m_VisibleRowCount;i++)
		   m_pGrid->SendMessage(WM_VSCROLL,SB_LINEDOWN,0);
	}else if(CurrentRow>m_VisibleRowCount+m_TopLeftCell.row-2)
	{
		int ls_row=m_pGrid->GetCurrentRow();
		m_pGrid->SetFocusRow(ls_row+lAddRows);
		for(int i=0;i<lAddRows;i++)
		   m_pGrid->SendMessage(WM_VSCROLL,SB_LINEDOWN,0);
	}
}

void CSharesRealTimeDetail::OnClose() 
{

	CDialog::OnClose();
}
void CSharesRealTimeDetail::OnCancel()
{
	if(m_pGrid) delete m_pGrid;
	CDialog::OnCancel();
}
