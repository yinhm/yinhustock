// CTaiKlineTreeCtl.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineTreeCtl.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "DIAEDITZJXT.h"
#include "CTaiKlineDlgEditIndecator.h"
#include "CTaiKlineDlgEditScreenStock.h"
#include "DIAGSGLMM.h"
#include "CTaiKlineSetNameTreeGroup.h"
#include "CTaiShanKlineShowView.h"
#include "KEYBRODEANGEL.h"
#include "CTaiScreen.h"
#include "CTaiScreenTest.h"
#include "CTaiScreensIndicate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OEM_LT


static const CString g_chAllIndex = "所有技术指标";
static CString g_strFileDialogOutPath = "";

CTaiKlineTreeCtl::CTaiKlineTreeCtl()
{
	m_bShowFromMouse = false;
	m_hAllIndex=0;
	m_bNewGroup			=true;
	m_bDeleteGroup		=true;
	m_bModifyGroupName	=true;
	m_bDeleteEquation	=true;
	m_bNewEquation		=true;
	m_bModifyEquation	=true;
	m_bRefreshEquation  =false;

	m_bScreenStock			=false;
	m_bScreenStockTest		=false;
	m_bScreenStockIndicator =false;
	m_bShowIndicator		=false;
	m_bShowKlineGroup		=false;
#ifdef OEM_LT
	m_bNewEquation		=false;
	m_bModifyEquation	=false;
#endif
}

CTaiKlineTreeCtl::~CTaiKlineTreeCtl()
{
}


BEGIN_MESSAGE_MAP(CTaiKlineTreeCtl, CTaiKlineColorTreeCtrl)
	//{{AFX_MSG_MAP(CTaiKlineTreeCtl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CTaiKlineTreeCtl::OnDestroy() 
{
	CTaiKlineColorTreeCtrl::OnDestroy();
	

	
}

#define RGBSUB RGB(20,0,100)
#define RGBEND RGB(200,0,0)
#define RGBEND2 RGB(0,0,0)

void CTaiKlineTreeCtl::InsertEqutionTree( int nKind,bool bExtend ,int nInsertKind)
{
	Formu_Array1 *pList = NULL;

	int i;
	for(i = 0;i<m_nKindArray.GetSize();i++)
	{
		if(m_nKindArray[i] == nKind)
			break;
	}
	if(i == m_nKindArray.GetSize())
		m_nKindArray.Add(nKind);



	CString s;
	switch(nKind)
	{
	case 0 :
		pList = &CMainFrame::m_taiShanDoc->m_formuar_index;  
		s = "指标公式";
		break;
	case 1 :
		pList = &CMainFrame::m_taiShanDoc->m_formuar_choose;  
		s = "条件选股公式";
		break;
	case 2 :
		pList = &CMainFrame::m_taiShanDoc->m_formuar_kline;  
		s = "形态组合公式";
		break;
	case 3 :
		pList = &CMainFrame::m_taiShanDoc->m_formuar_master;  
		s = "交易系统公式";
		break;
	default :
		InsertEqutionTreeOther(  nKind,bExtend);
		return;
	}



	int nImageKind[5]={4,3,5,0,0};
	SetImageList(CMainFrame::m_taiShanDoc->m_imagelist ,TVSIL_NORMAL);

	HTREEITEM hChapter  = InsertItem(s,17,17,TVI_ROOT,TVI_LAST );
	LOGFONT logfont;
	CFont *pFont = GetFont();
	pFont->GetLogFont( &logfont );
	logfont.lfWeight = FW_BOLD;
	SetItemFont(hChapter, logfont );
	SetItemColor( hChapter, RGB(100,0,20));

	if(! SetItemData( hChapter, (DWORD) (-(nKind+1) )))
		ASSERT(FALSE);

	int nOutV=(int)GetItemData( hChapter);

	int nSubKind = CFormularContent::m_strArrayKind[nKind].GetSize ();

	if(nSubKind == 0)
	{
		CString s= "其它";
		CFormularContent::m_strArrayKind[nKind].Add(s);
		nSubKind++;
	}

	logfont.lfWeight = FW_REGULAR;
	if(nInsertKind >0 )
	{
		HTREEITEM hSub = hChapter;
		for(int j=0;j<pList->GetSize ();j++)
		{
			CFormularContent * pIndex= pList->GetAt(j);
			if(pIndex->isOfen ==TRUE || nInsertKind==1)
			{

				HTREEITEM hEnd = InsertItem(pIndex->name + " (" +pIndex->subKindIndex +") " + pIndex->explainBrief ,nImageKind[nKind],nImageKind[nKind],hSub);
				SetItemFont(hEnd, logfont );
				if(pIndex->isOfen ==TRUE) SetItemColor( hEnd, RGBEND);
				else SetItemColor( hEnd, RGBEND2);
				if(! SetItemData( hEnd, (DWORD) pIndex ))
						ASSERT(FALSE);
			}
		}
		return ;
	}

	i=0;
	int j=0;
	if(nKind ==0)
	{


	}
	HTREEITEM* hSub = new HTREEITEM[nSubKind+1];
	for(i=0;i<nSubKind;i++)
	{
		hSub[i] = InsertItem(CFormularContent::m_strArrayKind[nKind][i],16,16,hChapter);
		SetItemFont(hSub[i], logfont );
		SetItemColor( hSub[i], RGBSUB);
		if(! SetItemData( hSub[i], (DWORD) (0 )))
			ASSERT(FALSE);
	}
	for(j=0;j<pList->GetSize ();j++)
	{
		CFormularContent * pIndex= pList->GetAt(j);
		if(pIndex->subKindIndex == "")
			pIndex->subKindIndex = "其它";


		for(i=0;i<nSubKind;i++)
		{
			if(CFormularContent::m_strArrayKind[nKind][i] == pIndex->subKindIndex)
				break;
		}
		if(i>=nSubKind) i=nSubKind-1; 

		CString s ="";


		HTREEITEM hEnd = InsertItem(pIndex->name + " " +s+ pIndex->explainBrief ,nImageKind[nKind],nImageKind[nKind],hSub[i]);
		SetItemFont(hEnd, logfont );
		if(pIndex->isOfen )	SetItemColor( hEnd, RGBEND);
		else SetItemColor( hEnd, RGBEND2);
		if(! SetItemData( hEnd, (DWORD) pIndex ))
				ASSERT(FALSE);
	}
	delete [] hSub;

	if(bExtend ==true)
		Expand(hChapter,TVE_EXPAND);
}

