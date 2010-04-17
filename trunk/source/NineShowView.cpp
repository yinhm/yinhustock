// NineShowView.cpp : implementation of the CNineShowView class
//

#include "stdafx.h"
#include "MainFrm.h"

#include "math.h"
#include "stdlib.h"

#include "CTaiShanApp.h"

#include "CTaiShanDoc.h"
#include "NineGridCtrl.h"
#include "NineShowFrm.h"

#include "NineShowView.h"
#include "keybrodeangel.h"
#include "cellRange.h"
#include "SetTimeInter.h"
#include "CTaiShanReportView.h"
#include "NineShowView.h"
#include "CTaiShanKlineShowView.h"

#include "ListTool.h"
#define ID_NINEGRID_MXDIALOG 19989

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define TOPLEFT      22  

#define FIRSTKEYFLAG  80

#define ADDINTER 4
#define TITLEBACK RGB(192,192,192)

extern BOOL QuickSort(STOCKDATASHOW *m_pStockData,BOOL bAscending,int low,int high);

int MenuNo1;
static void ExecuteMenu(CWnd* wnd,UINT id,BOOL bIsDays=TRUE,BOOL bIsSync=FALSE)
{
	CMenu popMenu;
    CWnd* pwnd;
    if (wnd == NULL)
       pwnd = AfxGetMainWnd();
    else pwnd = wnd;
	   popMenu.LoadMenu(id);
	CPoint posMouse;
	::GetCursorPos(&posMouse);
	if(id==IDR_STOCKTYPE)
	{
#ifdef WIDE_NET_VERSION
       popMenu.EnableMenuItem(ID_STOCKTYPE,MF_GRAYED|MF_BYCOMMAND);
#endif
	   if(MenuNo1<6)
	     popMenu.CheckMenuItem(ID_SHSTOCKA+MenuNo1,MF_CHECKED|MF_BYCOMMAND);
	   else if(MenuNo1==6)
	     popMenu.CheckMenuItem(ID_STOCKTYPE,MF_CHECKED|MF_BYCOMMAND);
       else if(MenuNo1==7)
	     popMenu.CheckMenuItem(ID_SZEB,MF_CHECKED|MF_BYCOMMAND);
	   if(bIsSync)
	      popMenu.CheckMenuItem(ID_SYNC,MF_CHECKED|MF_BYCOMMAND);
	}
	else
	{
	   popMenu.CheckMenuItem(IDD_TOTP+MenuNo1,MF_CHECKED|MF_BYCOMMAND);
#ifdef WIDE_NET_VERSION
       popMenu.EnableMenuItem(ID_SETTIMEINTER,MF_GRAYED|MF_BYCOMMAND);
       popMenu.EnableMenuItem(IDD_HLPFM,MF_GRAYED|MF_BYCOMMAND);
#else
	   if(bIsDays)
	      popMenu.EnableMenuItem(ID_SETTIMEINTER,MF_GRAYED|MF_BYCOMMAND);
#endif
    }
	popMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,posMouse.x,posMouse.y,pwnd);
}



IMPLEMENT_DYNCREATE(CNineShowView, CView)

BEGIN_MESSAGE_MAP(CNineShowView, CView)
	//{{AFX_MSG_MAP(CNineShowView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SYNC, OnSync)
	ON_COMMAND(ID_SZEB, OnSzeb)
	ON_COMMAND(ID_STOCKTYPE, OnStocktype)
	ON_UPDATE_COMMAND_UI(ID_SYNC, OnUpdateSync)
	ON_UPDATE_COMMAND_UI(ID_STOCKTYPE, OnUpdateStocktype)
	ON_UPDATE_COMMAND_UI(ID_SZEB, OnUpdateSzeb)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_SETTIMEINTER,OnSetTimeInter)
 	ON_COMMAND_RANGE(ID_SHSTOCKA,ID_SHZHZHAIQUAN,OnStockTypeChoose)
 	ON_UPDATE_COMMAND_UI_RANGE(ID_SHSTOCKA,ID_SHZHZHAIQUAN,OnUpdateShstocka)
 	ON_COMMAND_RANGE(IDD_TOTP,IDD_ADVSP,OnTechniqueIndexChoose)
	ON_MESSAGE(WM_REFRESHNINESHOW,OnRefreshNineShow)
 	ON_MESSAGE(WM_KEYBOARDGENIUS,OnKeyboardGenius)
	ON_MESSAGE(WM_USER_NINESHOW_ROW_CHANGED,OnNineShowRowChanged)
    ON_MESSAGE(WM_USER_CHANGECOLOR,OnGridColorChange)




END_MESSAGE_MAP()



CNineShowView::CNineShowView()
{
	
	m_nTimeInter=5;
	m_nSharesType=0; 
	m_nActiveList=0;
	m_nActiveItem=0;

	m_BackColor=BACKGROUNDCOLOR;
	m_TitleColor=RGB(0,10,150);
	m_StockNameColor=RGB(0,0,0);
	m_OtherDataColor=RGB(255,255,0);
	m_StockUpColor=RGB(255,0,255);
	m_StockDownColor=RGB(128,128,128);
	m_StockStopColor=RGB(255,64,0);
	m_CursorColor = RGB(0,0,168);
	
    m_nShowStockCount=0;
	m_nFocusCount=0;
    m_bSync=FALSE;
    CString str="总额最小排名";
	this->m_typetitle.Add(str);
	str="震幅最小排名";
	this->m_typetitle.Add(str);
    str="跌幅排名";
	this->m_typetitle.Add(str);
    str="委比最小排名";
	this->m_typetitle.Add(str);
    str="量比最小排名";
	this->m_typetitle.Add(str);
    str="外内盘最小比值排名";
	this->m_typetitle.Add(str);
    str="换手率最小排名";
	this->m_typetitle.Add(str);
	str="市盈率最小排名";
	this->m_typetitle.Add(str);
    str="分钟资金最小流向排名";
	this->m_typetitle.Add(str);
    str="分钟分时震幅最小排名";
	this->m_typetitle.Add(str);
    str="分钟换手速率最小排名";
	this->m_typetitle.Add(str);
    str="分钟跌幅排名";
	this->m_typetitle.Add(str);

    str="总额最大排名";
	this->m_typetitle.Add(str);
	str="震幅最大排名";
	this->m_typetitle.Add(str);
    str="涨幅排名";
	this->m_typetitle.Add(str);
    str="委比最大排名";
	this->m_typetitle.Add(str);
    str="量比最大排名";
	this->m_typetitle.Add(str);
    str="外内盘最大比值排名";
	this->m_typetitle.Add(str);
    str="换手率最大排名";
	this->m_typetitle.Add(str);
	str="市盈率最大排名";
	this->m_typetitle.Add(str);
    str="分钟资金最大流向排名";
	this->m_typetitle.Add(str);
    str="分钟分时震幅最大排名";
	this->m_typetitle.Add(str);
    str="分钟换手速率最大排名";
	this->m_typetitle.Add(str);
    str="分钟涨幅排名";
	this->m_typetitle.Add(str);


	IsQuit = 0;
	FirstMenu=0;
	m_unSaveMousePoint=0;
	m_nShowStockCount=0;
	m_nFocusCount=0;

    m_bSync=FALSE;
#ifdef WIDE_NET_VERSION
	m_SocketID.hWnd=NULL;
    m_SocketID.iFlag=-1;
#endif

}

