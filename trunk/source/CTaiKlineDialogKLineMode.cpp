// CTaiKlineDialogKLineMode.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDialogKLineMode.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogKLineMode dialog


CTaiKlineDialogKLineMode::CTaiKlineDialogKLineMode(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDialogKLineMode::IDD, pParent)
{
	pView = (CTaiShanKlineShowView* )pParent;
	ASSERT(pView!=NULL);
	//{{AFX_DATA_INIT(CTaiKlineDialogKLineMode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTaiKlineDialogKLineMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDialogKLineMode)
	DDX_Control(pDX, IDC_LIST_KLINE_MODE_L, m_kline_mode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDialogKLineMode, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDialogKLineMode)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_KLINE_MODE_L, OnKeydownListKlineModeL)
	ON_NOTIFY(NM_CLICK, IDC_LIST_KLINE_MODE_L, OnClickListKlineModeL)
	ON_WM_PAINT()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST_KLINE_MODE_L, OnKillfocusListKlineModeL)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogKLineMode message handlers

void CTaiKlineDialogKLineMode::OnOK() 
{

	return;

}

BOOL CTaiKlineDialogKLineMode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	SetWindowText("KÏß×éºÏ");
	
	CBitmap       m_bitmap;

    m_imagelist.Create(15,15, ILC_MASK, 2, 2);
    
	m_bitmap.LoadBitmap(IDB_BITMAP_137);
	m_imagelist.Add(&m_bitmap, RGB(255,255,255));


	m_kline_mode.SetImageList(&m_imagelist,LVSIL_SMALL);  


	LVITEM lvi;

	for(int i=0;i<pDoc->m_formuar_kline .GetSize();i++)
	{
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE ;
		lvi.iItem = i;
		lvi.iSubItem = 0;

		int len=pDoc->m_formuar_kline[i]->explainBrief.GetLength();
		int len2=pDoc->m_formuar_kline[i]->name .GetLength();
		char* ch=new char[len+2+len2];
		CString s = pDoc->m_formuar_kline[i]->name+" " +pDoc->m_formuar_kline[i]->explainBrief;
		strcpy(ch,s);

		lvi.pszText =ch;
		lvi.iImage = 0;
		lvi.stateMask = LVIS_FOCUSED|LVIS_SELECTED|LVIS_STATEIMAGEMASK ;//INDEXTOSTATEIMAGEMASK(1)|
		lvi.state = INDEXTOSTATEIMAGEMASK(2) ;

		m_kline_mode.InsertItem(&lvi);
		POINT p;
		p.x=2;
		p.y=i*16+3;
		
		m_kline_mode.SetItemPosition(i,p);

		delete[] ch;
	}


	m_kline_mode.SetItemState (0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED );//|LVIS_SELECTED

	return TRUE;  
}

void CTaiKlineDialogKLineMode::DrawKLineMode(CTaiShanKlineShowView* pView,CDC* pDC)
{
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 

	if(!pView->pKlineDrawing ->ValidDoKline()) return;
	ARRAY_BE pline;
	pline.line=new float[pView->m_nCountKline +241];
	memset(pline.line,0,(pView->m_nCountKline +241)*4);
	CBitmap       bitmap2;
	bitmap2.LoadBitmap(IDB_BITMAP_137);


	CString name;


	if(pDoc->m_formuar_kline.GetSize ()<=0)
		return;

	int nItem=0;
	POSITION pos = m_kline_mode.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
	   TRACE0("No items were selected!\n");
	   return;
	}
	else
	{
	   nItem = m_kline_mode.GetNextSelectedItem(pos);
	   TRACE1("Item %d was selected!\n", nItem);
	}

	name=pDoc->m_formuar_kline[nItem]->name;
	CaclKLineMode(pView,name,pline);

	//---	
	int bgn=pView->pKlineDrawing ->m_footBegin;
	int end=pView->pKlineDrawing ->m_footEnd;

	pView->pKlineDrawing ->m_nSon=0;
	pView->pKlineDrawing->SetRectCurrent(0);

	if(pline.b>bgn)
		bgn = pline.b;

	for(int i=bgn;i<=end;i++)
	{
		if(pline.line[i]<0.1)
			continue;
		int y=pView->pKlineDrawing ->YTransfer(pView->m_pkline [i].low);
		POINT p;
		p.y=y+2;
		if(p.y>pView->pKlineDrawing ->m_rectDrawLine.bottom-18)
			p.y = pView->pKlineDrawing ->m_rectDrawLine.bottom-18;
		if(p.y<pView->pKlineDrawing ->m_rectDrawLine.top)
			p.y = pView->pKlineDrawing ->m_rectDrawLine.top;
		p.x=(long)((i-pView->pKlineDrawing ->m_footBegin+0.5)*pView->m_rtKlineFiguer.rightX/pView->pKlineDrawing ->m_klinNumDefault-7);
		if(pDC->IsPrinting ())
		{
			CPoint p2(p);
			pView->pKlineDrawing ->OutBitmap(pDC,IDB_BITMAP_137,p2);
		}
		else
		{
		m_imagelist.Draw (pDC ,0,p,ILD_TRANSPARENT);
		}
	}

	delete[] pline.line;
	
}

