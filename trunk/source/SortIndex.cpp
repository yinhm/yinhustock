// SortIndex.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiScreenParent.h"
#include "CFormularCompute.h"
#include "MainFrm.h"
#include "CFormularContent.h"
#include "CTaiKlineFileHS.h"
#include "SortIndex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////



SortIndex::SortIndex(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(SortIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(SortIndex)
	m_periodstr = _T("");
	//}}AFX_DATA_INIT
	m_pImageList = NULL;
	m_pImageListSmall= NULL;
    m_bIsCalc=FALSE;
}
SortIndex::~SortIndex()
{
	if( m_pImageList != NULL)
		delete m_pImageList;
	if( m_pImageListSmall != NULL)
		delete m_pImageListSmall;
}
SortIndex::SortIndex(IndexDataInfo *pIndexSave,UINT nID,BOOL IsInit,BOOL IsChanged,CWnd* pParent)
	: CTaiScreenParent(SortIndex::IDD, pParent)
{
	memcpy(&m_nIndexSave,pIndexSave,sizeof(IndexDataInfo));
	m_nColumnId=nID;
	m_bIsInit=IsInit;
    m_bIsChangeTechnique=IsChanged;
	m_pImageList = NULL;
	m_pImageListSmall= NULL;
    m_bIsCalc=FALSE;

}
void SortIndex::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SortIndex)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LISTJISHU, m_TechniqueList);
	DDX_Control(pDX, IDC_SORTHELP, m_HelpCtl);

	DDX_CBString(pDX, IDC_COMBO_PERIOD, m_periodstr);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SortIndex, CTaiScreenParent)
	//{{AFX_MSG_MAP(SortIndex)
	ON_BN_CLICKED(IDC_JISHU, OnTechnique)
	ON_NOTIFY(NM_CLICK,IDC_LISTJISHU, OnClick)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL SortIndex::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();
	m_btOK.SetIcon(IDI_ICON_OK);
	m_btCancel.SetIcon(IDI_EXIT16);

	CFormularContent*	pIndex = NULL;
	pIndex=(CFormularContent*)(LookUpArray(-1,m_nIndexSave.sIndex_name,pDoc)); 
    InitTechniqueIndex(pIndex,m_bIsInit);

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	FillTechniqueToListCtrl();

	CWnd *pWnd;
    pWnd=this->GetDlgItem(IDC_COMBO_PERIOD);
	pWnd->ShowWindow(SW_SHOW);
	pWnd=this->GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL);
	pWnd->ShowWindow(SW_SHOW);

	if(!m_bIsChangeTechnique)
	{
		CButton *pWnd=(CButton *)this->GetDlgItem(IDC_JISHU);

	}
	return TRUE;  
}
void SortIndex::OnClick(NMHDR* wParam, LRESULT* lParam)
{
	char nIndexName[50];
	int temp=m_TechniqueList.GetNextItem( -1, LVNI_SELECTED    ) ;
	if(temp==-1)
		return;
	m_TechniqueList.GetItemText( temp, 0, nIndexName, 50 );
	CFormularContent*	pIndex = NULL;
	pIndex=(CFormularContent*)(LookUpArray(-1,nIndexName,pDoc)); 
    InitTechniqueIndex(pIndex,FALSE);
}

