// SHEETZX.cpp : implementation file
//
//

#include "stdafx.h"
#include "SHEETZX.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "KEYBRODEANGEL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetSelfBlockPage property page

IMPLEMENT_DYNCREATE(CTaiSysSetSelfBlockPage, CPropertyPage)

CTaiSysSetSelfBlockPage::CTaiSysSetSelfBlockPage() : CPropertyPage(CTaiSysSetSelfBlockPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiSysSetSelfBlockPage)
	m_strChooseStatus = _T("");
	//}}AFX_DATA_INIT
	m_psp.dwFlags |= PSH_HASHELP;
}

CTaiSysSetSelfBlockPage::~CTaiSysSetSelfBlockPage()
{
}

void CTaiSysSetSelfBlockPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiSysSetSelfBlockPage)
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_LIST3, m_ctrlChoosedStock);
	DDX_Control(pDX, IDC_LIST1, m_ctrlStockName);
	DDX_Text(pDX, IDC_STATIC_OUTPUT, m_strChooseStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaiSysSetSelfBlockPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiSysSetSelfBlockPage)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, OnSetfocusList1)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST3, OnSetfocusList3)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, OnDblclkList3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetSelfBlockPage message handlers

BOOL CTaiSysSetSelfBlockPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	InitListCtrl();
	updatechooselist(-1);
	int totalstockcount=m_ctrlChoosedStock.GetItemCount();
	m_strChooseStatus.Format("可用键盘精灵选择，自选股里一共有%d个股票",totalstockcount);
	m_ctrlStockName.SetFocus();
	UpdateData(FALSE);
	return FALSE;  
}

void CTaiSysSetSelfBlockPage::OnAdd() 
{
	CTaiShanDoc *pDoc=GetDocument();
    POSITION posSelected=m_ctrlStockName.GetFirstSelectedItemPosition();
    if(posSelected==NULL)
	{
		AfxMessageBox("请至少选择一个股票",MB_ICONASTERISK);
		return;
	}
	int nIndex,nDefIndex=0;
	int nSelectedCount=m_ctrlStockName.GetSelectedCount();
	int nTotal=pDoc->m_choosesymbol.GetCount();

	for(int i=0;i<nSelectedCount;i++)
	{
		BOOL  bFind=FALSE;  
		nIndex=m_ctrlStockName.GetNextSelectedItem(posSelected);
		CString strStockCode=GetStockCode(nIndex);
		if(strStockCode.IsEmpty())continue;

		SharesSymbol *addto =new SharesSymbol;
		strcpy(addto->sharesymbol,strStockCode);

		if(nTotal==0)             
		{
			m_MapChoose[strStockCode]=strStockCode;
			pDoc->m_choosesymbol.AddHead(addto);
			nDefIndex=0;
		}
		else
		{
			POSITION nPos=pDoc->m_choosesymbol.GetHeadPosition();
			for(int j=0;j<nTotal;j++)
			{
				char *serch=pDoc->m_choosesymbol.GetNext(nPos)->sharesymbol;
				if(strcmp(strStockCode,serch)==0)
				{ 
					bFind=TRUE;
					break;
				}				
			}
			if(bFind)
			{
				if(nSelectedCount==1)
				{
					AfxMessageBox((LPCTSTR)"你要加入的股票自选股中已存在，请重新选择。",MB_ICONSTOP);
					return;
				}
			}
			else
			{
				m_MapChoose[strStockCode]=strStockCode;		
				pDoc->m_choosesymbol.AddTail(addto);
				nDefIndex=pDoc->m_choosesymbol.GetCount()-1;
			}
		}
	}
	updatechooselist(nDefIndex);
	m_ctrlChoosedStock.SetFocus();
   	int totalstockcount=m_ctrlChoosedStock.GetItemCount();
	m_strChooseStatus.Format("可用键盘精灵选择，自选股里一共有%d个股票",totalstockcount);
	UpdateData(FALSE);
}