//

bool CTaiKlineTreeCtl::GetCurrentItemInfo(int &nKindRoot, int &nKindItem, CFormularContent *&pIndex)//

{
	nKindRoot = 0;
	nKindItem = -1;
	pIndex = NULL;

	HTREEITEM hItem = GetSelectedItem();

	HTREEITEM hRoot = hItem;//?
	nKindRoot =(int) GetItemData(  hRoot ) ;
	if(nKindRoot>=0 || nKindRoot<-100)
	{
		while(true)
		{
			hRoot=GetParentItem(hRoot);
			if(hRoot == NULL)
				break;
			nKindRoot =(int) GetItemData(  hRoot ) ;
			if(nKindRoot<0&& nKindRoot>-100)
				break;
		}
	}

	int nData = (int) GetItemData(  hItem ) ;
	if(nData ==0)
		nKindItem = 0;
	else if(!ItemHasChildren(hItem) && nData>0 && (nKindRoot>=-4&&nKindRoot<=-1 ||nKindRoot == -6))
		pIndex =( CFormularContent *)nData;
	return true;
}

bool CTaiKlineTreeCtl::CreateNewGroup()
{
	CString sName = "新组";

	CTaiKlineSetNameTreeGroup setName;
	setName.m_bShowFromMouse = m_bShowFromMouse ;
	if(setName.DoModal () == IDCANCEL)	
		return false;

	//end
	sName = setName.m_sName ;

	HTREEITEM hItem = GetSelectedItem();
	
	if(FindGroupQiTa(hItem, sName)!=NULL)
	{
		AfxMessageBox("此组名已经存在,请重新命名!");
		return false;
	}
	//end


	int nKindRoot = GetItemData(hItem);
	if(nKindRoot<=-1 && nKindRoot>=-3)
		CFormularContent::AddKindName(sName  , -nKindRoot-1);

	HTREEITEM hSub = InsertItem(sName,16,16,hItem);
	if(! SetItemData( hSub, (DWORD) (0 )))
		ASSERT(FALSE);
	SortChildren(  hItem );
	return true;
}

