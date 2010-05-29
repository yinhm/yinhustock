
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "SymbolContrast.h"

#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "CTaiStockInputDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolContrast property page

IMPLEMENT_DYNCREATE(CSymbolContrast, CPropertyPage)

CSymbolContrast::CSymbolContrast() : CPropertyPage(CSymbolContrast::IDD)
{
	pDoc = CMainFrame::m_taiShanDoc;
}

CSymbolContrast::~CSymbolContrast()
{
}

void CSymbolContrast::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CSymbolContrast)
	DDX_Control(pDX, 1022, m_button22);
	DDX_Control(pDX, 1012, m_button12);
	DDX_Control(pDX, 1004, m_button4);
	DDX_Control(pDX, 1000, m_ctrlSymbolCompare);
	DDX_Control(pDX, 1050, m_Tabctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolContrast, CPropertyPage)
	ON_WM_HELPINFO()
	//{{AFX_MSG_MAP(CSymbolContrast)
	ON_NOTIFY(TCN_SELCHANGE, 1050, OnSelchangezqsc)
	ON_BN_CLICKED(1012, OnAddNew)
	ON_BN_CLICKED(1022, OnEdit)
	ON_BN_CLICKED(1004, OnDelete)
	ON_NOTIFY(LVN_KEYDOWN, 1000, OnKeydown1000)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CSymbolContrast::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();


	m_Tabctrl.InsertItem(0, "上海");
	m_Tabctrl.InsertItem(1, "深圳");
	m_Tabctrl.InsertItem(2, "创业");

	char* arColumn[3] = { "股票代码", "股票名称", "拼音缩写" };
	LVCOLUMN listColumn;
	listColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	listColumn.fmt = LVCFMT_CENTER;

	CRect Rect;
	m_ctrlSymbolCompare.GetClientRect(Rect);
	int nWidth = (Rect.Width() - 16) / 3;

	for (int nColumn = 0; nColumn < 3; nColumn++)
		m_ctrlSymbolCompare.InsertColumn(nColumn, arColumn[nColumn], LVCFMT_CENTER, nWidth, nColumn);

	ListView_SetExtendedListViewStyle(m_ctrlSymbolCompare.m_hWnd, LVS_EX_FULLROWSELECT);
	InitListCtrl(TRUE);

	return TRUE;  
}

void CSymbolContrast::OnSelchangezqsc(NMHDR* pNMHDR, LRESULT* pResult) 
{

	if(m_Tabctrl.GetCurSel()==0)

		InitListCtrl(TRUE);
	if(m_Tabctrl.GetCurSel()==1)
	{

		InitListCtrl(FALSE);

	}
	if(m_Tabctrl.GetCurSel()==2)
	{
		m_ctrlSymbolCompare.DeleteAllItems();
	}

	*pResult = 0;
}

void CSymbolContrast::OnAddNew() 
{

	UpdateData(TRUE);
	CTaiStockInputDlg InputDlg;

	int wMarketType;

	if(m_Tabctrl.GetCurSel()==0)
	{
		wMarketType=SH_MARKET_EX;
		InputDlg.SetStockType(TRUE);
	}
	if(m_Tabctrl.GetCurSel()==1)
	{
		wMarketType=SZ_MARKET_EX;
		InputDlg.SetStockType(FALSE);
	}
	if(m_Tabctrl.GetCurSel()==2)
		return;

	InputDlg.nMarketType=wMarketType;
	int nKind=CSharesInformation::GetStockKind(wMarketType,InputDlg.m_strStockCode.GetBuffer(0));

	InputDlg.nKind=nKind;
	if(InputDlg.DoModal()==IDOK)
	{
		if(MessageBox("你真的决定增加新的股票代码吗?","警告",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2)==IDNO)
			return;

		int nIndex=0;
		m_ctrlSymbolCompare.InsertItem(nIndex,InputDlg.m_strStockCode);
		m_ctrlSymbolCompare.SetItemText(nIndex,1,InputDlg.m_strStockName);
		m_ctrlSymbolCompare.SetItemText(nIndex,2,InputDlg.m_strStockPyjc);

		m_ctrlSymbolCompare.SetItemState(nIndex,LVIS_SELECTED,LVIS_SELECTED);

		nKind=CSharesInformation::GetStockKind(wMarketType,InputDlg.m_strStockCode.GetBuffer(0));
		if(AddOneStockInfo(InputDlg.m_strStockCode,InputDlg.m_strStockName,InputDlg.m_strStockPyjc,nKind))
			::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_CHANGESTOCK,0,0);

	}
}

BOOL CSymbolContrast::AddOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc,int nKind)
{
	BOOL bResult=FALSE;
	pDoc->AddStockToKeyboard(strStockCode,nKind,strStockName,strStockPyjc);
	bResult=pDoc->m_sharesInformation.AddOneStockInfo(strStockCode,strStockName,strStockPyjc,nKind);
	return bResult;
}

