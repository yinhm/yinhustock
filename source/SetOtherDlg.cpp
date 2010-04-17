// SetOtherDlg.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "SetOtherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetOtherDlg dialog


CSetOtherDlg::CSetOtherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetOtherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetOtherDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INI
	m_BackColor=RGB(64,128,128);
	m_TitleColor=RGB(0,255,255);
	m_StockNameColor=RGB(0,255,0);
	m_OtherDataColor=RGB(0,0,255);
	m_StockUpColor=RGB(255,0,0);
	m_StockDownColor=RGB(255,255,0);
	m_StockStopColor=RGB(255,255,255);
}


void CSetOtherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetOtherDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetOtherDlg, CDialog)
	//{{AFX_MSG_MAP(CSetOtherDlg)
	ON_BN_CLICKED(IDC_SETFONT, OnSetfont)
	ON_BN_CLICKED(IDC_SETDEFAULT, OnSetdefault)
	//}}AFX_MSG_MAP
	ON_CONTROL(CCN_SELCHANGE, IDC_BACKCOLOR, OnBackColorChange)
 	ON_CONTROL(CCN_SELCHANGE, IDC_TITLECOLOR, OnTitleColorChange)
	ON_CONTROL(CCN_SELCHANGE, IDC_STOCKNAMECOLOR, OnStockNameColorChange)
 	ON_CONTROL(CCN_SELCHANGE, IDC_OTHERDATACOLOR, OnOtherDataColorChange)
	ON_CONTROL(CCN_SELCHANGE, IDC_STOCKUPCOLOR, OnStockUpColorChange)
 	ON_CONTROL(CCN_SELCHANGE, IDC_STOCKDOWNCOLOR, OnStockDownColorChange)
 	ON_CONTROL(CCN_SELCHANGE, IDC_STOCKSTOPCOLOR, OnStockStopColorChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CSetOtherDlg::OnSetfont() 
{

    CFontDialog dlg;
    if(dlg.DoModal() == IDOK)
		dlg.GetCurrentFont(&m_LogFont);
}

BOOL CSetOtherDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

    CRect rect;
    CWnd *pWnd = GetDlgItem(IDC_BACKCOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[0].SetColor(m_BackColor);
    m_nColorSet[0].Create(WS_VISIBLE|WS_CHILD, rect, this, IDC_BACKCOLOR);
 
    pWnd = GetDlgItem(IDC_TITLECOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[1].SetColor(m_TitleColor);
    m_nColorSet[1].Create(WS_VISIBLE|WS_CHILD, rect, this, IDC_TITLECOLOR);
 
    pWnd = GetDlgItem(IDC_STOCKNAMECOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[2].SetColor(m_StockNameColor);
    m_nColorSet[2].Create(WS_VISIBLE|WS_CHILD, rect, this, IDC_STOCKNAMECOLOR);
 
	pWnd = GetDlgItem(IDC_OTHERDATACOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[3].SetColor(m_OtherDataColor);
    m_nColorSet[3].Create(WS_VISIBLE|WS_CHILD, rect , this, IDC_OTHERDATACOLOR);
 
    pWnd = GetDlgItem(IDC_STOCKUPCOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[4].SetColor(m_StockUpColor);
    m_nColorSet[4].Create(WS_VISIBLE|WS_CHILD,rect , this, IDC_STOCKUPCOLOR);
 
    pWnd = GetDlgItem(IDC_STOCKDOWNCOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[5].SetColor(m_StockDownColor);
    m_nColorSet[5].Create(WS_VISIBLE|WS_CHILD,rect , this, IDC_STOCKDOWNCOLOR);
 
    pWnd = GetDlgItem(IDC_STOCKSTOPCOLOR);
    pWnd->SetDlgCtrlID(0xffff);
    pWnd->GetWindowRect(rect);
    ScreenToClient(&rect);
    m_nColorSet[6].SetColor(m_StockStopColor);
    m_nColorSet[6].Create(WS_VISIBLE|WS_CHILD,rect , this, IDC_STOCKSTOPCOLOR);

	return TRUE;  
}

void CSetOtherDlg::OnBackColorChange()
{
   m_BackColor =m_nColorSet[0].GetColor();

}

void CSetOtherDlg::OnTitleColorChange()
{
   m_TitleColor = m_nColorSet[1].GetColor();
}

void CSetOtherDlg::OnStockNameColorChange()
{
   m_StockNameColor = m_nColorSet[2].GetColor();
}

void CSetOtherDlg::OnOtherDataColorChange()
{
   m_OtherDataColor = m_nColorSet[3].GetColor();
}

void CSetOtherDlg::OnStockUpColorChange()
{
   m_StockUpColor = m_nColorSet[4].GetColor();
}

void CSetOtherDlg::OnStockDownColorChange()
{
   m_StockDownColor = m_nColorSet[5].GetColor();
}

void CSetOtherDlg::OnStockStopColorChange()
{
   m_StockStopColor = m_nColorSet[6].GetColor();
}

void CSetOtherDlg::OnSetdefault() 
{

	m_BackColor=RGB(63,63,63);
	m_TitleColor=RGB(0,255,255);
	m_StockNameColor=RGB(0,255,0);
	m_OtherDataColor=RGB(0,0,255);
	m_StockUpColor=RGB(255,0,255);
	m_StockDownColor=RGB(255,255,0);
	m_StockStopColor=RGB(255,255,255);
}
