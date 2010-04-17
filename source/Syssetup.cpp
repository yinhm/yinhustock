// Syssetup.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "Syssetup.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CSYSSETUP, CPropertyPage)

CSYSSETUP::CSYSSETUP() : CPropertyPage(CSYSSETUP::IDD)
{
	//{{AFX_DATA_INIT(CSYSSETUP)
	m_AlarmSound = FALSE;
	m_AutoOrganizeData = FALSE;
	m_autoclose = FALSE;
	m_autoday = FALSE;
	m_showxline = FALSE;
	m_showyline = FALSE;
	m_autosavezb = FALSE;
	m_cjmxyester = FALSE;
	m_tjxgpower = FALSE;
	m_showinfohq = FALSE;
	m_startalert = FALSE;
	m_volpower = FALSE;
	m_backpower = FALSE;
	m_autominute = FALSE;
	m_kline = -1;
	m_fourgraph = -1;
	//}}AFX_DATA_INIT
	pDoc =CMainFrame::m_taiShanDoc;
	m_autoclose=pDoc->m_systemOption.autoclose;
	m_autominute=pDoc->m_systemOption.autominute;
	m_autoday=pDoc->m_systemOption.autoday;
	m_showxline=pDoc->m_systemOption.showxline;
	m_showyline=pDoc->m_systemOption.showyline;
	m_AlarmSound=pDoc->m_systemOption.alarmsound;
	m_AutoOrganizeData=pDoc->m_systemOption.organizedata;
	m_tjxgpower=pDoc->m_propertyInitiate.bDoPowerSelectingStock;
	m_volpower=pDoc->m_propertyInitiate.bDoVolPowering;
	m_backpower=pDoc->m_propertyInitiate.bDoPowerAfter;
	m_autosavezb=pDoc->m_propertyInitiate.bSaveParam;
	m_showinfohq=pDoc->m_propertyInitiate.bShowCrossDlg;
	m_startalert=pDoc->m_propertyInitiate.bRunAlarmBegin;
	m_cjmxyester=pDoc->m_propertyInitiate.bUseMin5ChenBen;

		m_kline=pDoc->m_systemOption.kline;
	
		m_fourgraph=pDoc->m_systemOption.fourgraph;
      
	
}

CSYSSETUP::~CSYSSETUP()
{
}

void CSYSSETUP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSYSSETUP)
	DDX_Check(pDX, IDC_ALARM_SOUND, m_AlarmSound);
	DDX_Check(pDX, IDC_AUTO_ORGANIZEDATA, m_AutoOrganizeData);
	DDX_Check(pDX, IDC_AUTOCLOSE, m_autoclose);
	DDX_Check(pDX, IDC_AUTODAY, m_autoday);
	DDX_Check(pDX, IDC_SHOWXLINE, m_showxline);
	DDX_Check(pDX, IDC_SHOWYLINE, m_showyline);
	DDX_Check(pDX, IDC_SYSSET_AUTOSAVEZHIBIAO, m_autosavezb);
	DDX_Check(pDX, IDC_SYSSET_HSTODAYADDLAST, m_cjmxyester);
	DDX_Check(pDX, IDC_SYSSET_POWERSELECTSTOCK, m_tjxgpower);
	DDX_Check(pDX, IDC_SYSSET_SHOWINFOCROSS, m_showinfohq);
	DDX_Check(pDX, IDC_SYSSET_STARTALERT, m_startalert);
	DDX_Check(pDX, IDC_SYSSET_VOLPOWER, m_volpower);
	DDX_Check(pDX, IDC_BACKPOWER, m_backpower);
	DDX_Check(pDX, IDC_AUTOMINUTE, m_autominute);
	DDX_Radio(pDX, IDC_RADIO3, m_kline);
	DDX_Radio(pDX, IDC_RADIO4T, m_fourgraph);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSYSSETUP, CPropertyPage)
	//{{AFX_MSG_MAP(CSYSSETUP)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CSYSSETUP::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

#ifdef WIDE_NET_VERSION
	GetDlgItem(IDC_AUTOMINUTE)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_AUTOMINUTE))->SetCheck(0);
	GetDlgItem(IDC_AUTODAY)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_AUTODAY))->SetCheck(0);
	GetDlgItem(IDC_ALARM_SOUND)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ALARM_SOUND))->SetCheck(0);
	GetDlgItem(IDC_AUTO_ORGANIZEDATA)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_AUTO_ORGANIZEDATA))->SetCheck(0);
	GetDlgItem(IDC_SYSSET_STARTALERT)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_SYSSET_STARTALERT))->SetCheck(0);
	GetDlgItem(IDC_AUTOCLOSE)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_AUTOCLOSE))->SetCheck(0);
#endif
	return TRUE;  
	           
}

BOOL CSYSSETUP::OnHelpInfo(HELPINFO* pHelpInfo) 
{


	DoHtmlHelp(this);return TRUE;
}
