// RiskIndex.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "RiskIndex.h"
#include "SelectStock.h"
#include "mainfrm.h"
#include "CTaiKlineFileKLine.h"
#include "transactFee.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiskIndex dialog


CRiskIndex::CRiskIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CRiskIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRiskIndex)
	m_StartDate = CTime::GetCurrentTime();
	m_EndDate   = CTime::GetCurrentTime();
	//}}AFX_DATA_INIT
	m_StartDate-=5*24*3600;
}

CRiskIndex::~CRiskIndex()
{
	delete m_Gif;
}

void CRiskIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRiskIndex)
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_EndDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRiskIndex, CDialog)
	//{{AFX_MSG_MAP(CRiskIndex)
	ON_BN_CLICKED(IDC_CALC, OnCalc)
	ON_BN_CLICKED(IDC_ADDSTOCK, OnAddstock)
	ON_BN_CLICKED(IDC_SELDEFINE, OnSeldefine)
	ON_NOTIFY(NM_CLICK, IDC_TAB1, OnClickTab1)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiskIndex message handlers

///////////////////////////////////////////////////////////////

void CRiskIndex::GetCovMat(double *pPriceMat,int nDay,int nStock,double * &pCovMat)
{
	int i,j,k;


	pCovMat=new double[nStock*nStock];
	memset(pCovMat,00,sizeof(double)*nStock*nStock);
	
	double * tmpMat=new double[nStock*nStock];
	memset(tmpMat,00,sizeof(double)*nStock*nStock);

	double *meanX=new double[nStock];
	memset(meanX,00,sizeof(double)*nStock);


	for( i=0;i<nStock;i++)
	{
		for( k=0;k<nDay;k++)
			*(meanX+i) +=  *(pPriceMat + k*nStock+i);
		*(meanX+i) /= nDay;
	}

	double * tmpVec=new double[nStock];

	for( k=0;k<nDay;k++)
	{
		for(i=0;i<nStock;i++)
			*(tmpVec+i)=*(pPriceMat+k*nStock+i) - *(meanX+i);

		
		memset(tmpMat,00,sizeof(double)*nStock*nStock);

		for( i=0;i<nStock;i++)
			for( j=0;j<nStock;j++)
				*(tmpMat+i*nStock+j) += *(tmpVec+i) * (*(tmpVec+j));	//tmpMat为(X(t)-meanX)*(X(t)-meanX)'

		for( i=0;i<nStock;i++)
			for( j=0;j<nStock;j++)
			*(pCovMat+i*nStock+j)+= *(tmpMat+i*nStock+j)/(nDay-1);
	}
	
	delete []tmpVec;
	delete []tmpMat;
	delete []meanX; 
}

//////////////////////////////////////////////////////////////////////////////

double CRiskIndex::GetMaxEigVal(double *pCovMat,int nDay, int nStock)
{
	double *pMaxEigVec=new double[nStock];
	
	double *x=new double[nStock],epsi=0.0005,temp=2.0,max=1.0;
	memset( x,00,sizeof(double)*nStock);

	int i;
	for(i=0;i<nStock;i++)
		*(pMaxEigVec+i)=1.0;

	double MaxEigVal=1.0;

	while( fabs(temp-MaxEigVal)>epsi )
	{
	
		for( int i=0;i<nStock;i++)
		{
			*(x+i)=0.0;
			for( int j=0;j<nStock;j++)
				*(x+i)+=*(pCovMat+nStock*i+j)     *     *(pMaxEigVec+j);
		}
		
		max=*x;
		for( i=1;i<nStock;i++)
			max=fabs(max)<fabs(*(x+i))? *(x+i):max;

		for(i=0;i<nStock;i++)
			*(pMaxEigVec+i)=*(x+i)/max;

		temp=MaxEigVal;
		MaxEigVal=max;
	}


	double dSumAbsEigVec=0.0;
	for(  i=0;i<m_StockRiskArray.GetSize();i++)
		dSumAbsEigVec+=fabs( *(pMaxEigVec+i) );

	for(  i=0;i<m_StockRiskArray.GetSize();i++)
		m_StockRiskArray[i].Weight=*(pMaxEigVec+i)/dSumAbsEigVec;

	delete []pMaxEigVec;
	delete []x;
	return MaxEigVal;

}

/////////////////////////////////////////////////////////////////////

