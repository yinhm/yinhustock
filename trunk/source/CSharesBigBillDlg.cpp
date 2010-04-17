// CSharesBigBillDlg.cpp : implementation file
// by 俞明录

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "BGridCtrl.h"
#include "CTaiShanDoc.h"
#include "keybrodeangel.h"
#include "SetParam.h"
#include "SelectStock.h"
#include "ProgressDialog.h"
#include "CTaiKlineFileHS.h"
#include "CSharesBigBillDlg.h"
#define ID_TIMER 1998
#define ID_CGRID_MXDIALOG 1999

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_ADDSTOCK WM_USER+1200

/////////////////////////////////////////////////////////////////////////////
// CSharesBigBillDlg dialog


CSharesBigBillDlg::CSharesBigBillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSharesBigBillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSharesBigBillDlg)
	m_Check1 = TRUE;
	m_Check2 = TRUE;
	m_Check3 = TRUE;
	m_Check4 = TRUE;
	m_Radio1 = 0;
	m_Start = 0;
	m_End = 0;
	m_check5 = TRUE;
	m_check6 = TRUE;
	m_stop = FALSE;
	m_bSaveData = FALSE;
	//}}AFX_DATA_INIT
	m_pGrid=NULL;
	m_bListMode=TRUE;
	m_nBigBillCount=0;
	m_nRealRow=0;
	m_bIsCalc=0;
}
CSharesBigBillDlg::~CSharesBigBillDlg()
{
}


void CSharesBigBillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSharesBigBillDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SETUP, m_setup);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_CALC, m_calc);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_END, m_EndCtrl);
	DDX_Control(pDX, IDC_START, m_StartCtrl);
	DDX_Control(pDX, IDC_LIST1, m_MyList);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio1);
	DDX_DateTimeCtrl(pDX, IDC_START, m_Start);
	DDX_DateTimeCtrl(pDX, IDC_END, m_End);
	DDX_Check(pDX, IDC_CHECK5, m_check5);
	DDX_Check(pDX, IDC_CHECK6, m_check6);
	DDX_Check(pDX, IDC_STOP, m_stop);
	DDX_Check(pDX, IDC_SAVE, m_bSaveData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSharesBigBillDlg, CDialog)
	//{{AFX_MSG_MAP(CSharesBigBillDlg)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CALC, OnCalc)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK6, OnCheck6)
	ON_BN_CLICKED(IDC_SETUP, OnSetup)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_SAVE, OnSaveBigBillData)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStock)
    ON_MESSAGE(WM_USER_CHANGECOLOR,OnGridColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CSharesBigBillDlg::OnOK() 
{

    int l_Rows=m_pGrid->GetCurrentRow();
	if(l_Rows == 0)
	   return ;
	PBIG_LIST pCjmx;
	if(!m_pDoc->m_pStockBigBillData->GetBigBillData(l_Rows-1,pCjmx))
	   return;
	CString	stock_code = pCjmx->id;
    if(stock_code.Compare("") == 0)
        return;
	m_pDoc->m_sharesSymbol = stock_code;
	m_pDoc->m_stkKind=pCjmx->nKind;
	m_pDoc->m_GetFocus = true;
	CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
    pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer(0),m_pDoc->m_stkKind*10);
	this->ShowWindow(SW_MINIMIZE);   
}

BOOL CSharesBigBillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	time_t now; 
	time( &now );
	m_End = now;
	m_Start = now;

 	DWORD dwStyle=0;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE|WS_BORDER ;
	dwStyle &= ~WS_HSCROLL;

	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;

	m_pDoc->m_pStockCjMxTj=this;


	m_pGrid =new CBGridCtrl;
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->Create(CRect(0,0,10,10),this,ID_CGRID_MXDIALOG,dwStyle);
  
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
    m_pGrid->MoveWindow(13,20,433,260, TRUE);
    m_pGrid->SetColumnWidth(0,53);
    m_pGrid->SetColumnWidth(1,58);
    m_pGrid->SetColumnWidth(2,85);
    m_pGrid->SetColumnWidth(3,60);
    m_pGrid->SetColumnWidth(4,60);
    m_pGrid->SetColumnWidth(5,45);
    m_pGrid->SetColumnWidth(6,52);

	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetRowResize(FALSE);
	m_pGrid->SetColumnResize(FALSE);


    m_pGrid->SetFocus();
    m_pGrid->SetFocusRow(1); 
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);

    m_pGrid->Invalidate();                          


	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);   
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	FillToListCtrlHead();
	InitBigBillData();
	DisplayInfomation();
	this->UpdateData(FALSE);

    VisibleCells = m_pGrid->GetVisibleNonFixedCellRange();
    m_VisibleRowCount=VisibleCells.GetMaxRow() - VisibleCells.GetMinRow ()  + 1; 

    m_pImageList->Detach();
    if(m_pImageList) delete m_pImageList;

	m_pImageListSmall->Detach();
    if(m_pImageListSmall) delete m_pImageListSmall;
	SetTimer(ID_TIMER,3000,NULL);

	return TRUE;  
}

void CSharesBigBillDlg::PostNcDestroy() 
{
	

	if(m_pGrid)delete m_pGrid;
	m_pDoc->m_pStockCjMxTj=NULL;
	if(m_pDoc->m_pStockBigBillData)
	{
		m_pDoc->m_pStockBigBillData->m_pBigBillHead->Done=FALSE;
		delete m_pDoc->m_pStockBigBillData;
	    m_pDoc->m_pStockBigBillData=NULL;

	}
	delete this;
	CDialog::PostNcDestroy();
}