bool CTaiKlineTreeCtl::DeleteGroup()
{
	if(AfxMessageBox("是否真的删除此公式组及属于该组的所有公式？",MB_YESNO )==IDYES)
	{
	HTREEITEM hItem = GetSelectedItem();

	int nRtn = 1;
	if(ItemHasChildren( hItem ))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = GetChildItem(hItem);

	   while (hChildItem != NULL && nRtn!=2)
	   {
		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  if(hChildItem!=NULL)
		  {
				nRtn = DeleteEquation(hChildItem);
		  }
				
		  
	
		  hChildItem = hNextItem;
	   }
	}

	if(nRtn ==2) return false;
	CString s = GetItemText(hItem);
	if(s == "其它") return true;
	HTREEITEM hItemP = this->GetParentItem (hItem);

	int nKindRoot = GetItemData(hItemP);
	if(nKindRoot<=-1 && nKindRoot>=-3)
	{
		CFormularContent::DeleteKindName(s, -nKindRoot-1);
	}
	DeleteItem( hItem );
	return true;
	}
	return false;
}

bool CTaiKlineTreeCtl::ModifyGroupName()
{
	HTREEITEM hItem = GetSelectedItem();
	int nData = (int) GetItemData(  hItem ) ;
	ASSERT(nData == 0);
	CString sName = GetItemText(  hItem ) ;

	CTaiKlineSetNameTreeGroup setName;
	setName.m_bShowFromMouse = m_bShowFromMouse ;
	setName.m_sName = sName;
	CString sOld = sName;
	if(setName.DoModal () == IDCANCEL)	
		return false;


	HTREEITEM hItemP = this->GetParentItem (hItem);
	sName = setName.m_sName ;
	if(FindGroupQiTa(hItemP, sName)!=NULL)
	{
		AfxMessageBox("此组名已经存在,请重新命名!");
		return false;
	}

	this->SetItemText (hItem,sName);


	int nKindRoot = GetItemData(hItemP);
	if(nKindRoot<=-1 && nKindRoot>=-3)
	{
		CFormularContent::DeleteKindName(sOld, -nKindRoot-1);
		CFormularContent::AddKindName(sName  , -nKindRoot-1);
	}


	if (ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
	    HTREEITEM hChildItem =GetChildItem(hItem);
	    while (hChildItem != NULL)   
		{
		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  int dwData = (int)GetItemData(hChildItem); 
		  CFormularContent* pJi = GetLJishuFromInt(dwData);
		  if(pJi!=NULL)
		  {
			  pJi->subKindIndex = sName;
		  }
		  
		  hChildItem = hNextItem;   
		}
	}

	SortChildren(  hItemP );
	
	return true;
}

bool CTaiKlineTreeCtl::CreateNewEquation()
{
#ifdef OEM_LT

#endif

	bool bSucc = false;
	CString sName = "";
	CFormularContent * pJishu = NULL;
	CFormularContent * pJishuCurrent = NULL;


	int nKindRoot,  nKindItem;
	GetCurrentItemInfo(nKindRoot, nKindItem, pJishuCurrent);//
	HTREEITEM hItem = GetSelectedItem();

	ASSERT(nKindRoot>=-4&&nKindRoot<=-1);

	CString *strP = NULL;
	CString s = GetItemText(hItem);
	if(nKindItem == 0)
		strP = &s;
	if(pJishuCurrent)
		strP = &(pJishuCurrent->subKindIndex) ;
	pJishu = ShowEditIndicatorDlg( NULL, -1, -nKindRoot-1,strP);

	if(pJishu == NULL)
		return false;

	if(pJishu->subKindIndex == "")
		pJishu->subKindIndex = "其它";


	pJishu->AddKindName(pJishu->subKindIndex  , -nKindRoot-1);

	hItem = GetSelectedItem();
	if(hItem == NULL)
		return false;


	if(pJishuCurrent != NULL)
		hItem = GetParentItem(hItem);
	else if(nKindItem !=0)
		hItem = FindGroupQiTa(hItem);
	

	if(hItem == NULL ||-nKindRoot-1<0 ||-nKindRoot-1>4)
		return false;

	int nImageKind[5]={4,3,5,0,0};
	sName=pJishu ->name +" "+pJishu ->explainBrief ;
	HTREEITEM hSub = InsertItem(sName,nImageKind[-nKindRoot-1] ,nImageKind[-nKindRoot-1] ,hItem);
	if(hSub == NULL)
		return false;

	if(! SetItemData( hSub, (DWORD) (pJishu )))
		ASSERT(FALSE);
	SortChildren(  hItem );

	return true;

}
int CTaiKlineTreeCtl::DeleteEquation(HTREEITEM hItem)
{
	bool bNULL = false;
	if(hItem == NULL)
	{
		bNULL = true;
		hItem = GetSelectedItem();
	}
	int nData = (int) GetItemData(  hItem ) ;
	ASSERT(nData>0);


	CFormularContent * pJishu = NULL;
	CFormularContent * pJishuCurrent = NULL;
	int nKindRoot,  nKindItem;
	GetCurrentItemInfo(nKindRoot, nKindItem, pJishuCurrent);//
	if(bNULL == false)
	{
		pJishuCurrent = (CFormularContent * )nData;
	}
	ASSERT(nKindRoot>=-4&&nKindRoot<=-1);
	if(pJishuCurrent == NULL)
		return 0;
	if(pJishuCurrent->isSystem )
	{
		AfxMessageBox("不能删除系统指标！");
		return 2;
	}

	if(bNULL == true)
		if(AfxMessageBox("是否真的删除公式？",MB_YESNO) != IDYES)
			return 0;

	int nFoot = GetFoot(-nKindRoot-1,pJishuCurrent,true);
	ASSERT(nFoot == -1);


	DeleteItem( hItem );



	return 1;

}

