// TechDlgNeuralLearn.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgNeuralLearn.h"
#include "DIALOGXZZB.h"
#include "CTaiScreenParent.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
//#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineFileHS.h"
#include "SelectStock.h"
#include "KeyBrodEangel.h"
#include "CTaiKlineDlgNNetEditData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgNeuralLearn dialog
static bool g_bExec = false;
CTaiKlineDlgNeuralLearn::CTaiKlineDlgNeuralLearn(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgNeuralLearn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgNeuralLearn)
	m_sStockData = _T("");
	m_nDays = 1;
	m_tmBegin = 0;
	m_tmEnd = 0;
	m_sOutput = _T("");
	m_sInput = _T("");
	m_bPreDealData = TRUE;
	m_nMaxCount = 100;
	m_fDiff = 0.00001;
	m_nReal = 0;
	m_nTimesTrain = 3000;
	m_sFileName = _T("weightSave.wgt");
	m_sTrainInfo = _T("请选择输入、输出指标，加入股票数据。");
	m_nDaysManyDay = 50;
	m_nDaysDeleted = 0;
	//}}AFX_DATA_INIT

	m_maxNeededOut = 0 ;
	m_maxNeeded = 0 ;
	m_bPrepared = false;
	this->m_nDaysManyDay = CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfManyKline = m_NnetBP.m_nDaysManyDay;


}


void CTaiKlineDlgNeuralLearn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgNeuralLearn)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_BUTTON6, m_btn6);
	DDX_Control(pDX, IDC_BUTTON5, m_btn5);
	DDX_Control(pDX, IDC_BUTTON4, m_btn4);
	DDX_Control(pDX, IDC_BUTTON3, m_btn3);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_ADD_STOCK, m_add);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_conTmEnd);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_conTmBegin);
	DDX_Control(pDX, IDC_EDIT17, m_floatEdit2);
	DDX_Control(pDX, IDC_EDIT1, m_floatEdit1);
	DDX_Control(pDX, IDC_EDIT4, m_floatEditDiff);
	DDX_Control(pDX, IDC_EDIT5, m_floatEditMaxCount);
	DDX_Control(pDX, IDC_EDIT6, m_floatEditReal);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_EDIT8, m_conDays);
	DDX_Control(pDX, IDC_COMBO1, m_conPeriod);
	DDX_Control(pDX, IDC_LIST1, m_conStockData);
	DDX_LBString(pDX, IDC_LIST1, m_sStockData);
	DDX_Text(pDX, IDC_EDIT1, m_nDays);
	DDV_MinMaxInt(pDX, m_nDays, 0, 10);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tmBegin);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_tmEnd);
	DDX_Text(pDX, IDC_EDIT3, m_sOutput);
	DDX_Text(pDX, IDC_EDIT2, m_sInput);
	DDX_Check(pDX, IDC_CHECK1, m_bPreDealData);
	DDX_Text(pDX, IDC_EDIT5, m_nMaxCount);
	DDV_MinMaxInt(pDX, m_nMaxCount, 1, 10000000);
	DDX_Text(pDX, IDC_EDIT4, m_fDiff);
	DDV_MinMaxFloat(pDX, m_fDiff, 0.0000001, 100000);
	DDX_Text(pDX, IDC_EDIT6, m_nReal);
	DDX_Text(pDX, IDC_EDIT17, m_nTimesTrain);
	DDV_MinMaxInt(pDX, m_nTimesTrain, 1, 100000000);
	DDX_Text(pDX, IDC_EDIT7, m_sFileName);
	DDV_MaxChars(pDX, m_sFileName, 30);
	DDX_Text(pDX, IDC_EDIT9, m_sTrainInfo);
	DDX_Text(pDX, IDC_EDIT8, m_nDaysManyDay);
	DDX_Text(pDX, IDC_EDIT10, m_nDaysDeleted);
	DDV_MinMaxInt(pDX, m_nDaysDeleted, 0, 1000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgNeuralLearn, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgNeuralLearn)
	ON_BN_CLICKED(IDOK, OnTrain)
	ON_BN_CLICKED(IDC_BUTTON2, OnRestoreWeight)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonSelectInput)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonSelectOutput)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonDeleteStock)
	ON_BN_CLICKED(IDC_ADD_STOCK, OnAddStock)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonEditData)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonPrepareData)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeComboPeriod)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEditPreDays)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEditIn)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEditOut)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,GetStockSymbolList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgNeuralLearn message handlers

