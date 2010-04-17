// BjDlgChengBenSetting.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "BjDlgChengBenSetting.h"
#include "CTaiKlineMoveChenben.h"
#include "BjDlgAddData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BjDlgChengBenSetting dialog
//子窗口框架

BjDlgChengBenSetting::BjDlgChengBenSetting(CWnd* pParent /*=NULL*/)
	: CDialog(BjDlgChengBenSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(BjDlgChengBenSetting)
	m_nDays = 0;
	//}}AFX_DATA_INIT

	m_pChengBen = new CTaiKlineMoveChenben(NULL,NULL,NULL);
	m_brush = NULL;
}
BjDlgChengBenSetting::~BjDlgChengBenSetting()
{
	delete m_pChengBen;
	if(m_brush)
		delete m_brush;
}


void BjDlgChengBenSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BjDlgChengBenSetting)
	DDX_Control(pDX, IDC_STATIC_COLOR, m_color);
	DDX_Control(pDX, IDC_COMBO1, m_comb);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Text(pDX, IDC_EDIT1, m_nDays);
	DDV_MinMaxUInt(pDX, m_nDays, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BjDlgChengBenSetting, CDialog)
	//{{AFX_MSG_MAP(BjDlgChengBenSetting)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonDel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void BjDlgChengBenSetting::OnSelchangeCombo1() 
{//子窗口框架
	UpdateData();

	int n = m_tab.GetCurSel ();
	if(n<0) return;
	int nComb = m_comb.GetCurSel ();
	if(nComb<0) return;
	int nCount = m_pChengBen->m_chengBenBeforArr.GetSize();
	if(n) nCount = m_pChengBen->m_chengBenInsideArr.GetSize();

	if(nComb>= nCount) return;// serious allocation failure checking

	if(n)
	{
		this->m_nDays = m_pChengBen->m_chengBenInsideArr[nComb].m_nDays;
	}
	else
	{
		this->m_nDays = m_pChengBen->m_chengBenBeforArr[nComb].m_nDays;
	}

	UpdateData(FALSE);// serious allocation failure checking

	RedrawWindow();
	
}

void BjDlgChengBenSetting::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{// NOTE: the ClassWizard will add member functions here
	
	int n = m_tab.GetCurSel ();
	if(n<0) return;

	int nCount = m_pChengBen->m_chengBenBeforArr.GetSize();
	if(n) nCount = m_pChengBen->m_chengBenInsideArr.GetSize();

	m_comb.ResetContent ();
	for(int i = 0; i<nCount;i++)
	{
		CString s;
		s.Format ("第%d条线",i+1);
		m_comb.AddString (s);
	}
	m_comb.SetCurSel (0);
	OnSelchangeCombo1();
	*pResult = 0;
}

BOOL BjDlgChengBenSetting::OnInitDialog() //子窗口框架
{// NOTE: the ClassWizard will add member functions here
	CDialog::OnInitDialog();
	
	m_tab.InsertItem (0,"火焰山1");//Default FontName
	m_tab.InsertItem (1,"火焰山2");
	m_tab.SetCurSel (0);
	long result;
	OnSelchangeTab1(NULL, &result) ;

	UpdateData(FALSE);

	return TRUE;  
}

void BjDlgChengBenSetting::OnCancel() 
{

	
	CDialog::OnCancel();
}

void BjDlgChengBenSetting::OnOK() 
{

	m_pChengBen->WriteOutSettings ();

	CDialog::OnOK();
}

HBRUSH BjDlgChengBenSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{//大图象列表
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(CTLCOLOR_STATIC == nCtlColor)
	{
		if(pWnd == &m_color)
		{
			if(m_brush)
				delete m_brush;
			m_brush = new CBrush;

			int n = m_tab.GetCurSel ();
			if(n<0) return hbr;
			int nComb = m_comb.GetCurSel ();
			if(nComb<0) return hbr;
			int nCount = m_pChengBen->m_chengBenBeforArr.GetSize();
			if(n) nCount = m_pChengBen->m_chengBenInsideArr.GetSize();

			if(nComb>= nCount) return hbr;

			if(n)
			{
				m_brush->CreateSolidBrush (m_pChengBen->m_chengBenInsideArr[nComb].m_colorLine);
			}
			else
			{
				m_brush->CreateSolidBrush (m_pChengBen->m_chengBenBeforArr[nComb].m_colorLine);
			}
			hbr = (HBRUSH)m_brush->m_hObject ;
		}
	}
	
	return hbr;
}

void BjDlgChengBenSetting::OnButtonAdd() //子窗口框架
{
	UpdateData();

	int n = m_tab.GetCurSel ();
	if(n<0) return;
	int nComb = m_comb.GetCurSel ();

	BjDlgAddData dlg;// standard constructor
	if( dlg.DoModal() == IDOK)
	{
		BjChenBenSetting set;
		set.m_colorLine = dlg.m_color ;
		set.m_nDays = dlg.m_nDays ;

		if(n)
		{
			m_pChengBen->m_chengBenInsideArr.Add(set);
		}
		else
		{
			m_pChengBen->m_chengBenBeforArr.Add(set);
		}

		long result;
		OnSelchangeTab1(NULL, &result) ;
		if(nComb<0) nComb = 0;
		m_comb.SetCurSel(nComb);
	}
	
}

void BjDlgChengBenSetting::OnButtonDel() //子窗口框架
{// standard constructor
	int n = m_tab.GetCurSel ();
	if(n<0) return;
	int nComb = m_comb.GetCurSel ();
	if(nComb<0) return;

	{
		int nAll = 0;
		if(n)
		{
			m_pChengBen->m_chengBenInsideArr.RemoveAt(nComb);
			nAll = m_pChengBen->m_chengBenInsideArr.GetSize();
		}
		else
		{
			m_pChengBen->m_chengBenBeforArr.RemoveAt(nComb);
			nAll = m_pChengBen->m_chengBenBeforArr.GetSize();
		}

		long result;
		OnSelchangeTab1(NULL, &result) ;
		if(nAll) m_comb.SetCurSel(0);
	}
}

void BjDlgChengBenSetting::OnPaint() 
{// NOTE: the ClassWizard will add member functions here
	CPaintDC dc(this); 
	
	int n = m_tab.GetCurSel ();
	if(n<0) return ;
	int nComb = m_comb.GetCurSel ();
	if(nComb<0) return ;
	int nCount = m_pChengBen->m_chengBenBeforArr.GetSize();
	if(n) nCount = m_pChengBen->m_chengBenInsideArr.GetSize();

	if(nComb>= nCount) return ;

	COLORREF color;
	if(n)
	{
		color = m_pChengBen->m_chengBenInsideArr[nComb].m_colorLine;
	}
	else
	{
		color = m_pChengBen->m_chengBenBeforArr[nComb].m_colorLine;
	}

	CRect r;
	m_color.GetWindowRect(r);
	this->ScreenToClient (r);
	dc.FillSolidRect (r,color);

}