void CTaiSysSetSelfBlockPage::OnDelete() 
{

	CTaiShanDoc*   pDoc=GetDocument();
    POSITION  posSelected=m_ctrlChoosedStock.GetFirstSelectedItemPosition();
	if(posSelected==NULL)
	{
		AfxMessageBox("请至少选择一个股票",MB_ICONASTERISK);
		return;
	}
	int nSelectedCount=m_ctrlChoosedStock.GetSelectedCount(),nDefItem=0;
	CString strStockCode;
	int nTotal;
	for(int i=0;i<nSelectedCount;i++)
	{
		nTotal=pDoc->m_choosesymbol.GetCount();
  		int item=m_ctrlChoosedStock.GetNextSelectedItem(posSelected)-i;
		if(item==nTotal-1)
		{
			 strStockCode=pDoc->m_choosesymbol.GetTail()->sharesymbol;
			 pDoc->m_choosesymbol.RemoveTail();
			 nDefItem=nTotal-2;
		}
		else
		{
			POSITION nPos=pDoc->m_choosesymbol.GetHeadPosition();
			for(int j=0;j<item;j++)
				pDoc->m_choosesymbol.GetNext(nPos);
			strStockCode=pDoc->m_choosesymbol.GetAt(nPos)->sharesymbol;
			pDoc->m_choosesymbol.RemoveAt(nPos);
			nDefItem=item;
		}
		m_MapChoose.RemoveKey(strStockCode);
	}
	updatechooselist(nDefItem);
	m_ctrlChoosedStock.SetFocus();
	int totalstockcount=m_ctrlChoosedStock.GetItemCount();
	m_strChooseStatus.Format("可用键盘精灵选择，自选股里一共有%d个股票",totalstockcount);
	UpdateData(FALSE);
}

void CTaiSysSetSelfBlockPage::OnSetfocusList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

    if(m_btnDelete.IsWindowEnabled())
			m_btnDelete.EnableWindow(FALSE);
	if(!m_btnAdd.IsWindowEnabled())
			m_btnAdd.EnableWindow(TRUE);
	*pResult = 0;
}

void CTaiSysSetSelfBlockPage::OnSetfocusList3(NMHDR* pNMHDR, LRESULT* pResult) 
{

	if(m_btnAdd.IsWindowEnabled())
			m_btnAdd.EnableWindow(FALSE);
	if(!m_btnDelete.IsWindowEnabled())
			m_btnDelete.EnableWindow(TRUE);
	*pResult = 0;
}

void CTaiSysSetSelfBlockPage::updatechooselist(int nDefItem)
{
	CTaiShanDoc *pDoc=GetDocument();	
	int i=0;
	int nChoosedStock=pDoc->m_choosesymbol.GetCount();
	POSITION positionnow=pDoc->m_choosesymbol.GetHeadPosition();
	if(m_ctrlChoosedStock.GetItemCount()>0)
		m_ctrlChoosedStock.DeleteAllItems();
    for(i=0;i<nChoosedStock;i++)
	{
		char *strStockCode=pDoc->m_choosesymbol.GetAt(positionnow)->sharesymbol;
		CReportData *pDat1=NULL;
		pDoc->m_sharesInformation.Lookup(strStockCode,pDat1);
		if(!pDat1)continue;
		if(i!=nDefItem)
			m_ctrlChoosedStock.InsertItem(i,pDat1->name,0);
		else
		{
			LV_ITEM SelectedItem;
			SelectedItem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
			SelectedItem.iSubItem=0;
			SelectedItem.iItem=nDefItem;
			SelectedItem.iImage=0;
			SelectedItem.state=LVIS_FOCUSED |LVIS_SELECTED;
			CString namestr=pDat1->name;
			SelectedItem.pszText=namestr.GetBuffer(namestr.GetLength());
			m_ctrlChoosedStock.InsertItem(&SelectedItem);
		}
		pDoc->m_choosesymbol.GetNext(positionnow);
	}
	m_ctrlChoosedStock.EnsureVisible(nDefItem,FALSE);
}

void CTaiSysSetSelfBlockPage::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

    POSITION  nPos=m_ctrlStockName.GetFirstSelectedItemPosition();
    if(nPos!=NULL)
		OnAdd();
	*pResult = 0;
}

void CTaiSysSetSelfBlockPage::OnDblclkList3(NMHDR* pNMHDR, LRESULT* pResult) 
{

 
	POSITION  nPos=m_ctrlChoosedStock.GetFirstSelectedItemPosition();
    if(nPos!=NULL)
		OnDelete();
	*pResult = 0;
}

