// CTaiScreenParent.cpp : implementation file
//

#include "stdafx.h"
#include "StructTaiShares.h"
#include "CTaiShanApp.h"
#include "CTaiScreenParent.h"
#include "CTaiShanApp.h"
#include "CFormularCompute.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CFormularContent.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineFileKLine.h"
#include "DIALOGGSSM.h"
#include "SelectStock.h"
#include "TransactFee.h"
#include "KeyBrodEangel.h"
#include "CTaiKlineDoKline.h"
#include "CAlertSystem.h"
#include "CTaiKlineDlgYHParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CImageList CTaiScreenParent::m_imageList ;
CFont CTaiScreenParent::m_Font;
ARRAY_BE CTaiScreenParent::m_gLineOut;
CString CTaiScreenParent::m_gPeriodName[]={"换手","1分钟线","5分钟线","15分钟线","30分钟线",
								"60分钟线","日线","周线","月线","年线",
								"多日线"};
int CTaiScreenParent::nLKind[]={
		HS_KLINE,
		MINUTE1_KLINE,
		MIN5_KLINE,
		MIN15_KLINE,
		MIN30_KLINE,
		MIN60_KLINE,
		DAY_KLINE,
		WEEK_KLINE,
		MONTH_KLINE,
		YEAR_KLINE ,
		MANY_DAY_KLINE,
		 0
};
CString CTaiScreenParent::m_subIndexGroup[] = {"大于参数1",
		                       "小于参数1",
	                           "等于参数1",
							   "上穿参数1",
							   "下破参数1",
                               "大于参数1小于参数2",
							   "维持参数1周期后上穿参数2",
							   "维持参数1周期后下破参数2"
	};
static int nFootFinance[100] = {0,1,2,3,8,
								7,6,4,9,10,
								11,13,14,12,15,
								16,18,22,17,23,
								24,25,26,-1,-1,
								-1,-1,-1,26,-1,
								27,28,31,29,32,
								21,30,2,3,4,
			};


CFormularContent*	CTaiScreenParent::m_gpIndex[] = {NULL,NULL,NULL,NULL,NULL};


CTaiScreenParent::CTaiScreenParent(int nIDD,CWnd* pParent /*=NULL*/)
	: CDialog(nIDD, pParent)
{
	m_bXgnExt = false;

	m_bCaclYHParam = FALSE;
	memset(m_nLineType,0,sizeof(int)*64);
	m_tmHistory = 0;
	//{{AFX_DATA_INIT(CTaiScreenParent)
	m_and_or = 0;
	//}}AFX_DATA_INIT
//	m_bExecuteCalc = false;
	m_bDlgTest = false;
	m_pStockSelectList = NULL;
	static bool bCreateImg = false;
	if(bCreateImg == false)
	{
		m_imageList.Create(IDB_BITMAP12,16,0,RGB(192,192,192));
		LOGFONT m_lf;
		memset(&m_lf,0,sizeof(LOGFONT));
		m_lf.lfHeight = -13;
		m_lf.lfWidth=0;
		m_lf.lfWeight=400;
		m_lf.lfCharSet=134;
		strcpy(m_lf.lfFaceName,"宋体");
		VERIFY(m_Font.CreateFontIndirect(&m_lf));
		bCreateImg = true;
	}

	m_pExplainButton = NULL;

	m_bDlgIndicator = false;
	m_bShowBuyCase = false;
	m_bBuy = 1;
	pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
	this->m_pIndex = NULL;

	CFile fl;
	if(fl.Open ("buySellAdditional.dat",CFile::modeRead))
	{
		if(fl.GetLength ()>=sizeof(ADDITIONAL_BUYSELL))
		{
			fl.Read (&buySellAdditional,sizeof(ADDITIONAL_BUYSELL));
		}
		fl.Close ();
	}

}


void CTaiScreenParent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreenParent)
	DDX_Control(pDX, IDC_RICHEDIT1, m_conRich);


	DDX_Control(pDX, IDC_LIST_SUB_FORMULAR, m_sSubFormular);
	DDX_Control(pDX, IDC_LIST_FORMULAR_LINE_ALL, m_sLineAll);
	DDX_Control(pDX, IDC_CHOOSE_LIST, m_choose_list);

	DDX_Control(pDX, IDC_COMBO_PERIOD, m_period);
	DDX_Control(pDX, IDC_TREE_FORMULAR_ALL, m_treeFormularCtrl);

	DDX_Radio(pDX, IDC_RADIO_AND, m_and_or);
	DDX_Control(pDX, IDC_ADD, m_btAddCond   );
	DDX_Control(pDX, IDC_DELETE, m_btDeleteCond);
	DDX_Control(pDX, IDC_LOAD, m_btOpenGroup );
	DDX_Control(pDX, IDC_SAVE, m_btSaveGroup );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreenParent, CDialog)
	//{{AFX_MSG_MAP(CTaiScreenParent)
	ON_NOTIFY(NM_CLICK, IDC_TREE_FORMULAR_ALL, OnClickTreeFormularAll)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FORMULAR_ALL, OnSelchangedTreeFormularAll)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_CBN_SELCHANGE(IDC_LIST_SUB_FORMULAR, OnSelchangeListSubFormular)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RETURN, IDC_TREE_FORMULAR_ALL, OnReturnTreeFormularAll)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_FORMULAR_ALL, OnDblclkTreeFormularAll)
	ON_CBN_SELCHANGE(IDC_LIST_FORMULAR_LINE_ALL, OnSelchangeListFormularLineAll)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SELECT, OnSelchangeTabSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




int CTaiScreenParent::ReadKLine(CString symbol,int stkKind, Kline *&pKline, int nRead, int nKlineType,CBuySellList* pBuySellList, int nAddBlank ,bool bToday,CTime * tmEnd,CTime *tmBegin)
{
	if(tmEnd !=NULL && CTaiKlineFileKLine::IsNeedHs(nKlineType))
	{
		{
		
			CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
			int n = 0;

			CString sPath = FILE_PATH_HSH_SH;
			int nMarket = CSharesCompute::GetMarketKind(stkKind );
			if(nMarket == SZ_MARKET_EX ) 
			{
				sPath = FILE_PATH_HSH_SZ;
			}
			CBuySellList* bsList=NULL;
			{
				CTaiKlineFileHS fileHs(nMarket);
				CString m_fileName=tmEnd->Format("%Y%m%d");
				CFileFind find;
				CString filename =sPath + m_fileName +".hst";
				if(find.FindFile(filename)) 
					if(m_fileName!=_T(""))
					{
						if(fileHs.Open (filename,0))
						{
							fileHs.ReadHS2(symbol,*pBuySellList,true);
						}
					}
			}

			CTaiKlineTransferKline trans(pBuySellList);
			bToday = true;
			trans.TransferKlineHistory(symbol,stkKind,pKline, n,nKlineType,bToday);//used to transfer k line data to another kind of k line

			if(pKline == NULL) 
			{
				pKline = new Kline;
				return 0;
			}
			ASSERT(n>=0);
			return n;
		}
	}
	else
		return CTaiKlineFileKLine::ReadKLineAny(symbol,stkKind, pKline,  nRead,  nKlineType,pBuySellList, nAddBlank,  bToday,tmEnd,tmBegin);
}


CFormularContent* CTaiScreenParent::LookUpArray(int iKind, CString sIndex,CTaiShanDoc* pDoc)
{

	CFormularContent*	pIndex = NULL;
	switch(iKind)
	{
	case CTaiKlineTreeCtl::nKindIndex:
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_index,sIndex)); 
		break;
	case CTaiKlineTreeCtl::nKindChoose:
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_choose,sIndex)); 
		break;
	case CTaiKlineTreeCtl::nKindKline:
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_kline,sIndex)); 
		break;
	case CTaiKlineTreeCtl::nKindPriceAlert://
	case CTaiKlineTreeCtl::nKindUpDownAlert://
		pIndex = GetSpecialLJishu(-iKind-7);
		break;
	}
	return pIndex;
}



IndexDataInfo* CTaiScreenParent::SetCurrentFormular()
{

	IndexDataInfo* p;
	return p;

}