CNineShowView::~CNineShowView()
{
	CTaiShanDoc *pDoc=(CTaiShanDoc *)GetDocument();
    if(pDoc->m_NineShowView)
	    pDoc->m_NineShowView=NULL;

   for(int j=0;j<9;j++)
   {
	  m_pDoc->m_SystemInitData.m_TotalNineViewData.m_NineViewDate[j].showtype=m_NineShowData[j].showtype;
	  m_pDoc->m_SystemInitData.m_TotalNineViewData.m_NineViewDate[j].IsDay=m_NineShowData[j].IsDay;
	  m_pDoc->m_SystemInitData.m_TotalNineViewData.m_NineViewDate[j].minutecount=m_NineShowData[j].minutecount;
   }
   m_pDoc->m_SystemInitData.m_TotalNineViewData.NineInitDone=INITDATAID;

	this->m_nFont.DeleteObject();

	for(int i=0;i<9;i++)
	{
        if(m_NineShowData[i].pNineData!=NULL)
		{
			GlobalUnlock((HGLOBAL)m_NineShowData[i].pNineData);      
		    GlobalFree( (HGLOBAL)m_NineShowData[i].pNineData);
		}
		delete m_pGridCtrl[i];
	}
#ifdef WIDE_NET_VERSION
#endif

}

BOOL CNineShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	

	return CView::PreCreateWindow(cs);
}



void CNineShowView::OnDraw(CDC* pDC)
{
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CListTool listtool;
	int i,j;
	CRect TextRect;
	CString str;
	
	pDC->SetTextColor(m_OtherDataColor);
	pDC->SetBkColor(m_BackColor);

	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
		

		
			listtool.Draw_Button(pDC,m_NineShowData[i*3+j].rcTitle.left-1,m_NineShowData[i*3+j].rcTitle.top-1,
				m_NineShowData[i*3+j].rcTitle.right,m_NineShowData[i*3+j].rcTitle.bottom,1,TITLEBACK,1);

			m_NineShowData[i*3+j].rcTitle.OffsetRect( 0, ADDINTER );
		
			str=m_NineShowData[i*3+j].title;
			ShowOneData(pDC,str,m_NineShowData[i*3+j].rcTitle,m_TitleColor,DT_CENTER|DT_VCENTER);
			m_NineShowData[i*3+j].rcTitle.OffsetRect( 0, -ADDINTER );
            SetSortArrow(pDC,m_NineShowData+i*3+j);
		}
	}
}
void CNineShowView::SetSortArrow(CDC *pDC,NineShowData *l_NineShowDat)
{

	int iddd = IDB_1;
	if(!(l_NineShowDat->bAscending)) iddd = IDB_2;
	CImageList list;
	list.Create (iddd,17,1,RGB(255,255,255));
	CPoint p;
	 p.y =(l_NineShowDat->rcTitle.bottom+l_NineShowDat->rcTitle.top)/2-8;
     p.x =l_NineShowDat->rcTitle.right -35;
	list.Draw(pDC,0,p,ILD_TRANSPARENT);
	return;

     CPen *poldpen;
	 CRect rect;

	 CPen pen_white(PS_SOLID,1,RGB(255,255,255));  
	 CPen pen_black(PS_SOLID,1,RGB(0,0,0));     
	 int nBackMode;
     nBackMode=pDC->GetBkMode();
     if(nBackMode==TRANSPARENT) pDC->SetBkMode(OPAQUE);


	 rect.top =(l_NineShowDat->rcTitle.bottom+l_NineShowDat->rcTitle.top)/2-8;
     rect.bottom =(l_NineShowDat->rcTitle.bottom+l_NineShowDat->rcTitle.top)/2+4;
     rect.left =l_NineShowDat->rcTitle.right -17+2;
	 rect.right =l_NineShowDat->rcTitle.right-8+2;


	 poldpen = pDC->SelectObject(&pen_white);
	 if(l_NineShowDat->bAscending)
	 {
		 pDC->MoveTo(rect.left,rect.bottom);
		 pDC->LineTo(rect.right,rect.bottom);
		 pDC->MoveTo(rect.right,rect.bottom);
		 pDC->LineTo((rect.left+rect.right)/2,rect.top);
	 }
	 else
	 {
		 pDC->MoveTo(rect.right,rect.top+4);
		 pDC->LineTo((rect.left+rect.right)/2,rect.bottom+4);
	 }
	 pDC->SelectObject(&pen_black);
     if(l_NineShowDat->bAscending)
	 {
		 pDC->MoveTo(rect.left,rect.bottom);
		 pDC->LineTo((rect.left+rect.right)/2,rect.top);
	 }
	 else
	 {
		 pDC->MoveTo(rect.left,rect.top+4);
		 pDC->LineTo(rect.right,rect.top+4);
		 pDC->MoveTo(rect.left,rect.top+4);
		 pDC->LineTo((rect.left+rect.right)/2,rect.bottom+4);
	 }	 
	 pDC->SelectObject(poldpen);
	 if(nBackMode==TRANSPARENT) pDC->SetBkMode(TRANSPARENT);
} 



BOOL CNineShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	
	return DoPreparePrinting(pInfo);
}

void CNineShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	
}

void CNineShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{

}



#ifdef _DEBUG
void CNineShowView::AssertValid() const
{
	CView::AssertValid();
}

void CNineShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTaiShanDoc* CNineShowView::GetDocument() 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaiShanDoc)));
	return (CTaiShanDoc*)m_pDocument;
}
#endif


void CNineShowView::ShowOneData(CDC *pDC, CString str, CRect rect, COLORREF Color,UINT nFormat)
{
	int nBackMode;
    nBackMode=pDC->GetBkMode();
    if(nBackMode==OPAQUE) pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(m_BackColor);
	CFont *Oldfont=pDC->SelectObject(&m_nFont);

	COLORREF OldColor=pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));


	pDC->SetTextColor(Color);

	pDC->DrawText(str,rect,nFormat);

	pDC->SetTextColor(OldColor);
	pDC->SelectObject(Oldfont);
	if(nBackMode==OPAQUE) pDC->SetBkMode(OPAQUE);
}

CString CNineShowView::TurnDigitToStr(int nDig)
{
	CString str[]={_T("一"),_T("二"),_T("三"),_T("四"),
		_T("五"),_T("六"),_T("七"),_T("八"),_T("九"),_T("十")};
	CString strResult;
	if(nDig>10)
	{
		int tt=nDig/10;
		if(tt==1)
			strResult=str[9];
		else
			strResult=str[tt-1]+str[9];
 		if((nDig-tt*10)>0)strResult+=str[nDig-tt*10-1];
	}
	else
		strResult=str[nDig-1];
	return strResult;
}