void CTaiKlineDlgNeuralLearn::OnTrain() 
{
	
	if(	m_bPrepared == false)
	{
		AfxMessageBox("请先按“预处理数据”按钮，执行预处理数据。");
		return;
	}
	if(m_nReal<=9)
	{
		AfxMessageBox("训练用的数据太少，不能进行训练！");
		return;
	}

	CString sTitle;
	GetDlgItem(IDOK)->GetWindowText(sTitle);
	if(sTitle == "中止训练")
	{
		GetDlgItem(IDOK)->SetWindowText("开始训练") ;

		SetEnable(TRUE);
		g_bExec = false;
		return;
	}
	if(!UpdateData())
		return;

	STOCK_DATA_SELECTED stockData;
	int n ;
	if(m_nReal<=0)
	{
		g_bExec = false;
		GetDlgItem(IDOK)->SetWindowText("开始训练");
		return;
	}
	
	m_NnetBP.InitNnet ( m_maxNeeded, m_maxNeededOut,0.0000001,3000,0.01,0.5,false);
	
	bool bTestSucc = false;

	m_NnetBP.TestExpRate(fInArray[0].dataIn);

	int nTrainNow = fInArray.GetSize();
	int tot = nTrainNow;
	bool bSu  ;
	GetDlgItem(IDOK)->SetWindowText("中止训练");
	g_bExec = true;
	SetEnable(FALSE);
	int i=0;
	m_progress.SetRange32 (0,m_NnetBP.m_nRepeat);
	m_progress.ShowWindow(SW_SHOW);
	m_progress.SetPos (0);
	GetDlgItem(IDC_EDIT9)->ShowWindow(SW_HIDE);
	int k;
	for(k=0;k<m_NnetBP.m_nRepeat ;k++)
	{
		if(g_bExec == false)
		{
			GetDlgItem(IDC_EDIT9)->ShowWindow(SW_SHOW);
			return;
		}

		bSu = false;
		for( i=0;i<nTrainNow;i++)
		{
			bTestSucc = false;
			if(i==nTrainNow-1)
				bTestSucc = true;

			{
				CTaiShanApp::DispatchMessageEachTime();
				if(CMainFrame::m_pDlgNL == NULL) return;
				if(g_bExec == false)
				{
					GetDlgItem(IDC_EDIT9)->ShowWindow(SW_SHOW);
					return;
				}
			}

			if(bSu = m_NnetBP.FormularCompute(fInArray[i].dataIn,fInArray[i].dataOut , bTestSucc))
				break;
		}

		m_progress.OffsetPos (1);
	}
	m_progress.SetPos (0);
	m_progress.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT9)->ShowWindow(SW_SHOW);

	m_NnetBP.SaveWeight();




	g_bExec = false;
	GetDlgItem(IDOK)->SetWindowText("开始训练");
	SetEnable(TRUE);
	CString sNN;
	sNN.Format ("截至误差：%.6f，",m_NnetBP.m_diffPre) ;
	m_sTrainInfo = _T("训练完成。");
	m_sTrainInfo+=sNN;
	if(k>m_NnetBP.m_nRepeat) k = m_NnetBP.m_nRepeat;
	sNN.Format ("实际训练次数：%d",k) ;
	m_sTrainInfo+=sNN;

	UpdateData(FALSE);

}

