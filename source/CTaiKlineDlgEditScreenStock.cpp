// TechDlgEditScreenStock.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgEditScreenStock.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
#include "DIAEDITZBGSHS.h"
#include "CFormularContent.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineDlgCanUsedPeriod.h"
#include "CTaiKlineDlgParamExplain.h"
#include "DIALOGXZZB.h"
#include "DIAGSGLMM.h"
#include "CTaiKlineIndicatorsManage.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineDlgChangeIndexParam.h"
#include "KEYBRODEANGEL.h"
#include "DIALOGGSSM.h"
#include "CwdSelectFormu.h"
#include "CwdShowFormu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock dialog

bool g_bInitDlgEdit = false;

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock message handlers
CTaiKlineDlgEditScreenStock::CTaiKlineDlgEditScreenStock(int nID,CWnd* pParent)
	: CDialog(nID, pParent)
{
	m_bShowFromMouse = false;
	m_nKlineType = -1;
	m_nTabPre = 0;
	m_maxChars = 30;
	jishunow = NULL;
	m_bInit = false;//CFloatEdit
	//{{AFX_DATA_INIT(CTaiKlineDlgEditScreenStock)
	m_checkmm = FALSE;
	m_checkOften = FALSE;
	m_explainbrief = _T("");
	m_myhelp = _T("");
	m_name = _T("");
	m_password = _T("");
	m_xline = _T("");
	m_sTestInfo = _T("");
	m_sKindTime = _T("中期");
	m_sKindFormu = _T("");
	//}}AFX_DATA_INIT
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<4;j++)
			m_editParam[i][j] = _T("");
	}
	m_strPeriodPre ="日线";
	memset(m_nPeriodsUsed,1,16);

	m_strParamExplain = "";

	m_nID[0][0] = IDC_EDIT11;
	m_nID[0][1] = IDC_EDIT12;
	m_nID[0][2] = IDC_EDIT13;
	m_nID[0][3] = IDC_EDIT14;

	m_nID[1][0] = IDC_EDIT21;
	m_nID[1][1] = IDC_EDIT22;
	m_nID[1][2] = IDC_EDIT23;
	m_nID[1][3] = IDC_EDIT24;

	m_nID[2][0] = IDC_EDIT31;
	m_nID[2][1] = IDC_EDIT32;
	m_nID[2][2] = IDC_EDIT33;
	m_nID[2][3] = IDC_EDIT34;

	m_nID[3][0] = IDC_EDIT41;
	m_nID[3][1] = IDC_EDIT42;
	m_nID[3][2] = IDC_EDIT43;
	m_nID[3][3] = IDC_EDIT44;

	AfxInitRichEdit( );

}
CTaiKlineDlgEditScreenStock::~CTaiKlineDlgEditScreenStock()
{
}

void CTaiKlineDlgEditScreenStock::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgEditScreenStock)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_DYHS, m_dyhs);
	DDX_Control(pDX, IDC_CSGS, m_csgs);
	DDX_Control(pDX, IDC_BUTTON2, m_button2);
	DDX_Control(pDX, IDC_BUTTON1, m_button1);
	DDX_Control(pDX, IDC_BUTTON_CAN_USED, m_canused);
	DDX_Control(pDX, IDC_EDIT12, m_floatEdit12);
	DDX_Control(pDX, IDC_EDIT13, m_floatEdit13);
	DDX_Control(pDX, IDC_EDIT14, m_floatEdit14);
	DDX_Control(pDX, IDC_EDIT22, m_floatEdit22);
	DDX_Control(pDX, IDC_EDIT23, m_floatEdit23);
	DDX_Control(pDX, IDC_EDIT24, m_floatEdit24);
	DDX_Control(pDX, IDC_EDIT32, m_floatEdit32);
	DDX_Control(pDX, IDC_EDIT33, m_floatEdit33);
	DDX_Control(pDX, IDC_EDIT34, m_floatEdit34);
	DDX_Control(pDX, IDC_EDIT42, m_floatEdit42);
	DDX_Control(pDX, IDC_EDIT43, m_floatEdit43);
	DDX_Control(pDX, IDC_EDIT44, m_floatEdit44);
	DDX_Control(pDX, IDC_TAB1, m_tabParam);
	DDX_Control(pDX, IDC_NAME, m_namecor);
	DDX_Control(pDX, IDC_PASSWORD, m_passwordcor);
	DDX_Control(pDX, IDC_HFQS, m_hfqs);
	DDX_Control(pDX, IDC_INPUT, m_inputcor);
	DDX_Check(pDX, IDC_CHECKMM, m_checkmm);
	DDX_Check(pDX, IDC_CHECK1, m_checkOften);
	DDX_Text(pDX, IDC_EXPLAINBRIEF, m_explainbrief);
	DDX_Text(pDX, IDC_MYHELP, m_myhelp);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Control(pDX, IDC_XLINE, m_xlinecor);
	DDX_Text(pDX, IDC_XLINE, m_xline);
	DDX_Text(pDX, IDC_STATIC_TEST_INFO, m_sTestInfo);
	DDX_CBString(pDX, IDC_COMBO_KIND_TIME, m_sKindTime);
	DDX_Text(pDX, IDC_EDIT1, m_sKindFormu);
	//}}AFX_DATA_MAP
	if(this->iskline == 2) DDV_MaxChars(pDX, m_name, 10);
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgEditScreenStock, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgEditScreenStock)
	ON_BN_CLICKED(IDC_HFQS, OnRestoreDefault)
	ON_BN_CLICKED(IDC_CSGS, OnTestFormula)
	ON_BN_CLICKED(IDC_DYHS, OnAddFunc)
	ON_BN_CLICKED(IDC_BUTTON2, OnImportFormula)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECKMM, OnCheckmm)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_CAN_USED, OnButtonCanUsed)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_KIND_TIME, OnSelchangeComboKindTime)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_INPUT, OnChangeInput)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock message handlers

