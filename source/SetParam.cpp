// SetParam.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "SetParam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SetParam dialog

SetParam::SetParam(float *big,float *bigMax,float *param,float *bigGB,float *bigMaxGB,float *paramGB,BOOL isgb,CWnd* pParent)   // standard constructor
	: CDialog(SetParam::IDD, pParent)
{
	memcpy(this->nBig,big,4*sizeof(float));
	memcpy(this->nBigMax,bigMax,4*sizeof(float));
	memcpy(this->nParam,param,10*sizeof(float));
	memcpy(this->nBigGB,bigGB,4*sizeof(float));
	memcpy(this->nBigMaxGB,bigMaxGB,4*sizeof(float));
	memcpy(this->nParamGB,paramGB,10*sizeof(float));
	IsGbParam=FALSE;
	//{{AFX_DATA_INIT(SetParam)

	IsGb=isgb;
	m_Big1 = big[0];
	m_Big2 = big[1];
	m_Big3 = big[2];
	m_Big4 = big[3];
	m_BigMax1 = bigMax[0];
	m_BigMax2 = bigMax[1];
	m_BigMax3 = bigMax[2];
	m_BigMax4 = bigMax[3];
	m_Param0 = param[0];
	m_Param1 = param[1];
	m_Param2 = param[2];
	m_Param3 = param[3];
	m_Param4 = param[4];
	m_Param5 = param[5];
	m_Param6 = param[6];
	m_Param7 = param[7];
	m_Param8 = param[8];
	m_Radio = -1;
	m_Radio1 = 0;
	//}}AFX_DATA_INIT
}
SetParam::SetParam(CWnd* pParent /*=NULL*/)
	: CDialog(SetParam::IDD, pParent)
{
	m_Big1 = 0.0f;
	m_Big2 = 0.0f;
	m_Big3 = 0.0f;
	m_Big4 = 0.0f;
	m_BigMax1 = 0.0f;
	m_BigMax2 = 0.0f;
	m_BigMax3 = 0.0f;
	m_BigMax4 = 0.0f;
	m_Param0 = 0.0f;
	m_Param1 = 0.0f;
	m_Param2 = 0.0f;
	m_Param3 = 0.0f;
	m_Param4 = 0.0f;
	m_Param6 = 0.0f;
	m_Param5 = 0.0f;
	m_Param7 = 0.0f;
	m_Param8 = 0.0f;
	m_Radio = -1;
}


void SetParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetParam)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_BIG1, m_Big1);
	DDX_Text(pDX, IDC_BIG2, m_Big2);
	DDX_Text(pDX, IDC_BIG3, m_Big3);
	DDX_Text(pDX, IDC_BIG4, m_Big4);
	DDX_Text(pDX, IDC_BIGMAX1, m_BigMax1);
	DDX_Text(pDX, IDC_BIGMAX2, m_BigMax2);
	DDX_Text(pDX, IDC_BIGMAX3, m_BigMax3);
	DDX_Text(pDX, IDC_BIGMAX4, m_BigMax4);
	DDX_Text(pDX, IDC_PARAM0, m_Param0);
	DDX_Text(pDX, IDC_PARAM1, m_Param1);
	DDX_Text(pDX, IDC_PARAM2, m_Param2);
	DDX_Text(pDX, IDC_PARAM3, m_Param3);
	DDX_Text(pDX, IDC_PARAM4, m_Param4);
	DDX_Text(pDX, IDC_PARAM6, m_Param6);
	DDX_Text(pDX, IDC_PARAM5, m_Param5);
	DDX_Text(pDX, IDC_PARAM7, m_Param7);
	DDX_Text(pDX, IDC_PARAM8, m_Param8);
	DDX_Radio(pDX, IDC_RADIO1, m_Radio);
	DDX_Radio(pDX, IDC_RADIO5, m_Radio1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SetParam, CDialog)
	//{{AFX_MSG_MAP(SetParam)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL SetParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	this->CenterWindow();
	if(!this->IsGb)
		this->m_Radio=0;
	else
		this->m_Radio=1;
	this->UpdateData(FALSE);
	return TRUE;  
}
void SetParam::OnOK()
{
	IsGbParam=m_Radio1;
	if(!this->UpdateData(TRUE))
		return;
	if(!CheckParam())
	{
		IsGbParam=IsGbParam;
		this->UpdateData(FALSE);
		return;
	}

	BOOL Isgbjg=this->m_Radio;
	this->IsGb=this->m_Radio;

    if(!this->m_Radio1)
	{
		this->nBig[0]=this->m_Big1;
		this->nBig[1]=this->m_Big2;
		this->nBig[2]=this->m_Big3;
		this->nBig[3]=this->m_Big4;
		this->nBigMax[0]=this->m_BigMax1;
		this->nBigMax[1]=this->m_BigMax2;
		this->nBigMax[2]=this->m_BigMax3;
		this->nBigMax[3]=this->m_BigMax4;

		this->nParam[0]=this->m_Param0;
		this->nParam[1]=this->m_Param1;
		this->nParam[2]=this->m_Param2;
		this->nParam[3]=this->m_Param3;
		this->nParam[4]=this->m_Param4;
		this->nParam[5]=this->m_Param5;
		this->nParam[6]=this->m_Param6;
		this->nParam[7]=this->m_Param7;
		this->nParam[8]=this->m_Param8;
	}
	else
	{
		this->nBigGB[0]=this->m_Big1;
		this->nBigGB[1]=this->m_Big2;
		this->nBigGB[2]=this->m_Big3;
		this->nBigGB[3]=this->m_Big4;
		this->nBigMaxGB[0]=this->m_BigMax1;
		this->nBigMaxGB[1]=this->m_BigMax2;
		this->nBigMaxGB[2]=this->m_BigMax3;
		this->nBigMaxGB[3]=this->m_BigMax4;

		this->nParamGB[0]=this->m_Param0;
		this->nParamGB[1]=this->m_Param1;
		this->nParamGB[2]=this->m_Param2;
		this->nParamGB[3]=this->m_Param3;
		this->nParamGB[4]=this->m_Param4;
		this->nParamGB[5]=this->m_Param5;
		this->nParamGB[6]=this->m_Param6;
		this->nParamGB[7]=this->m_Param7;
		this->nParamGB[8]=this->m_Param8;
	}
	CDialog::OnOK();
}
void SetParam::OnRadio5()
{
	IsGbParam=m_Radio1;
	if(!this->UpdateData(TRUE))
	{
		m_Radio1=IsGbParam;
		this->UpdateData(FALSE);
		return;
	}
	if(!CheckParam())
	{
		m_Radio1=IsGbParam;
		this->UpdateData(FALSE);
		return;
	}
	if(IsGbParam==m_Radio1)
		return;
	this->nBigGB[0]=this->m_Big1;
	this->nBigGB[1]=this->m_Big2;
	this->nBigGB[2]=this->m_Big3;
	this->nBigGB[3]=this->m_Big4;
	this->nBigMaxGB[0]=this->m_BigMax1;
	this->nBigMaxGB[1]=this->m_BigMax2;
	this->nBigMaxGB[2]=this->m_BigMax3;
	this->nBigMaxGB[3]=this->m_BigMax4;

	this->nParamGB[0]=this->m_Param0;
	this->nParamGB[1]=this->m_Param1;
	this->nParamGB[2]=this->m_Param2;
	this->nParamGB[3]=this->m_Param3;
	this->nParamGB[4]=this->m_Param4;
	this->nParamGB[5]=this->m_Param5;
	this->nParamGB[6]=this->m_Param6;
	this->nParamGB[7]=this->m_Param7;
	this->nParamGB[8]=this->m_Param8;

	this->m_Big1=this->nBig[0];
	this->m_Big2=this->nBig[1];
	this->m_Big3=this->nBig[2];
	this->m_Big4=this->nBig[3];
	this->m_BigMax1=this->nBigMax[0];
	this->m_BigMax2=this->nBigMax[1];
	this->m_BigMax3=this->nBigMax[2];
	this->m_BigMax4=this->nBigMax[3];

	this->m_Param0=this->nParam[0];
	this->m_Param1=this->nParam[1];
	this->m_Param2=this->nParam[2];
	this->m_Param3=this->nParam[3];
	this->m_Param4=this->nParam[4];
	this->m_Param5=this->nParam[5];
	this->m_Param6=this->nParam[6];
	this->m_Param7=this->nParam[7];
	this->m_Param8=this->nParam[8];
    this->UpdateData(FALSE);

	CWnd *pWnd=this->GetDlgItem(IDC_DW);
	pWnd->SetWindowText("统计结果分类(手数)");

}
void SetParam::OnRadio6()
{
	IsGbParam=m_Radio1;
	if(!this->UpdateData(TRUE))
	{
		m_Radio1=IsGbParam;
		this->UpdateData(FALSE);
		return;
	}
	if(!CheckParam())
	{
		m_Radio1=IsGbParam;
		this->UpdateData(FALSE);
		return;
	}
	if(IsGbParam==m_Radio1)
		return;
	this->nBig[0]=this->m_Big1;
	this->nBig[1]=this->m_Big2;
	this->nBig[2]=this->m_Big3;
	this->nBig[3]=this->m_Big4;
	this->nBigMax[0]=this->m_BigMax1;
	this->nBigMax[1]=this->m_BigMax2;
	this->nBigMax[2]=this->m_BigMax3;
	this->nBigMax[3]=this->m_BigMax4;

	this->nParam[0]=this->m_Param0;
	this->nParam[1]=this->m_Param1;
	this->nParam[2]=this->m_Param2;
	this->nParam[3]=this->m_Param3;
	this->nParam[4]=this->m_Param4;
	this->nParam[5]=this->m_Param5;
	this->nParam[6]=this->m_Param6;
	this->nParam[7]=this->m_Param7;
	this->nParam[8]=this->m_Param8;


	this->m_Big1=this->nBigGB[0];
	this->m_Big2=this->nBigGB[1];
	this->m_Big3=this->nBigGB[2];
	this->m_Big4=this->nBigGB[3];
	this->m_BigMax1=this->nBigMaxGB[0];
	this->m_BigMax2=this->nBigMaxGB[1];
	this->m_BigMax3=this->nBigMaxGB[2];
	this->m_BigMax4=this->nBigMaxGB[3];

	this->m_Param0=this->nParamGB[0];
	this->m_Param1=this->nParamGB[1];
	this->m_Param2=this->nParamGB[2];
	this->m_Param3=this->nParamGB[3];
	this->m_Param4=this->nParamGB[4];
	this->m_Param5=this->nParamGB[5];
	this->m_Param6=this->nParamGB[6];
	this->m_Param7=this->nParamGB[7];
	this->m_Param8=this->nParamGB[8];
    this->UpdateData(FALSE);
	CWnd *pWnd=this->GetDlgItem(IDC_DW);
	pWnd->SetWindowText("统计结果分类(流通股本百分比)");
}
BOOL SetParam::CheckParam()
{
    if(m_Big1<0||m_BigMax1<0
		||m_Big2<0||m_BigMax2<0
		||m_Big3<0||m_BigMax3<0
		||m_Big4<0||m_BigMax4<0)
	{
		AfxMessageBox("选取参数时,输入参数不能为负数!");
		return FALSE;
	}
    if(m_Big1>=m_BigMax1||m_Big2>=m_BigMax2||m_Big3>=m_BigMax3||m_Big4>=m_BigMax4)
	{
		AfxMessageBox("选取参数范围时,前参数不能大于后参数!");
		return FALSE;
	}

	if(m_Param0<0||m_Param1<0||m_Param2<0||m_Param3<0
		||m_Param4<0||m_Param5<0||m_Param6<0||m_Param7<0||m_Param8<0)
	{
		AfxMessageBox("在选取统计分类参数时,参数不能负数!");
		return FALSE;
	}
	if(m_Param0>=m_Param1||m_Param1>=m_Param2
		||m_Param2>=m_Param3||m_Param3>=m_Param4
		||m_Param4>=m_Param5||m_Param5>=m_Param6
		||m_Param6>=m_Param7||m_Param7>=m_Param8)
	{
		AfxMessageBox("在选取统计成交分类参数时,前输入参数不能大于后输入参数!");
		return FALSE;
	}
	if(IsGbParam==1)
	{
		if(m_Big1>=1||m_BigMax1>=1
			||m_Big2>=1||m_BigMax2>=1
			||m_Big3>=1||m_BigMax3>=1
			||m_Big4>=1||m_BigMax4>=1)
		{
			AfxMessageBox("选取股本时,输入的参数不能大于1或小于0!");
			return FALSE;
		}
		if(m_Param0>=1||m_Param1>=1||m_Param2>=1||m_Param3>=1
			||m_Param4>=1||m_Param5>=1||m_Param6>=1||m_Param7>=1||m_Param8>=1)
		{
			AfxMessageBox("选取股本时,输入的参数不能大于1或小于0!");
			return FALSE;
		}
	}
	return TRUE;
}