void CTaiKlineDlgNeuralLearn::OnRestoreWeight() 
{
	
	if(!UpdateData())
		return;
    CString     defaultname;
	defaultname=this->m_sFileName ;
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));
	strdir.ReleaseBuffer ();

	CFileDialog  m_filedia(TRUE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,"*.wgt|*.wgt||");
	CString s2= strdir;
	s2.TrimRight ("\\");
	s2.TrimRight ("/");
	m_filedia.m_ofn.lpstrInitialDir=s2+"\\知识数据库";
	if(m_filedia.DoModal()==IDCANCEL)
		return;
	CString   filename;
	filename=m_filedia.GetFileName();
	if(filename.Find('.') == -1)
	{
	  CString temp;
	  temp.Format("%s.wgt",filename);
	  filename = temp;
	}

	CString   pathname;
	pathname=m_filedia.GetPathName ();
	CFileFind flFind;
	if(flFind.FindFile (pathname)==FALSE)
	{
		AfxMessageBox("文件内容无效或文件不存在！");
		return;
	}


	
	this->m_sFileName = filename;

	if(AfxMessageBox("是否装载文件的数据？",MB_YESNO)==IDYES   )
	{
		this->m_NnetBP .m_sFileName = pathname;
		this->m_NnetBP .LoadWeight ();
		this->m_sInput = m_NnetBP.m_pJishuIn ->name ;
		this->m_sOutput = m_NnetBP.m_pJishuOut ->name ;
		this->m_nDays = m_NnetBP.m_nDays ;
		int nKlineKind = m_NnetBP.m_nPeriod ;
		if(nKlineKind == MANY_DAY_KLINE)
			m_nDaysManyDay = m_NnetBP.m_nDaysManyDay;
//			CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfManyKline = m_NnetBP.m_nDaysManyDay;
		int i;
		for(i = 0;i<11;i++)
		{
			if(CTaiScreenParent::nLKind[i] == nKlineKind)
				break;
		}
		this->m_conPeriod .SetCurSel (i);
	}
	UpdateData(FALSE);

	 
	
}


void CTaiKlineDlgNeuralLearn::OnButtonSelectInput() 
{

	if(!UpdateData())
		return;
	CDialogLOGXZZB mdiaxzzb(this);
	if(mdiaxzzb.DoModal() == IDOK)
	{
		this->m_sInput = (mdiaxzzb.result);
		UpdateData(FALSE);
		{
			CFormularContent* pJishu = CTaiScreenParent::LookUpArray(-1, m_sInput,CMainFrame::m_taiShanDoc);
			if(pJishu == 0)
			{
				AfxMessageBox("输入指标不存在！");
				return;
			}
			if(m_NnetBP.m_pJishuIn ==NULL )
				m_NnetBP.m_pJishuIn = new CFormularContent ;
			m_NnetBP.m_pJishuIn->SetData (pJishu);
		}
		m_bPrepared = false;
	}
	
}

void CTaiKlineDlgNeuralLearn::OnButtonSelectOutput() 
{

	if(!UpdateData())
		return;
	CDialogLOGXZZB mdiaxzzb(this);
	if(mdiaxzzb.DoModal() == IDOK)
	{
		this->m_sOutput = (mdiaxzzb.result);
		UpdateData(FALSE);
		{
			CFormularContent* pJishu = CTaiScreenParent::LookUpArray(-1, m_sOutput,CMainFrame::m_taiShanDoc);
			if(pJishu == 0)
			{
				AfxMessageBox("输出指标不存在！");
				return;
			}
			if(m_NnetBP.m_pJishuOut ==NULL )
				m_NnetBP.m_pJishuOut = new CFormularContent ;
			m_NnetBP.m_pJishuOut->SetData (pJishu);
		}
		m_bPrepared = false;
	}
	
}

void CTaiKlineDlgNeuralLearn::OnButtonDeleteStock() 
{

	int nInd = m_conStockData.GetCurSel ();
	if(nInd < 0 )
		return;
	m_conStockData.DeleteString (nInd);
	m_bPrepared = false;
}

BOOL CTaiKlineDlgNeuralLearn::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	int nSel = 0;
	for(int i=0;i<11;i++)
	{
	   this->m_conPeriod .AddString(CTaiScreenParent::m_gPeriodName[i]);
	   if(i==6)
			   nSel = 6;
	}
	m_conPeriod.SetCurSel(nSel);

	CTime tmB(1988,1,1,1,1,0);
	CTime tmE(2037,1,1,1,1,0);
	m_conTmBegin.SetRange( &tmB, &tmE);
	m_conTmEnd.SetRange( &tmB, &tmE);

	this->m_tmEnd = CTime::GetCurrentTime ();
	this->m_tmBegin = CTime(1999,1,1,0,0,0);
	UpdateData(FALSE);
	CreateDirectory("知识数据库",NULL);

	GetDlgItem(IDC_EDIT4)->SetWindowText("0.00001");

	return TRUE;  
}