///////////////////////////////////////////////////////////////////
//    
//      入口参数：无
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnTestFormula() 
{
  bool errpram=false;
  UpdateData();
  CString input=""		;
  m_inputcor.GetWindowText(input);

//  m_inputcor.SetRedraw(FALSE);
	
 	int i,j;
	for(int flag=0;flag<1;flag++)
	{
	
		int n = m_tabParam.GetCurSel();
		if(n==0)
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[i][j]=GetStringParam(i,j);//m_edit[i][j];
		}
		else 
		{
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[i+4][j]=GetStringParam(i,j);//m_edit[i][j];
//					SetStringParam(i,j,m_editParam[i+4][j]);//=m_edit[i][j];
		}

	
		for(i=0;i<8;i++)
		{
			bool bEmpty = true;
			for(j=1;j<4;j++)
			{
				if(m_editParam[i][j] != "")
				{
					bEmpty = false;
					break;
				}
			}

			//test name
			if(bEmpty == false)
			{
				  if(m_editParam[i][0]=="")
				  {
					  errorposition=2000+(i+1)*10+1;
					  errormessage="参数名不能空";
					  errpram=true;
					  break;
				  }
				  else if(!CheckParmNameInput(m_editParam[i][0],errormessage))
				  {
					  errorposition=2000+(i+1)*10+1;
					  errpram=true;
					  break;
				  }
			}
			else
			{
				m_editParam[i][0]=="";
				for(int k=i+1;k<8;k++)
				{
					for(int l=0;l<4;l++)
						m_editParam[k][l]=="";
				}

				break;
			}

			//test param
			for(j=1;j<4;j++)
			{
				if(m_editParam[i][j] == "")
				{
					errorposition=2000+(i+1)*10+1+j;
					errormessage="输入值不能空";
					errpram=true;
					break;
				}
				else
				{
					  if(!CheckParmInput(m_editParam[i][j]))
					  {
						  errorposition=2000+(i+1)*10+1+j;
						  errormessage="输入值不合法";
						  errpram=true;
						  break;
					  }
				}
				//
				if(j==3)
				{
					if(StringToFloat(m_editParam[i][3])>StringToFloat(m_editParam[i][1])||StringToFloat(m_editParam[i][3])<StringToFloat(m_editParam[i][2]))
					{
						  errorposition=2014;
						  errormessage="缺省值不在合法的范围";
						  errpram=true;
						  break;
				   }
				}

			}

		}

	}
//----------------------
	if(iskline>1)
	{
	   m_xline.Remove(' ');
	  int strstart=0;
	  CString  xpos;
	  int j=0;
	  for(int i=0;i<m_xline.GetLength();i++)
	  {
		  char test=m_xline.GetAt(i);
		  bool isreturn=false;             
		  bool isfinish=(test==';');      
		  bool isdot=(test=='.');         
		  if(test==13&&m_xline.GetAt(i+1)==10)
		  {
			  isreturn=true;
			  
		  }
		  
		  if(!(isdot||isreturn||isfinish||isdot||(test>='0'&&test<='9'||test=='-')))
		  {
			  errormessage="指标水平位置设置错误";
			  errorposition=3000+i;     
			  errpram=true;
			  break;
		  }
		  if(isfinish||isreturn||i==m_xline.GetLength()-1)
		  {
			  xpos=m_xline.Mid(strstart,i-strstart);
			  if(xpos!="")
			  {
				  if(j<7)
				  {
	//				jishunow->posX[j]=atof(xpos);
					j++;
					for(int k=i+1;k<m_xline.GetLength();k++)
					{
						char test=m_xline.GetAt(k);
						if(test>='0'&&test<='9')
						{
							strstart=k;
							break;
						}

					}
				  }
				  else
				  {
					  errorposition=3000+i;
					  errormessage="水平线位置太多";
					  errpram=true;
					  break;
				  }
			  }

		  }
		  if(isreturn)
			  i++;

	  }
	}//end XLINE

  CString formal=input;
  formal.Remove(' ');
  if(formal.GetLength()==0)
  {
	  errormessage="公式内容不能为空";
	  m_inputcor.SetFocus();
//	  m_inputcor.SetRedraw();
	  UpdateData(FALSE);
	  return;

  }
  else
  {
	  formal=input;
  }
  if(formal.GetAt(formal.GetLength()-1)==';')
	  formal.Delete(formal.GetLength()-1);
  formal.MakeLower();
  Kline kline;
 CFormularCompute *equation=new CFormularCompute(0,0,&kline,formal);

  for(i=0;i<8;i++)
  {
	  if(m_editParam[i][0]!=""&&m_editParam[i][3]!="")
	  {  
		  float fpram=StringToFloat(m_editParam[i][3]);
		  m_editParam[i][0].MakeLower();
		  int addpra=equation->AddPara(m_editParam[i][0],fpram);
		  switch(addpra)
		  {
		  case(1):
			  errorposition=2011;
			  errpram=true;
			  errormessage="参数个数太多";
			  break;

		  case(2):
			  errorposition=2011;
			  errpram=true;
			  errormessage="参数名重复或是系统关键字";
			  break;

		  }
		  if(errpram == true)
			  break;
  
	  }
  }

  if(errpram)
  {
	  errorposition-=2000;        
	  switch(errorposition)
	  {
	  case 11:
		  GetDlgItem(IDC_EDIT11)->SetFocus();
		  break;
	  case 12:
		  GetDlgItem(IDC_EDIT12)->SetFocus();
		  break;
	  case 13:
		  GetDlgItem(IDC_EDIT13)->SetFocus();
		  break;
	  case 14:
		  GetDlgItem(IDC_EDIT14)->SetFocus();
		  break;

	  case 21:
		  GetDlgItem(IDC_EDIT21)->SetFocus();
		  break;
	  case 22:
		  GetDlgItem(IDC_EDIT22)->SetFocus();
		  break;
	  case 23:
		  GetDlgItem(IDC_EDIT23)->SetFocus();
		  break;
	  case 24:
		  GetDlgItem(IDC_EDIT24)->SetFocus();
		  break;

	  case 31:
		  GetDlgItem(IDC_EDIT31)->SetFocus();
		  break;
	  case 32:
		  GetDlgItem(IDC_EDIT32)->SetFocus();
		  break;
	  case 33:
		  GetDlgItem(IDC_EDIT33)->SetFocus();
		  break;
	  case 34:
		  GetDlgItem(IDC_EDIT34)->SetFocus();
		  break;

	  case 41:
		  GetDlgItem(IDC_EDIT41)->SetFocus();
		  break;
	  case 42:
		  GetDlgItem(IDC_EDIT42)->SetFocus();
		  break;
	  case 43:
		  GetDlgItem(IDC_EDIT43)->SetFocus();
		  break;
	  case 44:
		  GetDlgItem(IDC_EDIT44)->SetFocus();
		  break;
	  default:
		  if(iskline>1)//技术指标
		  {
			  m_xlinecor.SetFocus();
			  m_xlinecor.SetSel(errorposition-1000,errorposition-1000);
		  }
		  break;
	  }
  		if(errorposition==-1)
			m_sTestInfo="测试通过!";
		else
			m_sTestInfo=errormessage;
	  UpdateData(FALSE);

	  return;
  }

