// CTaiScreen.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiScreen.h"
#include "windowsx.h"
#include "CTaiKlineFileHS.h"
#include "ColumnSelect.h"
#include "CellRange.h"
#include "CTaiShanReportView.h"
#include "SelectStock.h"
#include "MainFrm.h"
#include "CAlertSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTime g_timeBegin(1999,1,1,8,0,0);
CTime g_timeEnd = CTime::GetCurrentTime();

CTaiScreen::CTaiScreen(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CTaiScreen::IDD, pParent)
{
	m_bShowSetCompute = false;
	m_bExecuteCalc = false;
	//{{AFX_DATA_INIT(CTaiScreen)
	m_choosed_number = 0;
	m_selected_number = 0;
	m_stock_number = 0;
	m_aggregation2 = 0;
	m_operator = 0;
	m_aggregation1 = 0;
	m_aggregation3 = 0;
	m_tmEnd = 0;
	m_nKindScreen = 0;
	m_tmBegin = 0;
	//}}AFX_DATA_INIT
	m_pExplainButton = &m_buttonExpl;
	m_pStockSelectList = &m_liststockrange;
}

void CTaiScreen::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreen)
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_CLOSE1, m_close1);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_comTmBegin);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonExpl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_conTmEnd);
	DDX_Control(pDX, IDC_AGGREGATION_LIST, m_aggregation_list);
	DDX_Control(pDX, IDC_SAVE_LIST, m_save_list);
	DDX_Control(pDX, IDC_STOCK_LIST, m_liststockrange);
	DDX_Text(pDX, IDC_CHOOSED_NUM, m_choosed_number);
	DDX_Text(pDX, IDC_SELECTED_NUMBER, m_selected_number);
	DDX_Text(pDX, IDC_STOCK_NUMBER, m_stock_number);
	DDX_CBIndex(pDX, IDC_AGGREGATION2, m_aggregation2);
	DDX_CBIndex(pDX, IDC_OPERATOR, m_operator);
	DDX_CBIndex(pDX, IDC_AGGREGATION1, m_aggregation1);
	DDX_CBIndex(pDX, IDC_AGGREGATION3, m_aggregation3);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_tmEnd);
	DDX_Radio(pDX, IDC_RADIO1, m_nKindScreen);
	DDX_Control(pDX, IDCHOOSESTOCK, m_btExec);
	DDX_Control(pDX, IDC_ADD_STOCKS_PARENT, m_btAddS);
	DDX_Control(pDX, IDC_DELETE_STOCKS2, m_btDeleteS);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tmBegin);
	DDX_Check(pDX, IDC_CHECK1, m_bCaclYHParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreen, CTaiScreenParent)
	//{{AFX_MSG_MAP(CTaiScreen)
	ON_NOTIFY(NM_CLICK, IDC_STOCK_LIST, OnClickStockList)
	ON_BN_CLICKED(IDCHOOSESTOCK, OnChoosestock)
	ON_BN_CLICKED(IDC_EQUAL, OnEqual)
	ON_COMMAND(ID_TO_GRID, OnToGrid)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioHistory)
	ON_BN_CLICKED(IDC_DELETE_STOCKS2, OnDeleteStocks2)
	ON_BN_CLICKED(IDC_ADD_STOCKS_PARENT, OnAddStocksParent)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonSaveResult)
	ON_NOTIFY(LVN_KEYDOWN, IDC_STOCK_LIST, OnKeydownStockList)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDOK, OnChoosestock)
	ON_BN_CLICKED(IDC_CLOSE1, OnClose1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTaiScreen::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();
	
	m_btDeleteS.SetIcon(IDI_ICON_DELETE);
	m_btAddS.SetIcon(IDI_ICON_ADD2);
	m_btExec.SetIcon(IDI_ICON_SCREEN);


	this->m_treeFormularCtrl.InsertEqutionTree (0,true);
	
	this->m_treeFormularCtrl.InsertEqutionTree (1,true);

	this->m_treeFormularCtrl.InsertEqutionTree (2);
	this->m_treeFormularCtrl.InsertEqutionTree (-6);
	HTREEITEM hh = m_treeFormularCtrl.GetRootItem( );
	m_treeFormularCtrl.EnsureVisible(hh);

	m_and_or = 0;
	UpdateData(FALSE);
	
	LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	myitem.iImage=6;

	m_liststockrange.SetImageList(&m_imageList,LVSIL_SMALL);
	FillToListCtrlHead(&m_liststockrange);

	SelectStock::FillToListCtrlItem(&m_liststockrange, "上证Ａ股", 1);
	SelectStock::FillToListCtrlItem(&m_liststockrange, "深证Ａ股", 4);
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	m_stock_number = m_stockchoose.GetSize ();
	ListCtrlSetSelectItem(&m_liststockrange);


	for(int i=0;i<8;i++)
      m_sSubFormular.AddString(m_subIndexGroup[i]);
    m_sSubFormular.SetCurSel(0);

	OnSelchangeListSubFormular();
	this->m_save_list.SetCurSel(0);
    
	ShowSetCompute(false);


	CTime pMinRange = CTime(1988,9,1,1,1,1);
	CTime pMaxRange = CTime(2037,9,1,1,1,1);
	this->m_conTmEnd.SetRange( &pMinRange, &pMaxRange );
	m_conTmEnd.EnableWindow(FALSE);
	this->m_comTmBegin .EnableWindow(FALSE);
	m_tmEnd = g_timeEnd;
	this->m_tmBegin  = g_timeBegin;
	UpdateData(FALSE);


	AddCComboBoxString(m_save_list,true);
	AddCComboBoxString(*((CComboBox*)GetDlgItem(IDC_AGGREGATION1)));
	AddCComboBoxString(*((CComboBox*)GetDlgItem(IDC_AGGREGATION2)));
	AddCComboBoxString(*((CComboBox*)GetDlgItem(IDC_AGGREGATION3)),true);
	return TRUE;  
} 