bool CTaiKlineTreeCtl::ModifyEquation()
{
#ifdef OEM_LT

#endif
	int flag = IDCANCEL; 

	int nKindRoot,  nKindItem;
	CFormularContent * pJishu = NULL;
	CFormularContent * pJishuCurrent = NULL;
	GetCurrentItemInfo(nKindRoot, nKindItem, pJishuCurrent);//
	ASSERT(nKindRoot>=-4&&nKindRoot<=-1);

	int listitem = GetFoot(-nKindRoot-1, pJishuCurrent);
	if(listitem == -1)
		return false;

	if(pJishuCurrent ==NULL)
		return false;

	int nKlineType = -1;
	if(CMainFrame::m_taiShanDoc->m_lmbViewArray .GetSize ()>0)
		nKlineType = CMainFrame::m_taiShanDoc->m_lmbViewArray[0]->m_nKlineKind2;

	int nTest = 3322;
	if(pJishuCurrent->sellStr != "")
		nTest*=9;
	int kk = 0;
	kk*=3;
	if(nTest == 3322*9)
		return false;

	pJishu = ShowEditIndicatorDlg(pJishuCurrent,  listitem,-nKindRoot-1,NULL,nKlineType);

	if(pJishu == NULL )
		return false;


	pJishu->AddKindName(pJishu->subKindIndex  , -nKindRoot-1);

	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)
		return false;

	CString sName = pJishu->name + " " + pJishu->explainBrief ;
	BOOL b = SetItemText(hItem,sName);
	if(b == FALSE)
		return false;
	if(! SetItemData( hItem, (DWORD) (pJishu )))
		ASSERT(FALSE);
	
	hItem = GetParentItem(hItem);
	if(hItem != NULL )
	SortChildren(  hItem );

	CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;
	CFormularContent::ReadWriteIndex(pDoc, 0, false);
	CFormularContent::ReadWriteIndex(pDoc, 1, false);
	CFormularContent::ReadWriteIndex(pDoc, 2, false);
	return true;
}


HTREEITEM CTaiKlineTreeCtl::FindGroupQiTa(HTREEITEM hRoot,CString sName)
{
	if (ItemHasChildren(hRoot))
	{
		HTREEITEM hNextItem;
	    HTREEITEM hChildItem =GetChildItem(hRoot);
	    while (hChildItem != NULL)   
		{
		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  CString s = GetItemText(hChildItem);  
		  if(s ==sName)
		  {
			  if(GetItemData (hChildItem) == 0)
				  return hChildItem;
		  }
		  hChildItem = hNextItem;   
		}
	}
	return NULL;

}

int CTaiKlineTreeCtl::GetFoot(int nKind, CFormularContent *pJishu,bool bDoRemove)
{
	int i,nRtn = -1;
	Formu_Array1 *pList[4];

	pList[0] = &CMainFrame::m_taiShanDoc->m_formuar_index;
	pList[1] = &CMainFrame::m_taiShanDoc->m_formuar_choose;
	pList[2] = &CMainFrame::m_taiShanDoc->m_formuar_kline;

	for( i=0;i<pList[nKind]->GetSize();i++)
	{
		if(pJishu==pList[nKind]->GetAt(i))
		{
			if(bDoRemove == true)
			{
				pList[nKind]->RemoveAt(i);
				delete pJishu;
			}
			else
				nRtn = i;
		}
	}

	return nRtn;
}