double CRiskIndex::GetMarketPortfolioStd(double *pPriceMat, double *pOutstandingMat, int nDay, int nStock)
{
	double *pBetaVec=new double[nStock];
	memset(pBetaVec,00,sizeof(double)*nStock);

	double *MarketPortfolio=new double[nDay];	
	memset(MarketPortfolio,00,sizeof(double)*nDay);


	for( int i=0;i<nDay;i++)
	{
		double mod=0.0;
		for( int j=0;j<nStock;j++)
		{
			*(MarketPortfolio+i)+=*(pPriceMat+i*nStock+j)*(*(pOutstandingMat+i*nStock+j));
			mod+=(*(pOutstandingMat+i*nStock+j)) * (*(pOutstandingMat+i*nStock+j));
		}
		*(MarketPortfolio+i)=*(MarketPortfolio+i)/sqrt(mod);
	}

	double MeanMarket=0.0, MarketStd=0.0;
	for( i=0;i<nDay;i++)
		MeanMarket+=*(MarketPortfolio+i)/nDay;

	for( i=0;i<nDay;i++)
		MarketStd+= (*(MarketPortfolio+i)-MeanMarket)*(*(MarketPortfolio+i)-MeanMarket)/(nDay-1);



	for(int j=0;j<nStock;j++)
	{
		double MeanStock=0.0;
		for( i=0;i<nDay;i++)
			MeanStock+=(*(pPriceMat+i*nStock+j))/nDay;
		for( i=0;i<nDay;i++)
			*(pBetaVec+j)+=( *(pPriceMat+i*nStock+j) - MeanStock )* (*(MarketPortfolio+i)-MeanMarket)/(nDay-1);
		*(pBetaVec+j)=*(pBetaVec+j)/MarketStd;

	}

	for(  i=0;i<m_StockRiskArray.GetSize();i++)
		m_StockRiskArray[i].Beta=*(pBetaVec+i);

	delete []pBetaVec;
	delete []MarketPortfolio;
	return sqrt(MarketStd);
}

///////////////////////////////////////////////////////////////////

BOOL CRiskIndex::PreProcessData(double *pPriceMat,double *pOutstandingMat,int nStock, int nDay,int nCriterion)
{

	double * AssistVector=new double[nStock];
	memset(AssistVector,00,sizeof(double)*nStock);

	double *mean=new double[nStock];
	memset(mean,00,sizeof(double)*nStock);

	for(int j=0;j<nStock;j++)
		for(int i=0;i<nDay;i++)
			*(AssistVector+j)=*(pPriceMat+i*nStock+j)==0 ? *(AssistVector+j)+1 : *(AssistVector+j);

	for(int i=0;i<nStock;i++)
	{
		if(*(AssistVector+i)>nCriterion)
		{
			for(int j=0;j<nDay;j++)
				*(pPriceMat+j*nStock+i)=0.0;
		}
		else if(*(AssistVector+i)>0)
		{
			for(j=0;j<nDay;j++)
			if(*(pPriceMat+j*nStock+i)==0.0)
			{
				if(j==0 )
				{
					int k=0;
					while(  k< nDay && *(pPriceMat+k*nStock+i) == 0.0    )
						k++;						
					*(pPriceMat+j*nStock+i)=*(pPriceMat+k*nStock+i);
				}
				else
					*(pPriceMat+j*nStock+i)=*(pPriceMat+(j-1)*nStock+i);
			}
		}
	}


	delete []mean;
	delete []AssistVector;

	return 1;
}

////////////////////////////////////////////////////////////////

int CRiskIndex::GetPriceMatrix( CTime& nStart,CTime& nEnd,double* &pPriceMat)
{
	int nCount=m_StockRiskArray.GetSize();
	SymbolKindArr StockSymbolArray;
	StockSymbolArray.SetSize( nCount );
	for(int i=0; i<nCount; i++ )
	{
		strcpy(StockSymbolArray[i].m_chSymbol,m_StockRiskArray[i].chSymbol);
	

	}

	int nDay=0;
	Kline* pKlineIndex=NULL;
	CTaiKlineFileKLine::OutputCloseVal(m_StockChoose, nDay, nStart,nEnd,pPriceMat,pKlineIndex);

	

	return nDay;
}

/////////////////////////////////////////////////

