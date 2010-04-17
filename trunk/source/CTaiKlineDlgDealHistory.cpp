// CTaiKlineDlgDealHistory.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "MainFrm.h"
#include "CTaiKlineDlgDealHistory.h"
#include "CTaiKlineDlgHistorySelect.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineWideNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RATE_SHOU 100
//CTaiKlineDlgDealHistory* CTaiKlineDlgDealHistory::pWnd->m_pDlgDealHistory = NULL;
/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgDealHistory dialog

CTaiKlineDlgDealHistory::CTaiKlineDlgDealHistory(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgDealHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgDealHistory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pView = (CTaiShanKlineShowView*)pParent;
	m_bHistorySynchrostep = false;
	m_oldSymbol = "";
	m_oldStkKind = 0;
	m_fileName = _T("");
	m_fileHsSh = NULL;
	m_fileHsSz = NULL;
	m_bShowHistory = false;

	strcpy(m_S0.name ,"上证指数" );
	strcpy(m_S1.name ,"深证指数" );

	m_Tidx[0] = new Tidxd[240];
	m_Tidx[1] = new Tidxd[240];
	m_Nidx[0] = new Rsdn1[240]; 
	m_Nidx[1] = new Rsdn1[240]; 


}


void CTaiKlineDlgDealHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgDealHistory)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
UINT ID_ARRAY[]=
{
	ID_HISTORY_A_STEP,ID_HISTORY_SWITCH,ID_HISTORY_GOHEAD,ID_HISTORY_GOBACK,
	ID_HISTORY_CLOSE
};


BEGIN_MESSAGE_MAP(CTaiKlineDlgDealHistory, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgDealHistory)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTORY_HIDE, OnHistoryHide)
	ON_COMMAND(ID_HISTORY_GOHEAD, OnHistoryGohead)
	ON_COMMAND(ID_HISTORY_GOBACK, OnHistoryGoback)
	ON_COMMAND(ID_HISTORY_CLOSE, OnHistoryClose)
	ON_COMMAND(ID_HISTORY_A_STEP, OnHistoryAStep)
	ON_COMMAND(ID_HISTORY_SWITCH, OnHistorySwitch)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgDealHistory message handlers

bool CTaiKlineDlgDealHistory::InitDoFenshi(CTaiKlineMin1 *pdoFen, CString symbol,int stkKind)
{
	if(GetFilePointer (symbol,stkKind)!=NULL && (m_bShowHistory == true))//|| m_bHistorySynchrostep == true
	{

	PrepareData(pdoFen, symbol,stkKind);

	m_oldSymbol = symbol;
	m_oldStkKind = stkKind;

	CReportData* pdt;
	if(pdoFen->pDoc->m_sharesInformation .Lookup (symbol.GetBuffer (0),pdt,stkKind))
		strcpy(pdoFen->m_dt.name ,pdt->name );
	else
		strcpy(pdoFen->m_dt.name, "");
	pdoFen->m_pS0 = &this->m_S0 ;
	pdoFen->m_pS1 = &this->m_S1 ;
	pdoFen->m_footEnd = 239;
	pdoFen->m_Tidx = (Tidxd **)m_Tidx ;
	pdoFen->m_Nidx = (Rsdn1 **)m_Nidx ;


	return true;
	}

	return false;
}

void CTaiKlineDlgDealHistory::PrepareData(CTaiKlineMin1 *pdoFen,CString symbol,int stkKind)
{
	if(m_oldSymbol != symbol || m_oldStkKind != stkKind)
	{
		pdoFen->m_pFileHs = GetFilePointer(symbol,stkKind) ;
		pdoFen->InitHs (true,true);

		GetCdat1Data(symbol,stkKind, &pdoFen->m_dt,false,pdoFen);
	}
}

void CTaiKlineDlgDealHistory::OnFileChanged()
{

	int stkKind = SHZS;
	GetCdat1Data(CSharesCompute::GetIndexSymbol(0),stkKind, &m_S0);
	stkKind = SZZS;
	GetCdat1Data(CSharesCompute::GetIndexSymbol(1),stkKind, &m_S1);

}