void CTaiKlineDlgNeuralLearn::AddListItem(CString symbol,int stkKind )
{
	CString s=" ";
	if(symbol.GetLength ()==4)
		s="   ";
	s=symbol+s;
	CString s2 = this->m_tmBegin .Format ("%Y%m%d ");
	s+=s2;
	s2 = this->m_tmEnd .Format ("%Y%m%d ");
	s+=s2;
	this->m_conStockData .AddString (s);

	int nLen = m_conStockData.GetCount ();

	m_conStockData.SetItemData( nLen-1,stkKind);

}

void CTaiKlineDlgNeuralLearn::OnAddStock() 
{

	if(!UpdateData())
		return;
	if(	m_tmBegin > m_tmEnd)
	{
		AfxMessageBox("请修改时间范围！");
		return;
	}

	SelectStock pDlg(this,TRUE);
	pDlg.DoModal();
	m_bPrepared = false;
}

void CTaiKlineDlgNeuralLearn::OnCancel() 
{

	DestroyWindow();

}

void CTaiKlineDlgNeuralLearn::OnClose() 
{

	DestroyWindow();
	

}

void CTaiKlineDlgNeuralLearn::PostNcDestroy() 
{

	g_bExec = false;
	delete this;
	CMainFrame::m_pDlgNL = NULL;
	CDialog::PostNcDestroy();
}