equation->Devide();
  if(iskline<2)
  {
	  if(equation->m_RlineNum!=1)
	  {
		  if(equation->m_errpos==-1)
		  {
			  
			  if(equation->m_RlineNum==0)
			  {
				  equation->m_errmsg="公式必须有一个返回值";
				  equation->m_errpos=formal.GetLength();

			  }
			  else
			  {
				  int k=0,j=0;
				  int formallength=formal.GetLength();
				  char test;
				  for(int i=formallength-1;i>=0;i--)
				  {
					  test=formal.GetAt(i);
					  if(test==' '||test==';'||test=='\t'||test=='\n'||test=='\r')
						  continue;
					  else
						  break;
				  }
				  for( j=i+1;j<formal.GetLength();j++)
					  formal.Delete(j);
  
				  formal.Insert(i+1,';');
				  for( i=0;i<formal.GetLength();i++)
				  {
					  char test=formal.GetAt(i);
					  if(test==':'&&formal.GetAt(i+1)=='=')
					  {
						  k++;
					  }
					  if(test==';')
					  {
						  k--;
						  if(k<-1)
							  break;
						  else
							  j=i;

					  }
					  
				  }
				  if(equation->m_errpos==-1&&k<-1)
				  {
					  equation->m_errpos=j;
					  equation->m_errmsg="公式只能有一个表达式";

				  }
			  }
		  }
	  }
	  errorposition=equation->m_errpos;
	  errormessage=equation->m_errmsg;
  }
  else
  {
	  errorposition=equation->m_errpos;
	  errormessage=equation->m_errmsg;
	  if(errorposition==-1)
	  {
		  if(equation->m_RlineNum==0)
		  {
			  errorposition=formal.GetLength();
			  errormessage="公式必须至少画一条线";
		  }
		  else if(equation->m_RlineNum>NUM_LINE_TOT)
		  {
			  errorposition=formal.GetLength();
			  errormessage="公式最多只能计算32条线";
		  }


	  }
  }
  delete equation;
//     return;
  if(errorposition!=-1)
  {
	 m_inputcor.SetSel(errorposition,errorposition);
	 long lng = m_inputcor.LineFromChar(errorposition);
	 if(lng>=0)
	 {
		 int nln = m_inputcor.GetFirstVisibleLine();
		 if(nln>lng)
			 m_inputcor.LineScroll(lng-nln ,  0 );
		 else if(nln<lng)
		 {
			 m_inputcor.LineScroll(lng-nln-1 ,  0 );
		 }

	 }
	}
 
  	if(errorposition==-1)
		m_sTestInfo="测试通过!";
	else
		m_sTestInfo=errormessage;


	m_inputcor.SetFocus();

  UpdateData(FALSE);

}