void CSharesBigBillDlg::OnClose() 
{
	
	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}
    FILE *fp;
	fp=fopen("BigBill.ini","w+b");
    if(fp!=NULL) 
	{
		int nId=8888;
		fwrite(&nId,4,1,fp);
      
	    int nCount=m_MyList.GetItemCount();
        fwrite(&nCount,1,4,fp);
	    for(int i=0;i<nCount;i++)
		{
	        char code[10];
            int nParam=m_MyList.GetItemData(i);
	        m_MyList.GetItemText( i, 0, code, 10);
	        fwrite(&nParam,1,4,fp);
	        fwrite(code,1,10,fp);
		}
	    fclose(fp);
	}

	KillTimer(ID_TIMER);
	CDialog::OnClose();
    DestroyWindow();	
}
void CSharesBigBillDlg::InitAllTitle()
{
    m_pGrid->SetColumnCount(5);

	m_nRows = 500;
	m_nFixRows = 1;
	m_nFixCols = 1;
	CString str;

	str.Format("代 码");
	m_ColumnTitleArray.Add(str);
	str.Format("证券名称");
	m_ColumnTitleArray.Add(str);
	str.Format("操作日期");
	m_ColumnTitleArray.Add(str);
	str.Format("操作类型");
	m_ColumnTitleArray.Add(str);
	str.Format("数量(手)");
	m_ColumnTitleArray.Add(str);
	str.Format("价格");
	m_ColumnTitleArray.Add(str);
	str.Format("涨跌幅");
	m_ColumnTitleArray.Add(str);

	
	m_nCols=m_ColumnTitleArray.GetSize(); 

}
int CSharesBigBillDlg::InitGridCtrl()   
{

	return 0;
}
BOOL CSharesBigBillDlg::InitGridContent() 
{
    m_pGrid->SetMainDocument(m_pDoc);
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


    for(int i=0;i<m_ColumnTitleArray.GetSize();i++)
	{
		CString str=m_ColumnTitleArray.GetAt(i);
        m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),i);
	}
 	return TRUE;
}

void CSharesBigBillDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	
}

BOOL CSharesBigBillDlg::PreTranslateMessage(MSG* pMsg) 
{

	CRect rc(13,20,433,260);
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE )
	{
		this->OnClose();
		return TRUE;
	}
	if(pMsg->message==WM_LBUTTONDOWN)
	{
		CPoint m_pos;
	    int nRow=m_pGrid->GetCurrentRow();
		m_pos=pMsg->pt;
		m_pos.y -=GetSystemMetrics(SM_CYCAPTION);
		m_pos.x -=13;
		ScreenToClient(&m_pos);
		m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);
 		if(m_CellTitleCommand.row==0&&m_CellTitleCommand.col==0)
		{
			int ss=m_pGrid->GetSortColumn();
			if(m_pGrid->GetSortColumn()!=-1)
			{
				 m_pGrid->SetSortColumn(-1 ,TRUE);
				 m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);

				 m_pGrid->QuickSortString();
			 	 m_pGrid->ClearSortColumn();
				 m_pGrid->SetFocusRow(1); 
				 m_pGrid->Invalidate();                         
  				 m_pGrid->SetMouseState(MOUSE_NOTHINGS);
			}
			return true;
		}
	}
	CWnd *pWnd=this->GetFocus();
	CWnd *pChildWnd1=this->GetDlgItem(IDC_START);
	CWnd *pChildWnd2=this->GetDlgItem(IDC_END);
    
	if((pWnd!=pChildWnd1&&pWnd!=pChildWnd2)&&m_check6)
	{ 
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
			   int nKind=m_diakeybrode.m_stkKind;
			   strcpy(result,m_diakeybrode.result);
			   
			   if(strlen(result)==6||strlen(result)==4)
			   {
				   FillToListCtrlItem(result,-1-nKind,TRUE);
				   m_pDoc->m_sharesInformation.RemoveStockCjmxTj(result,nKind,TRUE);
					int rtn = MessageBox("统计股票范围已发生变化,是否对大单提取进行数据刷新操作!","警告",MB_YESNO|MB_ICONWARNING);
					if(rtn==6)
					{
					   OnCalc();
					}

			   }
			}
			return true;
		}
	}
	if(pMsg->message==WM_LBUTTONDBLCLK)
	{
        CCellID m_CellTitleCommand;
		CPoint m_pos;
		CRect rc;
		m_pos=pMsg->pt;
		m_pos.y -=GetSystemMetrics(SM_CYCAPTION);
		m_pos.x -=13;
        m_pGrid->GetClientRect(&rc);
		ScreenToClient(&rc);
		ScreenToClient(&m_pos);
        if(rc.PtInRect(m_pos))
		{
			m_CellTitleCommand= m_pGrid->GetCellFromPt(m_pos);
			if(m_CellTitleCommand.row == 0||m_CellTitleCommand.col==-1)
			   return FALSE;
			if(m_CellTitleCommand.row > 0 )
			{
				PBIG_LIST pCjmx;
	            if(!m_pDoc->m_pStockBigBillData->GetBigBillData(m_CellTitleCommand.row-1,pCjmx))
					return FALSE;
				CString stock_code = pCjmx->id;
				if(stock_code.Compare("") == 0)
					return FALSE;
				
				m_pDoc->m_sharesSymbol = stock_code;
				m_pDoc->m_stkKind=pCjmx->nKind;
				m_pDoc->m_GetFocus = true;
				CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
				pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer (0),m_pDoc->m_stkKind*10);
				this->ShowWindow(SW_MINIMIZE);   
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
void CSharesBigBillDlg::FillToListCtrlHead()
{ 
   	CRect           rect;
	CString         strItem1= _T("证券名称");
	CString         strItem2= _T("证券类型");
	m_MyList.SetExtendedStyle(m_MyList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	this->m_MyList.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_MyList.SetImageList(m_pImageListSmall, LVSIL_SMALL);


	this->m_MyList.GetWindowRect(&rect);
	this->m_MyList.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1/2, 0);
	this->m_MyList.InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 1/2, 1);
}
void CSharesBigBillDlg::FillToListCtrlItem(char *Id,int nParam,BOOL StockType)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LPTSTR   pStrTemp1, pStrTemp2;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=Id;
    if(m_MyList.FindItem(&lvf)>=0)
		return;
    iItem=this->m_MyList.GetItemCount();
	for (iSubItem = 0; iSubItem <2; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0; 

		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

	
		CString strIconDesc, strIconShortDesc;
        strIconShortDesc=Id;


		if(StockType>0)
		   strIconDesc="股票";
		else
		   strIconDesc="板块";
		pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		pStrTemp2= strIconDesc.GetBuffer(strIconDesc.GetLength());
		lvitem.pszText = iSubItem == 0? pStrTemp1 : pStrTemp2;

		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_MyList.InsertItem(&lvitem);
		else
			this->m_MyList.SetItem(&lvitem);
	} 
	this->m_MyList.SetItemData( iItem, nParam );

}