float CTaiScreenParent::FormularComputeShares(CString symbol,int stkKind,int kCounts, IndexDataInfo* indexSv,bool& bSucc,Kline *&pKline,CBuySellList* pBuySellList ,bool bTjxg,ARRAY_BE* pLine)//to FormularCompute Each Stock and each formular
{
	CFormularContent*	pIndex;
	CFormularContent	jiShuTemp;
	bool bNewJishu = true;
	if(indexSv->iKind >-1 && indexSv->iKind <=-4)
		bNewJishu = false;
	if(indexSv->iKind == -1 && bTjxg == false)
		bNewJishu = false;

	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc ;

	pIndex=(CFormularContent*)LookUpArray(indexSv->iKind, indexSv->sIndex_name,pDoc); 
	if(pIndex==NULL && indexSv->iKind != CTaiKlineTreeCtl::nKindBaseInfo)
	{
		return 0;
	}

	if((indexSv->iKind == -1 || indexSv->iKind == CTaiKlineTreeCtl::nKindBaseInfo) && bTjxg == true )
	{
		jiShuTemp.name = "temp";
		jiShuTemp.numPara =0;
		jiShuTemp.fomular = GetGuoteIndex(indexSv,pIndex);//GetGuoteIndex(IndexDataInfo* indexSv)
		pIndex = &jiShuTemp;
	}

	if(pIndex!=NULL)
		if(!pIndex->IsValid()) return 0;

    CFormularCompute *m_formuCompute =new CFormularCompute(0,kCounts-1,pKline,pIndex,symbol,stkKind ,pBuySellList,nLKind[indexSv->iDataType] );
	//--
	int numPa=pIndex->numPara ;
	for(int j=0;j<numPa;j++)
	{
		m_formuCompute->AddPara (pIndex->namePara [j],indexSv->fParam[j]);
	}

	float nPoint=0;
	if(m_formuCompute->FormularCompute()==0)
	{
		int rtnLine=0;
		CString s;
		if(indexSv->iLine<=m_formuCompute->GetRlineNum () || bTjxg == true)
		{
			int nLn = bTjxg == false ? indexSv->iLine:0;
			if(pLine ==NULL)
			{
				if(m_formuCompute->GetIndexPoint(nLn,nPoint)!=0)
  				  nPoint=0;
			}
			else
			{
				 if(m_formuCompute->GetLine (nLn,*pLine,s)!=0)
					  nPoint=-1;
			}
		}		
	}
	delete m_formuCompute;
	return nPoint;
}

