// RealTimeDetailList.cpp : implementation file
//tel:13366898744
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "DetailGridCtrl.h"
#include "CTaiShanDoc.h"

#include "RealTimeDetailList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define ID_GRID_DIALOG 8787
/////////////////////////////////////////////////////////////////////////////
// CRealTimeDetailList dialog

CRealTimeDetailList::CRealTimeDetailList(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,BOOL IsF1)
	: CDialog(CRealTimeDetailList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRealTimeDetailList)
	
	//}}AFX_DATA_INIT
	m_pGrid=NULL;
	m_pView = m_pVwbaseView;
	m_bListMode=TRUE;
	m_nBigBillCount=0;
	m_nRealRow=0;
	m_IsF1=IsF1;
    m_bFirst=TRUE;
    m_bFirstDisplay=TRUE;
	m_nRealRow=0;

}


void CRealTimeDetailList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealTimeDetailList)
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealTimeDetailList, CDialog)
	//{{AFX_MSG_MAP(CRealTimeDetailList)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CRealTimeDetailList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

 	DWORD dwStyle=0;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE|WS_BORDER ;//检测该股票是否已记录在内存模板中
	dwStyle &= ~WS_HSCROLL;

	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
	m_pGrid =new CDetailGridCtrl;
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
    m_pGrid->SetColumnWidth(0,80);
    m_pGrid->SetColumnWidth(1,80);
    m_pGrid->SetColumnWidth(2,60);
    m_pGrid->SetColumnWidth(3,60);
    m_pGrid->SetColumnWidth(4,60);
    m_pGrid->SetColumnWidth(5,60);
    m_pGrid->SetColumnWidth(6,80);
    m_pGrid->SetColumnWidth(7,70);
    m_pGrid->SetColumnWidth(8,80);
    m_pGrid->SetColumnWidth(9,70);

	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetRowResize(FALSE);
	m_pGrid->SetColumnResize(FALSE);

    m_Width=0;
	m_Height=0;
	for(int i=0;i<10;i++)
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
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);//检测该股票是否已记录在内存模板中

    m_pGrid->Invalidate();                          
    if(m_IsF1 == true)
	   Show_1Min(TRUE);
	if(m_IsF1 == false)
       Show_5Min(TRUE);
    m_pGrid->SetDisplayMode(m_IsF1);
    m_bFirstDisplay=FALSE;
	SetTimer(1,3000,NULL);
	return TRUE; 
}
void CRealTimeDetailList::InitAllTitle()
{

	if(m_IsF1)
 	  m_nRows = 250;
	else
 	  m_nRows = 52;
	m_nFixRows = 1;
	m_nFixCols = 1;
	CString str;
	
    if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) || m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1))
	{
	    if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) && m_IsF1 == true)
		  SetWindowText("上证指数一分钟成交明细");
		if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1) && m_IsF1 == true)
          SetWindowText("深圳成指一分钟成交明细");
        if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) && m_IsF1 == false)
		  SetWindowText("上证指数五分钟成交明细");
		if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1) && m_IsF1 == false)
          SetWindowText("深圳成指五分钟成交明细");
		if(m_bFirst == TRUE)
		{
				str.Format("交易时间");
				m_ColumnTitleArray.Add(str);
				if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0))
				{
					str.Format("上证指数");
					m_ColumnTitleArray.Add(str);
				}
				else
				{
					str.Format("深圳成指");
					m_ColumnTitleArray.Add(str);
				}
				str.Format("涨跌值");
				m_ColumnTitleArray.Add(str);
				str.Format("涨跌幅");
				m_ColumnTitleArray.Add(str);
				str.Format("涨数");
				m_ColumnTitleArray.Add(str);
				str.Format("跌数");
				m_ColumnTitleArray.Add(str);
				str.Format("累积成交值");
				m_ColumnTitleArray.Add(str);
				str.Format("成交值");
				m_ColumnTitleArray.Add(str);
				str.Format("累积成交量");
				m_ColumnTitleArray.Add(str);
				str.Format("成交量");
				m_ColumnTitleArray.Add(str);
		}
		m_bFirst=FALSE;
	}
	m_nCols=m_ColumnTitleArray.GetSize(); 
	return ;  
}
int CRealTimeDetailList::InitGridCtrl()   	
{

	return 0;
}
BOOL CRealTimeDetailList::InitGridContent()  
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
	m_pGrid->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);//字体
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


    for(int i=0;i<m_ColumnTitleArray.GetSize();i++)
	{
		CString str=m_ColumnTitleArray.GetAt(i);
        m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),i);
	}
 	return TRUE;
}