void CTaiKlineDlgDealHistory::StringToTime(CTime &tm, CString s)
{
	int n = atoi(s);
	CTime t(n/10000,n%10000/100,n%100,8,0,0);
	tm = t;
}

void CTaiKlineDlgDealHistory::GetCdat1Data(CString symbol,int stkKind, CReportData *pDat, bool bIndex,CTaiKlineMin1 *pdoFen)
{
	CString symbolInd = symbol;
	int nSh = 0;
	if(symbolInd == CSharesCompute::GetIndexSymbol(1))
	{
		nSh = 1;
	}

	int nn = CTaiShanKlineShowView::IsIndexStock(symbol);
	if(bIndex  || nn == 1)
	{

		CBuySellList buySellList;
		CTaiKlineFileHS*	pFileHs = GetFilePointer(symbolInd,stkKind);
		if(pFileHs == NULL)
			return;

		int n = pFileHs->ReadHS2 (symbolInd,buySellList,false);

		if(n<=0)
			return ;

		for(int j=0;j<n;j++)
		{
			TRADE_DETAIL_H_PER* pHs=CTaiKlineFileHS::GetAtBS (&buySellList,j,n);
			pDat->m_Kdata1 [j].Price = pHs->price ;
			pDat->m_Kdata1 [j].Volume  = FABSMY(pHs->vol) ;
			if(j && pDat->m_Kdata1 [j].Volume<pDat->m_Kdata1 [j-1].Volume)
				pDat->m_Kdata1 [j].Volume=pDat->m_Kdata1 [j-1].Volume;
			pDat->m_Kdata1 [j].Amount  = pHs->buySellOrIndex .indexInfo .amount ;//pDat->m_Kdata1 [j].Price* pDat->m_Kdata1 [j].Volume;

			if(bIndex)
			{
				memcpy(&m_Tidx [nSh][j],&(pHs->buySellOrIndex .indexInfo .tidxd ),sizeof(Tidxd));
				m_Nidx [nSh][j].dnn = pHs->buySellOrIndex .indexInfo .decline ;
				m_Nidx [nSh][j].rsn  = pHs->buySellOrIndex .indexInfo .advance  ;
			}
			if(j==0)
				pDat->accb = pHs->buySellOrIndex .indexInfo .faccbaccs ;
			if(j==1)
				pDat->accs = pHs->buySellOrIndex .indexInfo .faccbaccs ;
		}
		GetBuySellInfo(symbol,stkKind,pDat, n-1, buySellList, true);

	
		CTaiKlineFileHS::RemoveHs (buySellList);
		return;
	}

	int n =0;
	if(pdoFen==NULL)
		return ;

	n=240;

	float fVolPre = 0;
	float fAmountPre = 0;
	for(int j=0;j<n;j++)
	{
		TRADE_DETAIL_H_PER* pHs=pdoFen->m_hsMin+j;
		pDat->m_Kdata1 [j].Price = pHs->price ;
		pDat->m_Kdata1 [j].Volume  = FABSMY(pHs->vol) ;
		pDat->m_Kdata1 [j].Amount  = fAmountPre+pDat->m_Kdata1 [j].Price* (pDat->m_Kdata1 [j].Volume-fVolPre)*RATE_SHOU;
		fVolPre = pDat->m_Kdata1 [j].Volume;
		fAmountPre = pDat->m_Kdata1 [j].Amount;

		TRACE("%s:%f\n",m_fileName,(float)pDat->m_Kdata1 [j].Price);
	}


	GetBuySellInfo(symbolInd,stkKind,pDat, n-1, pdoFen->m_hsMin1, bIndex);
}

void CTaiKlineDlgDealHistory::OnOk() 
{

	
}

void CTaiKlineDlgDealHistory::OnCancel() 
{
	if(pView!=NULL) pView->HistoryHide();

}

