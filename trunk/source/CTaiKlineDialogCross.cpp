// CTaiKlineDialogCross.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "CTaiKlineDialogCross.h"
#include "MemDC.h"
#include "StructKlineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogCross dialog


CTaiKlineDialogCross::CTaiKlineDialogCross(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDialogCross::IDD, pParent)
{
	m_nHeight = 240;
	m_bSaveBackground = true;
	pView = (CTaiShanKlineShowView* )pParent;
//	ASSERT(pView!=NULL);
	//{{AFX_DATA_INIT(CTaiKlineDialogCross)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_isFirst=true;
}


void CTaiKlineDialogCross::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDialogCross)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDialogCross, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDialogCross)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogCross message handlers
CTaiShanDoc* pDoc = NULL;		
CTaiShanKlineShowView* pView = NULL;	


void CTaiKlineDialogCross::OnPaint() 
{
	if(pView == NULL)
		return;
	if(pView->pMin1Drawing->m_pReportData==NULL)
		return;
	CClientDC dc(this); 
	CMemDC memDC(&dc);

	pDoc=CMainFrame::m_taiShanDoc; 
	CRect r2;
	GetClientRect(r2);
	memDC.FillSolidRect(r2,pDoc->m_colorArray[0]);
	int wd = r2.Width();
	int ht = r2.Height ();

	CFont ft2;
	LOGFONT lgf2=pDoc->m_fontstr[1];

	ft2.CreateFontIndirect (&lgf2 );
	CFont* pOldF=memDC.SelectObject(&ft2);
	m_nHeight = memDC.GetOutputTextExtent("宋体").cy +3;


	static CString sName2="";
	if(pView->pMin1Drawing ->m_pReportData->name!=sName2)
	{
		SetWindowText(pView->pMin1Drawing ->m_pReportData->name);
		sName2 = pView->pMin1Drawing ->m_pReportData->name;
	}

	memDC.SetTextColor( pDoc->m_colorArray[1]);
	memDC.SetBkMode(TRANSPARENT);
	if(m_isFirst==true)
	{
		m_isFirst=false;
	}
	float fVal[16];
	for(int k = 0;k<16;k++)
		fVal[k] = 0;
	if(pView->m_nKlineKind2 ==0)
	{

		
		static CString strMin1Name[] = {"时间","数值","成交价","成交量","成交额","涨跌值","涨跌幅","成本价","委买量","委卖量","换手率"};

		if(pView->pMin1Drawing ->m_footCurrent<0)
			pView->pMin1Drawing ->m_footCurrent = 0;
		if(pView->pMin1Drawing ->m_footCurrent>239)
			pView->pMin1Drawing ->m_footCurrent = 239;

		memDC.SetTextAlign( TA_LEFT );
		for(int i=0;i<11;i++)
		{
			memDC.TextOut (1,i*m_nHeight,strMin1Name[i]);
		}
		m_isFirst=false;

		memDC.SetTextAlign( TA_RIGHT );

		CStringArray sArr;
		int right=r2.right -1;
		CString s;
		s=pView->pMin1Drawing ->TimeToString(pView->pMin1Drawing ->m_footCurrent-pView->pMin1Drawing ->m_footBegin);
		sArr.Add(s);

		if(pView->m_hit>=REGION_PICT1&&pView->m_hit<=REGION_PICT5)
		{
			if(pView->pMin1Drawing ->m_currentValue>=1000000)
				s.Format ("%d",(int)pView->pMin1Drawing ->m_currentValue);
			else
				s.Format ("%.3f",pView->pMin1Drawing ->m_currentValue);
		}
		else
			s="";
		sArr.Add(s);

		if(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]<0)
			pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]=0;
		if(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]>10000000)
			pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]=0;
		s.Format ("%.3f",pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]);
		sArr.Add(s);
		fVal[2] = pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]
				-pView->pMin1Drawing ->m_close;

		s.Format ("%d",(int)pView->pMin1Drawing ->m_vol[pView->pMin1Drawing ->m_footCurrent]);
		sArr.Add(s);

		s.Format ("%.3f",(float)(pView->pMin1Drawing ->m_amount[pView->pMin1Drawing ->m_footCurrent]/10000));
		sArr.Add(s);

		s.Format ("%.3f",(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]-pView->pMin1Drawing ->m_close));
		sArr.Add(s);
		fVal[5] = fVal[2];

		if(pView->pMin1Drawing ->m_close>0)
		{
			s.Format ("%.3f",(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]-pView->pMin1Drawing ->m_close)*100/pView->pMin1Drawing ->m_close);
			s=s+"%";
		}
		else
			s="";
		sArr.Add(s);
		fVal[6] = fVal[2];

	
		float f1;
		if(pView->GetPriceAve(f1))
		{
			s.Format ("%.3f",f1 );
		}
		else
			s="";
		sArr.Add(s);
		fVal[7] = fVal[2];


		s.Format ("%d",(int)pView->pMin1Drawing ->m_pReportData->accb );
		sArr.Add(s);
		fVal[8] = fVal[2];
		
		s.Format ("%d",(int)pView->pMin1Drawing ->m_pReportData->accs );
		sArr.Add(s);
		fVal[9] = fVal[2];

	
		float ltp = CTaiShanKlineShowView::GetCapital(pView->pMin1Drawing ->m_pReportData);
		if(ltp>0)
		{
			s.Format ("%.3f",pView->pMin1Drawing->m_vol[pView->pMin1Drawing ->m_footCurrent]/ltp*100 );
		}
		else
			s="";
		sArr.Add(s);
		for(int i=0;i<sArr.GetSize ();i++)
		{
			if(i==2 || i>=5 && i<=9 ) SetColor(&memDC,fVal[i]);
			else memDC.SetTextColor( pDoc->m_colorArray[1]);
			memDC.TextOut (right,i*m_nHeight,sArr[i]);
		}

	}
	else if(pView->pKlineDrawing ->ValidDoKline()) 

	{
	
		static CString strKlineName[] = {"时间","数值","开盘","最高","最低","收盘","成交量","成交额","涨跌幅","振幅","成本价","换手率"};

		if(pView->pKlineDrawing ->m_footCurrent<0)
			pView->pKlineDrawing ->m_footCurrent = 0;
		if(pView->pKlineDrawing ->m_footCurrent>pView->m_nCountKline -1)
			pView->pKlineDrawing ->m_footCurrent = pView->m_nCountKline -1;

		memDC.SetTextAlign( TA_LEFT );
		for(int i=0;i<12;i++)
		{
			memDC.TextOut (1,i*m_nHeight,strKlineName[i]);
		}
		m_isFirst=false;

		memDC.SetTextAlign( TA_RIGHT );
		CStringArray sArr;


		int right=r2.right -1;
		CString s;

		if(pView->pKlineDrawing ->m_footCurrent>=0)
		{
			s=pView->pKlineDrawing ->TimeToStr(pView->pKlineDrawing ->m_footCurrent-pView->pKlineDrawing ->m_footBegin);
			sArr.Add(s);

			if(pView->m_hit>=REGION_PICT1&&pView->m_hit<=REGION_PICT5)
			{
				if(pView->pKlineDrawing ->m_currentValue>=1000000)
					s.Format ("%d",(int)pView->pKlineDrawing ->m_currentValue);
				else
					s.Format ("%.3f",pView->pKlineDrawing ->m_currentValue);
			}
			else
				s="";
			sArr.Add(s);

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].open);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[2] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].open
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].high);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[3] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].high
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].low);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[4] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].low
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].close);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[5] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].close
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%d",(int)pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].vol);
			sArr.Add(s);
			
			s.Format ("%.1f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].amount/10000);
			sArr.Add(s);

			s="";
			if(pView->pKlineDrawing ->m_footCurrent > 0)
			{
				if(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close>0 )
				{
					s.Format ("%.2f",(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].close
						-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close)*100/
						pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close);
					s+="%";
				}
			}
			sArr.Add(s);
			fVal[8] = fVal[5];
		
			s="";
			if(pView->pKlineDrawing ->m_footCurrent > 0)
			{
				if(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close>0 )
				{
					s.Format ("%.2f",(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].high
						-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].low)*100/
						pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close);
					s+="%";
				}
			}
			sArr.Add(s);

		
			s="";
			float f1;
			if(pView->GetPriceAve( f1,pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].vol,pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].amount))
				s.Format ("%.3f",f1);
			sArr.Add(s);

	
			float ltp = CTaiShanKlineShowView::GetCapital(pView->pMin1Drawing ->m_pReportData);
			if(ltp >0)
			{
				s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].vol/ltp*100  );
			}
			else
				s="";
			sArr.Add(s);

			for(int i=0;i<sArr.GetSize ();i++)
			{
				if(i>1 && i<6 || i == 8) SetColor(&memDC,fVal[i]);
				else memDC.SetTextColor( pDoc->m_colorArray[1]);
				memDC.TextOut (right,i*m_nHeight,sArr[i]);
			}

		}

	}
	memDC.SelectObject(pOldF);
	
	CPen pen(PS_SOLID   ,1, pDoc->m_colorArray[17]);
	CPen* penOld;
	penOld = memDC.SelectObject(&pen);
	for(int i=0;i<ht/m_nHeight; i++)
	{
		memDC.MoveTo(0,i*m_nHeight-3);
		memDC.LineTo(wd,i*m_nHeight-3);
	}
	memDC.SelectObject(penOld);
	

	CRect r;
	GetClientRect(r);

	ValidateRect(r);
	////////////////
	
}

