// CTaiKlineDlgZhibiaoTongji.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgZhibiaoTongji.h"
#include "MainFrm.h"
#include "StructKlineView.h"
#include "CTaiShanDoc.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgZhibiaoTongji dialog


CTaiKlineDlgZhibiaoTongji::CTaiKlineDlgZhibiaoTongji(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgZhibiaoTongji::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgZhibiaoTongji)
	m_tmB = 0;
	m_tmE = 0;
	m_num = 0;
	m_fTotal = 0.0f;
	m_fAverage = 0.0f;
	m_fHigh = 0.0f;
	m_fLow = 0.0f;
	//}}AFX_DATA_INIT

	m_nSon = 0;

	m_pView = (CTaiShanKlineShowView*)pParent;
	m_nKlineKind2 = m_pView->m_nKlineKind2 ;
}


void CTaiKlineDlgZhibiaoTongji::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgZhibiaoTongji)
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_DateTime2);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTime);
	DDX_Control(pDX, IDC_COMBO1, m_conZhibiao);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tmB);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_tmE);
	DDX_Text(pDX, IDC_EDIT_TIME_NUM, m_num);
	DDX_Text(pDX, IDC_EDIT1, m_fTotal);
	DDX_Text(pDX, IDC_EDIT7, m_fAverage);
	DDX_Text(pDX, IDC_EDIT8, m_fHigh);
	DDX_Text(pDX, IDC_EDIT9, m_fLow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgZhibiaoTongji, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgZhibiaoTongji)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER3, OnDatetimechangeDatetimepicker3)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgZhibiaoTongji message handlers

void CTaiKlineDlgZhibiaoTongji::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	TongJi(1);
	
	*pResult = 0;
}

void CTaiKlineDlgZhibiaoTongji::OnDatetimechangeDatetimepicker3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	TongJi(1);
	
	*pResult = 0;
}

BOOL CTaiKlineDlgZhibiaoTongji::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	if(m_nKlineKind2 >0)
	m_pView->pKlineDrawing ->m_bInvertRect=true;
	else
	m_pView->pMin1Drawing->m_bInvertRect=true;

	

	m_DateTime .SetTime(&m_tmB);
	m_DateTime2 .SetTime(&m_tmE);
	if (m_pView->m_nKlineKind2==0)
	{
		int nYear=m_tmB.GetYear();
		int nMonth=m_tmB.GetMonth();
		int nDay=m_tmB.GetDay();
		CTime tm1(nYear,nMonth,nDay,9,30,0);
		CTime tm2(nYear,nMonth,nDay,15,0,0);
		m_DateTime.SetRange(&tm1,&tm2);
		m_DateTime2.SetRange(&tm1,&tm2);
	}
	else
	{
		CTime tm1(1980,1,1,8,0,0,0);
		CTime tm2(2036,1,1,8,0,0,0);
		m_DateTime.SetRange( &tm1,&tm2 );
		m_DateTime2.SetRange( &tm1,&tm2 );
	}

	if(m_pView->m_nKlineKind2 >0 &&m_pView->m_nKlineKind2 <5)
	{
		m_DateTime2.SetFormat("yyy'/'MM'/'dd','hh':'mm" );
		m_DateTime.SetFormat("yyy'/'MM'/'dd','hh':'mm" );
	}
	else if(m_pView->m_nKlineKind2 >=5)
	{
		m_DateTime2.SetFormat("yyy'/'MM'/'dd");
		m_DateTime.SetFormat("yyy'/'MM'/'dd");
	}
	else if(m_pView->m_nKlineKind2==0)
	{
		m_DateTime.SetFormat("H':'mm");
		m_DateTime2.SetFormat("H':'mm");
	}

	TongJi(0);

	for(int i=0;i<m_pView->pKlineDrawing ->m_dataFormular [this->m_nSon].numLine ;i++)
	{
		CString s = ":"+m_pView->pKlineDrawing ->m_lineName[m_nSon][i];
		s.MakeUpper ();
		this->m_conZhibiao .AddString (m_pView->m_infoInit.initIndex[m_nSon].nameIndex+s);
	}
	if(m_pView->pKlineDrawing ->m_dataFormular [this->m_nSon].numLine>0)
		m_conZhibiao.SetCurSel (0);
	return TRUE;  
}

void CTaiKlineDlgZhibiaoTongji::PostNcDestroy() 
{

	if(m_nKlineKind2 >0)
	m_pView->pKlineDrawing ->m_bInvertRect=false;
	else
	m_pView->pMin1Drawing->m_bInvertRect=false;
	m_pView->m_dlgZhibiao = 0;
	delete this;
	CDialog::PostNcDestroy();
}

void CTaiKlineDlgZhibiaoTongji::OnOK() 
{
	// 
	DestroyWindow();

}

void CTaiKlineDlgZhibiaoTongji::OnCancel() 
{

	DestroyWindow();

}