BOOL CTaiSysSetSelfBlockPage::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_CHAR)
	{
	   	keyinput=pMsg->wParam;
		if(keyinput>='a'&&keyinput<='z')
			keyinput-=32;
		if((keyinput>='0'&&keyinput<='9')|(keyinput>='A'&&keyinput<='Z'))
		{
			DoKeyShow();	
			return TRUE;
		}
	}	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CTaiSysSetSelfBlockPage::DoKeyShow()
{	
	CTaiShanDoc *pDoc=GetDocument();
	CTaiKeyBoardAngelDlg m_diakeybrode;

	m_diakeybrode.input=keyinput;
	if(keyinput>='0'&&keyinput<='9')
		m_diakeybrode.ischar=false;
	else
		m_diakeybrode.ischar=true;
	m_diakeybrode.DoModal();
	char *pResult=m_diakeybrode.result;
	if(!m_diakeybrode.isresultok||!m_diakeybrode.isresultstock)	return;
	
	SharesSymbol *addto=new SharesSymbol;
	strcpy(addto->sharesymbol,pResult);

	BOOL bFind=FALSE;
	int nTotal=pDoc->m_choosesymbol.GetCount();
	if(nTotal==0)             
	{
		CString strStockCode=addto->sharesymbol;
		m_MapChoose[strStockCode]=strStockCode;
		pDoc->m_choosesymbol.AddHead(addto);
		updatechooselist(0);
	}
    else
	{
		POSITION nPos=pDoc->m_choosesymbol.GetHeadPosition();
		for(int i=0;i<nTotal;i++)
		{
			char *serch=pDoc->m_choosesymbol.GetNext(nPos)->sharesymbol;
			if(strcmp(pResult,serch)==0)
			{ 
				bFind=TRUE;
				break;
			}		
		}
		if(bFind)
		{
			AfxMessageBox("你要加入的股票自选股中已存在,请重新选择",MB_ICONSTOP);
			return;
		}
		else
		{
			CString strStockCode=addto->sharesymbol;
			m_MapChoose[strStockCode]=strStockCode;
			pDoc->m_choosesymbol.AddTail(addto);		
		}
	}
   	updatechooselist(nTotal);
	m_ctrlChoosedStock.SetFocus();
	int totalstockcount=m_ctrlChoosedStock.GetItemCount();
	m_strChooseStatus.Format("可用键盘精灵选择，自选股里一共有%d个股票",totalstockcount);
	UpdateData(FALSE);
}

CTaiShanDoc * CTaiSysSetSelfBlockPage::GetDocument()
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	ASSERT_VALID(pDoc);
	return pDoc;
}

void CTaiSysSetSelfBlockPage::InitListCtrl()
{
	CTaiShanDoc*   pDoc=GetDocument();

	int mapItemCount=pDoc->m_StockTypeMap[0].GetCount();
	CString strKey,strPost;
	POSITION mapPos;
	mapPos=pDoc->m_StockTypeMap[0].GetStartPosition();
	for(int k=0;k<mapItemCount;k++)
	{
		pDoc->m_StockTypeMap[0].GetNextAssoc(mapPos,strKey,strPost);
		m_MapChoose[strKey]=strKey;
	}

	m_ctrlStockName.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	CReportData *pDat0=NULL;
	pDoc->m_sharesInformation.GetStockItem(0,0,pDat0);

	LV_ITEM SelectedItem;
	SelectedItem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	SelectedItem.iSubItem=0;
	SelectedItem.iItem=0;
	SelectedItem.iImage=0;
	SelectedItem.state=LVIS_FOCUSED |LVIS_SELECTED;
	SelectedItem.pszText=pDat0->name;
	m_ctrlStockName.InsertItem(&SelectedItem);
	int nPos=1;
	for(int nStockType=0;nStockType<8;nStockType++)
	{
		for(int i=0;i<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);i++)
		{
			if(nStockType==0&&i==0)continue;
			CReportData *pDat1=NULL;
			pDoc->m_sharesInformation.GetStockItem(nStockType,i,pDat1);
			if(pDat1)
			{
				m_ctrlStockName.InsertItem(nPos,pDat1->name,0);
				nPos++;
			}
		}		
	}
	m_ctrlChoosedStock.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
}

CString CTaiSysSetSelfBlockPage::GetStockCode(int nIndex)
{
	CString strResult;
	strResult.Empty();
	int nItem=nIndex;
	int nCount;
	CTaiShanDoc*   pDoc=GetDocument();
	for(int nStockType=0;nStockType<8;nStockType++)
	{
		nCount=pDoc->m_sharesInformation.GetStockTypeCount(nStockType);
		if(nItem>(nCount-1))
			nItem-=nCount;
		else
			break;
	}
	if(nStockType<8)
	{
		CTaiShanDoc*   pDoc=GetDocument();
		CReportData *pDat1=NULL;
		pDoc->m_sharesInformation.GetStockItem(nStockType,nItem,pDat1);
		if(pDat1)
			strResult=pDat1->id;
	}
	return strResult;
}
