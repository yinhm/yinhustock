// SJGLCQSJ.cpp : implementation file
//

#include "stdafx.h"
#include "SJGLCQSJ.h"

#include "MainFrm.h"

#include "CTaiChuQuanSetDlg.h"
#include "OutinPower.h"

#include "KEYBRODEANGEL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanPage property page

IMPLEMENT_DYNCREATE(CTaiChuQuanPage, CPropertyPage)

CTaiChuQuanPage::CTaiChuQuanPage() : CPropertyPage(CTaiChuQuanPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiChuQuanPage)
	m_strStaticName = _T("");
	m_strStockCode = _T("");
	//}}AFX_DATA_INIT
}

CTaiChuQuanPage::~CTaiChuQuanPage()
{
}

void CTaiChuQuanPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiChuQuanPage)
	DDX_Control(pDX, IDC_LIST_CHUQUAN, m_ctrlChuQuan);
	DDX_Text(pDX, IDC_STATIC_NAME, m_strStaticName);
	DDX_Text(pDX, IDC_EDIT_KEY, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiChuQuanPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiChuQuanPage)
	ON_EN_CHANGE(IDC_EDIT_KEY, OnChangeEditKey)
	ON_BN_CLICKED(IDC_DELETECHUQUAN, OnDeleteInfo)
	ON_BN_CLICKED(IDC_ADD_ITEM, OnAddItem)
	ON_BN_CLICKED(IDC_MODIFYCQ, OnModifyCq)
	ON_BN_CLICKED(IDC_BUTTON_LEADOUT, OnButtonLeadout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanPage message handlers

BOOL CTaiChuQuanPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	LVCOLUMN listColumn;
	char * arColumn[5]={"除权日期","送股数","配股数","配股价", "分红"};
	listColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt=LVCFMT_CENTER;
	listColumn.cx=76;
	for( int nColumn=0;nColumn<5;nColumn++)
	{
		listColumn.iSubItem=nColumn;
		listColumn.pszText=arColumn[nColumn];
		m_ctrlChuQuan.InsertColumn(nColumn, &listColumn);
	}

	
	ListView_SetExtendedListViewStyle(  m_ctrlChuQuan.m_hWnd, LVS_EX_FULLROWSELECT);
	COLORREF color=RGB(255,255,220);
    m_ctrlChuQuan.SetBkColor( color );
	m_ctrlChuQuan.SetTextBkColor( color );


	EnableButton(FALSE);

	for(int i=0;i<80;i++)
		m_nChuQuanKind[i]=0;
	return TRUE;  
}

void CTaiChuQuanPage::OnChangeEditKey() 
{

	CString strStockCode;
	GetDlgItem(IDC_EDIT_KEY)->GetWindowText(strStockCode);         
	

	if(strStockCode.GetLength()==4||strStockCode.GetLength()==6)
	{
	
		CTaiShanDoc * pDoc=GetDocument();
		CReportData *Cdat=NULL;

		if(pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),Cdat))
		{
			
			EnableButton(TRUE);
			
		
			m_strStaticName=Cdat->name;
			GetDlgItem(IDC_STATIC_NAME)->SetWindowText(Cdat->name);

			
			Split *pSplit=NULL;
		
			int nTotleTimes=GetChuQuanInfo(strStockCode.GetBuffer(0),pSplit);
			if((nTotleTimes>0)&&(pSplit!=NULL))
			{
				POWER Power;
				for(int i=0;i<nTotleTimes;i++)
				{
					CTime time=CTime::GetCurrentTime();
					CString szTime=time.Format("%Y/%m/%d");

					Power.fAllocate=pSplit[i].Allocate;
					Power.fAllocatePrice=pSplit[i].AllocatePrice;
					Power.fDividend=pSplit[i].Bonus;
					Power.fGive=pSplit[i].Give;
					Power.nFlags=pSplit[i].nFlags;
					Power.nTime=pSplit[i].nTime;
					InsertItem(m_ctrlChuQuan.GetItemCount(),szTime,Power);
					m_nChuQuanKind[i]=Power.nFlags;
				}
			}		
		}
	}
	else 
	{
		m_strStaticName="";
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("");
		EnableButton(FALSE);
		if(m_ctrlChuQuan.GetItemCount()>0)
		{
			m_ctrlChuQuan.DeleteAllItems( );
			for(int i=0;i<80;i++)
				m_nChuQuanKind[i]=0;
		}
	}
}

