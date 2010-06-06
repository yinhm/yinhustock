
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDoFenshi.h"

#include "MainFrm.h"
#include "CTaiKlineDoKline.h"
#include "CFormularCompute.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "SheetScrollBar.h"
#include "SHARE.H"
#include "CTaiKlineDlgDealHistory.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineDialogShiDuanTJ.h"

#include "StkDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CTaiKlineMin1, CTaiKlineShowKline)

CTaiKlineMin1::CTaiKlineMin1()
{
}

CTaiKlineMin1::CTaiKlineMin1(CTaiShanDoc* pDoci, CTaiShanKlineShowView* pViewi)
{
	memset(&m_dt, 0, sizeof(CReportData));

	m_sOldSymbol = _T("");
	m_sOldStkKind = 0;

	m_bHist = FALSE;

	m_pMoveCB = NULL;
	m_nTextBox = 0;

	m_nCnp = -1;
	m_volPre = 0;

	pDoc = pDoci;
	pView = pViewi;

	m_nKlineCurrent=0;
	m_lineBgn=0;
	m_footBegin=0;
	m_footEnd=0;
	m_footCurrent=0;
	m_klinNumDefault=240;


	for (int i = 0; i < FIGUER_ALL_NUM; i++)
	{
		m_dataFormular[i].numLine = 0;
		for (int j = 0; j < 6; j++)
		{
			m_dataFormular[i].line[j].m_arrBE.line = new float[240];
			memset(m_dataFormular[i].line[j].m_arrBE.line, 0, 4 * 240);
			m_dataFormular[i].line[j].m_arrBE.b = 0;
		}
		for (int j = 6; j < Total_Count_Line; j++)
		{
			m_dataFormular[i].line[j].m_arrBE.line = NULL;
		}
	}


	m_lineDapan[0][0] = new float[240];
	m_lineDapan[0][1] = new float[240];
	m_lineDapan[0][2] = new float[240];
	m_lineDapan[1][0] = new float[240];
	m_lineDapan[1][1] = new float[240];
	m_lineDapan[1][2] = new float[240];

	for (int j = 0; j < 6; j++)
	{
		m_pKlineAdd[j] = new Kline[2];
	}

	for (int i = 0; i < 240; i++)
	{
		m_hsMin1.AddHead(m_hsMin + i);
	}

	m_pReportData = &m_dt;
}

CTaiKlineMin1::~CTaiKlineMin1()
{
	pView->RemoveHs(0);
	pView->RemoveHs(1);

	delete []m_lineDapan[0][0];
	delete []m_lineDapan[0][1];
	delete []m_lineDapan[0][2];
	delete []m_lineDapan[1][0];
	delete []m_lineDapan[1][1];
	delete []m_lineDapan[1][2];
}