void CRealTimeDetailList::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	

	
}

BOOL CRealTimeDetailList::PreTranslateMessage(MSG* pMsg) 
{

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_F2)	
	{
	  if(m_IsF1 == true)
	  {
		  m_IsF1 = false;
          m_pGrid->SetDisplayMode(m_IsF1);
		  m_bFirstDisplay=TRUE;
	      m_nRealRow=0;
	      Show_5Min(m_bFirstDisplay);
		  m_bFirstDisplay=FALSE;
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) && m_IsF1 == true)
		    SetWindowText("上证指数一分钟成交明细");
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1) && m_IsF1 == true)
		    SetWindowText("深圳成指一分钟成交明细");
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) && m_IsF1 == false)
		    SetWindowText("上证指数五分钟成交明细");
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1) && m_IsF1 == false)
		    SetWindowText("深圳成指五分钟成交明细");
	  }
	  else
	  {
		  m_IsF1 = true;
          m_pGrid->SetDisplayMode(m_IsF1);
	      m_bFirstDisplay=TRUE;
	      m_nRealRow=0;
	      Show_1Min(m_bFirstDisplay);
		  m_bFirstDisplay=FALSE;
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) && m_IsF1 == true)
		    SetWindowText("上证指数一分钟成交明细");
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1) && m_IsF1 == true)
		    SetWindowText("深圳成指一分钟成交明细");
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0) && m_IsF1 == false)
		    SetWindowText("上证指数五分钟成交明细");
		  if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1) && m_IsF1 == false)
		    SetWindowText("深圳成指五分钟成交明细");
	  }
	  return true;
	}	
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		CPoint m_pos;
	    int nRow=m_pGrid->GetCurrentRow();
		m_pos=pMsg->pt;
		m_pGrid->ScreenToClient(&m_pos);
		m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);
 		if(m_CellTitleCommand.row==0)
		{
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CRealTimeDetailList::Show_1Min(BOOL IsFirstDisplay)
{
	int CurrentRow;
    if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0))
       CurrentRow=m_pDoc->m_nANT[0]+1;
	else if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1))
       CurrentRow=m_pDoc->m_nANT[1]+1;

	int TotRow=CurrentRow;
	int lAddRows=CurrentRow -m_nRealRow;
    if(lAddRows==0)
	{
       m_pGrid->Invalidate();
	   return;
	}
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
    m_pGrid->SetRealRow(CurrentRow+1);
	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
    if(IsFirstDisplay)
	{
	    m_pGrid->PostMessage(WM_KEYDOWN,VK_END,0);
		return;
	}
	if(m_nRealRow==0&&CurrentRow>m_VisibleRowCount+m_TopLeftCell.row-2)
	{// NOTE: the ClassWizard will add member functions here
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
void CRealTimeDetailList::Show_5Min(BOOL IsFirstDisplay)
{// NOTE: the ClassWizard will add member functions here
	int CurrentRow;
	int tempRow;
    if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(0))
	{
       tempRow=m_pDoc->m_nANT[0]%5;
	   if(tempRow==0)
           CurrentRow=(int)m_pDoc->m_nANT[0]/5+1;
	   else
           CurrentRow=(int)m_pDoc->m_nANT[0]/5+2;
	}
	else if(m_pDoc->m_sharesSymbol == CSharesCompute::GetIndexSymbol(1))
	{
       tempRow=m_pDoc->m_nANT[1]%5;
	   if(tempRow==0)
           CurrentRow=(int)m_pDoc->m_nANT[1]/5;
	   else
           CurrentRow=(int)m_pDoc->m_nANT[1]/5+1;
	}
	int TotRow=CurrentRow;
	int lAddRows=CurrentRow -m_nRealRow;
    if(lAddRows==0)
	{
       m_pGrid->Invalidate();
	   return;
	}
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
    m_pGrid->SetRealRow(CurrentRow+1); 
	CCellID m_TopLeftCell=m_pGrid->GetTopleftNonFixedCell();
    if(IsFirstDisplay)
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


void CRealTimeDetailList::OnTimer(UINT nIDEvent) 
{// NOTE: the ClassWizard will add member functions here

    if(m_IsF1 == true)
	   this->Show_1Min(FALSE);	
	if(m_IsF1 == false)
       this->Show_5Min(FALSE);
	m_pGrid->Invalidate();
	CDialog::OnTimer(nIDEvent);
}

void CRealTimeDetailList::OnClose() 
{

	CDialog::OnClose();
}
void CRealTimeDetailList::OnCancel()
{
	if(m_pGrid) delete m_pGrid;
	CDialog::OnCancel();
}