BOOL CTaiKlineDialogCross::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	CClientDC dc(this);

	CFont ft2;
	LOGFONT lgf2=CMainFrame::m_taiShanDoc ->m_fontstr[1];
	ft2.CreateFontIndirect (&lgf2 );
	CFont* pOldF=dc.SelectObject(&ft2);
	m_nHeight = dc.GetOutputTextExtent("宋体").cy +3;
	dc.SelectObject (pOldF);


	return TRUE;  
}

void CTaiKlineDialogCross::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTaiKlineDialogCross::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN)
	{
	if(pView!=NULL)
		pView->SetFocus();
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CTaiKlineDialogCross::OnCancel()
{

	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	if(pView!=NULL)
		pView->m_isShowCross=0;	
	ShowWindow(SW_HIDE);

	if(pView!=NULL)
		pView->RedrawWindow();

}


void CTaiKlineDialogCross::OnOK() 
{
	
}

void CTaiKlineDialogCross::OnClose() 
{
	
	if(pView!=NULL)
		pView->m_isShowCross=0;	
	CRect r;
	if(pView!=NULL)
		pView->RedrawWindow();

	
	CDialog::OnClose();
}

void CTaiKlineDialogCross::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CRect r;
	if(pView!=NULL)
	{

	}
	
	
	CDialog::OnMouseMove(nFlags, point);
}

void CTaiKlineDialogCross::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	m_bSaveBackground = true;
	RedrawWindow();
	

	
}

BOOL CTaiKlineDialogCross::OnEraseBkgnd(CDC* pDC) 
{
	
	
	return TRUE;
}

void CTaiKlineDialogCross::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	
	
}

BOOL CTaiKlineDialogCross::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
}

void CTaiKlineDialogCross::SetColor(CDC *pDC, float fVal)
{
	if(fVal>0.0000001)
		pDC->SetTextColor( CMainFrame::m_taiShanDoc->m_colorArray[13]);
	else if(fVal<-0.0000001)
		pDC->SetTextColor( CMainFrame::m_taiShanDoc->m_colorArray[XIA_DIE_WORD]);
	else
		pDC->SetTextColor( CMainFrame::m_taiShanDoc->m_colorArray[PING_PAN_WORD]);

}
