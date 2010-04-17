// DIAEDITZJXT.cpp : implementation file
//

#include "stdafx.h"
#include "DIAEDITZJXT.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
#include "DIAEDITZBGSHS.h"
#include "DIALOGGSSM.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDialogEDITZJXT::CDialogEDITZJXT(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEDITZJXT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEDITZJXT)
	m_buyinput = _T("");
	m_checkmm = FALSE;
	m_edit11 = _T("");
	m_edit12 = _T("");
	m_edit13 = _T("");
	m_edit14 = _T("");
	m_edit15 = _T("");
	m_edit21 = _T("");
	m_edit22 = _T("");
	m_edit23 = _T("");
	m_edit24 = _T("");
	m_edit25 = _T("");
	m_edit31 = _T("");
	m_edit32 = _T("");
	m_edit33 = _T("");
	m_edit34 = _T("");
	m_edit35 = _T("");
	m_edit41 = _T("");
	m_edit42 = _T("");
	m_edit43 = _T("");
	m_edit44 = _T("");
	m_edit45 = _T("");
	m_explainbrief = _T("");
	m_input = _T("");
	m_name = _T("");
	m_password = _T("");
	m_sellinput = _T("");
	//}}AFX_DATA_INIT
}


void CDialogEDITZJXT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEDITZJXT)
	DDX_Control(pDX, IDC_NAME, m_namecor);
	DDX_Control(pDX, IDC_HFQS, m_hfqscor);
	DDX_Control(pDX, IDC_SELLINPUT, m_sellinputcor);
	DDX_Control(pDX, IDC_PASSWORD, m_passwordcor);
	DDX_Control(pDX, IDC_INPUT, m_inputcor);
	DDX_Control(pDX, IDC_EDIT45, m_edit45cor);
	DDX_Control(pDX, IDC_EDIT44, m_edit44cor);
	DDX_Control(pDX, IDC_EDIT43, m_edit43cor);
	DDX_Control(pDX, IDC_EDIT42, m_edit42cor);
	DDX_Control(pDX, IDC_EDIT41, m_edit41cor);
	DDX_Control(pDX, IDC_EDIT35, m_edit35cor);
	DDX_Control(pDX, IDC_EDIT34, m_edit34cor);
	DDX_Control(pDX, IDC_EDIT33, m_edit33cor);
	DDX_Control(pDX, IDC_EDIT32, m_edit32cor);
	DDX_Control(pDX, IDC_EDIT31, m_edit31cor);
	DDX_Control(pDX, IDC_EDIT25, m_edit25cor);
	DDX_Control(pDX, IDC_EDIT24, m_edit24cor);
	DDX_Control(pDX, IDC_EDIT23, m_edit23cor);
	DDX_Control(pDX, IDC_EDIT22, m_edit22cor);
	DDX_Control(pDX, IDC_EDIT21, m_edit21cor);
	DDX_Control(pDX, IDC_EDIT15, m_edit15cor);
	DDX_Control(pDX, IDC_EDIT14, m_edit14cor);
	DDX_Control(pDX, IDC_EDIT13, m_edit13cor);
	DDX_Control(pDX, IDC_EDIT12, m_edit12cor);
	DDX_Control(pDX, IDC_EDIT11, m_edit11cor);
	DDX_Control(pDX, IDC_BUYINPUT, m_buyinputcor);
	DDX_Text(pDX, IDC_BUYINPUT, m_buyinput);
	DDX_Check(pDX, IDC_CHECKMM, m_checkmm);
	DDX_Text(pDX, IDC_EDIT11, m_edit11);
	DDX_Text(pDX, IDC_EDIT12, m_edit12);
	DDX_Text(pDX, IDC_EDIT13, m_edit13);
	DDX_Text(pDX, IDC_EDIT14, m_edit14);
	DDX_Text(pDX, IDC_EDIT15, m_edit15);
	DDX_Text(pDX, IDC_EDIT21, m_edit21);
	DDX_Text(pDX, IDC_EDIT22, m_edit22);
	DDX_Text(pDX, IDC_EDIT23, m_edit23);
	DDX_Text(pDX, IDC_EDIT24, m_edit24);
	DDX_Text(pDX, IDC_EDIT25, m_edit25);
	DDX_Text(pDX, IDC_EDIT31, m_edit31);
	DDX_Text(pDX, IDC_EDIT32, m_edit32);
	DDX_Text(pDX, IDC_EDIT33, m_edit33);
	DDX_Text(pDX, IDC_EDIT34, m_edit34);
	DDX_Text(pDX, IDC_EDIT35, m_edit35);
	DDX_Text(pDX, IDC_EDIT41, m_edit41);
	DDX_Text(pDX, IDC_EDIT42, m_edit42);
	DDX_Text(pDX, IDC_EDIT43, m_edit43);
	DDX_Text(pDX, IDC_EDIT44, m_edit44);
	DDX_Text(pDX, IDC_EDIT45, m_edit45);
	DDX_Text(pDX, IDC_EXPLAINBRIEF, m_explainbrief);
	DDX_Text(pDX, IDC_INPUT, m_input);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_SELLINPUT, m_sellinput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEDITZJXT, CDialog)
	//{{AFX_MSG_MAP(CDialogEDITZJXT)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECKMM, OnCheckmm)
	ON_BN_CLICKED(IDC_CSGS, OnCsgs)
	ON_BN_CLICKED(IDC_DYHS, OnDyhs)
	ON_BN_CLICKED(IDC_HFQS, OnHfqs)
	ON_BN_CLICKED(IDC_MYHELP, OnMyhelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDialogEDITZJXT::OnPaint() 
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
    {   message="测试通过!";
	oldcolor=dc.SetTextColor(0x00ff00);
	dc.TextOut(datarect.left,datarect.top,message);
	}

	else
	{
		message=errormessage;
		oldcolor=dc.SetTextColor(0x0000ff);
		dc.TextOut(datarect.left,datarect.top,message);
	}
	
}

