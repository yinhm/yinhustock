// DIGEDITTJXG.cpp : implementation file
//

#include "stdafx.h"
#include "vwbase.h"
#include "DIGEDITTJXG.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
#include "DIAEDITZBGSHS.h"
#include "CFormularContent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDIGEDITTJXG::CDIGEDITTJXG(CWnd* pParent /*=NULL*/)
	: CDialog(CDIGEDITTJXG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDIGEDITTJXG)
	m_checkmm = FALSE;
	m_edit11 = _T("");
	m_edit12 = _T("");
	m_edit13 = _T("");
	m_edit14 = _T("");
	m_edit21 = _T("");
	m_edit22 = _T("");
	m_edit23 = _T("");
	m_edit24 = _T("");
	m_edit31 = _T("");
	m_edit33 = _T("");
	m_edit32 = _T("");
	m_edit34 = _T("");
	m_edit41 = _T("");
	m_edit42 = _T("");
	m_edit43 = _T("");
	m_edit44 = _T("");
	m_explainbrief = _T("");
	m_myhelp = _T("");
	m_name = _T("");
	m_input = _T("");
	m_password = _T("");
	//}}AFX_DATA_INIT
}


void CDIGEDITTJXG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDIGEDITTJXG)
	DDX_Control(pDX, IDC_NAME, m_namecor);
	DDX_Control(pDX, IDC_PASSWORD, m_passwordcor);
	DDX_Control(pDX, IDC_HFQS, m_hfqs);
	DDX_Control(pDX, IDC_INPUT, m_inputcor);
	DDX_Control(pDX, IDC_EDIT44, m_edit44cor);
	DDX_Control(pDX, IDC_EDIT43, m_edit43cor);
	DDX_Control(pDX, IDC_EDIT42, m_edit42cor);
	DDX_Control(pDX, IDC_EDIT41, m_edit41cor);
	DDX_Control(pDX, IDC_EDIT34, m_edit34cor);
	DDX_Control(pDX, IDC_EDIT33, m_edit33cor);
	DDX_Control(pDX, IDC_EDIT32, m_edit32cor);
	DDX_Control(pDX, IDC_EDIT31, m_edit31cor);
	DDX_Control(pDX, IDC_EDIT24, m_edit24cor);
	DDX_Control(pDX, IDC_EDIT23, m_edit23cor);
	DDX_Control(pDX, IDC_EDIT22, m_edit22cor);
	DDX_Control(pDX, IDC_EDIT21, m_edit21cor);
	DDX_Control(pDX, IDC_EDIT14, m_edit14cor);
	DDX_Control(pDX, IDC_EDIT13, m_edit13cor);
	DDX_Control(pDX, IDC_EDIT12, m_edit12cor);
	DDX_Control(pDX, IDC_EDIT11, m_edit11cor);
	DDX_Check(pDX, IDC_CHECKMM, m_checkmm);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDV_MaxChars(pDX, m_edit11, 6);
	DDX_Text(pDX, IDC_EDIT12, m_edit12);
	DDX_Text(pDX, IDC_EDIT13, m_edit13);
	DDX_Text(pDX, IDC_EDIT14, m_edit14);
	DDX_Text(pDX, IDC_EDIT21, m_edit21);
	DDV_MaxChars(pDX, m_edit21, 6);
	DDX_Text(pDX, IDC_EDIT22, m_edit22);
	DDX_Text(pDX, IDC_EDIT23, m_edit23);
	DDX_Text(pDX, IDC_EDIT24, m_edit24);
	DDX_Text(pDX, IDC_EDIT31, m_edit31);
	DDV_MaxChars(pDX, m_edit31, 6);
	DDX_Text(pDX, IDC_EDIT33, m_edit33);
	DDX_Text(pDX, IDC_EDIT32, m_edit32);
	DDX_Text(pDX, IDC_EDIT34, m_edit34);
	DDX_Text(pDX, IDC_EDIT41, m_edit41);
	DDV_MaxChars(pDX, m_edit41, 6);
	DDX_Text(pDX, IDC_EDIT42, m_edit42);
	DDX_Text(pDX, IDC_EDIT43, m_edit43);
	DDX_Text(pDX, IDC_EDIT44, m_edit44);
	DDX_Text(pDX, IDC_EXPLAINBRIEF, m_explainbrief);
	DDX_Text(pDX, IDC_MYHELP, m_myhelp);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 30);
	DDX_Text(pDX, IDC_INPUT, m_input);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDIGEDITTJXG, CDialog)
	//{{AFX_MSG_MAP(CDIGEDITTJXG)
	ON_BN_CLICKED(IDC_CSGS, OnCsgs)
	ON_BN_CLICKED(IDC_DYHS, OnDyhs)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECKMM, OnCheckmm)
	ON_BN_CLICKED(IDC_HFQS, OnHfqs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CDIGEDITTJXG::OnCsgs() 
{
  Kline *kline;
  bool errpram=false;
  UpdateData();

	
 	for(int flag=0;flag<1;flag++)
	{
	  if(m_edit14!=""||m_edit12!=""||m_edit13!="")
	  {
		  if(m_edit11=="")
		  {
			  errorposition=2011;
			  errormessage="参数名不能空";
			  errpram=true;
			  break;
		  }
		  else if(!CheckParmNameInput(m_edit11,errormessage))
		  {
			  errorposition=2011;
			  errpram=true;
			  break;
		  }
		  		  
	  }

	  if(m_edit11!=""&&m_edit14=="")
	  {
		  errorposition=2014;
		  errormessage="参数的缺省值不能空";
		  errpram=true;
	  }

	  if(!(m_edit11==""||m_edit14==""))
	  {
		  if(!CheckParmInput(m_edit12))
		  {
			  errorposition=2012;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
		  if(!CheckParmInput(m_edit13))
		  {
			  errorposition=2013;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;

		  }
			  
		   if(!CheckParmInput(m_edit14))
		  {
			  errorposition=2014;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
				  
		  if(StringToFloat(m_edit14)>StringToFloat(m_edit12)||StringToFloat(m_edit14)<StringToFloat(m_edit13))
		  {
				  errorposition=2014;
				  errormessage="缺省值不在合法的范围";
				  errpram=true;
		   }
	  }

	 if(m_edit24!=""||m_edit22!=""||m_edit23!="")
	  {
		  if(m_edit21=="")
		  {
			  errorposition=2021;
			  errormessage="参数名不能空";
			  errpram=true;
			  break;
		  }
		  else if(!CheckParmNameInput(m_edit21,errormessage))
		  {
			  errorposition=2021;
			  errpram=true;
			  break;
		  }
		  		  
	  }

	  if(m_edit21!=""&&m_edit24=="")
	  {
		  errorposition=2024;
		  errormessage="参数的缺省值不能空";
		  errpram=true;
	  }

	  if(!(m_edit21==""||m_edit24==""))
	  {
		  if(!CheckParmInput(m_edit22))
		  {
			  errorposition=2022;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
		  if(!CheckParmInput(m_edit23))
		  {
			  errorposition=2023;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;

		  }
			  
		   if(!CheckParmInput(m_edit24))
		  {
			  errorposition=2024;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
				  
		  if(StringToFloat(m_edit24)>StringToFloat(m_edit22)||StringToFloat(m_edit24)<StringToFloat(m_edit23))
		  {
				  errorposition=2024;
				  errormessage="缺省值不在合法的范围";
				  errpram=true;
		   }
	  }

	  if(m_edit34!=""||m_edit32!=""||m_edit33!="")
	  {
		  if(m_edit31=="")
		  {
			  errorposition=2031;
			  errormessage="参数名不能空";
			  errpram=true;
			  break;
		  }
		  else if(!CheckParmNameInput(m_edit31,errormessage))
		  {
			  errorposition=2031;
			  errpram=true;
			  break;
		  }
		  		  
	  }

	  if(m_edit31!=""&&m_edit34=="")
	  {
		  errorposition=2034;
		  errormessage="参数的缺省值不能空";
		  errpram=true;
	  }

	  if(!(m_edit31==""||m_edit34==""))
	  {
		  if(!CheckParmInput(m_edit32))
		  {
			  errorposition=2032;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
		  if(!CheckParmInput(m_edit33))
		  {
			  errorposition=2033;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;

		  }
			  
		   if(!CheckParmInput(m_edit34))
		  {
			  errorposition=2034;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
				  
		  if(StringToFloat(m_edit34)>StringToFloat(m_edit32)||StringToFloat(m_edit34)<StringToFloat(m_edit33))
		  {
				  errorposition=2034;
				  errormessage="缺省值不在合法的范围";
				  errpram=true;
		   }
	  }

	 if(m_edit44!=""||m_edit42!=""||m_edit43!="")
	  {
		  if(m_edit41=="")
		  {
			  errorposition=2041;
			  errormessage="参数名不能空";
			  errpram=true;
			  break;
		  }
		  else if(!CheckParmNameInput(m_edit41,errormessage))
		  {
			  errorposition=2041;
			  errpram=true;
			  break;
		  }
		  		  
	  }

	  if(m_edit41!=""&&m_edit44=="")
	  {
		  errorposition=2044;
		  errormessage="参数的缺省值不能空";
		  errpram=true;
	  }

	  if(!(m_edit41==""||m_edit44==""))
	  {
		  if(!CheckParmInput(m_edit42))
		  {
			  errorposition=2042;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
		  if(!CheckParmInput(m_edit43))
		  {
			  errorposition=2043;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;

		  }
			  
		   if(!CheckParmInput(m_edit44))
		  {
			  errorposition=2044;
			  errormessage="输入值不合法";
			  errpram=true;
			  break;
		  }
				  
		  if(StringToFloat(m_edit44)>StringToFloat(m_edit42)||StringToFloat(m_edit44)<StringToFloat(m_edit43))
		  {
				  errorposition=2044;
				  errormessage="缺省值不在合法的范围";
				  errpram=true;
		   }
	  }		
}

  CString formal=m_input;
  formal.Remove(' ');
  if(formal.GetLength()==0)
  {
	  errormessage="公式内容不能为空";
	  m_inputcor.SetFocus();
	  InvalidateRect(&datarect);
	  UpdateWindow();
	  return;

  }
  else
  {
	  formal=m_input;
  }
  if(formal.GetAt(formal.GetLength()-1)==';')
	  formal.Delete(formal.GetLength()-1);
  formal.MakeLower();
  CFormularCompute *equation=new CFormularCompute(0,0,kline,formal);

   if(m_edit11!=""&&m_edit14!="")
  {  
	  float fpram=StringToFloat(m_edit14);
	  m_edit11.MakeLower();
	  int addpra=equation->AddPara(m_edit11,fpram);
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
  
  }
   if(m_edit21!=""&&m_edit24!="")
   { 
	  float fpram=StringToFloat(m_edit24);
	  m_edit21.MakeLower();
	  int addpra=equation->AddPara(m_edit21,fpram);
	  switch(addpra)
	  {
	  case(1):
		  errorposition=2021;
		  errpram=true;
		  errormessage="参数个数太多";
		  break;

	  case(2):
		  errorposition=2021;
		  errpram=true;
		  errormessage="参数名重复或是系统关键字";
		  break;

	  }
   }

  if(m_edit31!=""&&m_edit34!="") 
  { 
	  float fpram=StringToFloat(m_edit34);
	  m_edit31.MakeLower();
	  int addpra=equation->AddPara(m_edit31,fpram);
	  switch(addpra)
	  {
	  case(1):
		  errorposition=2031;
		  errpram=true;
		  errormessage="参数个数太多";
		  break;

	  case(2):
		  errorposition=2031;
		  errpram=true;
		  errormessage="参数名重复或是系统关键字";
		  break;

	  }
  }

  if(m_edit41!=""&&m_edit44!="")
  { 
	  float fpram=StringToFloat(m_edit44);
	  m_edit41.MakeLower();
	  int addpra=equation->AddPara(m_edit41,fpram);
	  switch(addpra)
	  {
	  case(1):
		  errorposition=2041;
		  errpram=true;
		  errormessage="参数个数太多";
		  break;

	  case(2):
		  errorposition=2041;
		  errpram=true;
		  errormessage="参数名重复或是系统关键字";
		  break;

	  }
  }

 if(errpram)
  {
	  errorposition-=2000;     
	  switch(errorposition)
	  {
	  case(11):
		  m_edit11cor.SetFocus();
		  break;
	  case(12):
		  m_edit12cor.SetFocus();
		  break;
	  case(13):
		  m_edit13cor.SetFocus();
		  break;
	  case(14):
		  m_edit14cor.SetFocus();
		  break;
	  case(21):
		  m_edit21cor.SetFocus();
		  break;
      case(22):
		  m_edit22cor.SetFocus();
		  break;
	  case(23):
		  m_edit23cor.SetFocus();
		  break;
	  case(24):
		  m_edit24cor.SetFocus();
		  break;
	  case(31):
		  m_edit31cor.SetFocus();
		  break;
	  case(32):
		  m_edit32cor.SetFocus();
		  break;
	  case(33):
		  m_edit33cor.SetFocus();
		  break;
	  case(34):
		  m_edit34cor.SetFocus();
		  break;
	  case(41):
		  m_edit41cor.SetFocus();
		  break;
	  case(42):
		  m_edit42cor.SetFocus();
		  break;
	  case(43):
		  m_edit43cor.SetFocus();
		  break;
	  case(44):
		  m_edit44cor.SetFocus();
		  break;
	 
	  }
	  InvalidateRect(&datarect);
      UpdateWindow();
	  return;
  }

  equation->Devide();
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
			  int i;
			  for(i=formallength-1;i>=0;i--)
			  {
				  test=formal.GetAt(i);
				  if(test==' '||test==';'||test=='\t'||test=='\n'||test=='\r')
					  continue;
				  else
					  break;
			  }
			  for(int j=i+1;j<formal.GetLength();j++)
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
  m_inputcor.SetFocus();
  m_inputcor.SetSel(errorposition,errorposition);
 
  InvalidateRect(&datarect);
  UpdateWindow();
	
}


void CDIGEDITTJXG::OnOK() 
{
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
    UpdateData();
	CString name=m_name;
	CString input=m_input;
	name.Remove(' ');
	input.Remove(' ');
	bool isnameok=true;
	
	for(int i=0;i<m_name.GetLength();i++)
	{
		char test=m_name.GetAt(i);
		if((test&0x80)!=0)
		{
			i=i+1;
			continue;
		}
		if(!((test>='a'&&test<='z')||test=='_'||test=='%'||test == '(' || test == ')'||(test>='A'&&test<='Z')||(test>='0'&&test<='9')))
		{
			isnameok=false;
			break;
		}
		if(i==0&&test>='0'&&test<='9')
		{
			isnameok=false;
			break;
		}

	}
	if(!isnameok||name=="")
	{
		AfxMessageBox("指标名为空或非法，\n请重新输入",MB_OK);
		m_namecor.SetFocus();
		return;
	}
	
	OnCsgs();

	
	
	if(errorposition!=-1)
		if(AfxMessageBox("您编辑的公式没有通过\n语法检查，系统将把这\n\
个公式存入文件，\n要继续吗？",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
				    return;

	CFormularContent  *jishunew=new CFormularContent();
	jishunow=jishunew;
	if(item!=-1)
		if(iskline)
			pDoc->m_formuar_kline.RemoveAt(item);
		else
			pDoc->m_formuar_choose.RemoveAt(item);
	
	if(iskline)   
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
		
	else         
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
	
		

	CArray<BYTE,BYTE>pramarray;       
	if(m_edit11!=""&&m_edit14!="") pramarray.Add(1);
	if(m_edit21!=""&&m_edit24!="") pramarray.Add(2);
	if(m_edit31!=""&&m_edit34!="") pramarray.Add(3);
	if(m_edit41!=""&&m_edit44!="") pramarray.Add(4);

	BYTE pramnum=(BYTE)pramarray.GetSize();
	jishunow->numPara=pramnum;
	for(int i=0;i<pramnum;i++)
	{
		int line=pramarray.GetAt(i);
		switch(line)
		{
		case(1):
			jishunow->namePara[i]=m_edit11;
			jishunow->max[i]=StringToFloat(m_edit12);
			jishunow->min[i]=StringToFloat(m_edit13);
			jishunow->defaultVal[i]=StringToFloat(m_edit14);
			break;
		case(2):
			jishunow->namePara[i]=m_edit21;
			jishunow->max[i]=StringToFloat(m_edit22);
			jishunow->min[i]=StringToFloat(m_edit23);
			jishunow->defaultVal[i]=StringToFloat(m_edit24);
			break;
		case(3):
			jishunow->namePara[i]=m_edit31;
			jishunow->max[i]=StringToFloat(m_edit32);
			jishunow->min[i]=StringToFloat(m_edit33);
			jishunow->defaultVal[i]=StringToFloat(m_edit34);
			break;
		case(4):
			jishunow->namePara[i]=m_edit41;
			jishunow->max[i]=StringToFloat(m_edit42);
			jishunow->min[i]=StringToFloat(m_edit43);
			jishunow->defaultVal[i]=StringToFloat(m_edit44);
			break;
		}
	}	

	 


    
	 jishunow->fomular=m_input;
	 if((m_checkmm==TRUE))
	 jishunow->password=m_password;
	 jishunow->explainBrief=m_explainbrief;
	 jishunow->name=m_name;
	 jishunow->isProtected=m_checkmm;
	 jishunow->help=m_myhelp;
	CDialog::OnOK();
}

void CDIGEDITTJXG::OnDyhs() 
{
	CDialogEDITZBGSHS m_diazbgshs;
	if(m_diazbgshs.DoModal()==IDOK)
	{
		functionstr=m_diazbgshs.m_selectedstr;
		int length=functionstr.GetLength();
		int i;
		for(i=0;i<length;i++)
		{
			char test=functionstr.GetAt(i);
			if(test=='.'&&functionstr.GetAt(i+1)=='.')
				break;
		}
		if(i==length)
			functionstr="";
		else
			functionstr=functionstr.Left(i);
		m_input=m_input+" "+functionstr;
		UpdateData(FALSE);
		m_inputcor.SetFocus();
		i=m_input.GetLength();
		m_inputcor.SetSel(i,i);
	
	}

	
}

BOOL CDIGEDITTJXG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();

	ScreenToClient(&datarect);  
    datarect.left=235;
	datarect.top=205;
	datarect.right=400;
	datarect.bottom=220;

if(!iskline)                   
{	
	this->SetWindowText("条件选股公式编辑器");
	
	if(item!=-1)
	{
	jishunow=pDoc->m_formuar_choose.GetAt(item);
	m_name=jishunow->name;
	m_myhelp=jishunow->help;

    m_explainbrief=jishunow->explainBrief;
	if(jishunow->isProtected)
	{
		m_checkmm=true;
		m_password=jishunow->password;
	}
	else 
		m_passwordcor.EnableWindow(FALSE);

     numPara=jishunow->numPara;
     m_input=jishunow->fomular;
     m_inputcor.SetFocus();

		switch(numPara)
		{
		case (4):
			m_edit41=jishunow->namePara[3];
			ShowData(m_edit42,jishunow->max[3]);
			ShowData(m_edit43,jishunow->min[3]);
			ShowData(m_edit44,jishunow->defaultVal[3]);
		case(3):
			m_edit31=jishunow->namePara[2];
		    ShowData(m_edit32,jishunow->max[2]);
			ShowData(m_edit33,jishunow->min[2]);
			ShowData(m_edit34,jishunow->defaultVal[2]);
			
		case(2):
			m_edit21=jishunow->namePara[1];
			ShowData(m_edit22,jishunow->max[1]);
			ShowData(m_edit23,jishunow->min[1]);
			ShowData(m_edit24,jishunow->defaultVal[1]);
			
		case(1):
			m_edit11=jishunow->namePara[0];
			ShowData(m_edit12,jishunow->max[0]);
			ShowData(m_edit13,jishunow->min[0]);
			ShowData(m_edit14,jishunow->defaultVal[0]);
		}
		
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
	this->SetWindowText("K线组合公式编辑器");
	
	if(item!=-1)
	{
	jishunow=pDoc->m_formuar_kline.GetAt(item);
	m_name=jishunow->name;

	m_myhelp=jishunow->help;
    m_explainbrief=jishunow->explainBrief;
	if(jishunow->isProtected)
	{
		m_checkmm=true;
		m_password=jishunow->password;
	}
	else 
		m_passwordcor.EnableWindow(FALSE);

    numPara=jishunow->numPara;
    m_input=jishunow->fomular;
	m_inputcor.SetFocus();

		switch(numPara)
		{
		case (4):
			m_edit41=jishunow->namePara[3];
			ShowData(m_edit42,jishunow->max[3]);
			ShowData(m_edit43,jishunow->min[3]);
			ShowData(m_edit44,jishunow->defaultVal[3]);
		case(3):
			m_edit31=jishunow->namePara[2];
			ShowData(m_edit32,jishunow->max[2]);
			ShowData(m_edit33,jishunow->min[2]);
			ShowData(m_edit34,jishunow->defaultVal[2]);
			
		case(2):
			m_edit21=jishunow->namePara[1];
			ShowData(m_edit22,jishunow->max[1]);
			ShowData(m_edit23,jishunow->min[1]);
			ShowData(m_edit24,jishunow->defaultVal[1]);
			
		case(1):
			m_edit11=jishunow->namePara[0];
			ShowData(m_edit12,jishunow->max[0]);
			ShowData(m_edit13,jishunow->min[0]);
			ShowData(m_edit14,jishunow->defaultVal[0]);
		}
		
		UpdateData(FALSE);
	}
	else
	{
	 m_checkmm=false;
	 m_hfqs.EnableWindow(FALSE);	 
	 UpdateData(FALSE);

	}
}

	
	return TRUE;  
}

void CDIGEDITTJXG::ShowData(CString &str, float fdata)
{
	if(fdata==-1000)
		str="";
	else
	{
		str.Format("%.2f",fdata);
		int strlength=str.GetLength();
		if(str.GetAt(strlength-1)=='0'&&str.GetAt(strlength-2)=='0')
			str=str.Left(strlength-3);
		
	}


}

float CDIGEDITTJXG::StringToFloat(CString str)
{
	char * test;
	int length=str.GetLength();
	test=str.GetBuffer(length);
	return (float)atof(test);
}

void CDIGEDITTJXG::OnPaint() 
{
	CPaintDC dc(this); 
	CString   message;
	COLORREF   oldcolor;
	CFont  font;        
	CFont *polddefault; 
	LOGFONT m_fontdefault;
	memset(&m_fontdefault, 0, sizeof m_fontdefault);
	m_fontdefault.lfHeight = 15;
	lstrcpy(m_fontdefault.lfFaceName, _T("宋体"));
	m_fontdefault.lfOutPrecision = OUT_TT_PRECIS;
	m_fontdefault.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_fontdefault.lfQuality = PROOF_QUALITY;
	m_fontdefault.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;


    font.CreateFontIndirect(&m_fontdefault);
    polddefault=dc.SelectObject(&font);

	dc.SetBkMode(TRANSPARENT);
	if(errorposition==-1)
    {
		message="测试通过!";
		oldcolor=dc.SetTextColor(0xff0000);
		dc.TextOut(datarect.left,datarect.top,message);
	}

	else
	{
		message=errormessage;
		oldcolor=dc.SetTextColor(0x0000ff);
		dc.TextOut(datarect.left,datarect.top,message);
	}
	
    dc.SelectObject(polddefault);
	
}

void CDIGEDITTJXG::OnCheckmm() 
{
	if(IsDlgButtonChecked(IDC_CHECKMM))
	   m_passwordcor.EnableWindow(TRUE);
   else
	   m_passwordcor.EnableWindow(FALSE);
	   

	
}

BOOL CDIGEDITTJXG::CheckParmInput(CString parminput)
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

BOOL CDIGEDITTJXG::CheckParmNameInput(CString parname, CString &errmessage)
{
	parname.Remove(' ');
	char test=parname.GetAt(0);
	if(test<'A'||test>'z'||test=='_')
	{
		errormessage="参数名必须以字母开头";
		return FALSE;
	}
	int length=parname.GetLength();
	for(int i=1;i<length;i++)
	{
		test=parname.GetAt(i);
		if((test>='0'&&test<='9')||(test>='A'&&test<='z')||test=='_')
			continue;
		else
		{
			errormessage="参数名组成非法";
			return FALSE;
		}
	}
	return TRUE;

}

void CDIGEDITTJXG::OnHfqs() 
{

	OnInitDialog();
	
}


