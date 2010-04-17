/////////////////////////////////////////////////////////
// PageWnd.cpp : implementation file
// this file is designed to show MAX_PAGE page in a wnd
// each page can move in east & west direction
/////////////////////////////////////////////////////////
//
// this file is designed 
// 1999/4/10
// by yml
// Tel:13366898744
// 
///////////////////////////////////////////////////////////
//
// the method to use this class
//
// 1.create an object of CPageWnd
// 2.call CPageWnd::Create(....)
// 3.deal with the message from CPageWnd
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "Memdc.h"
#include "SheetScrollBar.h"
#include "PageWnd.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BOTTOM_HIGHT  16      //the bottom hight
#define DEFAULTFONTWIDTH 10   //Default page title font multiple coeff..

#define IDC_PAGEBUTTON  40021 //the id of the scrollbar
#define IDC_WNDIDNO     40022 //The id of the pagewnd 

#define DEFAULTFORMAT  DT_CENTER|DT_VCENTER|DT_SINGLELINE  //the default text show format
#define SYSTEMHEAD     1000
#define LEFT_LENG     BOTTOM_HIGHT*2

/////////////////////////////////////////////////////////////////////////////
// CPageWnd


void CPageWnd::ExecuteMenu(int mode)
{

	CPoint posMouse;
	::GetCursorPos(&posMouse);
    InitTypeMenu(mode) ;
	m_TypeMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,posMouse.x,posMouse.y,this);
}

CPageWnd::CPageWnd()
{
	//Initialize all parameters
	m_scrollbar = NULL;//new CScrollBar;
    m_nMovebar = 0;
	m_nBarlength = 250;
	m_nBarLen=m_nBarlength;
	m_TypeMenu=NULL;
	m_bMove = FALSE;

}

CPageWnd::~CPageWnd()
{
	m_Font.DeleteObject();
	if(m_TypeMenu) delete m_TypeMenu;
}


BEGIN_MESSAGE_MAP(CPageWnd, CWnd)
	//{{AFX_MSG_MAP(CPageWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////


BOOL CPageWnd::Create(LPCTSTR szTitle, LONG style, const RECT& rect, CWnd* parent) 
{

	LPCTSTR lpszBounceClass =
		AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,//版块
			LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW+1),
			NULL);
	return CWnd::Create(lpszBounceClass, szTitle, style, 
		rect, parent, IDC_WNDIDNO);
}

int CPageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    CRect rect;
	GetClientRect(rect);
	CSize size(2*BOTTOM_HIGHT,BOTTOM_HIGHT);
	m_PageBtn.Create(SBS_HORZ|WS_CHILD|WS_VISIBLE,CRect(CPoint(rect.left,rect.top),size),this,IDC_PAGEBUTTON);
    
	InitAllPara();
	m_TypeMenu = new CMenu;
	HMENU m_hmenu=CreatePopupMenu();
	m_TypeMenu->Attach( m_hmenu );

	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -12;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");


	m_Font.CreateFontIndirect(&m_lf);
	return 0;
}



void CPageWnd::InitAllPara()
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();

	m_nBeginX = BOTTOM_HIGHT * 2;
	m_nActivePage=0;
	m_bRefrash=TRUE;
 	CString str;
	str.Format("上证指数");
	m_strPageTitle.Add(str);
	str.Format("深证指数");

	m_strPageTitle.Add(str);
	str.Format("板块指数");
	m_strPageTitle.Add(str);
	str.Format("自选股票");
	m_strPageTitle.Add(str);
	str="TEST";
	m_strPageTitle.Add(str);
	str="TEST";
	m_strPageTitle.Add(str);
	//init page
	m_nFirstPage=0;
	m_nEndPage=m_strPageTitle.GetSize();
	int i;
	for(i=0;i<m_nEndPage;i++)
	{
		str=m_strPageTitle.GetAt(i);
		if(str.GetLength( )<5)
	      PageWidth[i]=5*DEFAULTFONTWIDTH;
		else
	      PageWidth[i]=str.GetLength( )*DEFAULTFONTWIDTH;
	}
	for(i=m_nEndPage;i<MAX_PAGE;i++)  
		PageWidth[i]=0;
}


