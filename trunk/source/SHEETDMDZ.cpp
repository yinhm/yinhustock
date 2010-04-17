// SHEETDMDZ.cpp : implementation file
//

#include "stdafx.h"
#include "SHEETDMDZ.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"

#include "CTaiShanReportView.h"
#include "CTaiShanApp.h"

#include "CTaiStockInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTaiCodeContradistinguishPage property page

IMPLEMENT_DYNCREATE(CTaiCodeContradistinguishPage, CPropertyPage)

CTaiCodeContradistinguishPage::CTaiCodeContradistinguishPage() : CPropertyPage(CTaiCodeContradistinguishPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiCodeContradistinguishPage)
	m_RadioSh = -1;
	//}}AFX_DATA_INIT


	m_bAdd=FALSE;
	m_bDelete=FALSE;
	m_bModify=FALSE;
	m_SelfBrush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

CTaiCodeContradistinguishPage::~CTaiCodeContradistinguishPage()
{
	
}

void CTaiCodeContradistinguishPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiCodeContradistinguishPage)
	DDX_Control(pDX, IDC_PHONETIC, m_EditPhonetic);
	DDX_Control(pDX, IDC_SYMBOL, m_EditCode);
	DDX_Control(pDX, IDC_NAME, m_EditName);
	DDX_Control(pDX, IDC_SYMBOLCOMPARE, m_ctrlSymbolCompare);
	DDX_Radio(pDX, IDC_RADIOSH, m_RadioSh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiCodeContradistinguishPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiCodeContradistinguishPage)
	ON_BN_CLICKED(IDC_RADIOSZ, OnRadiosz)
	ON_BN_CLICKED(IDC_RADIOSH, OnRadiosh)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_NOTIFY(NM_CLICK, IDC_SYMBOLCOMPARE, OnClickSymbolcompare)
	ON_NOTIFY(LVN_KEYDOWN, IDC_SYMBOLCOMPARE, OnKeydownSymbolcompare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiCodeContradistinguishPage message handlers

BOOL CTaiCodeContradistinguishPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
 

	CheckRadioButton(IDC_RADIOSH,IDC_RADIOSZ,IDC_RADIOSH);

	
	InitListCtrl();	

	
	m_EditCode.EnableWindow(FALSE);
	m_EditName.EnableWindow(FALSE);
	m_EditPhonetic.EnableWindow(FALSE);

	
	GetDlgItem(IDC_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_DEL)->EnableWindow(FALSE);

	m_nOldSel=-1;
	return TRUE;  
}


void CTaiCodeContradistinguishPage::InitListCtrl()
{
	CTaiShanDoc* pDoc=GetDocument();

	
	LVCOLUMN listColumn;
	char * arColumn[3]={"股票代码","股票名称","拼音简称"};
	listColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt=LVCFMT_CENTER;
	for( int nColumn=0;nColumn<3;nColumn++)
	{
		listColumn.iSubItem=nColumn;
		listColumn.pszText=arColumn[nColumn];
		m_ctrlSymbolCompare.InsertColumn(nColumn, &listColumn);
	}
   
	ListView_SetExtendedListViewStyle(m_ctrlSymbolCompare.m_hWnd, LVS_EX_FULLROWSELECT);
	COLORREF color=RGB(255,255,220);
    m_ctrlSymbolCompare.SetBkColor( color );
	m_ctrlSymbolCompare.SetTextBkColor( color );
	m_ctrlSymbolCompare.SetColumnWidth(0,80);
	m_ctrlSymbolCompare.SetColumnWidth(1,90);
	m_ctrlSymbolCompare.SetColumnWidth(2,90);

	
	LVITEM   listItem;
	listItem.mask=LVIF_TEXT|LVIF_IMAGE;
	m_ctrlSymbolCompare.DeleteAllItems();
	int nTimes;
	int i,j;
	int nStockType;
	switch( GetCheckedRadioButton(IDC_RADIOSH,IDC_RADIOSZ) )
	{
	case IDC_RADIOSH: 
		nTimes=0;
		for(i=0;i<4;i++)
		{
			nStockType=i;
			if(i>2)nStockType=6;
			for(j=0;j<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);j++)
			{
				CReportData *pDat1=NULL;
				pDoc->m_sharesInformation.GetStockItem(nStockType,j,pDat1);
				if(pDat1)
				{
					m_ctrlSymbolCompare.InsertItem (nTimes,  pDat1->id);
					m_ctrlSymbolCompare.SetItemText(nTimes,1,pDat1->name);
					m_ctrlSymbolCompare.SetItemText(nTimes,2,pDat1->Gppyjc);
					nTimes++;
				}
				if( nTimes == 20 ) m_ctrlSymbolCompare.Update(20);
			}
		}
		break;

	case IDC_RADIOSZ: 
		nTimes=0;
		for(i=3;i<7;i++)
		{
			nStockType=i;
			if(i>5)nStockType=7;
			for(j=0;j<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);j++)
			{
				CReportData *pDat1=NULL;
				pDoc->m_sharesInformation.GetStockItem(nStockType,j,pDat1);
				if(pDat1)
				{
					m_ctrlSymbolCompare.InsertItem (nTimes,  pDat1->id);
					m_ctrlSymbolCompare.SetItemText(nTimes,1,pDat1->name);
					m_ctrlSymbolCompare.SetItemText(nTimes,2,pDat1->Gppyjc);
					nTimes++;
				}
				if( nTimes == 20 ) m_ctrlSymbolCompare.Update(20);
			}
		}
		break;
	}
}

