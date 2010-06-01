
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ManagerStockType.h"

#include <share.h>
#include  <io.h>
#include <direct.h>
#include "mainfrm.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "managerchoose.h"
#include "modifystocktype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperviseSharesBlock dialog

///////////////////////////////////////////////////////////////////////////////////////////

CSuperviseSharesBlock::CSuperviseSharesBlock(CWnd* pParent /*=NULL*/) : CDialog(CSuperviseSharesBlock::IDD, pParent)
{
}

void CSuperviseSharesBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSuperviseSharesBlock)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_UPDATESTOCKTYPE, m_updatestocktype);
	DDX_Control(pDX, IDC_REFRESH_DAY, m_refreshday);
	DDX_Control(pDX, IDC_REFRESH, m_refresh);
	DDX_Control(pDX, IDC_MANAGERCHOOSE, m_managerchoose);
	DDX_Control(pDX, IDC_DELETESTOCKTYPE, m_delete);
	DDX_Control(pDX, IDC_ADDSTOCKTYPE, m_add);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LIST1, m_MyList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSuperviseSharesBlock, CDialog)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	//{{AFX_MSG_MAP(CSuperviseSharesBlock)
	ON_BN_CLICKED(IDC_IMPORT, OnImportStockType)
	ON_BN_CLICKED(IDC_ADDSTOCKTYPE, OnAddStockType)
	ON_BN_CLICKED(IDC_UPDATESTOCKTYPE, OnUpdateStockType)
	ON_BN_CLICKED(IDC_MANAGERCHOOSE, OnManagerChoose)
	ON_BN_CLICKED(IDC_DELETESTOCKTYPE, OnDeletestocktype)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_REFRESH_DAY, OnRefreshDayLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CSuperviseSharesBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bIsCalc = FALSE;

	CenterWindow();

	m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	m_pDoc->m_bOpenStockTypeDlg = TRUE;

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);
	m_pImageList->Create(IDB_BITMAP14, 32, 0, RGB(192, 192, 192));
	m_pImageListSmall->Create(IDB_BITMAP15, 16, 0, RGB(192, 192, 192));

	FillToListCtrlHead();

	CStringArray m_StockTypeCodeArray;
	m_pDoc->m_ManagerStockTypeData.GetAllStockTypeCode(m_StockTypeCodeArray);

	int l_nCount = m_StockTypeCodeArray.GetSize();
	for (int i = 0; i < l_nCount; i++)
	{
		STOCKTYPEINFO* l_pStktype;
		CString l_sStockType = m_StockTypeCodeArray.GetAt(i);
		m_pDoc->m_ManagerStockTypeData.GetStockTypePoint(l_pStktype, l_sStockType.GetBuffer(0));
		FillToListCtrlItem(l_pStktype);
	}

	m_MyList.SetFocus();
	m_MyList.SetItemState(0, LVIS_FOCUSED|LVIS_SELECTED, LVIS_SELECTED|LVIS_FOCUSED);

	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;

#ifdef WIDE_NET_VERSION
	CWnd* pWnd = GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
#endif

	return TRUE;
}