void CNineShowView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	

	if(cx==0||cy==0)
		return;
 	double Interx=(cx)/3;
	double Intery=(cy )/3;
	int i,j;
	int x1,y1,x2,y2;
	CRect TextRect;

	int count=0;
	
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			x1=int(m_Rect.left+j*Interx);
			y1=int(m_Rect.top+i*Intery);
			x2=int(m_Rect.left+(j+1)*Interx);
			y2=int(m_Rect.top+(i+1)*Intery);
			m_NineShowData[i*3+j].rcBig.SetRect( x1, y1, x2, y2 );
			
			y2=int(m_Rect.top+i*Intery+TOPLEFT);
			m_NineShowData[i*3+j].rcTitle.SetRect( x1, y1, x2, y2 );
		}
	}
 	SetWindowText("特别报道");
	GetClientRect(m_Rect);
	for( i=0;i<9;i++)
	{
		int x1=m_NineShowData[i].rcTitle.left;
		int y1=m_NineShowData[i].rcTitle.bottom;
        int x2=m_NineShowData[i].rcTitle.right -x1  ;
		int y2=m_NineShowData[i].rcBig.bottom - y1;

		m_pGridCtrl[i]->MoveWindow(x1,y1,x2,y2, TRUE);
		m_pGridCtrl[i]->SetColumnWidth(0,40);
	    m_pGridCtrl[i]->SetColumnWidth(1,(x2-17-40)/3-2);
		m_pGridCtrl[i]->SetColumnWidth(2,(x2-17-40)/3-10);
		m_pGridCtrl[i]->SetColumnWidth(3,(x2-17-40)/3+10);
	}
}
LRESULT CNineShowView::OnRefreshNineShow(WPARAM wPara, LPARAM lPara)
{
	char l_sStockCode[7];
	int nKind=0;
	CFrameWnd * pActiveWnd=((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetActiveFrame();
	if(pActiveWnd==NULL)
		return 0;
    if(!pActiveWnd->IsKindOf(RUNTIME_CLASS(CNineShowFrame)))
		return 0;

	if(m_bSync)
	{
		int l_nRows=m_pGridCtrl[m_nFocusCount]->GetCurrentRow();
		int l_RealRow=m_pGridCtrl[m_nFocusCount]->GetRealRow()-1;
		l_nRows--;
		if(l_nRows<l_RealRow&&l_RealRow!=0)
		{
	       strcpy(l_sStockCode,m_NineShowData[m_nFocusCount].pNineData[l_nRows].pItem->id);
		   nKind=m_NineShowData[m_nFocusCount].pNineData[l_nRows].pItem->kind;
		}
	}
	for(int i=0;i<9;i++)
		SortDataForTechnique(&m_NineShowData[i],m_pGridCtrl[i],!m_bSync);
	if(m_bSync)
	{
       for(int i=0;i<9;i++)
          if(i!=m_nFocusCount)
            ScrollToSelectStock(i,l_sStockCode,nKind);

       ScrollToSelectStock(m_nFocusCount,l_sStockCode,nKind);

	}
#ifdef WIDE_NET_VERSION
    SendRecvDataInfomation();
#endif

	return 0;
}
BOOL CNineShowView::PreTranslateMessage(MSG* pMsg) 
{

	 if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_LBUTTONDOWN  
		 || pMsg->message == WM_LBUTTONDBLCLK || pMsg->message == WM_RBUTTONDOWN ||
		 pMsg->message == WM_COMMAND|| pMsg->message == WM_MOUSEMOVE
		 || pMsg->message == WM_SYSKEYDOWN)
	{
	   this->IsQuit = 1;
       m_pGridCtrl[m_nFocusCount]->SetFocus( );
	}
    if(pMsg->message==WM_KEYDOWN&&(pMsg->wParam==VK_RIGHT||pMsg->wParam==VK_TAB&&!(GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1)))))
	{
		if(m_nFocusCount==8)
            m_nFocusCount=0;
		else
			m_nFocusCount++;
		SetNineShowFocus(m_nFocusCount);
	}
    if(pMsg->message==WM_KEYDOWN&&(pMsg->wParam==VK_LEFT||pMsg->wParam==VK_TAB&&(GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1)))))
	{
		if(m_nFocusCount==0)
            m_nFocusCount=8;
		else
			m_nFocusCount--;
		SetNineShowFocus(m_nFocusCount);
	}

   	if(pMsg->message==WM_RBUTTONDOWN)
	{
		return TRUE;
	}
   	if(pMsg->message==WM_LBUTTONDOWN)
	{
		CPoint m_pos;
		m_pos=pMsg->pt;
		ScreenToClient(&m_pos);
		for(int i=0;i<9;i++)
		{
			if(m_NineShowData[i].rcTitle.PtInRect(m_pos ))
			{
				return CView::PreTranslateMessage(pMsg);
			}else if(m_NineShowData[i].rcBig.PtInRect(m_pos ))
			{
                SetNineShowFocus(i,FALSE);

                CCellID m_CellTitleCommand;
		        CPoint m_posGrid=pMsg->pt;
                m_pGridCtrl[i]->ScreenToClient(&m_posGrid);
				m_CellTitleCommand= m_pGridCtrl[i]->GetCellFromPt(m_posGrid);
				if(m_CellTitleCommand.row == 0 )
				{
					return TRUE;
				}
			}
		}
	}

	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
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
           strcpy(result,m_diakeybrode.result);
           int nKind=m_diakeybrode.m_stkKind;
		   if(strlen(result)==6||strlen(result)==4)
		   {
                if(!ScrollToSelectStock(result,nKind))
				{
					CTaiShanDoc* pDoc = GetDocument();
					pDoc->m_sharesSymbol = result;
                    pDoc->m_stkKind=nKind;
					pDoc->m_GetFocus = true;
					CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
					pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)result,nKind*10);//stock_code.GetBuffer (0)
				}
				if(this->m_bSync)
				  SendMessage(WM_USER_NINESHOW_ROW_CHANGED,0,0);

				return TRUE;
		   }
		   else if(strlen(result)==2&&!m_diakeybrode.ischar)
           {
			   int m_key=atoi(result);
                switch(m_key)
				{
				case 81:
				case 82:
				case 83:
				case 84:
				case 85:
				case 86:
				case 87:
                case 89:
				
				if(m_key!=89)
				   this->FirstMenu = m_key - 81;
				else
				   this->FirstMenu = 7;
					PostMessage(WM_KEYBOARDGENIUS,m_key,0);
					return TRUE;
					break;
                default:
					{
 
				    CTaiShanDoc* pDoc=GetDocument();
					if(pDoc->m_NineShowView==NULL)
						return TRUE;
 
					IsQuit = 1;

					::PostMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_HOTKEYPROGRESS,0,m_key); 
					
					GetParent()->SendMessage(WM_CLOSE);
					return TRUE; 
					}
					break;
				}
		   }
		}
		else
         return TRUE;    

	}
	if(pMsg->message==WM_LBUTTONDBLCLK||pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
	{
	    CPoint m_pos;
		CString stock_code,stock_name;
		CTaiShanDoc *pDoc = GetDocument();
        CCellID m_CellTitleCommand;

			
		if(pMsg->message==WM_LBUTTONDBLCLK)
		{
			CPoint m_pos;
		    m_pos=pMsg->pt;
		    ScreenToClient(&m_pos);
			for(int i=0;i<9;i++)
			{
				if(m_NineShowData[i].rcTitle.PtInRect(m_pos ))
				{
					return CView::PreTranslateMessage(pMsg);;
				}else if(m_NineShowData[i].rcBig.PtInRect(m_pos ))
				{
					CPoint m_posGrid=pMsg->pt;
					m_pGridCtrl[i]->ScreenToClient(&m_posGrid);
			        m_CellTitleCommand=m_pGridCtrl[i]->GetCellFromPt(m_posGrid);
					m_nFocusCount=i;
					if(m_CellTitleCommand.row == 0)
					   return FALSE;
				}
			}
		}
		else 
		{
            m_CellTitleCommand.row=m_pGridCtrl[m_nFocusCount]->GetCurrentRow();
			if(m_CellTitleCommand.row == 0)
			   return FALSE;
            m_CellTitleCommand.col=2;
		}
		if(m_CellTitleCommand.row > 0 )
		{
			stock_code = m_NineShowData[m_nFocusCount].pNineData[m_CellTitleCommand.row-1].pItem->id;
			stock_name = m_NineShowData[m_nFocusCount].pNineData[m_CellTitleCommand.row-1].pItem->name;

            if(stock_code.Compare("") == 0)
				return FALSE;
			
			pDoc->m_sharesSymbol = stock_code;
			pDoc->m_stkKind=m_NineShowData[m_nFocusCount].pNineData[m_CellTitleCommand.row-1].pItem->kind;
			pDoc->m_GetFocus = true;
			CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
			pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),pDoc->m_stkKind*10);
		}
	}
	if(pMsg->message == WM_CHAR && pMsg->wParam == VK_ESCAPE)
    {
	  IsQuit = 1;
	  GetParent()->SendMessage(WM_CLOSE);
	  return true;
	}
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam == VK_F1)	
	{
	
	DoHtmlHelp(this);return TRUE;

	}
	
	return CView::PreTranslateMessage(pMsg);
}