BOOL CPageWnd::OnEraseBkgnd(CDC* pDC) 
{

	CClientDC dc(this);
	ShowAllPage((CDC *)&dc);
	ReleaseDC(&dc);
	return 1;
}

void CPageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    m_bRefrash=TRUE; 
 	m_nBeginX = 2*BOTTOM_HIGHT;	

	m_barstartplace.x = cx; 
	

}


void CPageWnd::OnPaint() 
{
	CPaintDC dc(this); 
	if(m_bRefrash)
	{
		m_bRefrash=FALSE;
		ShowAllPage(&dc);
	}
	ReleaseDC(&dc);

}

void CPageWnd::OnLButtonDown(UINT nFlags, CPoint points) 
{	

	if(!DoLButtonDown( nFlags,  points))
		DoRButtonDown( nFlags,  points);


}


void CPageWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	if(pScrollBar->m_hWnd==m_PageBtn.m_hWnd)
	{	
		m_PageBtn.EnableScrollBar(ESB_ENABLE_BOTH);
		switch(nSBCode)
		{
			case SB_LINELEFT: 
				m_bMove = false;
				m_nFirstPage--;
				if(m_nFirstPage<0)
				{
					m_nFirstPage=0;
				
					m_PageBtn.EnableScrollBar(ESB_DISABLE_LTUP);
				}
				else
				{
				
					m_bRefrash=TRUE;
					Invalidate();
				}
				break;
			case SB_LINERIGHT:
				m_bMove = false;
				m_nFirstPage++; 
				if(m_nFirstPage>m_nEndPage-1)
				{
					 m_nFirstPage=m_nEndPage-1;
			
                     m_PageBtn.EnableScrollBar(ESB_DISABLE_RTDN);
				}
				else
				{
				
					m_bRefrash=TRUE;
					Invalidate();
				}
				break;
		}
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


CString CPageWnd::GetPageTitle(int nPage)
{


    CString Result;
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();
    CString ShMenuStr[]={"上证指数","上证A股","上证B股","上证债券","上证基金"};
    CString SzMenuStr[]={"深证指数","深证A股","深证B股","深证债券","深证基金"};
    CString SzEbMenuStr[]={"创业指数","创业股"};
    switch(nPage)
	{
	case  SHPAGE:   switch(pDoc->m_nSharesSh)
					{
						case 0:  Result=ShMenuStr[0];
						         SetPageTitle(Result,SHPAGE);
							     break;
						case 1:  Result=ShMenuStr[1];
						         SetPageTitle(Result,SHPAGE);
							     break;
						case 2:  Result=ShMenuStr[2];
						         SetPageTitle(Result,SHPAGE);
							     break;
						case 3:  Result=ShMenuStr[3];
						         SetPageTitle(Result,SHPAGE);
							     break;
						case 4:  Result=ShMenuStr[4];
						         SetPageTitle(Result,SHPAGE);
							     break;
					}
	                return Result;
		            break;
	case  SZPAGE:   switch(pDoc->m_nSharesSz)
					{
						case 0:  Result=SzMenuStr[0];
						         SetPageTitle(Result,SZPAGE);
							     break;
						case 1:  Result=SzMenuStr[1];
						         SetPageTitle(Result,SZPAGE);
							     break;
						case 2:  Result=SzMenuStr[2];
						         SetPageTitle(Result,SZPAGE);
							     break;
						case 3:  Result=SzMenuStr[3];
						         SetPageTitle(Result,SZPAGE);
							     break;
						case 4:  Result=SzMenuStr[4];
						         SetPageTitle(Result,SZPAGE);
							     break;
					}
	                return Result;
		            break;
	case  SZEBPAGE: switch(pDoc->m_nSharesSzEb)
					{
						case 0:  Result=SzEbMenuStr[0];
						         SetPageTitle(Result,SZEBPAGE);
							     break;
						case 1:  Result=SzEbMenuStr[1];
						         SetPageTitle(Result,SZEBPAGE);
							     break;
					}
	                return Result;
		            break;
	}
    Result=m_strPageTitle.GetAt(nPage);
	return Result;
}


void CPageWnd::ShowPageTitle(CDC *pDC, CString &str, LPRECT lpRect, UINT nFormat,int nPage)
{
	int nBackMode=pDC->GetBkMode();
    if(nBackMode==OPAQUE) pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_Font);
	if(nPage==m_nActivePage)

		pDC->SetTextColor (CMainFrame::m_taiShanDoc ->m_colorArray[13]);

	else
		pDC->SetTextColor (CMainFrame::m_taiShanDoc ->m_colorArray[1]);
 	pDC->DrawText(str,lpRect,nFormat);
	if(nBackMode==OPAQUE) pDC->SetBkMode(OPAQUE);
}