BOOL CTaiScreenParent::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->InsertItem( 0, "分类");
	((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->InsertItem( 1, "全部");
	((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->InsertItem( 2, "常用");

	m_btAddCond.SetIcon(IDI_ICON_ADD2);
	m_btDeleteCond.SetIcon(IDI_ICON_DELETE);
	m_btOpenGroup.SetIcon(IDI_ICON_OPEN);
	m_btSaveGroup.SetIcon(IDI_ICON_SAVE);

	m_treeFormularCtrl.m_bNewGroup			=false;
	m_treeFormularCtrl.m_bDeleteGroup		=false;
	m_treeFormularCtrl.m_bModifyGroupName	=false;
	m_treeFormularCtrl.m_bDeleteEquation	=false;
	m_treeFormularCtrl.m_bNewEquation		=false;
#ifdef OEM
	m_treeFormularCtrl.m_bModifyEquation	=false;
#endif

	if(m_pExplainButton != NULL)
		m_pExplainButton->ShowWindow(SW_HIDE);

	pEdit = NULL;

	GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LIST_SUB_FORMULAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_HIDE);


	if(m_pStockSelectList!=NULL)
	{
		m_pStockSelectList->SetExtendedStyle(m_pStockSelectList->GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	}

	return TRUE;  
}

bool CTaiScreenParent::IsSuccess(CString StockId,int stkKind,int * pnBuyResult,int * pnSellResult, int nB,int nCount, Kline *pKline, int nRange, float fProfit,int bBuySell,CTime* tmEnd ,CTime* tmBegin )
{
	int tmB = 0;
	if(tmBegin)
		tmB = CTime(tmBegin->GetYear (),tmBegin->GetMonth (),tmBegin->GetDay (),0,0,0).GetTime();
	int tmE =0;
	if(tmEnd)
		tmE = CTime(tmEnd->GetYear (),tmEnd->GetMonth (),tmEnd->GetDay (),23,59,59).GetTime();


	float MidPrice;
	float HighPrice;
	int HighPriceItem;
	float profit;
	int bKindSell=0;
	int nTimelen;
	int bSuccCount=0;
	SCREEN_DETAIL detail;
	CTransactFee tf;
	SymbolKind sm;
	strcpy(sm.m_chSymbol ,StockId);
	sm.m_nSymbolKind = stkKind;
	float fee=tf.GetCommission(sm)*100;
	if(fee<0.01||fee>100)
		fee=0.75;

	SCREEN_RESULT  *chooseresult=new SCREEN_RESULT;
	nTimelen=nRange;

	int nMoveAdd = 0;
	CaclPrice(0, this->buySellAdditional .nEnterPoint [0], nMoveAdd);
	switch(bBuySell)
	{
		case 0:
			{
				for(int i=nB;i<nCount;i++)
				{
					if(pnBuyResult[i]==1)
					{
						TRACE("is = %d\n",i);
						int bSucc=10;
						int nMove;
						int nEnterPoint = this->buySellAdditional .nEnterPoint [0];
						if(i==nCount) nEnterPoint = 0;
						MidPrice=CaclPrice(pKline+i,nEnterPoint , nMove);

						if(i ==nCount-1 && nMove>0)
							MidPrice=CaclMid(pKline[i]);
						if((i+nRange)>nCount)
						{
							nTimelen=nCount-i;
							bSucc=2;
						}
						if(i<(nCount-1-nMoveAdd))
						{
						    HighPriceItem=i+1+nMoveAdd;

							HighPrice=CaclMid(pKline[i+1+nMoveAdd]);
							for(int bb=(i+1+nMoveAdd);bb<i+nTimelen;bb++)
							if(CaclMid(pKline[bb])>HighPrice)
							{
								HighPriceItem=bb;

								HighPrice=CaclMid(pKline[bb]);
							}



						
						}
						else 
						{
							HighPriceItem=i;
							HighPrice=CaclMid(pKline[i]);
						}
						
						profit=(HighPrice-MidPrice)/MidPrice;
						if(bSucc!=2)
						{
							if(HighPriceItem==i)
								bSucc=1;
							else if((profit/(HighPriceItem-i))>=(fProfit/nRange))
							{
								bSucc=0;
								bSuccCount++;
							}
							else 
								bSucc=1;
						}
						int nCurr = i+nMoveAdd;
						if(nCurr>=nCount) nCurr = nCount-1;
						ASSERT(HighPriceItem<nCount);
						if(HighPriceItem>=nCount) HighPriceItem = nCount-1;

						detail.buytime=pKline[nCurr].day;

						detail.selltime=pKline[HighPriceItem].day;
						detail.buyprice=MidPrice;
						detail.sellprice=HighPrice;
						detail.timerange=HighPriceItem-nCurr;
						detail.profit=profit*100;
						if(buySellAdditional.bSubstractFee)
                            detail.profit-=fee;
						detail.issuccess=bSucc;
						detail.nKindSell=bKindSell;
					
						if(tmB== 0 || detail.buytime>=tmB && detail.buytime<=tmE)
							chooseresult->m_detail.Add(detail);

					
						if(i>=nCount) i = nCount-1;
				
					}
					
				}
                chooseresult->choosecount=chooseresult->m_detail.GetSize();
				chooseresult->symbol=StockId;
				chooseresult->m_stkKind = stkKind;
				chooseresult->successcount=bSuccCount;
				if(chooseresult->m_detail.GetSize()==0)
					chooseresult->successpercent=0;
				else
				    chooseresult->successpercent=float(bSuccCount)/(chooseresult->m_detail.GetSize());
				chooseresult->nKindBuySell=0;
			}
			break;
		case 1:
			{
				for(int i=nB;i<nCount;i++)
				{
					if(pnSellResult[i]==1)
					{
						int bSucc=10;
						MidPrice=CaclMid(pKline[i]);
						if((i-nRange)<0)
						{
							nTimelen=i;
							bSucc=2;
						}
						if(i!=0)
						{
						    HighPriceItem=i-1;
							HighPrice=CaclMid(pKline[i-1]);
							for(int bb=(i-nTimelen);bb<i;bb++)
							if(CaclMid(pKline[bb])<HighPrice)
							{
								HighPriceItem=bb;
								HighPrice=CaclMid(pKline[bb]);
							}
						
						}
						else 
						{
							HighPriceItem=i;
							HighPrice=CaclMid(pKline[i]);
						}
						
						profit=(MidPrice-HighPrice)/HighPrice;
						if(bSucc!=2)
						{
							if(HighPriceItem==i)
								bSucc=1;
							else if((profit/(i-HighPriceItem))>=(fProfit/nRange))
							{
								bSucc=0;
								bSuccCount++;
							}
							else 
								bSucc=1;
						}
						detail.buytime=pKline[HighPriceItem].day;
						detail.selltime=pKline[i].day;
						detail.buyprice=HighPrice;
						detail.sellprice=MidPrice;
						detail.timerange=i-HighPriceItem;
						detail.profit=profit*100;
						if(buySellAdditional.bSubstractFee)
                            detail.profit-=fee;
						detail.issuccess=bSucc;
						detail.nKindSell=bKindSell;
						
						if(tmB== 0 || detail.buytime>=tmB && detail.buytime<=tmE)
							chooseresult->m_detail.Add(detail);
						
						if(i>=nCount) i = nCount-1;
				
					}
					
				}
                chooseresult->choosecount=chooseresult->m_detail.GetSize();
				chooseresult->symbol=StockId;
				chooseresult->successcount=bSuccCount;
				if(chooseresult->m_detail.GetSize()==0)
                   chooseresult->successpercent=0;
				else
				   chooseresult->successpercent=float(bSuccCount)/(chooseresult->m_detail.GetSize());
				chooseresult->nKindBuySell=1;
			}
			break;
		case 2:
			{
				for(int i=nB;i<nCount;i++)
				{
					if(pnBuyResult[i]==1)
					{
						int iTemp = i;
						int bSucc=10;
						int nMove;
						int nEnterPoint = this->buySellAdditional .nEnterPoint [0];
						if(i==nCount) nEnterPoint = 0;
						MidPrice=CaclPrice(pKline+i, nEnterPoint, nMove);
						if(i ==nCount-1 && nMove>0)
							MidPrice=CaclMid(pKline[i]);
						nMove = 0;

						HighPriceItem=0;
						for(int bb=i+1+nMoveAdd;bb<nCount;bb++)
							if(pnSellResult[bb]==1)
							{
                                HighPriceItem=bb;
								iTemp = bb--;
								break;
							}

					    if(HighPriceItem==0)
						{
						   HighPriceItem = nCount-1;
						   iTemp = nCount;
						   bSucc=2;
						}
					
                        for(int add=i+1+nMoveAdd;add<HighPriceItem;add++)
						{
							float addprofit=100*(CaclMid(pKline[add])-MidPrice)/MidPrice;
							if(buySellAdditional.bLose)
								if(-buySellAdditional.fMaxLose>addprofit)
								{
									HighPriceItem=add;
									bKindSell = 1;
                                    break;
								}
                            if(buySellAdditional.bWin)
								if(buySellAdditional.fMaxWin<addprofit)
								{
									HighPriceItem=add;
									bKindSell = 1;
                                    break;
								}
							if(buySellAdditional.bMaxDays)
								if(buySellAdditional.fMaxDays<(HighPriceItem -i - nMoveAdd))
								{
									HighPriceItem=i+nMoveAdd+int(buySellAdditional.fMaxDays);
									bKindSell = 1;
                                    break;
								}
						}
				
						HighPrice=CaclMid(pKline[HighPriceItem]);
						profit=(HighPrice-MidPrice)/MidPrice;
						if(bSucc == 10)
						{
							bSucc=1;
							if(HighPriceItem!=i && (profit/(HighPriceItem-i))>=(fProfit/nRange))
							{
								bSucc=0;
								bSuccCount++;
							}
						}
						
						int nCurr = i+nMoveAdd;
						if(nCurr>=nCount) nCurr = nCount-1;

						detail.buytime=pKline[nCurr].day;

						int nSellFoot = HighPriceItem;
						if(nSellFoot<=nCurr) nSellFoot = nCurr+1;
						if(nSellFoot>=nCount) nSellFoot = nCount-1;

						detail.selltime=pKline[nSellFoot].day;
						detail.buyprice=MidPrice;
						detail.sellprice=HighPrice;
						detail.timerange=nSellFoot-nCurr;
						detail.profit=profit*100;
						if(buySellAdditional.bSubstractFee)
                            detail.profit-=fee;
						detail.issuccess=bSucc;
						detail.nKindSell=bKindSell;
						
						if(tmB== 0 || detail.buytime>=tmB && detail.buytime<=tmE)
							chooseresult->m_detail.Add(detail);

						 i = iTemp;
					}
					
				}
                chooseresult->choosecount=chooseresult->m_detail.GetSize();
				chooseresult->symbol=StockId;
				chooseresult->successcount=bSuccCount;
				if(chooseresult->m_detail.GetSize()==0)
					chooseresult->successpercent=0;
				else
				    chooseresult->successpercent=float(bSuccCount)/(chooseresult->m_detail.GetSize());
				chooseresult->nKindBuySell=2;
			}
			break;
		default :
			
			return false;
			}
	chooseresult->m_stkKind = stkKind;
    m_testResultArray.Add(chooseresult);
	return true;
}

void CTaiScreenParent::OnDblclkParamList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CTaiScreenParent::OnSetfocusParamList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;	
}
void CTaiScreenParent::GetIndexLineName(CFormularContent *pIndex)
{
	if(!pIndex) return ;
	Kline nKline[20];
	CFormularCompute nEquation(0,19,nKline,pIndex,CSharesCompute::GetIndexSymbol(0) ,SHZS,NULL,5 );
	int numPa=pIndex->numPara ;
	for(int j=0;j<numPa;j++)
	{
		nEquation.AddPara (pIndex->namePara [j],pIndex->defaultVal [j]);
	}
	nEquation.Devide();
    nLineName.RemoveAll(); 
	for(int i=0;i<nEquation.GetRlineNum ();i++)
	{
        CString LName=nEquation.GetRlineName(i);
		LName.MakeUpper();
		this->nLineName.Add(LName);
		m_nLineType[i] = nEquation.m_RlineType[i];
	}
	ASSERT(nLineName.GetSize ()>0);
}

void CTaiScreenParent::OnClickTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{

	*pResult = 0;
}

void CTaiScreenParent::OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();
	m_nLineType[0] = 0;
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CPoint pt;
	::GetCursorPos (&pt);
	CPoint ptOld(pt);

	HTREEITEM hItem = this->m_treeFormularCtrl.GetSelectedItem ();
	if(hItem == NULL)
	{
	  return;
	}
	if ((hItem != NULL) )
	{
	   this->m_treeFormularCtrl .SelectItem(hItem);
	}
    m_hItem = hItem;
	int nKindRoot,nKindItem;
	m_pIndex = NULL;
	bool bSucc = m_treeFormularCtrl.GetCurrentItemInfo (nKindRoot,nKindItem,m_pIndex);	

	int i = 0,j=0;
	CString temp;
	this->nKind = nKindRoot;
	if(nKind == -7 || nKind == -8)
		m_pIndex = GetSpecialLJishu(-nKind-7);
	if(nKind != -6)
	m_conRich.SetLJishu(m_pIndex);
	if(m_pIndex == NULL || nKind == -6)
		goto DO_END;

	ReAddPeriodName();

	this->GetIndexLineName(m_pIndex);
	for( j = 0;j<3;j++)
	{
		int nID[] = {IDC_LIST_FORMULAR_LINE_ALL,IDC_EDIT_PARAM1,IDC_EDIT_PARAM2};
		if((CComboBox *)(GetDlgItem(nID[j]) == 0)) continue;
		while(((CComboBox *)(GetDlgItem(nID[j])))->GetCount())
		{
		  ((CComboBox *)(GetDlgItem(nID[j])))->DeleteString(0); 
		}
		for(i=0;i<this->nLineName.GetSize();i++)
		{
			((CComboBox *)(GetDlgItem(nID[j])))->InsertString(i,nLineName.GetAt(i));
		}
		if(j==0)
		((CComboBox *)(GetDlgItem(nID[j])))->SetCurSel(0);
	}

DO_END:
	
	if((nKind != -1 || m_pIndex == NULL) && nKind != -6)
	{
		GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_SUB_FORMULAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(SW_HIDE);//
		if(GetDlgItem(IDC_STATIC_COND_NEED)) GetDlgItem(IDC_STATIC_COND_NEED)->ShowWindow(SW_HIDE);//
		if(GetDlgItem(IDC_STATIC_P1)) GetDlgItem(IDC_STATIC_P1)->ShowWindow(SW_HIDE);//
		if(GetDlgItem(IDC_STATIC_P2)) GetDlgItem(IDC_STATIC_P2)->ShowWindow(SW_HIDE);//
		
	}
	else 
	{
		GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)->ShowWindow(SW_SHOW);
		if(m_bXgnExt == false)
		{
			GetDlgItem(IDC_LIST_SUB_FORMULAR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(SW_SHOW);
			if(GetDlgItem(IDC_STATIC_COND_NEED)) GetDlgItem(IDC_STATIC_COND_NEED)->ShowWindow(SW_SHOW);//
		}
	}

	if(nKind == -6)
	{
		((CComboBox *)(GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)))->ResetContent ();
		nLineName.RemoveAll ();

		int nn = -1;

		{
			
			for(i=0;i<37;i++)
			{
				if(nFootFinance[i]>=0)
				{
					CString ss ;
					ss.LoadString (IDS_ZGB+nFootFinance[i]);
					nLineName.Add (ss);
					((CComboBox *)(GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)))->AddString(ss);
				}
			}
		}
		((CComboBox *)(GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)))->SetCurSel(0);
	}

	if( m_pIndex == NULL)//
	{
		GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_HIDE);
		if(m_pExplainButton != NULL)
			m_pExplainButton->ShowWindow(SW_HIDE);
	}
	else if(m_pIndex->numPara == 0 )
	{
		GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_SHOW);
		if(m_pExplainButton != NULL)
			m_pExplainButton->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_SHOW);
		if(m_pExplainButton != NULL)
			m_pExplainButton->ShowWindow(SW_SHOW);
	}
	if(m_bDlgIndicator == true)
		GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_HIDE);
	
	//end
	OnSelchangeListSubFormular() ;
	UpdateData(FALSE);
	*pResult = 0;
}