void CTaiKlineMin1::InitMinuteLine()
{
	m_pReportData = &m_dt;

	CString symbol = pView->m_sharesSymbol;
	m_lineBgn = 0;
	m_footBegin = 0;

	CString s = symbol;
	pDoc->m_sharesSymbol = s;

	int isSz = 0;
	CString sIndex = CSharesCompute::GetIndexSymbol(0);

	m_bHist = false;
	if(pView->m_pDlgDealHistory !=NULL)
	{
		if(pView->m_pDlgDealHistory->InitDoFenshi(this,  symbol,pView->m_stkKind )==true)
			m_bHist = true;
	}

	if(m_bHist==false)
	{
		m_pFileHs=TSKDatabase()->GetTickFile (symbol,pView->m_stkKind );


		isSz=0;
		if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SZ_MARKET_EX) isSz=1;
		CReportData * pdt= NULL;
		if(pDoc->m_sharesInformation.Lookup(s.GetBuffer (0),pdt,pView->m_stkKind )==0)
		{

			return;
		}
		ASSERT(pdt);

		bool b = !(m_sOldSymbol == symbol && 	m_sOldStkKind == pView->m_stkKind);
		InitHs(b,false);
		if(b)
		{
			time_t mt = CTime::GetCurrentTime ().GetTime ();
			//((CMainFrame*)AfxGetMainWnd())->gSTOCKDLL.QueryMinData(symbol, CSharesCompute::GetMarketKind(pView->m_stkKind),mt);
		}

		if(pDoc->m_bInitDone ==FALSE)
			m_footEnd=pDoc->m_nOldANT [0];
		else
			m_footEnd=pDoc->m_nANT [isSz];

		{
			memcpy(&m_dt,pdt,sizeof(CReportData));
		}
#ifndef WIDE_NET_VERSION
		if(!CTaiShanKlineShowView::IsIndexStock(symbol))
		{
			for(int j=0;j<=m_footEnd;j++)
			{
				float f3 =FABSMY(m_hsMin[j].vol);
				if(m_pReportData->m_Kdata1[j].Volume  < f3 )
				{
					m_pReportData->m_Kdata1[j].Price = m_hsMin[j].price ;
					m_pReportData->m_Kdata1[j].Volume  = f3  ;
				}
				else
				{
					if(m_pReportData->m_Kdata1[j].Price>0) m_hsMin[j].price = m_pReportData->m_Kdata1[j].Price;
					if(m_pReportData->m_Kdata1[j].Volume>f3) m_hsMin[j].vol = m_pReportData->m_Kdata1[j].Volume;
				}
			}
		}
#endif

		pDoc->m_sharesInformation.GetIndexTidxd(m_Tidx);
		pDoc->m_sharesInformation.GetIndexRsdn(m_Nidx);

		m_pS0 = NULL;
		m_pS1 = NULL;

		int stkKind = /*SHZS*/SHAG;
		if (pDoc->m_sharesInformation.Lookup(sIndex.GetBuffer(0), m_pS0, stkKind) == 0)
			return;
		sIndex = CSharesCompute::GetIndexSymbol(1);
		stkKind = SZZS;
		if (pDoc->m_sharesInformation.Lookup(sIndex.GetBuffer(0), m_pS1, stkKind) == 0)
			m_pS1 = m_pS0;

		m_volPre = m_pReportData->totv;
		if (pView->m_isShowCross == 0)
		{
			m_footCurrent = m_footEnd;
		}
	}
	else
	{
		m_footEnd = 239;
		m_volPre = 10e15;
	}

	m_sOldSymbol = symbol;
	m_sOldStkKind = pView->m_stkKind;


	if (m_footEnd < 0) m_footEnd = 0;
	if (m_footEnd > 239) m_footEnd = 0;


	CReportData* pS0=NULL;
	pS0 = m_pS0;
	CReportData* pS1=m_pS1;
	CReportData* pdtInInit=NULL;
	pdtInInit=m_pReportData;

	m_close=pdtInInit->ystc;

	if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SZ_MARKET_EX)
	{
		CReportData* pS = pS0;
		pS0 = pS1;
		pS1 = pS;
	}
	m_close_Dapan[0]=pS0->ystc;
	m_close_Dapan[1]=pS1->ystc;

	if(m_footEnd+1>m_klinNumDefault)
	{
		ASSERT(FALSE);
		return;
	}

	//
	bool bZhiShu=false;
	if(CTaiShanKlineShowView::IsIndexStock(s))
	{
		m_pFlg[0]=pView->m_infoInit.flag_dapan[0];
		m_pFlg[1]=pView->m_infoInit.flag_dapan[1];
		if(CTaiShanKlineShowView::IsIndexStock3(s))
		{
			m_pFlg[0]=FS_LINXIAN;
			m_pFlg[1]=FS_VOL;
			if(m_nTextBox==0)
				m_nTextBox=1;
		}
		m_pFlg[2]=pView->m_infoInit.flag_dapan[2];
		m_pFlg[3]=pView->m_infoInit.flag_dapan[3];
		m_pFlg[4]=pView->m_infoInit.flag_dapan[4];
		bZhiShu=true;
	}
	else
	{
		m_pFlg[0]=pView->m_infoInit.flag[0];
		m_pFlg[1]=pView->m_infoInit.flag[1];
		m_pFlg[2]=pView->m_infoInit.flag[2];
		m_pFlg[3]=pView->m_infoInit.flag[3];
		m_pFlg[4]=pView->m_infoInit.flag[4];

	}

	float high=pS0->higp  ;
	float lowDapan=pS0->lowp  ;


	if((pS0->m_Kdata1)->Price>0)
		*m_lineDapan[0][0]=(pS0->m_Kdata1)->Price;
	else
	{
		if(pS0->opnp >0)
			*m_lineDapan[0][0]=pS0->opnp;
		else
			*m_lineDapan[0][0]=pS0->ystc ;
	}
	for(int k=1;k<=m_footEnd;k++)
	{
		if(((pS0->m_Kdata1)+k)->Price>0)
			*(m_lineDapan[0][0]+k)=((pS0->m_Kdata1)+k)->Price;
		else
			*(m_lineDapan[0][0]+k)=*(m_lineDapan[0][0]+k-1);
	}
	*m_lineDapan[0][2]=(pS0->m_Kdata1)->Volume;
	for(int k=1;k<=m_footEnd;k++)
	{
		if(((pS0->m_Kdata1)+k)->Volume>pS0->totv && pS0->kind != SZZS)
			((pS0->m_Kdata1)+k)->Volume=((pS0->m_Kdata1)+k-1)->Volume;
		if(((pS0->m_Kdata1)+k)->Volume<((pS0->m_Kdata1)+k-1)->Volume)
			((pS0->m_Kdata1)+k)->Volume=((pS0->m_Kdata1)+k-1)->Volume;
		if(((pS0->m_Kdata1)+k)->Volume>0)
			*(m_lineDapan[0][2]+k)=((pS0->m_Kdata1)+k)->Volume
			-((pS0->m_Kdata1)+k-1)->Volume;
		else
		{
			((pS0->m_Kdata1)+k)->Volume=((pS0->m_Kdata1)+k-1)->Volume;
			*(m_lineDapan[0][2]+k)=0;
		}
	}
	//1
	if(bZhiShu)
	{
		if((pS1->m_Kdata1)->Price>0)
			*m_lineDapan[1][0]=(pS1->m_Kdata1)->Price;
		else
		{
			if(pS1->opnp >0)
				*m_lineDapan[1][0]=pS1->opnp;
			else
				*m_lineDapan[1][0]=pS1->ystc ;
		}
		for(int k=1;k<=m_footEnd;k++)
		{
			if(((pS1->m_Kdata1)+k)->Price>0)
				*(m_lineDapan[1][0]+k)=((pS1->m_Kdata1)+k)->Price;
			else
				*(m_lineDapan[1][0]+k)=*(m_lineDapan[1][0]+k-1);
		}
		*m_lineDapan[1][2]=(pS1->m_Kdata1)->Volume;
		for(int k=1;k<=m_footEnd;k++)
		{
			if(((pS1->m_Kdata1)+k)->Volume>pS1->totv && pS1->kind != SZZS)
				((pS1->m_Kdata1)+k)->Volume=((pS1->m_Kdata1)+k-1)->Volume;
			if(((pS1->m_Kdata1)+k)->Volume<((pS1->m_Kdata1)+k-1)->Volume)
				((pS1->m_Kdata1)+k)->Volume=((pS1->m_Kdata1)+k-1)->Volume;
			if(((pS1->m_Kdata1)+k)->Volume>0)
				*(m_lineDapan[1][2]+k)=((pS1->m_Kdata1)+k)->Volume
				-((pS1->m_Kdata1)+k-1)->Volume;
			else
			{
				((pS1->m_Kdata1)+k)->Volume	=((pS1->m_Kdata1)+k-1)->Volume;
				*(m_lineDapan[1][2]+k)=0;
			}
		}
	}



	s=pdtInInit->name;
	s=pView->m_sharesSymbol +s;
	int j=0;
	float* f0;

	f0=m_amount;

	*f0=(pdtInInit->m_Kdata1)->Amount;
	for(j=1;j<=m_footEnd;j++)
	{
		if((pdtInInit->m_Kdata1+j)->Amount<=0)
			(pdtInInit->m_Kdata1+j)->Amount=(pdtInInit->m_Kdata1+j-1)->Amount;
	}
	for(j=1;j<=m_footEnd;j++)
	{
		if((pdtInInit->m_Kdata1+j)->Amount>(pdtInInit->m_Kdata1+j-1)->Amount&&(pdtInInit->m_Kdata1+j)->Amount>0)
			*(f0+j)=(pdtInInit->m_Kdata1+j)->Amount-(pdtInInit->m_Kdata1+j-1)->Amount;
		else
		{
			(pdtInInit->m_Kdata1+j)->Amount=(pdtInInit->m_Kdata1+j-1)->Amount;
			*(f0+j)=0;
		}
	}


	f0=m_vol;
	*f0=(pdtInInit->m_Kdata1)->Volume;
	for(j=1;j<=m_footEnd;j++)
	{
		if((pdtInInit->m_Kdata1+j)->Volume<=0)
			(pdtInInit->m_Kdata1+j)->Volume=(pdtInInit->m_Kdata1+j-1)->Volume;
	}

	for(j=1;j<=m_footEnd;j++)
	{
		if(((pdtInInit->m_Kdata1)+j)->Volume>pdtInInit->totv && pdtInInit->kind != SZZS)
			((pdtInInit->m_Kdata1)+j)->Volume=((pdtInInit->m_Kdata1)+j-1)->Volume;
		if((pdtInInit->m_Kdata1+j)->Volume>(pdtInInit->m_Kdata1+j-1)->Volume&&(pdtInInit->m_Kdata1+j)->Volume>0)
			*(f0+j)=(pdtInInit->m_Kdata1+j)->Volume-(pdtInInit->m_Kdata1+j-1)->Volume;
		else
		{
			(pdtInInit->m_Kdata1+j)->Volume=(pdtInInit->m_Kdata1+j-1)->Volume;
			*(f0+j)=0;
		}
	}

	for(int i=0;i<pView->m_infoInit.nCountMin1;i++)
	{

		for(int k=0;k<6;k++)
		{
			m_dataFormular[i].line[k].bNow=0;
			m_dataFormular[i].line [k].type=0;
		}

		int tmp=*(m_pFlg+i);
		float maxLingXian=(float)-10E20;
		float minLingXian=(float)10E20;
		if(i == 0)
		{
			if(CTaiShanKlineShowView::IsIndexStock3(symbol)==0 && tmp == FS_LINXIAN)
				tmp = FS_ZOUSHI;
		}
		switch(tmp)
		{

		case FS_LINXIAN:
		case FS_ZOUSHI:
			m_nameSon[i]="分时走势";
			m_lineName[i][0]="分时走势";
			m_lineName[i][1]="均线";
			m_dataFormular[i].numLine=2;

			f0=m_dataFormular[i].line [0].m_arrBE.line;
			if((pdtInInit->m_Kdata1)->Price<=0)
			{
				if(pdtInInit->opnp >0)
					*f0=pdtInInit->opnp;
				else
					*f0=m_close;
			}
			else
				*f0=(pdtInInit->m_Kdata1)->Price;
			for(j=1;j<=m_footEnd;j++)
			{
				if((pdtInInit->m_Kdata1+j)->Price>0)
					*(f0+j)=(pdtInInit->m_Kdata1+j)->Price;
				else
					*(f0+j)=*(f0+j-1);
			}

			if(bZhiShu==true)
			{

				f0=m_dataFormular[i].line [1].m_arrBE.line;
				f0[0] = m_dataFormular[i].line [0].m_arrBE.line[0];
				for (j =1;j <=m_footEnd;j++)
				{
					*(f0+j)=(f0[j-1]*j + m_dataFormular[i].line [0].m_arrBE.line[j])/(j+1);
				}
				break;
			}

			{
				bool b1 = true;
				f0=m_dataFormular[i].line [1].m_arrBE.line;
				f0[0] = m_dataFormular[i].line [0].m_arrBE.line[0];
				if((pdtInInit->m_Kdata1)->Volume > 0.01 )
					f0[0] = (pdtInInit->m_Kdata1)->Amount /((pdtInInit->m_Kdata1)->Volume *100);
				float totp = (pdtInInit->m_Kdata1)->Amount;
				int rate = 100;
				if(pdtInInit->totv>0 &&pdtInInit->lowp>0)
				{
					if(pdtInInit->totp/pdtInInit->totv/pdtInInit->lowp>1000)
						rate = 1000;
				}
				for (j =0;j <=m_footEnd;j++)
				{
					float f = 0;
					if(m_vol[j]>0)
					{
						f = m_amount[j]/m_vol[j]/rate;
						if(f > pdtInInit->higp || f < pdtInInit->lowp)
						{
							b1 = false;
							break;
						}
					}
				}
				if(b1)
				{
					for (j =1;j <=m_footEnd;j++)
					{

						{
							if((pdtInInit->m_Kdata1+j)->Volume>0.01)
							{
								float f = 0;
								pView->GetPriceAve(f,((pdtInInit->m_Kdata1+j)->Volume),(pdtInInit->m_Kdata1+j)->Amount);//(pdtInInit->m_Kdata1+j)->Amount /((pdtInInit->m_Kdata1+j)->Volume);
								*(f0+j)= f;
								if(*(f0+j)<0.01)
									*(f0+j) = *(f0+j-1);
							}
							else
								*(f0+j)=*(f0+j-1);
						}
						if(*(f0+j) > pdtInInit->higp || *(f0+j) < pdtInInit->lowp)
							*(f0+j)=*(f0+j-1);
					}
				}
				else
				{
					for (j =1;j <=m_footEnd;j++)
					{
						totp+=m_vol[j]*((pdtInInit->m_Kdata1+j)->Price)*rate;
						{
							if((pdtInInit->m_Kdata1+j)->Volume>0.01)
							{
								float f = 0;
								pView->GetPriceAve(f,((pdtInInit->m_Kdata1+j)->Volume),totp);//(pdtInInit->m_Kdata1+j)->Amount /((pdtInInit->m_Kdata1+j)->Volume);
								*(f0+j)= f;
								if(*(f0+j)<0.01)
									*(f0+j) = *(f0+j-1);
							}
							else
								*(f0+j)=*(f0+j-1);
						}
						if(*(f0+j) > pdtInInit->higp || *(f0+j) < pdtInInit->lowp)
							*(f0+j)=*(f0+j-1);
					}
				}

			}
			break;
		case FS_VOL:
			m_nameSon[i]="";
			m_lineName[i][0]="成交量";
			m_dataFormular[i].numLine=1;

			f0=m_dataFormular[i].line [0].m_arrBE.line; 
			*f0=m_vol[0];
			for(j=1;j<=m_footEnd;j++)
			{
				*(f0+j)=m_vol[j];
			}
			break;
		case FS_MMLD:
			m_nameSon[i]="买卖力道";
			m_lineName[i][0]="买入";
			m_lineName[i][1]="卖出";
			m_lineName[i][2]="买卖差";
			m_dataFormular[i].numLine=0;

			break;
		case FS_LB:
			m_nameSon[i]="量比指标";
			m_lineName[i][0]="量比";
			m_dataFormular[i].numLine=1;


			f0=m_dataFormular[i].line [0].m_arrBE.line; 
			if((pdtInInit->volume5 )>0)
				*f0=(pdtInInit->m_Kdata1)->Volume*240/(pdtInInit->volume5 );
			else
				*f0=0;
			if((pdtInInit->volume5 )>0)
			{
				for(j=1;j<=m_footEnd;j++)
				{
					if((pdtInInit->m_Kdata1+j)->Volume>0)
						*(f0+j)=(pdtInInit->m_Kdata1+j)->Volume*240/(j+1)/(pdtInInit->volume5 );
					else
						*(f0+j)=*(f0+j-1);
				}
			}
			else
			{
				memset(f0,0,4*(m_footEnd+1));
			}
			break;

		case FS_DUOKONG:
			m_nameSon[i]="多空指标";
			m_lineName[i][0]="多空";
			m_dataFormular[i].numLine=1;


			f0=m_dataFormular[i].line [0].m_arrBE.line;

			{
				for(j=0;j<=m_footEnd;j++)
				{
					int r=0;
					r=(m_Tidx [isSz][j].rp  -m_Tidx [isSz][j].dp);
					*(m_dataFormular[i].line [0].m_arrBE.line+j)=(float)r;
				}
			}

			break;
		case FS_TONGLUO:
			m_nameSon[i]="腾落指数";
			m_lineName[i][0]="腾落指数";
			m_dataFormular[i].numLine=1;

			{
				*(m_dataFormular[i].line [0].m_arrBE.line)= (m_Nidx [isSz][0].rsn -m_Nidx [isSz][0].dnn );
				for(j=1;j<=m_footEnd;j++)
				{
					*(m_dataFormular[i].line [0].m_arrBE.line+j)=*(m_dataFormular[i].line [0].m_arrBE.line+j-1) + (m_Nidx [isSz][j].rsn -m_Nidx [isSz][j].dnn );
				}
			}

			break;
		}

		if(i==0 && m_nKlineCurrent>0)
		{
			for(int nStk=0;nStk<m_nKlineCurrent;nStk++)
				AddFenshiAdded(m_symbolAdd[nStk],m_stkKindAdd[nStk],nStk+1);
		}
	}
	m_nameSon[0]=s;

}



