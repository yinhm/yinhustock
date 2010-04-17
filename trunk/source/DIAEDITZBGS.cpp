// DIAEDITZBGS.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DIAEDITZBGS.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
#include "DIAEDITZBGSHS.h"
#include "DIALOGGSSM.h"
#include "CFormularContent.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEDITZBGS dialog


CDialogEDITZBGS::CDialogEDITZBGS(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEDITZBGS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEDITZBGS)
	m_name = _T("");
	m_password = _T("");
	m_input = _T("");
	m_edit11 = _T("");
	m_edit21 = _T("");
	m_edit31 = _T("");
	m_edit41 = _T("");
	m_edit12 = _T("");
	m_edit13 = _T("");
	m_edit14 = _T("");
	m_edit22 = _T("");
	m_edit23 = _T("");
	m_edit24 = _T("");
	m_edit32 = _T("");
	m_edit33 = _T("");
	m_edit34 = _T("");
	m_edit42 = _T("");
	m_edit43 = _T("");
	m_edit44 = _T("");
	m_xline = _T("");
	m_explainbrief = _T("");
	m_checkmm = FALSE;
	//}}AFX_DATA_INIT

}


void CDialogEDITZBGS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEDITZBGS)
	DDX_Control(pDX, IDC_NAME, m_namecor);
	DDX_Control(pDX, IDC_HFQS, m_hfqs);
	DDX_Control(pDX, IDC_ZTDJ, m_ztdj);
	DDX_Control(pDX, IDC_EDIT41, m_edit41cor);
	DDX_Control(pDX, IDC_EDIT31, m_edit31cor);
	DDX_Control(pDX, IDC_EDIT21, m_edit21cor);
	DDX_Control(pDX, IDC_EDIT11, m_edit11cor);
	DDX_Control(pDX, IDC_INPUT, m_inputcor);
	DDX_Control(pDX, IDC_EDIT24, m_edit24cor);
	DDX_Control(pDX, IDC_EDIT44, m_edit44cor);
	DDX_Control(pDX, IDC_EDIT43, m_edit43cor);
	DDX_Control(pDX, IDC_EDIT42, m_edit42cor);
	DDX_Control(pDX, IDC_EDIT34, m_edit34cor);
	DDX_Control(pDX, IDC_EDIT33, m_edit33cor);
	DDX_Control(pDX, IDC_EDIT32, m_edit32cor);
	DDX_Control(pDX, IDC_EDIT23, m_edit23cor);
	DDX_Control(pDX, IDC_EDIT22, m_edit22cor);
	DDX_Control(pDX, IDC_EDIT14, m_edit14cor);
	DDX_Control(pDX, IDC_EDIT13, m_edit13cor);
	DDX_Control(pDX, IDC_EDIT12, m_edit12cor);
	DDX_Control(pDX, IDC_EDITPASSWORD, m_passwordcor);
	DDX_Control(pDX, IDC_XLINE, m_xlinecor);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 6);
	DDX_Text(pDX, IDC_EDITPASSWORD, m_password);
	DDX_Text(pDX, IDC_INPUT, m_input);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDV_MaxChars(pDX, m_edit11, 6);
	DDX_Text(pDX, IDC_EDIT21, m_edit21);
	DDV_MaxChars(pDX, m_edit21, 6);
	DDX_Text(pDX, IDC_EDIT31, m_edit31);
	DDV_MaxChars(pDX, m_edit31, 6);
	DDX_Text(pDX, IDC_EDIT41, m_edit41);
	DDV_MaxChars(pDX, m_edit41, 6);
	DDX_Text(pDX, IDC_EDIT12, m_edit12);
	DDX_Text(pDX, IDC_EDIT13, m_edit13);
	DDX_Text(pDX, IDC_EDIT14, m_edit14);
	DDX_Text(pDX, IDC_EDIT22, m_edit22);
	DDX_Text(pDX, IDC_EDIT23, m_edit23);
	DDX_Text(pDX, IDC_EDIT24, m_edit24);
	DDX_Text(pDX, IDC_EDIT32, m_edit32);
	DDX_Text(pDX, IDC_EDIT33, m_edit33);
	DDX_Text(pDX, IDC_EDIT34, m_edit34);
	DDX_Text(pDX, IDC_EDIT42, m_edit42);
	DDX_Text(pDX, IDC_EDIT43, m_edit43);
	DDX_Text(pDX, IDC_EDIT44, m_edit44);
	DDX_Text(pDX, IDC_XLINE, m_xline);
	DDX_Text(pDX, IDC_EXPLAINBRIEF, m_explainbrief);
	DDX_Check(pDX, IDC_CHECKMM, m_checkmm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEDITZBGS, CDialog)
	//{{AFX_MSG_MAP(CDialogEDITZBGS)
	ON_BN_CLICKED(IDC_CHECKMM, OnCheckmm)
	ON_BN_CLICKED(IDC_CSGS, OnCsgs)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DYHS, OnDyhs)
	ON_BN_CLICKED(IDC_HFQS, OnHfqs)
	ON_BN_CLICKED(IDC_MYHELP, OnMyhelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEDITZBGS message handlers

BOOL CDialogEDITZBGS::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();

	ScreenToClient(&datarect);  
    datarect.left=235;
	datarect.top=205;
	datarect.right=400;
	datarect.bottom=220;

	if(item!=-1)            
	{
		jishunow=pDoc->m_formuar_index.GetAt(item);
		m_name=jishunow->name;

		if(pDoc->m_formuar_index.GetAt(item)->isMainFiguer==1)
			 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_ZTDJ);
		else 
			 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_FT);
		CString strxline;
		for(int k=0;k<7;k++)
		{
			if(jishunow->posX[k]>=-1.0e24)
			{
				CString test;
				test.Format("%.2f",jishunow->posX[k]);
				strxline+=test;
				if((k+1)%2==0)
				{
					strxline+=13;
					strxline+=10;
				}
					
				else
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



		numPara=jishunow->numPara;
		help=jishunow->help;
		m_input=pDoc->m_formuar_index.GetAt(item)->fomular;
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
		help="";
		 CheckRadioButton(IDC_ZTDJ,IDC_FT,IDC_FT);
		 m_checkmm=false;
		 m_hfqs.EnableWindow(FALSE);	 
		 UpdateData(FALSE);

	}

	return TRUE;  
}