void CTaiKlineTreeCtl::InsertEqutionTreeOther(int nKind,bool bExtend)
{
	CString s;
	switch(nKind)
	{
	case -6 :
		s = "基本面选股";
		break;
	case -7 :
		s = "价格预警";
		break;
	case -8 :
		s = "涨跌幅预警";
		break;
	case -9 :
		s = "换手率预警";
		break;

	}

	
	int idb[7]={20,12,14,15,0,0,0};
	SetImageList(CMainFrame::m_taiShanDoc->m_imagelist ,TVSIL_NORMAL);

	HTREEITEM hChapter  = InsertItem(s,idb[-nKind-6],idb[-nKind-6]);

	
	LOGFONT logfont;
	CFont *pFont = GetFont();
	pFont->GetLogFont( &logfont );
	logfont.lfWeight = FW_BOLD;
	SetItemFont(hChapter, logfont );
	SetItemColor( hChapter, RGB(100,0,20));
	if(! SetItemData( hChapter, (DWORD) (nKind) ))
		ASSERT(FALSE);
	if(bExtend ==true)
		Expand(hChapter,TVE_EXPAND);

}

void CTaiKlineTreeCtl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{

	UINT uFlags;
	CPoint pt;
	::GetCursorPos (&pt);
	CPoint ptOld(pt);
	ScreenToClient (&pt);
	HTREEITEM hItem = HitTest(pt, &uFlags);
	if ((hItem != NULL) )
	{
	    SelectItem(hItem);
	}

	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	if(nKindItem == 0 || pIndex==NULL)
		if(sNameItem == g_chAllIndex )
			return;

	CMenu menu;
	menu.CreatePopupMenu( );

	if(pIndex != NULL)
	{
		if(nKindRoot == -3)
		{
			if(m_bShowKlineGroup == true)
			{
				menu.AppendMenu(MF_STRING | MF_ENABLED, nShowKlineGroup, strShowKlineGroup);
			}
		}
		if(m_bScreenStock	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nScreenStock, strScreenStock);
		if(m_bScreenStockTest	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nScreenStockTest, strScreenStockTest);
		if(m_bScreenStockIndicator	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nScreenStockIndicator, strScreenStockIndicator);

		if(nKindRoot == -1)
		{
			if(m_bShowIndicator	==true)
			menu.AppendMenu(MF_STRING | MF_ENABLED, nShowIndicator, strShowIndicator);
		}

		if(m_bModifyEquation	==true)
		{
			if(pIndex->sellStr == "")
				menu.AppendMenu(MF_STRING | MF_ENABLED, nModifyEquation, strModifyEquation);
		}
		if(m_bDeleteEquation	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nDeleteEquation, strDeleteEquation);

	}
	else if(nKindItem == 0)
	{
		if(m_bDeleteGroup	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nDeleteEquationGroup, strDeleteEquationGroup);
		if(m_bModifyGroupName	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nChangeNameGroup, strChangeNameGroup);
		if(m_bNewEquation	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nNewEquation, strNewEquation);

	}
	else if(nKindRoot<=-1 && nKindRoot>=-4)
	{
		if(m_bNewGroup	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nNewEquationGroup, strNewEquationGroup);
		if(m_bRefreshEquation)
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING | MF_ENABLED, nGroupEquation, strGroupEquation);
			menu.AppendMenu(MF_STRING | MF_ENABLED, nAllEquation, strAllEquation);
			menu.AppendMenu(MF_STRING | MF_ENABLED, nOftenEquation, strOftenEquation);
		}
	}
	else
		ASSERT(FALSE);

	(menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			ptOld.x,ptOld.y,this, NULL));

	*pResult = 0;
}

