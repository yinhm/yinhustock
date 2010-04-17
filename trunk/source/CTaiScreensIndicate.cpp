// CTaiScreensIndicate.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiScreensIndicate.h"
#include "CTaiKlineDlgStopLose.h"
#include "MainFrm.h"
#include "StructKlineView.h"
#include "CTaiShanDoc.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTaiScreensIndicate::CTaiScreensIndicate(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CTaiScreensIndicate::IDD, pParent)
{
	pView = (CTaiShanKlineShowView*)pParent;
	ASSERT(pView!=NULL);
	m_pExplainButton = &m_buttonExpl;
	m_bDlgIndicator = true;

	m_bUseing = false;
	m_bShowBuyCase = true;
	//{{AFX_DATA_INIT(CTaiScreensIndicate)
	m_mbly = 0.0f;
	m_mbzq = 0;
	m_bBuy = 0;
	m_CheckSelect = FALSE;
	//}}AFX_DATA_INIT
}


void CTaiScreensIndicate::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreensIndicate)
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT2, m_floatEdit2);
	DDX_Control(pDX, IDC_EDIT1, m_floatEdit1);
	DDX_Control(pDX, IDC_BUTTON2, m_buttonExpl);
	DDX_Text(pDX, IDC_EDIT2,m_mbly );
	DDX_Text(pDX, IDC_EDIT1,m_mbzq );
	DDV_MinMaxInt(pDX, m_mbzq, 1, 10000000);
	DDX_Check(pDX, IDC_CHECK1, m_CheckSelect);
	DDX_Radio(pDX, IDC_BUY_CASE, m_bBuy);
	DDX_Check(pDX, IDC_CHECK2, m_bCaclYHParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreensIndicate, CTaiScreenParent)
	//{{AFX_MSG_MAP(CTaiScreensIndicate)
	ON_BN_CLICKED(IDC_BUTTON1, OnSetAddCondition)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CTaiScreensIndicate::OnOK() 
{

	 if(!UpdateData(TRUE))
		 return;
	 if(m_indexSaveArray.GetSize()==0)
	 {
		 AfxMessageBox("请先选择指标");
		 return;
	 }
	 if(m_mbly<0)
	 {
		 AfxMessageBox("目标利润不能为负",MB_ICONSTOP);
		 (GetDlgItem(IDC_EDIT1))->SetFocus();
		 return;
	 }
	 if(m_mbzq<=0)
	 {
		 AfxMessageBox("目标周期应为大于0的整数",MB_ICONSTOP);
         (GetDlgItem(IDC_EDIT2))->SetFocus();
		 return;
	 }
	 if(m_CheckSelect)
		 pDoc->m_propertyInitiate.bTjxgZsFs=1;
	 else
		 pDoc->m_propertyInitiate.bTjxgZsFs=0;
	CString FileName;

	FileName="SaveTjxg.dat";
	WriteTizhToFile(FileName,m_and_or,false,m_mbzq,m_mbly);
	Calculate();

	CTaiScreenParent::OnOK();
}

BOOL CTaiScreensIndicate::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();

	this->m_indexSaveArray.RemoveAll();
	
	
	this->m_treeFormularCtrl.InsertEqutionTree (1,true);
	this->m_treeFormularCtrl.InsertEqutionTree (-6);

	this->m_treeFormularCtrl.InsertEqutionTree (0,true);

	this->m_treeFormularCtrl.InsertEqutionTree (2);
	if(m_pIndex)
		m_treeFormularCtrl.SelectItemJ (m_pIndex);
	else
	{
		HTREEITEM hh = m_treeFormularCtrl.GetRootItem( );
		m_treeFormularCtrl.EnsureVisible(hh);
	}
	m_and_or = 0;
	m_mbly=10.0;
	m_mbzq=20;



	for(int i=0;i<8;i++)
      m_sSubFormular.AddString(m_subIndexGroup[i]);
    m_sSubFormular.SetCurSel(0);
	////////////
	OnSelchangeListSubFormular();

	UpdateData(FALSE);
	
	if(pView)
	{
		SymbolKind sm;
		strcpy(sm.m_chSymbol ,pView->m_sharesSymbol);
		sm.m_nSymbolKind = pView->m_stkKind ;
		this->m_stockchoose .Add(sm );
	}
	return TRUE; 
}

void CTaiScreensIndicate::OnSetAddCondition() 
{
	
	CTaiKlineDlgStopLose DlgInsert(this);
	DlgInsert.DoModal();	
}

void CTaiScreensIndicate::OnCancel() 
{

	
	CTaiScreenParent::OnCancel();
}

void CTaiScreensIndicate::OnClose() 
{

	
	CTaiScreenParent::OnClose();
}

void CTaiScreensIndicate::Calculate()
{
	 int index_num=m_indexSaveArray.GetSize();

	 ClearTestResultArray();

	 Kline *pKline = pView->m_pkline;
     CBuySellList& buySellList = pView->m_hs;

	 int *pnNeedCount =new int [index_num];
	 int i;
	 for(i=0;i<index_num;i++)
	 {
		 IndexDataInfo index_save = m_indexSaveArray.GetAt(i);
		 pnNeedCount[i]=index_save.iBeginFoot;
	 }

	 ARRAY_BE linebe;
	 linebe.line = NULL;
	 i=0;

	 {
		bool bSucc;
		int nRead;
		CString StockId;
		StockId=pView->m_sharesSymbol;
		int stkKind = pView->m_stkKind;

		int resultb=0;
	
		IndexDataInfo index_save=m_indexSaveArray.GetAt(index_num-1);
		nRead=pView->m_nCountKline;
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
		int nKlineType = 6;
		for(i=0;i<11;i++)
		{
			if(nLKind[i] == pView->m_nKlineKind2)
			{
				nKlineType = i;
				break;
			}
		}
		
		for (int k=0;k<index_num;k++)
		{
			IndexDataInfo index_save=m_indexSaveArray.GetAt(k);
			index_save.iDataType = nKlineType ;
			int nDays=pnNeedCount[k];
			if(nDays<-1)
				continue;
			if(nRead<nDays)
				   continue;//
			
			float a=FormularComputeShares(StockId,stkKind,nRead,&index_save,bSucc,pKline, &buySellList,true,&linebe);
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
				if(m_and_or)//
					resultBuy[ii]=1;
				if(m_and_or==0)//
					if(resultBuy[ii]==NumBuy)
						resultBuy[ii]=1;
					else
						resultBuy[ii]=0;
			}
			if(resultSell[ii]>0)
			{
				if(m_and_or)//
					resultSell[ii]=1;
				if(m_and_or==0)//
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

		IsSuccess(StockId,stkKind,resultBuy,resultSell,resultb,nRead,pKline,m_mbzq,m_mbly/100,bBuySell);
		delete [] resultBuy;
		delete [] resultSell;
		if(linebe.line!=NULL)
			delete [] linebe.line;

    }
	delete [] pnNeedCount;

	m_bUseing = true;


}

void CTaiScreensIndicate::OnButton2() 
{

	ShowExplain(this->m_pIndex);
}

BOOL CTaiScreensIndicate::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
	

}

void CTaiScreensIndicate::SetIndicator(CFormularContent *pJishu)
{
	m_pIndex = pJishu;
}