void CTaiKlineMin1::DrawSon(CDC *pDC)
{
	SetRectDraw(m_nSon);

	if(m_nSon==0 && m_nKlineCurrent>0 && m_nKlineCurrent<6)
		CaclMaxAdded();
	for(int i=0;i<m_dataFormular[m_nSon].numLine ;i++)
	{
		CaclMaxFlt(m_dataFormular[m_nSon].line [i].m_arrBE.line,
			m_footBegin);
	}

	if(m_close<=0)
	{
		CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]);


		DrawCapt(pDC);
		return;
	}

	if(m_pFlg[m_nSon]==FS_ZOUSHI||m_pFlg[m_nSon]==FS_LINXIAN)
	{

		float cls;
		if(m_pFlg[m_nSon]==FS_ZOUSHI)
			cls=m_close;
		else
			cls=m_close_Dapan[0] ;

		float maxPrice=(float)(FABSMY(m_max_sun[m_nSon]-cls)>FABSMY(m_min_sun[m_nSon]-cls)?
			FABSMY(m_max_sun[m_nSon]-cls):FABSMY(m_min_sun[m_nSon]-cls));
		m_max_sun[m_nSon]=maxPrice+cls;


		m_min_sun[m_nSon]=cls-maxPrice;


		if(cls>0)
		{
			CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]);  
			CPen* pOldpen=pDC->SelectObject(&pen_line);
			int y=YTransfer((float)(cls));
			pDC->MoveTo(m_rectDrawLine.left,y);
			pDC->LineTo(m_rectDrawLine.right+1,y);
			pDC->MoveTo(m_rectDrawLine.left,y-1);
			pDC->LineTo(m_rectDrawLine.right+1,y-1);
			pDC->SelectObject(pOldpen);
		}
	}

	if(m_pFlg[m_nSon]==FS_VOL)
	{
		if(m_min_sun[m_nSon]>0)
			m_min_sun[m_nSon]=0;

	}


	if(m_pFlg[m_nSon]==FS_ZOUSHI||m_pFlg[m_nSon]==FS_LINXIAN)
		DrawRulorText(pDC,2);
	else
		DrawRulorText(pDC,3);


	DrawCapt(pDC);

	DrawLineIndex(pDC,true);

}



void CTaiKlineMin1::DrawCapt(CDC *pDC)
{
	if(m_pReportData==NULL)
		return;
	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	pDC->SetTextAlign( TA_LEFT );
	pDC->SetBkMode(TRANSPARENT);


	int left=m_rectDrawLine.left;

	int len=(pDC->GetOutputTextExtent(m_nameSon[m_nSon])).cx;
	int move=-1;
	int lf=4;

	CRect rtFill(pView->m_midLen +1,0,pView->m_rtMin1.rightX-1,0);
	if(m_nSon==0)
		rtFill.top=0;
	else
		rtFill.top=pView->m_rtMin1.rtBlw[m_nSon-1].m_yBottom+1;
	rtFill.bottom=rtFill.top+pView->m_heightCaption-2;
	pView->ClearBack (pDC,rtFill);


	pDC->SetTextColor( pDoc->m_colorArray [1]);
	pDC->TextOut(m_rectDrawLine.left+lf,m_rectDrawLine.top-pView->m_heightCaption-move,m_nameSon[m_nSon]);
	CString paras="<";
	pDC->ExtTextOut (left+len+lf,m_rectDrawLine.top-pView->m_heightCaption-move,ETO_CLIPPED,rtFill,paras,NULL);
	len+=(pDC->GetOutputTextExtent(paras)).cx;


	bool bDrawFiguer=true;
	if(m_nSon==0 && m_nKlineCurrent>0 && m_nKlineCurrent<6)
	{
		m_lineName[0][0]=m_pReportData->name ;
		for(int j=0;j<m_nKlineCurrent;j++)
		{
			m_lineName[0][j+1]=m_nameAdd[j];
		}
		bDrawFiguer=false;
	}

	int num=m_dataFormular[m_nSon].numLine;
	int nFoot=0;
	if(pView->m_isShowCross == 1)
		nFoot = m_footCurrent;
	else
		nFoot = m_footEnd;
	for(int i=0;i<num;i++)
	{ 

		CString pnm=m_lineName[m_nSon][i];
		pDC->SetTextColor( pDoc->m_colorArray [7+i]);


		pDC->ExtTextOut (left+len+lf,m_rectDrawLine.top-pView->m_heightCaption-move,ETO_CLIPPED,rtFill,pnm,NULL);
		len+=(pDC->GetOutputTextExtent(pnm)).cx;
		CString vl="";
		if(bDrawFiguer==true)
		{
			vl.Format (" %.3f",m_dataFormular[m_nSon].line[i].m_arrBE.line[nFoot]);
		}
		if(num>1)
		{
			if(i<num-1)
				vl+=" ";
		}
		pDC->ExtTextOut (left+len+lf,m_rectDrawLine.top-pView->m_heightCaption-move,ETO_CLIPPED,rtFill,vl,NULL);
		len+=(pDC->GetOutputTextExtent(vl)).cx;
	}
	pDC->SetTextColor(  pDoc->m_colorArray [1]);
	pDC->ExtTextOut (left+len+lf,m_rectDrawLine.top-pView->m_heightCaption-move,ETO_CLIPPED,rtFill,">",NULL);

	pDC->SelectObject(pOld);


	if(pView->m_isShowCross == 1 && m_nSon == 0)
	{
		CTaiKlineDlgDealHistory::GetBuySellInfoPerFoot(pView->m_sharesSymbol , m_pReportData, nFoot, m_hsMin1 , false);
	}

}



void CTaiKlineMin1::InitHs(bool bRemoveAll,bool bSkip)
{

	if(m_bHist == true && bRemoveAll==false)
		return;
	if(bSkip == true &&  bRemoveAll==false)
		return;

	if(this->m_pReportData!=NULL && bSkip == false)
	{
		if(this->m_pReportData->ystc ==0)
		{
			pView->RemoveHs(0);
			pView->RemoveHs(1);
			if(pView->m_tabNum<4)
				pView->m_nBeginHS=0;
			return;
		}
	}

	long addr = 0;
	short Curr_Min;

	Curr_Min=m_pFileHs->GetDataCount(pView->m_sharesSymbol);

	int first=0;
	if(bRemoveAll==true)
	{

		pView->RemoveHs(1);

		if(pView->m_tabNum<4)
			pView->m_nBeginHS=0;
		if(pView->m_tabNum==0)
			pView->m_nBeginHS = -1;
	}
	else
	{
		first=pView->m_hs.GetCount();
		if(first>=Curr_Min)
		{
			return;
		}
	}


	if(Curr_Min<0||Curr_Min>=480*16)
	{
		return;
	}





	if(bSkip == false)
#ifndef WIDE_NET_VERSION
		m_pFileHs->ReadHS (pView->m_sharesSymbol ,pView->m_hs,bRemoveAll);
#else
		m_pFileHs->ReadHS2 (pView->m_sharesSymbol ,pView->m_hs,bRemoveAll);
#endif
	else
		m_pFileHs->ReadHS2 (pView->m_sharesSymbol ,pView->m_hs,bRemoveAll);


	int nCount = pView->m_hs.GetCount();

	GetFenJia(first,pView->m_hs, pView->m_fenjia, pView->m_fenjia);

	if(bRemoveAll == true || m_bHist == false )
		CTaiKlineFileHS::TransferHsToMin1(pView->m_hs,m_hsMin,240);
}




int CTaiKlineMin1::DrawHs(CDC *pDC,int nBegin,BYTE flag)
{
	if(m_pReportData==NULL)
		return 0;
	pDC->SetTextAlign( TA_RIGHT);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[1];

	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	int modify=0;

	float cls;
	cls=m_pReportData->ystc;


	CRect rt;
	rt.top=0;
	rt.left=0;
	int nm=pView->m_infoInit.nCountMin1;
	if(pView->m_nKlineKind2==0)
	{
		if(pView->m_infoFiguer >0)
			rt.right=pView->m_rtMin1.rightX+ pView->m_midLen + pView->m_rightLength ;
		else
			rt.right=pView->m_rtMin1.rightX+ pView->m_midLen ;
		rt.bottom=pView->m_rtMin1.rtBlw[nm-1].m_yBottom+pView->m_heightCaption;
	}
	else
	{
		nm=pView->m_infoInit.nCountIndex;
		if(pView->m_infoFiguer >0)
			rt.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen + pView->m_rightLength ;
		else
			rt.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen ;
		rt.bottom=pView->m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+pView->m_heightCaption;
	}

	int top=19*RIGHTBOX_PERLINE+1;
	int bottom=rt.bottom-24;

	if(flag==0)
	{
		int left1=rt.right-pView->m_rightLength+41;
		int left2=rt.right-pView->m_rightLength+90;
		int left3=rt.right-16;
		if(bottom<top)
		{
			pDC->SelectObject(pOld);
			return 0; 
		}


		int nShow=(int)((bottom-top)/RIGHTBOX_PERLINE+0.2);

		int nHs=pView->m_hs.GetCount();

		if(nBegin>nHs-1)
		{
			pDC->SelectObject(pOld);
			return 0; 
		}

		if(nBegin <= -1)
		{
			if(nHs>nShow)
				nBegin = nHs-nShow;
			else
				nBegin = 0;
		}


		pView->m_scrollBar->SetScrollRange(0,nHs);

		pView->m_scrollBar->SetScrollPos(nBegin);

		for(int j=nBegin;j<nHs;j++)
		{
			if(j-nBegin+1>nShow)
				break;
			POSITION pos=pView->m_hs.FindIndex(nHs - j - 1 ) ;
			TRADE_DETAIL_H_PER* pHs=pView->m_hs.GetAt( pos );
			TRADE_DETAIL_H_PER* pHsPre = NULL;
			if(j>0)
			{
				pos=pView->m_hs.FindIndex(nHs - j  ) ;
				pHsPre=pView->m_hs.GetAt( pos );
			}

			CString s;
			float vl=pHs->price;
			int floatBit = 2;
			if( CSharesCompute::GetMarketKind(pView->m_stkKind ) == SH_MARKET_EX  && m_pReportData->kind == 2)
				floatBit =3 ;
			if(floatBit == 2)
				s.Format("%.2f",vl);
			else
				s.Format("%.3f",vl);
			if(vl>cls)
				pDC->SetTextColor( pDoc->m_colorArray[13]);
			else if(vl<cls)
				pDC->SetTextColor( pDoc->m_colorArray[XIA_DIE_WORD]);
			else
				pDC->SetTextColor( pDoc->m_colorArray[1]);
			pDC->TextOut(left2,top+RIGHTBOX_PERLINE*(j-nBegin)+modify,s);


			int nFlag ;
			float fVol = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
			CString sArrow="↑"  ;
			if(nFlag>0)
				pDC->SetTextColor( pDoc->m_colorArray[13]);
			else if(nFlag<0)
			{
				pDC->SetTextColor( pDoc->m_colorArray[XIA_DIE_WORD]);
				sArrow="↓";
			}
			else
			{
				pDC->SetTextColor( pDoc->m_colorArray[1]);
				sArrow=" * ";
			}
			s.Format("%d",(int)(fVol+0.5));
			s=s+sArrow;

			pDC->TextOut(left3,top+RIGHTBOX_PERLINE*(j-nBegin)+modify,s);

			pDC->SetTextColor( pDoc->m_colorArray[1]);
			CTime tm((time_t)pHs->time );
			s=tm.Format ("%H:%M");
			;
			pDC->TextOut(left1,top+RIGHTBOX_PERLINE*(j-nBegin)+modify,s);
		}
	}
	else
	{
		int left1=rt.right-pView->m_rightLength+41;
		int left2=rt.right-pView->m_rightLength+90;
		int left3=rt.right-22;
		if(bottom<top)
		{
			pDC->SelectObject(pOld);
			return 0; 
		}


		int nShow=(int)((bottom-top)/RIGHTBOX_PERLINE+0.2);

		int nHs=pView->m_fenjia.GetCount();

		pView->m_scrollBar->SetScrollRange(0,nHs);

		if(nBegin<0)
			nBegin =0;

		pView->m_scrollBar->SetScrollPos(nBegin);

		if(nBegin>nHs-1||nHs==0)
		{
			pDC->SelectObject(pOld);
			return 0; 
		}

		float max=0;
		POSITION pos;
		FENJIA* pf;
		if( ( pos = pView->m_fenjia.GetHeadPosition() ) == NULL )
			return 0;
		for(int j=0;j<nHs;j++)
		{
			pf=pView->m_fenjia.GetNext(pos);
			if(pf->vol >max )
			{
				max=pf->vol;
			}
		}



		if(nBegin<0)
			nBegin=0;
		for(int j=nBegin;j<nHs;j++)
		{
			if(j-nBegin+1>nShow)
				break;
			pos=pView->m_fenjia.FindIndex( j ) ;
			pf=pView->m_fenjia.GetAt( pos );

			CString s;
			float vl=pf->price;
			int floatBit = 2;
			if(CSharesCompute::GetMarketKind(pView->m_stkKind ) == SH_MARKET_EX  && m_pReportData->kind == 2)
				floatBit =3 ;
			if(floatBit == 2)
				s.Format("%.2f",vl);
			else
				s.Format("%.3f",vl);

			int nBrs;
			if(vl>cls)
			{
				pDC->SetTextColor( pDoc->m_colorArray[13]);
				nBrs=0;
			}
			else if(vl<cls)
			{
				pDC->SetTextColor( pDoc->m_colorArray[XIA_DIE_WORD]);
				nBrs=2;
			}
			else
			{
				pDC->SetTextColor( pDoc->m_colorArray[1]);
				nBrs=1;
			}


			pDC->TextOut(left1,top+RIGHTBOX_PERLINE*(j-nBegin)+modify,s);

			s.Format("%d",(int)pf->vol);
			pDC->TextOut(left2,top+RIGHTBOX_PERLINE*(j-nBegin)+modify,s);

			CBrush brush_w;
			if (!brush_w.CreateSolidBrush(pDoc->m_colorArray[13+nBrs]))
				ASSERT(FALSE);
			;
			CBrush* pOldBrush=pDC->SelectObject(&brush_w);
			if(max>0)
			{
				CRect rt((int)(left3-pf->vol*20/max),top+RIGHTBOX_PERLINE*(j-nBegin)+modify+4,
					left3,top+RIGHTBOX_PERLINE*(j-nBegin)+modify+14);
				pDC->FillRect(rt,&brush_w);
			}
			pDC->SelectObject(pOldBrush);
		}
	}
	pDC->SelectObject(pOld);

	return nBegin;


}