void CSuperviseSharesBlock::OnOK() 
{

	if(m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

	::SendMessage(m_pDoc->m_WsStock2000View->m_hWnd,WM_USER_CHANGESTOCK,0,0);
	m_pDoc ->m_bOpenStockTypeDlg=FALSE;

	CDialog::OnOK();
}

void CSuperviseSharesBlock::OnCancel()
{
	if (m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

	m_pDoc->m_bOpenStockTypeDlg = FALSE;

	CDialog::OnCancel();
	SendMessage(WM_CLOSE, 0, 0);
}

void CSuperviseSharesBlock::FillToListCtrlHead()
{ 
	CRect rect;
	CString strItem1 = _T("指数代码");
	CString strItem2 = _T("指数名称");
	CString strItem3 = _T("权重类型");
	CString strItem4 = _T("股票总数");

	m_MyList.SetExtendedStyle(m_MyList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_MyList.SetImageList(m_pImageList, LVSIL_NORMAL);
	m_MyList.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	m_MyList.GetWindowRect(&rect);
	int nWidth = (rect.Width() - 16) / 4;

	m_MyList.InsertColumn(0, strItem1, LVCFMT_LEFT, nWidth, 0);
	m_MyList.InsertColumn(1, strItem2, LVCFMT_LEFT,	nWidth, 1);
	m_MyList.InsertColumn(2, strItem3, LVCFMT_LEFT, nWidth, 2);
	m_MyList.InsertColumn(3, strItem4, LVCFMT_LEFT, nWidth, 3);
}

void CSuperviseSharesBlock::RefreshShowData()
{
	BeginWaitCursor();

	m_MyList.DeleteAllItems();

	CStringArray m_StockTypeCodeArray;
	m_pDoc->m_ManagerStockTypeData.GetAllStockTypeCode(m_StockTypeCodeArray);
	int l_nCount = m_StockTypeCodeArray.GetSize();
	for (int i = 0; i < l_nCount; i++)
	{
		STOCKTYPEINFO* l_pStktype;
		CString l_sStockType = m_StockTypeCodeArray.GetAt(i);
		m_pDoc->m_ManagerStockTypeData.GetStockTypePoint(l_pStktype, l_sStockType.GetBuffer(0));
		FillToListCtrlItem(l_pStktype);
	}

	EndWaitCursor();
}

void CSuperviseSharesBlock::FillToListCtrlItem(STOCKTYPEINFO* pStockType)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=pStockType->m_szCode;
	if(m_MyList.FindItem(&lvf)>=0)
		return;
	iItem=this->m_MyList.GetItemCount();        
	for (iSubItem = 0; iSubItem <4; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  

		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;


		CString strIconDesc[4];
		strIconDesc[0]=pStockType->m_szCode;
		strIconDesc[1]=pStockType->m_szName;

		switch(pStockType->m_iRightType)    
		{
		case ZGB:     strIconDesc[2]="总股本";
			break;  	   
		case LTG:     strIconDesc[2]="流通股本";
			break;  	   
		case OTHER:   strIconDesc[2]="相等权重";
			break;  	   
		}
		strIconDesc[3].Format("%d",pStockType->m_lStockCount);

		lvitem.pszText=strIconDesc[iSubItem].GetBuffer(strIconDesc[iSubItem].GetLength()); 
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_MyList.InsertItem(&lvitem);
		else
			this->m_MyList.SetItem(&lvitem);
	} 
}


void CSuperviseSharesBlock::OnImportStockType() 
{


	int stocklength;
	MSG message;
	char  blank[2]={' ',' '};  
	CString filename;
	CString DefExt="*";  
	CString FileName="板块数据\\*.*";  
	CString Filter="板块文件名 ( *.* )| *.* ||"; 
	CFileDialog OpenFileDialog(TRUE,(LPCSTR)DefExt,
		(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT ,
		(LPCSTR)Filter,this);
	if( OpenFileDialog.DoModal() ==IDOK )      
	{
		CWnd *pWnd=this->GetDlgItem(IDC_IMPORT);
		pWnd->EnableWindow(FALSE);
		BeginWaitCursor();
		//**************************************************************************************
		int index=0;
		int AddCounts=0;
		POSITION l_position=OpenFileDialog.GetStartPosition();
		while(l_position)
		{

			CFile stockfile;

			filename=OpenFileDialog.GetNextPathName(l_position);
			index++;
			if(stockfile.Open((LPCTSTR)filename,CFile::modeReadWrite))
			{
				int FileLength=stockfile.GetLength();
				stockfile.Read(&stocklength,4);
				int FileCalcLength=4+stocklength*12;
				if(FileLength!=FileCalcLength)
				{
					MessageBox("导入板块数据文件不正确,请重选板块文件! 本次导入操作失败.","板块导入操作提示",MB_OK);
					stockfile.Close();
					pWnd->EnableWindow(TRUE);
					EndWaitCursor();
					return;

				}
				for(int j=0;j<stocklength;j++)
				{
					STOCK_TYPE_INFO *pStock;
					SharesSymbol *symbol=new SharesSymbol;
					stockfile.Read(symbol->sharesymbol,6);
					stockfile.Read(&symbol->nKind,4);
					symbol->sharesymbol[6]='\0';
					stockfile.Read(&blank,2);
					if(!m_pDoc->m_ManagerStockTypeData.Lookup(symbol->sharesymbol,symbol->nKind,pStock))
					{
						AddCounts++;
					}
					delete symbol;
				}
				stockfile.Close();

			}
		}
		m_pDoc->m_ManagerStockTypeData.CheckStockCount(AddCounts);
		//*************************************************************************************
		m_Progress.SetRange(0,index);
		m_Progress.SetPos(0);
		index=0;
		l_position=OpenFileDialog.GetStartPosition();
		while(l_position)
		{
			CString l_sFileName;
			CString l_sFilePath;
			int l_nPos;
			CReportData *pCdat;
			STOCKTYPEINFO *l_pStockkType;
			filename=OpenFileDialog.GetNextPathName(l_position);
			filename.MakeReverse();
			l_nPos=filename.Find("\\",0);
			l_sFileName=filename.Left(l_nPos);
			l_sFileName.MakeReverse();
			if(l_sFileName.GetLength()>8)
			{
				MessageBox("输入板块名的长度不能大于8个字节","板块操作提示",MB_OK);
				continue;
			}
			filename.MakeReverse();
			l_sFilePath=filename.Left(filename.GetLength() - l_nPos-1);   
			m_pDoc->m_ManagerStockTypeData.ImportStockTypeData(l_sFilePath,l_sFileName,l_pStockkType);  //导入板块

			m_pDoc->m_sharesInformation.InsertItem(l_pStockkType->m_szCode,pCdat,STKTYPE);              //增加股票代码
			m_pDoc->m_WsStock2000View->LoadShowData(STKTYPE);             
			strcpy(pCdat->name ,l_pStockkType->m_szName );
			strcpy(pCdat->id ,l_pStockkType->m_szCode );
			pCdat->kind=STKTYPE  ;
			if(strlen(pCdat->name)>0)
				m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;

			m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
			WORD type=STKTYPE;
			m_pDoc->m_WsStock2000View->AddGridRow(type);              

			FillToListCtrlItem(l_pStockkType);        
			//*************************************************************************************
			index++;
			m_Progress.SetPos(index);
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			//*************************************************************************************
		}	
		pWnd->EnableWindow(TRUE);
		EndWaitCursor();

	}
	m_Progress.SetPos(0);
}


void CSuperviseSharesBlock::OnAddStockType()
{
	STOCKTYPEINFO* l_StockTypeInfo = new STOCKTYPEINFO;

	char temp[10];
	int l_nPos = m_pDoc->m_ManagerStockTypeData.GetInsertStockTypePos();
	sprintf(temp, "%2d", l_nPos);

	for (int i = 0; i < 2; i++)
	{
		if (temp[i] == ' ') temp[i] = '0';
	}

	sprintf(l_StockTypeInfo->m_szCode, "%s%s", "8K00", temp);
	l_StockTypeInfo->m_iRightType = 1;

	CModifyStockType l_ModifyStockType(l_StockTypeInfo, TRUE, this);
	if (l_ModifyStockType.DoModal() == IDOK)
	{
		RefreshShowData();
	}

	delete l_StockTypeInfo;
}


void CSuperviseSharesBlock::OnUpdateStockType() 
{
	STOCKTYPEINFO* l_pStockType;
	STOCKTYPEINFO* l_pStockTypeInfo;

	l_pStockType = new STOCKTYPEINFO;

	int temp = m_MyList.GetNextItem(-1, LVNI_SELECTED);
	if (temp == -1)
	{
		if (l_pStockType) delete l_pStockType;
		AfxMessageBox("请选择正确的板块!");
		return;
	}

	char l_StockTypeCode[10];
	m_MyList.GetItemText(temp, 0, l_StockTypeCode, 10);
	m_pDoc->m_ManagerStockTypeData.GetStockTypePoint(l_pStockTypeInfo, l_StockTypeCode);
	memcpy(l_pStockType, l_pStockTypeInfo, sizeof(STOCKTYPEINFO));

	CModifyStockType l_ModifyStockType(l_pStockType, FALSE);
	if (l_ModifyStockType.DoModal() == IDOK)       
	{
		RefreshShowData();                       
	}

	delete l_pStockType;
}

void CSuperviseSharesBlock::OnManagerChoose()
{
	CSuperviseChoose l_ManagerChooseDlg;
	l_ManagerChooseDlg.DoModal();
}

void CSuperviseSharesBlock::OnDeletestocktype() 
{
	int temp = m_MyList.GetNextItem(-1, LVNI_SELECTED);
	if (temp == -1)
	{
		AfxMessageBox("请选择将删除的板块!");
		return;
	}

	int rtn = MessageBox("是否确实进行这一删除操作?", "警告", MB_YESNO|MB_ICONWARNING);
	if (rtn != 6)
		return;

	char l_StockTypeCode[10];
	m_MyList.GetItemText(temp, 0, l_StockTypeCode, 10);
	m_pDoc->m_ManagerStockTypeData.DeleteStockType(l_StockTypeCode);

	CReportData* pCdat;
	if (m_pDoc->m_sharesInformation.Lookup(l_StockTypeCode, pCdat, STKTYPE))
	{
		m_pDoc->DeleteStockFromKeyboard(pCdat->id, pCdat->kind, pCdat->name, pCdat->Gppyjc);
		m_pDoc->m_sharesInformation.DeleteOneStockInfo(l_StockTypeCode, STKTYPE);
	}

	RefreshShowData();
}

void CSuperviseSharesBlock::OnRefresh() 
{
	CWnd* pWnd = GetDlgItem(IDC_REFRESH);

	pWnd->EnableWindow(FALSE);
	BeginWaitCursor();

	m_pDoc->m_ManagerStockTypeData.CalcMinuteRealTimeIndex();
	m_pDoc->m_ManagerStockTypeData.CalcAllStockTypeData();

	pWnd->EnableWindow(TRUE);
	EndWaitCursor();
}

void CSuperviseSharesBlock::OnExport() 
{

	char m_szCurrentDir[MAX_PATH],makedirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, m_szCurrentDir);
	sprintf(makedirectory,"%s\\板块数据",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
		_mkdir(makedirectory);

	int nCount=m_MyList.GetItemCount();
	char  blank[2]={' ',' '};             
	for(int i=0;i<nCount;i++)
	{
		char strStockCode[10];
		char strStockName[10];
		FILE *fp;
		SymbolKindArr StockCodeArray;
		char FileName[256];
		m_MyList.GetItemText( i, 0, strStockCode, sizeof(strStockCode) ) ;
		m_MyList.GetItemText( i, 1, strStockName, sizeof(strStockName) ) ;
		sprintf(FileName,"%s\\%s",makedirectory,strStockName);
		m_pDoc->m_ManagerStockTypeData.GetStockFromStockType(StockCodeArray,strStockCode);
		int length=StockCodeArray.GetSize();
		fp=_fsopen(FileName,"w+b",SH_DENYNO);
		if(fp!=NULL)
		{
			fwrite(&length,4,1,fp);
			for(int i=0;i<StockCodeArray.GetSize();i++)
			{
				SymbolKind m_SymbolKind=StockCodeArray.GetAt(i);
				fwrite(m_SymbolKind.m_chSymbol ,1,6,fp);
				fwrite(&m_SymbolKind.m_nSymbolKind ,1,4,fp);

				fwrite(&blank,1,2,fp);
			}
			fclose(fp);
		}       
	}
	if(nCount>0)
		AfxMessageBox("板块数据导出成功!");
}

void CSuperviseSharesBlock::OnRefreshDayLine() 
{

	int temp=m_MyList.GetNextItem( -1, LVNI_SELECTED   ) ; 
	char l_StockTypeCode[10];
	if(temp==-1)
	{
		AfxMessageBox("请选择正确的刷新板块!");
		return;
	}
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);

	BeginWaitCursor();
	m_bIsCalc=TRUE;
	m_MyList.GetItemText( temp, 0, l_StockTypeCode, 10);
	m_pDoc->m_ManagerStockTypeData.RefreshHistoryDayLineData( l_StockTypeCode,&m_Progress);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);

	if(temp+1<m_MyList.GetItemCount())
	{
		m_MyList.SetFocus();
		m_MyList.SetItemState(temp+1,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	}
	EndWaitCursor();
	m_bIsCalc=FALSE;
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 

}

void CSuperviseSharesBlock::OnClose() 
{
	if (m_bIsCalc)
	{
		AfxMessageBox("程序正在运算不能进行退出操作!");
		return;
	}

	::SendMessage(m_pDoc->m_WsStock2000View->m_hWnd, WM_USER_CHANGESTOCK, 0, 0);

	m_pDoc->m_bOpenStockTypeDlg = FALSE;

	CDialog::OnClose();
}

BOOL CSuperviseSharesBlock::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	DoHtmlHelp(this);
	return TRUE;
}