BOOL CTaiKlineTreeCtl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{


	bool bRefresh = false;
	if(nCode!=0)
		return CTaiKlineColorTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	m_bShowFromMouse = true;
	switch(nID)
	{
	case nNewEquationGroup:
		 CreateNewGroup ();
		 bRefresh = true;
		break;
	case nDeleteEquationGroup:
		 DeleteGroup ();
		 bRefresh = true;
		break;
	case nChangeNameGroup:
		 ModifyGroupName ();
		 bRefresh = true;
		break;
	case nNewEquation:
		 CreateNewEquation ();
		 bRefresh = true;
		break;
	case nModifyEquation:
		 ModifyEquation ();
		break;
	case nDeleteEquation:
		 DeleteEquation ();
		 bRefresh = true;
		break;
	case nGroupEquation:
	case nAllEquation:
	case nOftenEquation:
		ReInsertIterm(nID);
		break;
	case nScreenStock:
		CommandScreenStock();
		break;
	case nScreenStockTest:
		break;
	case nScreenStockIndicator:
		CommandScreenStockIndicator();
		break;
	case nShowIndicator:
		CommandShowIndicator();
		break;
	case nShowKlineGroup:
		CommandShowKlineGroup();
		break;
	default:
		m_bShowFromMouse = false;
		return CTaiKlineColorTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	if(	bRefresh == true)
	{
		((CMainFrame*)AfxGetMainWnd())->RefreshFormu();
		CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;
		CFormularContent::ReadWriteIndex(pDoc, 0, false);
		CFormularContent::ReadWriteIndex(pDoc, 1, false);
		CFormularContent::ReadWriteIndex(pDoc, 2, false);
	}
	m_bShowFromMouse = false;

	return TRUE;
}

CFormularContent * CTaiKlineTreeCtl::ShowEditIndicatorDlg(CFormularContent *pJishuCurrent, int listitem,int nKindRoot,CString * sGroup,int nKlineType)
{
	int flag=0;
	CFormularContent * pJishu = NULL;
	CTaiKlineDlgEditIndecator  diaeditzbgs;
	diaeditzbgs.m_nKlineType = nKlineType;
	CTaiKlineDlgEditScreenStock  diaedittjxg(IDD_6_EDIT_TJXG_KMODE);

	diaedittjxg.m_nKlineType = nKlineType;

	diaeditzbgs.item=listitem;
	diaedittjxg.item=listitem;
	if(pJishuCurrent!=NULL)
	{
	diaeditzbgs.m_sKindFormu = pJishuCurrent->subKindIndex ;
	diaedittjxg.m_sKindFormu = pJishuCurrent->subKindIndex ;
	}
	else if(sGroup!=NULL)
	{
		CString s = g_chAllIndex;
		if(s.CompareNoCase (*sGroup) != 0)
		{
			diaeditzbgs.m_sKindFormu = *sGroup;
			diaedittjxg.m_sKindFormu = *sGroup;
		}
	}
	
	switch(nKindRoot)
	{
	case 0:
		 diaeditzbgs.iskline=2;
		 if(pJishuCurrent!=NULL)
		 {
			if(pJishuCurrent->isSystem)
			{
				if(AfxMessageBox("这是一个系统指标，\r修改它的参数或算法\r会改变系统的缺省值，\r还要继续吗？",MB_YESNO |MB_ICONQUESTION)==IDNO)
				break;
			}
										 
			 if(pJishuCurrent->isProtected)
			{
				CDialogGSGLMM mdiagsglmm;
				mdiagsglmm.password=pJishuCurrent->password;
				if(mdiagsglmm.DoModal()==IDOK)
				{
					flag=diaeditzbgs.DoModal();
				
				}
			}
			 else
					flag=diaeditzbgs.DoModal();
		 }
		else  
			flag=diaeditzbgs.DoModal();

		if(flag==IDOK)
		{
			pJishu = diaeditzbgs.jishunow ;

		
			if(pJishu!=NULL)
			{
				CString name=pJishu->name ;
				CTaiKeyBoardAngelDlg dlg;
				dlg.AddIndicatorName(name);
			}
		}
	break;
	case 1:     
		 diaedittjxg.iskline=false;
		 if(pJishuCurrent!=NULL)
		 {
			 if(pJishuCurrent->isProtected)
			{
				CDialogGSGLMM mdiagsglmm;
				mdiagsglmm.password=pJishuCurrent->password;
				if(mdiagsglmm.DoModal()==IDOK)
				{
					flag=diaedittjxg.DoModal();
				}
			}
			 else
					flag=diaedittjxg.DoModal();
		 }
			
		else  
			flag=diaedittjxg.DoModal();

		if(flag==IDOK)
			pJishu = diaedittjxg.jishunow ;

	break;
	case 2:
		diaedittjxg.iskline=true;
		 if(pJishuCurrent!=NULL)
		 {
			if(pJishuCurrent->isProtected)
			{
				CDialogGSGLMM mdiagsglmm;
				mdiagsglmm.password=pJishuCurrent->password;
				if(mdiagsglmm.DoModal()==IDOK)
				{
					flag=diaedittjxg.DoModal();
				}
			}
			 else
					flag=diaedittjxg.DoModal();
		 }
		else  
			flag=diaedittjxg.DoModal();
		if(flag==IDOK)
			pJishu = diaedittjxg.jishunow ;
		break;
	}

	return pJishu;

}

CFormularContent* CTaiKlineTreeCtl::GetLJishuFromInt(int nData)
{
	CFormularContent* pJi = NULL;
	if(nData>0 || nData<-100)
		pJi = (CFormularContent*)nData;
	return pJi;

}

int CTaiKlineTreeCtl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTaiKlineColorTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	


	return 0;
}