void CTaiKlineDialogKLineMode::CaclKLineMode(CTaiShanKlineShowView* pView,CString name,ARRAY_BE& pline)
{
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 

	CFormularContent*	pIndex;
	pIndex=(CFormularContent*)pDoc->LookUpArray (pDoc->m_formuar_kline,name); 
	if(pIndex==NULL)
	{
		return;
	}

	CFormularCompute* pEqution = NULL;
	INPUT_PARAM input;
	input.pmax			=pView->m_nCountKline-1;
	input.pp			= pView->m_pkline;
	input.symbol		= pView->m_sharesSymbol ;
	input.nKindKline	= pView->m_nKlineKind2 ;
	input.pBuySellList	=&( pView->m_hs) ;
	input.pIndex		= pIndex;
	input.bAddBuySell   = false;
	input.m_stkKind		= pView->m_stkKind;
	if(ComputeFormu2(pEqution,input))
	{
		ASSERT(FALSE);
	}
	else
	{
		m_numLine = pEqution->GetRlineNum ();
		if(m_numLine!=1)
			return;
		CString namePer;int in=0;
		pEqution->GetLine (in,pline,namePer);
	}
	delete pEqution;
}

void CTaiKlineDialogKLineMode::OnClose() 
{

	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	CTaiShanKlineShowView* pView2=pView;
	DestroyWindow();
	delete pView->pKlineMode ;
	pView2->pKlineMode=NULL ;
	pView2->RedrawWindow ();

}

void CTaiKlineDialogKLineMode::OnKeydownListKlineModeL(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	

	pView->Invalidate();

	
	*pResult = 0;
}

void CTaiKlineDialogKLineMode::OnClickListKlineModeL(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	pView->Invalidate();
	*pResult = 0;
}

void CTaiKlineDialogKLineMode::OnPaint() 
{
	CPaintDC dc(this); 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	CMDIChildWnd *pChild =(CMDIChildWnd *) pFm->GetActiveFrame();
	CView* pView1=(CView*)pChild->GetActiveView();
	if(!pView1->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
		ShowWindow(SW_HIDE);
	}
	
}

void CTaiKlineDialogKLineMode::OnKillfocusListKlineModeL(NMHDR* pNMHDR, LRESULT* pResult) 
{

	int nItem=0;

	{

	}


}

void CTaiKlineDialogKLineMode::OnCancel()
{
   SendMessage(WM_CLOSE);
}

BOOL CTaiKlineDialogKLineMode::PreTranslateMessage(MSG* pMsg) 
{


	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		 if(pMsg->wParam != VK_PRIOR && pMsg->wParam != VK_NEXT &&
			 pMsg->wParam != VK_HOME &&pMsg->wParam != VK_END &&
			 pMsg->wParam != VK_UP &&pMsg->wParam != VK_LEFT &&
			 pMsg->wParam != VK_DOWN &&pMsg->wParam != VK_RIGHT
			 )
		 {
			CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

			pView->SetFocus();
			::SendMessage(pView->m_hWnd,pMsg->message,pMsg->wParam,pMsg->lParam);
			 return TRUE;
		 }
	}
	if(pMsg->message == WM_CHAR)
	{
		 if(pMsg->wParam != VK_PRIOR && pMsg->wParam != VK_NEXT &&
			 pMsg->wParam != VK_HOME &&pMsg->wParam != VK_END &&
			 pMsg->wParam != VK_UP &&pMsg->wParam != VK_LEFT &&
			 pMsg->wParam != VK_DOWN &&pMsg->wParam != VK_RIGHT
			 )
		 {
			pView->SetFocus();
			::SendMessage(pView->m_hWnd,pMsg->message,pMsg->wParam,pMsg->lParam);
			 return TRUE;
		 }
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTaiKlineDialogKLineMode::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
}

void CTaiKlineDialogKLineMode::Set(CFormularContent *pJishu)
{
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 


	if(pDoc->m_formuar_kline.GetSize ()<=0)
		return;
	int i;
	for(i = 0;i<pDoc->m_formuar_kline.GetSize ();i++)
	{
		if(pJishu == pDoc->m_formuar_kline[i])
			break;
	}
	if(i == pDoc->m_formuar_kline.GetSize ())
		return;

	m_kline_mode.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);

}