void CTaiScreen::OnClickStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}  
void CTaiScreen::OnChoosestock() 
{
	if(m_bExecuteCalc == true)
	{
		m_bExecuteCalc = false;
		EnableControl(true);
	    return;
	}

	UpdateData(TRUE);
    int stocknum = m_stockchoose.GetSize();
	int index_num = m_indexSaveArray.GetSize();

	IndexDataInfo indexSaveCur;
	bool bSelCur = GetCurSelData(indexSaveCur);
	if(m_indexSaveArray.GetSize() == 0 && !bSelCur)
	{
		AfxMessageBox("请先选择指标");
		EnableControl(true);

	    return;
	}
  

	m_resultarray.RemoveAll ();
    Kline *pKline=NULL;
	CBuySellList buySellList ;
	
	int * pnNeedCount = new int [index_num+1];
	for(int i=0;i<index_num;i++)
	{
		IndexDataInfo index_save = m_indexSaveArray.GetAt(i);
		int nDays=index_save.iBeginFoot ;
		pnNeedCount[i]=nDays;
	}
	if(bSelCur) index_num = 1;
    m_progress.SetRange32(0,stocknum);
	m_bExecuteCalc = true;
	EnableControl(false);
    for(int i=0;i<stocknum && m_bExecuteCalc == true;i++)
    {
		m_progress.OffsetPos(1);
		bool bSucc;
		int nRead;
		CString StockId;
		StockId=(m_stockchoose.GetAt(i)).m_chSymbol;
		int stkKind = (m_stockchoose.GetAt(i)).m_nSymbolKind;
		int result = 0 ;

	
		if(this->m_nKindScreen != 0)
		{
			IndexDataInfo index_save ;
			if(bSelCur) 
			{
				index_save = indexSaveCur;
			}
			else
				index_save = m_indexSaveArray.GetAt(index_num-1);
			nRead = ReadKLine(StockId,stkKind, pKline, -1,nLKind[index_save.iDataType],&buySellList, 48 ,TRUE);
			if(CalcEach(index_save, StockId,  stkKind, nRead, pKline, this->m_tmBegin, this->m_tmEnd,&buySellList))
				result=index_num;
			else result = 0;
		}
		else
		//
		{
			for(int k=0;k<index_num;k++)
			{
				IndexDataInfo index_save ;
				if(bSelCur) 
				{
					index_save = indexSaveCur;
				}
				else
					index_save = m_indexSaveArray.GetAt(k);
				int nDays=index_save.iBeginFoot;
				if(nDays<-1)
					continue;

			
				if(this->m_nKindScreen == 0)
					nRead = ReadKLine(StockId,stkKind, pKline, nDays,nLKind[index_save.iDataType],&buySellList, 48 ,TRUE);
				else
					nRead = ReadKLine(StockId,stkKind, pKline, nDays,nLKind[index_save.iDataType],&buySellList, 48 ,TRUE,&this->m_tmEnd );
				
				if(nRead < nDays )
				{
					if(m_and_or == 1)
					  continue;
					else
						break;
				}
				  float a = FormularComputeShares(StockId,stkKind,nRead,&index_save,bSucc,pKline, &buySellList,true);

				  if(a == 1)
					  result++;
				  if(m_and_or&&result>0 ||m_and_or==0&&result<k+1)
					  break;
			}
		}
		if(m_and_or==0&&result==index_num || m_and_or&&result>0)
		{
			SymbolKind sm;
			strcpy(sm.m_chSymbol ,StockId);
			sm.m_nSymbolKind = stkKind;
			this->m_resultarray.Add(sm);
		}
		CTaiShanApp::DispatchMessageEachTime();
		if(CMainFrame::dialogtjxg == NULL) return;

		this->m_selected_number = m_resultarray.GetSize();
		CString  sShow;
		sShow.Format ("%d",m_selected_number);
		GetDlgItem(IDC_SELECTED_NUMBER)->SetWindowText(sShow);
		GetDlgItem(IDC_SELECTED_NUMBER)->UpdateWindow();
		m_choosed_number = i+1;
		sShow.Format ("%d",m_choosed_number);
		GetDlgItem(IDC_CHOOSED_NUM)->SetWindowText(sShow);
		GetDlgItem(IDC_CHOOSED_NUM)->UpdateWindow();

		if(m_bExecuteCalc == false) break;
    }
	UpdateData(FALSE);
	m_progress.SetPos(0);
	CTaiKlineFileHS::RemoveHs(buySellList);
	delete [] pnNeedCount;

	  if(pKline)
	  {
		delete[] pKline;      
		pKline=NULL;
	  }
	

	UpdateData(TRUE);
	if(this->m_save_list.GetCurSel() != 0)
	{
	
		if(m_resultarray.GetSize ()==0)
			AfxMessageBox("选股结果为零！");
		else
		{
			CString s;
			m_save_list.GetWindowText(s);
			CTaiShanReportView::OnSaveScreenStocktype(m_resultarray,&s) ;
		}
	}
	else
	{
		
	  ::PostMessage((AfxGetApp()->m_pMainWnd)->m_hWnd,WM_USER_TJXG,0,0);
	}
	EnableControl(true);
	m_bExecuteCalc = false;
}  