CString CTaiScreenParent::GetGuoteIndex(IndexDataInfo *indexSv,CFormularContent *pIndex)
{
	CString s = "";
	CString sParam = "";
	CString sSub = "";

	switch(indexSv->iKind )
	{
	case CTaiKlineTreeCtl::nKindBaseInfo:
		sSub.Format("finance(%d)",GetFinanceFoot(indexSv->iLine)+1) ;//

	case -1:
		if(indexSv->iKind == -1)
		{
			ASSERT(pIndex!=NULL);

			sParam="(";
			sSub=GetIndexLineName(pIndex, indexSv->iLine);
			sSub = "\""+pIndex->name +"."+sSub+"\"";

		
			if(pIndex->numPara<=0)
			{
				sParam="";
			}
			else
			{
				for(int i=0;i<pIndex->numPara ;i++)
				{
					CString sp = CLongString::FloatToString((float)indexSv->fParam [i]);
					sp+=",";
					sParam+=sp;
				}
				sParam.TrimRight (",");
				sParam+=")";
			}
			sSub = sSub+sParam;
		}
		{

			CString s2 = CLongString::FloatToString(indexSv->fSubParam1);
			CString s3 = CLongString::FloatToString(indexSv->fSubParam2);

			if(indexSv->iLineParam[0] != CB_ERR ) 
			{
				s2 =GetIndexLineName(pIndex, indexSv->iLineParam[0]);
				s2 = "\""+pIndex->name +"."+s2+"\"";
			}
			if(indexSv->iLineParam[1] != CB_ERR ) 
			{
				s3 =GetIndexLineName(pIndex, indexSv->iLineParam[1]);
				s3 = "\""+pIndex->name +"."+s3+"\"";
			}

			switch(indexSv->iSunIndex )
			{
			case 0 :
				s = sSub+">"+s2;
				break;
			case 1 :
				s = sSub+"<"+s2;
				break;
			case 2 :
				s = sSub+"="+s2;
				break;
			case 3 :
				s = "cross("+sSub+","+s2+")";
				break;
			case 4 :
				s = "cross("+s2+","+sSub+")";
				break;
			case 5 :
				s = sSub+">"+s2 +" and "+sSub+"<"+s3;
				break;
			case 6 :
				s = "slowupcross("+s2+","+sSub+","+s3+")";
				break;
			case 7 :
				s = "slowdowncross("+s2+","+sSub+","+s3+")";
				break;
			}

		}
		break;
	}

	return s;
}

int CTaiScreenParent::GetCountValid(IndexDataInfo *indexSv, CTaiShanDoc *pDoc, int nAdd)
{
	int iKind=indexSv->iKind ;
	if(iKind == -6) return 1;
	CString sIndex_name = indexSv->sIndex_name ;
	Kline kline[2000];
	memset(kline,16,sizeof(Kline)*2000);
	int nKline = 2000;


	Kline* pKline = 0;
	CString sArr[8] ={"600640","600639","600811","600638","600642","600641",CSharesCompute::GetIndexSymbol(0),"1A0002"};
	int stkKind[8]={SHAG,SHAG,SHAG,SHAG,SHAG,SHAG,SHZS,SHZS};
	int jj;
	for(jj = 0;jj<8;jj++)
	{
		nKline = CTaiKlineFileKLine::ReadKLineAny(sArr[jj],stkKind[jj], pKline,  2000,  5,NULL, 0);//,tmEnd,tmBegin);
		if(nKline>=500)
			break;

	}
	if(nKline>2000) nKline = 2000;
	memcpy(kline,pKline,sizeof(Kline)*nKline);
	if(pKline!=0) delete [] pKline;

	if(jj==10 )
		nKline = 2000;

	CFormularContent*	pIndex;
	CFormularContent	jiShu;
	CString s ;
	int j=0;
	if(iKind < 0)
	{
		pIndex=LookUpArray( iKind, sIndex_name, pDoc);
		if(pIndex==NULL)
		{
			return -2;
		}

		s=pIndex->fomular ;
		s.MakeLower();
	}
	else
	{
		pIndex = &jiShu;
		pIndex->numPara =0;
		pIndex->fomular = sIndex_name;
		s=pIndex->fomular ;
		s.MakeLower();
	}
	
	CString str = " ";

	CFormularCompute* m_formuCompute =new CFormularCompute(0,nKline-1,kline,pIndex,CSharesCompute::GetIndexSymbol(0),SHZS,NULL,5);

	//--
	int numPa=0;
	if(iKind < 0)
	{
		numPa=pIndex->numPara ;
		for(j=0;j<numPa;j++)
		{
			m_formuCompute->AddPara (pIndex->namePara [j],indexSv->fParam [j]);
		}
	}

	int pos=-1;

	ARRAY_BE line;
	line.line=new float[nKline];
	memset(line.line,0,4*nKline);

	int maxNeeded ;

	if(m_formuCompute->FormularCompute()!=0)
	{
		delete m_formuCompute;
		maxNeeded = -2;
	}
	else
	{
		int rtnLine=0;
		int numLine =m_formuCompute->GetRlineNum ();
		CString namePer;

		if(m_formuCompute->m_bTotalData)
			return -1;

		for(int i=0;i<numLine ;i++)
		{
			rtnLine = m_formuCompute->GetLine (i,line,namePer);
			if(rtnLine==1)
			{
				maxNeeded = -2;
				break;
			}
			if(i==0)
				maxNeeded = line .b;
			else if(line .b>maxNeeded)
				maxNeeded = line .b;
		}
		delete m_formuCompute;
	}
	delete []line.line;

	if(maxNeeded>=0)
		maxNeeded+=nAdd;

	return maxNeeded;

}

CString CTaiScreenParent::GetIndexLineName(CFormularContent *pIndex, int nLine)
{
	CString LName = "";
	if(!pIndex) return LName;
	Kline nKline[20];
	CFormularCompute nEquation(0,19,nKline,pIndex,CSharesCompute::GetIndexSymbol(0),SHZS ,NULL,5 );
	int numPa=pIndex->numPara ;
	for(int j=0;j<numPa;j++)
	{
		nEquation.AddPara (pIndex->namePara [j],pIndex->defaultVal [j]);
	}
	nEquation.Devide();

	int nTot = nEquation.GetRlineNum ();
	ASSERT(nTot>nLine);
    LName=nEquation.GetRlineName(nLine);
	LName.MakeUpper();
	return LName;

}