int CNineShowView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -14;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");


	this->m_nFont.CreateFontIndirect(&m_lf);
	m_pDoc=this->GetDocument();
	if(m_pDoc->m_SystemInitData.m_TotalNineViewData.NineInitDone==INITDATAID)
	{
       for(int j=0;j<9;j++)
	   {
		m_NineShowData[j].showtype=m_pDoc->m_SystemInitData.m_TotalNineViewData.m_NineViewDate[j].showtype;
		m_NineShowData[j].IsDay=m_pDoc->m_SystemInitData.m_TotalNineViewData.m_NineViewDate[j].IsDay;
		m_NineShowData[j].minutecount=m_pDoc->m_SystemInitData.m_TotalNineViewData.m_NineViewDate[j].minutecount;
		m_NineShowData[j].bAscending=FALSE;
		m_NineShowData[j].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[j]);
	   }
	}
    else
	{
		m_NineShowData[0].showtype=IDRDR;
		m_NineShowData[0].IsDay=TRUE;
		m_NineShowData[0].minutecount=0;
		m_NineShowData[0].bAscending=FALSE;
		m_NineShowData[0].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[0]);

		m_NineShowData[1].showtype=IDADVSP;
		m_NineShowData[1].IsDay=FALSE;
		m_NineShowData[1].minutecount=5;
		m_NineShowData[1].bAscending=FALSE;
		m_NineShowData[1].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[1]);

		m_NineShowData[2].showtype=IDBSSPBAS;
		m_NineShowData[2].IsDay=TRUE;
		m_NineShowData[2].minutecount=0;
		m_NineShowData[2].bAscending=FALSE;
		m_NineShowData[2].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[2]);

		m_NineShowData[3].showtype=IDHLP;
		m_NineShowData[3].IsDay=TRUE;
		m_NineShowData[3].minutecount=0;
		m_NineShowData[3].bAscending=FALSE;
		m_NineShowData[3].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[3]);

		m_NineShowData[4].showtype=IDVOLBI;
		m_NineShowData[4].IsDay=TRUE;
		m_NineShowData[4].minutecount=0;
		m_NineShowData[4].bAscending=FALSE;
		m_NineShowData[4].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[4]);

		m_NineShowData[5].showtype=IDTOTP;
		m_NineShowData[5].IsDay=TRUE;
		m_NineShowData[5].minutecount=0;
		m_NineShowData[5].bAscending=FALSE;
		m_NineShowData[5].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[5]);

		m_NineShowData[6].showtype=IDDVOL;
		m_NineShowData[6].IsDay=TRUE;
		m_NineShowData[6].minutecount=0;
		m_NineShowData[6].bAscending=FALSE;
		m_NineShowData[6].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[6]);

		m_NineShowData[7].showtype=IDZJLS;
		m_NineShowData[7].IsDay=FALSE;
		m_NineShowData[7].minutecount=5;
		m_NineShowData[7].bAscending=FALSE;
		m_NineShowData[7].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[7]);

		m_NineShowData[8].showtype=IDEXCHANGEFM;
		m_NineShowData[8].IsDay=FALSE;
		m_NineShowData[8].minutecount=5;
		m_NineShowData[8].bAscending=FALSE;
		m_NineShowData[8].pNineData=NULL;
		GetNineShowTitle(&m_NineShowData[8]);
	}
	DWORD dwStyle=0;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE|WS_BORDER ;
	dwStyle &= ~WS_HSCROLL;
	for(int i=0;i<9;i++)
	{
		m_pGridCtrl[i] =new CNineGridCtrl;
		m_pGridCtrl[i]->EnableTitleTips(FALSE);
		m_pGridCtrl[i]->Create(CRect(0,0,10,10),this,ID_NINEGRID_MXDIALOG,dwStyle);
        InitGridContent(m_pGridCtrl[i]);
		m_pGridCtrl[i]->SetListMode(TRUE);
		m_pGridCtrl[i]->SetMainDocument(this->GetDocument());

		m_pGridCtrl[i]->SetItemState(1,1, m_pGridCtrl[i]->GetItemState(1,1) | GVIS_READONLY);
		m_pGridCtrl[i]->SetRowHeight(0, int(1.0*m_pGridCtrl[i]->GetRowHeight(0)));
    	m_pGridCtrl[i]->SetEditable(FALSE);
		m_pGridCtrl[i]->SetRowResize(FALSE);
		m_pGridCtrl[i]->SetColumnResize(FALSE);
   		m_pGridCtrl[i]->SetMouseState(MOUSE_NOTHINGS);
		m_pGridCtrl[i]->SetTimeInter(m_NineShowData[i].minutecount);

	    InitAllTitle(m_pGridCtrl[i],&m_NineShowData[i]);
        m_pGridCtrl[i]->SetRealRow(1+1);
	}
    m_pGridCtrl[m_nFocusCount]->SetFocus( );
	return 0;
}

void CNineShowView::InitAllTitle(CNineGridCtrl *pGrid,NineShowData *nNineShowData)
{
	    CString str="排名";
		pGrid->SetColumnInfo(0,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),ID_ROWNUM);
		str="股票名称";
		pGrid->SetColumnInfo(1,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),ID_STOCKNAME);
		str="最新价";
		pGrid->SetColumnInfo(2,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),ID_NEW);

        switch(nNineShowData->showtype)
		{
			case IDTOTP:
						str="资金总额";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_TOTP);
                        break;
			case IDHLP:
						str="震幅";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_HLP);
                        break;
			case IDRDR:
						str="涨幅";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_RDR);
                        break;
			case IDBSSPBAS:
						str="委比";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_BSSPBAS);
                        break;
			case IDVOLBI:
						str="量比";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_VOLBI);
                        break;
			case IDDVOL:
						str="外盘比值";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_DVOL);
                        break;
			case IDEXCHANGE:
						str="换手率";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_EXCHANGE);
                        break;
			case IDSYL:
						str="市盈率";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_SYL);
                        break;
			case IDZJLS:
						str="分时资金";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_ZJLS);
                        break;
			case IDHLPFM:
						str="分时震幅";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_HLPFM);
                        break;
			case IDEXCHANGEFM:
						str="分时换手";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_EXCHANGEFM);
                        break;
			case IDADVSP:
						str="分时涨速";
						pGrid->SetColumnInfo(3,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
								str.GetBuffer(0),ID_ADVSP);
                        break;

		}
}
BOOL CNineShowView::InitGridContent(CNineGridCtrl *pGrid )  
{
	m_nCols=4;
    m_nFixRows=1;
	m_nFixCols=0;
    m_nRows=20;  


	try {
 		pGrid->SetRowCount(m_nRows);
		pGrid->SetColumnCount(m_nCols);
		pGrid->SetFixedRowCount(m_nFixRows);
		pGrid->SetFixedColumnCount(m_nFixCols);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
   	pGrid->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);

//20000615
    pGrid->SetFixedBkColor(m_pDoc->m_colorArray[18]);
//20000615


	CFont Font;
	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -12;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");

	Font.CreateFontIndirect(&m_lf);

	pGrid->SetFont(&Font);
	Font.DeleteObject();
 	return TRUE;
}