void CSharesBigBillDlg::OnDelete() 
{

    CString Id;
	int temp=m_MyList.GetNextItem( -1, LVNI_SELECTED   ) ;
    int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;

	if(temp==-1)
		return;
	while(temp>=0)
	{
		m_MyList.DeleteItem(temp);
        temp=m_MyList.GetNextItem( -1,LVNI_SELECTED   ) ;
	}
	m_pDoc->m_sharesInformation.RemoveAllStockCjmxTj();
    int tmp=m_MyList.GetItemCount();
	int i;
	for(i=0;i<tmp;i++)
	{
	   char code[10];
       int nParam=m_MyList.GetItemData(i);
	   m_MyList.GetItemText( i, 0, code, 10);
	   if(nParam<0)
	   {
		   int nKind=-1-nParam;
		   m_pDoc->m_sharesInformation.RemoveStockCjmxTj(code,nKind,TRUE);
	   }
	   else
	   {
		  if(nParam==CHOOSESTK)
		  {
			  SymbolKindArr l_StockArray;
			  m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
			  int DisplayCount=l_StockArray.GetSize();

			  for(int row=0 ;row < (int)DisplayCount  ;row++)          
			  {
				 SymbolKind l_CodeSymbol=l_StockArray.GetAt(row);
	             m_pDoc->m_sharesInformation.RemoveStockCjmxTj(l_CodeSymbol.m_chSymbol,l_CodeSymbol.m_nSymbolKind,TRUE);
			  }
		  }
		  else if(nParam<CHOOSESTK)
		  {
			   int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(nParam);
			   for(int j=0;j<temp;j++)
			   {
			 	    CReportData *Cdat;
				    m_pDoc->m_sharesInformation.GetStockItem(nParam,j,Cdat);
				    if(Cdat==NULL)
					   continue;
					Cdat->IsMxTj=TRUE;
			   }
		  }
		  else 
		  {
				SymbolKindArr l_StockCodeArray;
				CStringArray l_StockTypeNameArray;
		        m_pDoc->m_ManagerStockTypeData.GetAllStockTypeName(l_StockTypeNameArray);
				CString l_sStockTypeName=l_StockTypeNameArray.GetAt(nParam-1000);
				m_pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
				int DisplayCount=l_StockCodeArray.GetSize();
				for(int row=0 ;row <  (int)DisplayCount  ;row++)           
				{    
					 SymbolKind m_SymbolKind; 
					 m_SymbolKind=l_StockCodeArray.GetAt(row) ;
		             m_pDoc->m_sharesInformation.RemoveStockCjmxTj(m_SymbolKind.m_chSymbol,m_SymbolKind.m_nSymbolKind,TRUE);
				}
		  }
	   }
	}
	rtn = MessageBox("统计股票范围已发生变化,是否对大单提取进行数据刷新操作!","警告",MB_YESNO|MB_ICONWARNING);
	if(rtn==6)
	{
       OnCalc();
	}
	if(i>0)
	{
       m_MyList.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	}
}
 
void CSharesBigBillDlg::OnRadio1() 
{

	GetDlgItem(IDC_START)-> EnableWindow(FALSE);
	GetDlgItem(IDC_END)-> EnableWindow(FALSE);
}