void CDialogEDITZBGS::OnCheckmm() 
{

   if(IsDlgButtonChecked(IDC_CHECKMM))
	   m_passwordcor.EnableWindow(TRUE);
   else
	   m_passwordcor.EnableWindow(FALSE);
	   
	
}

CString CDialogEDITZBGS::FloatToString(float input)
{
	CString result;
	result.Format("%.2f",input);
	return result;
}

void CDialogEDITZBGS::OnOK() 
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
		if(i==0&&(!(test>='A'&&test<='z')))
		{
			isnameok=false;
			break;
		}
		
		if((test&0x80)!=0)
		{
			i=i+1;
			continue;
		}
		if(!((test>='a'&&test<='z')||test=='_'||test=='%'||(test>='A'&&test<='Z')||(test>='0'&&test<='9')))
		{
			isnameok=false;
			break;
		}
		

	}
	if(!isnameok||name=="")
	{
		AfxMessageBox("指标为空或非法\n请重新输入！",MB_OK);
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
	jishunow->isSystem=false;
	if(item!=-1)
	{
		CString namepre=pDoc->m_formuar_index.GetAt(item)->name;
		int keylength=pDoc->m_keychardata.GetSize();
		for( int j=0;j<keylength;j++)
		{
				if(strcmp(pDoc->m_keychardata.GetAt(j)->key,namepre)==0)
					break;
		}
		pDoc->m_keychardata.RemoveAt(j);

		pDoc->m_formuar_index.RemoveAt(item);
	}
	
		
	int length=pDoc->m_formuar_index.GetSize();
	if(length>0)
	{
		for(int k=0;k<length;k++)
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
				if(item!=-1)
				{
					int keylength=pDoc->m_keychardata.GetSize();
					char *namechar=name.GetBuffer(name.GetLength());
					DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
					strcpy(keytoadd->key,namechar);
					if(m_name.GetLength()<=4)
						name+="指标";
					else
						name+="线";
					strcpy(keytoadd->data,name.GetBuffer(name.GetLength()));
					keytoadd->id=3;

					for(int  j=0;j<keylength;j++)
					{
						if(strcmp(pDoc->m_keychardata.GetAt(j)->key,m_name)>=0)
							break;
					}
					pDoc->m_keychardata.InsertAt(j,keytoadd);
				}
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
	
		
	

	CArray<BYTE,BYTE>pramarray;       
	if(m_edit11!=""&&m_edit14!="") pramarray.Add(1);
	if(m_edit21!=""&&m_edit24!="") pramarray.Add(2);
	if(m_edit31!=""&&m_edit34!="") pramarray.Add(3);
	if(m_edit41!=""&&m_edit44!="") pramarray.Add(4);

	BYTE pramnum=(BYTE)pramarray.GetSize();
	jishunow->numPara=pramnum;
	for(i=0;i<pramnum;i++)
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

	 

		for(int k=0;k<7;k++)
	{
		jishunow->posX[k]=-1.0e25f;
	} 
	CString xpos;

	int j=0,strstart=0;
	for( i=0;i<m_xline.GetLength();i++)
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
			 jishunow->posX[j]=float( atof(xpos) );
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

	  }
	  if(isreturn)
		  i++;

  }

    int itemid;
	itemid=GetCheckedRadioButton(IDC_ZTDJ,IDC_FT);
	if(itemid==IDC_ZTDJ)
		jishunow->isMainFiguer=true;
	else
		jishunow->isMainFiguer=false;


	 jishunow->fomular=m_input;
	 if(m_checkmm==TRUE)
	 jishunow->password=m_password;
	 jishunow->explainBrief=m_explainbrief;
	 jishunow->name=m_name;
	 jishunow->isProtected=m_checkmm;
	 jishunow->help=help;
		
	CDialog::OnOK();
}

