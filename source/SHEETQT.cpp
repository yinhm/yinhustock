// SHEETQT.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "SHEETQT.h"

#include "CTaiShanDoc.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetOthersPage property page

IMPLEMENT_DYNCREATE(CTaiSysSetOthersPage, CPropertyPage)

CTaiSysSetOthersPage::CTaiSysSetOthersPage() : CPropertyPage(CTaiSysSetOthersPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiSysSetOthersPage)
	m_autoclose = FALSE;
	m_autominute = FALSE;
	m_maingraph = -1;
	m_autoday = FALSE;
	m_fourgraph = -1;
	m_kline = -1;
	m_AlarmSound=FALSE;
	m_AutoOrganizeData=FALSE;
	m_bAutoSaveParamOfJishu = FALSE;
	m_bHsTodayAddLast = FALSE;
	m_bDoPowerSelectingStock = FALSE;
	m_bShowInfoCrossing = FALSE;
	m_bStartAlertOnBegin = FALSE;
	m_bDoVolPowering = FALSE;
	//}}AFX_DATA_INIT
	
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
    
//	m_buyrate=pDoc->m_systemdata.buyrate;                    
	m_autoclose=pDoc->m_systemOption.autoclose;                 
	m_autominute=pDoc->m_systemOption.autominute;                
	m_autoday=pDoc->m_systemOption.autoday;                    
//	m_changepage=pDoc->m_systemdata.changepage;                   
//	m_AdvSpTimes=pDoc->m_systemdata.AdvSpTimes;                 
//	m_fiveminutesave=pDoc->m_systemdata.fiveminutesave;          
    m_showxline=pDoc->m_systemOption.showxline;                 
	m_showyline=pDoc->m_systemOption.showyline;
//	m_noticesave=pDoc->m_systemdata.noticesave;                   
//	m_AlarmPercent=pDoc->m_systemdata.AlarmPercent;                   
	m_AlarmSound=pDoc->m_systemOption.alarmsound;
	m_AutoOrganizeData=pDoc->m_systemOption.organizedata;
	index=0;                           
	if(pDoc->m_systemOption.kline)
		m_kline=0;
	else m_kline=1;        
	if(pDoc->m_systemOption.fourgraph)
		m_fourgraph=0;
	else m_fourgraph=1;       
}

CTaiSysSetOthersPage::~CTaiSysSetOthersPage()
{
}

void CTaiSysSetOthersPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiSysSetOthersPage)
	DDX_Control(pDX, IDC_LISTSELECT, m_listselect);
	DDX_Control(pDX, IDC_LISTALL, m_listall);
	DDX_Check(pDX, IDC_AUTOCLOSE, m_autoclose);
	DDX_Check(pDX, IDC_AUTOMINUTE, m_autominute);
	DDX_Check(pDX, IDC_SHOWXLINE,m_showxline);
	DDX_Check(pDX, IDC_SHOWYLINE,m_showyline);
	DDX_Check(pDX, IDC_AUTODAY, m_autoday);
	DDX_Radio(pDX, IDC_RADIO4T, m_fourgraph);
	DDX_Radio(pDX, IDC_RADIO3, m_kline);
	DDX_Check(pDX,IDC_ALARM_SOUND,m_AlarmSound);
	DDX_Check(pDX,IDC_AUTO_ORGANIZEDATA,m_AutoOrganizeData);
	DDX_Check(pDX, IDC_SYSSET_AUTOSAVEZHIBIAO, m_bAutoSaveParamOfJishu);
	DDX_Check(pDX, IDC_SYSSET_HSTODAYADDLAST, m_bHsTodayAddLast);
	DDX_Check(pDX, IDC_SYSSET_POWERSELECTSTOCK, m_bDoPowerSelectingStock);
	DDX_Check(pDX, IDC_SYSSET_SHOWINFOCROSS, m_bShowInfoCrossing);
	DDX_Check(pDX, IDC_SYSSET_STARTALERT, m_bStartAlertOnBegin);
	DDX_Check(pDX, IDC_SYSSET_VOLPOWER, m_bDoVolPowering);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiSysSetOthersPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiSysSetOthersPage)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTALL, OnDblclkListall)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTSELECT, OnDblclkListselect)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO4T, OnRadio4t)
	ON_BN_CLICKED(IDC_RADIO9T, OnRadio9t)
	ON_NOTIFY(NM_SETFOCUS, IDC_LISTALL, OnSetfocusListall)
	ON_NOTIFY(NM_SETFOCUS, IDC_LISTSELECT, OnSetfocusListselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CTaiSysSetOthersPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
    
	CheckRadioButton(IDC_RADIO3,IDC_RADIO4,IDC_RADIO3);
	CheckRadioButton(IDC_RADIO4T,IDC_RADIO9T,IDC_RADIO9T);
	UpdateData(FALSE);
	const int combqtlength=5;
    CString  combstring[combqtlength]={"(开+收+高+低)/4","(高+低+收)/3",
		                                   "(高+低+2收)/4","(高+收)/2","收盘价"};

	int iComItem=pDoc->m_systemdata.middleprice;
 

	m_listall.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	zbtotalnumber=pDoc->m_formuar_index.GetSize();
	m_listselect.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int j=0;
	for(int i=0;i<zbtotalnumber;i++)
	{
		m_listall.InsertItem(i,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
		if(pDoc->m_formuar_index.GetAt(i)->isOfen)
			m_listselect.InsertItem(j++,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
		
	}
	GetDlgItem(IDC_ADD)->SetFocus();
	
	return FALSE;  
}

void CTaiSysSetOthersPage::OnAdd() 
{
	
    CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
    POSITION  position=m_listall.GetFirstSelectedItemPosition();
	if(position==NULL)
	{
		AfxMessageBox("请选择一个指标",MB_OK|MB_ICONASTERISK );
			return;
	}
	int selectedcount=m_listall.GetSelectedCount( );
	int item;
	for(int i=0;i<selectedcount;i++)
	{
		item=m_listall.GetNextSelectedItem(position);
		CFormularContent  *toadd;
		
		toadd=pDoc->m_formuar_index.GetAt(item);

		if(toadd->isOfen)
		{
			CString str=toadd->name+"已经是常用指标，请重新选择";
			AfxMessageBox((LPCTSTR)str,MB_OK|MB_ICONASTERISK );
		}
		else
			toadd->isOfen=true;
	}
	m_listselect.DeleteAllItems();
	UpdateImageList();		
}

void CTaiSysSetOthersPage::OnDel() 
{
	
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
	POSITION  position=m_listselect.GetFirstSelectedItemPosition();
	int total=pDoc->m_formuar_index.GetSize();
	if(position==NULL)
	{
		AfxMessageBox("请选择一个指标",MB_OK|MB_ICONASTERISK );
			return;
	}
	int selectedcount=m_listselect.GetSelectedCount();
	int item;
	int *array=new int[selectedcount];
	for(int j=0;j<selectedcount;j++)
	{
		item=m_listselect.GetNextSelectedItem(position);
		
		int i;
		for(i=0;i<total;i++)
		{
			if(pDoc->m_formuar_index.GetAt(i)->isOfen)
			item--;
			if(item==-1)           
				break;
		}
		array[j]=i;
		
	}
	for(int j=0;j<selectedcount;j++)
	{
		pDoc->m_formuar_index.GetAt(array[j])->isOfen=false;
	}
	delete array;
	m_listselect.DeleteAllItems();
	UpdateImageList();
}

void CTaiSysSetOthersPage::UpdateImageList()
{
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();

	int total=pDoc->m_formuar_index.GetSize();
	int j=0;
	for(int i=0;i<total;i++)
	{
		if(pDoc->m_formuar_index.GetAt(i)->isOfen)
		m_listselect.InsertItem(j++,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
	}
}

void CTaiSysSetOthersPage::OnDblclkListall(NMHDR* pNMHDR, LRESULT* pResult) 
{

	POSITION  position=m_listall.GetFirstSelectedItemPosition();
    if(position!=NULL)
		OnAdd();
	*pResult = 0;
}

void CTaiSysSetOthersPage::OnDblclkListselect(NMHDR* pNMHDR, LRESULT* pResult) 
{

	POSITION  position=m_listselect.GetFirstSelectedItemPosition();
	if(position!=NULL)
	OnDel();	
	*pResult = 0;
}

void CTaiSysSetOthersPage::OnRadio3() 
{

	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4,IDC_RADIO3);
	if(itemid==IDC_RADIO3)
		m_kline=0;
	else
		m_kline=1;
}

void CTaiSysSetOthersPage::OnRadio4() 
{

	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4,IDC_RADIO3);
	if(itemid==IDC_RADIO3)
		m_kline=1;
	else
		m_kline=0;
}

void CTaiSysSetOthersPage::OnRadio4t() 
{
	
	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4T,IDC_RADIO9T);
	if(itemid==IDC_RADIO4T)
		m_fourgraph=0;
	else
		m_fourgraph=1;
}

void CTaiSysSetOthersPage::OnRadio9t() 
{

	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4T,IDC_RADIO9T);
	if(itemid==IDC_RADIO4T)
		m_fourgraph=1;
	else
		m_fourgraph=0;
}

void CTaiSysSetOthersPage::OnSetfocusListall(NMHDR* pNMHDR, LRESULT* pResult) 
{

	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_DEL)->EnableWindow(FALSE);
	*pResult = 0;
}

void CTaiSysSetOthersPage::OnSetfocusListselect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_DEL)->EnableWindow(TRUE);
	*pResult = 0;
}