int CPageWnd::GetActivePage()
{
    return m_nActivePage;
}


void CPageWnd::DrawPageBox(CDC *pDC, int x, int cx, BOOL bActive)
{
	CPoint pPage[5];
	double cxB=BOTTOM_HIGHT/3.464;

    CPen BlackPen(PS_SOLID,1,CMainFrame::m_taiShanDoc ->m_colorArray[14]);//17
	CPen *OldPen=pDC->SelectObject(&BlackPen);

    pPage[0]=CPoint(x-cxB,0);
    pPage[1]=CPoint(x+cxB,BOTTOM_HIGHT);
    pPage[2]=CPoint(x+cx-cxB,BOTTOM_HIGHT);
    pPage[3]=CPoint(x+cx+cxB,0); 

	CBrush FillBrush(CMainFrame::m_taiShanDoc ->m_colorArray[18]);
	CBrush FrameBrush(CMainFrame::m_taiShanDoc ->m_colorArray[14]);
	CBrush ActiveBrush(CMainFrame::m_taiShanDoc ->m_colorArray[18]);
	CRgn rgn;
	CPen pen(PS_SOLID,1,CMainFrame::m_taiShanDoc ->m_colorArray[18]);
	if(!bActive)
	{

	}
	else
	{
		rgn.CreatePolygonRgn(pPage,4,ALTERNATE);
	    pDC->FillRgn(&rgn,&ActiveBrush);
		pDC->FrameRgn(&rgn,&FrameBrush,1,1);

		pDC->SelectObject(&pen);
		pDC->MoveTo(pPage[0].x +1,pPage[0].y);
		pDC->LineTo(pPage[3].x -1,pPage[3].y);
		rgn.DeleteObject(); 
	}
	pDC->SelectObject(OldPen);

	pDC->SelectObject(OldPen);
}