//--------------
void CTaiKlineDlgEditScreenStock::OnOK() 
{
    UpdateData();
	if(jishunow)
		if(jishunow->sellStr != "")
			return;

	CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
	CString name=m_name;
	  CString input=""		;
	  m_inputcor.GetWindowText(input);
	name.Remove(' ');

	bool isnameok=true;
	
	for(int i=0;i<m_name.GetLength();i++)
	{
		char test=m_name.GetAt(i);
		if((test&0x80)!=0)
		{
			i=i+1;
			continue;
		}
//		if(!((test>='a'&&test<='z')||test=='#'||test=='_'||test=='%'||test == '(' || test == ')'||(test>='A'&&test<='Z')||(test>='0'&&test<='9')))
		{

		}
//	
		{

		}

	}
	if(!isnameok||name==""||name=="STOCKMODEKLINE"||name=="STOCKFILTER")
	{
		AfxMessageBox("指标名为空或非法，\n请重新输入",MB_OK);
		m_namecor.SetFocus();
		return;
	}

	OnTestFormula();

	
//-----------		
	if(errorposition!=-1)
		if(AfxMessageBox("您编辑的公式没有通过\n语法检查，系统将强制存储，\n\
要继续吗？",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
				    return;

	CFormularContent  *jishunew = NULL;
	if(item == -1)
	{
	jishunew=new CFormularContent();
	jishunow=jishunew;
	}
	else
	{
		if(iskline == 1)
			jishunow =pDoc->m_formuar_kline.GetAt(item);
		else if(iskline == 0)
			jishunow =pDoc->m_formuar_choose.GetAt(item);
		else
		{
			jishunow =pDoc->m_formuar_index.GetAt(item);
			CString namepre=jishunow->name;
			CTaiKeyBoardAngelDlg::DeleteIndicatorName(namepre);
		}

		int nKind = 0;
		if(iskline == 1)
			nKind =1;
		else if(iskline == 0)
			nKind =2;
		if(FindIndicator(nKind, m_name,jishunow))
		{
			AfxMessageBox(m_name+" 公式已存在，请重新选择!");
			return;
		}
	}
	
	if(item == -1)
	{
		if(iskline == 1)      
		{
			int length=pDoc->m_formuar_kline.GetSize();
			if(length>0)
			{
				int k;
				for(k=0;k<length;k++)
				{
					if(m_name.Compare(pDoc->m_formuar_kline.GetAt(k)->name)<=0)
						break;
				}
				if(k>=length)
				{
					pDoc->m_formuar_kline.Add(jishunew);
					addpos=length;
				}
				else
				{

					if(m_name.Compare(pDoc->m_formuar_kline.GetAt(k)->name)==0)
					{
						
						AfxMessageBox(m_name+"K线组合公式已存在，请重新选择!");
						m_namecor.SetFocus();
						delete jishunew;
						return ;
					}
					else
					{
						pDoc->m_formuar_kline.InsertAt(k,jishunew);
						addpos=k;
					}
				}
			}
			else
			{
				addpos=0;
				pDoc->m_formuar_kline.Add(jishunew);
			}
		}
			
		else  if(iskline == 0)
		{
			int length=pDoc->m_formuar_choose.GetSize();
			if(length>0)
			{
				int k;
				for(k=0;k<length;k++)
				{
					if(m_name.Compare(pDoc->m_formuar_choose.GetAt(k)->name)<=0)
						break;
				}
				if(k>=length)
				{
					pDoc->m_formuar_choose.Add(jishunew);
					addpos=length;
				}
				else
				{
					if(m_name.Compare(pDoc->m_formuar_choose.GetAt(k)->name)==0)
					{
						AfxMessageBox(m_name+"条件选股公式已存在，请重新选择!");
						m_namecor.SetFocus();
						delete jishunew;
						return ;
					}
					else
					{
					
						pDoc->m_formuar_choose.InsertAt(k,jishunew);
						addpos=k;
					}
				}
			}
			else
			{
				addpos=0;
				pDoc->m_formuar_choose.Add(jishunew);
			}
		}
		else
		{
			int length=pDoc->m_formuar_index.GetSize();
			if(length>0)
			{
				int k;
				for(k=0;k<length;k++)
				{
					if(m_name.Compare(pDoc->m_formuar_index.GetAt(k)->name)<=0)
						break;
				}
				if(k>=length)
				{
					pDoc->m_formuar_index.Add(jishunew);
					addpos=length;
				}
				else
				{

					if(m_name.Compare(pDoc->m_formuar_index.GetAt(k)->name)==0)
					{
						AfxMessageBox(m_name+"指标公式已存在，请重新选择!");
						delete jishunew;
						return ;
					}
					else
					{
						pDoc->m_formuar_index.InsertAt(k,jishunew);
						addpos=k;
					}
				}
			}
			else
			{
				pDoc->m_formuar_index.Add(jishunew);
				addpos=0;
			}
		}
	}
			
	if(iskline == 2)
	{
	//-----------------------
			for(int k=0;k<7;k++)
			{
				jishunow->posX[k]=-10e20;
			} 
			CString xpos;

			int j=0,strstart=0;
			for(int i=0;i<m_xline.GetLength();i++)
		  {
			  char test=m_xline.GetAt(i);
			  bool isreturn=false;           
			  bool isfinish=(test==';');    
			  if(test==13&&m_xline.GetAt(i+1)==10)
			  {
				  isreturn=true;
				  
			  }
			  
			  if(isfinish||isreturn||i==m_xline.GetLength()-1)
			  {
				  xpos=m_xline.Mid(strstart,i-strstart);
				  if(xpos!="")
				  {
					 jishunow->posX[j]=atof(xpos);
					 j++;
					for(int k=i+1;k<m_xline.GetLength();k++)
					{
						char test=m_xline.GetAt(k);
						if(test>='0'&&test<='9'||test == '-')
						{
							strstart=k;
							break;
						}
					}
					  
				  }

			  }
			  if(isreturn)
				  i++;

		  }
	//--------------------
			int itemid;
			itemid=GetCheckedRadioButton(IDC_ZTDJ,IDC_FT);
			if(itemid==IDC_ZTDJ)
				jishunow->isMainFiguer=true;
			else
				jishunow->isMainFiguer=false;
	}
	
	
		
//--------------
	int pramnum=0;
	for(int i=0;i<8;i++)
	{
		if(m_editParam[i][0]!=""&&m_editParam[i][3]!="")
			pramnum++;
		else
			break;
	}
	jishunow->numPara=pramnum;
	for(int i=0;i<pramnum;i++)
	{
		jishunow->namePara[i]=m_editParam[i][0];
		jishunow->namePara[i].MakeLower ();
		jishunow->max[i]=StringToFloat(m_editParam[i][1]);
		jishunow->min[i]=StringToFloat(m_editParam[i][2]);

		float f = StringToFloat(m_editParam[i][3]);
		if(m_nKlineType!=-1)
			CTaiKlineDlgChangeIndexParam::SetParamLjishu(jishunow,m_nKlineType,i,(CMainFrame::m_taiShanDoc)->m_propertyInitiate.bSaveParam,f);
		else
			jishunow->defaultVal[i]=f;
	}	

	 //temp initiate
	 jishunow->InitDefaultValArray();

//------------------------
	 jishunow->fomular=input;
	 if((m_checkmm==TRUE))
		 jishunow->password=m_password;
	 jishunow->explainBrief=m_explainbrief;
	 jishunow->name=m_name;
	 jishunow->isProtected=m_checkmm;
	 jishunow->help=m_myhelp;
	 if(m_sKindTime!="长期"&&m_sKindTime!="中期"&&m_sKindTime!="短期")
		 m_sKindTime = "中期";                  
	 jishunow->subKindIndexTime = m_sKindTime;
	 memcpy(jishunow->nPeriodsUsed , m_nPeriodsUsed,11);
//	 CComboBox* pCombo = (CComboBox* )GetDlgItem(IDC_COMBO_KIND_TIME)->GetWindowText();
	 jishunow->explainParam = m_strParamExplain ;
	 jishunow->subKindIndex = m_sKindFormu;
	 jishunow->isOfen  = m_checkOften;

	 if(pramnum!=jishunow->defaultValArray.GetSize())
		jishunow->AddDefaultValToArray();

	 CFormularCompute::FanEach(jishunow);

	 CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////
//      功能说明：增加函数
//      入口参数：无
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnAddFunc() 
{
	UpdateData();
	CDialogEDITZBGSHS m_diazbgshs;
	if(m_diazbgshs.DoModal()==IDOK)
	{
		m_inputcor.SetFocus();
		functionstr=m_diazbgshs.m_selectedstr;
		int length=functionstr.GetLength();
		int i;
		for(i=0;i<length;i++)
		{
			char test=functionstr.GetAt(i);
			if(test=='-'&&functionstr.GetAt(i+1)=='-')
				break;
		}
		if(i==length)
			functionstr="";
		else
			functionstr=functionstr.Left(i);
		m_inputcor.ReplaceSel(functionstr);
	
	}

	
}

BOOL CTaiKlineDlgEditScreenStock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTaiShanDoc*	pDoc=(CMainFrame::m_taiShanDoc );

	if(m_nKlineType!=-1)
	{
		CString sTl ;
		GetWindowText(sTl);
		CString s2 = CTaiScreenParent::GetPeriodName(m_nKlineType);
		sTl+="（";
		sTl+=s2;
		sTl+="）";
		SetWindowText(sTl);
	}

	if(m_bInit == false)
	{
		m_tabParam.InsertItem (0,"参数1-4");
		m_tabParam.InsertItem (1,"参数5-8");
		m_bInit = true;
	}

	if(item!=-1)
	{
//	
	}

	if(iskline<2)                   
	{
		if(iskline==0)
			this->SetWindowText("条件选股公式编辑器");
		else
			this->SetWindowText("K线组合公式编辑器");
		
		if(item!=-1)
		{
			if(iskline==0)
				jishunow=pDoc->m_formuar_choose.GetAt(item);
			if(iskline==1)
				jishunow=pDoc->m_formuar_kline.GetAt(item);
			m_name=jishunow->name;
			m_myhelp=jishunow->help;
			m_checkOften = jishunow->isOfen ;
		//	m_namecor.SetReadOnly(TRUE);
			m_explainbrief=jishunow->explainBrief;
			if(jishunow->isProtected)
			{
				m_checkmm=true;
				m_password=jishunow->password;
			}
			else 
				m_passwordcor.EnableWindow(FALSE);

			 m_inputcor.SetWindowText(jishunow->fomular);
			 m_inputcor.SetFocus();
			 numPara = jishunow->numPara ;

			 //temp initiate
			 jishunow->InitDefaultValArray();

			 ParamIntoOther(0);
			 ParamIntoOther(2);


//			m_strPeriodPre ="";
			memcpy(m_nPeriodsUsed,jishunow->nPeriodsUsed,16);

			m_sKindTime=jishunow->subKindIndexTime  ;
			this->m_sKindFormu = jishunow->subKindIndex ;
			m_strParamExplain = jishunow->explainParam ;

			UpdateData(FALSE);
		}
		else
		{
			 m_checkmm=false;
			 m_hfqs.EnableWindow(FALSE);	 
			 UpdateData(FALSE);

		}
	}
	else
	{
		if(item!=-1)          
		{
			jishunow=pDoc->m_formuar_index.GetAt(item);
			m_name=jishunow->name;
	//		m_namecor.SetReadOnly(TRUE);
			if(pDoc->m_formuar_index.GetAt(item)->isMainFiguer==1)
				 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_ZTDJ);
			else 
				 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_FT);
			CString strxline;
			for(int k=0;k<7;k++)
			{
				if(jishunow->posX[k]>=-10e19)
				{
					CString test;
					test.Format("%.2f",jishunow->posX[k]);
					strxline+=test;
					strxline+=';';

				}
				else
					break;
			}
			m_xline=strxline;
			m_explainbrief=jishunow->explainBrief;
			if(jishunow->isProtected)
			{
				m_checkmm=true;
				m_password=jishunow->password;
			}
			else 
			{
				m_checkmm=false;
				m_passwordcor.EnableWindow(FALSE);
			}

			if(jishunow->isSystem)
				m_namecor.SetReadOnly(TRUE);


		//---------------------
			m_myhelp=jishunow->help;
			m_inputcor.SetWindowText(pDoc->m_formuar_index.GetAt(item)->fomular);
				m_inputcor.SetFocus();

			 //temp initiate
			 jishunow->InitDefaultValArray();
			m_checkOften = jishunow->isOfen ;

			ParamIntoOther(0);
			ParamIntoOther(2);

			memcpy(m_nPeriodsUsed,jishunow->nPeriodsUsed,16);
			m_sKindTime=jishunow->subKindIndexTime  ;
			this->m_sKindFormu = jishunow->subKindIndex ;
			m_strParamExplain = jishunow->explainParam ;

			UpdateData(FALSE);
		}
		else           
		{
			m_myhelp="";
			 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_FT);
			 m_checkmm=false;
			 m_hfqs.EnableWindow(FALSE);	 
			 UpdateData(FALSE);

		}

//		m_xlinecor.ShowWindow (SW_SHOW);
			;
	}

	if(jishunow)
		if(jishunow->sellStr != "")
		{
			m_inputcor.SetWindowText("");
		}


	g_bInitDlgEdit = false;
	OnChangeInput2() ;
	m_inputcor.SetEventMask(m_inputcor.GetEventMask() |
    ENM_CHANGE);
	g_bInitDlgEdit = true;

	DoMoveWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////