void CDialogEDITZJXT::OnCheckmm() 
{

	 if(IsDlgButtonChecked(IDC_CHECKMM))
	   m_passwordcor.EnableWindow(TRUE);
   else
	   m_passwordcor.EnableWindow(FALSE);
	   
	
}

void CDialogEDITZJXT::OnCsgs() 
{
  Kline *kline;
  bool errpram=false;
  UpdateData();
  kline = NULL;
  inputpre=m_input;

  int j;
for(j=0;j<1;j++)
{
 if(m_edit11==""&&(m_edit14!=""||m_edit15!=""))
  {
	  errorposition=2011;
	  errormessage="参数名不能空";
	  errpram=true;
	  break;
  }
  if(m_edit11!=""&&m_edit14=="")
  {
	  errorposition=2014;
	  errormessage="参数的缺省值不能空";
	  errpram=true;
	  break;
  }
  if(m_edit11!=""&&m_edit15=="")
  {
	  errorposition=2015;
	  errormessage="参数的测试步长不能空";
	  errpram=true;
	  break;
  }
  if(m_edit21==""&&(m_edit24!=""||m_edit25!=""))
  {
	  errorposition=2021;
	  errormessage="参数名不能空";
	  errpram=true;
	  break;
  }
  if(m_edit21!=""&&m_edit24=="")
  {
	  errorposition=2024;
	  errormessage="参数的缺省值不能空";
	  errpram=true;
	  break;
  }
  if(m_edit21!=""&&m_edit25=="")
  {
	  errorposition=2025;
	  errormessage="参数的测试步长不能空";
	  errpram=true;
	  break;
  }

  if(m_edit31==""&&(m_edit34!=""||m_edit35!=""))
  {
	  errorposition=2031;
	  errormessage="参数名不能空";
	  errpram=true;
	  break;
  }
  if(m_edit31!=""&&m_edit34=="")
  {
	  errorposition=2034;
	  errormessage="参数的缺省值不能空";
	  errpram=true;
	  break;
  }
  if(m_edit31!=""&&m_edit35=="")
  {
	  errorposition=2035;
	  errormessage="参数的测试步长不能空";
	  errpram=true;
	  break;
  }
  if(m_edit41==""&&(m_edit44!=""||m_edit45!=""))
  {
	  errorposition=2041;
	  errormessage="参数名不能空";
	  errpram=true;
	  break;
  }
  if(m_edit41!=""&&m_edit44=="")
  {
	  errorposition=2044;
	  errormessage="参数的缺省值不能空";
	  errpram=true;
	  break;
  }
  if(m_edit41!=""&&m_edit45=="")
  {
	  errorposition=2045;
	  errormessage="参数的测试步长不能空";
	  errpram=true;
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
	  case(14):
		  m_edit14cor.SetFocus();
		  break;
	  case(15):
		  m_edit15cor.SetFocus();
		  break;
	  case(21):
		  m_edit11cor.SetFocus();
		  break;
	  case(24):
		  m_edit14cor.SetFocus();
		  break;
	  case(25):
		  m_edit25cor.SetFocus();
		  break;
	  case(31):
		  m_edit11cor.SetFocus();
		  break;
	  case(34):
		  m_edit14cor.SetFocus();
		  break;
	  case(35):
		  m_edit35cor.SetFocus();
		  break;
	  case(41):
		  m_edit11cor.SetFocus();
		  break;
	  case(44):
		  m_edit14cor.SetFocus();
		  break;
	  case(45):
		  m_edit45cor.SetFocus();
		  break;
	   }
	  InvalidateRect(&datarect);
      UpdateWindow();
	  return;
  }

  j=0;
  
  for(int i=0;i<m_input.GetLength();i++)  
 {  
	  
	  if(m_input.GetAt(i)==13&&m_input.GetAt(i+1)==10)  
	  { 
		 m_input.SetAt(i,' ');
		 m_input.SetAt(i+1,' ');
	  }
  }

  AfxMessageBox(_T("完成中"));
  /*
  int inputlength=m_input.GetLength();
  if(m_input.GetAt(inputlength-1)!=';')
	  m_input+=';';
  CString buyinput=m_buyinput.Remove(' ');
  CString sellinput=m_sellinput.Remove(' ');
  if(buyinput!="")
  m_input=m_input+m_buyinput+';';
  if(sellinput!="")
  m_input+=m_sellinput;
  m_input.MakeLower();
  CFormularCompute *equation=new CFormularCompute(0,0,kline,m_input);
//------------------------
  if(m_edit11!=""&&m_edit14!=""&&m_edit15!="")
  {  
	  float fpram=StringToFloat(m_edit14);
	  m_edit11.MakeLower();
	  equation->AddPara(m_edit11,fpram);
  
  }
   if(m_edit21!=""&&m_edit24!=""&&m_edit25!="")
   { 
	  float fpram=StringToFloat(m_edit24);
	  m_edit21.MakeLower();
	  equation->AddPara(m_edit21,fpram);
   }

  if(m_edit31!=""&&m_edit34!=""&&m_edit35!="") 
  { 
	  float fpram=StringToFloat(m_edit34);
	  m_edit31.MakeLower();
	  equation->AddPara(m_edit31,fpram);
  }

  if(m_edit41!=""&&m_edit44!=""&&m_edit45!="")
  { 
	  float fpram=StringToFloat(m_edit44);
	  m_edit41.MakeLower();
	  equation->AddPara(m_edit41,fpram);
  }
//------------------
  equation->Devide();
  errorposition=equation->m_errpos;
  errormessage=equation->m_errmsg;
  if(errorposition>inputlength)
  {
	  int buylength=m_buyinput.GetLength();
	  if(errorposition>inputlength+buylength)
	  {
		  errorposition=errorposition-inputlength-buylength;
		  m_sellinputcor.SetFocus();
		  m_sellinputcor.SetSel(errorposition,errorposition);
	  }
	  else
	  {
		  errorposition=errorposition-inputlength;
		  m_buyinputcor.SetFocus();
		  m_buyinputcor.SetSel(errorposition,errorposition);
	  }
  }
  else
  {
  m_inputcor.SetFocus();
  m_inputcor.SetSel(errorposition,errorposition);
  }
  m_input=inputpre;
  InvalidateRect(&datarect);
  UpdateWindow();
	//*/
}