void CTaiScreen::OnCancel() 
{	
	OnClose();
}  

void CTaiScreen::OnEqual() 
{
   UpdateData(TRUE);
   SymbolKindArr tempArray;
   int num1 = 0;
   int num2 = 0;
   int num3 = 0;

   CString sBlockName;
   GetDlgItem(IDC_AGGREGATION1)->GetWindowText(sBlockName);
   SymbolKindArr sArr[2];
   CAlertSystem::GetScreenStockBlockSymbol(sArr[0], sBlockName);
   num1 = sArr[0].GetSize ();
   GetDlgItem(IDC_AGGREGATION2)->GetWindowText(sBlockName);
   CAlertSystem::GetScreenStockBlockSymbol(sArr[1], sBlockName);
   num2 = sArr[1].GetSize ();

   if(num1 == 0 ||num2 == 0)
   {
     AfxMessageBox("请选择一个不为空的集合");
	 return;
   }

   while(m_aggregation_list.GetCount())
      m_aggregation_list.DeleteString(0);
   if(tempArray.GetSize())
      tempArray.RemoveAll();

   tempArray.Copy(sArr[0]);

	SymbolKind strTemp;
   if(m_operator == 0)
   {
	 int i = 0;
	 for(i = 0;i < num2;i++)
	 {
	   strTemp = sArr[1].GetAt(i) ;
	   if(FindStr(&tempArray,strTemp) == -1)
          tempArray.Add(strTemp);
	 }
	 CReportData *Cdat;
	 for(i = 0;i<tempArray.GetSize();i++)
	 {
	    strTemp = tempArray.GetAt(i);
		if(pDoc->m_sharesInformation.Lookup(strTemp.m_chSymbol ,Cdat,strTemp.m_nSymbolKind ))
		{
			CString s = strTemp.m_chSymbol ;
			m_aggregation_list.AddString(s + "  " + Cdat->name);
		}
	 }
     m_choose_result_save.Copy(tempArray);
   }
   
   if(m_operator == 1)
   {
	 int i = 0;
	 for(i = 0;i < num2;i++)
	 {
	   strTemp = sArr[1].GetAt(i);
	   num3 = FindStr(&tempArray,strTemp);
	   if(num3 != -1)
	   {
		   tempArray.RemoveAt(num3);
	   }
	 }
	 CReportData *Cdat;
	 for(i = 0;i<tempArray.GetSize();i++)
	 {
	    strTemp = tempArray.GetAt(i);
		if(pDoc->m_sharesInformation.Lookup(strTemp.m_chSymbol ,Cdat,strTemp.m_nSymbolKind ))
		{
			CString s = strTemp.m_chSymbol ;
			m_aggregation_list.AddString(s + "  " + Cdat->name);
		}
	 }
     m_choose_result_save.Copy(tempArray);
   }


   if(m_operator == 2)
   {
	 int i = 0;
	 tempArray.RemoveAll();
	 for(i = 0;i < num2;i++)
	 {
	   strTemp = sArr[1].GetAt(i);
	   num3 = FindStr(&sArr[0],strTemp);
	   if(num3 != -1)
	   {
		   tempArray.Add(strTemp);
	   }
	 }
	 CReportData *Cdat;
	 for(i = 0;i<tempArray.GetSize();i++)
	 {
	    strTemp = tempArray.GetAt(i);
		if(pDoc->m_sharesInformation.Lookup(strTemp.m_chSymbol ,Cdat,strTemp.m_nSymbolKind ))
		{
			CString s = strTemp.m_chSymbol ;
			m_aggregation_list.AddString(s + "  " + Cdat->name);
		}
	 }
     m_choose_result_save.Copy(tempArray);
   }

   GetDlgItem(IDC_AGGREGATION3)->GetWindowText(sBlockName);
   if(sBlockName =="显示牌")
   {
	   OnToGrid();
   }
   else
   {
		
		if(m_choose_result_save.GetSize ()>0)
		{
			CTaiShanReportView::OnSaveScreenStocktype(m_choose_result_save,&sBlockName) ;
		}
   }
} 

