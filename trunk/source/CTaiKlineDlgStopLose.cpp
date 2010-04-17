// TechDlgStopLose.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgStopLose.h"
//#include "CTaiScreenParent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgStopLose dialog


CTaiKlineDlgStopLose::CTaiKlineDlgStopLose(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgStopLose::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgStopLose)
	m_bLose = FALSE;
	m_bWin = FALSE;
	m_bMaxDays = FALSE;
	m_bUseInitFinace = FALSE;
	m_bUsestractFee = FALSE;
	m_bAddPreProfit = FALSE;
	m_fMaxLose = 0.0f;
	m_fMaxWin = 0.0f;
	m_fInitFinace = 0.0f;
	m_Enterpoint = -1;
	m_fMaxDays = 0;
	//}}AFX_DATA_INIT
	//CTaiScreenParent
	GetInfo = (CTaiScreenParent*)pParent;
	if(GetInfo) GetAdditional(GetInfo->buySellAdditional);

	m_Enterpoint= buySellAdditional.nEnterPoint[0];
	m_bLose= buySellAdditional.bLose;
	m_bWin = buySellAdditional.bWin;
	m_bMaxDays= buySellAdditional.bMaxDays;
	m_bUsestractFee= buySellAdditional.bSubstractFee;
	m_bAddPreProfit= buySellAdditional.bAddPreProfit;
	m_bUseInitFinace= buySellAdditional.bUseInitFinace;
    //if(!m_fMaxLose)
	m_fMaxLose= buySellAdditional.fMaxLose;
	//if(!m_fMaxWin)
	m_fMaxWin= buySellAdditional.fMaxWin;
	//if(!m_fMaxDays)
	m_fMaxDays=int( buySellAdditional.fMaxDays);
	//if(!m_fInitFinace)
	m_fInitFinace= buySellAdditional.fInitFinace;
	m_ColorPoint= buySellAdditional.nColorPoint[0];
  
}


void CTaiKlineDlgStopLose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgStopLose)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_EDIT8, m_fInitF);
	DDX_Control(pDX, IDC_EDIT7, m_fMaxD);
	DDX_Control(pDX, IDC_EDIT2, m_fMaxW);
	DDX_Control(pDX, IDC_EDIT1, m_fMaxL);
	DDX_Control(pDX, IDC_TABLIST, m_nTabCtrl);
	DDX_Check(pDX, IDC_CHECK1, m_bLose);
	DDX_Check(pDX, IDC_CHECK2, m_bWin);
	DDX_Check(pDX, IDC_CHECK5, m_bMaxDays);
	DDX_Check(pDX, IDC_CHECK3, m_bUseInitFinace);
	DDX_Check(pDX, IDC_CHECK6, m_bUsestractFee);
	DDX_Check(pDX, IDC_CHECK7, m_bAddPreProfit);
	DDX_Text(pDX, IDC_EDIT1, m_fMaxLose);
	DDV_MinMaxFloat(pDX, m_fMaxLose, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT2, m_fMaxWin);
	DDV_MinMaxFloat(pDX, m_fMaxWin, 0.f, 10000.f);
	DDX_Text(pDX, IDC_EDIT8, m_fInitFinace);
	DDV_MinMaxFloat(pDX, m_fInitFinace, 1.f, 1.e+009f);
	DDX_CBIndex(pDX, IDC_COMBO1, m_Enterpoint);
	DDX_Text(pDX, IDC_EDIT7, m_fMaxDays);
	DDV_MinMaxInt(pDX, m_fMaxDays, 1, 10000);
	//}}AFX_DATA_MAP
	
	
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgStopLose, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgStopLose)
	ON_BN_CLICKED(IDC_CHECK1, OnLose)
	ON_BN_CLICKED(IDC_CHECK2, OnWin)
	ON_BN_CLICKED(IDC_CHECK5, OnMayDays)
	ON_BN_CLICKED(IDC_CHECK3, OnUseInitFinace)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABLIST, OnSelchangeTablist)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_COLOR, OnSetColor)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgStopLose message handlers