void CPageWnd::ShowAllPage(CDC *pDC)
{
	if(!CMainFrame::m_taiShanDoc)
		return;
	
	CDC dcMem;
	CBitmap bmp;
	CRect ClientRect;
	GetClientRect(&ClientRect);
    CRect m_DrawRect;
	m_DrawRect.SetRect(/*LEFT_LENG+*/1 ,ClientRect.top,ClientRect.right,ClientRect.bottom);
    dcMem.CreateCompatibleDC(pDC);
    bmp.CreateCompatibleBitmap(pDC,m_DrawRect.Width(),m_DrawRect.Height()) ;
    CBitmap *pbmOld=dcMem.SelectObject(&bmp);


 	
	CBrush brush(CMainFrame::m_taiShanDoc ->m_colorArray[18]);

    CRect m_Draw;
	m_Draw.SetRect(0 ,0,m_DrawRect.Width(),m_DrawRect.Height());

	dcMem.FillRect(m_Draw,&brush);
	CPen pen(PS_SOLID,1,CMainFrame::m_taiShanDoc ->m_colorArray[14]);//17
	CPen* OldPen = dcMem.SelectObject(&pen);
	dcMem.MoveTo(m_DrawRect.left ,m_DrawRect.top );
	dcMem.LineTo(m_DrawRect.right ,m_DrawRect.top);
	dcMem.SelectObject(OldPen);
	
    int nPage=m_nFirstPage,bActivePage=-1;
    CString Title;
	CRect rect,bActiveRect;
	int bActivex=-1;

	int xBegin=0 ;

	
	m_nBeginX=2*BOTTOM_HIGHT;

	int beginX = PageWidth[nPage] + PageWidth[nPage+1];
	while(nPage < m_nActivePage && m_bMove == true)
	{
	  beginX += PageWidth[nPage+2];
      if(beginX >= m_barstartplace.x)
	  {
		  nPage = m_nFirstPage+1;
          m_nFirstPage = nPage;
		  m_bMove=FALSE;
		  break;
	  }
	  nPage++;
	}
	CString tt;
	tt.Format("%d",m_nFirstPage);
	TRACE(tt);
	if(beginX < m_barstartplace.x)
	{
	   nPage=m_nFirstPage;	
	}
	////////////////////////////////////////

    if(m_nActivePage == 0 && m_bMove == true)
	{
	  nPage = 0;
	  m_nFirstPage = 0;
	}
	while(nPage<m_nEndPage)
	{
	
		Title=GetPageTitle(nPage);
	
		DrawPageBox(&dcMem,xBegin,PageWidth[nPage],FALSE);
		rect.SetRect(xBegin,0,xBegin+PageWidth[nPage],BOTTOM_HIGHT);
	
		ShowPageTitle(&dcMem,Title,rect,DEFAULTFORMAT,nPage);
		if(nPage==m_nActivePage)
		{
			bActivex=xBegin;
			bActiveRect=rect;
			bActivePage=nPage;
		}
	
		xBegin+=PageWidth[nPage];
		if(xBegin>ClientRect.right)break;
		nPage++;
	}
	if(bActivex>=0) 
	{
		Title=GetPageTitle(bActivePage);
		DrawPageBox(&dcMem,bActivex,PageWidth[bActivePage],TRUE);
		ShowPageTitle(&dcMem,Title,bActiveRect,DEFAULTFORMAT,bActivePage);
	}
   //////////////
	
	
	
	
	
	////////////////////////////////////////////////////
	pDC->BitBlt(LEFT_LENG+1 ,0,ClientRect.right,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pbmOld);
	

}


void CPageWnd::ReplacePage(int nPage,CString ReplaceTitle)
{
 
	int xBegin=m_nBeginX;
	CRect rect;
	CClientDC dc(this);    
	m_strPageTitle.SetAt(nPage,ReplaceTitle);
    int ss=ReplaceTitle.GetLength( );
    if(ReplaceTitle.GetLength( )<5)
       PageWidth[nPage]=5*DEFAULTFONTWIDTH;
	else
	   PageWidth[nPage]=ReplaceTitle.GetLength( )*DEFAULTFONTWIDTH;
 	ShowAllPage(&dc);
	ReleaseDC(&dc);
}


void CPageWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	DoRButtonDown( nFlags,  point);
	CWnd::OnRButtonDown(nFlags, point);
}


void CPageWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
   if( m_nMovebar == 1)
   {
	  SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
	  SetCapture();

	  point.x = point.x + 3;
	  m_nBarlength = m_nBarlength + (-point.x + m_barstartplace.x);

	  if(point.x <33)
		  point.x=33;
	  if(point.x +50 > m_barendplace.x)
         point.x= m_barendplace.x -50;
	  m_nBarlength = m_nBarlength + (-point.x + m_barstartplace.x);

      MSG msg;

	  SetWindowPos(0,0,m_barstartplace.y,point.x,m_barendplace.y,SWP_NOREDRAW|SWP_SHOWWINDOW);
	  m_bRefrash=TRUE;
	  Invalidate();
	  if(m_nBarLen > m_nBarlength)
	  if(PeekMessage(&msg,this->GetSafeHwnd() ,WM_PAINT,WM_PAINT,PM_REMOVE))
	  {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	  }

      CRect rc(point.x,m_barstartplace.y,m_barendplace.x,m_barendplace.y);
	  m_scrollbar->SetWindowPos(0,point.x,m_barstartplace.y,rc.Width() ,rc.Height(),SWP_NOREDRAW);
	  m_scrollbar->m_bRefrash=TRUE;
	  m_scrollbar->Invalidate();
	  if(PeekMessage(&msg,m_scrollbar->GetSafeHwnd() ,WM_PAINT,WM_PAINT,PM_REMOVE))
	  {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	  }

	  m_barstartplace.x = point.x;
      m_nBarLen = m_nBarlength;

   }
   else
   {

     if(point.x > m_barstartplace.x - 5)
	 {
	   SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
       
	 }
     else
	 { 
        m_nMovebar = 0;
	 }
   }
   CWnd::OnMouseMove(nFlags, point);
}

void CPageWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_nMovebar = 0;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}
void CPageWnd::InitTypeMenu(int mode) 
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();
    CString ShMenuStr[]={"上证指数","上证A股","上证B股","上证债券","上证基金"};
    CString SzMenuStr[]={"深证指数","深证A股","深证B股","深证债券","深证基金"};
    CString SzEbMenuStr[]={"创业指数","创业股"};
	


    POSITION pos=m_StockType.GetHeadPosition( );
	while(pos)
	{
	    DynaMenuDef *m_pMenuDef;
		m_pMenuDef = m_StockType.GetNext( pos );
        m_TypeMenu->DeleteMenu(m_pMenuDef->m_nID,MF_BYCOMMAND);
		delete m_pMenuDef;
	}
    m_StockType.RemoveAll();

	int index=9000;
	int index2[]={9000,9001,9002,9003,9004};
	switch(mode)
	{
	case 0:
		{
			CStringArray m_StockTypeNameArray;
			pDoc->m_ManagerStockTypeData.GetAllStockTypeName(m_StockTypeNameArray);
			int l_nCount=m_StockTypeNameArray.GetSize();
			for(int i=0;i<l_nCount;i++)
			{
			    CString l_sStockType=m_StockTypeNameArray.GetAt(i);
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=l_sStockType;
				m_pMenuDef->m_nID=index++;
				m_StockType.AddTail(m_pMenuDef);
			}
		}
		break;
	case 1:
		{
			pos=pDoc->m_Tjxgblocklist.GetHeadPosition();
			while(pos)
			{
				BLOCKSTOCK * blocknow=pDoc->m_Tjxgblocklist.GetNext(pos);
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=blocknow->blockname;
				m_pMenuDef->m_nID=index++;
				m_StockType.AddTail(m_pMenuDef);
			}
		}
		break;
	case 2:
		{
			for(int i=0;i<5;i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=ShMenuStr[i];
				m_pMenuDef->m_nID=index2[i];
				m_StockType.AddTail(m_pMenuDef);
			}
		}
		break;
	case 3:
		{
			for(int i=0;i<5;i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=SzMenuStr[i];
				m_pMenuDef->m_nID=index2[i];
				m_StockType.AddTail(m_pMenuDef);
			}
		}
		break;
	case 4:
		{
			for(int i=0;i<5;i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=SzEbMenuStr[i];
				m_pMenuDef->m_nID=index++;
				m_StockType.AddTail(m_pMenuDef);
			}
		}
		break;
	}
    pos=m_StockType.GetHeadPosition( );
	index=0;
	while(pos)
	{
	    DynaMenuDef *m_pMenuDef;
		m_pMenuDef = m_StockType.GetNext( pos );
		if(index%10==0&&index!=0)
			m_TypeMenu->AppendMenu(MF_STRING|MF_MENUBARBREAK ,
		         m_pMenuDef->m_nID, m_pMenuDef->m_nString);
		else
            m_TypeMenu->AppendMenu(MF_STRING,
		         m_pMenuDef->m_nID, m_pMenuDef->m_nString);
		index++;
	}
}