void CDialogEDITZJXT::OnDyhs()   
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

void CDialogEDITZJXT::OnOK() 
{
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
    UpdateData();
	if(item==-1)
	{
		CString name=m_name;
		CString input=m_input;
		CString buyinput=m_buyinput;
		CString sellinput=m_sellinput;
		name.Remove(' ');
		input.Remove(' ');
		buyinput.Remove(' ');
		sellinput.Remove(' ');
		bool isnameok=true;
		for(int i=0;i<m_name.GetLength();i++)
		{
			char test=m_name.GetAt(i);
			if(!((test>='a'&&test<='z')||test=='_'||(test>='A'&&test<='Z')||(test>='0'&&test<='9')))
			{
				isnameok=false;
				break;
			}

		}
		if(!isnameok||name=="")
		{
			AfxMessageBox("",MB_OK);
			m_namecor.SetFocus();
			return;
		}
			if(input=="")
		{
			AfxMessageBox("专家系统公式内容不能为空\n请输入",MB_OK);
			m_inputcor.SetFocus();
			return;
		}
			if(buyinput=="")
		{
			AfxMessageBox("买入条件公式内容不能为空\n请输入",MB_OK);
			m_buyinputcor.SetFocus();
			return;
		}
			if(sellinput=="")
		{
			AfxMessageBox("卖出条件公式内容不能为空\n请输入",MB_OK);
			m_sellinputcor.SetFocus();
			return;
		}

	
	}
	OnCsgs();
	
//----------------		
	if(errorposition!=-1)
		if(AfxMessageBox("您编辑的公式没有通过\n语法检查，系统将把这\n\
个公式存入文件，\n要继续吗？",MB_OKCANCEL|MB_ICONQUESTION)==IDCANCEL)
				    return;

	if(item==-1)
	{
		CFormularContent  *jishunew=new CFormularContent();
		pDoc->m_formuar_master.Add(jishunew);
		jishunow=jishunew;
				
		
	}
//-------------
	CArray<BYTE,BYTE>pramarray;       
	if(m_edit11!=""&&m_edit14!=""&&m_edit15!="") pramarray.Add(1);
	if(m_edit21!=""&&m_edit24!=""&&m_edit25!="") pramarray.Add(2);
	if(m_edit31!=""&&m_edit34!=""&&m_edit35!="") pramarray.Add(3);
	if(m_edit41!=""&&m_edit44!=""&&m_edit45!="") pramarray.Add(4);

	int pramnum=pramarray.GetSize();
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
			jishunow->stepLen[i]=StringToFloat(m_edit15);
			break;
		case(2):
			jishunow->namePara[i]=m_edit21;
			jishunow->max[i]=StringToFloat(m_edit22);
			jishunow->min[i]=StringToFloat(m_edit23);
			jishunow->defaultVal[i]=StringToFloat(m_edit24);
			jishunow->stepLen[i]=StringToFloat(m_edit25);
			break;
		case(3):
			jishunow->namePara[i]=m_edit31;
			jishunow->max[i]=StringToFloat(m_edit32);
			jishunow->min[i]=StringToFloat(m_edit33);
			jishunow->defaultVal[i]=StringToFloat(m_edit34);
			jishunow->stepLen[i]=StringToFloat(m_edit35);
			break;
		case(4):
			jishunow->namePara[i]=m_edit41;
			jishunow->max[i]=StringToFloat(m_edit42);
			jishunow->min[i]=StringToFloat(m_edit43);
			jishunow->defaultVal[i]=StringToFloat(m_edit44);
			jishunow->stepLen[i]=StringToFloat(m_edit45);
			break;
		}
	}	

	 