void SortIndex::FillTechniqueToListCtrl()
{ 
   	CRect           rect;
	int             iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM         lvitem;
	CString         strItem1= _T("技术指标");
	CString         strItem2= _T("指标简要说明");
	LPTSTR          pStrTemp1, pStrTemp2;

	m_TechniqueList.SetImageList(m_pImageList, LVSIL_NORMAL);
	m_TechniqueList.SetImageList(m_pImageListSmall, LVSIL_SMALL);


	m_TechniqueList.GetWindowRect(&rect);
	m_TechniqueList.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1/3, 0);
	m_TechniqueList.InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 2/3, 1);

	int num=pDoc->m_formuar_index.GetSize();
	for(iItem=0;iItem<num;iItem++)
	{
		if(pDoc->m_formuar_index.ElementAt(iItem)->isOfen )
		{
			for (iSubItem = 0; iSubItem < 2; iSubItem++)
			{
				if (iSubItem == 0)
					iIcon = 0; 

				lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM  : 0);
				lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
				lvitem.iSubItem = iSubItem;
				lvitem.lParam = 1000+iItem;

			
				CString strIconDesc, strIconShortDesc;
                strIconShortDesc=pDoc->m_formuar_index.ElementAt(iItem)->name;
                strIconDesc=pDoc->m_formuar_index.ElementAt(iItem)->explainBrief;
				pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
				pStrTemp2= strIconDesc.GetBuffer(strIconDesc.GetLength());
				lvitem.pszText = iSubItem == 0? pStrTemp1 : pStrTemp2;
 
				lvitem.iImage = iIcon;
				if (iSubItem == 0)
					iActualItem = m_TechniqueList.InsertItem(&lvitem); 
				else
					m_TechniqueList.SetItem(&lvitem); // modify existing item (the sub-item text)
			} 
		}
	}
}
void SortIndex::OnTechnique() 
{

	return;
	static BOOL bExpand=TRUE;
	ExpandDialog(IDC_DIVIDER,bExpand);
	bExpand=!bExpand;
}
void SortIndex::ExpandDialog(int nResourceID,BOOL bExpand)
{
	static CRect rcLarge;
	static CRect rcSmall;
	CString sExpand;
	if(rcLarge.IsRectEmpty() )
	{
		CRect rcLandmark;
		CWnd * pWnd=GetDlgItem(nResourceID);
		ASSERT(pWnd);
		GetWindowRect(rcLarge);
		pWnd->GetWindowRect(rcLandmark) ;
		rcSmall=rcLarge;
		rcSmall.bottom =rcLandmark.top;
	}
    if(bExpand)
	{
		SetWindowPos(NULL,0,0,rcLarge.Width(),rcLarge.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
		sExpand="<<指标设置";

	}
	else
	{
		SetWindowPos(NULL,0,0,rcSmall.Width(),rcSmall.Height(),
			SWP_NOMOVE|SWP_NOZORDER);
		sExpand="指标设置>>";

	}
	this->SetDlgItemText(IDC_JISHU,sExpand);
}
void SortIndex::EnableVisibleChildren()
{
   CWnd *pWndCtl=GetWindow(GW_CHILD);
   CRect rcTest;
   CRect rcControl;
   CRect rcShow;
   GetWindowRect(rcShow);
   while(pWndCtl!=NULL)
   {
	   pWndCtl->GetWindowRect(rcControl);

	   if(rcTest.IntersectRect(rcShow,rcControl) )
		   pWndCtl->EnableWindow(TRUE);
	   else
		   pWndCtl->EnableWindow(FALSE);
       
	   pWndCtl=pWndCtl->GetWindow(GW_HWNDNEXT);
   }
}

void SortIndex::OnOK() 
{
	
	m_bIsCalc=TRUE;
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

	CWnd *pWnd=this->GetDlgItem(IDCANCEL);
	pWnd->EnableWindow(FALSE);

	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);

	this->UpdateData(TRUE);
	int index=0;
    index=this->m_sLineAll.GetCurSel( ) ;
	this->m_sLineAll.GetLBText(index,m_sLTypeName); 
    m_nIndexSave.iLine= index;

	index=m_period.GetCurSel();
	m_nIndexSave.iDataType=index;

	char nIndexName[50];
	int temp=m_TechniqueList.GetNextItem( -1, LVNI_SELECTED    ) ;
	if(temp!=-1)
	{
	   m_TechniqueList.GetItemText( temp, 0, nIndexName, 50 );
       strcpy(m_nIndexSave.sIndex_name,nIndexName); 
    }

	float fVal[8];
	m_conRich.GetParamVal((float *)fVal);
	for(int i=0;i<8;i++)
	{
        m_nIndexSave.fParam[i] =fVal[i];
	}

	CalcTechniqueIndexData();
	m_bIsCalc=FALSE;
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
	CTaiScreenParent::OnOK();
}
void SortIndex::InitTechniqueIndex(CFormularContent*	pIndex,BOOL IsInit)
{
	char temp[50];
	int defSelectType=0;
	sprintf(temp,"%s %s",pIndex->name,"技术指标"); 
	this->SetWindowText(temp); 
	
	GetIndexLineName(pIndex);
    int count=this->m_sLineAll.GetCount();
	for(int i=0;i<count;i++ )
      this->m_sLineAll.DeleteString(0) ;

	for(int i=0;i<this->nLineName.GetSize();i++ )
      this->m_sLineAll.AddString( nLineName.GetAt(i) );
    count=m_period.GetCount( );
    for(int i=0;i<count;i++ )
	   m_period.DeleteString( 0 );
    
	for(int i=0;i<11;i++)
		pIndex->nPeriodsUsed[i]=1;


	int i;
	for(i=0;i<11;i++)
	{
       switch(i)
	   {
	   case 0:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("明细");
		   break;
	   case 1:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("1分钟");
		   break;
	   case 2:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("5分钟");
		   break;
	   case 3:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("15分钟");
		   break;
	   case 4:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("30分钟");
		   break;
	   case 5:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("60分钟");
		   break;
	   case 6:
             if(pIndex->nPeriodsUsed[i]==1)
			 {
				 defSelectType=i;
                m_period.AddString("日线");
			 }
		   break;
	   case 7:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("周线");
		   break;
	   case 8:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("月线");
		   break;
	   case 9:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("年线");
		   break;
	   case 10:
             if(pIndex->nPeriodsUsed[i]==1)
                m_period.AddString("多日线");
		   break;
	   }
	}
	this->m_HelpCtl.SetWindowText(pIndex->help);
	if(IsInit)
	{
       int sel=0;
       switch(m_nIndexSave.iDataType)
	   {
		   case 0:
                sel=m_period.FindString(0,"明细");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 1:
                sel=m_period.FindString(0,"1分钟");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 2:
                sel=m_period.FindString(0,"5分钟");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 3:
                sel=m_period.FindString(0,"15分钟");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case	4: 
                sel=m_period.FindString(0,"30分钟");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 5:
                sel=m_period.FindString(0,"60分钟");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 6:
                sel=m_period.FindString(0,"日线");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 7:
                sel=m_period.FindString(0,"周线");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 8:
                sel=m_period.FindString(0,"月线");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case 9:
                sel=m_period.FindString(0,"年线");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
		   case	10:
                sel=m_period.FindString(0,"多日线");
				if(sel!=CB_ERR )
                   m_period.SetCurSel(sel);
				else
                   m_period.SetCurSel(0);
                break;
	   }
       this->m_sLineAll.SetCurSel(m_nIndexSave.iLine);

	   	m_conRich.SetLJishu(pIndex);

		CString temp;
		int i = 0;
	}
	else
	{
       this->m_sLineAll.SetCurSel(0);
       m_period.SetCurSel(defSelectType);

	   	m_conRich.SetLJishu(pIndex);
	}
}