BOOL CTaiKlineDlgStopLose::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	
	

	GetDlgItem(IDC_STARANGE)->GetWindowRect(&m_rectsample);
	ScreenToClient(&m_rectsample);

	if(m_bLose)
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	else
        GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	if(m_bWin)
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	else
        GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	if(m_bMaxDays)
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	else
        GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	if(m_bUseInitFinace)
		GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	else
        GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	m_nTabCtrl.InsertItem(0,"多头买入");
	m_nTabCtrl.InsertItem(1,"多头卖出");
	m_nTabCtrl.SetCurSel(0);
	GetDlgItem(IDC_CHECK7)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDlgStopLose::OnLose() 
{
	
	UpdateData(TRUE);
	if(m_bLose)
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CTaiKlineDlgStopLose::OnWin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_bWin)
		GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CTaiKlineDlgStopLose::OnMayDays() 
{

	UpdateData(TRUE);
	if(m_bMaxDays)
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CTaiKlineDlgStopLose::OnUseInitFinace() 
{

	UpdateData(TRUE);
	if(m_bUseInitFinace)
		GetDlgItem(IDC_EDIT8)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT8)->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CTaiKlineDlgStopLose::OnOK() 
{

	UpdateData(TRUE);

	if(m_nTabCtrl.GetCurSel()==0)
	{
		 buySellAdditional.nEnterPoint[0]=m_Enterpoint;
         buySellAdditional.nColorPoint[0]=m_ColorPoint; 
	}
    if(m_nTabCtrl.GetCurSel()==1)
	{
		 buySellAdditional.nEnterPoint[1]=m_Enterpoint;
		 buySellAdditional.nColorPoint[1]=m_ColorPoint;
	}
     buySellAdditional.bLose=m_bLose;
	 buySellAdditional.bWin=m_bWin;
	 buySellAdditional.bMaxDays=m_bMaxDays;
	 buySellAdditional.bSubstractFee=m_bUsestractFee;
	 buySellAdditional.bAddPreProfit=m_bAddPreProfit;
	 buySellAdditional.bUseInitFinace=m_bUseInitFinace;
     buySellAdditional.fMaxLose=m_fMaxLose;
	 buySellAdditional.fMaxWin=m_fMaxWin;
	 buySellAdditional.fMaxDays=float(m_fMaxDays);
	 buySellAdditional.fInitFinace=m_fInitFinace;
	
	
	CFile BuySellFile;
    BuySellFile.Open("buySellAdditional.dat",CFile::modeWrite|CFile::modeCreate);
	BuySellFile.Write(& buySellAdditional,sizeof(ADDITIONAL_BUYSELL));

	if(GetInfo) SetAdditional(GetInfo->buySellAdditional);
	CDialog::OnOK();
}

void CTaiKlineDlgStopLose::OnSelchangeTablist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	if(m_nTabCtrl.GetCurSel()==0)
	{
		m_Enterpoint= buySellAdditional.nEnterPoint[0];
		m_ColorPoint= buySellAdditional.nColorPoint[0];
		InvalidateRect(&m_rectsample);
		UpdateWindow();

	}
	if(m_nTabCtrl.GetCurSel()==1)
	{
       
		m_Enterpoint= buySellAdditional.nEnterPoint[1];
		m_ColorPoint= buySellAdditional.nColorPoint[1];
		InvalidateRect(&m_rectsample);
		UpdateWindow();

	}
	
	
	UpdateData(FALSE);
	*pResult = 0;
}

void CTaiKlineDlgStopLose::OnSelchangeCombo1() 
{

	UpdateData(TRUE);
	if(m_nTabCtrl.GetCurSel()==0)
		 buySellAdditional.nEnterPoint[0]=m_Enterpoint;
	if(m_nTabCtrl.GetCurSel()==1)
		 buySellAdditional.nEnterPoint[1]=m_Enterpoint;
	
}

void CTaiKlineDlgStopLose::OnSetColor() 
{

    CColorDialog m_setcolor(m_ColorPoint,CC_RGBINIT,this);
	if(m_setcolor.DoModal())
	{
		m_ColorPoint=m_setcolor.GetColor();
		InvalidateRect(&m_rectsample);
		UpdateWindow();
		if(m_nTabCtrl.GetCurSel()==0)
		      buySellAdditional.nColorPoint[0]=m_ColorPoint; 
        if(m_nTabCtrl.GetCurSel()==1)
		      buySellAdditional.nColorPoint[1]=m_ColorPoint; 
	}	
}

void CTaiKlineDlgStopLose::OnPaint() 
{
	CPaintDC dc(this); 
	
	

	
	CBrush brush;                      
	CBrush *poldbrush;
	brush.CreateSolidBrush(m_ColorPoint);
	poldbrush=dc.SelectObject(&brush);
	dc.FillRect(&m_rectsample,&brush);
	dc.SelectObject(poldbrush);
	brush.DeleteObject();
	

}

void CTaiKlineDlgStopLose::SetAdditional(ADDITIONAL_BUYSELL &bsAdditional)
{
	memcpy( &bsAdditional,&(this->buySellAdditional),sizeof(ADDITIONAL_BUYSELL));
}

void CTaiKlineDlgStopLose::GetAdditional(ADDITIONAL_BUYSELL &bsAdditional)
{
	memcpy( &(this->buySellAdditional),&bsAdditional,sizeof(ADDITIONAL_BUYSELL));

}
