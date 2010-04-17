// CTaiKlineDialogChangKLine.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDialogChangKLine.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiChuQuanSetDlg.h"
#include "CTaiKlineDlgBaseInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogChangKLine dialog


CTaiKlineDialogChangKLine::CTaiKlineDialogChangKLine(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDialogChangKLine::IDD, pParent)
{
	pView = (CTaiShanKlineShowView* )pParent;
	ASSERT(pView!=NULL);
	//{{AFX_DATA_INIT(CTaiKlineDialogChangKLine)
	m_close = 0.0f;
	m_low = 0.0f;
	m_open = 0.0f;
	m_high = 0.0f;
	m_vol = 0;
	m_amountStr = _T("");
	m_fInnerPartVol = 0.0f;
	m_fOuterPartVol = 0.0f;
	//}}AFX_DATA_INIT
	m_bInit = false;
}


void CTaiKlineDialogChangKLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDialogChangKLine)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON_POWER, m_power);
	DDX_Control(pDX, IDC_BUTTON_EVENT_BIG, m_big);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_delete);
	DDX_Control(pDX, IDC_EDIT7, m_floatEdit7);
	DDX_Control(pDX, IDC_EDIT1, m_floatEdit9);
	DDX_Control(pDX, IDC_EDIT_VOL_CHG, m_floatEdit8);
	DDX_Control(pDX, IDC_EDIT_OPEN_CHG, m_floatEdit5);
	DDX_Control(pDX, IDC_EDIT_LOW_CHG, m_floatEdit4);
	DDX_Control(pDX, IDC_EDIT_HIGH_CHG, m_floatEdit3);
	DDX_Control(pDX, IDC_EDIT_CLOSE_CHG, m_floatEdit2);
	DDX_Control(pDX, IDC_EDIT_AMOUNT_CHG, m_conAmount);
	DDX_Control(pDX, IDC_DTPICKER1, m_dtPicker);
	DDX_Text(pDX, IDC_EDIT_CLOSE_CHG, m_close);
	DDV_MinMaxFloat(pDX, m_close, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_LOW_CHG, m_low);
	DDV_MinMaxFloat(pDX, m_low, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_OPEN_CHG, m_open);
	DDV_MinMaxFloat(pDX, m_open, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_HIGH_CHG, m_high);
	DDV_MinMaxFloat(pDX, m_high, 0, 100000);
	DDX_Text(pDX, IDC_EDIT_VOL_CHG, m_vol);
	DDV_MinMaxInt(pDX, m_vol, 0, 1000000000);
	DDX_Text(pDX, IDC_EDIT_AMOUNT_CHG, m_amountStr);
	DDX_Text(pDX, IDC_EDIT1, m_fInnerPartVol);
	DDX_Text(pDX, IDC_EDIT7, m_fOuterPartVol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDialogChangKLine, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDialogChangKLine)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_POWER, OnButtonPower)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_BIG, OnButtonEventBig)
	ON_EN_CHANGE(IDC_EDIT_HIGH_CHG, OnChangeEditHighChg)
	ON_EN_CHANGE(IDC_EDIT_LOW_CHG, OnChangeEditLowChg)
	ON_EN_CHANGE(IDC_EDIT_AMOUNT_CHG, OnChangeEditAmountChg)
	ON_EN_CHANGE(IDC_EDIT_OPEN_CHG, OnChangeEditOpenChg)
	ON_EN_CHANGE(IDC_EDIT_CLOSE_CHG, OnChangeEditCloseChg)
	ON_EN_CHANGE(IDC_EDIT_VOL_CHG, OnChangeEditVolChg)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogChangKLine message handlers