//      功能说明：浮点数转换为字符串
//      入口参数：fdata：浮点数
//      返回参数：str:返回字符串
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::ShowData(CString &str, float fdata)
{
	if(fdata==-1000)
		str="";
	else
	{
		str.Format("%.4f",fdata);
		int strlength=str.GetLength();
		str.TrimRight ("0");
		str.TrimRight (".");
	}
}

///////////////////////////////////////////////////////////////////
//      功能说明：字符串转换为浮点数
//      入口参数：str:字符串
//      返回参数：返回浮点数
/////////////////////////////////////////////////////////////////////////////
float CTaiKlineDlgEditScreenStock::StringToFloat(CString str)
{
	char * test;
	int length=str.GetLength();
	test=str.GetBuffer(length);
	return (float)atof(test);
}

void CTaiKlineDlgEditScreenStock::OnPaint() 
{
	CDialog::OnPaint();
	
	// Do not call CDialog::OnPaint() for painting messages
}

///////////////////////////////////////////////////////////////////
//      功能说明：测试参数数据是否合法
//      入口参数：parminput:参数数据
//      返回参数：返回是否正确
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnCheckmm() 
{
	if(IsDlgButtonChecked(IDC_CHECKMM))
	   m_passwordcor.EnableWindow(TRUE);
   else
	   m_passwordcor.EnableWindow(FALSE);
	   

	
}