void CNineShowView::GetNineShowTitle(NineShowData *nNineData)
{
	CString nTitle;
	if(nNineData->IsDay)
		nTitle="今日";
	else
	{
		nTitle=TurnDigitToStr(nNineData->minutecount);
	}
	if(nNineData->bAscending)
	  nTitle+=m_typetitle.GetAt(nNineData->showtype);
	else
	  nTitle+=m_typetitle.GetAt(nNineData->showtype+12);
    strcpy(nNineData->title,nTitle.GetBuffer(0));
	return;
}

void CNineShowView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	CDC *pDC=this->GetDC();
	CListTool listtool;
	pDC->SetTextColor(m_OtherDataColor);
	pDC->SetBkColor(m_BackColor);
	for(int i=0;i<9;i++)
	{
        if(m_NineShowData[i].rcTitle.PtInRect(point ))
		{
			m_NineShowData[i].bAscending=!m_NineShowData[i].bAscending;
			GetNineShowTitle(&m_NineShowData[i]);
			listtool.Draw_Button(pDC,m_NineShowData[i].rcTitle.left,m_NineShowData[i].rcTitle.top,
				m_NineShowData[i].rcTitle.right,m_NineShowData[i].rcTitle.bottom,1,TITLEBACK,1);
			m_NineShowData[i].rcTitle.OffsetRect( 0, ADDINTER );
			CString str=m_NineShowData[i].title;
			ShowOneData(pDC ,str,m_NineShowData[i].rcTitle,m_TitleColor,DT_CENTER|DT_VCENTER);
			m_NineShowData[i].rcTitle.OffsetRect( 0, -ADDINTER );
			SetSortArrow(pDC,m_NineShowData+i);
            SortDataForTechnique(m_NineShowData+i,m_pGridCtrl[i]);
		}
	}
	ReleaseDC(pDC );
	CView::OnLButtonDblClk(nFlags, point);
}

void CNineShowView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	
	CView::OnRButtonDown(nFlags, point);
}
void CNineShowView::OnTechniqueIndexChoose(UINT nID)
{
	int which=nID-IDD_TOTP;
	m_NineShowData[m_unSaveMousePoint].showtype=which;
	if(nID>=IDD_ZJLS)
	{
#ifdef WIDE_NET_VERSION 
		 m_NineShowData[m_unSaveMousePoint].minutecount=5;
	     m_NineShowData[m_unSaveMousePoint].IsDay=FALSE;
#else
         CSetTimeInter settimeinter;
		 m_NineShowData[m_unSaveMousePoint].minutecount=5;
	     settimeinter.m_TimeInter=5;
         if(settimeinter.DoModal()==IDOK)
		 {
		

			  m_NineShowData[m_unSaveMousePoint].minutecount=settimeinter.m_TimeInter;
		      m_NineShowData[m_unSaveMousePoint].IsDay=FALSE;

		 }
#endif
    }
	else
	   m_NineShowData[m_unSaveMousePoint].IsDay=TRUE;

	m_NineShowData[m_unSaveMousePoint].bAscending=FALSE;
	GetNineShowTitle(&m_NineShowData[m_unSaveMousePoint]);

	CDC *pDC=this->GetDC();
	CListTool listtool;
	pDC->SetTextColor(m_OtherDataColor);
	pDC->SetBkColor(m_BackColor);
	listtool.Draw_Button(pDC,m_NineShowData[m_unSaveMousePoint].rcTitle.left,m_NineShowData[m_unSaveMousePoint].rcTitle.top,
		m_NineShowData[m_unSaveMousePoint].rcTitle.right,m_NineShowData[m_unSaveMousePoint].rcTitle.bottom,1,TITLEBACK,1);
	m_NineShowData[m_unSaveMousePoint].rcTitle.OffsetRect( 0, ADDINTER );
	CString str=m_NineShowData[m_unSaveMousePoint].title;
	ShowOneData(pDC ,str,m_NineShowData[m_unSaveMousePoint].rcTitle,m_TitleColor,DT_CENTER|DT_VCENTER);
	m_NineShowData[m_unSaveMousePoint].rcTitle.OffsetRect( 0, -ADDINTER );
	SetSortArrow(pDC,m_NineShowData+m_unSaveMousePoint);
	ReleaseDC(pDC );

    m_pGridCtrl[m_unSaveMousePoint]->SetTimeInter(m_NineShowData[m_unSaveMousePoint].minutecount);
	InitAllTitle(m_pGridCtrl[m_unSaveMousePoint],&m_NineShowData[m_unSaveMousePoint]);
	SortDataForTechnique(m_NineShowData+m_unSaveMousePoint,m_pGridCtrl[m_unSaveMousePoint ]);
}
void CNineShowView::OnStockTypeSelect()
{
    m_nSharesType=6;

	FirstMenu = 6;
	CString str;
    str.Format("板块指数");

	SetShowData(m_pDoc->m_sharesInformation.GetStockTypeCount(8));
	LoadShowData(8);
}
void CNineShowView::OnStockTypeChoose(UINT nID)
{
	int which;
	if(ID_SZEB==nID)
      which=7;
	else if(nID==ID_STOCKTYPE)
	  which=6;
    else
	  which=nID-ID_SHSTOCKA;
	CTaiShanDoc *pDoc=GetDocument();
    m_nSharesType=which;

	FirstMenu = which;
	int l_nStockType;
	CString str;
 	switch(which)
	{
	     case 0 :  str.Format("上海证券A股");
			       l_nStockType=1;
			       break;
	     case 1 :  str.Format("上海证券B股");
			       l_nStockType=2;
			       break;
	     case 2 :  str.Format("深圳证券A股");
			       l_nStockType=4;
			       break;
	     case 3 :  str.Format("深圳证券B股");
			       l_nStockType=5;
			       break;
	     case 4 :  str.Format("上证债券");
			       l_nStockType=6;
			       break;
	     case 5 :  str.Format("深证债券");
			       l_nStockType=7;
			       break;
	     case 6 :  str.Format("板块指数");
			       l_nStockType=10;
			       break;
	     case 7 :  str.Format("深圳创股");
			       l_nStockType=9;
			       break;
	}

	SetShowData(m_pDoc->m_sharesInformation.GetStockTypeCount(l_nStockType));
	LoadShowData(l_nStockType);
}
void CNineShowView::LoadShowData(int nStockType)
{
    int l_StockCount=m_pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
	m_pDoc->m_sharesInformation.RecvStockDataForType(m_NineShowData[0].pNineData,nStockType);
	for(int i=1;i<9;i++)
	  memcpy(m_NineShowData[i].pNineData,m_NineShowData[0].pNineData,sizeof(STOCKDATASHOW)*l_StockCount);
	m_nRows=l_StockCount;
	if(m_nRows<10)
       m_nRows=10;
    for(int i=0;i<9;i++)
	{
		try
		{
 		       m_pGridCtrl[i]->SetRowCount(m_nRows+5);
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return ;
		}
		m_pGridCtrl[i]->SetShowData(m_NineShowData[i].pNineData);
		m_pGridCtrl[i]->SetRealRow(l_StockCount+1); 


        m_pGridCtrl[i]->SetScrollPos32(SB_VERT, 0, TRUE);
		m_pGridCtrl[i]->SetFocusRow(1); 
   		m_pGridCtrl[i]->SetMouseState(MOUSE_NOTHINGS);
	}
    SetNineShowFocus(m_nFocusCount);
	for(int i=0;i<9;i++)
	  SortDataForTechnique(&m_NineShowData[i],m_pGridCtrl[i]);


}

