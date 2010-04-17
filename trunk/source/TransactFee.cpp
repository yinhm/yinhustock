// TransactFee.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "TransactFee.h"
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransactFee dialog

CTransactFee::CTransactFee(CWnd* pParent /*=NULL*/)
	: CDialog(CTransactFee::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransactFee)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CFile file( g_rate,CFile::modeRead);
	file.Read(m_ShRate,sizeof(m_ShRate));
	file.Read(m_SzRate,sizeof(m_SzRate));
	file.Close();
}


void CTransactFee::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransactFee)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT_FEE, m_fRate);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_MsFlexGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransactFee, CDialog)
	//{{AFX_MSG_MAP(CTransactFee)
	ON_EN_KILLFOCUS(IDC_EDIT_FEE, OnKillfocusEditFee)
	ON_NOTIFY(NM_CLICK, IDC_TAB1, OnClickTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CTransactFee::OnInitDialog() //市场
{
	CDialog::OnInitDialog();
	
	
	TC_ITEM tci;
	CString str[]={"上海","深圳","创业"};
	tci.mask=TCIF_TEXT;
	for(int i=0;i<3;i++)
	{
		tci.pszText=(LPSTR)(LPCTSTR)str[i];
		tci.cchTextMax=str[i].GetLength();
		this->m_TabCtrl.InsertItem(i,&tci);
	}

	char *chRowTitle[5]={"A股","B股","债券","基金","投资基金"};
	char *chColTitle[5]={"印花税","佣金","最低佣金","过户费","最低过户费"};
	for(int i=0;i<5;i++)
	{
		m_MsFlexGrid.SetTextArray((i+1)*6,chRowTitle[i]);
		m_MsFlexGrid.SetTextArray(i+1,chColTitle[i]);
	}

	char ch[10];
	for(int i=0;i<5;i++)
	for(int j=0;j<5;j++)
	{
		sprintf(ch,"%.4f",m_ShRate[i][j]);
		m_MsFlexGrid.SetTextMatrix(i+1,j+1,ch);
	}

	GetDlgItem(IDC_EDIT_FEE)->ShowWindow(SW_HIDE);

	return TRUE; 
}

BEGIN_EVENTSINK_MAP(CTransactFee, CDialog)
    //{{AFX_EVENTSINK_MAP(CTransactFee)
	ON_EVENT(CTransactFee, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTransactFee::OnDblClickMsflexgrid() 
{// NOTE: the ClassWizard will add member functions here
	if( m_TabCtrl.GetCurSel()== 2 )
		return;

	GetDlgItem(IDC_EDIT_FEE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_ITEM)->ShowWindow(SW_SHOW);

	char *chRowTitle[5]={"A股","B股","债券","基金","投资基金"};
	char *chColTitle[5]={"印花税","佣金","最低佣金","过户费","最低过户费"};

	this->m_uRow=m_MsFlexGrid.GetRow()-1;
	this->m_uCol=m_MsFlexGrid.GetCol()-1;
	CString str1=chRowTitle[m_uRow];
	CString str2=chColTitle[m_uCol];
	GetDlgItem(IDC_STATIC_ITEM)->SetWindowText(str1+" 的 "+str2);
	GetDlgItem(IDC_EDIT_FEE)->SetWindowText( m_MsFlexGrid.GetText() );

	GetDlgItem(IDC_EDIT_FEE)->SetFocus();


}



void CTransactFee::OnKillfocusEditFee() 
{// NOTE: the ClassWizard will add member functions here

	CString str;
	GetDlgItem(IDC_EDIT_FEE)->GetWindowText(str);
	m_MsFlexGrid.SetText(str);	

	if( m_TabCtrl.GetCurSel()==0 )
		m_ShRate[m_uRow][m_uCol]=atof(str);
	else
		m_SzRate[m_uRow][m_uCol]=atof(str);

	GetDlgItem(IDC_EDIT_FEE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ITEM)->ShowWindow(SW_HIDE);
}

void CTransactFee::OnOK() 
{

	if( GetFileAttributes(g_rate)==FILE_ATTRIBUTE_READONLY )
	{
		SetFileAttributes(g_rate,FILE_ATTRIBUTE_NORMAL);
 		CFile file(g_rate,CFile::modeWrite|CFile::modeCreate);
		file.Write(m_ShRate,sizeof(m_ShRate));
		file.Write(m_SzRate,sizeof(m_SzRate));
		file.Close();
		SetFileAttributes(g_rate,FILE_ATTRIBUTE_READONLY);
	}
	else
	{
 		CFile file(g_rate,CFile::modeWrite|CFile::modeCreate);
		file.Write(m_ShRate,sizeof(m_ShRate));
		file.Write(m_SzRate,sizeof(m_SzRate));
		file.Close();
	}
	
	CDialog::OnOK();
}

void CTransactFee::OnClickTab(NMHDR* pNMHDR, LRESULT* pResult) 
{// NOTE: the ClassWizard will add member functions here
	char ch[10];
	int i=0;

	int nCurTab=this->m_TabCtrl.GetCurSel();
	switch(nCurTab)
	{
	case 0:
	for( i=0;i<5;i++)
		for( i=0;i<5;i++)
		for(int j=0;j<5;j++)
		{
			sprintf(ch,"%.4f",m_ShRate[i][j]);
			m_MsFlexGrid.SetTextMatrix(i+1,j+1,ch);
		}
		break;
	case 1:
		for( i=0;i<5;i++)
		for(int j=0;j<5;j++)
		{
			sprintf(ch,"%.4f",m_SzRate[i][j]);
			m_MsFlexGrid.SetTextMatrix(i+1,j+1,ch);
		}
		break;
	case 2:
		for( i=0;i<5;i++)
		for(int j=0;j<5;j++)
		{
			
			m_MsFlexGrid.SetTextMatrix(i+1,j+1,"未知");
		}
		break;
	}
	
	*pResult = 0;
}

float CTransactFee::GetCommission(SymbolKind StockSymbol)
{// NOTE: the ClassWizard will add member functions here
	enum StockType{AShare,BShare,Bond,Fund,InvestFund};
	enum RateType{Tax,Comm,MiniComm,Fee,MiniFee};

	CReportData* pDat;
	double dCommission=0.0;
	
	if ( (CMainFrame::m_taiShanDoc)->m_sharesInformation.Lookup(StockSymbol.m_chSymbol, pDat,StockSymbol.m_nSymbolKind ) )     //检测该股票是否已记录在内存模板中
	{
		switch(pDat->kind) 
		{
		case 0:
			
			break;
		case 1:
			if( StockSymbol.m_chSymbol[0]=='5')
				dCommission+=m_ShRate[Fund][Tax]+m_ShRate[Fund][Comm]+m_ShRate[Fund][Fee];
			else
				dCommission+=m_ShRate[AShare][Tax]+m_ShRate[AShare][Comm]+m_ShRate[AShare][Fee];
			break;
		case 2:
			dCommission+=m_ShRate[BShare][Tax]+m_ShRate[BShare][Comm]+m_ShRate[BShare][Fee];
			break;
		case 3:
			break;
		case 4:
			dCommission+=m_SzRate[AShare][Tax]+m_SzRate[AShare][Comm]+m_SzRate[AShare][Fee];
			break;
		case 5:
			dCommission+=m_SzRate[BShare][Tax]+m_SzRate[BShare][Comm]+m_SzRate[BShare][Fee];
			break;
		case 6:
			dCommission+=m_ShRate[Bond][Tax]+m_ShRate[Bond][Comm]+m_ShRate[Bond][Fee];
			break;
		case 7:
			dCommission+=m_SzRate[Bond][Tax]+m_SzRate[Bond][Comm]+m_SzRate[Bond][Fee];
			break;
		default:
			dCommission=0.0075;
			break;
		}
	}
	return float(dCommission);
}