///////////////////////////////////////////////////////////////////
//      功能说明：测试参数数据是否合法
//      入口参数：parminput:参数数据
//      返回参数：返回是否正确
/////////////////////////////////////////////////////////////////////////////
BOOL CTaiKlineDlgEditScreenStock::CheckParmInput(CString parminput)
{
	int inputlength=parminput.GetLength();
	char test;
	for(int i=0;i<inputlength;i++)
	{
		
		test=parminput.GetAt(i);
		if(!(test=='-'||test=='+'||test=='.'||(test>='0'&&test<='9')))
		{
			return FALSE;
		}

	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////
//      功能说明：检测参数名称是否合法
//      入口参数：parname:参数名称，errmessage：错误信息
//      返回参数：返回是否正确
/////////////////////////////////////////////////////////////////////////////
BOOL CTaiKlineDlgEditScreenStock::CheckParmNameInput(CString parname, CString &errmessage)
{
	parname.Remove(' ');
	char test=parname.GetAt(0);
	if(test&0x80 == 0 && (test<'A'||test>'z'||test=='_'))
	{
		errormessage="参数名必须以字母开头";
		return FALSE;
	}
	int length=parname.GetLength();
	for(int i=1;i<length;i++)
	{
		test=parname.GetAt(i);
		if((test>='0'&&test<='9')||(test>='A'&&test<='z')||test=='_'||((BYTE)test)>=0x80 )
			continue;
		else
		{
			errormessage="参数名组成非法";
			return FALSE;
		}
	}
	return TRUE;

}

///////////////////////////////////////////////////////////////////
//      功能说明：恢复缺省数据
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnRestoreDefault() 
{
	// TODO: Add your control notification handler code here
	((CComboBox*)GetDlgItem(IDC_COMBO_KIND_TIME))->ResetContent( );

	OnInitDialog();
	
}

///////////////////////////////////////////////////////////////////
//      功能说明：引入技术指标公式
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnImportFormula() 
{
	// TODO: Add your control notification handler code here
	CFxjSelectFormu dlg;
	if(dlg.DoModal () == IDOK)
	{
		CFxjShowFormu dlg2 (this);
		dlg2.m_s =dlg.GetJishu() ->fomular ;

		if(dlg.GetJishu() ->sellStr != "")
			return;

		dlg2.m_title = dlg.GetJishu() ->name ;
		if(dlg2.DoModal () == IDOK)
		{
//			m_inputcor.Paste ();
		}

	}
	return;

	CDialogLOGXZZB mdiaxzzb(this);
	int item2 = -1;
	if(mdiaxzzb.DoModal() == IDOK)
	{
		CString s = mdiaxzzb.result;
		CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc ;
		int n = pDoc->m_formuar_index .GetSize ();
		for(int i=0;i<n;i++)
		{
			if(s == pDoc->m_formuar_index.GetAt (i)->name )
			{
				item2 = i;
				break;
			}
		}
		if(item2==-1)
			return;

		//test
		if(pDoc->m_formuar_index.GetAt (item2)->isProtected !=0)
		{
			CDialogGSGLMM mdiagsglmm;
			mdiagsglmm.password=pDoc->m_formuar_index.GetAt (item2)->password;
			if(mdiagsglmm.DoModal()!=IDOK)
				return;
		}
		
		ASSERT(item==-1);
		item = item2;
		int temp = iskline;
		iskline = 2;

		CString s2 = this->m_sKindFormu ;
		((CComboBox*)GetDlgItem(IDC_COMBO_KIND_TIME))->ResetContent( );

		this->OnInitDialog ();
		this->m_sKindFormu = s2;
		UpdateData(FALSE);

		iskline = temp;
		item = -1;
	
	}
	
}


void CTaiKlineDlgEditScreenStock::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}



///////////////////////////////////////////////////////////////////
//      功能说明：改变标签的字体的颜色
//      入口参数：参阅msdn帮助文件
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
HBRUSH CTaiKlineDlgEditScreenStock::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
   if (pWnd->GetDlgCtrlID() == IDC_STATIC_TEST_INFO )
   {
	   if(errorposition==-1)
			pDC->SetTextColor(RGB(255,0,0));
		else
			pDC->SetTextColor(RGB(0,0,255));
   }

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

///////////////////////////////////////////////////////////////////
//      功能说明：参数1-4与参数5-8之间的切换处理
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	UpdateData();
//	static int nTabPre = 0;
	int n = m_tabParam.GetCurSel();
	int i,j;
	if(m_nTabPre!=n)
	{
		if(n==0)
		{//把参数信息(5-8)保存到m_editParam中,把参数信息(1-4)从m_editParam中恢复到m_edit
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[4+i][j]=GetStringParam(i,j);//m_edit[i][j];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					SetStringParam(i,j,m_editParam[i][j]);
		}
		else if(n==1)//
		{
		  if(GetStringParam(0,0)==""||GetStringParam(1,0)==""||GetStringParam(2,0)==""||GetStringParam(3,0)=="")
		  {
			  m_tabParam.SetCurSel(0);
			  n=0;
		  }
		  else//把参数信息(1-4)保存到m_editParam中,把参数信息(5-8)从m_editParam中恢复到m_edit
		  {
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					m_editParam[i][j]=GetStringParam(i,j);//m_edit[i][j];
			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					SetStringParam(i,j,m_editParam[i+4][j]);
		  }
		}
		UpdateData(FALSE);
	}
	m_nTabPre = n;

	*pResult = 0;
}