BOOL CTaiKlineDialogChangKLine::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	SetWindowText("编辑历史数据");


	DATE newValue;
	COleDateTime oleTm;
	oleTm.SetDateTime(2037, 1,1,9,30, 0); 
	newValue=(DATE)oleTm;
	m_dtPicker.SetMaxDate(newValue);
	oleTm.SetDateTime(1988, 1,1,9,30, 0); 
	newValue=(DATE)oleTm;
	m_dtPicker.SetMinDate(newValue);

	if(pView->m_nKlineKind2 >0 &&pView->m_nKlineKind2 <5)
	{
		m_dtPicker.SetCustomFormat("yyy'/'MM'/'dd','hh':'mm" );
		m_dtPicker.EnableWindow( FALSE );
	}
	else if(pView->m_nKlineKind2 >=5)
	{
		m_dtPicker.SetCustomFormat("yyy'/'MM'/'dd");
	}
	
	LoadData( m_foot);
	UpdateData(FALSE);

	m_bInit = true;

	return TRUE;  
}

void CTaiKlineDialogChangKLine::LoadData(int foot)
{
	
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	m_foot=foot;

	m_close=pView->m_pkline[foot].close;
	m_high=pView->m_pkline[foot].high;
	m_low=pView->m_pkline[foot].low;
	m_open=pView->m_pkline[foot].open;

	if(m_high<m_close ||m_high<m_low ||m_high<m_open)
		m_high = max(max(m_close,m_open),m_low);
	if(m_low>m_close ||m_low>m_high ||m_low>m_open)
		m_low = min(min(m_close,m_open),m_high);

	m_vol=(int)pView->m_pkline[foot].vol;
	m_amount=pView->m_pkline[foot].amount/10000;
	m_amountStr.Format("%.1f",m_amount);

	if(!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol))
	{
		float f = pView->m_pkline[foot].vol
			-pView->m_pkline[foot].volPositive;
		if(pView->m_pkline[foot].volPositive>pView->m_pkline[foot].vol)
			f = -1;
		if(pView->m_pkline[foot].volPositive<0)
			f = -1;
		if(f>=0)
		{
			m_fInnerPartVol = f;
			m_fOuterPartVol = pView->m_pkline[foot].volPositive;
		}
		else
		{
			m_fInnerPartVol = pView->m_pkline[foot].vol/2;
			m_fOuterPartVol = m_fInnerPartVol;
		}
		if(m_fInnerPartVol<0.00001) m_fInnerPartVol = 0;
		if(m_fOuterPartVol<0.00001) m_fOuterPartVol = 0;

	}

	CString s;



	CTime tmB(1970,1,1,9,30,0);
	CTime tmNow;
	CTime tmEnd;
	CTimeSpan tmAll(pView->m_pkline[foot].day);
	tmNow=tmB+tmAll;
	LONG nYear=tmNow.GetYear ();
	LONG nMon=tmNow.GetMonth  ();
	LONG nDay=tmNow.GetDay  ();
	LONG nH=tmNow.GetHour  ();
	LONG nM=tmNow.GetMinute  ();

	VARIANT vart;

	vart=m_dtPicker.GetYear();
	vart.lVal=nYear;
	m_dtPicker.SetYear(vart);
	vart=m_dtPicker.GetMonth();
	vart.lVal=nMon;
	m_dtPicker.SetMonth(vart);
	vart=m_dtPicker.GetDay();
	vart.lVal=nDay;
	m_dtPicker.SetDay(vart);
	if(pView->m_nKlineKind2 >0 &&pView->m_nKlineKind2 <5)
	{
		vart=m_dtPicker.GetHour();
		vart.lVal=nH;
		m_dtPicker.SetHour(vart);
		vart=m_dtPicker.GetMinute();
		vart.lVal=nM;
		m_dtPicker.SetMinute(vart);
	}
}

void CTaiKlineDialogChangKLine::OnOK() 
{

	CString s9;
	m_conAmount.GetWindowText(s9);
	for(int i=0; i<s9.GetLength();i++)
	{
		if(s9[i]!='.' && (s9[i]<'0'||s9[i]>'9'))
		{
			AfxMessageBox("请在“成交额”编辑框内键入一个正浮点数！");
			m_conAmount.SetSel(0,-1);
			return ;
		}
	}

	if(UpdateData(TRUE) == 0)
		return;

	int rtn;
	if((rtn = AddData()) <=0)
	{
		if(rtn == -1||rtn == -2)
			AfxMessageBox("最高、最低价数据不合理，请修改。");
		return;
	}
	CDialog::OnOK();
}

