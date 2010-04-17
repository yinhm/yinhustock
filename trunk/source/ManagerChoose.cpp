// ManagerChoose.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "selectstock.h"
#include "keybrodeangel.h"
#include "ManagerChoose.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperviseChoose dialog


CSuperviseChoose::CSuperviseChoose(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperviseChoose::IDD, pParent)
{
	
}


void CSuperviseChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperviseChoose)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_LIST1, m_MyList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSuperviseChoose, CDialog)
	//{{AFX_MSG_MAP(CSuperviseChoose)
	ON_BN_CLICKED(IDC_BUTTON1, OnAddStockToChoose)
	ON_BN_CLICKED(IDC_BUTTON2, OnDeleteStockFromChoose)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStockCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CSuperviseChoose::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);   
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	this->m_MyList.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_MyList.SetImageList(m_pImageListSmall, LVSIL_SMALL);
    FillToListCtrlHead();
	SymbolKindArr l_StockArray;
	m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
	int l_nCount=l_StockArray.GetSize();
    for(int i=0;i<l_nCount;i++)
	{
		CReportData *pCdat;
		SymbolKind l_sCode=l_StockArray.GetAt(i);
        if(m_pDoc->m_sharesInformation.Lookup(l_sCode.m_chSymbol,pCdat,l_sCode.m_nSymbolKind))
		{
           InsertStockToList(pCdat->name,pCdat->id,l_sCode.m_nSymbolKind);
		}
	}
	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;
    m_MyList.SetFocus();
    m_MyList.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	return TRUE;  
}

void CSuperviseChoose::OnOK() 
{

	
	CDialog::OnOK();
}
BOOL CSuperviseChoose::InsertStockToList(char *name,char *code,int nKind)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LPTSTR   pStrTemp1, pStrTemp2;
    m_MyList.SetColumnWidth( 0, 80 );
	iItem=this->m_MyList.GetItemCount();
	for (iSubItem = 0; iSubItem <2; iSubItem++)
	{
		if (iSubItem == 0)
			iIcon = 0;  

		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM  : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

	
		CString strIconDesc, strIconShortDesc;
        strIconShortDesc=name;
	    strIconDesc=code;
		pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		pStrTemp2= strIconDesc.GetBuffer(strIconDesc.GetLength());
		lvitem.pszText = iSubItem == 0? pStrTemp1 : pStrTemp2;

		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_MyList.InsertItem(&lvitem); 
		else
			this->m_MyList.SetItem(&lvitem); 
		m_MyList.SetItemData(iItem,nKind);
	} 
	return TRUE;
}

BOOL CSuperviseChoose::PreTranslateMessage(MSG* pMsg) 
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
			  CReportData *Cdat;
	 		  if (m_pDoc->m_sharesInformation.Lookup(result , Cdat,nKind) == TRUE)      
			  {
				  LV_ITEM  lvitem;
				  LVFINDINFO  lvf;
				  lvf.flags=LVFI_STRING;
				  lvf.psz=Cdat->name;
				  if(m_MyList.FindItem(&lvf)>=0)
			 		 return  TRUE;
              
				   m_pDoc->m_ManagerStockTypeData.InsertStockToChoose(Cdat->id,nKind);
                   InsertStockToList(Cdat->name,Cdat->id,nKind);
			  }
		   }
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSuperviseChoose::OnAddStockToChoose() 
{
	
	SelectStock pDlg(this,TRUE);
	pDlg.DoModal();
}

void CSuperviseChoose::OnDeleteStockFromChoose() 
{

    CString Id;
    int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;

	int temp=m_MyList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
		return;
	while(temp>=0)
	{
		char l_szCode[10];
		m_MyList.GetItemText( temp, 1, l_szCode, 20);
		int nKind=m_MyList.GetItemData(temp);
        m_pDoc->m_ManagerStockTypeData.DeleteStockFromChoose(l_szCode,nKind); 
		m_MyList.DeleteItem(temp);
        temp=m_MyList.GetNextItem( -1,LVNI_SELECTED   ) ;
	}
    m_MyList.SetFocus();
    m_MyList.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;


}
void CSuperviseChoose::FillToListCtrlHead()
{ 
   	CRect           rect;
	CString         strItem1= _T("股票名称");
	CString         strItem2= _T("股票代码");


	this->m_MyList.GetWindowRect(&rect);
	this->m_MyList.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1/4, 0);
	this->m_MyList.InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 1/4, 1);
}
LRESULT CSuperviseChoose::OnAddStockCode(WPARAM wParam, LPARAM lParam)
{
	SHARES_DATA_ADD *pAddCode;
	int lsCount=(int)lParam;
    pAddCode=(SHARES_DATA_ADD *)wParam;
	for(int i=0;i<lsCount;i++)
	{
		if(pAddCode[i].StockType<0)
		{
		    CReportData *Cdat;
			int nKind=-pAddCode[i].StockType-1;
	 	    if (m_pDoc->m_sharesInformation.Lookup(pAddCode[i].name, Cdat,nKind) == TRUE)    
			{
				LV_ITEM  lvitem;
				int iItem=0;
				LVFINDINFO  lvf;
				lvf.flags=LVFI_STRING;
				lvf.psz=Cdat->name;
				iItem=m_MyList.FindItem(&lvf);
				if(iItem>=0)
				{
                    if(nKind==m_MyList.GetItemData(iItem))  
				       continue;
				}
				m_pDoc->m_ManagerStockTypeData.InsertStockToChoose(Cdat->id,nKind);
                InsertStockToList(Cdat->name,Cdat->id,nKind);
			}
		}
	}

	return 0;
}