float CTaiKlineDlgZhibiaoTongji::TongJi(int nFlags)
{
	float high=0;
	float low=(float)10E20;
	float vol=0;

	int begin,end;
	CTaiKlineShowKline* pDok = m_pView->pKlineDrawing ;

	if(nFlags==0)
	{
		if(m_pView->m_rectHitPos.left<m_pView->m_rectHitPos.right)
		{
			begin=m_pView->m_rectHitPos.left*pDok->m_klinNumDefault /m_pView->m_rtKlineFiguer.rightX;
			end=m_pView->m_rectHitPos.right*pDok->m_klinNumDefault /m_pView->m_rtKlineFiguer.rightX;
		}
		else
		{
			begin=m_pView->m_rectHitPos.right*pDok->m_klinNumDefault /m_pView->m_rtKlineFiguer.rightX;
			end=m_pView->m_rectHitPos.left*pDok->m_klinNumDefault /m_pView->m_rtKlineFiguer.rightX;
		}

		if(m_pView->m_rectHitPos.top<m_pView->m_rtKlineFiguer .rtBlw [0].m_yBottom )
			m_nSon = 0;
		else if(m_pView->m_rectHitPos.top<m_pView->m_rtKlineFiguer .rtBlw [1].m_yBottom )
			m_nSon = 1;
		else if(m_pView->m_rectHitPos.top<m_pView->m_rtKlineFiguer .rtBlw [2].m_yBottom )
			m_nSon = 2;
		else if(m_pView->m_rectHitPos.top<m_pView->m_rtKlineFiguer .rtBlw [3].m_yBottom )
			m_nSon = 3;
		else if(m_pView->m_rectHitPos.top<m_pView->m_rtKlineFiguer .rtBlw [4].m_yBottom )
			m_nSon = 4;

		if(begin>=end)
			return -1;
		begin=pDok->m_footBegin+begin;
		if(begin>pDok->m_footEnd)
			return -1;
		end=pDok->m_footBegin+end;
		if(end>pDok->m_footEnd)
			end=pDok->m_footEnd;
	}
	else
	{
		CTime tmB(1970,1,1,9,30,0);
		CTimeSpan tmAll=this->m_tmB-tmB;
		int rtn;
		if(tmAll.GetTotalSeconds ()>m_pView->m_pkline[m_pView->m_nCountKline-1].day)
			rtn=m_pView->m_nCountKline-1;
		else
			rtn=m_pView->FindTimeK(tmAll.GetTotalSeconds ());
		if(rtn<0)
			return -1;
		begin=rtn;

		tmAll=this->m_tmE-tmB;
		if(tmAll.GetTotalSeconds ()>m_pView->m_pkline[m_pView->m_nCountKline-1].day)
			rtn=m_pView->m_nCountKline-1;
		else
			rtn=m_pView->FindTimeK(tmAll.GetTotalSeconds ());
		if(rtn<0)
			return -1;
		end=rtn;
		if(begin>end)
		{
			int tmp;
			tmp=begin;
			begin=end;
			end=tmp;
		}
	}

	int nZhibiao = this->m_conZhibiao .GetCurSel ();
	if(nZhibiao<0) nZhibiao = 0;

	float junJia=0;
	vol = 0;
	for(int i=begin;i<=end;i++)
	{
		float ff = pDok->m_dataFormular[m_nSon].line [nZhibiao].m_arrBE.line[i];
		if( ff>high)
			high = ff;
		if( ff<low)
			low = ff;
		vol+=ff;
	}
	junJia = vol;
	if((end-begin+1)==0)
		junJia=junJia;
	else
		junJia=junJia/(end-begin+1);
	junJia = (int(junJia*100+0.5))/100.0;

	this->m_fAverage = junJia;
	this->m_fHigh = high;
	this->m_fLow = low;
	this->m_fTotal  = vol;



	//---	
	if(nFlags==0)
	{
		CTime tmB(1970,1,1,9,30,0);
		CTime tmNow=tmB+m_pView->m_pkline[begin].day;
		this->m_tmB=tmNow;
		tmNow=tmB+m_pView->m_pkline[end].day;
		this->m_tmE=tmNow;
	}

	CString s;
	s.Format("%d",end-begin+1);
	this->m_num=end-begin+1;
	UpdateData(FALSE);

	pDok->m_nBeginFootTJ = begin;
	pDok->m_nEndFootTJ = end;
	
	if(begin>=0 && begin<m_pView->m_nCountKline 
		&& end>=0 && end<m_pView->m_nCountKline )
	{
		if(begin<pDok->m_footBegin || end>pDok->m_footEnd )
		{
			if(begin<pDok->m_footBegin )
				pDok->m_footBegin = begin;
			if(end>pDok->m_footEnd)
				pDok->m_footEnd=end;
			pDok->m_footCurrent=pDok->m_footEnd;
			pDok->m_klinNumDefault = pDok->m_footEnd - pDok->m_footBegin + 1;
		}

	}
	CTaiKlineDC* pMemDC = new CTaiKlineDC(m_pView,&m_pView->m_bit);
	pDok->DrawKlineFiguer (pMemDC);
	delete pMemDC;

	m_pView->RedrawWindowNoCacl ();

}

void CTaiKlineDlgZhibiaoTongji::OnSelchangeCombo1() 
{

	TongJi(1);
}