int CTaiKlineDlgDealHistory::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	if ( !m_wndToolBar.CreateEx(this) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;     
	}
    
	CImageList  imgList;          
	CString     str;
	const int nButton = 5;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(40, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);


	imgList.Create(IDB_51, 22, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&imgList);
	imgList.Detach();
	imgList.Create(IDB_52, 22, 0, RGB(255, 0, 255));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&imgList);
	imgList.Detach();

	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons(NULL, nButton);


	CString StringArray[]={"同步","切换","向前","向后","关闭"};
	for( int k=0;k<nButton;k++)
	{
		m_wndToolBar.SetButtonInfo(k, ID_ARRAY[k], TBSTYLE_BUTTON, k);
		str = (StringArray[k]);
		m_wndToolBar.SetButtonText(k, str);


	}

	
	CRect rectToolBar(0,0,40,45);
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(27,21));
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY| CBRS_SIZE_DYNAMIC);
	

	if(!this->m_wndToolTip.Create(this,TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE))
	{
	  return -1;
	}
    m_wndToolTip.AddTool(&m_wndToolBar,LPSTR_TEXTCALLBACK);


	if(pView !=NULL)
	{
		CRect r;
		pView->GetWindowRect (r );
		CRect rcWindow;
		GetWindowRect(rcWindow);
		CRect rc;
		GetClientRect(rc);
		r.top = r.bottom -43-(rcWindow.Height ()-rc.Height ());
		r.right = r.left + 40*5+6;
		MoveWindow(r, FALSE);
	}



	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	if(pView !=NULL) pView->SetFocus ();

    m_wndToolTip.Activate(TRUE);	
	
	return 0;
}
BOOL CTaiKlineDlgDealHistory::OnToolTipNotify(UINT id,NMHDR *pNMHDR,LRESULT *pResult)
{
  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
  UINT nID = pNMHDR->idFrom;

  if(pTTT->uFlags & TTF_IDISHWND)
  {
	  nID = ::GetDlgCtrlID((HWND)nID);
	  if(nID == 0)
		  return FALSE;
  }
  switch(nID)
  {
     case  ID_HISTORY_OPEN:
	      pTTT->lpszText = "打开历史回忆";
	      break;
 	 case  ID_HISTORY_A_STEP:
          pTTT->lpszText = "同步历史回忆";
	      break;
     case ID_HISTORY_SWITCH:
		  pTTT->lpszText = "切换";
	      break;
	 case ID_HISTORY_GOHEAD:
		   pTTT->lpszText = "向前";
	      break;
	 case ID_HISTORY_GOBACK:
		  pTTT->lpszText = "向后";
	      break;
	 case ID_HISTORY_HIDE:
		  pTTT->lpszText = "隐藏";
	      break;
     case ID_HISTORY_CLOSE:
          pTTT->lpszText = "关闭历史回忆";
	      break;
  }
  return TRUE;
} 


void CTaiKlineDlgDealHistory::OnClose() 
{

	if(pView!=NULL) pView->HistoryHide();

}

bool CTaiKlineDlgDealHistory::OnDoGo(bool bGoHead, bool bDoReal)
{
	CStringArray sArray;
	CTaiKlineDlgHistorySelect::GetFileNameArray(sArray);
	if(sArray.GetSize ()<=0)
		return false;

	if(bGoHead== true)
	{
		CString s = sArray[0];
		if(bDoReal == true)
		{
			if(s == this->m_fileName) return false;

			for(int i = 0 ;i< sArray.GetSize ();i++)
			{
				if(this->m_fileName <= sArray[i] && this->m_fileName!=s)
				{
					this->m_fileName =s;
			
					OpenFile();

					return true;
				}
				s=  sArray[i];
			}
		}
		else 
		{
			if(sArray.GetSize ()>0)
			{
				if(m_fileName>sArray[0])
					return true;
			}
			return false;
		}
	}
	else
	{

		if(bDoReal == true)
		{
			for(int i = 0 ;i< sArray.GetSize ();i++)
			{
				if(this->m_fileName < sArray[i] )
				{
					this->m_fileName =sArray[i];
			
					OpenFile();

					return true;
				}

			}
		}
		else 
		{
			if(sArray.GetSize ()>0)
			{
				if(m_fileName<sArray[sArray.GetSize ()-1])
					return true;
			}
			return false;
		}
	}


	return false;
}

