// SHEETWG.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "SHEETWG.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"

#include "cellRange.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetInterfacePage property page

IMPLEMENT_DYNCREATE(CTaiSysSetInterfacePage, CPropertyPage)

CTaiSysSetInterfacePage::CTaiSysSetInterfacePage() : CPropertyPage(CTaiSysSetInterfacePage::IDD)
{

	//{{AFX_DATA_INIT(CTaiSysSetInterfacePage)
	//}}AFX_DATA_INIT
}

CTaiSysSetInterfacePage::~CTaiSysSetInterfacePage()
{

}

void CTaiSysSetInterfacePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiSysSetInterfacePage)
	DDX_Control(pDX, IDC_SETFONT, m_setfont);
	DDX_Control(pDX, IDC_COMBO1, m_sheetwgcom1);
	DDX_Control(pDX, IDC_LIST1, m_sheetwglist1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaiSysSetInterfacePage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiSysSetInterfacePage)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_SETCOLOR, OnSetcolor)
	ON_BN_CLICKED(IDC_SETFONT, OnSetfont)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CTaiSysSetInterfacePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	iLastComItem=0;
	
	CString liststring[systemcolorlength]={"背景","图形文字 ","坐标","阳线",
	                       "阴线","美国线","趋势线","指标线1","指标线2",
						   "指标线3","指标线4","指标线5","指标线6",
						   "显示牌上涨字","显示牌平盘字","显示牌下跌字",
						   "显示牌高亮条","显示牌框线","显示牌背景"};

	int i;
	for (i=0; i<systemcolorlength;i++)
		m_sheetwglist1.AddString((LPCTSTR) liststring[i]);
	
	m_sheetwglist1.SetCurSel(0);      
	CString  combstring[comblength]={"自定义","黑色背景","白色背景"};
        
	for ( i=0;i<3;i++)
		m_sheetwgcom1.AddString((LPCTSTR)combstring[i]);
	
	m_sheetwgcom1.SetCurSel(0);  
    GetDlgItem(IDC_STATICYL)->GetWindowRect(&m_rectsample);
	ScreenToClient(&m_rectsample);
	m_rectsample.top+=5;
	
    CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*   pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();

    fontdefault=pDoc->m_fontdefault;
	m_logfont=pDoc->m_fontdefault;
	for( i=0;i<systemcolorlength;i++)
		cr[i]=pDoc->m_colorArray[i];
	for(i=0;i<fontstructlength;i++)
		fr[i]=pDoc->m_fontstr[i];

	return TRUE; 
}

void CTaiSysSetInterfacePage::OnSelchangeList1() 
{
	int  curselnum;
	curselnum=m_sheetwglist1.GetCurSel();
	switch(curselnum)
	{
		case 1:
		case 13:                      
	  		 if(!m_setfont.IsWindowEnabled())	
				m_setfont.EnableWindow(true);
			 break;
		default:
			if(m_setfont.IsWindowEnabled())
				m_setfont.EnableWindow(false);
	}
}

void CTaiSysSetInterfacePage::OnSetcolor() 
{    
    CColorDialog m_colordia(cr[m_sheetwglist1.GetCurSel()],CC_RGBINIT,this);
    if(m_colordia.DoModal())
	{
		cr[m_sheetwglist1.GetCurSel()]=m_colordia.GetColor();
		InvalidateRect(&m_rectsample);
		UpdateWindow();
	}
}

void CTaiSysSetInterfacePage::OnSetfont() 
{
	LOGFONT fFontDefault;
    switch(m_sheetwglist1.GetCurSel())
	{
		case 1:
			fFontDefault=fr[0];
			break;
		case 13:
			fFontDefault=fr[1];
			break;
	}
	CFontDialog m_fontdia(&fFontDefault);
	if(m_fontdia.DoModal())
	{
		switch(m_sheetwglist1.GetCurSel())
		{
			case 1:
				m_fontdia.GetCurrentFont(&fr[0]);
				InvalidateRect(&m_rectsample);
				UpdateWindow();
				break;
			case 13:
			    m_fontdia.GetCurrentFont(&fr[1]);
                InvalidateRect(&m_rectsample);
				UpdateWindow();
				break;
		}
	}
}