void SortIndex::CalcTechniqueIndexData()
{
	MSG message;
	int nDays=GetCountValid(&m_nIndexSave,pDoc, 10);

	int m_totalCount=pDoc->m_sharesInformation.GetCount();
	long m_index=0;
    for(int i=0;i<11;i++)
	{
		int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			bool bSucc;
			Kline *pKline=NULL;
			CBuySellList l_BuySellList;
			pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;
            int GetDays=ReadKLine(StockId,i, pKline, nDays,nLKind[m_nIndexSave.iDataType],&l_BuySellList, 48 ,TRUE);
			Cdat->Index[this->m_nColumnId-9000]=FormularComputeShares(StockId,i,GetDays,&m_nIndexSave,bSucc,pKline, &l_BuySellList );

            if(!bSucc)
			{

			}
			if(pKline)
			{
			    delete pKline;      
				pKline=NULL;
			}
			CTaiKlineFileHS::RemoveHs(l_BuySellList);

			m_index++;
			int spos=100*m_index /m_totalCount   ;
			this->m_Progress.SetPos(spos);
	//**************************************************************
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
	}
}

void SortIndex::OnClose() 
{

	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

	CTaiScreenParent::OnClose();
}
void SortIndex::OnCancel()
{
	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}
	CDialog::OnCancel();
}
