// CTaiKlineDlgAddSelfKline.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgAddSelfKline.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgAddSelfKline dialog


CTaiKlineDlgAddSelfKline::CTaiKlineDlgAddSelfKline(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgAddSelfKline::IDD, pParent)
{
	pView = (CTaiShanKlineShowView* )pParent;
	//{{AFX_DATA_INIT(CTaiKlineDlgAddSelfKline)
	m_bUseNNet = FALSE;
	m_tmBegin = 0;
	m_close = 0.0f;
	m_low = 0.0f;
	m_open = 0.0f;
	m_high = 0.0f;
	m_vol = 0;
	m_amountStr = _T("");
	m_fInnerPartVol = 0.0f;
	m_fOuterPartVol = 0.0f;
	m_nDays = 1;
	//}}AFX_DATA_INIT
}


void CTaiKlineDlgAddSelfKline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgAddSelfKline)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT7, m_floatEdit7);
	DDX_Control(pDX, IDC_EDIT2, m_floatEdit6);
	DDX_Control(pDX, IDC_EDIT1, m_floatEdit9);
	DDX_Control(pDX, IDC_EDIT_VOL_CHG, m_floatEdit8);
	DDX_Control(pDX, IDC_EDIT_OPEN_CHG, m_floatEdit5);
	DDX_Control(pDX, IDC_EDIT_LOW_CHG, m_floatEdit4);
	DDX_Control(pDX, IDC_EDIT_HIGH_CHG, m_floatEdit3);
	DDX_Control(pDX, IDC_EDIT_CLOSE_CHG, m_floatEdit2);
	DDX_Control(pDX, IDC_EDIT_AMOUNT_CHG, m_conAmount);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_conDtTm);
	DDX_Check(pDX, IDC_CHECK1, m_bUseNNet);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tmBegin);
	DDX_Text(pDX, IDC_EDIT_CLOSE_CHG, m_close);
	DDV_MinMaxFloat(pDX, m_close, 0.0, 100000);
	DDX_Text(pDX, IDC_EDIT_LOW_CHG, m_low);
	DDV_MinMaxFloat(pDX, m_low, 0.0, 100000);
	DDX_Text(pDX, IDC_EDIT_OPEN_CHG, m_open);
	DDV_MinMaxFloat(pDX, m_open, 0.0, 100000);
	DDX_Text(pDX, IDC_EDIT_HIGH_CHG, m_high);
	DDV_MinMaxFloat(pDX, m_high, 0.0, 100000);
	DDX_Text(pDX, IDC_EDIT_VOL_CHG, m_vol);
	DDV_MinMaxInt(pDX, m_vol, 0, 1000000000);
	DDX_Text(pDX, IDC_EDIT_AMOUNT_CHG, m_amountStr);
	DDX_Text(pDX, IDC_EDIT1, m_fInnerPartVol);
	DDV_MinMaxFloat(pDX, m_fInnerPartVol, 0.f, 9.9e+013f);
	DDX_Text(pDX, IDC_EDIT7, m_fOuterPartVol);
	DDV_MinMaxFloat(pDX, m_fOuterPartVol, 0.f, 9.9e+013f);
	DDX_Text(pDX, IDC_EDIT2, m_nDays);
	DDV_MinMaxInt(pDX, m_nDays, 1, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgAddSelfKline, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgAddSelfKline)
	ON_BN_CLICKED(IDOK, OnAdd)
	ON_BN_CLICKED(IDC_CHECK1, OnUseNNet)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgAddSelfKline message handlers