void CTaiCodeContradistinguishPage::OnAdd() 
{
	UpdateData(TRUE);
	
	CTaiStockInputDlg InputDlg;

	
    if(m_RadioSh==0)
		InputDlg.SetStockType(TRUE);
	else
		InputDlg.SetStockType(FALSE);

	if(InputDlg.DoModal()==IDOK) 
	{
		if(MessageBox("你真的决定增加新的股票代码吗?","警告",MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
	
		int nIndex=0;
		m_ctrlSymbolCompare.InsertItem (nIndex,  InputDlg.m_strStockCode);
		m_ctrlSymbolCompare.SetItemText(nIndex,1,InputDlg.m_strStockName);
		m_ctrlSymbolCompare.SetItemText(nIndex,2,InputDlg.m_strStockPyjc);

       
		m_ctrlSymbolCompare.SetItemState(nIndex,LVIS_SELECTED,LVIS_SELECTED);

	
		m_EditCode.SetSel(0,-1);
		m_EditCode.ReplaceSel(InputDlg.m_strStockCode);
		m_EditName.SetSel(0,-1);
		m_EditName.ReplaceSel(InputDlg.m_strStockName);
		m_EditPhonetic.SetSel(0,-1);
		m_EditPhonetic.ReplaceSel(InputDlg.m_strStockPyjc);

		AddOneStockInfo(InputDlg.m_strStockCode,InputDlg.m_strStockName,InputDlg.m_strStockPyjc);
		
		m_bAdd=TRUE;
		SetModified();
	}
}

void CTaiCodeContradistinguishPage::OnDel() 
{
	POSITION pos = m_ctrlSymbolCompare.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("请首先选择须删除的股票项！",MB_ICONSTOP);
		return;
	}
	else
	{
		if( AfxMessageBox("真的要删除这支股票吗？",MB_ICONQUESTION|MB_YESNO)==IDNO )
			return;

		int nItem = m_ctrlSymbolCompare.GetNextSelectedItem(pos);
		CString strCode,strName,strHypy;
		
		strCode=m_ctrlSymbolCompare.GetItemText(nItem,0);
	
		strName=m_ctrlSymbolCompare.GetItemText( nItem, 1);
	
		strHypy=m_ctrlSymbolCompare.GetItemText( nItem, 2);
		if(!DeleteOneStockInfo(strCode,strName,strHypy))
		{
			MessageBox("删除失败","警告",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		m_ctrlSymbolCompare.DeleteItem(nItem);	
	}

	
	m_bDelete=TRUE;
	SetModified();
}

void CTaiCodeContradistinguishPage::OnModify() 
{

	if( m_ctrlSymbolCompare.GetNextItem(-1,LVIS_SELECTED)<0 )
	{
		AfxMessageBox("请先选择须修改的股票项！",MB_ICONASTERISK);
		return;
	}


	m_EditCode.EnableWindow(FALSE);
	m_EditName.EnableWindow(TRUE);
	m_EditPhonetic.EnableWindow(TRUE);


	GotoDlgCtrl(GetDlgItem(IDC_NAME));

	
	m_bModify=TRUE;
	SetModified();	
}

void CTaiCodeContradistinguishPage::OnRadiosz() 
{
	
	m_bAdd=FALSE;
	m_bDelete=FALSE;
	m_bModify=FALSE;

	((CEdit*)GetDlgItem(IDC_SYMBOL))->SetLimitText(4);  
	InitListCtrl();	
}

void CTaiCodeContradistinguishPage::OnRadiosh() 
{
	
	m_bAdd=FALSE;
	m_bDelete=FALSE;
	m_bModify=FALSE;

	((CEdit*)GetDlgItem(IDC_SYMBOL))->SetLimitText(6);  
	InitListCtrl();		
}

void CTaiCodeContradistinguishPage::OnClickSymbolcompare(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int Index=m_ctrlSymbolCompare.GetNextItem(-1,LVIS_SELECTED);
	if(Index<0)
	{
		*pResult = 0;
		return;
	}
    

	if(m_nOldSel!=Index)
	{
		m_nOldSel=Index;
		
		m_EditCode.EnableWindow(FALSE);
		m_EditName.EnableWindow(FALSE);
		m_EditPhonetic.EnableWindow(FALSE);
		m_bModify=FALSE;
	}	
	
	
	CString szSymbol,szName,szPhonetic;
	szSymbol=m_ctrlSymbolCompare.GetItemText(Index,0);
	m_EditCode.SetSel(0,-1);
	m_EditCode.ReplaceSel(szSymbol);
	szName  =m_ctrlSymbolCompare.GetItemText(Index,1);
	m_EditName.SetSel(0,-1);
	m_EditName.ReplaceSel(szName);
	szPhonetic  =m_ctrlSymbolCompare.GetItemText(Index,2);
	m_EditPhonetic.SetSel(0,-1);
	m_EditPhonetic.ReplaceSel(szPhonetic);

	
	GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_DEL)->EnableWindow(TRUE);

	*pResult = 0;
}

void CTaiCodeContradistinguishPage::OnChangeName() 
{
	CString strStockHypy;


	if(!m_bModify)
	{
		strStockHypy=m_ctrlSymbolCompare.GetItemText(m_nOldSel,2);
	
		if(!strStockHypy.IsEmpty())
			return;
	}

	
	CString strCode,strName;
	CTaiShanDoc* pDoc=GetDocument();
	
	m_EditCode.GetWindowText(strCode);
	m_EditName.GetWindowText(strName);
	char *pName,strHypy[10];
	pName=strName.GetBuffer(strName.GetLength());
	pDoc->StockNameConvert(pName,strHypy);
	m_EditPhonetic.SetWindowText(strHypy);

	
	m_ctrlSymbolCompare.SetItemText(m_nOldSel,1,pName);
	m_ctrlSymbolCompare.SetItemText(m_nOldSel,2,strHypy);
	ModifyOneStockInfo(strCode,strName,strHypy);
}


HBRUSH CTaiCodeContradistinguishPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( nCtlColor == CTLCOLOR_EDIT 
		&&(	   pWnd->GetDlgCtrlID() == IDC_SYMBOL 
			|| pWnd->GetDlgCtrlID() == IDC_NAME
			|| pWnd->GetDlgCtrlID() == IDC_PHONETIC )
	   )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(150,50,30));
		return (HBRUSH)m_SelfBrush.GetSafeHandle();
	}
	else
		return hbr;
}