void CTaiChuQuanPage::OnDeleteInfo() 
{
	CTaiShanDoc * pDoc=GetDocument();
	int Index=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( Index<0 )
	{
		AfxMessageBox("请选择其中一项",MB_ICONASTERISK);
		return;
	}

	if( AfxMessageBox("真的要删除吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDNO )
		return;
    UpdateData(TRUE);
	m_nChuQuanKind[Index]=0;
	DeleteChuQuanInfo(m_strStockCode,Index);
	m_ctrlChuQuan.DeleteItem( Index );
}

void CTaiChuQuanPage::OnAddItem() 
{
	CTaiShanDoc * pDoc=GetDocument();	
	LVITEM   listItem;
	listItem.mask=LVIF_TEXT|LVIF_IMAGE;
	UpdateData(TRUE);
	CTaiChuQuanSetDlg AddChuquan;
	if( AddChuquan.DoModal() ==IDOK )
	{
		int nTimes=m_ctrlChuQuan.GetItemCount();
		
		float fGive =AddChuquan.m_fGive/10.0f;
		float fAlloc=AddChuquan.m_fAlloc/10.0f;
		float fPrice=AddChuquan.m_fPrice;
		float fDivid=AddChuquan.m_fDivid/10.0f;
		
		if( fGive==0&&fAlloc==0&&fPrice==0&&fDivid==0)return;

		if( !IsAlreadyChuQuan(AddChuquan.m_timet) )
		{
			AfxMessageBox("同一天只能除权一次。",MB_ICONASTERISK);
			return;
		}

		CTime tm(AddChuquan.m_timet);
		CString str;

		str=tm.Format("%Y/%m/%d");
		POWER Power;
		Power.nTime=tm.GetTime();
		Power.fGive=fGive;
		Power.fAllocate=fAlloc;
		Power.fAllocatePrice=fPrice;
		Power.fDividend=fDivid;
		Power.nFlags=AddChuquan.m_kind;
		m_nChuQuanKind[nTimes]=Power.nFlags;
	
		InsertItem(nTimes,str,Power);
	
		AddChuQuanInfo(m_strStockCode,&Power);
	}
}

///////////////////////////////////////////////////////////////

void CTaiChuQuanPage::OnModifyCq() 
{
	CTaiChuQuanSetDlg AddChuquan;
	AddChuquan.m_OperateID='M';
	UpdateData(TRUE);

	int nSel=m_ctrlChuQuan.GetNextItem(-1,LVIS_SELECTED);
	if( nSel<0 )
	{
		AfxMessageBox("请选择其中一项.",MB_ICONASTERISK);
		return;
	}

	CString strDate=m_ctrlChuQuan.GetItemText( nSel, 0);
	strDate=strDate.Left(4)+strDate.Mid(5,2)+strDate.Right(2);

	int year=atoi(strDate)/10000;
	int month=(atoi(strDate)/100)%100;
	int day=atoi(strDate)%100;
	CTime tm(year,month,day,8,30,0);

	CString szGive =m_ctrlChuQuan.GetItemText( nSel, 1);
	szGive.TrimLeft();
	CString szAlloc=m_ctrlChuQuan.GetItemText( nSel, 2);
	szAlloc.TrimLeft();
	CString szPrice=m_ctrlChuQuan.GetItemText( nSel, 3);
	szPrice.TrimLeft();
	CString szDivid=m_ctrlChuQuan.GetItemText( nSel, 4);
	szDivid.TrimLeft();

	float fGive =(float(atof( szGive )))*10.0f;
	float fAlloc=(float(atof( szAlloc)))*10.0f;
	float fPrice=(float(atof( szPrice)));
	float fDivid=(float(atof( szDivid)))*10.0f;

	AddChuquan.m_timet=tm.GetTime();
	AddChuquan.m_fGive =fGive;
	AddChuquan.m_fAlloc=fAlloc;
	AddChuquan.m_fPrice=fPrice;
	AddChuquan.m_fDivid=fDivid;
    AddChuquan.m_kind=m_nChuQuanKind[nSel];

	if( AddChuquan.DoModal() == IDOK )
	{
		if( !IsAlreadyChuQuan(AddChuquan.m_timet) )
		{
			AfxMessageBox("同一天只能除权一次。",MB_ICONASTERISK);
			return;
		}

		m_ctrlChuQuan.DeleteItem( nSel );
	
		CTime tm(AddChuquan.m_timet);
		POWER Power;
		Power.nTime=tm.GetTime();
		Power.fGive=AddChuquan.m_fGive/10.0f;
		Power.fAllocate=AddChuquan.m_fAlloc/10.0f;
		Power.fAllocatePrice=AddChuquan.m_fPrice;
		Power.fDividend=AddChuquan.m_fDivid/10.0f;
		Power.nFlags=AddChuquan.m_kind;
		m_nChuQuanKind[nSel]=Power.nFlags;

		if( Power.fGive==0&&Power.fGive==0&&Power.fAllocate==0&&Power.fDividend==0)
			return;

		CString str;
		str=tm.Format("%Y/%m/%d");
		InsertItem(nSel,str,Power);
		ModifyChuQuanInfo(m_strStockCode,nSel,&Power);
	}
}

void CTaiChuQuanPage::OnButtonLeadout() 
{
	CTaiChuQuanOutInDlg dlg;
	dlg.DoModal();
}

BOOL CTaiChuQuanPage::IsAlreadyChuQuan(int nSeconds)
{
	CTime tm(nSeconds);
	int DateToAdd=atoi(tm.Format("%Y%m%d"));
	for( int i=0;i<this->m_ctrlChuQuan.GetItemCount();i++)
	{
		CString str=this->m_ctrlChuQuan.GetItemText(i,0);
		if( DateToAdd == atoi(str.GetBuffer(0)))
			return FALSE;
	}
	return TRUE;
}

BOOL CTaiChuQuanPage::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message==WM_CHAR)
	{
		if(pMsg->wParam>='0'&&pMsg->wParam<='9')
		{
			CTaiKeyBoardAngelDlg m_diakeybrode;
			char input=pMsg->wParam;
			m_diakeybrode.input=input;
			m_diakeybrode.ischar=false;

			CRect rect;
			GetDlgItem(IDC_EDIT_KEY)->GetWindowRect(rect);

			m_diakeybrode.SetStartPos(TRUE,rect.left,rect.bottom);
			m_diakeybrode.DoModal();
			if(m_diakeybrode.isresultok)
			{
				CString strResult;            
				strResult=m_diakeybrode.result;
				m_strStockCode=strResult;
				if(m_ctrlChuQuan.GetItemCount()>0)
				{
					m_ctrlChuQuan.DeleteAllItems( );
					for(int i=0;i<80;i++)
						m_nChuQuanKind[i]=0;
				}
				GetDlgItem(IDC_EDIT_KEY)->SetWindowText(strResult);
			}
			return TRUE;
		}
	}
	return CPropertyPage::PreTranslateMessage(pMsg);
}