void CDialogEDITZBGS::OnCsgs() 
{
  Kline *kline=NULL;
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
	  
	  if(!(isdot||isreturn||isfinish||isdot||(test>='0'&&test<='9')))
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
	  default:
		  m_xlinecor.SetFocus();
		  m_xlinecor.SetSel(errorposition-1000,errorposition-1000);
	  }
	  InvalidateRect(&datarect);
      UpdateWindow();
	  return;
  }

 

  equation->Devide();
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
		  errormessage="公式最多只能画18线";
	  }


  }
  m_inputcor.SetFocus();
  m_inputcor.SetSel(errorposition,errorposition);


  InvalidateRect(&datarect);
  UpdateWindow();
  delete equation;
	
}

void CDialogEDITZBGS::OnPaint() 
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

void CDialogEDITZBGS::OnDyhs()   
{
	UpdateData();
	CDialogEDITZBGSHS m_diazbgshs;
	if(m_diazbgshs.DoModal()==IDOK)
	{
		functionstr=m_diazbgshs.m_selectedstr;
		int length=functionstr.GetLength();
		for(int i=0;i<length;i++)  
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

void CDialogEDITZBGS::OnHfqs()   
{
	OnInitDialog();
	
}

//////////////////////////////////////////////////////////////////////|

float CDialogEDITZBGS::StringToFloat(CString str)
{ 
	char * test;
	int length=str.GetLength();
	test=str.GetBuffer(length);
	return (float)atof(test);

}
//////////////////////////////////////////////////////////////////////|

void CDialogEDITZBGS::ShowData(CString &str, float fdata)
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

void CDialogEDITZBGS::OnMyhelp() 
{

	CDialogLOGGSSM mdiagssm;
	mdiagssm.m_edit=help;
	if(mdiagssm.DoModal()==IDOK)
		help=mdiagssm.m_edit;
	

	
}

BOOL CDialogEDITZBGS::CheckParmInput(CString parminput)
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
BOOL CDialogEDITZBGS::CheckParmNameInput(CString parname, CString &errmessage)
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