void CTaiKlineMin1::DrawDapan(CDC *pDC,int nMarket,int flagFrame)
{

	int tempNum=m_nSon;
	m_nSon=0;
	DrawLittleFrame(pDC,flagFrame);


	num_sun=0;
	m_max_dapan[nMarket][num_sun]=(float)-9.0E20;
	m_min_dapan[nMarket][num_sun]=(float)9.0E20;

	float* pFlt;
	for(int j=0;j<1 ;j++)
	{
		pFlt=m_lineDapan[nMarket][j];
		for(int i=0;i<=m_footEnd;i++)
		{
			if(*(pFlt+i)>m_max_dapan[nMarket][num_sun])
				m_max_dapan[nMarket][num_sun]=*(pFlt+i);
			if(*(pFlt+i)<m_min_dapan[nMarket][num_sun])
				m_min_dapan[nMarket][num_sun]=*(pFlt+i);
		}
	}

	float maxPrice=(float)(FABSMY(m_max_dapan[nMarket][num_sun]-m_close_Dapan[nMarket])>FABSMY(m_min_dapan[nMarket][num_sun]-m_close_Dapan[nMarket])?
		FABSMY(m_max_dapan[nMarket][num_sun]-m_close_Dapan[nMarket]):FABSMY(m_min_dapan[nMarket][num_sun]-m_close_Dapan[nMarket]));
	m_max_dapan[nMarket][num_sun]=maxPrice+m_close_Dapan[nMarket];


	m_min_dapan[nMarket][num_sun]=m_close_Dapan[nMarket]-maxPrice;


	int nFlags2=m_pFlg[m_nSon];
	m_pFlg[m_nSon]=FS_LINXIAN;
	float tempMax=m_max_sun[m_nSon];
	float tempMin=m_min_sun[m_nSon];
	m_max_sun[m_nSon]=m_max_dapan[nMarket][num_sun];
	m_min_sun[m_nSon]=m_min_dapan[nMarket][num_sun];

	if(m_max_sun[m_nSon]==m_min_sun[m_nSon])
	{
		m_max_sun[m_nSon]=tempMax;
		m_min_sun[m_nSon]=tempMin;
		m_pFlg[m_nSon] = nFlags2;
		m_nSon=tempNum;
		return;
	}


	DrawRulorText(pDC,1);
	m_max_dapan[nMarket][num_sun]=m_max_sun[m_nSon];
	m_min_dapan[nMarket][num_sun]=m_min_sun[m_nSon];

	m_pFlg[m_nSon]=nFlags2;
	m_nSon=tempNum;
	m_max_sun[0]=tempMax;
	m_min_sun[0]=tempMin;


	DrawLittleDapan(pDC,0,nMarket);

	num_sun=1;
	m_max_dapan[nMarket][num_sun]=(float)-9.0E20;
	m_min_dapan[nMarket][num_sun]=(float)9.0E20;

	pFlt=m_lineDapan[nMarket][2];
	for(int i=0;i<=m_footEnd;i++)
	{
		if(*(pFlt+i)<0)
			*(pFlt+i)=0;
		if(*(pFlt+i)>m_max_dapan[nMarket][num_sun])
			m_max_dapan[nMarket][num_sun]=*(pFlt+i);
		if(*(pFlt+i)<m_min_dapan[nMarket][num_sun])
			m_min_dapan[nMarket][num_sun]=*(pFlt+i);
	}

	m_min_dapan[nMarket][num_sun]=0;
	tempNum=m_nSon;
	m_nSon=1;
	nFlags2=m_pFlg[m_nSon];
	m_pFlg[m_nSon]=FS_VOL;
	tempMax=m_max_sun[m_nSon];
	tempMin=m_min_sun[m_nSon];
	m_max_sun[m_nSon]=m_max_dapan[nMarket][num_sun];
	m_min_sun[m_nSon]=m_min_dapan[nMarket][num_sun];

	if(m_max_sun[m_nSon]==m_min_sun[m_nSon])
	{
		m_max_sun[m_nSon]=tempMax;
		m_min_sun[m_nSon]=tempMin;
		m_pFlg[m_nSon] = nFlags2;
		m_nSon=tempNum;
		return;
	}
	DrawLittleFrame(pDC,flagFrame);
	DrawRulorText(pDC,1);
	m_pFlg[m_nSon]=nFlags2;
	m_nSon=tempNum;
	m_max_sun[1]=tempMax;
	m_min_sun[1]=tempMin;

	DrawLittleDapan(pDC,2,nMarket);



}



void CTaiKlineMin1::DrawLittleGegu(CDC *pDC)
{
	int flagDoc=pView->m_infoInit.flag[1];
	pView->m_infoInit.flag[1]=1;
	InitMinuteLine();

	int n1 = m_dataFormular[0].numLine;
	m_dataFormular[0].numLine = 1;
	for(int i=0;i<2;i++)
	{
		m_nSon=i;
		m_max_sun[m_nSon]=(float)-9.0E20;
		m_min_sun[m_nSon]=(float)9.0E20;

		DrawLittleFrame(pDC);

		for(int i=0;i<m_dataFormular[m_nSon].numLine ;i++)
		{
			CaclMaxFlt(m_dataFormular[m_nSon].line [i].m_arrBE.line,
				m_footBegin);
		}
		if(m_pFlg[m_nSon]==FS_ZOUSHI||m_pFlg[m_nSon]==FS_LINXIAN)
		{
			if(m_close>0)
			{
				float cls;
				cls=m_close;

				float maxPrice=(float)(FABSMY(m_max_sun[m_nSon]-cls)>FABSMY(m_min_sun[m_nSon]-cls)?
					FABSMY(m_max_sun[m_nSon]-cls):FABSMY(m_min_sun[m_nSon]-cls));
				m_max_sun[m_nSon]=maxPrice+cls;
				if(cls-maxPrice<=0)
				{
					m_dataFormular[0].numLine = n1;
					return;
				}

				m_min_sun[m_nSon]=cls-maxPrice;


				if(m_close>0)
				{
					int y=YTransfer((float)(m_close));
					if(y>=pView->m_heightCaption )
					{
						CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]);  
						CPen* pOldpen=pDC->SelectObject(&pen_line);
						pDC->MoveTo(m_rectDrawLine.left,y);
						pDC->LineTo(m_rectDrawLine.right+1,y);
						pDC->MoveTo(m_rectDrawLine.left,y-1);
						pDC->LineTo(m_rectDrawLine.right+1,y-1);
						pDC->SelectObject(pOldpen);
					}
				}

			}
		}
		if(m_pFlg[m_nSon]==FS_VOL)
		{
			if(m_min_sun[m_nSon]>0)
				m_min_sun[m_nSon]=0;

		}

		if(m_max_sun[m_nSon]==m_min_sun[m_nSon])
			continue;

		DrawRulorText(pDC,1);

		DrawLineIndex(pDC,true);
	}
	pView->m_infoInit.flag[1]=flagDoc;

	m_dataFormular[0].numLine = n1;


}



void CTaiKlineMin1::DrawLittleDapan(CDC *pDC,int flagLine,int nMarket)