int CRiskIndex::GetOutstandingMatrix(int nDay,double* &pOutstandingMat)
{
	int nStock=m_StockRiskArray.GetSize();
	pOutstandingMat=new double[nStock*nDay];

	CStringArray StockSymbolArray;
	StockSymbolArray.SetSize( nStock );
	for(int  i=0;i<nStock;i++)
		StockSymbolArray[i]=m_StockRiskArray[i].chSymbol;


	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CReportData *Cdat;

	for( i=0;i<nStock;i++)
		pOutstandingMat[i]= pDoc->m_sharesInformation.Lookup(this->m_StockChoose[i].m_chSymbol, Cdat,m_StockChoose[i].m_nSymbolKind) == TRUE && Cdat->pBaseInfo!=NULL ? (Cdat->pBaseInfo)->ltAg : 0.0;

	for( int nRow=1; nRow<nDay;nRow++ )
		memcpy(pOutstandingMat+nRow*nStock, pOutstandingMat, sizeof(double)*nStock );
		
	return nStock;

}

void CRiskIndex::OnCalc() 
{


	UpdateData(TRUE);
	if( ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() )
		m_dRiskIndex=this->GetRiskIndex(m_StartDate, m_EndDate);
	else 
	{
		CTime CurrentTime=CTime::GetCurrentTime();
		CTime nStart,nEnd;
		int nOffset=CurrentTime.GetDayOfWeek()-1;

		CString szWeek;
		GetDlgItem(IDC_WEEKS)->GetWindowText(szWeek);
		int nWeek=atoi( szWeek );
		if( nWeek <=0  )
		{
			AfxMessageBox("至少为1");	
			((CEdit*)GetDlgItem(IDC_WEEKS))->SetFocus();
			return;
		}
		else if( nWeek == 1 )
		{
			nStart=CurrentTime.GetTime()-(nOffset-1)*24*3600;
			nEnd=CurrentTime;
		}
		else
		{
			nStart=CurrentTime.GetTime()-( 7*(nWeek-1)+nOffset-1)*24*3600;
			nEnd  =nStart + 4*24*3600;
		}
		m_dRiskIndex=this->GetRiskIndex(nStart, nEnd);
	}
	if( m_dRiskIndex >=0 )
		OutputToList(m_dRiskIndex);

}


void CRiskIndex::OnAddstock() 
{
	CListCtrl* pListCtrl=(CListCtrl*)GetDlgItem(IDC_SYMBOL_LIST);
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 

	UpdateData();
	SelectStock pDlg(this,FALSE,FALSE);
	pDlg.m_pSelectResultList  = pListCtrl;
	pDlg.DoModal();

	SelectStock::GetStockSymbolArray(pListCtrl,m_StockChoose);
	UpdateData(FALSE);


	CReportData *Cdat=NULL;
	StockRisk StkRsk;
	char ch[8];
	int nCount=m_StockChoose.GetSize();
	for(int i=0;i<nCount;i++)
	{
		strcpy(ch,m_StockChoose[i].m_chSymbol);

		if( pDoc->m_sharesInformation.Lookup(ch, Cdat,m_StockChoose[i].m_nSymbolKind) )
		{
			StkRsk.Beta=0.0;
			strcpy(StkRsk.chName,Cdat->name);
			strcpy(StkRsk.chSymbol,ch);
			StkRsk.Weight=0.0;
			m_StockRiskArray.Add(StkRsk);
		}
	}

	//
	pListCtrl->DeleteAllItems();	
	for( i=0;i<m_StockRiskArray.GetSize();i++)
	{
		pListCtrl->InsertItem( i, m_StockRiskArray[i].chName);
		pListCtrl->SetItemText(i,1,m_StockRiskArray[i].chSymbol);
	}

	m_TabCtrl.SetCurSel(2);


}