///////////////////////////////////////////////////////////////////
//      功能说明：参数转换
//      入口参数：nFlag=0:前四个参数输出到编辑框,nFlag=1:后四个参数输出到编辑框,nFlag=2:所有参数输出到缓存
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::ParamIntoOther(int nFlag)//
{
	int i,j;
	if(nFlag >1)
	{
		if(jishunow->numPara>8)
			jishunow->numPara = 0;
		for(i=0;i<8;i++)
		{
			if(i<jishunow->numPara)
			{
				m_editParam[i][0]=jishunow->namePara[i];
				ShowData(m_editParam[i][1],jishunow->max[i]);
				ShowData(m_editParam[i][2],jishunow->min[i]);
				float f = jishunow->defaultVal[i];
				if(m_nKlineType!=-1)
					f = CFormularContent::GetParamDataEach(i,m_nKlineType,jishunow);
				ShowData(m_editParam[i][3],f);

			}
			else
			{
				for(j=0;j<4;j++)
					m_editParam[i][j]="";
			}
			;
		}
	}
	else
	{
		if(jishunow->numPara>8)
			jishunow->numPara = 0;
		int nBegin = 0;
		if(nFlag==1&&jishunow->numPara>4)
			nBegin = 4;
		for(i=nBegin;i<4+nBegin;i++)
		{
			if(i<jishunow->numPara-nBegin)
			{
				SetStringParam(i,0,jishunow->namePara[i]);
				CString s;
				ShowData(s,jishunow->max[i]);
				SetStringParam(i,1,s);
				ShowData(s,jishunow->min[i]);
				SetStringParam(i,2,s);

				float f = jishunow->defaultVal[i];
				if(m_nKlineType!=-1)
					f = CFormularContent::GetParamDataEach(i,m_nKlineType,jishunow);

				ShowData(s,f);
				SetStringParam(i,3,s);
			}
			else
				for(j=0;j<4;j++)
					SetStringParam(i,j,"");
		}
	}
}

///////////////////////////////////////////////////////////////////
//      功能说明：改变技术指标数据的指标可用时间周期的内容
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnButtonCanUsed() 
{
	// TODO: Add your control notification handler code here
	CTaiKlineDlgCanUsedPeriod dlgCanUsed;
	dlgCanUsed.InputString(m_nPeriodsUsed);
	if(dlgCanUsed.DoModal ()==IDOK)
		dlgCanUsed.OutSetting (m_nPeriodsUsed);
}