{
	int leftf=m_rectDrawLine.left;
	int m_high=m_rectDrawLine.top;
	int low=m_rectDrawLine.bottom;
	int rightf=m_rectDrawLine.right;

	if(m_footEnd<m_footBegin)
		return ;
	float cellWidth=(float)(rightf-leftf)/(m_klinNumDefault);



	float tempY=0;
	CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]);
	CPen* pOldpen=pDC->SelectObject(&pen_line);
	if(flagLine<2) 
	{
		int y;

		if((m_max_dapan[nMarket][0]-m_min_dapan[nMarket][0])==0)
			tempY=0;
		else
			tempY=((m_rectDrawLine.top-m_rectDrawLine.bottom)
			/(m_max_dapan[nMarket][0]-m_min_dapan[nMarket][0]));
		y=(int)(tempY*(m_close_Dapan[nMarket]-m_min_dapan[nMarket][0])+m_rectDrawLine.bottom);
		pDC->MoveTo(m_rectDrawLine.left,y);
		pDC->LineTo(m_rectDrawLine.right+1,y);
		pDC->MoveTo(m_rectDrawLine.left,y-1);
		pDC->LineTo(m_rectDrawLine.right+1,y-1);
	}


	COLORREF	clr;
	switch(flagLine)
	{
	case 0:
		clr=( pDoc->m_colorArray [7]);
		break;
	case 1:
		clr=( pDoc->m_colorArray [8]);
		break;
	case 2:
		clr=( pDoc->m_colorArray [7]);
		break;
	}
	CPen pen;
	if (!pen.CreatePen(PS_SOLID,1,clr))
		return;
	pDC->SelectObject(&pen);

	if(flagLine==0) 
	{
		float j1=m_lineDapan[nMarket][flagLine][0];
		pDC->MoveTo((int)(leftf+cellWidth/2),(int)(tempY*(j1-m_min_dapan[nMarket][flagLine])+m_rectDrawLine.bottom));
		for(int j=0; j<=m_footEnd; j++ )
		{
			pDC->LineTo((int)(leftf+cellWidth/2+(j-m_footBegin)*cellWidth),
				(int)(tempY*(m_lineDapan[nMarket][flagLine][j]-m_min_dapan[nMarket][flagLine])+m_rectDrawLine.bottom));
		}
	}
	else if(flagLine==2)
	{
		if((m_max_dapan[nMarket][1]-m_min_dapan[nMarket][1])>0)
		{
			if((m_max_dapan[nMarket][1]-m_min_dapan[nMarket][1])==0)
				tempY=0;
			else
				tempY=((m_rectDrawLine.top-m_rectDrawLine.bottom)
				/(m_max_dapan[nMarket][1]-m_min_dapan[nMarket][1]));
			for(int j=0; j<=m_footEnd; j++ )
			{
				pDC->MoveTo((int)(leftf+cellWidth/2+(j)*cellWidth),
					(int)(tempY*(m_lineDapan[nMarket][flagLine][j]-m_min_dapan[nMarket][1])+m_rectDrawLine.bottom));
				pDC->LineTo((int)(leftf+cellWidth/2+(j)*cellWidth),
					(int)(m_rectDrawLine.bottom));
			}
		}
	}
	pDC->SelectObject(pOldpen);



}



void CTaiKlineMin1::DrawLittleFrame(CDC *pDC,int nMarket)
{
	if(nMarket == 0)
	{
		CRect rt;
		rt.top=0;
		rt.left=0;
		int nm=pView->m_infoInit.nCountMin1;
		if(pView->m_nKlineKind2==0)
		{
			if(pView->m_infoFiguer >0)
				rt.right=pView->m_rtMin1.rightX+ pView->m_midLen + pView->m_rightLength ;
			else
				rt.right=pView->m_rtMin1.rightX+ pView->m_midLen ;
			rt.bottom=pView->m_rtMin1.rtBlw[nm-1].m_yBottom+pView->m_heightCaption;
		}
		else
		{
			nm=pView->m_infoInit.nCountIndex;
			if(pView->m_infoFiguer >0)
				rt.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen + pView->m_rightLength ;
			else
				rt.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen ;
			rt.bottom=pView->m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+pView->m_heightCaption;
		}


		int i=0;

		m_rectDrawLine.left=rt.right-pView->m_rightLength+2;
		m_rectDrawLine.right=rt.right-m_midLen_L;

		int top = 19*RIGHTBOX_PERLINE+8;
		int bott = rt.bottom-30;
		if(m_nSon==0)
		{
			m_rectDrawLine.top=top;//--
			m_rectDrawLine.bottom=(int)(top+0.618
				*(bott-top)-1);//--
		}
		else
		{
			m_rectDrawLine.top=(int)(top+0.618
				*(bott-top)+1);
			m_rectDrawLine.bottom=bott;
		}


		CPen pen;
		if (!pen.CreatePen(PS_SOLID,1,pDoc->m_colorArray[2]))
			return;
		CPen* pOldpen=pDC->SelectObject(&pen);

		pDC->MoveTo (m_rectDrawLine.left,top);
		pDC->LineTo (m_rectDrawLine.right,top);
		pDC->LineTo (m_rectDrawLine.right,bott);
		pDC->LineTo (m_rectDrawLine.left,bott);
		pDC->LineTo (m_rectDrawLine.left,top);

		pDC->MoveTo (m_rectDrawLine.left,(int)(top+0.618
			*(bott-top)));
		pDC->LineTo (m_rectDrawLine.right,(int)(top+0.618
			*(bott-top)));

		pDC->SelectObject(pOldpen);
	}
	else
	{
		CRect rt;
		rt.top=0;
		rt.left=0;
		int nm=pView->m_infoInit.nCountMin1;
		if(pView->m_nKlineKind2==0)
		{
			if(pView->m_infoFiguer >0)
				rt.right=pView->m_rtMin1.rightX+ pView->m_midLen + pView->m_rightLength ;
			else
				rt.right=pView->m_rtMin1.rightX+ pView->m_midLen ;
			rt.bottom=pView->m_rtMin1.rtBlw[nm-1].m_yBottom+pView->m_heightCaption-30;
		}
		else
		{
			nm=pView->m_infoInit.nCountIndex;
			if(pView->m_infoFiguer >0)
				rt.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen + pView->m_rightLength ;
			else
				rt.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen ;
			rt.bottom=pView->m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+pView->m_heightCaption-30;
		}


		int i=0;

		m_rectDrawLine.left=rt.right-pView->m_rightLength+2;
		m_rectDrawLine.right=rt.right-m_midLen_L;

		if(m_nSon==0)
		{
			m_rectDrawLine.top=18*RIGHTBOX_PERLINE+7;//--
			m_rectDrawLine.bottom=(int)(18*RIGHTBOX_PERLINE+7+0.618
				*(rt.bottom-m_rectDrawLine.top-7));//--
		}
		else
		{
			m_rectDrawLine.top=(int)(18*RIGHTBOX_PERLINE+7+0.618
				*(rt.bottom-18*RIGHTBOX_PERLINE-7-5)+2);//--
			m_rectDrawLine.bottom=rt.bottom-5;//--
		}


		CPen pen;
		if (!pen.CreatePen(PS_SOLID,1,pDoc->m_colorArray[2]))
			return;
		CPen* pOldpen=pDC->SelectObject(&pen);

		pDC->MoveTo (m_rectDrawLine.left,18*RIGHTBOX_PERLINE+7);
		pDC->LineTo (m_rectDrawLine.right,18*RIGHTBOX_PERLINE+7);
		pDC->LineTo (m_rectDrawLine.right,rt.bottom-5);//--
		pDC->LineTo (m_rectDrawLine.left,rt.bottom-5);//--
		pDC->LineTo (m_rectDrawLine.left,18*RIGHTBOX_PERLINE+7);



		pDC->MoveTo (m_rectDrawLine.left,(int)(18*RIGHTBOX_PERLINE+7+0.618
			*(rt.bottom-18*RIGHTBOX_PERLINE-7-5)+2));
		pDC->LineTo (m_rectDrawLine.right,(int)(18*RIGHTBOX_PERLINE+7+0.618
			*(rt.bottom-18*RIGHTBOX_PERLINE-7-5)+2));

		pDC->SelectObject(pOldpen);

		CString s9;
		if(pView->m_nKlineKind2 == 0)
		{
			if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SH_MARKET_EX)
				s9 = "深圳成分指数";
			else
				s9 = "上证指数";
		}
		else
		{
			if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SH_MARKET_EX)
				s9 = "上证指数";
			else
				s9 = "深圳成分指数";
		}

		pDC->SetTextAlign( TA_LEFT );
		pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
		pDC->SetBkMode(TRANSPARENT);

		CFont ft2;
		LOGFONT lgf2=pDoc->m_fontstr[0];
		lgf2.lfHeight=-14;
		lgf2.lfWeight =FW_BOLD;
		ft2.CreateFontIndirect (&lgf2 );
		CFont* pOld=pDC->SelectObject(&ft2);

		pDC->TextOut(m_rectDrawLine.left+2 , 17*RIGHTBOX_PERLINE +4, s9);

		pDC->SelectObject(pOld);
	}

}


CString CTaiKlineMin1::TimeToString(int foot,bool bRight)
{
	CString s;
	if(foot<-1)
		return s;

	int isSz=0;
	if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SZ_MARKET_EX) isSz=1;

	int numMorning=(pDoc->m_nDel_End_A_hr[isSz]-pDoc->m_nDel_Start_A_hr[isSz])*60+pDoc->m_nDel_End_A_min[isSz]-pDoc->m_nDel_Start_A_min[isSz];
	CTime tm=CTime::GetCurrentTime ();

	int nMove = 0;

	if(foot<numMorning-nMove)
	{
		CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay ()
			,(pDoc->m_nDel_Start_A_hr[isSz]*60+pDoc->m_nDel_Start_A_min[isSz]+foot)/60
			,(pDoc->m_nDel_Start_A_hr[isSz]*60+pDoc->m_nDel_Start_A_min[isSz]+foot)%60,0);
		s=tm2.Format ("%H:%M");
	}
	else
	{
		CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay ()
			,(int)((foot-numMorning-pDoc->m_nDel_Start_B_min[isSz]+60)/60+pDoc->m_nDel_Start_B_hr[isSz])-1
			,(int)((foot-numMorning-pDoc->m_nDel_Start_B_min[isSz]+60)%60),0);
		s=tm2.Format ("%H:%M");
	}
	return s;



}



int CTaiKlineMin1::OnLeft()
{
	if(m_klinNumDefault<=0)
		return 0;
	if(pView->m_isShowCross==0)
	{
		pView->m_nOldHitPos =-1;
		pView->m_nOldHitY =-1;
		m_footCurrent=m_footEnd;

		pView->m_isShowCross=1;

		pView->RedrawWindow ();
		if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_RESTORE);
		pView->SetFocus ();
	} 
	else
	{
		if(::GetKeyState(VK_SHIFT)&0x8000)
			m_footCurrent=m_footCurrent-(m_footEnd-m_footBegin)/10;
		else 
			m_footCurrent--;
	}

	if(m_footCurrent>m_footEnd||m_footCurrent<m_footBegin )
	{
		m_footCurrent=m_footEnd;
	}

	m_currentValue=m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent];

	m_nSon=0;
	SetRectDraw(0);

	int foot=m_footCurrent-m_footBegin;
	int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/(float)m_klinNumDefault+0.5);
	int yNow=YTransfer(m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent]);
	xNow+=pView->m_rtMin1 .leftX;
	pView->m_pointMove.x=xNow;
	pView->m_pointMove.y=yNow;

	POINT CurPos;
	CurPos.x = xNow;
	CurPos.y = yNow;
	ClientToScreen(pView->m_hWnd,&CurPos);
	if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)
	{
		CRect dlgRect;
		GetWindowRect(pView->m_dlgShowCross->m_hWnd,&dlgRect);
		if(PtInRect(dlgRect,CurPos))
		{
			CRect ClientRect;
			pView->GetClientRect(&ClientRect);
			if(CurPos.x > ClientRect.Width()/2)
				MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2 - dlgRect.Width(),CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
			else
				MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2,CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
		}
	}
	SetCursorPos(CurPos.x,CurPos.y);

	return 1;


}


int CTaiKlineMin1::OnRight()
{		if(m_klinNumDefault<=0)
return 0;
if(pView->m_isShowCross==0)
{
	pView->m_nOldHitPos =-1;
	pView->m_nOldHitY =-1;
	m_footCurrent=m_footBegin;


	pView->m_isShowCross=1;

	pView->RedrawWindow ();
	if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_RESTORE);
	pView->SetFocus ();
}
else
{
	if(::GetKeyState(VK_SHIFT)&0x8000)
		m_footCurrent=m_footCurrent+(m_footEnd-m_footBegin)/10;
	else
		m_footCurrent++;
}

