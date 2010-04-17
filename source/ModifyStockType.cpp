// ModifyStockType.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "mainfrm.h"
#include "SelectStock.h"
#include "keybrodeangel.h"
#include "ModifyStockType.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyStockType dialog

CModifyStockType::CModifyStockType(PSTOCKTYPEINFO &pStockTypeInfo,BOOL IsAddStockType,CWnd* pParent )   // standard constructor
	: CDialog(CModifyStockType::IDD, pParent)
{
	
	m_pStockTypeInfo=pStockTypeInfo;
	m_bIsAddNewStockType=IsAddStockType;
    m_bIsChanged=FALSE;
}


CModifyStockType::CModifyStockType(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyStockType::IDD, pParent)
{
}


void CModifyStockType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyStockType)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_REFRESH, m_refresh);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_LIST2, m_MyList);
	DDX_Text(pDX, IDC_CODE, m_sStockTypeCode);
	DDX_Text(pDX, IDC_NAME, m_sStockTypeName);
	DDV_MaxChars(pDX, m_sStockTypeName, 8);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Radio(pDX, IDC_RADIO1,m_nRightType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyStockType, CDialog)
	//{{AFX_MSG_MAP(CModifyStockType)
	ON_BN_CLICKED(IDC_ADD, OnAddStock)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteStock)
	ON_BN_CLICKED(IDC_RADIO1, OnChangeStockTypeRight)
	ON_BN_CLICKED(IDC_RADIO2, OnChangeStockTypeRight)
	ON_BN_CLICKED(IDC_RADIO3, OnChangeStockTypeRight)
	ON_BN_CLICKED(IDOK, OnUpdateOk)
	ON_BN_CLICKED(IDC_REFRESH, OnRefreshHistoryData)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStockCode)
	ON_EN_CHANGE(IDC_NAME, OnChangedName)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyStockType message handlers