int CTaiKlineDialogChangKLine::AddData()
{
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();


	if(m_high<m_close ||m_high<m_low ||m_high<m_open)
		return -1;
	if(m_low>m_close ||m_low>m_high ||m_low>m_open)
		return -2;

	Kline kln;
	kln.open = m_open;
	kln.high = m_high;
	kln.low  = m_low;
	kln.close = m_close;
	kln.vol = (float)m_vol;
	kln.amount = (float)(atof(m_amountStr)*10000);;
	if(!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol))
	{
		kln.volPositive = m_fOuterPartVol;
	}
	else
	{
		kln.advance = (WORD)(int)(this->m_fInnerPartVol );
		kln.decline = (WORD)(int)(this->m_fOuterPartVol );
	}

	if(pView->m_nKlineKind2 ==1)
	{
		if(MessageBox("是否保存对该数据修改？","修改数据",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return -10;
		int day = pView->m_pkline [m_foot].day;
		pView->m_pkline [m_foot]=kln;
		pView->m_pkline [m_foot].day = day;
		pView->WriteKLine (m_foot,true);
		return 1;
	}


	if(m_foot>pView->m_nCountKlineToday )
	{
		int day = pView->m_pkline [m_foot].day;
		pView->m_pkline [m_foot]=kln;
		pView->m_pkline [m_foot].day = day;
		pView->WriteKLine (m_foot,true);
		return 1;
	}

	CTime tmB(1970,1,1,9,30,0);
	CTimeSpan tmAll;

	VARIANT vart;
	vart=m_dtPicker.GetYear();
	int nYear=vart.lVal ;
	vart=m_dtPicker.GetMonth();
	int nMon=vart.lVal ;
	vart=m_dtPicker.GetDay();
	int nDay=vart.lVal ;

	if(nYear<1988 || nYear>2037)
	{
		AfxMessageBox("日期不合理！");
		return -10;
	}

	CTime tmNow(nYear,nMon,nDay,9,30,0);
	tmAll=tmNow-tmB;

	
	int rtn=pView->FindTimeK(tmAll.GetTotalSeconds ());
	if(rtn==-1)
	{

		rtn = m_foot;
		pView->m_bAddKline=false;
	}
	if(pView->m_bAddKline==false)
	{
		if(MessageBox("是否修改该数据？","修改数据",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return -10;
		int day = pView->m_pkline [rtn].day;
		pView->m_pkline [rtn]=kln;
		pView->m_pkline [rtn].day = day;
		pView->WriteKLine (rtn,true);
	}
	else
	{
		if(MessageBox("是否修改该数据？","修改数据",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return -10;

		Kline* kl=new Kline[pView->m_nCountKline+241];
		memcpy(kl,pView->m_pkline+rtn ,sizeof(Kline)*(pView->m_nCountKline-rtn));
		pView->m_pkline [rtn]=kln;
		pView->m_pkline [rtn].day=tmAll.GetTotalSeconds ();
		memcpy(pView->m_pkline+rtn+1 ,kl,sizeof(Kline)*(pView->m_nCountKline-rtn));
		pView->m_nCountKline+=1;
		pView->m_nCountKlineFile+=1;
		delete[]  kl;
		pView->WriteKLine (rtn,false);
	}
	pView->pKlineDrawing ->m_bToCacl =true;
	pView->pKlineDrawing ->m_bInitFoot =true;
	pView->RedrawWindow();

	return 1;

	
}

void CTaiKlineDialogChangKLine::OnButtonDelete() 
{
	
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	if(m_foot>=pView->m_nCountKlineFile || pView->m_nCountKlineFile<=0)
		return;
	if(pView->m_nKlineKind2 !=5)
		return;

	CTime tmB(1970,1,1,9,30,0);
	CTimeSpan tmAll;

	VARIANT vart;
	vart=m_dtPicker.GetYear();
	int nYear=vart.lVal ;
	vart=m_dtPicker.GetMonth();
	int nMon=vart.lVal ;
	vart=m_dtPicker.GetDay();
	int nDay=vart.lVal ;

	CTime tmNow(nYear,nMon,nDay,9,30,0);
	tmAll=tmNow-tmB;

	
	int rtn=pView->FindTimeK(tmAll.GetTotalSeconds ());

	if(rtn==-1 || pView->m_bAddKline==true || pView->m_nCountKline-rtn-1<0)
	{

		if(m_foot>=0 && m_foot<pView->m_nCountKline)
			rtn=m_foot;
		else
			return;
	}

	if(MessageBox("是否删除K线数据？","删除K线数据",MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;


	Kline* kl=new Kline[pView->m_nCountKline+241];

	memcpy(kl,pView->m_pkline+rtn+1 ,sizeof(Kline)*(pView->m_nCountKline-rtn-1));
	memcpy(pView->m_pkline+rtn ,kl,sizeof(Kline)*(pView->m_nCountKline-rtn-1));

	pView->m_nCountKline-=1;
	pView->m_nCountKlineFile-=1;
	delete[]  kl;
	
	pView->WriteKLine (rtn,false,true);
	OnCancel();
	pView->pKlineDrawing ->m_bToCacl =true;
	pView->pKlineDrawing ->m_bInitFoot =true;
	pView->RedrawWindow();

	
}

BEGIN_EVENTSINK_MAP(CTaiKlineDialogChangKLine, CDialog)
    //{{AFX_EVENTSINK_MAP(CTaiKlineDialogChangKLine)
	ON_EVENT(CTaiKlineDialogChangKLine, IDC_DTPICKER1, 2 /* Change */, OnChangeDtpicker1, VTS_NONE)
	ON_EVENT(CTaiKlineDialogChangKLine, IDC_DTPICKER1, -602 /* KeyDown */, OnKeyDownDtpicker1, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTaiKlineDialogChangKLine::OnChangeDtpicker1() 
{

	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	if(pView->m_nKlineKind2 < 5)
		return;

	VARIANT vart;
	vart=m_dtPicker.GetYear();
	int nYear=vart.lVal ;
	vart=m_dtPicker.GetMonth();
	int nMon=vart.lVal ;
	vart=m_dtPicker.GetDay();
	int nDay=vart.lVal ;

	if(nYear<1988 || nYear>2037)
	{
		AfxMessageBox("日期不合理！");
		return ;
	}

	CTime tmNow(nYear,nMon,nDay,9,30,0);
	CTime tmB(1970,1,1,9,30,0);
	CTimeSpan tmAll=tmNow-tmB;


	int rtn=pView->FindTimeK(tmAll.GetTotalSeconds ());
	if(rtn==-1 || pView->m_bAddKline==true)
	{
		AfxMessageBox("找不到指定日期的K线数据！",MB_ICONWARNING);
		return ;
	}

	int foot = rtn;
	m_foot = foot;

	m_close=pView->m_pkline[foot].close;
	m_high=pView->m_pkline[foot].high;
	m_low=pView->m_pkline[foot].low;
	m_open=pView->m_pkline[foot].open;


	if(m_high<m_close ||m_high<m_low ||m_high<m_open)
		m_high = max(max(m_close,m_open),m_low);
	if(m_low>m_close ||m_low>m_high ||m_low>m_open)
		m_low = min(min(m_close,m_open),m_high);

	m_vol=(int)pView->m_pkline[foot].vol;
	if(!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol))
	{
		float f = pView->m_pkline[foot].vol
			-pView->m_pkline[foot].volPositive;
		if(f>=0)
		{
			m_fInnerPartVol = f;
			m_fOuterPartVol = pView->m_pkline[foot].volPositive;
		}
		else
		{
			m_fInnerPartVol = pView->m_pkline[foot].vol/2;
			m_fOuterPartVol = m_fInnerPartVol;
		}
	}
	else
	{
		m_fInnerPartVol = 0;
		m_fOuterPartVol = 0;
	}


	m_amount=pView->m_pkline[foot].amount/10000;
	m_amountStr.Format("%.1f",m_amount);

	UpdateData(FALSE);

	
}

void CTaiKlineDialogChangKLine::OnKeyDownDtpicker1(short FAR* KeyCode, short Shift) 
{

	
}

BOOL CTaiKlineDialogChangKLine::PreTranslateMessage(MSG* pMsg) 
{

	if (pMsg->message == WM_CHAR)
	{
		TCHAR chCharCode = (TCHAR) pMsg->wParam;
		if(chCharCode=='\n')
			OnOK();
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CTaiKlineDialogChangKLine::OnCancel() 
{

	
	CDialog::OnCancel();
}

void CTaiKlineDialogChangKLine::OnButtonPower() 
{

	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	VARIANT vart;
	vart=m_dtPicker.GetYear();
	int nYear=vart.lVal ;
	if(nYear<1988 || nYear>2037)
	{
		AfxMessageBox("日期不合理！");
		return ;
	}

	CTaiChuQuanSetDlg addChuquan;
	addChuquan.pView = pView;

	int nFinded=-1;
	CTime tmB(1970,1,1,8,0,0);
	CTime tmNow;
	CTimeSpan tmAll(pView->m_pkline[m_foot].day);
	tmNow=tmB+tmAll;
	nYear=tmNow.GetYear ();
	int nMon=tmNow.GetMonth  ();
	int nDay=tmNow.GetDay  ();
	addChuquan.m_timet=pView->m_pkline[m_foot].day;
	addChuquan.m_fromWhich = 1;//1 = 
	for(int i=0;i<pView->pKlineDrawing -> m_PowerArray .GetSize();i++)
	{

		CTime tmEnd=tmB+pView->pKlineDrawing ->m_PowerArray[i].nTime;
		if(tmEnd.GetDay()==nDay&&tmEnd.GetMonth()==nMon&&tmEnd.GetYear()==nYear)
		{
			addChuquan.m_timet=pView->m_pkline[m_foot].day;
			
			
			addChuquan.m_fAlloc=(float)(pView->pKlineDrawing ->m_PowerArray[i].fAllocate*10);
			addChuquan.m_fDivid=pView->pKlineDrawing ->m_PowerArray[i].fDividend*10;
			addChuquan.m_fPrice=pView->pKlineDrawing ->m_PowerArray[i].fAllocatePrice;
			addChuquan.m_fGive=(float)(pView->pKlineDrawing ->m_PowerArray[i].fGive*10);
////////////////////////////////////end one//////////////////////////////////

			addChuquan.m_kind=pView->pKlineDrawing ->m_PowerArray[i].nFlags;
			if(addChuquan.m_kind<0||addChuquan.m_kind>2)
				addChuquan.m_kind=0;
			nFinded=i;
			break;
		}
	}

	if(addChuquan.DoModal()==IDOK)
	{
		int tmInt = addChuquan.m_timet ;
		nFinded = -1;
		for(int i=0;i<pView->pKlineDrawing ->m_PowerArray .GetSize();i++)
		{
	//		nFinded=-1;
			
			if(pView->pKlineDrawing ->m_PowerArray[i].nTime/(24*3600) == tmInt/(24*3600))
			{
				nFinded=i;
				break;
			}
		}

		POWER Power;
	

		Power.fGive=addChuquan.m_fGive/10.0f;
		Power.fAllocate=addChuquan.m_fAlloc/10.0f;
		Power.fAllocatePrice=addChuquan.m_fPrice;
		Power.fDividend=addChuquan.m_fDivid/10.0f;
		Power.nFlags=addChuquan.m_kind;
///////////////////////////////////////////end changed two
/////////////////////////////////////////////////end modify of 2000/3/7/////////////////

		
		Power.nTime =addChuquan.m_timet ;//tmAll.GetTotalSeconds();

	
		if(Power.fGive<=0&&Power.fAllocate<=0&&Power.fAllocatePrice<=0
			&&Power.fDividend<=0)
		{
			if(nFinded>=0)
			{
				pView->pKlineDrawing ->m_PowerArray.RemoveAt(nFinded);
			}
		
			pView->pKlineDrawing->WritePower(pView->m_sharesSymbol,pView->m_stkKind);
		
			if(pView->m_bDoPower==true)
			{
				pView->m_bDoPower=false;
				pView->pKlineDrawing->TransferKlineTypeData(pView->m_sharesSymbol,pView->m_stkKind ,pView->m_pkline,-1);
				pView->m_bDoPower=true;
				pView->pKlineDrawing->ComputePower(pView->m_sharesSymbol,pView->m_stkKind ,pView->m_pkline,pView->m_nCountKline);
				pView->pKlineDrawing->m_bToCacl=true;
				pView->pKlineDrawing ->m_bInitFoot =false;//

				if(pView->m_isShowCross==1)
					pView->m_dlgShowCross->RedrawWindow();
				pView->RedrawWindow();
			}
			return;
		}

		if(nFinded>=0)
			pView->pKlineDrawing ->m_PowerArray[nFinded]=Power;
		else
			pView->pKlineDrawing ->m_PowerArray.Add(Power);
		pView->pKlineDrawing ->WritePower (pView->m_sharesSymbol,pView->m_stkKind );

	
		if(pView->m_bDoPower==true)
		{
			pView->m_bDoPower=false;
			pView->pKlineDrawing->TransferKlineTypeData(pView->m_sharesSymbol,pView->m_stkKind ,pView->m_pkline,-1);
			pView->m_bDoPower=true;
			pView->pKlineDrawing->ComputePower(pView->m_sharesSymbol,pView->m_stkKind ,pView->m_pkline,pView->m_nCountKline);
			pView->pKlineDrawing->m_bToCacl=true;
			pView->pKlineDrawing ->m_bInitFoot =false;//

			if(pView->m_isShowCross==1)
				pView->m_dlgShowCross->RedrawWindow();
			pView->RedrawWindow();
		}
	}
	
}

void CTaiKlineDialogChangKLine::OnButtonEventBig() 
{

	VARIANT vart;
	vart=m_dtPicker.GetYear();
	int nYear=vart.lVal ;
	if(nYear<1988 || nYear>2037)
	{
		AfxMessageBox("日期不合理！");
		return ;
	}

	CTaiKlineDlgBaseInfo bsInfo(this);
	bsInfo.DoModal();
	
}

void CTaiKlineDialogChangKLine::OnChangeEditHighChg() 
{

	if(m_bInit ==true)
		UpdateData(TRUE);	
	
	
}

void CTaiKlineDialogChangKLine::OnChangeEditLowChg() 
{
	
	if(m_bInit ==true)
		UpdateData(TRUE);	
	

	
}

void CTaiKlineDialogChangKLine::OnChangeEditAmountChg() 
{
	
	CString s;
	m_conAmount.GetWindowText(s);
	for(int i=0; i<s.GetLength();i++)
	{
		if(s[i]!='.' && (s[i]<'0'||s[i]>'9'))
		{
			AfxMessageBox("请键入一个正浮点数！");
			m_conAmount.SetSel(0,-1);
			break;
		}
	}

	
}

void CTaiKlineDialogChangKLine::OnChangeEditOpenChg() 
{
	
	if(m_bInit ==true)
		UpdateData(TRUE);	
	
	

	
}

void CTaiKlineDialogChangKLine::OnChangeEditCloseChg() 
{
	
	if(m_bInit ==true)
		UpdateData(TRUE);	
	
	

	
}

void CTaiKlineDialogChangKLine::OnChangeEditVolChg() 
{
	
	if(m_bInit ==true)
		UpdateData(TRUE);	
	
	
	
}

BOOL CTaiKlineDialogChangKLine::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
}