if(m_footCurrent>m_footEnd||m_footCurrent<m_footBegin )
{
	m_footCurrent=m_footBegin;
}

m_currentValue=m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent];

m_nSon=0;
SetRectDraw(0);

int foot=m_footCurrent-m_footBegin;
int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/(float)(m_klinNumDefault)+0.5);
int yNow=YTransfer(m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent]);

xNow+=pView->m_rtMin1 .leftX;
pView->m_pointMove.x=xNow;
pView->m_pointMove.y=yNow;

POINT CurPos;
CurPos.x = xNow;
CurPos.y = yNow;
ClientToScreen(pView->m_hWnd,&CurPos);
if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)
{
	CRect dlgRect;
	GetWindowRect(pView->m_dlgShowCross->m_hWnd,&dlgRect);
	if(PtInRect(dlgRect,CurPos))
	{
		CRect ClientRect;
		pView->GetClientRect(&ClientRect);
		if(CurPos.x > ClientRect.Width()/2)
			MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2 - dlgRect.Width(),CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
		else
			MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2,CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
	}
}
SetCursorPos(CurPos.x,CurPos.y);

return 1;


}
int CTaiKlineMin1::OnEnd()
{
	if(m_klinNumDefault<=0)
		return 0;
	if(pView->m_isShowCross==0)
	{

		if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_RESTORE);
		pView->m_isShowCross=1;
	}

	m_footCurrent=m_footEnd;

	m_currentValue=m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent];

	m_nSon=0;
	SetRectDraw(0);

	int foot=m_footCurrent-m_footBegin;
	int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
	int yNow=YTransfer(m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent]);

	xNow+=pView->m_rtMin1 .leftX;
	pView->m_pointMove.x=xNow;
	pView->m_pointMove.y=yNow;
	return 1;


}

int CTaiKlineMin1::OnHome()
{
	if(pView->m_isShowCross==0)
	{

		if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)pView->m_dlgShowCross->ShowWindow(SW_RESTORE);
		pView->m_isShowCross=1;
	}

	m_footCurrent=m_footBegin;

	m_currentValue=m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent];

	m_nSon=0;
	SetRectDraw(0);

	int foot=m_footCurrent-m_footBegin;
	int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
	int yNow=YTransfer(m_dataFormular[0].line [0].m_arrBE.line[m_footCurrent]);

	xNow+=pView->m_rtMin1 .leftX;
	pView->m_pointMove.x=xNow;
	pView->m_pointMove.y=yNow;
	return 1;


}


int CTaiKlineMin1::OutString(int num, CPoint p)
{
	m_nSon=num;
	if(num<0||num>3)
		return 0;

	SetRectDraw(m_nSon);

	CRect r;
	pView->GetClientRect(r);


	int foot;
	if((pView->m_rtMin1.rightX-pView->m_rtMin1.leftX)<=0)
		return 0;
	else foot=(p.x-pView->m_rtMin1.leftX)*m_klinNumDefault /(pView->m_rtMin1.rightX-pView->m_rtMin1.leftX);
	foot+=m_footBegin;
	if(foot>m_footEnd)
		return 0;



	if(m_klinNumDefault<=0)
		return 0;
	float widper=pView->m_rtMin1 .rightX/(float)m_klinNumDefault;
	if(m_rectDrawLine.bottom-m_rectDrawLine.top<=1)
		return 0;
	for(int i=0;i<m_dataFormular[m_nSon].numLine ;i++)
	{
		CRgn rgn;

		if(foot<m_dataFormular[m_nSon].line[i].bNow) break;
		else if(foot>m_dataFormular[m_nSon].line[i].bNow)
		{
			pt[0].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot-1])-2;
			pt[1].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot-1])+2;
			pt[2].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[5].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
			int add=1;
			if(foot==m_footEnd)
				add=0;
			pt[3].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot+add])+2;
			pt[4].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot+add])-2;

		}
		else
		{
			pt[0].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
			pt[1].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[2].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[3].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[4].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
			pt[5].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;

		}
		pt[0].x=(int)((TransferX(p.x)-1)*widper+widper/2)-1;
		pt[1].x=(int)((TransferX(p.x)-1)*widper+widper/2)-1;
		pt[2].x=(int)((TransferX(p.x))*widper+widper/2);
		pt[3].x=(int)((TransferX(p.x)+1)*widper+widper/2)+1;
		pt[4].x=(int)((TransferX(p.x)+1)*widper+widper/2)+1;
		pt[5].x=(int)((TransferX(p.x))*widper+widper/2);


		rgn.CreatePolygonRgn (pt,6,ALTERNATE);
		if(rgn.PtInRegion (p)!=0)
		{

			CString rStr;
			rStr.Format (" %.3f\n",(float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot]);
			m_tipStr=m_lineName[m_nSon][i]+rStr;
			m_tipStr.MakeUpper( );

			return 1;
		}
	}

	return 0;

}


void CTaiKlineMin1::DrawRulorX(CDC *pDC)
{
	CString s;
	int isSz=0;
	if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SZ_MARKET_EX) isSz=1;
	int numMorning=(pDoc->m_nDel_End_A_hr[isSz]-pDoc->m_nDel_Start_A_hr[isSz])*60
		+pDoc->m_nDel_End_A_min[isSz]-pDoc->m_nDel_Start_A_min[isSz];
	int numAfternoon=(pDoc->m_nDel_End_B_hr[isSz]-pDoc->m_nDel_Start_B_hr[isSz])*60
		+pDoc->m_nDel_End_B_min[isSz]-pDoc->m_nDel_Start_B_min[isSz];

	CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]); 
	CPen* pOldpen=pDC->SelectObject(&pen_line);

	int len=0;
	float widPer=(pView->m_rtMin1 .rightX-pView->m_rtMin1 .leftX)/(float)m_klinNumDefault;
	int top=pView->m_rtMin1.rtBlw[pView->m_infoInit.nCountMin1-1].m_yBottom;
	int gap=2;

	pDC->MoveTo((int)(pView->m_rtMin1.leftX),pView->m_heightCaption);
	pDC->LineTo((int)(pView->m_rtMin1.rightX),pView->m_heightCaption);
	for(int k=0;k<pView->m_infoInit . nCountMin1-1;k++)
	{
		pDC->MoveTo(pView->m_rtMin1.leftX,pView->m_heightCaption + pView->m_rtMin1 . rtBlw[k].m_yBottom);
		pDC->LineTo(pView->m_rtMin1 . rightX,pView->m_heightCaption + pView->m_rtMin1 . rtBlw[k].m_yBottom);
	}


	int foot=numMorning;
	pDC->MoveTo((int)(foot*widPer)+pView->m_rtMin1.leftX,pView->m_heightCaption);
	pDC->LineTo((int)(foot*widPer)+pView->m_rtMin1.leftX,top);
	foot/=2;
	pDC->MoveTo((int)(foot*widPer)+pView->m_rtMin1.leftX,pView->m_heightCaption);
	pDC->LineTo((int)(foot*widPer)+pView->m_rtMin1.leftX,top);
	foot=numMorning+numAfternoon/2;
	pDC->MoveTo((int)(foot*widPer)+pView->m_rtMin1.leftX,pView->m_heightCaption);
	pDC->LineTo((int)(foot*widPer)+pView->m_rtMin1.leftX,top);

	foot=numMorning/4;
	CPoint p1,p2;
	p1.x=(int)(foot*widPer+pView->m_rtMin1.leftX);
	p2.x=p1.x;
	p1.y=pView->m_heightCaption;
	p2.y=top;
	ShowVirtualLine(pDC,p1,p2);

	foot=numMorning*3/4;
	p1.x=(int)((foot*widPer)+pView->m_rtMin1.leftX);
	p2.x=p1.x;
	ShowVirtualLine(pDC,p1,p2);

	foot=numMorning+numAfternoon/4;
	p1.x=(int)(foot*widPer+pView->m_rtMin1.leftX);
	p2.x=p1.x;
	ShowVirtualLine(pDC,p1,p2);

	foot=numMorning+numAfternoon*3/4;
	p1.x=(int)(foot*widPer+pView->m_rtMin1.leftX);
	p2.x=p1.x;
	ShowVirtualLine(pDC,p1,p2);


	pDC->SetTextAlign( TA_RIGHT );
	pDC->SetTextColor( pDoc->m_colorArray [2]);
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];

	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	CTime tm=CTime::GetCurrentTime ();
	CTime tm1(tm.GetYear (),tm.GetMonth (),tm.GetDay (),pDoc->m_nDel_Start_A_hr[isSz]
	,pDoc->m_nDel_Start_A_min[isSz],0);
	s=tm1.Format ("%H:%M");
	CRect r(pView->m_rtMin1 .leftX,top,pView->m_rtMin1 .rightX,top+pView->m_heightCaption);
	foot=0;
	pDC->SetTextAlign( TA_LEFT );
	pDC->ExtTextOut (pView->m_rtMin1 .leftX,top,ETO_CLIPPED,r,s,NULL);
	pDC->SetTextAlign( TA_RIGHT );

	foot=numMorning/2;
	CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay ()
		,(pDoc->m_nDel_Start_A_hr[isSz]*60+pDoc->m_nDel_Start_A_min[isSz]+foot)/60
		,(pDoc->m_nDel_Start_A_hr[isSz]*60+pDoc->m_nDel_Start_A_min[isSz]+foot)%60,0);
	s=tm2.Format ("%H:%M");
	pDC->ExtTextOut ((int)(foot*widPer)+pView->m_rtMin1 .leftX-gap+22,top,ETO_CLIPPED,r,s,NULL);

	foot=numMorning;
	CTime tm3(tm.GetYear (),tm.GetMonth (),tm.GetDay ()
		,(pDoc->m_nDel_Start_A_hr[isSz]*60+pDoc->m_nDel_Start_A_min[isSz]+foot)/60
		,(pDoc->m_nDel_Start_A_hr[isSz]*60+pDoc->m_nDel_Start_A_min[isSz]+foot)%60,0);
	s=tm3.Format ("%H:%M");
	pDC->ExtTextOut ((int)(foot*widPer)+pView->m_rtMin1 .leftX-gap+22,top,ETO_CLIPPED,r,s,NULL);

	foot=numMorning+numAfternoon/2;
	CTime tm4(tm.GetYear (),tm.GetMonth (),tm.GetDay ()
		,(int)((foot-numMorning-pDoc->m_nDel_Start_B_min[isSz]+60)/60+pDoc->m_nDel_Start_B_hr[isSz])-1
		,(int)((foot-numMorning-pDoc->m_nDel_Start_B_min[isSz]+60)%60),0);
	s=tm4.Format ("%H:%M");
	pDC->ExtTextOut ((int)(foot*widPer)+pView->m_rtMin1 .leftX-gap+22,top,ETO_CLIPPED,r,s,NULL);

	foot=numMorning+numAfternoon;
	CTime tm5(tm.GetYear (),tm.GetMonth (),tm.GetDay ()
		,(int)((foot-numMorning-pDoc->m_nDel_Start_B_min[isSz]+60)/60+pDoc->m_nDel_Start_B_hr[isSz])-1
		,(int)((foot-numMorning-pDoc->m_nDel_Start_B_min[isSz]+60)%60),0);
	s=tm5.Format ("%H:%M");
	pDC->ExtTextOut ((int)(foot*widPer)+pView->m_rtMin1 .leftX-gap,top,ETO_CLIPPED,r,s,NULL);

	pDC->SelectObject(pOld);
	pDC->SelectObject(pOldpen);
}