CTaiShanDoc * CTaiChuQuanPage::GetDocument()
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	ASSERT_VALID(pDoc);
	return pDoc;
}

void CTaiChuQuanPage::InsertItem(int nItem,CString strNo, POWER nPower)
{
	CString str;

	m_ctrlChuQuan.InsertItem (nItem, strNo);

	str.Format("%6.2f",nPower.fGive);
	m_ctrlChuQuan.SetItemText(nItem,1,str);

	str.Format("%6.2f",nPower.fAllocate);
	m_ctrlChuQuan.SetItemText(nItem,2,str);

	str.Format("%6.2f",nPower.fAllocatePrice);
	m_ctrlChuQuan.SetItemText(nItem,3,str);

	str.Format("%6.2f",nPower.fDividend);
	m_ctrlChuQuan.SetItemText(nItem,4,str);
}

void CTaiChuQuanPage::AddChuQuanInfo(CString strStockCode, POWER *pPower)
{
	CTaiShanDoc * pDoc=GetDocument();
	Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.AddChuQuanInfo(strStockCode,&pSplit);
}

void CTaiChuQuanPage::ModifyChuQuanInfo(CString strStockCode, int nWhichItem, POWER *pPower)
{
	CTaiShanDoc * pDoc=GetDocument();
	Split pSplit;
	memcpy(&pSplit,pPower,sizeof(Split));
	pDoc->m_sharesInformation.ModifyChuQuanInfo(strStockCode,nWhichItem,&pSplit);
}

void CTaiChuQuanPage::DeleteChuQuanInfo(CString strStockCode, int nWhichItem)
{
	CTaiShanDoc * pDoc=GetDocument();
	pDoc->m_sharesInformation.DeleteChuQuanInfo(strStockCode,nWhichItem);
}

int CTaiChuQuanPage::GetChuQuanInfo(CString strStockCode, PSplit &pSplit)
{
	CTaiShanDoc * pDoc=GetDocument();
	return pDoc->m_sharesInformation.GetChuQuanInfo(strStockCode,pSplit);
}

void CTaiChuQuanPage::EnableButton(BOOL bEnable)
{
	GetDlgItem(IDC_ADD_ITEM)->EnableWindow(bEnable);
	GetDlgItem(IDC_DELETECHUQUAN)->EnableWindow(bEnable);
	GetDlgItem(IDC_MODIFYCQ)->EnableWindow(bEnable);
}