//-------------------
    
	 jishunow->fomular=m_input;
	 if((m_checkmm==1))
		 jishunow->isProtected=true;
	 jishunow->password=m_password;
	 jishunow->explainBrief=m_explainbrief;
	 jishunow->buyStr=m_buyinput;
	 jishunow->sellStr=m_sellinput;
	 jishunow->name=m_name;
	 jishunow->isProtected=m_checkmm;	

	CDialog::OnOK();
}

BOOL CDialogEDITZJXT::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	

	ScreenToClient(&datarect);
    datarect.left=250;
	datarect.top=50;
	datarect.right=380;
	datarect.bottom=80;
	
	if(item!=-1)
	{
	jishunow=pDoc->m_formuar_master.GetAt(item);
	m_name=jishunow->name;
	m_namecor.SetReadOnly();
	m_explainbrief=jishunow->explainBrief;
	if(jishunow->isProtected)
	{
		m_checkmm=true;
		m_password=jishunow->password;
	}
	else 
		m_passwordcor.EnableWindow(FALSE);

     numPara=jishunow->numPara;
	 m_buyinput=jishunow->buyStr;
	 m_sellinput=jishunow->sellStr;
     m_input=jishunow->fomular;
     m_inputcor.SetFocus();

		switch(numPara)
		{
		case (4):
			m_edit41=jishunow->namePara[3];
			ShowData(m_edit42,jishunow->max[3]);
			ShowData(m_edit43,jishunow->min[3]);
			ShowData(m_edit44,jishunow->defaultVal[3]);
			ShowData(m_edit45,jishunow->stepLen[3]);
		case(3):
			m_edit31=jishunow->namePara[2];
		    ShowData(m_edit32,jishunow->max[2]);
			ShowData(m_edit33,jishunow->min[2]);
			ShowData(m_edit34,jishunow->defaultVal[2]);
			ShowData(m_edit35,jishunow->stepLen[2]);
			
		case(2):
			m_edit21=jishunow->namePara[1];
			ShowData(m_edit22,jishunow->max[1]);
			ShowData(m_edit23,jishunow->min[1]);
			ShowData(m_edit24,jishunow->defaultVal[1]);
			ShowData(m_edit25,jishunow->stepLen[1]);
		case(1):
			m_edit11=jishunow->namePara[0];
			ShowData(m_edit12,jishunow->max[0]);
			ShowData(m_edit13,jishunow->min[0]);
			ShowData(m_edit14,jishunow->defaultVal[0]);
			ShowData(m_edit15,jishunow->stepLen[0]);
		}
		
		UpdateData(FALSE);
	}
	else
	{
	 m_checkmm=false;
	 m_hfqscor.EnableWindow(FALSE);	 
	 UpdateData(FALSE);

	}
	

	
	return TRUE; 
}

void CDialogEDITZJXT::ShowData(CString &str, float fdata)
{
		if(fdata<=-1000) 
		str="";
	else
		str.Format("%.2f",fdata);


}

float CDialogEDITZJXT::StringToFloat(CString str)
{
	char * test;
	int length=str.GetLength();
	test=str.GetBuffer(length);
	return float(atof(test));

}

void CDialogEDITZJXT::OnHfqs() 
{
	OnInitDialog();
	
}

void CDialogEDITZJXT::OnMyhelp() 
{
	
		CDialogLOGGSSM mdiagssm;
		mdiagssm.m_edit=jishunow->help;
	if(mdiagssm.DoModal()==IDOK)
		jishunow->help=mdiagssm.m_edit;
	

	
}