void CTaiKlineMin1::ShowMultiFiguer(CDC *pDC,int flag)
{
	if(m_pReportData==NULL)
		return;
	pDC->SelectClipRgn (NULL);//
	CRect r(m_rectDrawLine.left,m_rectDrawLine.top-pView->m_heightCaption,
		m_rectDrawLine.right+pView->m_midLen,m_rectDrawLine.top);


	int flagDoc=pView->m_infoInit.flag[1];
	pView->m_infoInit.flag[1]=1;


	CPen pen(PS_SOLID,1,pDoc->m_colorArray[2]);
	CPen* pOldpen=pDC->SelectObject(&pen);

	pDC->MoveTo (m_rectDrawLine.left,m_rectDrawLine.top);
	pDC->LineTo (m_rectDrawLine.right,m_rectDrawLine.top);
	pDC->LineTo (m_rectDrawLine.right,m_rectDrawLine.bottom);//--
	pDC->LineTo (m_rectDrawLine.left,m_rectDrawLine.bottom);//--
	pDC->LineTo (m_rectDrawLine.left,m_rectDrawLine.top);

	pDC->SelectObject(pOldpen);


	if(flag==0)
	{
		pDC->SetTextAlign( TA_LEFT );
		pDC->SetTextColor( pDoc->m_colorArray [1]);
		pDC->SetBkMode(TRANSPARENT);
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];

		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);

		CString s=m_pReportData->name  ;
		pDC->ExtTextOut (r.left ,r.top,ETO_CLIPPED,r,s,NULL);
		pDC->SelectObject(pOld);
	}


	m_nSon=flag;
	float max_sun=m_max_sun[m_nSon];
	float min_sun=m_min_sun[m_nSon];
	m_max_sun[m_nSon]=(float)-1.0E20;
	m_min_sun[m_nSon]=(float)1.0E20;
	int klinNumDefault=m_klinNumDefault;
	int footB=m_footBegin;
	int footE=m_footEnd;
	int axisType=m_axisType;
	m_axisType=0;


	int fenshiN=pView->m_infoInit.nCountMin1;
	pView->m_infoInit.nCountMin1=2;

	int numln=m_dataFormular[m_nSon].numLine;
	InitMinuteLine();
	m_dataFormular[m_nSon].numLine=1;

	pView->m_infoInit.nCountMin1=fenshiN;


	if(flag==0)
	{
		CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]);  
		CPen* pOldpen=pDC->SelectObject(&pen_line);
		int y=YTransfer((float)(m_close));
		pDC->MoveTo(m_rectDrawLine.left,y);
		pDC->LineTo(m_rectDrawLine.right+1,y);
		pDC->MoveTo(m_rectDrawLine.left,y-1);
		pDC->LineTo(m_rectDrawLine.right+1,y-1);
		pDC->SelectObject(pOldpen);
	}
	m_dataFormular[m_nSon].line [0].bNow=m_footBegin;
	CaclMaxFlt(m_dataFormular[m_nSon].line [0].m_arrBE.line,
		m_footBegin);

	if(flag==0)
	{

		float cls;
		cls=m_close;

		float maxPrice=(float)(FABSMY(m_max_sun[m_nSon]-cls)>FABSMY(m_min_sun[m_nSon]-cls)?
			FABSMY(m_max_sun[m_nSon]-cls):FABSMY(m_min_sun[m_nSon]-cls));
		m_max_sun[m_nSon]=maxPrice+cls;


		m_min_sun[m_nSon]=cls-maxPrice;
	}



	ShowLittleRulor(pDC,2);


	CString sName=m_lineName[m_nSon][0];
	if(flag==1)
		m_lineName[m_nSon][0]="成交量";
	DrawLineIndex(pDC,true);
	m_lineName[m_nSon][0]=sName;

	m_dataFormular[m_nSon].numLine=numln;


	m_max_sun[m_nSon]=max_sun;
	m_min_sun[m_nSon]=min_sun;
	m_klinNumDefault=klinNumDefault;
	m_footBegin=footB;
	m_footEnd=footE;
	m_axisType=axisType;


	pView->m_infoInit.flag[1]=flagDoc;

}



void CTaiKlineMin1::AddFenshiAdded(CString symbol,int stkKind,int nStock)
{

	CString s=symbol ;

	CReportData* pCdat1;
	if(pDoc->m_sharesInformation.Lookup(s.GetBuffer (0),pCdat1,stkKind)==0)
		return;

	int isSz=0;
	if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;

	bool bZhiShu=false;

	m_dataFormular[0].numLine=1+m_nKlineCurrent;


	int j=0;
	float* f0;
	f0=m_dataFormular[0].line [nStock].m_arrBE.line;
	if(f0)
	{
		if((pCdat1->m_Kdata1)->Price<=0)
			*f0=(pCdat1)->ystc;
		else
			*f0=(pCdat1->m_Kdata1)->Price;
		for(j=1;j<=m_footEnd;j++)
		{
			if((pCdat1->m_Kdata1+j)->Price>0)
				*(f0+j)=(pCdat1->m_Kdata1+j)->Price;
			else
				*(f0+j)=*(f0+j-1);
		}
	}

}


void CTaiKlineMin1::ShowCNP(CDC *pDC,int nFlags)
{
	if(nFlags<0)
		return;

	int end;
	int begin;
	if(m_footEnd<0)
		m_footEnd = 0;
	if(m_footBegin<0)
		m_footBegin = 0;
	if(m_footEnd>=240)
		m_footEnd = 239;
	if(m_footBegin>=240)
		m_footBegin = 239;

	if(nFlags==1)
	{
		begin=m_footCurrent;
		if(m_footCurrent>m_footEnd||m_footCurrent<=m_footBegin)
			begin=m_footBegin;
		end=m_footEnd;
	}
	else if(nFlags==2)
	{
		end=m_footCurrent;
		if(m_footCurrent>=m_footEnd||m_footCurrent<m_footBegin)
		{
			end=m_footEnd;
			m_footCurrent = m_footEnd;
		}

		begin=m_footBegin;
	}
	else
	{
		end=m_footEnd;
		begin=m_footBegin;
	}
	ASSERT(m_footBegin>=0);
	ASSERT(m_footEnd<240);


	m_nSon=0;
	SetRectDraw(m_nSon);


	int n=m_rectDrawLine.bottom-m_rectDrawLine.top+1;
	if(n<=0 || n>10000000)
		return;


	float max=0;


	int nHs=pView->m_hs.GetCount();
	if(nHs<=0)
		return;

	int yCnp = 0;
	POSITION pos;
	TRADE_DETAIL_H_PER* pHs;
	TRADE_DETAIL_H_PER* pHsPre = NULL;

	if( ( pos = pView->m_hs.GetHeadPosition() ) == NULL )
		return ;

	float* cnp=new  float[n];
	memset(cnp,0,n*4);
	for(int j=0;j<nHs;j++)
	{
		pHs=pView->m_hs.GetNext(pos);

		if(pHs->time<begin || pHs->time>end)
		{
			pHsPre = pHs;
			continue;
		}

		int foot=YTransfer(pHs -> price)-pView->m_heightCaption;
		if(foot>=n || foot <0)
		{
			pHsPre = pHs;
			continue;
		}

		if(pHsPre!=NULL)
		{
			ASSERT(FABSMY(pHsPre->vol)-FABSMY(pHs->vol)>=0);
			cnp[foot] += (float)FABSMY(FABSMY(pHsPre->vol)-FABSMY(pHs->vol));
		}
		if(j==nHs-1)
			cnp[foot] += (float)FABSMY(pHs->vol);
		if(cnp[foot]>max)
		{
			max=cnp[foot];
			yCnp = foot;
		}

		pHsPre = pHs;
	}


	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[6]); 
	CPen* pOldpen=pDC->SelectObject(&pen_line);

	if(max>0)
	{
		for(int i=0;i<n;i++)
		{
			if(cnp[i] == 0)
				continue;
			pDC->MoveTo(m_rectDrawLine.right,pView->m_heightCaption+i);
			pDC->LineTo(m_rectDrawLine.right-(int)(cnp[i]*m_rectDrawLine.right/2/max),pView->m_heightCaption+i);
		}

		CString s;
		s.Format ("%d",(int)max);
		int len=(pDC->GetOutputTextExtent(s)).cy;
		CRect r(0,pView->m_heightCaption ,pView->m_rtMin1 .rightX ,pView->m_rtMin1 .rtBlw [0].m_yBottom );
		if((m_footEnd-m_footBegin+1)!=0)
		{
			float widPer=(float)(m_rectDrawLine.right-m_rectDrawLine.left)/(m_footEnd-m_footBegin+1);
			pDC->ExtTextOut (m_rectDrawLine.right/2,yCnp-len-2+pView->m_heightCaption,ETO_CLIPPED,r,s,NULL);
		}
	}

	delete[] cnp;

	pDC->SelectObject(pOldpen);
}


void CTaiKlineMin1::SetRectDraw(int nFigu)
{
	m_rectDrawLine.left=pView->m_rtMin1.leftX;
	m_rectDrawLine.right=pView->m_rtMin1.rightX;
	if(nFigu==0)
		m_rectDrawLine.top=pView->m_heightCaption;
	else
		m_rectDrawLine.top=pView->m_rtMin1.rtBlw[nFigu-1].m_yBottom+pView->m_heightCaption;
	m_rectDrawLine.bottom=pView->m_rtMin1.rtBlw[nFigu].m_yBottom;


}