void CTaiScreenParent::OnAdd() 
{

	if(!m_choose_list.IsWindowVisible ())
		return;
	if(this->m_hItem == NULL || (m_pIndex == NULL && nKind!=-6))
	{
		AfxMessageBox("请选择一个条件组合");
		return;
	}
	UpdateData();


	
	IndexDataInfo index_save;
	memset(&index_save,0,sizeof(index_save));
	if(m_pIndex)
		strcpy(index_save.sIndex_name,m_pIndex->name);
	else
		strcpy(index_save.sIndex_name,"");
    index_save.iKind = nKind;

	UpdateParams(&index_save);	
	if(m_pIndex!=NULL && (m_gpIndex[0] == m_pIndex || m_gpIndex[1] == m_pIndex))
	{
		if(index_save.fParam [0] >= index_save.fParam [1] )
		{
		AfxMessageBox("指标参数设置不合理！");
		return;
		}
	}

	int nDays=GetCountValid(&index_save,pDoc,10);
    index_save.iBeginFoot  = nDays ;
	

	if(m_bCaclYHParam == TRUE)
	{
		//lmb11
#ifndef OEM
		if(m_stockchoose.GetSize ()>0)
			CTaiKlineDlgYHParam::CalcYH(&index_save, &m_stockchoose );
		else
			AfxMessageBox("必须首先加入股票，才能根据这些股票的数据进行优化参数计算！");
#endif
	}
	
	IndexDataInfo GetIndex;
	if(m_indexSaveArray.GetSize() > 0)
	{
		for(int i=0;i<m_indexSaveArray.GetSize();i++)
		{
		  GetIndex = m_indexSaveArray.GetAt(i);   
		  if(memcmp(&GetIndex,&index_save,sizeof(IndexDataInfo)) == 0)
		  {
			  AfxMessageBox("指标已选入,请选择其他指标或改变参数!");
			  return;
		  }
		}
	}
	//////////////////////////////////////
	m_indexSaveArray.Add(index_save);
	AddIndexArrayToList(&index_save);

	SetHorizonExtent();

	if(m_choose_list.GetCount ()>0)
		m_choose_list.SetCurSel (m_choose_list.GetCount ()-1);
}

void CTaiScreenParent::OnSave() 
{

	if(this->m_choose_list .GetCount ()<=0)
	{
		AfxMessageBox("没有可用的条件组合！");
		return;
	}
    CString     defaultname;
	defaultname="";
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,"*.TJZ|*.TJZ||");
	m_filedia.m_ofn.lpstrInitialDir=strdir;
	if(m_filedia.DoModal()==IDCANCEL)
		return;
	CString   filename;
	filename=m_filedia.GetPathName();
	int nPnt ;
	if((nPnt = filename.Find('.')) == -1)
	{
	  CString temp;
	  temp.Format("%s.TJZ",filename);
	  filename = temp;
	}
	else
	{
		if(filename.GetLength ()>4)
		{
		CString s = filename.Right (4);
		s.MakeUpper ();
		if(s!=".TJZ") 
		{
			AfxMessageBox("文件扩展名不对，请重新输入！");
			return;
		}
		}
		else return;
	}
	UpdateData(TRUE);
	WriteTizhToFile(filename,m_and_or);	
} 
void CTaiScreenParent::OnDelete() 
{
	
   int i = m_choose_list.GetCurSel();	
   if(i == LB_ERR)
   {
      AfxMessageBox("请选择要删除的指标!");
	  return;
   }
   this->m_indexSaveArray.RemoveAt(i,1);
   m_choose_list.DeleteString(i);
	SetHorizonExtent()	;

	if(m_choose_list.GetCount ()>i)
		m_choose_list.SetCurSel (i);
	else if(m_choose_list.GetCount ()>0)
		m_choose_list.SetCurSel (0);
}

void CTaiScreenParent::OnLoad() 
{
	
	UpdateData();
  	CString     defaultname;
	defaultname="";
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));
	
	CFileDialog  m_filedia(TRUE,0,"*.TJZ",OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,"*.TJZ|*.TJZ||");
	m_filedia.m_ofn.lpstrInitialDir=strdir;
	if(m_filedia.DoModal()==IDCANCEL)
	{
	
		return;
	}
	UpdateData(FALSE);
	CString filename=m_filedia.GetPathName();
    LoadTjxgZsFile(filename);
}  

BOOL CTaiScreenParent::UpdateParams(IndexDataInfo *pIndex_save)
{
	UpdateData(TRUE);

	float fVal[8];
	m_conRich.GetParamVal((float *)fVal);
	for(int i=0;i<8;i++)
	{
        pIndex_save->fParam[i] =fVal[i];
	}
	char str[20];
	m_period.GetLBText(m_period.GetCurSel(),str);

	for(int i=0;i<11;i++)
		if(strcmp(str,m_gPeriodName[i]) == 0)
		  pIndex_save->iDataType = i;

	if(pIndex_save->iKind == -6)
		pIndex_save->iDataType = 6;
    
    pIndex_save->iLine =  m_sLineAll.GetCurSel();
	if(pIndex_save->iLine<0) pIndex_save->iLine = 0;

	pIndex_save->iSunIndex = m_sSubFormular.GetCurSel();
    
	int i = m_sSubFormular.GetCurSel();
	if(i < 0)
	  i = 0;
	CString sStr;
    m_sSubFormular.GetLBText(i,sStr);
	pIndex_save->iSunIndex =0;
	for(i=0;i<8;i++)
	{  
	  if(sStr == m_subIndexGroup[i])
	  {
		  pIndex_save->iSunIndex = i;
		  break;
	  }
	}
	pIndex_save->iLineParam[0] = CB_ERR ;
	pIndex_save->iLineParam[1] = CB_ERR ;
	if(sStr.Find("参数1",0) != -1)
	{
		CComboBox * pBox = (CComboBox*)GetDlgItem(IDC_EDIT_PARAM1);
		CString s = "";
		if(pBox)
		pBox->GetWindowText (s);
	   pIndex_save->fSubParam1 = float( atof(s) );
		if(pBox)
		pIndex_save->iLineParam[0] = pBox->GetCurSel();
	}
	if(sStr.Find("参数2",0) != -1)
	{
		CComboBox * pBox = (CComboBox*)GetDlgItem(IDC_EDIT_PARAM2);
		CString s = "";
		if(pBox)
		pBox->GetWindowText (s);
	   pIndex_save->fSubParam2 = float( atof(s) );
		if(pBox)
		pIndex_save->iLineParam[1] = pBox->GetCurSel();
	}

	if(m_nLineType[pIndex_save->iLine] == BUY_CASE)
	{
		pIndex_save->iSunIndex = 0;
		pIndex_save->fSubParam1 = 0;
		pIndex_save->fSubParam2 = 0;
		pIndex_save->bBuyCase = 0;
	}
	else if(m_nLineType[pIndex_save->iLine] == SELL_CASE)
	{
		pIndex_save->iSunIndex = 0;
		pIndex_save->fSubParam1 = 0;
		pIndex_save->fSubParam2 = 0;
		pIndex_save->bBuyCase = 1;
	}
    else pIndex_save->bBuyCase  = this->m_bBuy ;	
	if((int)(pIndex_save->bBuyCase) != m_bBuy && m_bShowBuyCase == true)
	{
		CString s3 = "买入条件！";
		if((int)(pIndex_save->bBuyCase) == 1)
			s3 = "卖出条件！";

		AfxMessageBox("系统将把此条件设为"+s3);
	}

	return TRUE;
}  

void CTaiScreenParent::OnSelchangeListSubFormular() 
{
  int index = m_sSubFormular.GetCurSel();
  if(index < 0)
	  return;
  if(!GetDlgItem(IDC_LIST_SUB_FORMULAR)->IsWindowVisible())
	  return;
  CString str;
  m_sSubFormular.GetLBText(index,str);
  if(str.Find("参数1",0) != -1)
  {
	  (GetDlgItem(IDC_EDIT_PARAM1))->ShowWindow(SW_SHOW);
	  (GetDlgItem(IDC_EDIT_PARAM1))->EnableWindow(TRUE);
		if(GetDlgItem(IDC_STATIC_P1)) GetDlgItem(IDC_STATIC_P1)->ShowWindow(SW_SHOW);//
  }
  else
  {
	 (GetDlgItem(IDC_EDIT_PARAM1))->ShowWindow(SW_HIDE);
     (GetDlgItem(IDC_EDIT_PARAM1))->EnableWindow(FALSE);
		if(GetDlgItem(IDC_STATIC_P1)) GetDlgItem(IDC_STATIC_P1)->ShowWindow(SW_HIDE);//
  }

  if(str.Find("参数2",0) != -1)
  {
      (GetDlgItem(IDC_EDIT_PARAM2))->ShowWindow(SW_SHOW);
	  (GetDlgItem(IDC_EDIT_PARAM2))->EnableWindow(TRUE);
		if(GetDlgItem(IDC_STATIC_P2)) GetDlgItem(IDC_STATIC_P2)->ShowWindow(SW_SHOW);//
  }
  else
  {
		if(GetDlgItem(IDC_STATIC_P2)) GetDlgItem(IDC_STATIC_P2)->ShowWindow(SW_HIDE);//
	 (GetDlgItem(IDC_EDIT_PARAM2))->ShowWindow(SW_HIDE);
     (GetDlgItem(IDC_EDIT_PARAM2))->EnableWindow(FALSE);
  }
  
} 


int CTaiScreenParent::FromKlineKindToFoot(int nKlineKind)
{
	int n = -1;
	for(int i=0;i<11;i++)
	{
		if(nLKind[i] == nKlineKind)
		{
			n=i;
			break;
		}

	}
	return n;
}