void CTaiKlineTreeCtl::ReInsertIterm(int nID)
{
	int n = 0;
	if(nID == nAllEquation)
		n = 1;
	else if(nID == nOftenEquation)
		n = 2;
	ReInsertIterm2(n);

}

void CTaiKlineTreeCtl::SelectItemJ(CFormularContent *p)
{
	if(p == NULL) return;
	HTREEITEM root = GetRootItem( );

	while(root)
	{
		int  i= 0;
		i = GetItemData(root);
		if( (i==-1 || i==-2 || i==-3)) 
		{
			HTREEITEM h = RecursiveLook(p,  root);
			if(h)
			{
				SelectItem(h);
				break;
			}
		}
		root = GetNextItem(root, TVGN_NEXT     );
	}
	return ;

}

HTREEITEM CTaiKlineTreeCtl::RecursiveLook(CFormularContent *p, HTREEITEM root)
{
	if (ItemHasChildren(root))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = GetChildItem(root);

	   while (hChildItem != NULL)
	   {
			DWORD dw = GetItemData(hChildItem);
		   if(dw == (DWORD)p)
		   {
			   return hChildItem;
		   }

		   HTREEITEM hrtn = RecursiveLook(p,  hChildItem);
		   if(hrtn) 
			   return hrtn;

		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  hChildItem = hNextItem;
	   }
	}
	return NULL;

}