BOOL CPageWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra,
							AFX_CMDHANDLERINFO* pHandlerInfo)
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();

	if (pHandlerInfo == NULL)
	{
		if(nCode == CN_COMMAND)
		{
			POSITION pos=m_StockType.GetHeadPosition( );
			while(pos)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef = m_StockType.GetNext( pos );
				if(m_pMenuDef->m_nID==nID)
				{
				  if(GetActivePage()==m_nEndPage-2)
				  {
					 CString sStockTypeChoose;
				     sStockTypeChoose=m_pMenuDef->m_nString;
					 strcpy(pDoc->m_SystemInitData.StockTypeName,m_pMenuDef->m_nString.GetBuffer(0));
				     pView->ChangeToStockType(pDoc, sStockTypeChoose);
				  }
				  else if(GetActivePage()==m_nEndPage-1)
				  {
				     CString sStockScreenChoose=m_pMenuDef->m_nString;
                     strcpy(pDoc->m_SystemInitData.ScreenStockName,m_pMenuDef->m_nString.GetBuffer(0));
				     pView->ChangeToTjxg(pDoc, sStockScreenChoose);
				  }
				  else if(GetActivePage()==m_nFirstPage)
				  {
				     pDoc->m_nSharesSh=m_pMenuDef->m_nID-9000;
				     pView->ChangeToPage(m_nActivePage);;
				  }
				  else if(GetActivePage()==m_nFirstPage+1)
				  {
				     pDoc->m_nSharesSz=m_pMenuDef->m_nID-9000;
				     pView->ChangeToPage(m_nActivePage);
				  }
				  else if(GetActivePage()==m_nFirstPage+2)
				  {
				     pDoc->m_nSharesSzEb=m_pMenuDef->m_nID-9000;
				     pView->ChangeToPage(m_nActivePage);
				  }
				  ReplacePage(GetActivePage(),m_pMenuDef->m_nString);
				}
			}
		}
	
	}
	
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CPageWnd::SetPageTitle(CString m_title,int index)
{
    m_strPageTitle.SetAt(index,m_title);
	CString	str=m_strPageTitle.GetAt(index);
	int temp=str.GetLength( )*DEFAULTFONTWIDTH;
	if(temp<5*DEFAULTFONTWIDTH)
		temp=5*DEFAULTFONTWIDTH;
    PageWidth[index]=temp;
}

void CPageWnd::DoRButtonDown(UINT nFlags, CPoint point)
{
	int nSplitx=m_nBeginX;
	for(int i=m_nFirstPage;i<m_nEndPage;i++)
	{
		
		if((point.x >nSplitx) && (point.x <nSplitx+PageWidth[i]))
		{
			if(i!=GetActivePage())
				DoLButtonDown( nFlags,  point);
			if(i==STKTYPEPAGE)
			{
	            ExecuteMenu(0);
				break;
			}
			else 
			if(i==TJXGPAGE)
			{
	            ExecuteMenu(1);
				break;
			}
			else if(i==SHPAGE)
			{
	            ExecuteMenu(2);
				break;
			}
			else if(i==SZPAGE)
			{
	            ExecuteMenu(3);
				break;
			}

		}
		nSplitx+=PageWidth[i];
	}
}

bool CPageWnd::DoLButtonDown(UINT nFlags, CPoint points)
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
	CClientDC   dc(this);

    if(points.x > m_barstartplace.x - 5)
	{
	   SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
       m_nMovebar = 1;
    }
	else
	{
	  m_nMovebar = 0;
      int nSplitx=m_nBeginX;
	  for(int i=m_nFirstPage;i<m_nEndPage;i++)
	  {
	
		if((points.x >nSplitx) && (points.x <nSplitx+PageWidth[i]))
		{
			if(i!=m_nActivePage)
			{
			
                if((nSplitx+PageWidth[i])>m_barstartplace.x)
					m_bMove=TRUE;
				else
					m_bMove=FALSE;


				m_nActivePage=i; 
                ShowAllPage((CDC*)&dc);				
                
 				pView->PostMessage(WM_SHOWTYPE,0,0);
				pView->ChangeToPage(m_nActivePage);
 				return true; 
			}
		}
		nSplitx+=PageWidth[i]; 
	  }
	}
	return false;
}