void CNineShowView::OnSetTimeInter()
{
	CClientDC dc(this);
	CSetTimeInter settimeinter;
	settimeinter.m_TimeInter=m_NineShowData[m_unSaveMousePoint].minutecount;

	if(settimeinter.DoModal()==IDOK)
	{
	
		m_NineShowData[m_unSaveMousePoint].minutecount=settimeinter.m_TimeInter;
	    GetNineShowTitle(&m_NineShowData[m_unSaveMousePoint]);

		CListTool listtool;
		dc.SetTextColor(m_OtherDataColor);
		dc.SetBkColor(m_BackColor);
		listtool.Draw_Button(&dc,m_NineShowData[m_unSaveMousePoint].rcTitle.left,m_NineShowData[m_unSaveMousePoint].rcTitle.top,
			m_NineShowData[m_unSaveMousePoint].rcTitle.right,m_NineShowData[m_unSaveMousePoint].rcTitle.bottom,1,TITLEBACK,1);
		m_NineShowData[m_unSaveMousePoint].rcTitle.OffsetRect( 0, ADDINTER );
		CString str=m_NineShowData[m_unSaveMousePoint].title;
		ShowOneData(&dc ,str,m_NineShowData[m_unSaveMousePoint].rcTitle,m_TitleColor,DT_CENTER|DT_VCENTER);
		m_NineShowData[m_unSaveMousePoint].rcTitle.OffsetRect( 0, -ADDINTER );
		SetSortArrow(&dc,m_NineShowData+m_unSaveMousePoint);
	}
    m_pGridCtrl[m_unSaveMousePoint]->SetTimeInter(m_NineShowData[m_unSaveMousePoint].minutecount); 
}
BOOL CNineShowView::SetShowData(int StockCount)
{
	HGLOBAL	hMem;
	LPVOID hp;

	if(m_nShowStockCount >= StockCount)
		return TRUE;
	for(int i=0;i<9;i++)
	{
		if(m_NineShowData[i].pNineData!=NULL)
		{
			GlobalUnlock((HGLOBAL)m_NineShowData[i].pNineData);      
		    GlobalFree( (HGLOBAL)m_NineShowData[i].pNineData);
			m_NineShowData[i].pNineData=NULL;
		}
	}
	for(int i=0;i<9;i++)
	{
         hMem=GlobalAlloc( GPTR, (StockCount)* sizeof( STOCKDATASHOW) );
	     hp=GlobalLock(hMem);
		 if( hp )
		 {
            m_NineShowData[i].pNineData=(STOCKDATASHOW *)hp;
		    m_nShowStockCount = StockCount;
		 }
		 else
		 {
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		 }
	 }
	return TRUE;
}
LRESULT CNineShowView::OnKeyboardGenius(WPARAM wPara, LPARAM lPara)
{
	int WhichStockType=(int)wPara-FIRSTKEYFLAG;
	if(WhichStockType==9)
       WhichStockType=8;
	CTaiShanDoc *pDoc=GetDocument();
	m_nSharesType=WhichStockType-1;
	int l_nStockType;
	CString str;
 	switch(m_nSharesType)
	{
	     case 0 :  str.Format("上海证券A股");
			       l_nStockType=1;
			       break;
	     case 1 :  str.Format("上海证券B股");
			       l_nStockType=2;
			       break;
	     case 2 :  str.Format("深圳证券A股");
			       l_nStockType=4;
			       break;
	     case 3 :  str.Format("深圳证券B股");
			       l_nStockType=5;
			       break;
	     case 4 :  str.Format("上证债券");
			       l_nStockType=6;
			       break;
	     case 5 :  str.Format("深证债券");
			       l_nStockType=7;
			       break;
	     case 6 :  str.Format("板块指数");
			       l_nStockType=10;
			       break;
	     case 7 :  str.Format("深圳创股");
			       l_nStockType=9;
			       break;

	}

	SetShowData(m_pDoc->m_sharesInformation.GetStockTypeCount(l_nStockType));
	LoadShowData(l_nStockType);

	return 0;
}
void CNineShowView::SetNineShowFocus(int nShowPos,BOOL IsSetPos)
{
    for(int j=0;j<9;j++)
	{
		if(m_pGridCtrl[j]->GetFocusState())
		{
           m_pGridCtrl[j]->SetFocusState(FALSE);
           m_pGridCtrl[j]->Invalidate(); 
		}
	}
	m_nFocusCount=nShowPos;
    m_pGridCtrl[nShowPos]->SetFocusState(TRUE);
    m_pGridCtrl[nShowPos]->SetFocus( );
	if(IsSetPos)
	   m_pGridCtrl[nShowPos]->GotoLine(m_pGridCtrl[nShowPos]->GetCurrentRow(),TRUE);
    m_pGridCtrl[nShowPos]->SetMouseState(MOUSE_NOTHINGS);
    m_pGridCtrl[nShowPos]->Invalidate(); 

}
void CNineShowView::SortDataForTechnique(NineShowData *pNineShowData,CNineGridCtrl *pGrid,BOOL IsRefresh)
{
    CTaiShanDoc* pDoc = GetDocument();
	int index=0;
	int DisplayCount=0;
	int l_nStockType;
 	switch(m_nSharesType)
	{
	     case 0 :  
			       l_nStockType=1;
			       break;
	     case 1 :  
			       l_nStockType=2;
			       break;
	     case 2 :  
			       l_nStockType=4;
			       break;
	     case 3 :  
			       l_nStockType=5;
			       break;
	     case 4 :  
			       l_nStockType=6;
			       break;
	     case 5 :  l_nStockType=7;
			       break;
	     case 6 :  
			       l_nStockType=10;
			       break;
	     case 7 :  
			       l_nStockType=9;
			       break;
	}
	int l_RealRow=m_pGridCtrl[m_nFocusCount]->GetRealRow()-1;
	DisplayCount=m_pDoc->m_sharesInformation.GetStockTypeCount(l_nStockType);
	if(l_RealRow<DisplayCount)
	{
		SetShowData(m_pDoc->m_sharesInformation.GetStockTypeCount(l_nStockType));
		LoadShowData(l_nStockType);
	}
	for(int i=0;i<DisplayCount;i++)
	{
		CReportData *pData;
		pData = pNineShowData->pNineData[i].pItem;

		if(pData==NULL)
			continue;
		switch(pNineShowData->showtype)
		{
		case IDTOTP:                                
			   pNineShowData->pNineData[i].ItemData=pData->totp;
			 break;

		case IDHLP:                                
              if(pData->ystc !=0)
			  {
                  pNineShowData->pNineData[i].ItemData=(pData->higp - pData->lowp) /pData->ystc ;
			  }
			  else
                  pNineShowData->pNineData[i].ItemData=0;
			break;
		case IDRDR:                               
			 if(pData->ystc !=0&&pData->nowp!=0)
			 {
               pNineShowData->pNineData[i].ItemData= (pData->nowp - pData->ystc )/ pData->ystc ;
			 }
			 else
               pNineShowData->pNineData[i].ItemData=0;
			 
			break;
		case IDBSSPBAS:                          
			{
				float temp=0;
				if (pData->accb+pData->accs>0)
					if(pData->accs==0&&pData->accb!=0)
					   temp=100;
					else if(pData->accs!=0&&pData->accb==0)
					   temp=-100;
					else
					   temp=(float)(pData->accb-pData->accs)/(float)(pData->accb+pData->accs)*100.0;     //计算委比
				else
					temp=0;
				pNineShowData->pNineData[i].ItemData=temp;
			}
			break;
		case IDVOLBI:                             
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
              pNineShowData->pNineData[i].ItemData=(pData->totv / m_pDoc->m_nANT[0]) / (pData->volume5  );
			}
			else
              pNineShowData->pNineData[i].ItemData=0;
			
			break;
		case IDDVOL:                              
			if(pData->dvol+pData->rvol>0)
              pNineShowData->pNineData[i].ItemData=pData->rvol/(pData->totv);
			else
              pNineShowData->pNineData[i].ItemData=0;

			break;

        case IDEXCHANGE:
			  if(pData->pBaseInfo==NULL)
                 pNineShowData->pNineData[i].ItemData=0;
              else
			  {
				  if(CTaiShanReportView::IsBg(pData-> kind))
                     if(pData->pBaseInfo->Bg!=0) 
                        pNineShowData->pNineData[i].ItemData=pData->totv/pData->pBaseInfo->Bg;
					 else
			            pNineShowData->pNineData[i].ItemData=0.0f;
				  else
					  if(pData->pBaseInfo->ltAg!=0)
			            pNineShowData->pNineData[i].ItemData=pData->totv/pData->pBaseInfo->ltAg;
					  else
			            pNineShowData->pNineData[i].ItemData=0.0f;
			  }
			  break;
        case IDSYL:
			  if(pData->pBaseInfo==NULL)
			     pNineShowData->pNineData[i].ItemData=0;
              else
			  {
				 if(pData->pBaseInfo->mgsy!=0)
                   pNineShowData->pNineData[i].ItemData=abs(pData->nowp/pData->pBaseInfo->mgsy); 
				 else
			       pNineShowData->pNineData[i].ItemData=0.0f;
			  }
			  break;
		case IDZJLS:       
		  if(m_pDoc->m_nANT[0]>pNineShowData->minutecount)
			  {
				int j=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[j].Price==0&&j!=0&&m_pDoc->m_nANT[0]-j<pNineShowData->minutecount)
				{
					j--;
				}
                pNineShowData->pNineData[i].ItemData=pData->m_Kdata1[j].Amount - pData->m_Kdata1[m_pDoc->m_nANT[0] - pNineShowData->minutecount ].Amount;
			  }
			  else
			  {
				int j=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[j].Price==0&&j!=0&&m_pDoc->m_nANT[0]-j<pNineShowData->minutecount)
				{
					j--;
				}
                pNineShowData->pNineData[i].ItemData=pData->m_Kdata1[j].Amount ;
			  }

			  break;
		case IDHLPFM:     
			{
				float high=0,low=0,totp=0;
				int minute=pNineShowData->minutecount;
				if(m_pDoc->m_nANT[0] < minute)
                    minute=m_pDoc->m_nANT[0];
				for(int j=0;j<minute;j++)
				{   
				    if(pData->m_Kdata1[m_pDoc->m_nANT[0]-j].Price==0)
						continue;
					if(high==0&&low==0)
					{
						high=low=pData->m_Kdata1[m_pDoc->m_nANT[0] -j].Price;
						continue;
					}
					if(pData->m_Kdata1[m_pDoc->m_nANT[0] -j].Price > high  )
						high=pData->m_Kdata1[m_pDoc->m_nANT[0] -j].Price;
					else if(pData->m_Kdata1[m_pDoc->m_nANT[0] -j].Price <low)
						low=pData->m_Kdata1[m_pDoc->m_nANT[0] -j].Price;
				}
				if(pData->ystc==0)
				  pNineShowData->pNineData[i].ItemData=0.0f;
				else
				  pNineShowData->pNineData[i].ItemData=(high-low)/(pData->ystc);
			}
			break;
		case IDEXCHANGEFM:  
			  if(pData->pBaseInfo==NULL)
			     pNineShowData->pNineData[i].ItemData=0.0f;
              else
			  {
				  int j=m_pDoc->m_nANT[0];
				  while( pData->m_Kdata1[j].Price==0&&j!=0&&m_pDoc->m_nANT[0]-j<pNineShowData->minutecount)
				  {
				 	  j--;
				  }
				  if(CTaiShanReportView::IsBg(pData-> kind))
                     if(pData->pBaseInfo->Bg!=0) 
					 {
						 if(m_pDoc->m_nANT[0] > pNineShowData->minutecount)
                           pNineShowData->pNineData[i].ItemData=(pData->m_Kdata1[j].Volume - pData->m_Kdata1[j-pNineShowData->minutecount].Volume)/pData->pBaseInfo->Bg;
						 else
                           pNineShowData->pNineData[i].ItemData=(pData->m_Kdata1[j].Volume)/pData->pBaseInfo->Bg;
					 }
					 else
			            pNineShowData->pNineData[i].ItemData=0.0f;
				  else
					  if(pData->pBaseInfo->ltAg!=0)
					  {
						 if(m_pDoc->m_nANT[0] > pNineShowData->minutecount)
			                pNineShowData->pNineData[i].ItemData=(pData->m_Kdata1[j].Volume - pData->m_Kdata1[j -pNineShowData->minutecount].Volume)/pData->pBaseInfo->ltAg;
						 else
			                pNineShowData->pNineData[i].ItemData=(pData->m_Kdata1[j].Volume)/pData->pBaseInfo->ltAg;
					  }
					  else
			            pNineShowData->pNineData[i].ItemData=0.0f;
			  }
			  break;
		case IDADVSP:      
			{
				float m_temp;
				if( pNineShowData->minutecount <m_pDoc->m_nANT[0])
					if( pData->m_Kdata1[m_pDoc->m_nANT[0] - pNineShowData->minutecount ].Price!=0)
					{
						float m_nowp=0;
						int j=m_pDoc->m_nANT[0];
						while( pData->m_Kdata1[j].Price==0&&j!=0)
						{
							  j--;
						}
						m_nowp=pData->m_Kdata1[j].Price;
						m_temp= 100*(m_nowp - 
						pData->m_Kdata1[m_pDoc->m_nANT[0] - pNineShowData->minutecount ].Price)/pData->m_Kdata1[m_pDoc->m_nANT[0] - pNineShowData->minutecount ].Price;
					}   
					else
						m_temp=0.0f;
				else
					if(pData->opnp !=0)
					{
						float m_nowp=0;
						int j=m_pDoc->m_nANT[0];
						while( pData->m_Kdata1[j].Price==0&&j!=0)
						{
							  j--;
						}
						m_nowp=pData->m_Kdata1[j].Price;

						m_temp= 100*(m_nowp - pData->opnp )/pData->opnp ;
					}
					else
						m_temp=0.0f;
				  pNineShowData->pNineData[i].ItemData=m_temp;
			}
			break;

		}
	}
	QuickSort(pNineShowData->pNineData,pNineShowData->bAscending,0,DisplayCount - 1 );
	if(IsRefresh)
       pGrid->Invalidate();
}
BOOL CNineShowView::ScrollToSelectStock(char *zqdm,int nKind)
{
   CTaiShanDoc* pDoc = GetDocument();
   CString m_zqdm=zqdm;
   CString m_findzqdm=zqdm;
   BOOL l_isFind=FALSE;
   int l_nStockType=0;
 	switch(m_nSharesType)
	{
	     case 0 :  
			       l_nStockType=1;
			       break;
	     case 1 :  
			       l_nStockType=2;
			       break;
	     case 2 :  
			       l_nStockType=4;
			       break;
	     case 3 :  
			       l_nStockType=5;
			       break;
	     case 4 :  
			       l_nStockType=6;
			       break;
	     case 5 :  l_nStockType=7;
			       break;
	     case 6 :  
			       l_nStockType=8;
			       break;
	}
   if(nKind==l_nStockType)
   {
	   int temp=pDoc->m_sharesInformation.GetStockTypeCount(l_nStockType);
	   for(int i=0 ;i<temp;i++)
	   {
         if(strcmp(m_findzqdm.GetBuffer(0),m_NineShowData[m_nFocusCount].pNineData[i].pItem->id)==0)
			{
               m_pGridCtrl[m_nFocusCount]->GotoLine(i+1,TRUE);
               m_pGridCtrl[m_nFocusCount]->Invalidate();                            
			   l_isFind=TRUE;
			   break;
			}
	   }
   }
    m_pGridCtrl[m_nFocusCount]->SetMouseState(MOUSE_NOTHINGS);
	return l_isFind;
}
BOOL CNineShowView::ScrollToSelectStock(int nShowCounts,char *zqdm,int nKind)
{
   CTaiShanDoc* pDoc = GetDocument();
   CString m_zqdm=zqdm;
   CString m_findzqdm=zqdm;
   BOOL l_isFind=FALSE;
   int l_nStockType=0;
 	switch(m_nSharesType)
	{
	     case 0 :  
			       l_nStockType=1;
			       break;
	     case 1 :  
			       l_nStockType=2;
			       break;
	     case 2 :  
			       l_nStockType=4;
			       break;
	     case 3 :  
			       l_nStockType=5;
			       break;
	     case 4 :  
			       l_nStockType=6;
			       break;
	     case 5 :  l_nStockType=7;
			       break;
	     case 6 :  
			       l_nStockType=10;
			       break;
	     case 7:  
			       l_nStockType=9;
			       break;
	}
   if(nKind==l_nStockType)
   {
	   int temp=pDoc->m_sharesInformation.GetStockTypeCount(l_nStockType);
	   for(int i=0 ;i<temp;i++)
	   {
         if(strcmp(m_findzqdm.GetBuffer(0),m_NineShowData[nShowCounts].pNineData[i].pItem->id)==0)
			{
               m_pGridCtrl[nShowCounts]->GotoLine(i+1,TRUE);
               m_pGridCtrl[nShowCounts]->Invalidate();                           
			   l_isFind=TRUE;
			   break;
			}
	   }
   }
    m_pGridCtrl[nShowCounts]->SetMouseState(MOUSE_NOTHINGS);
	return l_isFind;
}