CTaiShanKlineShowView* CTaiKlineTreeCtl::GetTargetView()
{
	CTaiShanKlineShowView* pView = NULL;
   CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

   CView *pView2 = pchildfram->GetActiveView();
   if(pView2 != NULL)
   {
	   if(!pView2->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	   {
		   pView = NULL;
	   }
	   else
		   pView = (CTaiShanKlineShowView* )pView2;
   }

   return pView;
}

void CTaiKlineTreeCtl::CommandShowKlineGroup()
{
	CTaiShanKlineShowView * pWnd = CTaiKlineTreeCtl::GetTargetView();
	if(!pWnd) return;
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	if(nKindItem == 0 || pIndex==NULL)
		return;

	if(nKindRoot<-3 ||nKindRoot>-1)
		return;

	pWnd->ShowKlineMode(pIndex);
}
void CTaiKlineTreeCtl::CommandScreenStock()
{
	HTREEITEM hItem = GetSelectedItem ();
	if(hItem == NULL )
	{
	  return;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	if(nKindItem == 0 || pIndex==NULL)
		return;

	if(nKindRoot<-3 ||nKindRoot>-1)
		return;


	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CTaiScreen* pDlg = pMain->ShowTjxg();
	pDlg->m_treeFormularCtrl.SelectItemJ(pIndex);
}

void CTaiKlineTreeCtl::CommandScreenStockIndicator()
{
	CTaiShanKlineShowView * pWnd = CTaiKlineTreeCtl::GetTargetView();
	if(!pWnd) return;
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	if(nKindItem == 0 || pIndex==NULL)
		return;

	if(nKindRoot<-3 ||nKindRoot>-1)
		return;

	pWnd->ShowMark(pIndex);

}

void CTaiKlineTreeCtl::CommandShowIndicator()
{
	HTREEITEM hItem = GetSelectedItem ();
	if(hItem == NULL )
	{
	  return;
	}

	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	if(nKindItem == 0 || pIndex==NULL)
		return;


	if(nKindRoot != CTaiKlineTreeCtl::nKindIndex)
		return;

	CTaiShanKlineShowView * pWnd = CTaiKlineTreeCtl::GetTargetView();
	if(pWnd)
	{
		(pWnd)->DrawZhibiao(pIndex->name ,-1);
	}


}

void CTaiKlineTreeCtl::ReInsertIterm2(int n)
{
	DeleteAllItems();
	for(int i = 0;i<this->m_nKindArray.GetSize();i++)
	{
		this->InsertEqutionTree(m_nKindArray[i],false,n);
	}


	HTREEITEM hroot = GetRootItem( );
	if(hroot) SelectItem( hroot );
}
bool CTaiKlineTreeCtl::CanBeginDrag()
{
	UINT uFlags;
	HTREEITEM hItem = GetSelectedItem();
	if ((hItem == NULL) )
	{
	    return false;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	
	if(pIndex!=NULL && (nKindRoot>=-3&&nKindRoot<=-1))
		return true;
	return false;
}
bool CTaiKlineTreeCtl::CanDroped(HTREEITEM	hitem)
{
	UINT uFlags;
	HTREEITEM hItem = GetSelectedItem();
	if ((hItem == NULL) )
	{
	    return false;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	
	HTREEITEM hRoot;
	hRoot=GetParentItem(hItem);
	if(hRoot == NULL)
		return false;
	if(nKindItem == 0 && pIndex==NULL && (nKindRoot>=-3&&nKindRoot<=-1))
		return true;
	return false;
}
void CTaiKlineTreeCtl::DoDropOut(CWnd* pWnd,HTREEITEM hItem)
{
	UINT uFlags;
	BOOL b = SelectItem(hItem);
	if ((hItem == NULL) || !b )
	{
	    return ;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	
	if(pIndex!=NULL && (nKindRoot>=-3&&nKindRoot<=-1))
	{
		if(nKindRoot == -1)
		{
			((CTaiShanKlineShowView *)pWnd)->DrawZhibiao(pIndex->name ,-2);
		}
		else
		{
			((CTaiShanKlineShowView *)pWnd)->ShowMark(pIndex);
		}
	}
}
void CTaiKlineTreeCtl::DoDropedSelf(HTREEITEM hItem)
{
	int i = 0;
}

void CTaiKlineTreeCtl::ExportFormular(BOOL bCompleteSecret, BOOL bPassword, CString strPassword)
{
	Formu_Array1 arr[3];
	HTREEITEM root = GetRootItem( );

	while(root)
	{
		int  i= 0;
		i = GetItemData(root);
		if( (i==-1 || i==-2 || i==-3)) 
		{
			RecursiveAddChecked(arr[-i-1], root,true);
		}
		root = GetNextItem(root, TVGN_NEXT     );
	}

	int nTotal = 0;
	for(int i = 0;i<3;i++)
	{
		nTotal+=arr[i].GetSize ();
	}
	if(nTotal==0)
		return;

	CString fileName = "";

	CString defaultname="*.xj";  
	CString     defaultname2 = defaultname+"||";

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,defaultname2);
	if(g_strFileDialogOutPath == "")
		m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
	else
		m_filedia.m_ofn .lpstrInitialDir = g_strFileDialogOutPath;
	                                    //
	if(m_filedia.DoModal()==IDOK)
	{
		CString filename=m_filedia.GetPathName();
		CString ss2 = m_filedia.GetFileName();
		CString sPath = filename;
		sPath.Delete(filename.GetLength()-ss2.GetLength(),ss2.GetLength());
		g_strFileDialogOutPath = sPath;

		if(filename.Find('.') == -1)
		{
	      CString temp = defaultname;
		  temp.TrimLeft ("*");
		  filename+=temp;
		}
		fileName = filename;

		CFormularContent::ExportFormular(&arr[0], 3, fileName, bCompleteSecret,  bPassword,  strPassword);
	}

}

void CTaiKlineTreeCtl::RecursiveAddChecked(Formu_Array1 &arr, HTREEITEM root,bool bTestCheck)
{
	if (ItemHasChildren(root))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = GetChildItem(root);
		if(GetCheck(root))
			bTestCheck = false;

	   while (hChildItem != NULL)
	   {

		   {

		   }

		   RecursiveAddChecked(arr,  hChildItem,bTestCheck);

		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  hChildItem = hNextItem;
	   }
	}
	else
	{
		DWORD dw = GetItemData(root);
		if(dw > 0 )
		{
			if(!bTestCheck || GetCheck(root))
				arr.Add((CFormularContent*)dw);
		}
	}

}





















