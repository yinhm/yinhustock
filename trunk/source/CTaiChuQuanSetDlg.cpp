// CTaiChuQuanSetDlg.cpp : implementation file
// by 俞明录

#include "stdafx.h"
#include "resource.h"

#include "CTaiChuQuanSetDlg.h"


#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanSetDlg dialog

CTaiChuQuanSetDlg::CTaiChuQuanSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiChuQuanSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiChuQuanSetDlg)
	m_CqDate =_T("");
	m_fGive =0.0f;
	m_fPrice =0.0f;
	m_fAlloc =0.0f;
	m_kind = 0;
	m_fDivid = 0.0f;
	//}}AFX_DATA_INIT
	pView =NULL;
	CTime tm=CTime::GetCurrentTime();
	m_timet=tm.GetTime();
	m_fromWhich=0;
}

void CTaiChuQuanSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiChuQuanSetDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_EDIT_CQGIVE, m_fGive);
	DDX_Text(pDX, IDC_EDIT_CQDIVID, m_fDivid);
	DDX_Text(pDX, IDC_EDIT_CQPRICE, m_fPrice);
	DDX_Text(pDX, IDC_EDIT_CQALLOC, m_fAlloc);
	DDX_Radio(pDX, IDC_RADIO_FIR, m_kind);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaiChuQuanSetDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiChuQuanSetDlg)
	ON_EN_CHANGE(IDC_EDIT_CQGIVE, OnChangeEditCqgive)
	ON_EN_CHANGE(IDC_EDIT_CQALLOC, OnChangeEditCqalloc)
	ON_EN_CHANGE(IDC_EDIT_CQDIVID, OnChangeEditCqdivid)
	ON_EN_CHANGE(IDC_EDIT_CQPRICE, OnChangeEditCqprice)
	//}}AFX_MSG_MAP
	ON_NOTIFY(DTN_DATETIMECHANGE,IDC_DATETIME_1, OnDTNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CTaiChuQuanSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	
	if( m_OperateID == 'M')
	   this->SetWindowText("修改除权信息");
	CWnd *pWnd = GetDlgItem(IDC_EDIT_CQDATE);
	if(!pWnd) return TRUE;

	CRect rc;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); 
	ScreenToClient(rc);
	m_DateTime.Create(WS_CHILD|WS_VISIBLE|DTS_LONGDATEFORMAT|DTS_UPDOWN,rc,this,IDC_DATETIME_1);
	
	CTime pMinRange = CTime(1988,9,1,1,1,1);
	CTime pMaxRange = CTime(2037,9,1,1,1,1);
	this->m_DateTime.SetRange( &pMinRange, &pMaxRange );


	if(m_timet!=0&&m_timet!=1)
	{
		CTime tm(m_timet);
		m_DateTime .SetTime(&tm);
	}
	GetDlgItem(IDC_RADIO_FIR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SEC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_END)->ShowWindow(SW_HIDE);
	return TRUE;  
}

void CTaiChuQuanSetDlg::OnOK() 
{
	UpdateData(TRUE);


	if(m_fGive==0&&m_fAlloc==0&&m_fPrice==0&&m_fDivid==0)
		if( AfxMessageBox("该数据无效，是否重新设置？",MB_YESNO|MB_ICONQUESTION)==IDYES)
			return;

	if(m_fAlloc!=0 &&m_fPrice==0||m_fAlloc==0&&m_fPrice!=0)
	{
		AfxMessageBox("配股数和配股价\n同时为零或同时不为零!");
		return;
	}

	CTime tm(m_timet);
	m_DateTime .GetTime(tm);

	CTime tmB(1970,1,1,9,30,0);
	int nYear;
	int nMon,nDay;
	nYear=tm.GetYear ();
	nMon=tm.GetMonth ();
	nDay=tm.GetDay ();
	CTime tm2(nYear,nMon,nDay,9,30,0);
	CTimeSpan tmAll=tm2-tmB;
	m_timet=tmAll.GetTotalSeconds();

	m_DateTime.GetTime(tm);
	int nDy = tm.GetDayOfWeek ();
	if(nDy == 1|| nDy ==7)
		if( AfxMessageBox("日期是星期六或星期天，是否重新选择？",MB_ICONQUESTION|MB_YESNO)==IDYES )
			return;
	CDialog::OnOK();
}

void CTaiChuQuanSetDlg::OnChangeEditCqgive() 
{
	ValidateCheck(IDC_EDIT_CQGIVE);
}

void CTaiChuQuanSetDlg::OnChangeEditCqalloc() 
{
	ValidateCheck(IDC_EDIT_CQALLOC);
}

void CTaiChuQuanSetDlg::OnChangeEditCqdivid() 
{
	ValidateCheck(IDC_EDIT_CQDIVID);
}

void CTaiChuQuanSetDlg::OnChangeEditCqprice() 
{
	ValidateCheck(IDC_EDIT_CQPRICE);
}

void CTaiChuQuanSetDlg::ValidateCheck(UINT nID)
{
	CString str;
	GetDlgItem(nID)->GetWindowText(str);
	for( int j=0;j<str.GetLength();j++)
	{
		if( !_istdigit( str[j] ) && str[j]!='.' )
		{
			AfxMessageBox("请输入一个非负数。",MB_ICONASTERISK);
			this->GotoDlgCtrl(GetDlgItem(nID));
			break;
		}
	}
}

void CTaiChuQuanSetDlg::OnDTNotify(NMHDR *pHdr, LRESULT * pRes)
{
	if(m_fromWhich == 1)//
	{
		CTaiShanDoc* pDoc=CMainFrame::m_taiShanDoc; 

		if(pView == NULL)return;

		CTime tm;
		m_DateTime .GetTime(tm);
		int nDy = tm.GetDayOfWeek ();
		if(nDy == 1|| nDy ==7)
		{
			this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("所选日期是星期六或星期天，请重新选择！");
		}
		else
			this->GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("");
		int nFinded=-1;
		int nYear=tm.GetYear ();
		int nMon=tm.GetMonth  ();
		int nDay=tm.GetDay  ();
		CTime tmB(1970,1,1,8,0,0);
		for(int i=0;i<pView->pKlineDrawing ->m_PowerArray .GetSize();i++)
		{
			CTime tmEnd=tmB+pView->pKlineDrawing ->m_PowerArray[i].nTime;
			if(tmEnd.GetDay()==nDay&&tmEnd.GetMonth()==nMon&&tmEnd.GetYear()==nYear)
			{
				 m_timet=pView->pKlineDrawing ->m_PowerArray[i].nTime;
				 m_fAlloc=pView->pKlineDrawing ->m_PowerArray[i].fAllocate*10;
				 m_fDivid=pView->pKlineDrawing ->m_PowerArray[i].fDividend*10;
				 m_fPrice=pView->pKlineDrawing ->m_PowerArray[i].fAllocatePrice;
				 m_fGive=pView->pKlineDrawing ->m_PowerArray[i].fGive*10;
				 m_kind=pView->pKlineDrawing ->m_PowerArray[i].nFlags;
				if( m_kind<0 || m_kind>2)
					 m_kind=0;
				nFinded=i;
				break;
			}
		}
		if(nFinded<0)
		{
			m_fAlloc =0.0f;
			m_fDivid =0.0f;
			m_fPrice =0.0f;
			m_fGive  =0.0f;
			m_kind = 0;
		}
		UpdateData(FALSE);
	}
}
