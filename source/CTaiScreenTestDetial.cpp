// CTaiScreenTestDetial.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiScreenTestDetial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CTaiScreenTestDetial::CTaiScreenTestDetial(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiScreenTestDetial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiScreenTestDetial)
		
	//}}AFX_DATA_INIT
}


void CTaiScreenTestDetial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreenTestDetial)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_flexgrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreenTestDetial, CDialog)
	//{{AFX_MSG_MAP(CTaiScreenTestDetial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CTaiScreenTestDetial::SetDetail(SCREEN_RESULT *chooseresult, CString stockname)
{
     m_chooseresult=chooseresult;
	 m_stockname=stockname;
}

BOOL CTaiScreenTestDetial::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_stockname+"条件选股测试明细");
	SCREEN_DETAIL detail;
	CString title[]={"序号","买进时间","买进价格","卖出时间","卖出价格","收益率%","结果"};
	m_flexgrid.SetRow(0);
	for(int i=0;i<7;i++)
	{
		m_flexgrid.SetCol(i);
		m_flexgrid.SetText(title[i]);
		
	}
	int length=100*title[0].GetLength();
		m_flexgrid.SetColWidth(0,length);
		m_flexgrid.SetColWidth(1,1000);
		m_flexgrid.SetColWidth(3,1000);
    int num=m_chooseresult->m_detail.GetSize();
    CTime bTime; 
	CString str;
	m_flexgrid.SetRows(num+1);
	for(int j=0;j<num;j++)
	{
		detail=m_chooseresult->m_detail[j];
		str.Format("%d",j+1);
        m_flexgrid.SetTextMatrix(j+1,0,str.GetBuffer(0));
		str.ReleaseBuffer();
		bTime=detail.buytime;
		str=bTime.Format("%Y/%m/%d");
		m_flexgrid.SetTextMatrix(j+1,1,str.GetBuffer(0));
		str.ReleaseBuffer();
		str.Format("%.2f",detail.buyprice);
		m_flexgrid.SetTextMatrix(j+1,2,str.GetBuffer(0));
		str.ReleaseBuffer();
		bTime=detail.selltime;
		str=bTime.Format("%Y/%m/%d");
		m_flexgrid.SetTextMatrix(j+1,3,str.GetBuffer(0));
		str.ReleaseBuffer();
		str.Format("%.2f",detail.sellprice);
		m_flexgrid.SetTextMatrix(j+1,4,str.GetBuffer(0));
		str.ReleaseBuffer();
		str.Format("%.2f",detail.profit);
		m_flexgrid.SetTextMatrix(j+1,5,str.GetBuffer(0));
		str.ReleaseBuffer();
		if(detail.issuccess==0)
		   m_flexgrid.SetTextMatrix(j+1,6,"成功");
		if(detail.issuccess==1)
		   m_flexgrid.SetTextMatrix(j+1,6,"失败");
        if(detail.issuccess==2)
		   m_flexgrid.SetTextMatrix(j+1,6,"未完成");
	}

	return TRUE; 
}