///////////////////////////////////////////////////////////////////
//      功能说明：改变指标公式数据的指标参数注释内容
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditScreenStock::OnButton1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogLOGGSSM mdiagssm;
	mdiagssm.m_bReadOnly = false;
	mdiagssm.m_edit=m_strParamExplain;
	if(mdiagssm.DoModal()==IDOK)
		m_strParamExplain=mdiagssm.m_edit;
	UpdateData(FALSE);
}

void CTaiKlineDlgEditScreenStock::OnSelchangeComboKindTime() 
{
	// TODO: Add your control notification handler code here

}

void CTaiKlineDlgEditScreenStock::ChangePeriodParamData(bool bSave)
{




}



CString CTaiKlineDlgEditScreenStock::GetStringParam(int x, int y)
{
	CString s ="";
	GetDlgItem(m_nID[x][y])->GetWindowText(s);
	return s;
}

void CTaiKlineDlgEditScreenStock::SetStringParam(int x, int y, CString sIn)
{
	GetDlgItem(m_nID[x][y])->SetWindowText(sIn);
}

BOOL CTaiKlineDlgEditScreenStock::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineIndicatorsManage::IDD);
//	return TRUE;//
	DoHtmlHelp(this);return TRUE;
}

bool CTaiKlineDlgEditScreenStock::FindIndicator(int nKind, CString sName,CFormularContent  *jishunow)
{
	CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	Formu_Array1* pArray[3] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose),&(pDoc->m_formuar_kline )};
	int length=pArray[nKind]->GetSize();
	if(length>0)
	{
		for(int k=0;k<length;k++)
		{
			if(sName.Compare(pArray[nKind]->GetAt(k)->name)==0 && jishunow!=pArray[nKind]->GetAt(k))
				return true;
		}
	}
	else return false;
	return false;
}

void CTaiKlineDlgEditScreenStock::OnChangeInput() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData();
	if(g_bInitDlgEdit)
		OnChangeInput2();
	
	
}

void CTaiKlineDlgEditScreenStock::OnChangeInput2()
{
	// TODO: Add your control notification handler code here
  CString formal=""		;
  m_inputcor.GetWindowText(formal);

  if(formal.GetLength()==0)
  {
	  return;
  }
  formal.MakeLower();
  Kline kline;
  CFormularCompute equation(0,0,&kline,formal);
  equation.m_bShowColor = true;
	
  bool errpram = false;
  for(int i=0;i<8;i++)
  {
	  if(m_editParam[i][0]!=""&&m_editParam[i][3]!="")
	  {  
		  float fpram=StringToFloat(m_editParam[i][3]);
		  m_editParam[i][0].MakeLower();
		  int addpra=equation.AddPara(m_editParam[i][0],fpram);
		  switch(addpra)
		  {
		  case(1):
		  case(2):
			  errpram=true;
			  break;
		  }
		  if(errpram == true)
		  {
			  AfxMessageBox("请先设置正确参数！");
			  return;
		  }
	  }
  }
  equation.Devide();

  //取得位置颜色信息
  if(equation.m_errpos==-1 && equation.m_pKindPos )
  {
  //formal;
	  COLORREF  clr[10] = {RGB(0,0,255),RGB(0,150,0),
		  RGB(120,0,0),RGB(255,0,0),
		  RGB(0,255,255),RGB(0,0,120)};

	  m_inputcor.SetRedraw(FALSE);
	  CPoint pt =  GetCaretPos( );
	  long nStartChar;
	  long nEndChar;
	  m_inputcor.GetSel( nStartChar, nEndChar ) ;

	  static int iii = 0;
	  iii++;
	  TRACE("iii = %d",iii);
	  for(int k = 0;k<equation.m_nSizeKindPos ;k++)
	  {
		  int b = equation.m_pKindPos[k].m_nPos;
		  int e ;
		  if(k == equation.m_nSizeKindPos -1)
			  e = formal.GetLength ()-1;
		  else
			  e = equation.m_pKindPos[k+1].m_nPos;
		  if(e<0) e = 0;
		  if(e>formal.GetLength ()-1)
			  e = formal.GetLength ()-1;
		  if(b<0) b = 0;
		  if(b>formal.GetLength ()-1)
			  b = formal.GetLength ()-1;

		  m_inputcor.SetSel(b,e);
		  if(equation.m_pKindPos[k].m_nKind<10
			  &&equation.m_pKindPos[k].m_nKind>=0)
			m_inputcor.SetSelectColor( clr[equation.m_pKindPos[k].m_nKind]);

	  }
	  m_inputcor.SetSel( nStartChar, nEndChar ) ;
//	  m_inputcor.HideSelection(TRUE, FALSE);

//SetSel(-1,-1);
	  SetCaretPos( pt );
	  m_inputcor.SetRedraw();
	  m_inputcor.RedrawWindow();
  }

}

void CTaiKlineDlgEditScreenStock::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect r;
	if(m_inputcor.m_hWnd )
	{
		GetDlgItem (IDC_EDIT41)->GetWindowRect(r);
		this->ScreenToClient (r);
//		m_inputcor.GetWindowRect(r);
		CRect r2(5,r.bottom+12,cx-5,cy-5);
		m_inputcor.MoveWindow(r2);
	}
	
}
void CTaiKlineDlgEditScreenStock::DoMoveWindow()
{
	if(m_bShowFromMouse)
	{
		CPoint p;
		GetCursorPos(&p);
		CRect r;
		GetWindowRect(r);
		this->SetWindowPos(NULL,p.x,p.y,r.Width (),r.Height (),SWP_NOREDRAW);
	}

}
