// ByjDrawRightRect.cpp: implementation of the ByjDrawRightRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ByjDrawRightRect.h"
#include "GetSetReg.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDoKline.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineMoveChenben.h"
#include "StructTaiShares.h"
#include "StructKlineView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString ByjDrawRightRect::m_nameIndex="";

ByjDrawRightRect::ByjDrawRightRect()
{

}

ByjDrawRightRect::~ByjDrawRightRect()
{

}
ByjDrawRightRect::ByjDrawRightRect(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc)
{
	pDoc = bsDoc;	
 	pView = bsView;	
	if(m_nameIndex == "")
	{
		GetSetReg reg;
		char ch[512];
		DWORD ncb = sizeof(ch);
		DWORD typ = REG_SZ;
		if(reg.GetValue ("software\\indicator","RightName",typ,(unsigned char *)ch,ncb) == ERROR_SUCCESS)
		{
			ch[511] = 0;
			m_nameIndex = ch;
		}
	}
	m_min_sun = 0;
	m_max_sun = 0;

	for(int i = 0;i<18;i++)
		m_lineName[i] = "";

	m_nValidIndicator = 0;


}
void ByjDrawRightRect::CalcSonFiguer(CDC* pDC,CRect &rt)
{
	m_rectDrawLine = rt;
	{
		m_dataFormular .numLine = 0;
		CFormularContent*	pIndex;
		pIndex=(CFormularContent*)pDoc->LookUpArray (pDoc->m_formuar_index,m_nameIndex); 
		if(pIndex!=NULL)
			if(!pIndex->IsValid()) return;

		if(pIndex==NULL || pView->m_nCountKline<=0)
		{
			return;
		}
	
		m_nValidIndicator  = CTaiScreenParent::IsPeriodValid(pView->m_nKlineKind2 ,pIndex);

		CString s ;

		CString str = " ";


		CFormularCompute* m_formuCompute = NULL;
		INPUT_PARAM input;
		input.pmax			=pView->m_nCountKline-1;
		input.pp			= pView->m_pkline;
		input.symbol		= pView->m_sharesSymbol ;
		input.nKindKline	= pView->m_nKlineKind2 ;
		input.pBuySellList	=&( pView->m_hs) ;
		input.pIndex		= pIndex;
		input.bAddBuySell   = false;
		input.m_stkKind		= pView->m_stkKind;

		float fMax, fMin;
		fMax = pView->m_pkline[0].high ;
		fMin = pView->m_pkline[0].low ;
		for(int j=1;j<pView->m_nCountKline;j++)
		{
			if(pView->m_pkline[j].high>fMax)
				fMax = pView->m_pkline[j].high;
			if(pView->m_pkline[j].low<fMin)
				fMin = pView->m_pkline[j].low;
		}

		ARRAY_Right_Box::SetTime (CTime(pView->m_pkline [pView->pKlineDrawing ->m_footCurrent].day )) ;
		TRACE("pView->pKlineDrawing ->m_footCurrent: %d   ; \n",pView->pKlineDrawing ->m_footCurrent);
		ARRAY_Right_Box::GetRuler(fMax, fMin);

		CTaiKlineShowKline *pDoK = pView->pKlineDrawing ;
		pDoK->m_nSon=0;
		pDoK->SetRectCurrent(0);

		if(ComputeFormu2(m_formuCompute,input)==0)
		{
			int rtnLine=0;
			m_dataFormular .numLine =m_formuCompute->GetRlineNum ();
			if(m_dataFormular .numLine>18)
				m_dataFormular .numLine = 18;

			for(int i=0 ;i<m_dataFormular .numLine ;i++)
			{

				CString namePer;
				int nIn = i ;
				rtnLine = m_formuCompute->GetLine (nIn,m_dataFormular .line [i].m_arrBE,namePer);
				if(rtnLine==1)
					m_dataFormular .line [i].m_arrBE.b = -1;
				
				m_dataFormular .line [i].type = m_formuCompute->m_RlineType[i] ;
			
				if(namePer.GetLength()>18)
					m_lineName [i]="未知";
				else
					m_lineName [i]=namePer;
				GetMaxMin(i,i==0);


			}
			for(int i=0 ;i<m_dataFormular .numLine ;i++)
			{
				DrawSonFiguer( pDC,i);
			}
			DrawCaption(pDC, pIndex);
				
		}
		else
			;

		delete m_formuCompute;
		ARRAY_Right_Box::Reset ();
	}

}
void ByjDrawRightRect::DrawSonFiguer(CDC* pDC,int j)
{
	if(!m_dataFormular .line[j].m_arrBE .lineRight.m_pData)
		return;

	CTaiKlineShowKline *pDoK = pView->pKlineDrawing ;


	float high = pDoK->YTransfer((int)m_rectDrawLine.top);//
	float low = pDoK->YTransfer((int)m_rectDrawLine.bottom);


	{
		float fGap;
		int nFirstFoot;
		int nBeginY = m_dataFormular .line[j].m_arrBE .lineRight.GetFirstY( high,  low,  m_rectDrawLine.top, m_rectDrawLine. bottom, fGap, nFirstFoot);

		float max = m_max_sun;
		float min = m_min_sun;

		int nFootEnd = ARRAY_Right_Box::m_nCount-1;
		for(int i = nFirstFoot;i<ARRAY_Right_Box::m_nCount ;i++)
		{
			int y = nBeginY-fGap*(i - nFirstFoot);
			if(y<0) 
			{
				nFootEnd = i;
				break;
			}
		}


//		this->m_dataFormular .line[j].m_arrBE .lineRight  .GetMaxMin( nFirstFoot,  nFootEnd, max, min);

		COLORREF	clr;
		clr=pDoc->m_colorArray [j%6+7];

		int nStyle[3] = {PS_SOLID,PS_DOT,PS_DASHDOTDOT};
		CPen pen(nStyle[j/6],1,clr);
		CPen* pOldpen=pDC->SelectObject(&pen);

		float fScale ;
		bool b = GetScale( m_rectDrawLine.right ,  m_rectDrawLine.left ,  max,  min, fScale);

		int flag = m_dataFormular .line[j].type ;
		for(int i = nFirstFoot;i<nFootEnd ;i++)
		{
			int right = (m_dataFormular .line[j].m_arrBE .lineRight.m_pData[i]-min)*fScale+m_rectDrawLine.left;
			int  y = nBeginY-fGap*(i-nFirstFoot);
			switch(flag)
			{
				case LINESTICK:
				case STICK_VOL_FS:
					pDC->MoveTo (m_rectDrawLine.left,y);
					pDC->LineTo (right,y);
					if(flag==STICK_VOL_FS)
						break;
				case 0:
					if( i == nFirstFoot)
						pDC->MoveTo (right,y);
					else
						pDC->LineTo (right,y);
					break;
				case VOLSTICK:
					break;

				case STICK:
				case STICK_LINGXIAN_FS:
					pDC->MoveTo (m_rectDrawLine.left,y);
					pDC->LineTo (right,y);
					break;



				case CROSSDOT:
					pDC->MoveTo(right-2,y-2);
					pDC->LineTo(right+2,y+2);
					pDC->MoveTo(right-2,y+2);
					pDC->LineTo(right+2,y-2);
					break;
				case BUY_CASE:
				case SELL_CASE:
				case CIRCLE:

					pDC->Arc(right-2,y-2,right+2,y+2,right+2,y+2,right+2,y+2);
					break;

			}
		}
		pDC->SelectObject(pOldpen);

	}
}