void CTaiSysSetInterfacePage::OnPaint() 
{
	CPaintDC dc(this); 
	

//-------------------------------------
	CBrush brush;                      
	CBrush *poldbrush;
	brush.CreateSolidBrush(cr[0]);
	poldbrush=dc.SelectObject(&brush);
	dc.FillRect(&m_rectsample,&brush);
	dc.SelectObject(poldbrush);
	brush.DeleteObject();
//---------------------------------------
	CPen pen;                          
	CPen *poldpen;
	pen.CreatePen(PS_SOLID,1,cr[2]);
    poldpen=dc.SelectObject(&pen);
	int m_rect_middle=(m_rectsample.left+m_rectsample.right)/2; 
	dc.MoveTo(m_rectsample.left+40,m_rectsample.top+5);  
	dc.LineTo(m_rectsample.left+40,m_rectsample.bottom-5);

	dc.MoveTo(m_rectsample.left+35,m_rectsample.top+30);
	dc.LineTo(m_rect_middle+50,m_rectsample.top+30);
	dc.MoveTo(m_rectsample.left+35,m_rectsample.top+60);
	dc.LineTo(m_rect_middle+50,m_rectsample.top+60);
	dc.MoveTo(m_rectsample.left+35,m_rectsample.top+90);
	dc.LineTo(m_rect_middle+50,m_rectsample.top+90);
	dc.SelectObject(poldpen);
	pen.DeleteObject();

    CFont font;
	CFont *ptrOldFont; 
    font.CreateFontIndirect(&fr[0]);
    ptrOldFont=dc.SelectObject(&font);
	COLORREF oldcolor; 
	oldcolor=dc.SetTextColor(cr[2]);
	dc.SetBkMode(TRANSPARENT);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	int y=m_rectsample.top+30-tm.tmHeight/2;
    int x=m_rectsample.left+((30-tm.tmAveCharWidth*4)<0?0:30-tm.tmAveCharWidth*4);
	dc.TextOut(x,y,"80.0");
	dc.TextOut(x,y+30,"50.0");
	dc.TextOut(x,y+60,"20.0");

//---------
	oldcolor=dc.SetTextColor(cr[1]);
	dc.TextOut(m_rectsample.left+80,m_rectsample.top+5,"8888.88");
	dc.SelectObject(ptrOldFont);
	font.DeleteObject();
//----
	CPen pennew;
	pennew.CreatePen(PS_SOLID,1,cr[6]);
	poldpen=dc.SelectObject(&pennew);
	dc.MoveTo(m_rectsample.left+40,m_rectsample.bottom-20);
	dc.LineTo(m_rectsample.left+m_rectsample.top+40,m_rectsample.top);
	dc.SelectObject(poldpen);
	pennew.DeleteObject();

//---------
    int beep=(m_rect_middle-m_rectsample.left+10)/5;
	int lx=m_rectsample.left+40;
    int ly=m_rectsample.top+60;
	for(int i=0;i<6;i++)
	{
    	CPen pennow;
		pennow.CreatePen(PS_SOLID,1,cr[7+i]);
		poldpen=dc.SelectObject(&pennow);
		dc.MoveTo(lx,ly+i*8);
		dc.LineTo(lx+beep,ly+8+i*8);
		dc.LineTo(lx+2*beep,ly-8+i*8);
		dc.LineTo(lx+3*beep,ly+8+i*8);
		dc.LineTo(lx+4*beep,ly-8+i*8);
		dc.LineTo(lx+5*beep,ly+8+i*8);
		dc.SelectObject(poldpen);
		pennow.DeleteObject();
	}
//----
	CPen penaline;
	penaline.CreatePen(PS_SOLID,2,cr[5]);
	poldpen=dc.SelectObject(&penaline);
	dc.MoveTo(lx+10,ly-40);
	dc.LineTo(lx+10,ly-10);
	dc.MoveTo(lx+30,ly-35);
	dc.LineTo(lx+30,ly-5);
	dc.SelectObject(poldpen);
	penaline.DeleteObject();
	CPen penaline2;
	penaline2.CreatePen(PS_SOLID,1,cr[5]);
	poldpen=dc.SelectObject(&penaline2);
	dc.MoveTo(lx+4,ly-35);
	dc.LineTo(lx+10,ly-35);
	dc.MoveTo(lx+10,ly-20);
	dc.LineTo(lx+16,ly-20);
    dc.MoveTo(lx+24,ly-25);
	dc.LineTo(lx+30,ly-25);
	dc.MoveTo(lx+30,ly-15);
	dc.LineTo(lx+36,ly-15);
	dc.SelectObject(poldpen);
	penaline2.DeleteObject();

//-
    CBrush brushupline;                               
	brushupline.CreateSolidBrush(cr[3]);    
	poldbrush=dc.SelectObject(&brushupline);
	RECT   upkline;
	upkline.left=lx+120;
	upkline.right=lx+130;
	upkline.top=ly-35;
	upkline.bottom=ly-5;
	dc.FillRect(&upkline,&brushupline);
	dc.SelectObject(poldbrush);
	brushupline.DeleteObject();

	CPen penupkline;                   
	penupkline.CreatePen(PS_SOLID,1,cr[3]);
	poldpen=dc.SelectObject(&penupkline);
	dc.MoveTo(lx+125,ly-40);
	dc.LineTo(lx+125,ly+5);
	dc.SelectObject(poldpen);
	penupkline.DeleteObject();

	CBrush brushdownline;                           
	brushdownline.CreateSolidBrush(cr[4]);
	poldbrush=dc.SelectObject(&brushdownline);
	RECT   downkline;
	downkline.left=lx+140;
	downkline.right=lx+150;
	downkline.top=ly-45;
	downkline.bottom=ly-15;
	dc.FillRect(&downkline,&brushdownline);
	dc.SelectObject(poldbrush);
	brushdownline.DeleteObject();

	CPen pendownkline;            
	pendownkline.CreatePen(PS_SOLID,1,cr[4]);
	poldpen=dc.SelectObject(&pendownkline);
	dc.MoveTo(lx+145,ly-50);
	dc.LineTo(lx+145,ly-5);
	dc.SelectObject(poldpen);
	pendownkline.DeleteObject();
	
 //--
	CBrush brushshowbar;            
	brushshowbar.CreateSolidBrush(cr[18]);
	poldbrush=dc.SelectObject(&brushshowbar);
	RECT   showbar;
	showbar.left=m_rect_middle+50;
	showbar.right=m_rectsample.right-5;
	showbar.top=m_rectsample.top+5;
	showbar.bottom=m_rectsample.bottom-5;
	dc.FillRect(&showbar,&brushshowbar);
	dc.SelectObject(poldbrush);
	brushshowbar.DeleteObject();
//----
    CBrush brushshinebar;            
	brushshinebar.CreateSolidBrush(cr[16]);
	poldbrush=dc.SelectObject(&brushshinebar);
	RECT   shinebar;
	shinebar.left=m_rect_middle+50;
	shinebar.right=m_rectsample.right-5;
	shinebar.top=m_rectsample.top+5;
	shinebar.bottom=m_rectsample.top+25;
	dc.FillRect(&shinebar,&brushshinebar);
	dc.SelectObject(poldbrush);
	brushshinebar.DeleteObject();
//-------	
	CFont fdefault;        
	CFont *polddefault; 
    fdefault.CreateFontIndirect(&fontdefault);
    polddefault=dc.SelectObject(&fdefault);
	oldcolor=dc.SetTextColor(cr[13]);
	dc.TextOut(m_rect_middle+50,m_rectsample.top+5,"上涨");
	oldcolor=dc.SetTextColor(cr[14]);
	dc.TextOut(m_rect_middle+95,m_rectsample.top+5,"平盘");
	oldcolor=dc.SetTextColor(cr[15]);
	dc.TextOut(m_rect_middle+140,m_rectsample.top+5,"下跌");
    dc.SelectObject(polddefault);
	fdefault.DeleteObject();
	
	CFont fontrise;      
	CFont *priseold;
    fontrise.CreateFontIndirect(&fr[1]);
	priseold=dc.SelectObject(&fontrise);
    oldcolor=dc.SetTextColor(cr[13]);
	dc.TextOut(m_rect_middle+50,m_rectsample.top+25,"9999");
    dc.TextOut(m_rect_middle+50,m_rectsample.top+45,"8888");
	dc.TextOut(m_rect_middle+50,m_rectsample.top+65,"7777");
	dc.TextOut(m_rect_middle+50,m_rectsample.top+85,"6666");
	dc.SelectObject(priseold);
	fontrise.DeleteObject();

	CFont fontequal;
	CFont *equalold;
    fontequal.CreateFontIndirect(&fr[1]);
	equalold=dc.SelectObject(&fontequal);
    oldcolor=dc.SetTextColor(cr[14]);
	dc.TextOut(m_rect_middle+95,m_rectsample.top+25,"8888");
    dc.TextOut(m_rect_middle+95,m_rectsample.top+45,"7777");
	dc.TextOut(m_rect_middle+95,m_rectsample.top+65,"6666");
	dc.TextOut(m_rect_middle+95,m_rectsample.top+85,"5555");
	dc.SelectObject(equalold);
	fontequal.DeleteObject();

	CFont fontdown;  
	CFont *downold;
    fontdown.CreateFontIndirect(&fr[1]);
	downold=dc.SelectObject(&fontdown);
    oldcolor=dc.SetTextColor(cr[15]);
	dc.TextOut(m_rect_middle+140,m_rectsample.top+25,"7777");
    dc.TextOut(m_rect_middle+140,m_rectsample.top+45,"6666");
	dc.TextOut(m_rect_middle+140,m_rectsample.top+65,"5555");
	dc.TextOut(m_rect_middle+140,m_rectsample.top+85,"4444");
	dc.SelectObject(downold);
	fontdown.DeleteObject();

//-----
	CPen pentableline;     
	pentableline.CreatePen(PS_SOLID,1,cr[17]);
	poldpen=dc.SelectObject(&pentableline);
	for(int i=0;i<5;i++)
	{
		dc.MoveTo(m_rect_middle+50,m_rectsample.top+i*20+25);
	    dc.LineTo(m_rectsample.right-5,m_rectsample.top+i*20+25);
	}
	for(int i=0;i<5;i++)
	{
		dc.MoveTo(m_rect_middle+50+i*45,m_rectsample.top+5);
		dc.LineTo(m_rect_middle+50+i*45,m_rectsample.top+105);
	}
	dc.SelectObject(poldpen);
	pentableline.DeleteObject();
}

void CTaiSysSetInterfacePage::OnSelchangeCombo1() 
{
	CFile systemfile;
	systemfile.Open((LPCTSTR)"系统文件.DAT",CFile::modeReadWrite);
	int index=m_sheetwgcom1.GetCurSel();
	if(iLastComItem==0)
	{
		for(int i=0;i<19;i++)
			crbak[i]=cr[i];
		for(int i=0;i<2;i++)
			frbak[i]=fr[i];
	}

	if(index>0)
	{
		systemfile.Seek(4+(index-1)*76,CFile::begin);
		for(int i=0;i<19;i++)
			systemfile.Read(&cr[i],4);
		systemfile.Seek(2000+(index-1)*120,CFile::begin);
		for(int i=0;i<2;i++)
			systemfile.Read(&fr[i],60);
		systemfile.Close();
		InvalidateRect(&m_rectsample);
		UpdateWindow();
	}
	else if(index==0)
	{
		for(int i=0;i<19;i++)
			cr[i]=crbak[i];
		for(int i=0;i<2;i++)
			fr[i]=frbak[i];
		InvalidateRect(&m_rectsample);
		UpdateWindow();
	}
	iLastComItem=index;	
}