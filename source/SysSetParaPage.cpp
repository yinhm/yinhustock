// SysSetParaPage.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "SysSetParaPage.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CTaiSysSetParameterPage, CPropertyPage)

CTaiSysSetParameterPage::CTaiSysSetParameterPage() : CPropertyPage(CTaiSysSetParameterPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiSysSetParameterPage)
	m_fBuyRate = 0.0f;
	m_nCapitalTime = 0;
	m_nChangeHandTime = 0;
	m_nChangPageTime = 0;
	m_nDaysOfDayKline = 0;
	m_nCountOfHistoryFile = 0;
	m_nDaysOfMin5Kline = 0;
	m_nDaysOfManyKline = 0;
	m_nMidWay = 0;
	m_nNoFinishedTest = 0;
	m_nQuantityTime = 0;
	m_fSellRate = 0;
	m_nSwingTime = 0;
	m_nUpSpeedTime = 0;
	m_nTimespanAlert = 0;
	m_noticesave = 0;
	m_nVol = 0.0f;
	m_nPer = 0.0f;
	m_nAcount = 0.0f;
	m_nFormat = -1;
	m_nAddress = _T("");
	m_nRealTime = 0;
	m_nBlockTime = 0;
	m_nKswtAddr = _T("");
	m_cjl_rateV = 0.5f;
	m_ltp_rateV = 0.2f;
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_taiShanDoc;
	m_nDaysOfDayKline=pDoc->m_propertyInitiate.daysOfDayKline;
	m_nDaysOfManyKline=pDoc->m_propertyInitiate.daysOfManyKline;
	m_nCountOfHistoryFile=pDoc->m_propertyInitiate.countOfHistoryFile;
    m_nDaysOfMin5Kline =pDoc->m_propertyInitiate.daysOfMin5Kline;
	m_noticesave=pDoc->m_systemdata.noticesave;
    
    m_nCapitalTime =pDoc->m_syshqset.capital;
	m_nQuantityTime=pDoc->m_syshqset.quangtity;
	m_nSwingTime=pDoc->m_syshqset.swing;
	m_nUpSpeedTime=pDoc->m_syshqset.upspeed;
	m_nChangeHandTime=pDoc->m_syshqset.changehand;

    m_nNoFinishedTest=pDoc->m_propertyInitiate.nTestUnfinished;
	m_nMidWay=pDoc->m_systemdata.middleprice;

    m_nTimespanAlert=pDoc->m_propertyInitiate.nSecondsAlarm;
    m_nChangPageTime=pDoc->m_systemdata.changepage;

	if(pDoc->m_propertyInitiate.LTP_rate==NULL)
		m_ltp_rateV=float(0.1);
	else
		m_ltp_rateV=pDoc->m_propertyInitiate.LTP_rate;
	if(pDoc->m_propertyInitiate.CJL_rate==NULL)
		m_cjl_rateV=0.5;
	else
		m_cjl_rateV=pDoc->m_propertyInitiate.CJL_rate;
	
	m_nVol=pDoc->m_dadanchoice.Vol;
	m_nPer=pDoc->m_dadanchoice.Per;
	m_nAcount=pDoc->m_dadanchoice.Acount;
	m_nFormat=pDoc->m_dadanchoice.Format;
	m_nAddress=&pDoc->m_SystemInitData.NetAddr[0];
	if(pDoc->m_SystemInitData.KswtAddr==NULL)
	   m_nKswtAddr=" ";
	else
	   m_nKswtAddr=pDoc->m_SystemInitData.KswtAddr;
	m_nRealTime=pDoc->m_SystemInitData.RealTime;
	m_nBlockTime=pDoc->m_SystemInitData.BlockTime;
}

CTaiSysSetParameterPage::~CTaiSysSetParameterPage()
{
}

void CTaiSysSetParameterPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiSysSetParameterPage)
	DDX_Control(pDX, IDC_EDIT6, m_nBlockT);
	DDX_Control(pDX, IDC_EDIT5, m_nRealT);
	DDX_Control(pDX, IDC_SYSSET_WARNTIME, m_nTimespan);
	DDX_Control(pDX, IDC_SYSSET_UPSPEEDTIME, m_nUpSpeed);
	DDX_Control(pDX, IDC_SYSSET_SWINGTIME, m_nSwing);
	DDX_Control(pDX, IDC_SYSSET_QUANTITYTIME, m_nQuantity);
	DDX_Control(pDX, IDC_SYSSET_FIVEMINU, m_nDaysOfMIn);
	DDX_Control(pDX, IDC_SYSSET_FENBI, m_nCountOf);
	DDX_Control(pDX, IDC_SYSSET_DAYLINE, m_nDaysOfDay);
	DDX_Control(pDX, IDC_SYSSET_CHANGEPAGETIME, m_nChangePage);
	DDX_Control(pDX, IDC_SYSSET_CHANGEHANDTIME, m_nChange);
	DDX_Control(pDX, IDC_SYSSET_CAPITALTIME, m_nCapital);
	DDX_Control(pDX, IDC_INFOSAVE, m_notice);
	DDX_Control(pDX, IDC_EDIT4, m_nacouny);
	DDX_Control(pDX, IDC_EDIT3, m_nper);
	DDX_Control(pDX, IDC_EDIT1, m_n);
	DDX_Control(pDX, IDC_SYSSET_MULTIDAY, m_nDaysOfMany);
	DDX_Text(pDX, IDC_SYSSET_CAPITALTIME, m_nCapitalTime);
	DDV_MinMaxInt(pDX, m_nCapitalTime, 5, 60);
	DDX_Text(pDX, IDC_SYSSET_CHANGEHANDTIME, m_nChangeHandTime);
	DDV_MinMaxInt(pDX, m_nChangeHandTime, 5, 60);
	DDX_Text(pDX, IDC_SYSSET_CHANGEPAGETIME, m_nChangPageTime);
	DDV_MinMaxInt(pDX, m_nChangPageTime, 1, 10000);
	DDX_Text(pDX, IDC_SYSSET_DAYLINE, m_nDaysOfDayKline);
	DDV_MinMaxInt(pDX, m_nDaysOfDayKline, 1000, 20000);
	DDX_Text(pDX, IDC_SYSSET_FENBI, m_nCountOfHistoryFile);
	DDV_MinMaxInt(pDX, m_nCountOfHistoryFile, 0, 10000);
	DDX_Text(pDX, IDC_SYSSET_FIVEMINU, m_nDaysOfMin5Kline);
	DDV_MinMaxInt(pDX, m_nDaysOfMin5Kline, 1, 10000);
	DDX_Text(pDX, IDC_SYSSET_MULTIDAY, m_nDaysOfManyKline);
	DDX_CBIndex(pDX, IDC_SYSSET_MIDWAY, m_nMidWay);
	DDX_CBIndex(pDX, IDC_SYSSET_NOFINISHED, m_nNoFinishedTest);
	DDX_Text(pDX, IDC_SYSSET_QUANTITYTIME, m_nQuantityTime);
	DDV_MinMaxInt(pDX, m_nQuantityTime, 5, 60);
	DDX_Text(pDX, IDC_SYSSET_SWINGTIME, m_nSwingTime);
	DDV_MinMaxInt(pDX, m_nSwingTime, 5, 60);
	DDX_Text(pDX, IDC_SYSSET_UPSPEEDTIME, m_nUpSpeedTime);
	DDV_MinMaxInt(pDX, m_nUpSpeedTime, 5, 60);
	DDX_Text(pDX, IDC_SYSSET_WARNTIME, m_nTimespanAlert);
	DDV_MinMaxInt(pDX, m_nTimespanAlert, 1, 10000);
	DDX_Text(pDX, IDC_INFOSAVE, m_noticesave);
	DDV_MinMaxInt(pDX, m_noticesave, 1, 10000);
	DDX_Text(pDX, IDC_EDIT1, m_nVol);
	DDV_MinMaxFloat(pDX, m_nVol, 1.f, 9.e+006f);
	DDX_Text(pDX, IDC_EDIT3, m_nPer);
	DDV_MinMaxFloat(pDX, m_nPer, 0.f, 100.f);
	DDX_Text(pDX, IDC_EDIT4, m_nAcount);
	DDV_MinMaxFloat(pDX, m_nAcount, 0.f, 9.e+006f);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nFormat);
	DDX_Text(pDX, IDC_EDITADDRESS, m_nAddress);
	DDV_MaxChars(pDX, m_nAddress, 30);
	DDX_Text(pDX, IDC_EDIT5, m_nRealTime);
	DDV_MinMaxInt(pDX, m_nRealTime, 5, 60);
	DDX_Text(pDX, IDC_EDIT6, m_nBlockTime);
	DDV_MinMaxInt(pDX, m_nBlockTime, 1, 12);
	DDX_Text(pDX, IDC_EDIT7, m_nKswtAddr);
	DDV_MaxChars(pDX, m_nKswtAddr, 40);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiSysSetParameterPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiSysSetParameterPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CTaiSysSetParameterPage::OnSelchangeCombo1() 
{
	
    UpdateData(TRUE);
	if(m_nFormat==0)
	{
        GetDlgItem(IDC_CJHAND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT1)->SetFocus();
		GetDlgItem(IDC_LTPER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CJJE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WANYUAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT4)->ShowWindow(SW_HIDE);

	}
	if(m_nFormat==1)
	{
        GetDlgItem(IDC_CJHAND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LTPER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT3)->SetFocus();
		GetDlgItem(IDC_CJJE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_WANYUAN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT4)->ShowWindow(SW_HIDE);
	}
	if(m_nFormat==2)
	{
        GetDlgItem(IDC_CJHAND)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LTPER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CJJE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WANYUAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT4)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT4)->SetFocus();
	}
}

BOOL CTaiSysSetParameterPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog(); 
	

	GetDlgItem(IDC_SYSSET_DAYLINE)->SetFocus();
    GetDlgItem(IDC_CJHAND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LTPER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CJJE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_WANYUAN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT4)->ShowWindow(SW_HIDE);

	if(m_nFormat==0)
	{
        GetDlgItem(IDC_CJHAND)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);
	}
	if(m_nFormat==1)
	{
		GetDlgItem(IDC_LTPER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT3)->ShowWindow(SW_SHOW);
	}
	if(m_nFormat==2)
	{
		GetDlgItem(IDC_CJJE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_WANYUAN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT4)->ShowWindow(SW_SHOW);
	}	


#ifdef WIDE_NET_VERSION
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);


	GetDlgItem(IDC_SYSSET_DAYLINE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_FENBI)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_FIVEMINU)->EnableWindow(FALSE);
	GetDlgItem(IDC_INFOSAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_CAPITALTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_QUANTITYTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_SWINGTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_UPSPEEDTIME)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_SYSSET_WARNTIME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYSSET_CHANGEHANDTIME)->EnableWindow(FALSE);
	
#endif
	return TRUE;  
}

BOOL CTaiSysSetParameterPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	DoHtmlHelp(this);return TRUE;
}
