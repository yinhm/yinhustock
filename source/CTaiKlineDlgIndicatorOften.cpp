// TechDlgIndicatorOften.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgIndicatorOften.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CFormularContent.h"
#include "CTaiKlineIndicatorsManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgIndicatorOften dialog


CTaiKlineDlgIndicatorOften::CTaiKlineDlgIndicatorOften(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgIndicatorOften::IDD, pParent)
{
	pDoc=CMainFrame::m_taiShanDoc ;
	m_pIndex = 0;

	//{{AFX_DATA_INIT(CTaiKlineDlgIndicatorOften)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTaiKlineDlgIndicatorOften::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgIndicatorOften)
	DDX_Control(pDX, IDC_LISTSELECT, m_listselect);
	DDX_Control(pDX, IDC_LISTALL, m_listall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgIndicatorOften, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgIndicatorOften)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTALL, OnDblclkListall)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTSELECT, OnDblclkListselect)
	ON_WM_DESTROY()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgIndicatorOften message handlers

void CTaiKlineDlgIndicatorOften::OnAdd() 
{
	// TODO: Add your control notification handler code here
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
//		CFormularContent  *toadd;
		
//		toadd=pDoc->m_formuar_index.GetAt(item);

		if(m_pIndex[item] == 1 )
		{
			CString s2;
			s2 = m_listall.GetItemText (item,0);
			CString str=s2+"已经是常用指标，请重新选择";
			AfxMessageBox((LPCTSTR)str,MB_OK|MB_ICONASTERISK );
		}
		else
		{
			m_pIndex[item] = 1;
//			toadd->isOfen=true;
			
		}
	}
	m_listselect.DeleteAllItems();
	UpdateImageList();		
	
	
}

void CTaiKlineDlgIndicatorOften::OnDel() 
{
	

	POSITION  position=m_listselect.GetFirstSelectedItemPosition();
	int total=pDoc->m_formuar_index.GetSize();
	if(position==NULL)
	{
		AfxMessageBox("请选择一个指标",MB_OK|MB_ICONASTERISK );
			return;
	}
	int selectedcount=m_listselect.GetSelectedCount();
	int item;
	for(int j=0;j<selectedcount;j++)
	{
		item=m_listselect.GetNextSelectedItem(position);
									
		CString s2;
		s2 = m_listselect.GetItemText (item,0);
		for(int i=0;i<total;i++)
		{
			if(pDoc->m_formuar_index.GetAt(i)->name == s2)
			{
				m_pIndex[i] = 0;
				break;
			}
		}
	
	}
	m_listselect.DeleteAllItems();
	UpdateImageList();	
}

BOOL CTaiKlineDlgIndicatorOften::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_listall.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int zbtotalnumber=pDoc->m_formuar_index.GetSize();
	m_pIndex = new int[zbtotalnumber];
	memset(m_pIndex,0,zbtotalnumber*sizeof(int));
	
	m_listselect.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int j=0;
	for(int  i=0;i<zbtotalnumber;i++)
	{
		m_listall.InsertItem(i,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
		if(pDoc->m_formuar_index.GetAt(i)->isOfen)
		{
			m_listselect.InsertItem(j++,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
			m_pIndex[i]=1;
		}
		
	}
	
	return TRUE; 
}

void CTaiKlineDlgIndicatorOften::UpdateImageList()
{

	int total=pDoc->m_formuar_index.GetSize();
	int j=0;
	for(int i=0;i<total;i++)
	{
		CString s2;
		s2 = m_listall.GetItemText (i,0);
		if(m_pIndex[i])
			m_listselect.InsertItem(j++,s2,4);
	}



}

void CTaiKlineDlgIndicatorOften::OnDblclkListall(NMHDR* pNMHDR, LRESULT* pResult) 
{

	POSITION  position=m_listall.GetFirstSelectedItemPosition();
    if(position!=NULL)
		OnAdd();
	*pResult = 0;
}

void CTaiKlineDlgIndicatorOften::OnDblclkListselect(NMHDR* pNMHDR, LRESULT* pResult) 
{

	POSITION  position=m_listselect.GetFirstSelectedItemPosition();
	if(position!=NULL)
	OnDel();
	
	*pResult = 0;
}

void CTaiKlineDlgIndicatorOften::OnOK() 
{

	int total=pDoc->m_formuar_index.GetSize();
	for(int i=0;i<total;i++)
	{
		if(m_pIndex[i]==0)
			pDoc->m_formuar_index.GetAt(i)->isOfen = FALSE;
		else
			pDoc->m_formuar_index.GetAt(i)->isOfen = TRUE;
	}
	CFormularContent::ReadWriteIndex(pDoc,0, false);

	CDialog::OnOK();
}

void CTaiKlineDlgIndicatorOften::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if(m_pIndex != 0) delete [] m_pIndex;

	
}

BOOL CTaiKlineDlgIndicatorOften::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineIndicatorsManage::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}