void CNineShowView::OnSync() 
{
	
	if(m_bSync)
		m_bSync=FALSE;
	else
		m_bSync=TRUE;
	SetNineShowSync(m_bSync);
	for(int i=0;i<9;i++)
		m_pGridCtrl[i]->SetGridSync(m_bSync);

}
void CNineShowView::SetNineShowSync(BOOL bIsSync)
{
	if(bIsSync)
	{
		int l_nRows=m_pGridCtrl[m_nFocusCount]->GetCurrentRow();
		int l_RealRow=m_pGridCtrl[m_nFocusCount]->GetRealRow()-1;
		l_nRows--;
		if(l_nRows>=l_RealRow||l_RealRow==0)
		   return;
        for(int i=0;i<9;i++)
		{
		  if(i!=m_nFocusCount)
             ScrollToSelectStock(i,m_NineShowData[m_nFocusCount].pNineData[l_nRows].pItem->id,m_NineShowData[m_nFocusCount].pNineData[l_nRows].pItem->kind);
		}
	}
	else 
	{
        for(int i=0;i<9;i++)
		  if(i!=m_nFocusCount)
		  {
			  m_pGridCtrl[i]->GotoLine(1,TRUE);
			  m_pGridCtrl[i]->SetMouseState(MOUSE_NOTHINGS);
			  m_pGridCtrl[i]->Invalidate(); 
		  }
	}
}
LRESULT CNineShowView::OnNineShowRowChanged(WPARAM wPara, LPARAM lPara) 
{
	char l_sStockCode[7];
	int nKind=0;
	if(m_bSync)
	{
		int l_nRows=m_pGridCtrl[m_nFocusCount]->GetCurrentRow();
		int l_RealRow=m_pGridCtrl[m_nFocusCount]->GetRealRow()-1;
		l_nRows--;
		if(l_nRows>=l_RealRow||l_RealRow==0)
		   return 0;
		strcpy(l_sStockCode,m_NineShowData[m_nFocusCount].pNineData[l_nRows].pItem->id);
        nKind= m_NineShowData[m_nFocusCount].pNineData[l_nRows].pItem->kind;
       for(int i=0;i<9;i++)
          if(i!=m_nFocusCount)
            ScrollToSelectStock(i,l_sStockCode,nKind);
       ScrollToSelectStock(m_nFocusCount,l_sStockCode,nKind);
	}

	return 0;
}

