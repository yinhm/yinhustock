// TechDlgNNetEditData.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgNNetEditData.h"
#include "CTaiKlineDlgNeuralLearn.h"
#include "TechNnetBP.h"
#include "CAlertSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgNNetEditData dialog


CTaiKlineDlgNNetEditData::CTaiKlineDlgNNetEditData(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgNNetEditData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgNNetEditData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = (CTaiKlineDlgNeuralLearn*)pParent;
}


void CTaiKlineDlgNNetEditData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgNNetEditData)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_LIST2, m_conData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgNNetEditData, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgNNetEditData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgNNetEditData message handlers

void CTaiKlineDlgNNetEditData::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CTaiKlineDlgNNetEditData::OnOK() 
{
	
	try
	{
    CString Id;
	int temp=m_conData.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
		return;
	while(temp>=0)
	{
		m_conData.DeleteItem(temp);
		m_pParent->fInArray .RemoveAt(temp);
        temp=m_conData.GetNextItem( -1,LVNI_SELECTED   ) ;
	}
	}
	catch(...)
	{
	}

}

BOOL CTaiKlineDlgNNetEditData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	try
	{
	CString			strItem1[3]= {_T("股票代码"),_T("时间"),_T("平均输出结果")};
	
	int nCol = 3+m_pParent->m_maxNeededOut;
	int gap = 2;
	int i;
	for(i=0;i<3;i++)
	{
		this->m_conData .InsertColumn(i, strItem1[i], LVCFMT_LEFT,
			80, i);
	}
	for(i=0;i<m_pParent->m_maxNeededOut;i++)
	{
		CString s;
		s.Format ("输出数据%d",i+1);
		this->m_conData .InsertColumn(3+i,s, LVCFMT_LEFT,
			80, 3+i);
	}
	CString strPt[64];
	int n = m_pParent->fInArray .GetSize ();
	for(i=0;i<n;i++)
	{
		strPt[0]=m_pParent->fInArray [i].symbol ;
		strPt[1]=CTime(m_pParent->fInArray [i].time).Format ("%Y%m%d") ;
		float f=0;
		for(int j=0;j<m_pParent->m_maxNeededOut ;j++)
		{
			f+=m_pParent->fInArray [i].dataOut [j];
		}
		if(m_pParent->m_maxNeededOut>0)
			f/=m_pParent->m_maxNeededOut;
		strPt[2] = CLongString::FloatToString(f);
		for(int j=3;j<nCol ;j++)
		{
			strPt[j]= CLongString::FloatToString(m_pParent->fInArray [i].dataOut [j-3]);
		}

		CAlertSystem::InsertRow(&m_conData, strPt, nCol, 0);
	}
	}
	catch(...)
	{
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