void CTaiKlineDlgDealHistory::OpenFile()
{

	if(m_fileHsSh!=NULL)
		delete m_fileHsSh;
	if(m_fileHsSz!=NULL)
		delete m_fileHsSz;
	m_fileHsSh = new CTaiKlineFileHS(SH_MARKET_EX);
	m_fileHsSz = new CTaiKlineFileHS(SZ_MARKET_EX);

	m_oldSymbol = "";
	CString sDate = m_fileName;
	if(m_fileName!=_T(""))
	{
		CString sPath = FILE_PATH_HSH_SH;
		CString filename =sPath + m_fileName +".hst";
		if(!m_fileHsSh->Open (filename,0))
		{
			delete m_fileHsSh;
			m_fileHsSh = NULL;
		}

		sPath = FILE_PATH_HSH_SZ;
		filename =sPath + m_fileName +".hst";
		if(!m_fileHsSz->Open (filename,0))
		{
			delete m_fileHsSz;
			m_fileHsSz = NULL;
		}
	}
	

	if(pView!=NULL)
	{
		if(pView->m_pWideNet) pView->m_pWideNet->AddStockHsHistoryFileData(pView->m_sharesSymbol,pView->m_stkKind, sDate);//AddStockFirst(pView->m_sharesSymbol);
	}

	OnFileChanged();
}

void CTaiKlineDlgDealHistory::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_fileHsSh!=NULL)
		delete m_fileHsSh;
	if(m_fileHsSz!=NULL)
		delete m_fileHsSz;
	
	m_fileHsSh=NULL;
	m_fileHsSz=NULL;

	delete m_Tidx[0];
	delete m_Tidx[1];
	delete m_Nidx[0];
	delete m_Nidx[1];
	
}

bool CTaiKlineDlgDealHistory::HistoryOpen(CTaiShanKlineShowView* pWnd,CString * fileName)
{
	if(pWnd->m_pDlgDealHistory == NULL)
	{
		CTaiKlineDlgHistorySelect select;
		bool bOk = false;
		if(fileName!=NULL)
		{
			bOk = true;
			CStringArray sArray;
			CTaiKlineDlgHistorySelect::GetFileNameArray(sArray);
			int  n = sArray.GetSize();
			if(n<=0) return false;
			select.m_fileName = "";
			for(int i=0;i<n;i++)
			{
				if(sArray[i]>=*fileName)
				{
					select.m_fileName = sArray[i];
					break;
				}
			}
			if(select.m_fileName =="")
			{
				select.m_fileName  = sArray[n-1];
			}

			if(*fileName != select.m_fileName)
				AfxMessageBox("将显示"+select.m_fileName+"日的历史回忆数据！");


		}
		else
		{
			if(select.DoModal () == IDOK)
				bOk = true;
		}

		if(	bOk == true)
		{
			if(select.m_fileName!="")
			{
			pWnd->m_pDlgDealHistory = new CTaiKlineDlgDealHistory(pWnd);
			pWnd->m_pDlgDealHistory->m_fileName = select.m_fileName ;
			pWnd->m_pDlgDealHistory->m_bShowHistory = true;

			///b
			pWnd->m_pDlgDealHistory->OpenFile();
			////e

			pWnd->m_pDlgDealHistory->Create (CTaiKlineDlgDealHistory::IDD);
			pWnd->m_pDlgDealHistory->ShowWindow (SW_SHOW);
			pWnd->m_pDlgDealHistory->SetWindowText("历史回忆("+pWnd->m_pDlgDealHistory->m_fileName+")");
			return true;
			}
		}
	}
	return false;

}

bool CTaiKlineDlgDealHistory::HistoryAStep(int time)
{
	CStringArray sArray;
	CTaiKlineDlgHistorySelect::GetFileNameArray(sArray);
	if(sArray.GetSize ()<=0)
		return false;

	CTime tm(time);
	CString stm = tm.Format("%Y%m%d");
	for(int i=0;i<sArray.GetSize ();i++)
	{
		if(stm == sArray[i])
		{
			m_fileName = stm;
			m_oldSymbol =="";
			OpenFile();
			return true;
		}
	}

	return false;
}