void CSymbolContrast::InitListCtrl(BOOL bShanghai)
{

	LVITEM listItem;
	listItem.mask=LVIF_TEXT|LVIF_IMAGE;
	m_ctrlSymbolCompare.DeleteAllItems();
	int nTimes;
	int i,j;
	int nStockType;
	if(bShanghai)
	{
		nTimes=0;
		for(i=0;i<5;i++)
		{
			nStockType=i;
			if(i==3)
				nStockType=SHZQ;
			if(i==4)
				nStockType=SHJIJIN;

			for(j=0;j<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);j++)
			{
				CReportData *pDat1=NULL;
				pDoc->m_sharesInformation.GetStockItem(nStockType,j,pDat1);
				if(pDat1)
				{
					m_ctrlSymbolCompare.InsertItem(nTimes,pDat1->id);
					m_ctrlSymbolCompare.SetItemText(nTimes,1,pDat1->name);
					m_ctrlSymbolCompare.SetItemText(nTimes,2,pDat1->Gppyjc);
					nTimes++;
				}
				if(nTimes==20)
					m_ctrlSymbolCompare.Update(20);
			}
		}
	}
	else 
	{
		nTimes=0;
		for(i=3;i<8;i++)
		{
			nStockType=i;
			if(i==6)
				nStockType=SZZQ;
			if(i==7)
				nStockType=SZJIJIN;
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
				if(nTimes==20)
					m_ctrlSymbolCompare.Update(20);
			}
		}
	}

}

void CSymbolContrast::OnEdit() 
{

	POSITION pos=m_ctrlSymbolCompare.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("请先选择须修改的股票项!",MB_ICONASTERISK);
		return;
	}
	int nItem = m_ctrlSymbolCompare.GetNextSelectedItem(pos);


	CString strCode,strName,strHypy;
	strCode=m_ctrlSymbolCompare.GetItemText(nItem,0);
	strName=m_ctrlSymbolCompare.GetItemText(nItem,1);
	strHypy=m_ctrlSymbolCompare.GetItemText(nItem,2);
	int nKind,nMarketType;
	int nTab=this->m_Tabctrl.GetCurSel();
	switch(nTab)
	{
	case 0:
		nMarketType=SH_MARKET_EX;
		break;
	case 1:
		nMarketType=SZ_MARKET_EX;
		break;
	default:    
		return;
		break;	
	}
	nKind=CSharesInformation::GetStockKind(nMarketType,strCode.GetBuffer(0));
	CTaiStockInputDlg EditDlg;
	EditDlg.nKind=nKind;
	EditDlg.SetStockInfo(strCode,strName,strHypy);
	EditDlg.SetEditFlag(TRUE);

	EditDlg.SetStockType(!m_Tabctrl.GetCurSel());//
	if(EditDlg.DoModal()==IDOK)
	{
		m_ctrlSymbolCompare.DeleteItem(nItem);
		m_ctrlSymbolCompare.InsertItem(nItem,EditDlg.m_strStockCode);
		m_ctrlSymbolCompare.SetItemText(nItem,1,EditDlg.m_strStockName);
		m_ctrlSymbolCompare.SetItemText(nItem,2,EditDlg.m_strStockPyjc);

		if(!ModifyOneStockInfo(EditDlg.m_strStockCode,EditDlg.m_strStockName,EditDlg.m_strStockPyjc,nKind))
		{
			MessageBox("修改失败!","警告",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
	}
	else 
		return;


}

void CSymbolContrast::OnDelete() 
{

	POSITION pos=m_ctrlSymbolCompare.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("请首先选择须删除的股票项!",MB_ICONASTERISK);
		return;
	}
	else 
	{
		if( AfxMessageBox("真的要删除这支股票吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
			return;

		int nItem = m_ctrlSymbolCompare.GetNextSelectedItem(pos);
		CString strCode,strName,strHypy;
		strCode=m_ctrlSymbolCompare.GetItemText(nItem,0);
		strName=m_ctrlSymbolCompare.GetItemText(nItem,1);
		strHypy=m_ctrlSymbolCompare.GetItemText(nItem,2);
		int nKind,nMarketType;
		int nTab=this->m_Tabctrl.GetCurSel();
		switch(nTab)
		{
		case 0:
			nMarketType=SH_MARKET_EX;
			break;
		case 1:
			nMarketType=SZ_MARKET_EX;
			break;
		default:    
			return;
			break;	
		}
		nKind=CSharesInformation::GetStockKind(nMarketType,strCode.GetBuffer(0));
		if(strCode==CSharesCompute::GetIndexSymbol(0)||strCode==CSharesCompute::GetIndexSymbol(1))
		{
			AfxMessageBox("该股票不允许删除!");
			return;
		}
		if(CTaiShanKlineShowView::CheckStockSymbol(strCode)==false)
		{
			AfxMessageBox("该股票信息正在使用，您不能删除!");
			return;
		}
		if(!ConDeleteOneStockInfo(strCode,strName,strHypy,nKind))
		{
			MessageBox("删除失败!","警告",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		m_ctrlSymbolCompare.DeleteItem(nItem);
		m_bDelete=TRUE;
		::SendMessage(pDoc->m_WsStock2000View->m_hWnd,WM_USER_CHANGESTOCK,0,0);


	}

}

BOOL CSymbolContrast::ConDeleteOneStockInfo(CString strCode, CString strName, CString strHypy,int nKind)
{
	BOOL bResult=FALSE;
	pDoc->DeleteStockFromKeyboard(strCode,nKind,strName,strHypy);
	bResult=pDoc->m_sharesInformation.DeleteOneStockInfo(strCode,nKind);
	return TRUE;
}

BOOL CSymbolContrast::ModifyOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc,int nKind)
{
	BOOL bResult=FALSE;
	pDoc->ModifyStockOfKeyboard(strStockCode,nKind,strStockName,strStockPyjc);
	bResult=pDoc->m_sharesInformation.MOdifyOneStockInfo(strStockCode,strStockName,strStockPyjc,nKind);
	return bResult;
}

void CSymbolContrast::OnKeydown1000(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if(pLVKeyDow->wVKey==VK_DELETE)
		this->OnDelete();
	*pResult = 0;
}

BOOL CSymbolContrast::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	DoHtmlHelp(this);return TRUE;
}