void CTaiScreenParent::ReAddPeriodName()
{
	char strPre2[20];
	int nPre = 6;
	if(m_period.GetCurSel()>0) nPre = m_period.GetCurSel();
	if(m_period.GetCount()<=nPre) nPre = m_period.GetCount()-1;
	if(nPre<0) nPre=0;

	bool bFirst = false;
	if(m_period.GetCurSel()<0) bFirst = true;

	m_period.GetLBText(nPre,strPre2);
	CString strPre = strPre2;
	if(bFirst == true) strPre = m_gPeriodName[6];

	this->m_period.ResetContent();
	int nSel = 0;
	if(m_pIndex==NULL)
		return;

	int k=0;
	for(int i=0;i<11;i++)
	{
	   if(m_pIndex->nPeriodsUsed[i] == 1)
	   {
		   this->m_period.AddString(m_gPeriodName[i]);
		   k++;
		   if(strPre==m_gPeriodName[i])
			   nSel = k-1;
	   }
	}
	if(nSel<0) nSel = 0;
	if(m_period.GetCount()<=nSel)
		nSel = 0;
	m_period.SetCurSel(nSel);

}


void CTaiScreenParent::SetHorizonExtent()
{
	CSize   sz;
	int nMaxListLen = 0;
	CDC* pDC = m_choose_list.GetDC();
	for(int i=0;i<m_choose_list.GetCount( ) ;i++)
	{
		CString s;
		m_choose_list.GetText( i, s) ;
		sz = pDC->GetTextExtent(s);
		if(sz.cx > nMaxListLen)
			nMaxListLen = sz.cx;
	}
	m_choose_list.SetHorizontalExtent(nMaxListLen);
	m_choose_list.ReleaseDC (pDC);

}

void CTaiScreenParent::ShowExplain(CFormularContent *pJishu)
{
	if(pJishu!=NULL)
	{
		CDialogLOGGSSM mdiagssm;
		mdiagssm.m_edit=pJishu->help ;
		mdiagssm.DoModal();
	}
}

void CTaiScreenParent::FillToListCtrlHead(CListCtrl *pList)
{
   	CRect           rect;
	CString         strItem1= _T("证券名称");
	CString         strItem2= _T("类型");
	
	pList->InsertColumn(0, strItem1, LVCFMT_LEFT,
		80, 0);
	pList->InsertColumn(1, strItem2, LVCFMT_LEFT,
		80, 1);

}

void CTaiScreenParent::AddIndexArrayToList(IndexDataInfo* pIndex_Save)
{
	  IndexDataInfo index_save;
	  int n = m_indexSaveArray.GetSize();
	  CFormularContent *pJishu;
	  if(pIndex_Save!=NULL)
		  n=1;
	  for(int i=0;i<n;i++)
	  {
		 if(pIndex_Save!=NULL)
			index_save = *pIndex_Save;
		 else
			index_save = m_indexSaveArray[i];

		 pJishu = LookUpArray(index_save.iKind,index_save.sIndex_name,pDoc);

		this->GetIndexLineName(pJishu);

		 CString sTemp = "" ;
		 int j;
		 if(pJishu == NULL && index_save.iKind == -6)
		 {
			 sTemp = "";
			 CString ss;
 			 ss.LoadString (IDS_ZGB+index_save.iLine);

			 sTemp+= ss;
			 sTemp+= m_subIndexGroup[index_save.iSunIndex];
			 CString s;
			s=CLongString::FloatToString(index_save.fSubParam1);
			 if(index_save.iLineParam [0]!=CB_ERR)
			 {
				 if(index_save.iLineParam [0]<nLineName.GetSize ())
				 {
					 s=nLineName.GetAt(index_save.iLineParam [0]);
				 }
				 else
					 ASSERT(FALSE);
				 ;
			 }
			 sTemp.Replace("参数1",s);
			 s=CLongString::FloatToString(index_save.fSubParam2);
			 if(index_save.iLineParam [1]!=CB_ERR)
			 {
				 if(index_save.iLineParam [1]<nLineName.GetSize ())
				 {
					 s=nLineName.GetAt(index_save.iLineParam [1]);
				 }
				 else
					 ASSERT(FALSE);
				 ;
			 }
			 sTemp.Replace("参数2",s);
		 }
		 else if(pJishu == NULL)
		 {
			 if(pIndex_Save==NULL)
			 {
				 CString ss;
				 ss.Format ("第%d个",i+1);
				 AfxMessageBox(ss+"指标不存在，无法加入！");
				m_indexSaveArray.RemoveAt(i);
				i--;
				n--;
			 }
		    continue;
		 }
		 else
		 {
		 sTemp = pJishu->name;
		 for( j=0;j<pJishu->numPara ;j++)
		 {
			 CString s = "";
			 s=CLongString::FloatToString(index_save.fParam[j]);
			 if(j==0)
				 s = "("+s;
			 s+=",";
			 if(j == pJishu->numPara-1)
			 {
				 s.TrimRight (",");
				 s = s+")";
			 }
			 sTemp+=s;
		 }
		 CString sPeriod;

		 sTemp+= " ";
		 if(m_bDlgIndicator == false )
			sTemp+= m_gPeriodName[index_save.iDataType];
		 sTemp+= " ";

		 if(index_save.iKind == -1)
		 {
			 GetIndexLineName(pJishu);
			 sTemp+= nLineName.GetAt(index_save.iLine);
			 sTemp+= m_subIndexGroup[index_save.iSunIndex];
			 CString s;
			 s=CLongString::FloatToString(index_save.fSubParam1);
			 if(index_save.iLineParam [0]!=CB_ERR)
			 {
				 if(index_save.iLineParam [0]<nLineName.GetSize ())
				 {
					 s=nLineName.GetAt(index_save.iLineParam [0]);
				 }
				 else
					 ASSERT(FALSE);
				 ;
			 }
			 sTemp.Replace("参数1",s);
			 s=CLongString::FloatToString(index_save.fSubParam2);
			 if(index_save.iLineParam [1]!=CB_ERR)
			 {
				 if(index_save.iLineParam [1]<nLineName.GetSize ())
				 {
					 s=nLineName.GetAt(index_save.iLineParam [1]);
				 }
				 else
					 ASSERT(FALSE);
				 ;
			 }
			 sTemp.Replace("参数2",s);
		 }
		 }

		 CString sKind[]={"指标:","选股:","模式:","","",
							"基本面选股:","价格:","涨跌幅:","","",
							 "","","","",""};
		 CString sBuy[]={"买入","卖出"};
		 sTemp = sKind[-index_save.iKind-1]+sTemp;
		 if(m_bShowBuyCase == true)
		 {
			 sTemp =sBuy[ index_save.bBuyCase]+":"+sTemp;
		 }

		 m_choose_list.AddString(sTemp);
	  }

}

void CTaiScreenParent::AddStockToList(CListCtrl *pList)
{
	int n = m_symbolTypeArray.GetSize();
	pList->DeleteAllItems ();
	for(int i=0;i<n;i++)
		SelectStock::FillToListCtrlItem(pList,m_symbolTypeArray[i].m_sSymbol, m_symbolTypeArray[i].m_nType);


}

CFormularContent* CTaiScreenParent::GetSpecialLJishu(int nJishu)
{
	if(m_gpIndex[nJishu]!=NULL)
		return m_gpIndex[nJishu];
	m_gpIndex[nJishu] = new CFormularContent;
	CFormularContent* p = m_gpIndex[nJishu];
	p->numPara = 2;
	p->namePara [0] = "价低于";
	p->namePara [1] = "或价高于";
	p->max [0] = 9000;
	p->max [1] = 9000;
	p->min [0] = 0.1;
	p->min [1] = 0.1;
	p->defaultVal [0] = 10;
	p->defaultVal [1] = 20;
	p->explainParam = "股票最新价低于 param#1 \r\n或高于 param#2 ";
	memset(p->nPeriodsUsed,0,16);
	p->nPeriodsUsed[6]=(BYTE)1;
	switch(nJishu)
	{
	case 0:
		p->name = "价格预警";
		p->fomular = "(dynainfo(7))";
		break;
	case 1:
		p->name = "涨跌幅预警";
		p->max [0] =  10000;
		p->max [1] =  10000;
		p->min [0] = -10000;
		p->min [1] = -10000;
		p->namePara [0] = "涨跌幅低于百分之";
		p->namePara [1] = "或涨跌幅高于百分之";
		p->defaultVal [0] = -5.0f;
		p->defaultVal [1] = 5.0f;
		p->fomular = "(dynainfo(7)-dynainfo(3))*100/dynainfo(3)";
		p->explainParam = "股票涨跌幅低于百分之 param#1\r\n 或高于百分之 param#2 ";
		break;
	case 2:
		break;
	}

	return p;
}