int CTaiScreen::FindStr(SymbolKindArr* array,SymbolKind& str)
{
  SymbolKind strTemp;
  int iArraySize = array->GetSize();
  int i = 0;
  for(i=0;i<iArraySize;i++)
  {
    strTemp = array->GetAt(i);
	if(strcmp(strTemp.m_chSymbol , str.m_chSymbol ) == 0 && strTemp.m_nSymbolKind   == str.m_nSymbolKind )
	{
		return i;
	}
  }
  return -1;
}

BOOL CTaiScreen::PreTranslateMessage(MSG* pMsg)
{
  if(pMsg->message == WM_RBUTTONDOWN)
  {
	  CWnd *pWnd = GetDlgItem(IDC_AGGREGATION_LIST);
	  if(pWnd == NULL)
		  return FALSE;
	  POINT pt;
	  pt.x = pMsg->pt.x;
	  pt.y = pMsg->pt.y;

	  CRect rect(0,0,0,0);
	  pWnd->GetWindowRect(&rect);
	  rect.NormalizeRect();
	  if(rect.PtInRect(pt))
	  {
		return TRUE;
	  }
  }
  return CTaiScreenParent::PreTranslateMessage(pMsg);
}  

void CTaiScreen::OnToGrid() 
{
  if(m_resultarray.GetSize())
	 m_resultarray.RemoveAll();
  if(m_aggregation3 < 0)
	  return;
  if(m_choose_result_save.GetSize() == 0)
	  return;
  m_resultarray.Copy(m_choose_result_save);
  ::PostMessage((AfxGetApp()->m_pMainWnd)->m_hWnd,WM_USER_TJXG,0,0);	
} 