void CTaiCodeContradistinguishPage::OnOK() 
{
	if( ((CPropertySheet*)GetParent())->GetActiveIndex( ) != 0 )
		return;

	SetModified(FALSE);	

	m_EditCode.EnableWindow(FALSE);
	m_EditName.EnableWindow(FALSE);
	m_EditPhonetic.EnableWindow(FALSE);

	UpdateData(TRUE);

	if(m_bAdd||m_bModify||m_bDelete)
	{
		CTaiShanDoc* pDoc=GetDocument();
		::SendMessage( pDoc->m_WsStock2000View->m_hWnd ,WM_USER_CHANGESTOCK,0,0);
	}
	CPropertyPage::OnOK();
}

CTaiShanDoc * CTaiCodeContradistinguishPage::GetDocument()
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	return pDoc;
}


BOOL CTaiCodeContradistinguishPage::AddOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc)
{
	CTaiShanDoc* pDoc=GetDocument();
	BOOL bResult=FALSE;
	pDoc->AddStockToKeyboard(strStockCode,strStockName,strStockPyjc);
    bResult=pDoc->m_sharesInformation.AddOneStockInfo(strStockCode,strStockName,strStockPyjc);
	return bResult;
}


BOOL CTaiCodeContradistinguishPage::ModifyOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc)
{
	CTaiShanDoc* pDoc=GetDocument();
	BOOL bResult=FALSE;
	pDoc->ModifyStockOfKeyboard(strStockCode,strStockName,strStockPyjc);
	bResult=pDoc->m_sharesInformation.MOdifyOneStockInfo(strStockCode,strStockName,strStockPyjc);
	return bResult;
}