bool ByjDrawRightRect::GetScale(int n1, int n2, float f1, float f2, float& fOut)
{
	if(f1-f2==0)
		return false;
	fOut = (n1-n2)/(f1-f2);
	return true;
}


void ByjDrawRightRect::SaveName(CString name)
{
	m_nameIndex = name;
	if(m_nameIndex == "")
	{
		GetSetReg reg;
		char ch[512];
		strcpy(ch,name);
		DWORD ncb = sizeof(ch);
		DWORD typ = REG_SZ;
		if(reg.SetValue ("software\\indicator","RightName",typ,(unsigned char *)ch,ncb) == ERROR_SUCCESS)
		{
		}
	}
}

void ByjDrawRightRect::GetMaxMin(int j,bool bFirst)
{
	if(!m_dataFormular .line[j].m_arrBE .lineRight.m_pData)
		return;

	CTaiKlineShowKline *pDoK = pView->pKlineDrawing ;


	float high = pDoK->YTransfer((int)m_rectDrawLine.top);
	float low = pDoK->YTransfer((int)m_rectDrawLine.bottom);


	float fGap;
	int nFirstFoot;
	int nBeginY = m_dataFormular .line[j].m_arrBE .lineRight.GetFirstY( high,  low,  m_rectDrawLine.top, m_rectDrawLine. bottom, fGap, nFirstFoot);

	float max ;
	float min ;
	int nFootEnd = ARRAY_Right_Box::m_nCount-1;
	for(int i = nFirstFoot;i<ARRAY_Right_Box::m_nCount ;i++)
	{
		int y = nBeginY-fGap*(i - nFirstFoot);
		if(y<0) 
		{
			nFootEnd = i;
			break;
		}
	}


	this->m_dataFormular .line[j].m_arrBE .lineRight  .GetMaxMin( nFirstFoot,  nFootEnd, max, min);
	if(bFirst)
	{
		this->m_max_sun = max;
		m_min_sun = min;
	}
	else
	{
		if(m_max_sun < max)
			m_max_sun = max;
		if(m_min_sun > min)
			m_min_sun = min;
	}


}

void ByjDrawRightRect::DrawCaption(CDC *pDC, CFormularContent *pIndex)
{

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	pDC->SetTextAlign( TA_LEFT );
	pDC->SetTextColor(  pDoc->m_colorArray [1]);
	pDC->SetBkMode(OPAQUE  );

	CString s = pIndex->name +":";
	if(s == "") s = "指标名称：";
	int y = 2;
	pDC->TextOut(m_rectDrawLine.left+2,y,s);

	int left=(pDC->GetOutputTextExtent(s)).cx+m_rectDrawLine.left+2;

	int len=0;
	int num=m_dataFormular.numLine;

	for(int i=0;i<num;i++)
	{ 
		CString pnm=m_lineName[i];
		pnm.MakeUpper();
		pDC->SetTextColor( pDoc->m_colorArray [7+i%6]);
		CString vl;
		vl = " ";
		pnm+=vl;

		pDC->TextOut (left+len,2 ,pnm);
		len+=(pDC->GetOutputTextExtent(pnm)).cx;
	}
	pDC->SelectObject(pOld);

}