void CTaiScreen::OnButton2() 
{

	ShowExplain(this->m_pIndex);
}

void CTaiScreen::OnButton1()
{
	m_bShowSetCompute?m_bShowSetCompute=false:m_bShowSetCompute=true;
	ShowSetCompute(m_bShowSetCompute);
}

void CTaiScreen::ShowSetCompute(bool bShow)
{
	CString sTile = _T("高级>>");
	CRect r;
	CRect rParent;
	GetWindowRect(rParent);
	if(bShow==true)
	{
		sTile = _T("<<高级");
		GetDlgItem(IDC_AGGREGATION_LIST)->GetWindowRect(r);
		rParent.right = r.right +18;
	}
	else
	{
		GetDlgItem(IDC_BUTTON1)->GetWindowRect(r);
		rParent.right = r.right +8;
	}
	MoveWindow(rParent);
	GetDlgItem(IDC_BUTTON1)->SetWindowText(sTile);
}

void CTaiScreen::OnRadio1() 
{
	m_conTmEnd.EnableWindow(FALSE);
	m_comTmBegin.EnableWindow(FALSE);
	
}

void CTaiScreen::OnRadioHistory() 
{
	m_conTmEnd.EnableWindow(TRUE);
	m_comTmBegin.EnableWindow(TRUE);
	
}

void CTaiScreen::OnDeleteStocks2() 
{
	DeleteStocks();
}

void CTaiScreen::OnAddStocksParent() 
{
	AddStocks();

}

void CTaiScreen::EnableControl(bool bEnable)
{
	int nID[] = {IDC_ADD,IDC_DELETE,IDC_LOAD,IDC_SAVE,IDC_ADD_STOCKS_PARENT,IDC_DELETE_STOCKS2,
	IDC_STOCK_LIST,IDC_TREE_FORMULAR_ALL,IDC_RADIO1,IDC_RADIO3,IDC_DATETIMEPICKER2};

	for(int i=0;i<11;i++)
	{
		if(i==10)
		{
			if(this->m_nKindScreen == 1)
				GetDlgItem(nID[i])->EnableWindow(bEnable);
		}
		else
			GetDlgItem(nID[i])->EnableWindow(bEnable);
	}
	if(bEnable == true)
		GetDlgItem(IDCHOOSESTOCK)->SetWindowText("选股");
	else
		GetDlgItem(IDCHOOSESTOCK)->SetWindowText("停止");
}

void CTaiScreen::OnButtonSaveResult() 
{
	UpdateData();
    if(m_choose_result_save.GetSize() <=0)
	{
		AfxMessageBox("股票个数为零，请重新进行集合运算！");
		return ;
	}
	CString s = "";
	CTaiShanReportView::OnSaveScreenStocktype(m_choose_result_save,&s) ;

	AddCComboBoxString(m_save_list,true);
	AddCComboBoxString(*((CComboBox*)GetDlgItem(IDC_AGGREGATION1)));
	AddCComboBoxString(*((CComboBox*)GetDlgItem(IDC_AGGREGATION2)));
	AddCComboBoxString(*((CComboBox*)GetDlgItem(IDC_AGGREGATION3)),true);
	
}