bool CTaiKlineDlgDealHistory::GetBuySellInfo(CString symbol,int stkKind,CReportData* pDat, int nFoot, CBuySellList& buySellList, bool bIndex)
{
	CTaiKlineFileKLine* pFileK = CTaiKlineFileKLine::GetFilePointer (symbol,stkKind,true);
	Kline* kline = NULL;
	CTime tm;
	StringToTime(tm,m_fileName);
	int n = pFileK->ReadKlineAccordingTime(symbol, kline, tm,6); 
	ASSERT(n<=6);

	int n2 = buySellList.GetCount ();
	if(n2==0)
	{
		if(kline!=NULL) delete [] kline;
		return false;
	}

	if(n==0)
	{
		memset(pDat,0,sizeof(CReportData));
	}
	else
	{
		if(n-2>=0)
			pDat->ystc  = kline[n-2].close ;
		else
			pDat->ystc  = kline[n-1].open ;
		pDat->opnp   = kline[n-1].open  ;
	}
	pDat->volume5  = 0;
	for(int i=0;i<n-1;i++)
		pDat->volume5 += kline[i].vol ;
	pDat->volume5/=5;

	n = n2;
	if(kline!=NULL) delete [] kline;

	return GetBuySellInfoPerFoot(symbol, pDat, nFoot, buySellList, bIndex);
}


bool CTaiKlineDlgDealHistory::GetBuySellInfoPerFoot(CString symbol, CReportData *pDat, int nFoot, CBuySellList &buySellList, bool bIndex)
{
	int n = buySellList.GetCount ();
	TRADE_DETAIL_H_PER* pHs=CTaiKlineFileHS::GetAtBS (&buySellList,0,n);
	pDat->higp = pHs->price ;
	pDat->lowp = pDat->higp;
	pDat->dvol = 0;
	pDat->rvol =0; 

	pHs = CTaiKlineFileHS::GetAtBS (&buySellList,nFoot,n);
	TRADE_DETAIL_H_PER* pHsPre = NULL;
	if(nFoot>0)
		pHsPre = CTaiKlineFileHS::GetAtBS (&buySellList,nFoot-1,n);
	CTaiKlineFileHS::Cdat1ToHs(pDat, pHs,false, pHsPre);
	if(bIndex==false)
	{
		pDat->totp = 0;
		pDat->totv = 0;
	}
	else
	{
		pDat->totp = pDat->m_Kdata1 [nFoot].Amount ;
		pDat->totv = pDat->m_Kdata1 [nFoot].Volume ;
	}
	
	if(bIndex==false)
	{
		pDat->accb = pDat->vbuy1 + pDat->vbuy2 + pDat->vbuy3;
		pDat->accs = pDat->vsel1 + pDat->vsel2 + pDat->vsel3;
	}
	
	pHsPre = NULL;
	for(int i=0;i<nFoot+1;i++)
	{
		pHs = CTaiKlineFileHS::GetAtBS (&buySellList,i,n);

		if(pDat->higp < pHs->price )
			pDat->higp = pHs->price;
		if(pDat->lowp > pHs->price )
			pDat->lowp = pHs->price ;
		int nFlag ;
		float vol = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
		if(nFlag == 1)
			pDat->rvol += (vol);
		else if(nFlag == -1)
			pDat->dvol += (vol);
		else
		{
			pDat->rvol += (vol/2);
			pDat->dvol += (vol/2);
		}

		if(bIndex==false)
		{
			pDat->totp += vol*pHs->price ;
			pDat->totv += vol;
		}

		pHsPre = pHs;
	}
	if(bIndex==false)
		pDat->totp*=100;

	

	return true;

}

void CTaiKlineDlgDealHistory::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);

}

void CTaiKlineDlgDealHistory::OnMouseMove(UINT nFlags, CPoint point) 
{

	
	CDialog::OnMouseMove(nFlags, point);
}

void CTaiKlineDlgDealHistory::OnHistoryHide() 
{

}


void CTaiKlineDlgDealHistory::OnHistoryGohead() 
{

	if(OnDoGo(true,true))
	{
		pView->pKlineDrawing->m_bToCacl =TRUE;
		pView->pKlineDrawing ->m_bInitFoot =true;//
		pView->RedrawWindow();
	}
	SetWindowText("历史回忆("+m_fileName+")");
	pView->SetFocus();
	
}