void CTaiKlineMin1::GetFenJia(int first,CBuySellList &buySellList, CFJList &fenjiaUp, CFJList &fenjiaDown, int nFlag)
{
	FENJIA* pf=new FENJIA();
	int nCount = buySellList.GetCount();
	CFJList * fenjiaUse = &fenjiaUp;
	for(int i=first;i<nCount;i++)
	{


		POSITION pos=buySellList.FindIndex(nCount - i - 1 ) ;
		TRADE_DETAIL_H_PER* pHs=buySellList.GetAt( pos );
		TRADE_DETAIL_H_PER* pHsPre = NULL;
		if(i>0)
		{
			pos=buySellList.FindIndex(nCount - i  ) ;
			pHsPre=buySellList.GetAt( pos );
		}
		int nFlag2;
		pf->vol = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag2);


		pf->price =pHs->price ;
		if(pHsPre==NULL)
			pf->vol =(float)FABSMY(pHs->vol) ;
		else
		{

			if(FABSMY(pHs->vol)-FABSMY(pHsPre->vol)>=0)
				pf->vol =(float)FABSMY(FABSMY(pHs->vol)-FABSMY(pHsPre->vol)) ;
			else
				pf->vol =0;
		}


		int nFor = 1;
		if(nFlag == 1)
		{
			if(nFlag2>0) fenjiaUse = &fenjiaUp;
			else if(nFlag2<0)fenjiaUse = &fenjiaDown;
			else nFor = 2;
		}

		for(int j=0;j<nFor;j++)
		{
			if(nFor == 2)
			{
				if(j==0) fenjiaUse = &fenjiaUp;
				else fenjiaUse = &fenjiaDown;
			}
			int num=fenjiaUse->GetCount();
			if(num==0)
			{
				FENJIA* pf2=new FENJIA();
				pf2->price =pf->price ;
				pf2->vol =pf->vol/nFor ;
				fenjiaUse->AddTail(pf2);
			}
			else
			{
				FENJIA* tmp=fenjiaUse->GetHead();
				if( ( pos = fenjiaUse->GetHeadPosition() ) == NULL )
				{
					return;
				}
				for(int j=0;j<num;j++)
				{
					POSITION pos2=pos;
					tmp=fenjiaUse->GetNext(pos);
					if((int)(pf->price*1000) >(int)(tmp->price*1000) )
					{
						FENJIA* pf2=new FENJIA();
						pf2->price =pf->price ;
						pf2->vol =pf->vol/nFor ;
						fenjiaUse->InsertBefore(pos2,pf2);
						break;
					}
					else if((int)(pf->price*1000) == (int)(tmp->price*1000) )
					{
						tmp->vol+=pf->vol/nFor ;
						break;
					}
					if(j==num-1)
					{
						FENJIA* pf2=new FENJIA();
						pf2->price =pf->price ;
						pf2->vol =pf->vol/nFor ;
						fenjiaUse->AddTail( pf2 );
					}

				}

			}
		}
	}
	delete pf;

}



float CTaiKlineMin1::TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags)
{
	float close=0;
	float high=0;
	float low=(float)10E20;
	float open=0;

	float vol=0;
	float amount=0;
	float zhDie=0;
	float zhDieFu=0;
	float junJia_jiaQuan=0;

	int begin,end;
	float cellWidth=(float)(pView->m_rtMin1.rightX-pView->m_rtMin1.leftX)/m_klinNumDefault;

	if (nFlags==0)
	{
		if(pView->m_rectHitPos.left<pView->m_rectHitPos.right)
		{
			begin=(int)((pView->m_rectHitPos.left-pView->m_rtMin1.leftX-(cellWidth+1)/2)/cellWidth);
			end=(int)((pView->m_rectHitPos.right-pView->m_rtMin1.leftX-(cellWidth+1)/2)/cellWidth);
		}
		else
		{
			begin=(int)((pView->m_rectHitPos.right-pView->m_rtMin1.leftX-(cellWidth+1)/2)/cellWidth);
			end=(int)((pView->m_rectHitPos.left-pView->m_rtMin1.leftX-(cellWidth+1)/2)/cellWidth);
		}

		if(begin>=end)
			return -1;
		begin=m_footBegin+begin;
		if(begin>m_footEnd)
			begin=m_footEnd;
		end=m_footBegin+end;
		if(end>m_footEnd)
			end=m_footEnd;
	}
	else
	{
		CTime tmB=pTJ->m_tmB;
		CTime tmE=pTJ->m_tmE;
		if (tmB>tmE)
		{
			tmB=pTJ->m_tmE;
			tmE=pTJ->m_tmB;
		}

		CTime tmAStart(tmB.GetYear(),tmB.GetMonth(),tmB.GetDay(),pDoc->m_nDel_Start_A_hr[0],pDoc->m_nDel_Start_A_min[0],0);
		CTime tmAEnd(tmB.GetYear(),tmB.GetMonth(),tmB.GetDay(),pDoc->m_nDel_End_A_hr[0],pDoc->m_nDel_End_A_min[0],0);
		CTime tmPStart(tmB.GetYear(),tmB.GetMonth(),tmB.GetDay(),pDoc->m_nDel_Start_B_hr[0],pDoc->m_nDel_Start_B_min[0],0);
		CTime tmPEnd(tmB.GetYear(),tmB.GetMonth(),tmB.GetDay(),pDoc->m_nDel_End_B_hr[0],pDoc->m_nDel_End_B_min[0],0);

		if (tmB>=tmAEnd && tmB<tmPStart)
			tmB=tmAEnd;
		if (tmE>tmAEnd && tmE<=tmPStart)
			tmE=tmPStart;
		CTimeSpan tms;
		if (tmB>tmAEnd) 
		{
			tms=tmAEnd-tmAStart;
			begin=tms.GetTotalMinutes();
			tms=tmB-tmPStart;
			begin+=tms.GetTotalMinutes();
		}
		else
		{
			tms=tmB-tmAStart;
			begin=tms.GetTotalMinutes();
		}

		if (tmE>tmAEnd) 
		{
			tms=tmAEnd-tmAStart;
			end=tms.GetTotalMinutes();
			tms=tmE-tmPStart;
			end+=tms.GetTotalMinutes();
		}
		else
		{
			tms=tmE-tmAStart;
			end=tms.GetTotalMinutes();
		}

		if(begin>=end)
			return -1;
		begin=m_footBegin+begin;
		if(begin>m_footEnd)
			return -1;
		end=m_footBegin+end;
		if(end>m_footEnd)
			end=m_footEnd;
	}

	float junJia=0;
	int nNum=0;		
	float fTotal=0;
	int nBeginValid=begin-1;
	int nEndValid=end+1;	

	for(int i=begin;i<=end;i++)
	{
		if (m_vol[i]>0 && m_amount>0)
		{
			vol+=m_vol[i];
			amount+=m_amount[i];
			if(m_hsMin[i].price>high)
				high=m_hsMin[i].price;
			if(m_hsMin[i].price<low)
				low=m_hsMin[i].price;
			fTotal+=m_hsMin[i].price;

			if (open<0.0000000001 && m_hsMin[i].price>0)
			{
				nBeginValid=i;
				open=m_hsMin[i].price;
			}
			if (m_hsMin[i].price>0)
			{
				nEndValid=i;
				close=m_hsMin[i].price;
			}
			nNum++;
		}
	}
	if (nNum<=0)
		return -1;

	junJia_jiaQuan=amount/vol;
	junJia=fTotal/(float)nNum;
	zhDie=m_hsMin[nEndValid].price-m_hsMin[nBeginValid].price;
	if (m_hsMin[nBeginValid].price==0)
		zhDieFu=0;
	else
		zhDieFu=zhDie/m_hsMin[nBeginValid].price*100;

	CString s;
	s.Format("%.3f",open);
	pTJ->m_open=s;		
	s.Format("%.3f",high);
	pTJ->m_high=s;		
	s.Format("%.3f",low);
	pTJ->m_low=s;		
	s.Format("%.3f",close);
	pTJ->m_close=s;		
	s.Format("%.0f",vol);
	pTJ->m_vol=s;		
	s.Format("%.2f",amount/10000);
	s=s+"万";
	pTJ->m_amount=s;

	s.Format("%.3f",zhDie);
	pTJ->m_zhDie=s;		
	s.Format("%.2f",zhDieFu);
	s=s+"%";
	pTJ->m_zhDieFu=s;	

	s.Format("%.3f",junJia);
	pTJ->m_junjia=s;
	s.Format("%.3f",junJia_jiaQuan);
	pTJ->m_chengBenTJ=s;	

	s="";
	{
		float ltp = CTaiShanKlineShowView::GetCapital(pView->pMin1Drawing ->m_pReportData);	
		if(ltp >0)
		{
			s.Format("%.3f",vol/ltp*100 );
			s+="%";
		}
	}
	pTJ->m_huanShou=s;		


	if(nFlags==0)
	{
		CTime tmB=m_hsMin[nBeginValid].time;
		pTJ->m_tmB=tmB;	
		CTime tmE=m_hsMin[nEndValid].time;
		pTJ->m_tmE=tmE;	
	}

	s.Format("%d",nNum);
	pTJ->m_num=s;

	m_nBeginFootTJ = begin;	
	m_nEndFootTJ = end;		

	CTaiKlineDC* pMemDC = new CTaiKlineDC(pView,&pView->m_bit);
	this->DrawMin1Figuer (pMemDC);
	delete pMemDC;

	pView->RedrawWindowNoCacl ();

	ValidDoKline(true);
	return 1;
}

void CTaiKlineMin1::CaclMaxAdded()
{
	if (m_close == 0)
	{
		m_dataFormular[0].numLine = 1;
		return;
	}

	for (int i = 1; i < m_dataFormular[0].numLine; i++)
	{
		CReportData* pCdat1;
		if (pDoc->m_sharesInformation.Lookup(m_symbolAdd[i-1].GetBuffer(0), pCdat1, m_stkKindAdd[i-1]) == 0)
		{	
			m_dataFormular[0].numLine = 1;
			return;
		}

		float close = pCdat1->ystc;
		CString s = m_symbolAdd[i-1];

		if (close == 0)
		{
			m_dataFormular[0].numLine = 1;
			return;
		}

		for (int j = 0; j <= m_footEnd; j++)
		{
			m_dataFormular[0].line[i].m_arrBE.line[j] = (m_dataFormular[0].line[i].m_arrBE.line[j] - close) * m_close / close + m_close;
		}
	}
}

void CTaiKlineMin1::DrawMin1Figuer(CDC* pDC)
{
	pView->DrawRectPer(pDC);
	DrawRulorX(pDC);
	pView->ShowTextRect(m_nTextBox, pDC);
	pView->ShowTransferText(pDC);

	InitMinuteLine();

	for (int i = 0; i < pView->m_infoInit.nCountMin1; i++)
	{
		m_nSon=i;
		m_max_sun[m_nSon]=(float)-9.0E20;
		m_min_sun[m_nSon]=(float)9.0E20;
		DrawSon(pDC);

		if(i == 0 && pView->m_isShowCross == false)
		{
			if(m_nCnp == 2)
				ShowCNP(pDC,0);
			else
				ShowCNP(pDC,m_nCnp);
		}
	}


	if(m_bInvertRect == true)
	{
		CRect r;
		pView->GetCurrClientRect(r);
		int nb = m_nBeginFootTJ - this->m_footBegin;
		if(nb<0)
			nb = 0;
		int ne =   m_nEndFootTJ - this->m_footBegin+1;
		if(ne<0)
			ne=0;
		if(ne>m_klinNumDefault)
			ne = m_klinNumDefault;
		float f = (float)(pView->m_rtMin1.rightX-pView->m_rtMin1.leftX)/m_klinNumDefault;
		r.left = pView->m_rtMin1.leftX+f*nb;
		r.right = pView->m_rtMin1.leftX+f*ne;
		r.top = 2;


		pDC->InvertRect(r);
	}


}

int CTaiKlineMin1::TransferX(int x)
{
	int rtn;
	rtn = (int)((float)((x - pView->m_rtMin1.leftX) * m_klinNumDefault) / (pView->m_rtMin1.rightX - pView->m_rtMin1.leftX));
	return rtn;
}