BOOL CTaiKlineDlgNeuralLearn::PreTranslateMessage(MSG* pMsg) 
{
	
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CTaiKlineDlgNeuralLearn::GetStockSymbolList(WPARAM wParam, LPARAM lParam)
{
	
	CTaiShanDoc* pDoc=CMainFrame::m_taiShanDoc; 

	SHARES_DATA_ADD *pAddCode=(SHARES_DATA_ADD *)wParam;
	int lsCount=(int)lParam;
	CReportData *Cdat;

	for(int i=0;i<lsCount;i++)
	{
	
		ASSERT(pAddCode[i].StockType<=-1);
		if(pAddCode[i].StockType==-1  && pDoc->m_sharesInformation.Lookup(pAddCode[i].name, Cdat,-pAddCode[i].StockType -1) == TRUE )
		{
			CString s = pAddCode[i].name;
			AddListItem( s,Cdat->kind );
		}
	}
	return 0;
}


void CTaiKlineDlgNeuralLearn::SetEnable(BOOL bEnable)
{
	int id[] = {IDC_COMBO1,IDC_BUTTON1,IDC_BUTTON2,IDC_BUTTON3,IDC_BUTTON4,IDC_BUTTON5,IDC_BUTTON6,IDC_ADD_STOCK,IDCANCEL};
	int i=0;
	for(i=0;i<9;i++)
	{
		GetDlgItem(id[i])->EnableWindow (bEnable);
	}
}

void CTaiKlineDlgNeuralLearn::OnButtonEditData() 
{

	CTaiKlineDlgNNetEditData dlg(this);
	if(dlg.DoModal () == IDOK)
		m_bPrepared = false;
}

void CTaiKlineDlgNeuralLearn::OnButtonPrepareData() 
{
	
	CString sTitle;
	GetDlgItem(IDOK)->GetWindowText(sTitle);
	if(sTitle == "中止训练")
		return;
	
	fInArray.RemoveAll ();

	m_bPrepared = false;
	if(!UpdateData())
		return;

	STOCK_DATA_SELECTED stockData;
	int n = this->m_conStockData .GetCount ();
	if(n<=0)
	{
		AfxMessageBox("请选择股票数据！");
		return;
	}

	int nSel = this->m_conPeriod .GetCurSel ();
	if(nSel<0)
	{
		AfxMessageBox("请选择时间周期！");
		return;
	}

	
	if(this->m_sInput == "" || this->m_sOutput == "" )
	{
		AfxMessageBox("请选择输入或输出指标！");
		return;
	}
	if(m_NnetBP.m_pJishuIn ==NULL )
	{
		CFormularContent* pJishu = CTaiScreenParent::LookUpArray(-1, m_sInput,CMainFrame::m_taiShanDoc);
		if(pJishu == 0)
		{
			AfxMessageBox("输入指标不存在！");
			return;
		}
		m_NnetBP.m_pJishuIn = new CFormularContent ;
		m_NnetBP.m_pJishuIn->SetData (pJishu);
	}
	if(m_NnetBP.m_pJishuOut ==NULL )
	{
		CFormularContent* pJishu = CTaiScreenParent::LookUpArray(-1, m_sOutput,CMainFrame::m_taiShanDoc);
		if(pJishu == 0)
		{
			AfxMessageBox("输出指标不存在！");
			return;
		}
		m_NnetBP.m_pJishuOut = new CFormularContent ;
		m_NnetBP.m_pJishuOut->SetData (pJishu);
	}
	if(this->m_sFileName == "")
	{
		AfxMessageBox("知识数据库名称不能为空！");
		return;
	}
	CString sPath = "知识数据库\\"+m_sFileName;
	CFileFind filefind;
	if(filefind.FindFile (sPath) == TRUE)
		if(AfxMessageBox("文件已经存在，是否覆盖原始文件的数据？",MB_YESNO)!=IDYES   )
			return;

	EnableWindow(FALSE);
	this->m_NnetBP .m_sFileName = this->m_sFileName ;

	Kline *pKline=NULL;
	CBuySellList buySellList;

	int nArrData = 0;
	for(int i = 0;i<n;i++)
	{
		if(nArrData>=8000)
		{
			AfxMessageBox("数据太多，将用前8000个数据进行计算！");
			break;
		}

		CString s ;
		m_conStockData.GetText (i,s);
		stockData.m_symbol = s.Left (7);
		stockData.m_symbol.TrimRight (" ");
		stockData.m_stkKind = (BYTE)(m_conStockData.GetItemData (i));

		CString stm = s.Mid(7,8);
		int ntm = atoi(stm);
		stockData.m_tmBegin = CTime(ntm/10000,ntm%10000/100,ntm%100,0,0,0);
		stm = s.Right(9);
		stm.TrimRight (" ");
		ntm = atoi(stm);
		stockData.m_tmEnd = CTime(ntm/10000,ntm%10000/100,ntm%100,0,0,0);
		
	
		int nKline = CTaiKlineFileKLine::ReadKLineAny(stockData.m_symbol,stockData.m_stkKind, pKline, 0, 
			CTaiScreenParent::nLKind[nSel], &buySellList,48,true, 
			&(stockData.m_tmEnd),&(stockData.m_tmBegin));

		if(nKline<=0)
			continue;

	
		int pos=-1;
		ARRAY_BE line;

		CFormularCompute* m_formuCompute = NULL;
		INPUT_PARAM input;
		input.pmax			= nKline-1;
		input.pp			= pKline;
		input.symbol		= stockData.m_symbol ;
		input.nKindKline	= CTaiScreenParent::nLKind[nSel] ;
		input.pBuySellList	=&(buySellList) ;
		input.pIndex		= m_NnetBP.m_pJishuIn;
		input.bAddBuySell   = false;
		input.m_stkKind		= stockData.m_stkKind;
		if(ComputeFormu2(m_formuCompute,input)!=0)
		{
		
			CTaiKlineFileHS::RemoveHs(buySellList);
			if(pKline)
				delete[] pKline;     
			pKline = NULL;
			delete m_formuCompute;
			EnableWindow();
			return;
		}
		CFormularCompute* m_formuComputeOut = NULL;
		input.pIndex		= m_NnetBP.m_pJishuOut;
		if(ComputeFormu2(m_formuComputeOut,input)!=0)
		{
		
			CTaiKlineFileHS::RemoveHs(buySellList);
			if(pKline)
				delete[] pKline;      
			delete m_formuComputeOut;
			EnableWindow();
			return;
		}

		
		int rtnLine=0;
		m_maxNeeded =m_formuCompute->GetRlineNum ();
		float* fp=new float[nKline*m_maxNeeded];
		int nMustNeed2 = 0;
		int nEndData = 0;
		for(int j=0;j<m_maxNeeded;j++)
		{
			line.line = &fp[j*nKline];
			CString namePer;
			if(m_formuCompute->GetLine (j,line,namePer)==1)
			{
				nMustNeed2 = -1;
				break;
			}
			if(nMustNeed2<line.b)
				nMustNeed2 = line.b;

			if(j==0) nEndData = line.e;

			if(line.e<nEndData)
				nEndData = line.e;

		}
		delete m_formuCompute;

	
		if(nKline-this->m_nDaysDeleted -1<nEndData)
			nEndData = nKline-this->m_nDaysDeleted -1;

		
		m_maxNeededOut =m_formuComputeOut->GetRlineNum ();
		float* fpOut=new float[nKline*m_maxNeededOut];
		if(nMustNeed2!=-1)
		for(int j=0;j<m_maxNeededOut;j++)
		{
			line.line = &fpOut[j*nKline];
			CString namePer;
			if(m_formuComputeOut->GetLine (j,line,namePer)==1)
			{
				nMustNeed2 = -1;
				break;
			}
			if(nMustNeed2<line.b)
				nMustNeed2 = line.b;

			if(line.e<nEndData)
				nEndData = line.e;

		}
		delete m_formuComputeOut;



		if(nMustNeed2 >-1)
		{

		
			InKlineData inData;
			int nMove = 0;
			if(this->m_nDays >0) nMove = m_nDays;

			for(int j = nMustNeed2+nMove;j<nEndData;j++)
			{
			
				for(int k=0;k<m_maxNeeded;k++)
				{
					inData.dataIn[k] = fp[k*nKline+j-nMove];
				}

			
				for(int k=0;k<m_maxNeededOut;k++)
				{
					inData.dataOut [k] = fpOut[k*nKline+j];//CTaiKlineNnetBP::RegularData(fpOut[k*nKline+j]);
				}
				inData.time =  pKline[j-nMove].day ;
				strcpy(inData.symbol ,stockData.m_symbol);

				fInArray.Add(inData);
				nArrData++;
			}
		

		}
		delete[] fp;
		delete[] fpOut;
	}

	
	CTaiKlineFileHS::RemoveHs(buySellList);
	if(pKline)
		delete[] pKline;   


	{
		int i=fInArray.GetSize();
		CArray<InKlineData,InKlineData> fArr;
		for(int j = 0;j<i;j++)
		{
		
			int k = fArr.GetSize();
			int l;
			for(l=0;l<k;l++)
			{
				if(memcmp(&fInArray[j],&fArr[l],sizeof(InKlineData)) == 0)
					break;
			}
			if(l>=k)//
				fArr.Add(fInArray[j]);
		}
		fInArray.RemoveAll ();
		fInArray.Append(fArr);
	}
	
	bool bStatistics = false;
	if(AfxMessageBox("是否把输入数据进行统计分类？",MB_YESNO) == IDYES)
		bStatistics = true;
	m_NnetBP.PrepareKlineData(fInArray, this->m_fDiff , this->m_nMaxCount ,m_maxNeeded,m_maxNeededOut,m_bPreDealData,bStatistics);

	this->m_nReal = fInArray.GetSize ();
	if(m_nReal>this->m_nMaxCount )
	{
		AfxMessageBox("实际使用数据个数不能超过最大个数！");
		fInArray.RemoveAt(m_nMaxCount,m_nReal-m_nMaxCount);
		m_nReal = m_nMaxCount ;
	}
	m_sTrainInfo = _T("可以执行训练。");
	UpdateData(FALSE);
	m_bPrepared = true;
	EnableWindow();
}

void CTaiKlineDlgNeuralLearn::OnSelchangeComboPeriod() 
{
	
	m_bPrepared = false;
	CString s;
	m_conPeriod.GetWindowText (s);
	if(s != "多日线")
	{
		m_conDays.ShowWindow (SW_HIDE);
	}
	else
	{
		m_conDays.ShowWindow (SW_SHOW);
	}
	UpdateData(FALSE);

}

void CTaiKlineDlgNeuralLearn::OnChangeEditPreDays() 
{
	
	m_bPrepared = false;
	
}

void CTaiKlineDlgNeuralLearn::OnChangeEditIn() 
{
	
	m_bPrepared = false;
	
}

void CTaiKlineDlgNeuralLearn::OnChangeEditOut() 
{
	
	m_bPrepared = false;
	
}