float CTaiScreenParent::CaclMid(Kline kline)
{
    int j=pDoc->m_systemdata.middleprice;
	if(j<0||j>4)
		j=0;
	float rtn;
	switch(j)
	{
	case(0):
		rtn=(kline.high+kline.low+2*kline.close)/4;
		break;
	case(1):
		rtn=(kline.high+kline.low+kline.close)/3;
		break;
	case(2):
		rtn=(kline.high+kline.low)/2;
		break;
	case(3):
		rtn=(kline.high+kline.low+kline.open+kline.close)/4;
		break;
	case(4):
		rtn=kline.close;
		break;
	}
	return rtn;
}
CTaiScreenParent::~CTaiScreenParent()  
{
	ClearTestResultArray();
}

void CTaiScreenParent::ClearTestResultArray()
{
	int n = m_testResultArray.GetSize();
	for(int i=0;i<n;i++)
	{
	
			 SCREEN_RESULT *chooseresult;
			 chooseresult=m_testResultArray.GetAt(i);
			 delete chooseresult;
	}
	m_testResultArray.RemoveAll();

}

float CTaiScreenParent::GetParamValue(int i)
{
	if(m_pIndex==NULL) return 0;
	int nSel =  m_period.GetCurSel();
	if(nSel <0) nSel =0;
	return CFormularContent::GetParamDataEach(i,nLKind[nSel], m_pIndex);
}

void CTaiScreenParent::PostNcDestroy() 
{
	
	
	CDialog::PostNcDestroy();
}
BOOL CTaiScreenParent::PreTranslateMessage(MSG* pMsg) 
{
	

	if(m_pStockSelectList!=0)
	{
		if(m_pStockSelectList->m_hWnd == pMsg->hwnd)
		{
			if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
			{
				CTaiKeyBoardAngelDlg m_diakeybrode(this);
				if(pMsg->wParam>='a'&&pMsg->wParam<='z')
					pMsg->wParam-=32;
				char input=pMsg->wParam;
				m_diakeybrode.input=input;
				if(pMsg->wParam>='0'&&pMsg->wParam<='9')
					m_diakeybrode.ischar=false;
				else
					m_diakeybrode.ischar=TRUE;

				if(m_diakeybrode.DoModal() != IDOK)
					return TRUE;
			
				if(m_diakeybrode.isresultok ==TRUE && m_diakeybrode.isresultstock ==TRUE)
				{
					CString s3 = m_diakeybrode.result;
					SelectStock::FillToListCtrlItem(m_pStockSelectList,s3.GetBuffer(0),-1-m_diakeybrode.m_stkKind );
					SelectStock::GetStockSymbolArray(m_pStockSelectList,m_stockchoose);
					ListCtrlSetSelectItem(m_pStockSelectList);
					NotifySubclass();
				}
				return TRUE;
			}
		}
		
	}
	if(m_treeFormularCtrl.m_hWnd == pMsg->hwnd)
		if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		{

				OnAdd();
			return TRUE;
		}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTaiScreenParent::AddAStockKeyBoard(CListCtrl *pList)
{

}

void CTaiScreenParent::OnDestroy() 
{


	CDialog::OnDestroy();
	
	
	
}

void CTaiScreenParent::OnReturnTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}

void CTaiScreenParent::OnDblclkTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(this->m_pIndex!=NULL)
		OnAdd() ;
	*pResult = 0;
}

CString CTaiScreenParent::GetPeriodName(int nKlineType)
{
	for(int i = 0;i<11;i++)
	{
		if(nLKind[i]==nKlineType)
		{
			return m_gPeriodName[i];
		}
	}
	CString s="";
	return s;

}

void CTaiScreenParent::OnSelchangeListFormularLineAll() 
{
	
	if(!GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)->IsWindowVisible())
		  return;
	int index = m_sLineAll.GetCurSel();
	if(index < 0)
		  return;
	int index2 = m_sSubFormular.GetCurSel();
	if(index2 < 0)
		  index2 = 0;

	if(m_nLineType[index] == BUY_CASE || m_nLineType[index] == SELL_CASE)
	{
		if(m_sSubFormular.IsWindowVisible())
			m_sSubFormular.ShowWindow(SW_HIDE);
		(GetDlgItem(IDC_EDIT_PARAM1))->ShowWindow(SW_HIDE);
		(GetDlgItem(IDC_EDIT_PARAM2))->ShowWindow(SW_HIDE);

		if(GetDlgItem(IDC_STATIC_COND_NEED)) GetDlgItem(IDC_STATIC_COND_NEED)->ShowWindow(SW_HIDE);//
		if(GetDlgItem(IDC_STATIC_P1)) GetDlgItem(IDC_STATIC_P1)->ShowWindow(SW_HIDE);//
		if(GetDlgItem(IDC_STATIC_P2)) GetDlgItem(IDC_STATIC_P2)->ShowWindow(SW_HIDE);//
	}
	else
	{
		if(m_bXgnExt == false)
		{
			if(!m_sSubFormular.IsWindowVisible())
				m_sSubFormular.ShowWindow(SW_SHOW);
			OnSelchangeListSubFormular() ;
		}
	}
	
}

void CTaiScreenParent::NotifySubclass()
{

}

void CTaiScreenParent::ListCtrlSetSelectItem(CListCtrl *pList, int nItem)
{
	int temp=pList->GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp!=-1)
		return;
	int n = pList->GetItemCount ();
	if(n>0)
	{
		pList->SetItemState(0,LVIS_SELECTED , LVIS_SELECTED) ;
	}
			
}

void CTaiScreenParent::AddStocks()
{
	if(m_pStockSelectList!=0)
	{
		SelectStock pDlg(this,FALSE,true);
		pDlg.m_pSelectResultList  = m_pStockSelectList;
		pDlg.DoModal();
		ListCtrlSetSelectItem(m_pStockSelectList);
		NotifySubclass();
	}
}

void CTaiScreenParent::DeleteStocks()
{
	if(m_pStockSelectList!=0)
	{
		SelectStock::DeleteSelectedItem(m_pStockSelectList);
		SelectStock::GetStockSymbolArray(m_pStockSelectList,m_stockchoose);
	
		CAlertSystem::ListCtrlToSymbolType(m_symbolTypeArray,m_pStockSelectList);
		ListCtrlSetSelectItem(m_pStockSelectList);
		NotifySubclass();
	}

}

int CTaiScreenParent::IsPeriodValid(int nKlineType,CFormularContent* pJishu)
{
	int rtn = 0;
	for(int i=0;i<11;i++)
	{
	   if(pJishu->nPeriodsUsed[i] == 1 && nKlineType == nLKind[i])
	   {
		   rtn = 1;
		   break;
	   }
	}
	return rtn;
}

void CTaiScreenParent::WriteTizhToFile(CString FileName,int m_and_or, bool bZh,int m_mbzq,float m_mbly)
{
	CFile file;
	if(bZh)
	{
		if(file.Open(FileName,CFile::modeRead)!=0)
		{
			
			if(AfxMessageBox("文件已存在，是否替换？",MB_YESNO)!=IDYES)
				return;
		}
        file.Close();
	}
	if(file.Open(FileName,CFile::modeCreate|CFile::modeWrite)==0)
	{
		AfxMessageBox("找不到文件"+FileName);
		return;
	}
	IndexDataInfo indexToSave;
	file.SeekToBegin();

	int *pi = &m_and_or;
	file.Write(pi,1);
	for(int i=0;i<m_indexSaveArray.GetSize();i++)
	{
	  indexToSave = m_indexSaveArray.GetAt(i);
	  file.SeekToEnd();
	  file.Write(&indexToSave,sizeof(IndexDataInfo));
	}

    if(!bZh)
	{
		file.Write(&m_mbzq,sizeof(int));
		file.Write(&m_mbly,sizeof(float));
	}
	file.Close();
}

BOOL CTaiScreenParent::LoadTjxgZsFile(CString FileName, bool bShow)
{
	CFile file;
	if(file.Open(FileName,CFile::modeRead)==0)
	{
	    if(bShow)
		    AfxMessageBox("找不到文件"+FileName);
		return FALSE;
	}
	else
	{	
	  int i = 0;
	  int nRead = 0;
	  CString sTemp;
	 
	  if(bShow)
	  while(m_choose_list.GetCount())
	      m_choose_list.DeleteString(0);
	  this->m_indexSaveArray.RemoveAll();
	  file.Read(&i,1);
	  m_and_or = i;
	  
	  IndexDataInfo index_save;
	  nRead = file.Read(&index_save,sizeof(IndexDataInfo));
	  while(nRead == sizeof(IndexDataInfo))
	  {
		  
	     m_indexSaveArray.Add(index_save);
		  nRead = file.Read(&index_save,sizeof(IndexDataInfo));
	  }
	  if(bShow)
	  {
	      AddIndexArrayToList();
          SetHorizonExtent();
	  }
	}                    
	file.Close();
	if(bShow)
    if(m_choose_list.GetCount ()>0)
		m_choose_list.SetCurSel (m_choose_list.GetCount ()-1);
     return TRUE;
}