void CTaiKlineDlgAddSelfKline::OnAdd() 
{
	// TODO: Add your control notification handler code here
	if(UpdateData() == FALSE)
		return;
	if(m_nDays<=0) return;
	if(m_high<m_close ||m_high<m_low ||m_high<m_open
		||m_low>m_close ||m_low>m_high ||m_low>m_open
		||m_vol<0 ||(float)(atof(m_amountStr)<0) ||
		!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol)&&(m_fOuterPartVol<0 || m_fOuterPartVol>m_vol
		||m_fInnerPartVol<0||m_fInnerPartVol>m_vol))
	{
		AfxMessageBox("K线数据有误！");
		return ;
	}

	Kline kln;
	if(m_bUseNNet == FALSE)
	{
		kln.open = m_open;
		kln.high = m_high;
		kln.low  = m_low;
		kln.close = m_close;
		kln.vol = (float)m_vol;
		kln.amount = (float)(atof(m_amountStr)*10000);;
		if(!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol))
		{
			kln.volPositive = m_fOuterPartVol;
		}
		else
		{
			kln.advance = (WORD)(int)(this->m_fInnerPartVol );
			kln.decline = (WORD)(int)(this->m_fOuterPartVol );
		}

		Kline *pKline = new Kline[m_nDays];
		int nBegin = this->m_tmBegin .GetTime ();
		for(int i=0;i<this->m_nDays;i++)
		{
			pKline[i] = kln;
			pKline[i].day = nBegin+i*24*60*60;
		}

		pView->SetKlineExt(pKline, m_nDays);
		delete [] pKline;
	}
	else
	{
	}


	pView->ShowAll (pView->m_sharesSymbol,false,true);
	
}

void CTaiKlineDlgAddSelfKline::OnUseNNet() 
{

	UpdateData();
	
}


BOOL CTaiKlineDlgAddSelfKline::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	CTime tmB(1988,1,1,1,1,0);
	CTime tmE(2037,1,1,1,1,0);
	m_conDtTm.SetRange( &tmB, &tmE);

	if(pView->m_nCountKlineToday>0)
		LoadData(pView->m_nCountKlineToday -1);

	
	return TRUE;  
}

void CTaiKlineDlgAddSelfKline::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	
	*pResult = 0;
	UpdateData();
}

void CTaiKlineDlgAddSelfKline::LoadData(int foot)
{

	CTaiShanDoc* pDoc=CMainFrame::m_taiShanDoc; 

	m_close=pView->m_pkline[foot].close;
	m_high=pView->m_pkline[foot].high;
	m_low=pView->m_pkline[foot].low;
	m_open=pView->m_pkline[foot].open;

	if(m_high<m_close ||m_high<m_low ||m_high<m_open)
		m_high = max(max(m_close,m_open),m_low);
	if(m_low>m_close ||m_low>m_high ||m_low>m_open)
		m_low = min(min(m_close,m_open),m_high);

	m_vol=(int)pView->m_pkline[foot].vol;
	float m_amount=pView->m_pkline[foot].amount/10000;
	m_amountStr.Format("%.1f",m_amount);

	if(!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol))
	{
		float f = pView->m_pkline[foot].vol
			-pView->m_pkline[foot].volPositive;
		if(pView->m_pkline[foot].volPositive>pView->m_pkline[foot].vol)
			f = -1;
		if(pView->m_pkline[foot].volPositive<0)
			f = -1;
		if(f>=0)
		{
			m_fInnerPartVol = f;
			m_fOuterPartVol = pView->m_pkline[foot].volPositive;
		}
		else
		{
			m_fInnerPartVol = pView->m_pkline[foot].vol/2;
			m_fOuterPartVol = m_fInnerPartVol;
		}
		if(m_fInnerPartVol<0.00001) m_fInnerPartVol = 0;
		if(m_fOuterPartVol<0.00001) m_fOuterPartVol = 0;
	}

	CString s;

	//---
	CTime tmB = CTime::GetCurrentTime();
	CTime tmNow (tmB.GetTime()+48*60*60);
	m_tmBegin = tmNow;
	UpdateData(FALSE);

}

BOOL CTaiKlineDlgAddSelfKline::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	DoHtmlHelp(this);return TRUE;
}