void CTaiKlineDlgDealHistory::OnHistoryGoback() 
{
	
	if(OnDoGo(false,true))
	{
		pView->pKlineDrawing->m_bToCacl =TRUE;
		pView->pKlineDrawing ->m_bInitFoot =true;//
		pView->RedrawWindow();
	}
	SetWindowText("历史回忆("+ m_fileName+")");
	pView->SetFocus();
	
}

void CTaiKlineDlgDealHistory::OnHistoryClose() 
{
	pView->HistoryHide();

}

void CTaiKlineDlgDealHistory::OnHistoryAStep() 
{
	
	 m_bHistorySynchrostep == true? m_bHistorySynchrostep = false: m_bHistorySynchrostep = true;
	 m_bShowHistory  = false;

	pView -> pMin1Drawing->m_bHist =false;
	SetWindowText("历史回忆");
	pView -> pMin1Drawing->m_sOldSymbol ="";
	if(m_bHistorySynchrostep == true)
	{
		if(pView->m_nKlineKind2 == 0) pView->m_nKlineKind2 = 5; 
		pView->ShowAll(pView -> m_sharesSymbol);
	}
	else
	{
		pView -> pMin1Drawing->InitMinuteLine();
		pView -> RedrawWindow();
	}
	pView -> SetFocus();

	int nState = m_wndToolBar.GetToolBarCtrl().GetState( ID_ARRAY[0]);
	m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[0],m_bHistorySynchrostep == true
		? nState|TBSTATE_CHECKED:  nState&~TBSTATE_CHECKED);//TBSTATE_CHECKED   
	
	nState = m_wndToolBar.GetToolBarCtrl().GetState( ID_ARRAY[1]);
	m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[1],m_bShowHistory == true
		? nState|TBSTATE_CHECKED:  nState&~TBSTATE_CHECKED);//TBSTATE_CHECKED   
	
}


void CTaiKlineDlgDealHistory::OnHistorySwitch() 
{
	
	if(m_bHistorySynchrostep == true ) return;

	 m_bShowHistory == true? m_bShowHistory = false: m_bShowHistory = true;
	if( m_bShowHistory == true)
	{
		 m_oldSymbol = "";
		pView -> pMin1Drawing->m_bHist =true;
		 SetWindowText("历史回忆("+ m_fileName+")");
	}
	else
	{
		pView -> pMin1Drawing->m_sOldSymbol ="";
		pView -> pMin1Drawing->m_bHist =false;
		 SetWindowText("历史回忆");
	}
	pView -> pMin1Drawing->InitMinuteLine();


	if(pView!=NULL)
	{
		if(pView->m_pWideNet)
		{
			pView->m_pWideNet->SetHsHistoryRequestFlag(false);

			pView->m_pWideNet->AddStockFirst(pView->m_sharesSymbol,pView->m_stkKind);
		}
	}

	pView -> RedrawWindow();
	pView -> SetFocus();

	int nState = m_wndToolBar.GetToolBarCtrl().GetState( ID_ARRAY[1]);
	m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[1],m_bShowHistory == true
		? nState|TBSTATE_CHECKED:  nState&~TBSTATE_CHECKED); 
	
}


BOOL CTaiKlineDlgDealHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	for(int  k=0;k<5;k++)
	{
		m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[k],TBSTATE_ENABLED );//TBSTATE_CHECKED   
	}

	int nState = m_wndToolBar.GetToolBarCtrl().GetState( ID_ARRAY[1]);
	m_wndToolBar.GetToolBarCtrl().SetState( ID_ARRAY[1],m_bShowHistory == true
		? nState|TBSTATE_CHECKED:  nState&~TBSTATE_CHECKED);  


	return TRUE;  
}

void CTaiKlineDlgDealHistory::SetCheck1(int n)
{
}

BOOL CTaiKlineDlgDealHistory::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
}
CTaiKlineFileHS* CTaiKlineDlgDealHistory::GetFilePointer (CString symbol,int stkKind)
{
	CTaiKlineFileHS*	pFileHs;

	pFileHs=m_fileHsSh ;
	if(SZ_MARKET_EX == CSharesCompute::GetMarketKind(stkKind))
		pFileHs=m_fileHsSz ;

	return pFileHs;

}