float CTaiScreenParent::CaclPrice(Kline *pKline, int nEnterPoint, int &nMove)
{

	nMove = 0;
	if(pKline == 0)
	{
		if(nEnterPoint>=5) nMove =1;
		return 0;
	}

	float f = pKline->close ;
	switch(nEnterPoint)
	{
	case 0:
		f = CaclMid(*pKline);
		break;
	case 1:
		f = pKline->open ;
		break;
	case 2:
		f = pKline->high ;
		break;
	case 3:
		f = pKline->low ;
		break;
	case 5:
		nMove = 1;
		f = CaclMid(*(pKline+1));
		break;
	case 6:
		nMove = 1;
		f = (pKline+1)->open ;
		break;
	case 7:
		nMove = 1;
		f = (pKline+1)->high ;
		break;
	case 8:
		nMove = 1;
		f = (pKline+1)->low ;
		break;
	case 9:
		nMove = 1;
		f = (pKline+1)->close ;
		break;
	}
	return f;
}

bool CTaiScreenParent::TestSuccessEach(CString StockId,int stkKind, int nCount, Kline *pKline, int nRange, float fProfit, CBuySellList *pList,CTime* tmEnd ,CTime* tmBegin )
{
	ARRAY_BE linebe;
	linebe.line = NULL;
	bool bSucc;
	int nRead = nCount;
	ASSERT(pList);

	if(nCount<=0) return false;

	int resultb=0;
	int * resultBuy;
	int * resultSell;
	int bBuySell;
	resultBuy =new int[nRead];//
	resultSell=new int[nRead];
	linebe.line = new float[nRead];
	for(int i=0;i<nRead;i++)
	{
		resultBuy[i]=0;
		resultSell[i]=0;
	}

	int NumBuy=0;
	int NumSell=0;
	int index_num=m_indexSaveArray.GetSize();
	IndexDataInfo index_save;
	int nPeriod = 6;
	if(index_num>0)
	{
		index_save = m_indexSaveArray.GetAt(index_num-1);
		nPeriod = index_save.iDataType;
	}
	for (int k=0;k<index_num;k++)
	{
		index_save=m_indexSaveArray.GetAt(k);
		index_save.iDataType = nPeriod;
		
		float a=FormularComputeShares(StockId,stkKind,nRead,&index_save,bSucc,pKline, pList,true,&linebe);

		if(a==-1)
			continue;
		if(linebe.b>resultb)
			resultb=linebe.b;
		if(index_save.bBuyCase==0)
		{
			NumBuy++;
			for(int j=resultb;j<nRead;j++)
			resultBuy[j]+=(int)linebe.line[j];
		}
		else 
		{
			NumSell++;
			for(int j=resultb;j<nRead;j++)
			resultSell[j]+=(int)linebe.line[j];
		}
        
	}

	for(int ii=resultb;ii<nRead;ii++)
	{ 
		if(resultBuy[ii]>0)
		{
			if(m_and_or)
				resultBuy[ii]=1;
			if(m_and_or==0)
				if(resultBuy[ii]==NumBuy)
					resultBuy[ii]=1;
				else
					resultBuy[ii]=0;
		}
		if(resultSell[ii]>0)
		{
			if(m_and_or)
				resultSell[ii]=1;
			if(m_and_or==0)
				if(resultSell[ii]==NumSell)
					resultSell[ii]=1;
				else
					resultSell[ii]=0;
		}
		
	}

	if(NumBuy==0&&NumSell>0)
         bBuySell=1;
    if(NumBuy>0&&NumSell==0)
		bBuySell=0;
	if(NumBuy>0&&NumSell>0)
		bBuySell=2;

	IsSuccess(StockId,stkKind,resultBuy,resultSell,resultb,nRead,pKline,nRange,fProfit,bBuySell, tmEnd , tmBegin );
	delete [] resultBuy;
	delete [] resultSell;
	if(linebe.line!=NULL)
		delete [] linebe.line;
	CTaiShanApp::DispatchMessageEachTime();
	return true;
}

float CTaiScreenParent::GetProfitAverage(float &fRateSucc,int& nBuy)
{
	int Total= m_testResultArray.GetSize();
	if(Total<=0)
	{
		fRateSucc = 0;
		return 0;
	}
	CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc;

	float fProfit = 0;
	float fRate = 0;

	for(int i=0;i<Total;i++)
	{
        SCREEN_RESULT * chooseresult= m_testResultArray.GetAt(i);
		int num;
		float num1=0.0;
		float num2=0.0;
		float DataArray[10];
	    memset(DataArray,0x00,sizeof(float)*10);
		CReportData * pdt = 0;
		if(!pDoc->m_sharesInformation.Lookup(chooseresult->symbol.GetBuffer(0),pdt,chooseresult->m_stkKind ))
			continue;
		SCREEN_DETAIL detail;
		num=chooseresult->m_detail.GetSize();

		for(int n=0;n<num;n++)
		{
		    detail=chooseresult->m_detail.GetAt(n);
			if(detail.profit>0)
			{
			     DataArray[6]++;
			     num1+=detail.profit;
			}
			else
			{
			     DataArray[7]++;
				 num2+=detail.profit;
			}
            DataArray[0]+=detail.profit;
			if(n==0)
			{
			     DataArray[4]=detail.profit;
				 DataArray[5]=detail.profit;
			}
			if(DataArray[4]<detail.profit)
		         DataArray[4]=detail.profit;
            if(DataArray[5]>detail.profit)
			     DataArray[5]=detail.profit;
			
		}
		if(num!=0)
		{
			fProfit+=(DataArray[0]/num);
			fRate+=(100*DataArray[6]/num);
		}
		nBuy = num;
	}

	fProfit/=Total;
	fRateSucc = fRate/Total;
	nBuy = nBuy/Total;
	return fProfit;
}
void CTaiScreenParent::OnClickListParam(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDblclkParamList( pNMHDR, pResult) ;
}
int CTaiScreenParent::GetPeriodFoot(int nKlineType)
{
	for(int i = 0;i<11;i++)
	{
		if(nLKind[i]==nKlineType)
		{
			return i;
		}
	}
	return 0;

}


int CTaiScreenParent::GetFinanceFoot(int nFootIn)
{
	int k = 0;
	int i;
	for(i = 0;i<37;i++)
	{
		if(k == nFootIn)
			break;
		if(nFootFinance[i]!= -1)
			k++;
	}
	if(i == 37)
		i = 0;

	return i;
}

void CTaiScreenParent::OnSelchangeTabSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int n = ((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->GetCurSel();
	if(n>=0)
		this->m_treeFormularCtrl.ReInsertIterm2(n);
	*pResult = 0;
}

bool CTaiScreenParent::GetCurSelData(IndexDataInfo &indexSave)
{
	if(m_choose_list.GetCount ()>0)
		return false;
	if(!m_choose_list.IsWindowVisible ())
		return false;
	if(this->m_hItem == NULL || (m_pIndex == NULL && nKind!=-6))
	{
		AfxMessageBox("请选择一个条件组合");
		return false;
	}
	UpdateData();

	memset(&indexSave,0,sizeof(indexSave));
	if(m_pIndex)
		strcpy(indexSave.sIndex_name,m_pIndex->name);
	else
		strcpy(indexSave.sIndex_name,"");
    indexSave.iKind = nKind;	

	UpdateParams(&indexSave);
	if(m_pIndex!=NULL && (m_gpIndex[0] == m_pIndex || m_gpIndex[1] == m_pIndex))
	{
		if(indexSave.fParam [0] >= indexSave.fParam [1] )
		{
		AfxMessageBox("指标参数设置不合理！");
		return false;
		}
	}

	int nDays=GetCountValid(&indexSave,pDoc,10);
    indexSave.iBeginFoot  = nDays ;
	
	
	if(m_bCaclYHParam == TRUE)
	{
	
#ifndef OEM
		if(m_stockchoose.GetSize ()>0)
			CTaiKlineDlgYHParam::CalcYH(&indexSave, &m_stockchoose );
		else
			AfxMessageBox("必须首先加入股票，才能根据这些股票的数据进行优化参数计算！");
#endif
	}


	return true;


}