void CTaiScreen::RefreshJishuTreeCtrl()
{
	m_treeFormularCtrl.DeleteAllItems();


	this->m_treeFormularCtrl.InsertEqutionTree (1);

	this->m_treeFormularCtrl.InsertEqutionTree (0);

	this->m_treeFormularCtrl.InsertEqutionTree (2);

	this->m_pIndex = NULL;
}
void CTaiScreen::PostNcDestroy() 
{
	CMainFrame::dialogtjxg = NULL;
	delete this;
	CTaiScreenParent::PostNcDestroy();
}

void CTaiScreen::OnKeydownStockList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	*pResult = 0;
}

void CTaiScreen::OnClose() 
{
	UpdateData();
	g_timeEnd = m_tmEnd;
	g_timeBegin  = m_tmBegin;
	if(m_bExecuteCalc == false)
		DestroyWindow();
}

void CTaiScreen::AddCComboBoxString(CComboBox &comboBox,bool bInserFirst)
{
	comboBox.ResetContent();
	CStringArray strArr;
	CString s = "显示牌";
	if(bInserFirst == true) strArr.Add (s);
	POSITION position=pDoc->m_Tjxgblocklist.GetHeadPosition();
	while(position)
	{
		BLOCKSTOCK * blocknow=pDoc->m_Tjxgblocklist.GetNext(position);
		int stocklength;
		CString sBlock = blocknow->blockname;
		strArr.Add (sBlock);
	}

	int n = strArr.GetSize ();
	for(int i =0 ;i<n;i++)
	{
		comboBox.AddString(strArr[i]);
	}
	comboBox.SetCurSel (0);
}

BOOL CTaiScreen::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	
	DoHtmlHelp(this);return TRUE;
}

void CTaiScreen::NotifySubclass()
{
	UpdateData();
	SelectStock::GetStockSymbolArray(&m_liststockrange,m_stockchoose);
	m_stock_number = m_stockchoose.GetSize ();
	UpdateData(FALSE);
}

bool CTaiScreen::CalcEach(IndexDataInfo& index_save2,CString StockId, int stkKind, int nCount, Kline *pKline, CTime &tmB, CTime &tmE, CBuySellList *pList)
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
	resultBuy =new int[nRead];
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
	if(index_num>0)
	{
		index_save = m_indexSaveArray.GetAt(index_num-1);
	}
	else 
		index_save = index_save2;

	int nn = index_num;
	if(nn ==0) nn=1;
	for (int k=0;k<nn;k++)
	{
		if(index_num)
			index_save=m_indexSaveArray.GetAt(k);
		
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

	//
	CTime tmB2(tmB.GetYear (),tmB.GetMonth (),tmB.GetDay (),0,0,0); 
	int bt = tmB2.GetTime();
	CTime tmE2(tmE.GetYear (),tmE.GetMonth (),tmE.GetDay (),23,59,59); 
	int et = tmE2.GetTime ();

	int b=-1;
	int e=-1;
	bool bRtn = false;
	for(int ii=0;ii<nRead;ii++)
	{
		if(b==-1)
		{
			if(pKline[ii].day >= bt)
			{
				b = ii;
			}
		}
		if(e==-1)
		{
			if(pKline[ii].day > et)
			{
				e = ii-1;
				if(e<0) goto END_PT;
			}
		}
	}
	if(b==-1) goto END_PT;
	if(e==-1) e = nRead-1;


	for(int ii=b;ii<=e;ii++)
	{
		if(resultSell[ii]+resultBuy[ii]>0)
		{
			bRtn = true;
			break;
		}
	}

END_PT:
	delete [] resultBuy;
	delete [] resultSell;
	if(linebe.line!=NULL)
		delete [] linebe.line;
	CTaiShanApp::DispatchMessageEachTime();
	return bRtn;

}

void CTaiScreen::OnClose1() 
{
	OnClose() ;
}
