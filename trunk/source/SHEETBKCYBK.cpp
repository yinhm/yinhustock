// SHEETBKCYBK.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "vwbase.h"
#include "SHEETBKCYBK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiCommonBlockSetDlg dialog


CTaiCommonBlockSetDlg::CTaiCommonBlockSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiCommonBlockSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiCommonBlockSetDlg)
	m_nIndex = 0;
	//}}AFX_DATA_INIT
}


void CTaiCommonBlockSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiCommonBlockSetDlg)
	DDX_Radio(pDX, IDC_COMMONBLOCK1, m_nIndex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaiCommonBlockSetDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiCommonBlockSetDlg)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED,IDC_COMMONBLOCK1,IDC_COMMONBLOCK10,OnChangeChoice)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CTaiCommonBlockSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_COMMONBLOCKNAME)->SetWindowText("");
	GetDlgItem(IDC_INDEX)->SetWindowText("");
	int nTotalSize=m_strName.GetSize();
	int i;
	if(nTotalSize>0)
	{
		for(i=0;i<nTotalSize;i++)
		{
			GetDlgItem(IDC_COMMONBLOCK1+i)->SetWindowText(m_strName.GetAt(i));
		}
		if(nTotalSize<10)
		{
			for(i=nTotalSize;i<10;i++)
				GetDlgItem(IDC_COMMONBLOCK1+i)->EnableWindow(FALSE);
		}
	}
	else
	{
		for(i=0;i<10;i++)
			GetDlgItem(IDC_COMMONBLOCK1+i)->EnableWindow(FALSE);
	}

	return FALSE;  
}

void CTaiCommonBlockSetDlg::OnChangeChoice(UINT nID)
{
	int nIndex=nID-IDC_COMMONBLOCK1;
	if(nIndex<m_strName.GetSize())
	{
		CString strShortcutKey;
		strShortcutKey.Format("%d",90+nIndex);
		GetDlgItem(IDC_COMMONBLOCKNAME)->SetWindowText(m_strName.GetAt(nIndex));
		GetDlgItem(IDC_INDEX)->SetWindowText(strShortcutKey);
	}
}