BOOL CModifyStockType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->CenterWindow();
	

	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	FillToListCtrlHead();
	if(m_bIsAddNewStockType)
	{
       m_sStockTypeCode=m_pStockTypeInfo->m_szCode;
	   m_nRightType=1;
	   m_fTotalRight=0;
	   m_nTotalCount=0;
	}
	else
	{
       m_sStockTypeCode=m_pStockTypeInfo->m_szCode;
       m_sStockTypeName=m_pStockTypeInfo->m_szName;
	   m_nRightType=m_pStockTypeInfo->m_iRightType;
	   m_fTotalRight=m_pDoc->m_ManagerStockTypeData.GetStockBlockRight(m_pStockTypeInfo->m_szCode);
	   m_nTotalCount=m_pStockTypeInfo->m_lStockCount;
       SymbolKindArr m_StockCodeArray;
	   if(m_pDoc->m_ManagerStockTypeData.GetStockFromStockType(m_StockCodeArray,m_sStockTypeCode.GetBuffer(0)))          
	   {
			int DisplayCount=m_StockCodeArray.GetSize();

			for(int row=0 ;row <  (int)DisplayCount  ;row++)          
			{    
				 CReportData *Cdat;
				 SymbolKind m_SymbolKind; 
				 m_SymbolKind=m_StockCodeArray.GetAt(row) ;
	 			 if (m_pDoc->m_sharesInformation.Lookup(m_SymbolKind.m_chSymbol , Cdat,m_SymbolKind.m_nSymbolKind) == TRUE)     //检测该股票是否已记录在内存模板中
				 {
					 if(Cdat->pBaseInfo==NULL)
						 continue;
                     FillToListCtrlItem(Cdat);
				 }
			}
	   }
	}
	this->UpdateData(FALSE);
    CWnd *pWnd=this->GetDlgItem(IDC_NAME);
	pWnd->SetFocus();

    m_MyList.SetFocus();
    m_MyList.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;

	return TRUE;  
}
void CModifyStockType::FillToListCtrlHead()
{
   	CRect           rect;
	CString         strItem1= _T("股票代码");
	CString         strItem2= _T("股票名称");
	CString         strItem3= _T("总股本");
	CString         strItem4= _T("流通股本");
	CString         strItem5= _T("权重比例(%)");

	m_MyList.SetExtendedStyle(m_MyList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	this->m_MyList.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_MyList.SetImageList(m_pImageListSmall, LVSIL_SMALL);


	this->m_MyList.GetWindowRect(&rect);
	this->m_MyList.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1/5, 0);
	this->m_MyList.InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 1/5, 1);
	this->m_MyList.InsertColumn(2, strItem3, LVCFMT_LEFT,
		rect.Width() * 1/5, 2);
	this->m_MyList.InsertColumn(3, strItem4, LVCFMT_LEFT,
		rect.Width() * 1/5, 3);
	this->m_MyList.InsertColumn(4, strItem5, LVCFMT_LEFT,
		rect.Width() * 1/5, 3);

}
void CModifyStockType::FillToListCtrlItem(CReportData *pCdat)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=pCdat->id;
    if(m_MyList.FindItem(&lvf)>=0)
		return;
    iItem=this->m_MyList.GetItemCount();
	CString strIconDesc[5];
    strIconDesc[0]=pCdat->id;
    strIconDesc[1]=pCdat->name;
    strIconDesc[2].Format("%6.2f",pCdat->pBaseInfo->zgb);

	if(CTaiShanReportView::IsBg( pCdat->kind))
	{
        strIconDesc[3].Format("%6.2f",pCdat->pBaseInfo->Bg);
	}
	else
    {
        strIconDesc[3].Format("%6.2f",pCdat->pBaseInfo->ltAg);
	}
	if(m_fTotalRight!=0)
	{
	    switch(this->m_nRightType)
		{
			case 0:
					strIconDesc[4].Format("%8.3f",pCdat->pBaseInfo->zgb/m_fTotalRight);
					break;
			case 1:
					if(CTaiShanReportView::IsBg( pCdat->kind))
					{
						strIconDesc[4].Format("%8.3f",pCdat->pBaseInfo->Bg/m_fTotalRight);
					}
					else
					{
						strIconDesc[4].Format("%8.3f",pCdat->pBaseInfo->ltAg/m_fTotalRight);
					}
					break;
			case 2:
					strIconDesc[4].Format("%8.3f",(float)1/m_fTotalRight);
					break;
		}
	}
	for (iSubItem = 0; iSubItem <5; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  
		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

	
       
		lvitem.pszText=strIconDesc[iSubItem].GetBuffer(strIconDesc[iSubItem].GetLength()); 
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_MyList.InsertItem(&lvitem); 
		else
			this->m_MyList.SetItem(&lvitem); 
		m_MyList.SetItemData(iItem,pCdat->kind);
	} 
	
}
void CModifyStockType::RefreshShowData()
{
	this->UpdateData();
	m_fTotalRight=0;
	m_nTotalCount=0;

    int tmp=m_MyList.GetItemCount();
	for(int i=0;i<tmp;i++)
	{
	   char temp[20];
       switch(this->m_nRightType)
	   {
	     case ZGB:    m_MyList.GetItemText( i, 2, temp, 20);
			          m_fTotalRight+=atof(temp);
				      m_nTotalCount++;
				      break;
	     case LTG:    m_MyList.GetItemText( i, 3, temp, 20);
			          m_fTotalRight+=atof(temp);
				      m_nTotalCount++;
				      break; 
	     case OTHER:  m_fTotalRight+=1;
				      m_nTotalCount++;
				      break;
	  }
	}
	for(int i=0;i<tmp;i++)
	{
	   char temp[20];
	   float l_fRight;
	   CString l_sRight;
       switch(this->m_nRightType)
	   {
	     case ZGB:    m_MyList.GetItemText( i, 2, temp, 20);
			          l_fRight=atof(temp)/m_fTotalRight;
					  l_sRight.Format("%8.3f",l_fRight);
                      m_MyList.SetItemText( i, 4 , l_sRight );
				      break;
	     case LTG:    m_MyList.GetItemText( i, 3, temp, 20);
			          l_fRight=atof(temp)/m_fTotalRight;
					  l_sRight.Format("%8.3f",l_fRight);
                      m_MyList.SetItemText( i, 4 , l_sRight );
				      break; 
	     case OTHER:  l_fRight=(float)1/m_nTotalCount;
					  l_sRight.Format("%8.3f",l_fRight);
                      m_MyList.SetItemText( i, 4 , l_sRight );
				      break;
	  }
	}
	m_bIsChanged=TRUE;
}
void CModifyStockType::OnAddStock()
{
	SelectStock pDlg(this,TRUE);
	pDlg.DoModal();
}
void CModifyStockType::OnDeleteStock()
{
    CString Id;
	int temp=m_MyList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请选择将要删除股票!");
		return;
	}
    int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;

	while(temp>=0)
	{
		char l_sStockCode[10];
		CString l_strCode;
        m_MyList.GetItemText( temp, 0, l_sStockCode, 10);
        l_strCode=l_sStockCode;
		m_MyList.DeleteItem(temp);
        temp=m_MyList.GetNextItem( -1,LVNI_SELECTED   ) ;
	}

   RefreshShowData(); 
   m_MyList.SetFocus();
   m_MyList.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
}
void CModifyStockType::OnChangeStockTypeRight()
{
   RefreshShowData(); 
}
BOOL CModifyStockType::PreTranslateMessage(MSG* pMsg)
{
    CWnd *pWnd=this->GetFocus();
	CWnd *pChildWnd=this->GetDlgItem(IDC_NAME);
	if(pWnd!=pChildWnd)
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
			  LVFINDINFO  lvf;
			  lvf.flags=LVFI_STRING;
			  lvf.psz=result;
			  if(m_MyList.FindItem(&lvf)>=0)
			 	 return  TRUE;
              
			  CReportData *Cdat;
	 		  if (m_pDoc->m_sharesInformation.Lookup(result , Cdat,nKind) == TRUE)     
			  {
				   if(Cdat->pBaseInfo!=NULL)
                     FillToListCtrlItem(Cdat);
			  }
		   }
		}
        RefreshShowData(); 
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CModifyStockType::OnAddStockCode(WPARAM wParam, LPARAM lParam) 
{
	SHARES_DATA_ADD *pAddCode;
	int lsCount=(int)lParam;
    pAddCode=(SHARES_DATA_ADD *)wParam;
	for(int i=0;i<lsCount;i++)
	{
		if(pAddCode[i].StockType<0)
		{
			int nKind=-pAddCode[i].StockType-1;
		    LVFINDINFO  lvf;
		    lvf.flags=LVFI_STRING;
		    lvf.psz=pAddCode[i].name;
			int iItem=m_MyList.FindItem(&lvf);
		    if(iItem >=0)
			{
				if(nKind==m_MyList.GetItemData(iItem))
			       continue;
			}
		    CReportData *pCdat;
	 	    if (m_pDoc->m_sharesInformation.Lookup(pAddCode[i].name, pCdat,nKind) == TRUE)     //检测该股票是否已记录在内存模板中
			{
				if(CSharesInformation::IsZhiShu(pCdat->kind))
				{
					MessageBox("指数类将不能增加到板块中!","窗口操作提示",MB_OK);
					continue;
				}
				if(pCdat->pBaseInfo==NULL||pCdat->pBaseInfo->zgb<=0)
				{
					MessageBox("在将股票加入板块时,该股票财务数据不能为空!","窗口操作提示",MB_OK);
					continue;
				}
               if(pCdat->pBaseInfo!=0) 
                 FillToListCtrlItem(pCdat);
			}
		}
	}
    RefreshShowData(); 

	return 0;
}