void CSharesBigBillDlg::OnRadio2() 
{

	GetDlgItem(IDC_START)-> EnableWindow(TRUE);
	GetDlgItem(IDC_END)-> EnableWindow(TRUE);
}
void CSharesBigBillDlg::DisplayInfomation()
{
CString nStr="";
char gb[20];
   if(m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsGbjg)
	   strcpy(gb,"%股本");
   else
	   strcpy(gb,"手数");
   if(m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV==0)
	   return;
if(!m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsGbjg)
nStr.Format("成交大单统计:\r\n\
   大单买入:  笔数%d ,  手数%10.2f;\r\n\
   大单卖出:  笔数%d ,  手数%10.2f;\r\n\
   大单挂买:  笔数%d ,  手数%10.2f;\r\n\
   大单挂卖:  笔数%d ,  手数%10.2f;\r\n\
成交分类统计:\r\n\
    %.2f %s以下:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f %s以上:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n",
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[3],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[3],

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[0],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[0]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[0],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[1],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[1]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[1],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[2],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[2]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[2],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[3],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[3],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[3],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[3]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[3],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[4],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[4],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[4],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[4]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[4],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[5],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[5],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[5],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[5]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[5],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[6],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[6],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[6],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[6]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[6],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[7],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[7],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[7],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[7]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[7],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[8],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[8],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[8],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[8]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetType[8],
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[9],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[9],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[9]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV
);
else
nStr.Format("成交大单统计:\r\n\
   大单买入:  笔数%d ,  手数%10.2f;\r\n\
   大单卖出:  笔数%d ,  手数%10.2f;\r\n\
   大单挂买:  笔数%d ,  手数%10.2f;\r\n\
   大单挂卖:  笔数%d ,  手数%10.2f;\r\n\
成交分类统计:\r\n\
    %.2f %s以下:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f%s--%.2f%s:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n\
   %.2f %s以上:  笔数%ld ,  手数%.2f , 占总成交%.2f;\r\n",
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigCount[3],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalBigVolumn[3],

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[0]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[0],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[0]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[0]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[1]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[1],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[1]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[1]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[2]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[2],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[2]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[2]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[3]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[3],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[3],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[3]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[3]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[4]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[4],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[4],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[4]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[4]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[5]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[5],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[5],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[5]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[5]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[6]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[6],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[6],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[6]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[6]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[7]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[7],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[7],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[7]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[7]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[8]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[8],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[8],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[8]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV,

m_pDoc->m_pStockBigBillData->m_pBigBillHead->BigBillSetTypeGB[8]*100,
gb,
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalCount[9],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[9],
m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalVolumn[9]/m_pDoc->m_pStockBigBillData->m_pBigBillHead->totalV
);

this->m_edit.SetWindowText(nStr);
}
void CSharesBigBillDlg::InitBigBillFileData()
{
   FILE *fp;
   fp=fopen("BigBill.ini","rb");
   if(fp!=NULL)
   {
		int nId=0;
		int nCount=0;
		fread(&nId,4,1,fp);
		if(nId!=8888)
		{
		    AfxMessageBox("不是大单管理信息文件!");
			fclose(fp);
            return;
		}
        fread(&nCount,1,4,fp);
	    m_pDoc->m_sharesInformation.RemoveAllStockCjmxTj();
		for(int i=0;i<nCount;i++)
		{
           char code[10];
		   int nParam;
		   fread(&nParam,1,4,fp);
		   fread(code,1,10,fp);
		   if(nParam<0)
		   {
              int nKind=-1-nParam;
			  FillToListCtrlItem(code,nParam,TRUE);
			  m_pDoc->m_sharesInformation.RemoveStockCjmxTj(code,nKind,TRUE);
		   }
		   else
		   {
			  if(nParam==0||nParam==3||nParam==8)
				  continue; 
			  FillToListCtrlItem(code,nParam,FALSE);
			  if(nParam==CHOOSESTK)
			  {
				  SymbolKindArr l_StockArray;
				  m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
				  int DisplayCount=l_StockArray.GetSize();
             
				  for(int row=0 ;row <DisplayCount  ;row++)          
				  {
					 SymbolKind l_SymbolKind;
					 l_SymbolKind=l_StockArray.GetAt(row);
					 m_pDoc->m_sharesInformation.RemoveStockCjmxTj(l_SymbolKind.m_chSymbol,l_SymbolKind.m_nSymbolKind,TRUE);
				  }
			  }
			  else if(nParam<CHOOSESTK)
			  {
				   int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(nParam);
				   for(int j=0;j<temp;j++)
				   {
			 			CReportData *Cdat;
						m_pDoc->m_sharesInformation.GetStockItem(nParam,j,Cdat);
						if(Cdat==NULL)
						   continue;
						Cdat->IsMxTj=TRUE;
				   }
			  }
			  else 
			  {
					SymbolKindArr l_StockCodeArray;
					CStringArray l_StockTypeNameArray;
					m_pDoc->m_ManagerStockTypeData.GetAllStockTypeName(l_StockTypeNameArray);
					CString l_sStockTypeName=l_StockTypeNameArray.GetAt(nParam-1000);
					m_pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
					int DisplayCount=l_StockCodeArray.GetSize();
					for(int row=0 ;row <  (int)DisplayCount  ;row++)           
					{    
						 SymbolKind m_SymbolKind; 
						 m_SymbolKind=l_StockCodeArray.GetAt(row) ;
						 m_pDoc->m_sharesInformation.RemoveStockCjmxTj(m_SymbolKind.m_chSymbol,m_SymbolKind.m_nSymbolKind,TRUE);
					}
			  }
		   }
		}
	    fclose(fp);
   }

}

void CSharesBigBillDlg::InitBigBillData()
{


    m_pDoc->m_pStockBigBillData=new CSharesBigBillData;

	m_Check1 =m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsCalc[0] ;
	m_Check2 =m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsCalc[1] ;
	m_Check3 =m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsCalc[2] ;
	m_Check4 =m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsCalc[3] ;
	this->m_bSaveData=m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsSaveData;
	if(this->m_bSaveData)
	{
		InitBigBillFileData();
		m_nRealRow=m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount;
        m_nBigBillCount=m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount;
 		try { m_pGrid->SetRowCount(m_nRealRow+100); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		m_nRows=m_nRealRow+100;
	}
	else
	{
		InitBigBillFileData();
        m_pDoc->m_pStockBigBillData->ClearRealTimeHsData();
 		m_nRealRow=0;
        m_nBigBillCount=0;
		try { m_pGrid->SetRowCount(m_nRealRow+500); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}
		m_nRows=m_nRealRow+500;
	}
    m_pGrid->SetRealRow(m_nRealRow+1);
	m_pGrid->GotoLine(1,TRUE);
	m_pGrid->Invalidate(); 
	m_pDoc->m_pStockBigBillData->m_pBigBillHead->Done=TRUE;
}
void CSharesBigBillDlg::SetGridRealRowData()
{
    m_nRealRow=m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount;;
	m_nBigBillCount=m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount;
    AddGridRow(m_nRealRow );

	m_pGrid->Invalidate(); 
    if(m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsChange)
	{
      DisplayInfomation();
	  m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsChange=FALSE;
	}
}
void CSharesBigBillDlg::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent==ID_TIMER&&m_nBigBillCount<m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount)
	{
		if(this->m_stop)
		{
			CDialog::OnTimer(nIDEvent);
			return;
		}
   
	    AddGridRow(m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount);
		m_nRealRow=m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount;


	    m_nBigBillCount=m_pDoc->m_pStockBigBillData->m_pBigBillHead->ItemCount;
		m_pGrid->Invalidate(); 
	}
    if(m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsChange)
	{
      DisplayInfomation();
	  m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsChange=FALSE;
	}
	CDialog::OnTimer(nIDEvent);
}

