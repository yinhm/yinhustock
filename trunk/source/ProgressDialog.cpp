// ProgressDialog.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ProgressDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog


CProgressDialog::CProgressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDialog)
	m_zqdm = _T("");
	m_comp = _T("");
	//}}AFX_DATA_INIT
}


void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDialog)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_ZQDM, m_zqdm);
	DDX_Text(pDX, IDC_COMP, m_comp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDialog, CDialog)
	//{{AFX_MSG_MAP(CProgressDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CProgressDialog::OnClose() 
{
	
	if(!this->m_DialogDown )
	{
       MessageBox("不能关闭数据接收窗口!","警告",MB_OK|MB_ICONWARNING);
	   return ;
	}
	CDialog::OnClose();
    DestroyWindow();	
}

void CProgressDialog::PostNcDestroy() 
{

    delete this;	
	CDialog::PostNcDestroy();
}

void CProgressDialog::OnSize(UINT nType, int cx, int cy) 
{
	CRect rc;
	if(cx==0||cy==0)
		return;
	CDialog::OnSize(nType, cx, cy);
	this->CenterWindow ();

	
}
void CProgressDialog::DisplayZqxx(CString zqdm, int m_pos) 
{


      this->m_zqdm=zqdm;
	  this->m_comp.Format("%d",m_pos);
      this->m_comp +=_T("%");
	  m_Progress.SetPos(m_pos); 
	  UpdateData(FALSE);
}

BOOL CProgressDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
    m_Progress.SetRange(0,100);
	m_Progress.SetPos(0); 

	
	return TRUE;  
}

void CProgressDialog::OnOK() 
{

	
	CDialog::OnOK();
    DestroyWindow();	
}