void CModifyStockType::OnUpdateOk()
{
	STOCKTYPEINFO *l_pStockType;
	this->UpdateData(TRUE);
	if(!m_bIsChanged)
	{
      CDialog::OnOK();
	  return;
	}
	if(this->m_sStockTypeName.GetLength()<=0||m_MyList.GetItemCount()<1)
	{
		AfxMessageBox("请选择正确板块数据!");
		return ;
	}
	CWnd *pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);

	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

    BeginWaitCursor();
	CheckStockCount();
	if(m_bIsAddNewStockType)
	{
		if(m_pDoc->m_ManagerStockTypeData.CheckBlockName(m_sStockTypeName.GetBuffer(0),NULL))
		{
			AfxMessageBox("请选择正确板块名称! 板块名称不能重复");
			pWnd->EnableWindow(TRUE);
			return ;
		}
		int l_nStockTypePos=m_pDoc->m_ManagerStockTypeData.GetInsertStockTypePos();
		char temp[3];
		if(!m_pDoc->m_ManagerStockTypeData.InsertStockType(l_pStockType,l_nStockTypePos))
		{
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%2d",l_nStockTypePos);
		for(int i=0;i<2;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K00",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
		int tmp=m_MyList.GetItemCount();
		for(int i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			CReportData *l_pCdat;
			int nKind=m_MyList.GetItemData(i);
			m_MyList.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerStockTypeData.InsertStockToType(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,STKTYPE);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=STKTYPE;
        m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    else
	{
		char BlockCode[MAX_PATH];
		strcpy(BlockCode,m_sStockTypeCode.GetBuffer(0));
		if(m_pDoc->m_ManagerStockTypeData.CheckBlockName(m_sStockTypeName.GetBuffer(0),BlockCode))
		{
			AfxMessageBox("请选择正确板块名称! 板块名称不能重复");
			pWnd->EnableWindow(TRUE);
			return ;
		}
		m_pDoc->m_ManagerStockTypeData.DeleteStockType(m_sStockTypeCode.GetBuffer(0));
		int l_nStockTypePos=m_pDoc->m_ManagerStockTypeData.GetInsertStockTypePos();
		char temp[3];
		if(!m_pDoc->m_ManagerStockTypeData.InsertStockType(l_pStockType,l_nStockTypePos))
		{
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%2d",l_nStockTypePos);
		for(int i=0;i<2;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K00",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
		int tmp=m_MyList.GetItemCount();
		for(int i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			CReportData *l_pCdat;
			int nKind=m_MyList.GetItemData(i);
			m_MyList.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerStockTypeData.InsertStockToType(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,STKTYPE);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=STKTYPE;
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    m_pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),	&m_Progress);
    EndWaitCursor();
	CDialog::OnOK();
}
void CModifyStockType::OnRefreshHistoryData()
{
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);

	STOCKTYPEINFO *l_pStockType;
	this->UpdateData(TRUE);
    BeginWaitCursor();
	if(!m_bIsChanged)
	{
      m_pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),&m_Progress);
	  pWnd->EnableWindow(TRUE);
      EndWaitCursor();
	  return;
	}
	else
	{
		int rtn = MessageBox("该板块数据已发生变化,是否在保存板块数据后进行数据刷新操作!","警告",MB_YESNO|MB_ICONWARNING);
		if(rtn!=6)
		{
            m_pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),&m_Progress);
	        pWnd->EnableWindow(TRUE);
            EndWaitCursor();
			return;
		}
	}
	if(this->m_sStockTypeName.GetLength()<=0||m_MyList.GetItemCount()<1)
	{
		AfxMessageBox("请选择正确板块数据!");
	    pWnd->EnableWindow(TRUE);
        EndWaitCursor();
		return ;
	}
    CheckStockCount();
	if(m_bIsAddNewStockType)
	{
		int l_nStockTypePos=m_pDoc->m_ManagerStockTypeData.GetInsertStockTypePos();
		char temp[3];
		if(!m_pDoc->m_ManagerStockTypeData.InsertStockType(l_pStockType,l_nStockTypePos))
		{
	        pWnd->EnableWindow(TRUE);
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%2d",l_nStockTypePos);
		for(int i=0;i<2;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K00",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
            
		int tmp=m_MyList.GetItemCount();
		for(int i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			int nKind=m_MyList.GetItemData(i);
			CReportData *l_pCdat;
			m_MyList.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerStockTypeData.InsertStockToType(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,STKTYPE);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=STKTYPE;
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    else
	{
		m_pDoc->m_ManagerStockTypeData.DeleteStockType(m_sStockTypeCode.GetBuffer(0));
		int l_nStockTypePos=m_pDoc->m_ManagerStockTypeData.GetInsertStockTypePos();
		char temp[3];
		if(!m_pDoc->m_ManagerStockTypeData.InsertStockType(l_pStockType,l_nStockTypePos))
		{
	        pWnd->EnableWindow(TRUE);
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%2d",l_nStockTypePos);
		for(int i=0;i<2;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K00",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
            
		int tmp=m_MyList.GetItemCount();
		for(int i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			int nKind=m_MyList.GetItemData(i);
			CReportData *l_pCdat;
			m_MyList.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerStockTypeData.InsertStockToType(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,STKTYPE);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=STKTYPE;
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    m_pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),	&m_Progress);
	m_bIsChanged=FALSE;
    m_bIsAddNewStockType=FALSE;
	pWnd->EnableWindow(TRUE);
    EndWaitCursor();
}
void CModifyStockType::CheckStockCount()
{
	int tmp=m_MyList.GetItemCount();
	int index=0;
	for(int i=0;i<tmp;i++)
	{
        char l_szStockCode[10];
		PSTOCK_TYPE_INFO pStock;
		m_MyList.GetItemText( i, 0, l_szStockCode , 20);
		int nKind=m_MyList.GetItemData(i);
        if(!m_pDoc->m_ManagerStockTypeData.Lookup(l_szStockCode,nKind,pStock))
		{
		   index++;
		}
	}
	m_pDoc->m_ManagerStockTypeData.CheckStockCount(index);
}
void CModifyStockType::OnChangedName()
{
	m_bIsChanged=TRUE;

}