BOOL CRiskIndex::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

    m_Gif=new CPlayGif(this->m_hWnd);
	m_Gif->Load("pingce.gif");
	
	m_Gif->Play();
	m_Gif->Pause(FALSE);


	TC_ITEM tci;
	CString str[]={"风险指数","风险方向","所选股票"};
	tci.mask=TCIF_TEXT;
	for( int i=0;i<3;i++)
	{
		tci.pszText=(LPSTR)(LPCTSTR)str[i];
		m_TabCtrl.InsertItem(i,&tci);
	}
	
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	GetDlgItem(IDC_WEEKS)->SetWindowText("1");
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);


	CListCtrl* pListCtrl;
	CRect rect;

	pListCtrl=(CListCtrl*)GetDlgItem(IDC_EIGVEC_LIST);
	ListView_SetExtendedListViewStyle(  pListCtrl->m_hWnd, LVS_EX_FULLROWSELECT);
	pListCtrl->GetClientRect(&rect);
	pListCtrl->InsertColumn(0,"名称",LVCFMT_LEFT,rect.Width()/4);
	pListCtrl->InsertColumn(1,"权重",LVCFMT_LEFT,rect.Width()/4);
	pListCtrl->InsertColumn(2,"代码",LVCFMT_CENTER,rect.Width()/4);
	pListCtrl->InsertColumn(3,"Beta",LVCFMT_LEFT,rect.Width()/4);

	pListCtrl=(CListCtrl*)GetDlgItem(IDC_SYMBOL_LIST);
	ListView_SetExtendedListViewStyle(  pListCtrl->m_hWnd, LVS_EX_FULLROWSELECT);
	pListCtrl->GetClientRect(&rect);
	pListCtrl->InsertColumn(0,"名称",LVCFMT_LEFT,rect.Width()/2);
	pListCtrl->InsertColumn(1,"代码",LVCFMT_LEFT,rect.Width()/2);

	pListCtrl=(CListCtrl*)GetDlgItem(IDC_RISK_LIST);
	ListView_SetExtendedListViewStyle(  pListCtrl->m_hWnd, LVS_EX_FULLROWSELECT);
	pListCtrl->GetClientRect(&rect);
	pListCtrl->InsertColumn(0,"序号",LVCFMT_LEFT,rect.Width()/2);
	pListCtrl->InsertColumn(1,"风险指数",LVCFMT_LEFT,rect.Width()/2);
	
	return TRUE; 
}

