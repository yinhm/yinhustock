// DiaStockdetail.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DiaStockdetail.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "CTaiShanDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDiaStockdetail::CDiaStockdetail(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,bool IsF1)
	: CDialog(CDiaStockdetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaStockdetail)
	
	//}}AFX_DATA_INIT
	m_pView = m_pVwbaseView;
	prenum = 0;
	prenumF2 = 0;
	first = 0;
    m_IsF1 = IsF1;
    num = 0;
}



void CDiaStockdetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaStockdetail)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaStockdetail, CDialog)
	//{{AFX_MSG_MAP(CDiaStockdetail)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CDiaStockdetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rect;
    iRootItem = 0;
	iRootItem1 = 0;

	CTaiShanDoc *pDoc = (((CTaiShanKlineShowView*)m_pView))->GetDocument();	
	m_list.GetWindowRect(&rect);
	m_list.SetBkColor(pDoc->m_colorArray[0]);
	if(((CTaiShanKlineShowView*)m_pView)->m_sharesSymbol != CSharesCompute::GetIndexSymbol(0) && ((CTaiShanKlineShowView*)m_pView)->m_sharesSymbol != CSharesCompute::GetIndexSymbol(1))
	{
		if(m_IsF1 == true)
		{
			int width = (rect.Width()-10)/4;
			m_list.InsertColumn(0,"时间",LVCFMT_LEFT,width);
			m_list.InsertColumn(1,"成交价",LVCFMT_LEFT,width);
			m_list.InsertColumn(2,"涨跌值",LVCFMT_LEFT,width);
			m_list.InsertColumn(3,"现手",LVCFMT_LEFT,width);
			m_list.DeleteAllItems();
		}
		else
		{
			int width = rect.Width()/2;
			m_list.InsertColumn(0,"成交价",LVCFMT_LEFT,width);
			m_list.InsertColumn(1,"分手",LVCFMT_LEFT,width);
			m_list.DeleteAllItems();
		}
	
		m_list.pDoc = pDoc;

		if(m_IsF1 == true)
		{ 
			m_list.m_IsF1 = true;
			this->ShowStockDetail();
		}
		else
		{
			m_list.m_IsF1 = false;
			this->ShowStockDetailF2();
		}
		  SetTimer(1,1000,NULL);
	}
	
	
	return TRUE;  
}

void  CDiaStockdetail::ShowStockDetail()
{

} 

void CDiaStockdetail::OnTimer(UINT nIDEvent) 
{
    if(m_IsF1 == true)
	  this->ShowStockDetail();	
	else
      this->ShowStockDetailF2();	
	
	CDialog::OnTimer(nIDEvent);
}

void  CDiaStockdetail::ShowStockDetailF2()
{
  	(((CTaiShanKlineShowView*)m_pView))->pMin1Drawing->InitHs();
	CTaiShanDoc *pDoc = (((CTaiShanKlineShowView*)m_pView))->GetDocument();

	
	int iSubItem = 1;
	int iRootItem1 = 0;
	
	num = ((CTaiShanKlineShowView*)m_pView)->m_fenjia.GetCount();
	if((((CTaiShanKlineShowView*)m_pView)->m_sharesSymbol.GetAt(1) == 'A') | (((CTaiShanKlineShowView*)m_pView)->m_sharesSymbol.GetAt(1) == 'B') | (((CTaiShanKlineShowView*)m_pView)->m_sharesSymbol.GetAt(1)=='C'))
		return;
	if(num == 0)
		return;
    
	if(num <= prenumF2)
	{
	   m_list.num = num;
	   for(int j=0;j<num;j++)
	   {
	      POSITION pos;
		  pos = ((CTaiShanKlineShowView*)m_pView)->m_fenjia.FindIndex(j);

          FENJIA* pFenJia = ((CTaiShanKlineShowView*)m_pView)->m_fenjia.GetAt( pos );
          CString str = m_list.GetItemText(j,1);
          if(atoi(str) == int(fabs(pFenJia->vol)))
			  continue;
          str.Format("%d",(int)fabs(pFenJia->vol));
		  m_list.SetItemText(j,1,str);
	   }
	
	}
	if(num >prenumF2)
	{
		m_list.DeleteAllItems();
		for(int j=0;j<num;j++)
		{
			POSITION pos;
			pos=((CTaiShanKlineShowView*)m_pView)->m_fenjia.FindIndex(num - j -1 ) ;

			
			FENJIA* pFenJia = ((CTaiShanKlineShowView*)m_pView)->m_fenjia.GetAt( pos );
			CString str;
            
			//
		    if(CSharesCompute::GetMarketKind(((CTaiShanKlineShowView*)m_pView)->m_stkKind) == SH_MARKET_EX && m_pView->pMin1Drawing->m_pReportData->kind == 2)
			  str.Format("%.3f",pFenJia->price);
			else
              str.Format("%.2f",pFenJia->price);
			if(pFenJia->price - m_pView->pMin1Drawing->m_close > 0)
			   m_list.RedOrGreen[num-j-1] = 1;
			else if(pFenJia->price - m_pView->pMin1Drawing->m_close < 0)
			   m_list.RedOrGreen[num-j-1] = 2;
			else if(pFenJia->price - m_pView->pMin1Drawing->m_close == 0)
				m_list.RedOrGreen[num-j-1] = 3;
			m_list.InsertItem( iRootItem1, str);
			str.Format("%d",(int)fabs(pFenJia->vol));
			m_list.SetItemText( iRootItem1, iSubItem++, str);
			iSubItem = 1;
		}
    }
	prenumF2 = num;
	first = 1;
}


BOOL CDiaStockdetail::PreTranslateMessage(MSG* pMsg) 
{

    if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_F1)	
	{
	  if(m_IsF1 == true)
		  return false;
	  else
	  {
		  m_IsF1 = true;
		  this->prenum = 0;
		  this->num = 0;
		  this->first = 0;
	      for(int i = 1;i >= 0;i--)
		  {
			  m_list.DeleteColumn(i);
		  }
		  this->OnInitDialog();
		  this->Invalidate();
	  }
	}	
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_F2)	
	{
	  if(m_IsF1 == false)
		  return false;
	  else
	  {
		  this->prenumF2 = 0;
		  this->num = 0;
		  m_IsF1 = false;
	      for(int i = 3;i >= 0;i--)
		  {
			  m_list.DeleteColumn(i);
		  }
		  this->OnInitDialog();
		  this->Invalidate();
	  }
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDiaStockdetail::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if(nChar == VK_F1)	
	{
	  if(m_IsF1 == true)
		  return;
	  else
	  {
		  m_IsF1 = true;
	      this->first = 0;
		  m_list.DeleteAllItems();
		  for(int i = 3;i >= 0;i--)
		  {
			  m_list.DeleteColumn(i);
		  }
		  this->OnInitDialog();
		  this->Invalidate();
	  }
	}	
	
	if(nChar == VK_F2)	
	{
	  if(m_IsF1 == false)
		  return;
	  else
	  {
		  m_IsF1 = false;
		  m_list.DeleteAllItems();
		  for(int i = 1;i >= 0;i--)
		  {
			  m_list.DeleteColumn(i);
		  }
	      this->OnInitDialog();
		  this->Invalidate();
	  }
	}
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