void CNineShowView::OnSzeb() 
{

    OnStockTypeChoose(ID_SZEB);	
}

void CNineShowView::OnStocktype() 
{
	
    OnStockTypeChoose(ID_STOCKTYPE);	
	
}
LRESULT CNineShowView::OnGridColorChange(WPARAM wParam, LPARAM lParam)
{
   for(int i=0;i<9;i++)
   {
		m_pGridCtrl[i]->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);
		m_pGridCtrl[i]->SetFixedBkColor(m_pDoc->m_colorArray[18]);
		m_pGridCtrl[i]->Invalidate();                          
   }

//20000615
	return 0;
}

void CNineShowView::OnUpdateSync(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(m_bSync);	

}

void CNineShowView::OnUpdateShstocka(CCmdUI* pCmdUI) 
{

    int temp=pCmdUI->m_nID -ID_SHSTOCKA;
	pCmdUI->SetCheck(temp== m_nSharesType);

}


void CNineShowView::OnUpdateStocktype(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(m_nSharesType==6);
}

void CNineShowView::OnUpdateSzeb(CCmdUI* pCmdUI) 
{
	
	pCmdUI->SetCheck(m_nSharesType==7);
}
void CNineShowView::SetStockCodeForDisplayF10()
{
    int row=m_pGridCtrl[m_nFocusCount]->GetCurrentRow();
	if(row > 0 )
	{
		CString stock_code = m_NineShowData[m_nFocusCount].pNineData[row-1].pItem->id;
        if(stock_code.Compare("") == 0)
			return ;
		m_pDoc->m_sharesSymbol = stock_code;
        m_pDoc->m_stkKind=m_NineShowData[m_nFocusCount].pNineData[row-1].pItem->kind;
	}
}
CString CNineShowView::GetStockCodeForDisplayKLine()
{
    int row=m_pGridCtrl[m_nFocusCount]->GetCurrentRow();
	if(row > 0 )
	{
		CString stock_code = m_NineShowData[m_nFocusCount].pNineData[row-1].pItem->id;
		return stock_code;
	}
}
#ifdef WIDE_NET_VERSION
void CNineShowView::SendRecvDataInfomation()
{
}
#endif