void CRiskIndex::QuickSort(CArray<StockRisk,StockRisk&>& StockRiskArray,	int low, int high,bool bAscend/*from small to big*/)
{
	StockRisk pivot;
	int scanUp, scanDown;
	int mid;
	if( high-low <= 0 )
		return;
	else if(high-low==1)
	{
		if( bAscend )
		{
			if( StockRiskArray[high].Weight<StockRiskArray[low].Weight )
			{
				StockRisk tmp=StockRiskArray[low];
				StockRiskArray[low]=StockRiskArray[high];
				StockRiskArray[high]=tmp;
			}
		}
		else
		{
			if( StockRiskArray[high].Weight>StockRiskArray[low].Weight )
			{
				StockRisk tmp=StockRiskArray[low];
				StockRiskArray[low]=StockRiskArray[high];
				StockRiskArray[high]=tmp;
			}
		}
		return;

	}
	mid=(low+high)/2;
	pivot=StockRiskArray[mid];
	StockRisk tmp=StockRiskArray[low];
	StockRiskArray[low]=StockRiskArray[mid];
	StockRiskArray[mid]=tmp;

	scanUp=low+1;
	scanDown=high;
	do
	{
		if( bAscend )
		{
			while( scanUp<=scanDown && StockRiskArray[scanUp].Weight<=pivot.Weight)
				scanUp++;
			while( pivot.Weight< StockRiskArray[scanDown].Weight )
				scanDown--;
		}
		else
		{
			while( scanUp<=scanDown && StockRiskArray[scanUp].Weight>=pivot.Weight)
				scanUp++;
			while( pivot.Weight> StockRiskArray[scanDown].Weight )
				scanDown--;
		}

		if( scanUp <scanDown )
		{
			StockRisk tmp=StockRiskArray[scanUp];
			StockRiskArray[scanUp]=StockRiskArray[scanDown];
			StockRiskArray[scanDown]=tmp;
		}
	}while(scanUp<scanDown);

	StockRiskArray[low]=StockRiskArray[scanDown];
	StockRiskArray[scanDown]=pivot;

	if( low<scanDown-1)
		QuickSort(StockRiskArray,low,scanDown-1,  bAscend);
	if( scanDown+1<high)
		QuickSort(StockRiskArray,scanDown+1,high, bAscend);

}
BOOL CRiskIndex::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{


	static bool bAscend=false;
	LV_DISPINFO* lv_dispinfo=(LV_DISPINFO*)lParam;

	if(lv_dispinfo->hdr.code==LVN_COLUMNCLICK && lv_dispinfo->item.iItem==1 && m_TabCtrl.GetCurSel() == 1 )
	{
		if( bAscend )
		{
			QuickSort(	m_StockRiskArray,0,  m_StockRiskArray.GetSize()-1,bAscend);
			OutputToList(m_dRiskIndex);
			bAscend=!bAscend;
		}
		else
		{
			QuickSort( m_StockRiskArray,0,m_StockRiskArray.GetSize()-1,bAscend );
			OutputToList(m_dRiskIndex);
			bAscend=!bAscend;
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}


void CRiskIndex::OutputToList(double dRiskIndex)
{
	char ch[20];
	CListCtrl* pListCtrl;

	///
	pListCtrl=(CListCtrl*)GetDlgItem(IDC_RISK_LIST);
	int nCount=pListCtrl->GetItemCount();
	sprintf(ch,"%d",  nCount+1 );
	pListCtrl->InsertItem( nCount, ch);
	sprintf(ch,"%+.3f",dRiskIndex);
	pListCtrl->SetItemText(nCount,1, ch );   

	///
	pListCtrl=(CListCtrl*)GetDlgItem(IDC_EIGVEC_LIST);
	pListCtrl->DeleteAllItems();	

	for( int i=0;i<m_StockRiskArray.GetSize();i++)
	{
		pListCtrl->InsertItem( i, m_StockRiskArray[i].chName);
		
		sprintf(ch,"%+.3f",m_StockRiskArray[i].Weight);
		pListCtrl->SetItemText(i,1, ch );   
		
		pListCtrl->SetItemText(i,2,m_StockRiskArray[i].chSymbol);

		sprintf(ch,"%+.3f",m_StockRiskArray[i].Beta);
		pListCtrl->SetItemText(i,3, ch );     
	}

}

void CRiskIndex::OnSeldefine() 
{

	this->OnAddstock();
	
}

void CRiskIndex::OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nCurTab=this->m_TabCtrl.GetCurSel();

	switch(nCurTab)
	{
	case 0:
		GetDlgItem(IDC_RISK_LIST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EIGVEC_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYMBOL_LIST)->ShowWindow(SW_HIDE);
		break;
	case 1:
		GetDlgItem(IDC_RISK_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EIGVEC_LIST)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYMBOL_LIST)->ShowWindow(SW_HIDE);
		break;
	case 2:
		GetDlgItem(IDC_RISK_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EIGVEC_LIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYMBOL_LIST)->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}

void CRiskIndex::OnSelchangeCombo1() 
{

	switch(  ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel( ) )
	{
	case 0:
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_INTERVAL)->ShowWindow(SW_HIDE);
		break;
	case 1:
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERVAL)->ShowWindow(SW_SHOW);
		break;
	}
	
}

void CRiskIndex::OnRadio1() 
{
	
	GetDlgItem(IDC_DATETIMEPICKER1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DATETIMEPICKER2)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_WEEKS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SPIN1)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);

}

void CRiskIndex::OnRadio2() 
{
	

	GetDlgItem(IDC_DATETIMEPICKER1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DATETIMEPICKER2)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_WEEKS)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SPIN1)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);


}

double CRiskIndex::GetRiskIndex(CTime& nStart, CTime& nEnd)
{
	double *pPriceMat=NULL, *pOutstandingMat=NULL,	*pCovMat=NULL,	*pMaxEigVec=NULL,	*pBetaVec=NULL;
	int nDay=GetPriceMatrix( nStart,nEnd,pPriceMat);
	if( nDay<1 )	
	{
		AfxMessageBox("所选股票数据严重缺失");
		return -1.0;
	}

	int nStock=GetOutstandingMatrix(nDay,pOutstandingMat);
	if( nStock <1 )	
	{
		AfxMessageBox("没有选择股票");
		return -1.0;
	}

	PreProcessData(pPriceMat,pOutstandingMat,nStock,nDay,/*nCriterion*/nDay/2);
	GetCovMat(pPriceMat,nDay,nStock,pCovMat);
	double dMaxEigVal=GetMaxEigVal(pCovMat,nDay,nStock);
	double dMarketStd=GetMarketPortfolioStd(pPriceMat,pOutstandingMat,nDay,nStock);
 
	delete []pPriceMat;
	delete []pOutstandingMat;
	delete []pCovMat;

	return dMarketStd / sqrt(dMaxEigVal);	

}