BOOL CTaiCodeContradistinguishPage::DeleteOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc)
{
	CTaiShanDoc* pDoc=GetDocument();
	BOOL bResult=FALSE;
	pDoc->DeleteStockFromKeyboard(strStockCode,strStockName,strStockPyjc);
    bResult=pDoc->m_sharesInformation.DeleteOneStockInfo(strStockCode);
	return TRUE;
}

void CTaiCodeContradistinguishPage::StockTypeUpdate(CReportData  *Cdat)
{
	CTaiShanDoc* pDoc=GetDocument();


	if(strlen(Cdat->id)==6|| strlen(Cdat->id)==4)
	{
		if ( Cdat->id[0]<'5' || Cdat->id[0]=='7' )
		{
			Cdat->kind='7';
			if((Cdat->id[1]=='A') ||(Cdat->id[1]=='B' )||(Cdat->id[1]=='C'))
            {
				pDoc->m_lStockTypeCount[0]++;
			}
			else if(Cdat->id[0]=='7') 
			{
				pDoc->m_lStockTypeCount[6]++; 
			}
			else
			{
				pDoc->m_lStockTypeCount[6]++;
			}
		}
		else 
		{ 
			
			if ( Cdat->id[0] < '9' ) 
			{
				Cdat->kind='1';
				pDoc->m_lStockTypeCount[1]++;
			}
			else  
			{
				Cdat->kind='2';
				pDoc->m_lStockTypeCount[2]++;
			}
		}
	}

	
	else 
	{ 
		if(Cdat->id[0]=='9'&&Cdat->id[1]=='9') 
		{
		   Cdat->kind='6';
		   pDoc->m_lStockTypeCount[3]++;
           return ;
		}
		if ( Cdat->id[0]=='0' ||  Cdat->id[0]=='4' )
		{
			Cdat->kind='4';
			pDoc->m_lStockTypeCount[4]++;
		}
		else if(Cdat->id[0]=='2')
        {
			if( Cdat->id[1]=='A'||Cdat->id[1]=='B'||Cdat->id[1]=='C' )
			{
			   Cdat->kind='6';
			   pDoc->m_lStockTypeCount[3]++;
			}
			else
			{
			   Cdat->kind='5';
			   pDoc->m_lStockTypeCount[5]++;
			}
		}
		else 
		{
                if(Cdat->id[0]=='3'||Cdat->id[0]=='8')
				{
			        pDoc->m_lStockTypeCount[7]++;
				}
				else	
				{
			        pDoc->m_lStockTypeCount[7]++;
				}
				Cdat->kind='6';
		}
	}
	if ( Cdat->kind == '1' )       
		 Cdat->kind=1;
	else if ( Cdat->kind == '2' )       
		Cdat->kind=2; 
	else if ( Cdat->kind == '4' )         
		Cdat->kind=11;
	else if ( Cdat->kind == '5' )         
		Cdat->kind=12;
	else if ( Cdat->kind == '6' )       
		Cdat->kind=13;
	else if ( Cdat->kind == '7' )         
		Cdat->kind=20;
}

void CTaiCodeContradistinguishPage::OnKeydownSymbolcompare(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if(pLVKeyDow->wVKey==VK_DOWN||pLVKeyDow->wVKey==VK_UP)
	{
		int Index=m_ctrlSymbolCompare.GetNextItem(-1,LVIS_SELECTED);
		if(pLVKeyDow->wVKey==VK_DOWN)
		{
			Index++;
			if(Index>m_ctrlSymbolCompare.GetItemCount())	Index=0;
		}
		else
		{
			Index--;
			if(Index<0)
				Index=m_ctrlSymbolCompare.GetItemCount()-1;
		}
		if(Index<0)
		{
			*pResult = 0;
			return;
		}
    
		
		if(m_nOldSel!=Index)
		{
			m_nOldSel=Index;
			
			m_EditCode.EnableWindow(FALSE);
			m_EditName.EnableWindow(FALSE);
			m_EditPhonetic.EnableWindow(FALSE);
			m_bModify=FALSE;
		}	
		
		
		CString szSymbol,szName,szPhonetic;
		szSymbol=m_ctrlSymbolCompare.GetItemText(Index,0);
		m_EditCode.SetSel(0,-1);
		m_EditCode.ReplaceSel(szSymbol);
		szName  =m_ctrlSymbolCompare.GetItemText(Index,1);
		m_EditName.SetSel(0,-1);
		m_EditName.ReplaceSel(szName);
		szPhonetic  =m_ctrlSymbolCompare.GetItemText(Index,2);
		m_EditPhonetic.SetSel(0,-1);
		m_EditPhonetic.ReplaceSel(szPhonetic);

		
		GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DEL)->EnableWindow(TRUE);
	}
	*pResult = 0;
}