void CSharesBigBillDlg::OnCalc() 
{

	m_bIsCalc=TRUE;
    CWnd *pWnd=this->GetDlgItem(IDC_CALC);
	pWnd->EnableWindow(FALSE);
    pWnd=this->GetDlgItem(IDCANCEL);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

	this->UpdateData(TRUE);
    m_pDoc->m_pStockBigBillData->ClearRealTimeHsData();
	m_pDoc->m_pStockBigBillData->m_pBigBillHead->Done=FALSE;
	KillTimer(ID_TIMER);

    m_nRealRow=0;
	m_nBigBillCount=0;
    m_pGrid->SetRealRow(m_nRealRow+1); 
    m_pGrid->GotoLine(1,TRUE);
    m_pGrid->Invalidate();

	if(!this->m_Radio1)
	{
         GetBigBillDataToday();
	}
	else
	{
		 if(this->m_Start.GetTime()>this->m_End.GetTime())
		 {
			 MessageBox("历史大单提取开始时间不能大于结束时间!","时间操作提示",MB_OK);
			 CDateTimeCtrl *pWnd=(CDateTimeCtrl *)this->GetDlgItem(IDC_END);
			 pWnd->SetFocus();
			 return;
		 }
         GetBigBillDataHistory();
	}
	SetGridRealRowData();
	m_pDoc->m_pStockBigBillData->m_pBigBillHead->Done=TRUE;
    m_pGrid->PostMessage(WM_KEYDOWN,VK_END,0);
	SetTimer(ID_TIMER,3000,NULL);
    pWnd=this->GetDlgItem(IDC_CALC);
	pWnd->EnableWindow(TRUE);
    pWnd=this->GetDlgItem(IDCANCEL);
	pWnd->EnableWindow(TRUE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
	m_bIsCalc=FALSE;
}
void CSharesBigBillDlg::AddGridRow(int CurrentRow )
{
	int TotRow=CurrentRow;
	int lAddRows=CurrentRow -m_nRealRow;

	if( CurrentRow >= m_nRows) 
	{
		TotRow=TotRow+100;
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
void CSharesBigBillDlg::ExpandDialog(BOOL nDisplayTj,BOOL nDisplayAll)
{
	static CRect rcLarge;
	static CRect rcMiddle;
	static CRect rcSmall;
	CString sExpand;
	if(rcLarge.IsRectEmpty() )
	{
		CRect rcLandmark1;
		CRect rcLandmark2;
		CWnd * pWnd1=GetDlgItem(IDC_DISPLAYTJ);
		ASSERT(pWnd1);
		CWnd * pWnd2=GetDlgItem(IDC_DISPLAYSET);
		ASSERT(pWnd2);
		GetWindowRect(rcLarge);
		
		pWnd1->GetWindowRect(rcLandmark1) ;
		pWnd2->GetWindowRect(rcLandmark2) ;

		rcSmall=rcLarge;
		rcSmall.bottom =rcLandmark1.bottom;
		rcSmall.right=rcLandmark2.right;

		rcMiddle=rcLarge;
        rcMiddle.right=rcLandmark2.right;
	}
    if(nDisplayTj==FALSE&&nDisplayAll==FALSE)
	{
		SetWindowPos(NULL,0,0,rcSmall.Width(),rcSmall.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
		CWnd * pWnd1=GetDlgItem(IDC_TJDISP);
		ASSERT(pWnd1);
        pWnd1->ShowWindow(SW_HIDE);

	}
	else if(nDisplayTj==TRUE&&nDisplayAll==FALSE)
	{
		SetWindowPos(NULL,0,0,rcMiddle.Width(),rcMiddle.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
		CWnd * pWnd1=GetDlgItem(IDC_TJDISP);
		ASSERT(pWnd1);
        pWnd1->ShowWindow(SW_SHOW);
	}else if(nDisplayAll==TRUE)
	{
		SetWindowPos(NULL,0,0,rcLarge.Width(),rcLarge.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
		CWnd * pWnd1=GetDlgItem(IDC_TJDISP);
		ASSERT(pWnd1);
        pWnd1->ShowWindow(SW_SHOW);
	}
}
void CSharesBigBillDlg::EnableVisibleChildren()
{
   CWnd *pWndCtl=GetWindow(GW_CHILD);
   CRect rcTest;
   CRect rcControl;
   CRect rcShow;
   GetWindowRect(rcShow);
   while(pWndCtl!=NULL)
   {
	   pWndCtl->GetWindowRect(rcControl);

	   if(rcTest.IntersectRect(rcShow,rcControl) )
		   pWndCtl->EnableWindow(TRUE);
	   else
		   pWndCtl->EnableWindow(FALSE);
       
	   pWndCtl=pWndCtl->GetWindow(GW_HWNDNEXT);
   }
}


void CSharesBigBillDlg::OnCheck5() 
{

	this->UpdateData(TRUE);
    ExpandDialog(m_check5, m_check6);
}
void CSharesBigBillDlg::OnSaveBigBillData()
{
	this->UpdateData(TRUE);
    m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsSaveData=this->m_bSaveData;
}

void CSharesBigBillDlg::OnCheck6() 
{

	this->UpdateData(TRUE);
    ExpandDialog(m_check5, m_check6);
}

void CSharesBigBillDlg::OnSetup() 
{

	float pBigData[4],pBigMaxData[4],CjmxType[10];
	float pBigDataGB[4],pBigMaxDataGB[4],CjmxTypeGB[10];
	BOOL IsGB=m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsGbjg;
    m_pDoc->m_pStockBigBillData->GetParamData(pBigData,pBigMaxData,CjmxType);
    m_pDoc->m_pStockBigBillData->GetParamDataGB(pBigDataGB,pBigMaxDataGB,CjmxTypeGB);

    SetParam nDlg(pBigData,pBigMaxData,CjmxType,pBigDataGB,pBigMaxDataGB,CjmxTypeGB, m_pDoc->m_pStockBigBillData->m_pBigBillHead->IsGbjg,this);  
    if(nDlg.DoModal( )==IDOK)
	{
		m_pDoc->m_pStockBigBillData->SetBigBillBigType(nDlg.IsGb);
		m_pDoc->m_pStockBigBillData->SetBigDataForSetup(nDlg.nBig,nDlg.nBigMax,nDlg.nParam);
		m_pDoc->m_pStockBigBillData->SetBigDataForSetupGB(nDlg.nBigGB,nDlg.nBigMaxGB,nDlg.nParamGB);
		if(nDlg.IsGb!=IsGB||
			memcmp(pBigData,nDlg.nBig,sizeof(float)*4)!=0||memcmp(nDlg.nBigMax,pBigMaxData,sizeof(float)*4)!=0||memcmp(CjmxType,nDlg.nParam,sizeof(float)*9)!=0||
			memcmp(pBigDataGB,nDlg.nBigGB,sizeof(float)*4)!=0||memcmp(nDlg.nBigMaxGB,pBigMaxDataGB,sizeof(float)*4)!=0||memcmp(CjmxTypeGB,nDlg.nParamGB,sizeof(float)*9)!=0)
		{
			int rtn = MessageBox("统计股票参数设置已发生变化,是否对大单提取进行数据刷新操作!","警告",MB_YESNO|MB_ICONWARNING);
			if(rtn==6)
			{
			   OnCalc();
			}
		}
		this->DisplayInfomation();
	}
}


void CSharesBigBillDlg::OnCheck1() 
{
	
	BOOL IsCalc[4];
	this->UpdateData(TRUE);
	IsCalc[0]=this->m_Check1;
	IsCalc[1]=this->m_Check2;
	IsCalc[2]=this->m_Check3;
	IsCalc[3]=this->m_Check4;

    m_pDoc->m_pStockBigBillData->SetIsCalcForBig(IsCalc);
	
}
void CSharesBigBillDlg::OnCancel() 
{

	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}
    FILE *fp;
	fp=fopen("BigBill.ini","w+b");
    if(fp!=NULL) 
	{
		int nId=8888;
		fwrite(&nId,4,1,fp);
      
	    int nCount=m_MyList.GetItemCount();
        fwrite(&nCount,1,4,fp);
	    for(int i=0;i<nCount;i++)
		{
	        char code[10];
            int nParam=m_MyList.GetItemData(i);
	        m_MyList.GetItemText( i, 0, code, 10);
	        fwrite(&nParam,1,4,fp);
	        fwrite(code,1,10,fp);
		}
	    fclose(fp);
	}
	KillTimer(ID_TIMER);
	CDialog::OnCancel();
    DestroyWindow();	
}
void CSharesBigBillDlg::OnAdd() 
{
	SelectStock pDlg(this,FALSE);
	pDlg.DoModal();
}
LRESULT CSharesBigBillDlg::OnAddStock(WPARAM wParam, LPARAM lParam)
{
	SHARES_DATA_ADD *pAddMxtj;
	int lsCount=(int)lParam;
    pAddMxtj=(SHARES_DATA_ADD *)wParam;
	for(int i=0;i<lsCount;i++)
	{
		if(pAddMxtj[i].StockType<0)
		{
			int nKind=-1-pAddMxtj[i].StockType;
            FillToListCtrlItem(pAddMxtj[i].name,pAddMxtj[i].StockType,TRUE);
		    m_pDoc->m_sharesInformation.RemoveStockCjmxTj(pAddMxtj[i].name,nKind,TRUE);
		}
		else
		{
		  int nParam=pAddMxtj[i].StockType;
		  if(nParam==0||nParam==3||nParam==8)
              continue; 
          FillToListCtrlItem(pAddMxtj[i].name,pAddMxtj[i].StockType,FALSE);
		  if(nParam==CHOOSESTK)
		  {
			  SymbolKindArr l_StockArray;
			  m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
			  int DisplayCount=l_StockArray.GetSize();
             
			  for(int row=0 ;row <DisplayCount  ;row++)           
			  {
				 SymbolKind l_SymbolKind;
				 l_SymbolKind=l_StockArray.GetAt(row);
	             m_pDoc->m_sharesInformation.RemoveStockCjmxTj(l_SymbolKind.m_chSymbol,l_SymbolKind.m_nSymbolKind,TRUE);
			  }
		  }
		  else if(nParam<CHOOSESTK)
		  {
			   int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(nParam);
			   for(int j=0;j<temp;j++)
			   {
			 	    CReportData *Cdat;
				    m_pDoc->m_sharesInformation.GetStockItem(nParam,j,Cdat);
				    if(Cdat==NULL)
					   continue;
					Cdat->IsMxTj=TRUE;
			   }
		  }
		  else 
		  {
				SymbolKindArr l_StockCodeArray;
				CStringArray l_StockTypeNameArray;
		        m_pDoc->m_ManagerStockTypeData.GetAllStockTypeName(l_StockTypeNameArray);
				CString l_sStockTypeName=l_StockTypeNameArray.GetAt(nParam-1000);
				m_pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
				int DisplayCount=l_StockCodeArray.GetSize();
				for(int row=0 ;row <  (int)DisplayCount  ;row++)           
				{    
					 SymbolKind m_SymbolKind; 
					 m_SymbolKind=l_StockCodeArray.GetAt(row) ;
		             m_pDoc->m_sharesInformation.RemoveStockCjmxTj(m_SymbolKind.m_chSymbol,m_SymbolKind.m_nSymbolKind,TRUE);
				}
		  }
		}
	}
//***********************************
	if(lsCount>0)
	{
		int rtn = MessageBox("统计股票范围已发生变化,是否对大单提取进行数据刷新操作!","警告",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
           OnCalc();
		}
	}
//***********************************

	return 0;
}


void CSharesBigBillDlg::OnStop() 
{
	// 
	this->UpdateData(TRUE);
}
void CSharesBigBillDlg::GetBigBillDataToday()
{
	MSG message;
	int index=0;
 	CProgressDialog *dlg;
	dlg=new CProgressDialog;
	dlg->Create(IDD_DIALOG_PROGRESS,NULL);
    dlg->ShowWindow(SW_SHOW); 
	dlg->m_DialogDown =FALSE;
	CWnd *pWnd;
//
//
	pWnd=dlg->GetDlgItem(IDC_ZQDM) ;
	pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_DISPLAY_INFO) ;
	pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_COMP) ;
	pWnd->ShowWindow(SW_HIDE);
	CButton *m_pButton;
	m_pButton =(CButton *)dlg->GetDlgItem(IDOK); 
    m_pButton->EnableWindow(FALSE) ;
	dlg->SetWindowText(_T("当天明细数据提取"));
    CProgressCtrl *pProgressCtrl;
	pProgressCtrl =(CProgressCtrl *)dlg->GetDlgItem(IDC_PROGRESS); 
	CTaiKlineFileHS *pTempFileHS;

    pProgressCtrl->SetRange(0,m_pDoc->m_sharesInformation.GetCount());
    pProgressCtrl->SetPos(0);
	for(int i=0;i<10;i++)
	{
		if(i==0||i==3||i==8)
			continue;
	    int l_nStockCount=m_pDoc->m_sharesInformation.GetStockTypeCount(i);
	    for(int j=0;j<l_nStockCount;j++)
		{
			CReportData *Cdat;
			m_pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
			   continue;
			if(!Cdat->IsMxTj)
				continue;
            CBuySellList pBuySellList;
			KlineEx *pKlineEx=NULL;
			TRADE_DETAIL_BIG_LIST_PER *pBigBillData;
			int nTotalCount=0;
			if(Cdat->kind==SHAG||Cdat->kind==SHBG||Cdat->kind==SHZQ)
			{
                CTaiKlineFileHS::m_fileHsSh->BillTransferDataEx(CTaiKlineFileHS::m_fileHsSh, Cdat->id, &pBuySellList, pKlineEx);
				pTempFileHS=CTaiKlineFileHS::m_fileHsSh;
			}
			else if(Cdat->kind==SZAG||Cdat->kind==SZBG||Cdat->kind==SZZQ)
			{
                CTaiKlineFileHS::m_fileHsSz->BillTransferDataEx(CTaiKlineFileHS::m_fileHsSz, Cdat->id, &pBuySellList, pKlineEx);
				pTempFileHS=CTaiKlineFileHS::m_fileHsSz;
			}
            nTotalCount=pBuySellList.GetCount();
			HGLOBAL hMem=GlobalAlloc(  GPTR, sizeof(TRADE_DETAIL_BIG_LIST_PER)*nTotalCount);
			if(hMem!=NULL)
			   pBigBillData=(TRADE_DETAIL_BIG_LIST_PER *)GlobalLock(  hMem );
			else
               continue; 
            for(int i=0;i<nTotalCount;i++)
            {
                  TRADE_DETAIL_H_PER *pBuySellData;
                  pBuySellData=pTempFileHS->GetAtBS(&pBuySellList, i,nTotalCount);
                  memcpy(&pBigBillData[i].vol, &pBuySellData->vol,12);
                  memcpy(&pBigBillData[i].buyPrice[0], &pKlineEx[i].buyPrice[0],sizeof(KlineEx));
			}
            
			m_pDoc->m_pStockBigBillData->CalcBigBillHistoryData(Cdat,pBigBillData, nTotalCount) ;
            GlobalUnlock((HGLOBAL)pBigBillData);
            GlobalFree((HGLOBAL)pBigBillData);
			delete [] pKlineEx;
            CTaiKlineFileHS::m_fileHsSh->RemoveHs(pBuySellList);
//*************************************************************************
			index++;
            pProgressCtrl->SetPos(index); 
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

//*************************************************************************
		}
	}    m_pButton->EnableWindow(TRUE) ;
	dlg->m_DialogDown =TRUE;
	dlg->ShowWindow(SW_HIDE);
	dlg->OnClose() ;

}
void CSharesBigBillDlg::GetBigBillDataHistory()
{
	MSG message;
	int index=0;

	CTaiKlineFileHS *m_fileHsSh=NULL;
	CTaiKlineFileHS *m_fileHsSz=NULL;
	CTaiKlineFileHS *pTempFileHS;
	long lNowTime;
    CTime m_NowTime = CTime::GetCurrentTime();
	CTimeSpan  lDaySpan(1, 0,0, 0 );

    
    lNowTime=m_pDoc->GetStockDay(m_NowTime.GetTime());
    time_t tTemp=m_Start.GetTime( );
    time_t tTempEnd=m_End.GetTime( );
	if(tTemp<=0||tTempEnd<=0)
	{
       MessageBox("请输入正确时间!","操作提示",MB_OK);
	   return;
	}
 	CProgressDialog *dlg;
	dlg=new CProgressDialog;
	dlg->Create(IDD_DIALOG_PROGRESS,NULL);
    dlg->ShowWindow(SW_SHOW); 
	dlg->m_DialogDown =FALSE;
	CWnd *pWnd;
	pWnd=dlg->GetDlgItem(IDC_ZQMC) ;
	if(pWnd)pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_ZQDM) ;
	if(pWnd)pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_DISPLAY_INFO) ;
	if(pWnd)pWnd->ShowWindow(SW_HIDE);
	pWnd=dlg->GetDlgItem(IDC_COMP) ;
	if(pWnd)pWnd->ShowWindow(SW_HIDE);
	CButton *m_pButton;
	m_pButton =(CButton *)dlg->GetDlgItem(IDOK); 
    m_pButton->EnableWindow(FALSE) ;
	dlg->SetWindowText(_T("历史明细数据提取"));
    CProgressCtrl *pProgressCtrl;
	pProgressCtrl =(CProgressCtrl *)dlg->GetDlgItem(IDC_PROGRESS); 
    pProgressCtrl->SetRange(0,m_pDoc->m_sharesInformation.GetCount());
	while(tTemp<=m_End.GetTime( ))
	{ 
		CTime ctTime(tTemp);
		if(m_pDoc->GetStockDay(tTemp)==lNowTime)
		{
           if(ctTime.GetDayOfWeek()!=1&&ctTime.GetDayOfWeek()!=7)
             GetBigBillDataToday();
           tTemp +=lDaySpan.GetTotalSeconds();     
		   continue;
		}
		char szDate[10];
		sprintf(szDate,"%d%2d%2d",ctTime.GetYear(),ctTime.GetMonth(),ctTime.GetDay());
		for(int i=0;i<8;i++)
		{
			if(szDate[i]==' ')
			   szDate[i]='0';
		}
		szDate[8]='\0';	
        tTemp +=lDaySpan.GetTotalSeconds();     
    
		CString strTime=szDate;
		m_fileHsSh=CTaiKlineFileHS::BillOpenFileHs(strTime,TRUE,FALSE);
		m_fileHsSz=CTaiKlineFileHS::BillOpenFileHs(strTime,FALSE,FALSE);
		if(m_fileHsSh==NULL||m_fileHsSz==NULL)
			continue;
        pProgressCtrl->SetPos(0);
		index=0;
		for(int i=0;i<10;i++)
		{
			if(i==0||i==3||i==8)
				continue;
			int l_nStockCount=m_pDoc->m_sharesInformation.GetStockTypeCount(i);
			for(int j=0;j<l_nStockCount;j++)
			{
				CReportData *Cdat;
				m_pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
				if(Cdat==NULL)
				   continue;
				if(!Cdat->IsMxTj)
					continue;
				CBuySellList pBuySellList;
				KlineEx *pKlineEx=NULL;
				TRADE_DETAIL_BIG_LIST_PER *pBigBillData;
				int nTotalCount=0;
				if(Cdat->kind==SHAG||Cdat->kind==SHBG||Cdat->kind==SHZQ)
				{
					m_fileHsSh->BillTransferDataEx(m_fileHsSh, Cdat->id, &pBuySellList, pKlineEx);
					pTempFileHS=m_fileHsSh;
				}
				else if(Cdat->kind==SZAG||Cdat->kind==SZBG||Cdat->kind==SZZQ)
				{
					m_fileHsSz->BillTransferDataEx(m_fileHsSz, Cdat->id, &pBuySellList, pKlineEx);
					pTempFileHS=m_fileHsSz;
				}
				nTotalCount=pBuySellList.GetCount();
				HGLOBAL hMem=GlobalAlloc(  GPTR, sizeof(TRADE_DETAIL_BIG_LIST_PER)*nTotalCount);
				if(hMem!=NULL)
				   pBigBillData=(TRADE_DETAIL_BIG_LIST_PER *)GlobalLock(  hMem );
				else
				   continue; 
				for(int i=0;i<nTotalCount;i++)
				{
					  TRADE_DETAIL_H_PER *pBuySellData;
					  pBuySellData=pTempFileHS->GetAtBS(&pBuySellList, i,nTotalCount);
					  memcpy(&pBigBillData[i].vol, &pBuySellData->vol,12);
					  memcpy(&pBigBillData[i].buyPrice[0], &pKlineEx[i].buyPrice[0],sizeof(KlineEx));
				      int tempp=m_pDoc->GetStockDay(pBigBillData[i].time);
				}
            
				m_pDoc->m_pStockBigBillData->CalcBigBillHistoryData(Cdat,pBigBillData, nTotalCount) ;
				GlobalUnlock((HGLOBAL)pBigBillData);
				GlobalFree((HGLOBAL)pBigBillData);
				delete [] pKlineEx;
				pTempFileHS->RemoveHs(pBuySellList);
//********************************************************************
			index++;
            pProgressCtrl->SetPos(index); 
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
//********************************************************************

			}
		}
		m_fileHsSh->Close();
		delete m_fileHsSh;
		m_fileHsSz->Close();
		delete m_fileHsSz;
	}
    m_pButton->EnableWindow(TRUE) ;
	dlg->m_DialogDown =TRUE;
	dlg->ShowWindow(SW_HIDE);
	dlg->OnClose() ;
}
LRESULT CSharesBigBillDlg::OnGridColorChange(WPARAM wParam, LPARAM lParam)
{
	m_pGrid->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);
    m_pGrid->SetFixedBkColor(m_pDoc->m_colorArray[18]);
    m_pGrid->Invalidate();                           

	return 0;
}

BOOL CSharesBigBillDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
		DoHtmlHelp(this);return TRUE;


}
