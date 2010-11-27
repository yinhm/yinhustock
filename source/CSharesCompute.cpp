
#include "stdafx.h"
#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "CTaiShanApp.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "StockDrv.h"
#include "StructTaiShares.h"
#include "CSharesCompute.h"
//#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"

#include "CTaiKlineFileHS.h"
#include "StkDatabase.h"

CSharesCompute::CSharesCompute()
{

}
CSharesCompute::~CSharesCompute()
{

}
void CSharesCompute::chk_rdp(CReportData  *Cdat,float NewPrice)                                                       //计算该股票RDP值
{
	if (NewPrice==0 || (Cdat->psel1==0 && Cdat->pbuy1==0))
		Cdat->rdp='9';
	else if (Cdat->psel1==0 && Cdat->pbuy1==0 && NewPrice==Cdat->ystc)
		Cdat->rdp='0';
	else if (Cdat->psel1==0 && Cdat->pbuy1==0 && NewPrice>Cdat->ystc && Cdat->ystc>0)
		Cdat->rdp='3';
	else if (Cdat->psel1==0 && Cdat->pbuy1==0 && NewPrice<Cdat->ystc && NewPrice>0)
		Cdat->rdp='4';
	else if (Cdat->psel1==0 && Cdat->pbuy1>0)
		Cdat->rdp='1';
	else if (Cdat->pbuy1==0 && Cdat->psel1>0)
		Cdat->rdp='2';
	else if (Cdat->nowp==0)
		Cdat->rdp='9';
	else if (Cdat->nowp<NewPrice)
		Cdat->rdp='5';
	else if (Cdat->nowp>NewPrice)
		Cdat->rdp='6';
	else if (Cdat->rdp=='9' && Cdat->pbuy1>0 && NewPrice>Cdat->pbuy1)
		Cdat->rdp='5';
	else if (Cdat->rdp=='9' && Cdat->psel1>0 && NewPrice<Cdat->psel1)
		Cdat->rdp='6';
}
float Vol;
float Per;
float Acount;

void CSharesCompute::SendCuoHeDataToDialog(CReportData *Cdat)
{
	CMainFrame *pMain=(CMainFrame *)(AfxGetApp()->m_pMainWnd);
	char  strBuySell[10];
	if(Cdat->InOut==0)
		strcpy(strBuySell,"大单买入");
	else if(Cdat->InOut==1)
		strcpy(strBuySell,"大单卖出");
	else if(Cdat->InOut==2)
		strcpy(strBuySell,"内部撮合");
	else
		return;
	BOOL IsSend=FALSE;
	if(Cdat->kind==SHAG||Cdat->kind==SHBG||Cdat->kind==SZAG||Cdat->kind==SZBG||Cdat->kind==EBAG)
	{
		switch(this->m_MainDocument->m_dadanchoice.Format)
		{
		case 0:     if(Cdat->nowv>=this->m_MainDocument->m_dadanchoice.Vol)
					{
						IsSend=TRUE;
					}
					break; 
		case 1:     if(Cdat->pBaseInfo!=NULL)
					{
						if(Cdat->kind==SHBG||Cdat->kind==SZBG)  
						{
							if(Cdat->pBaseInfo->Bg>0&&(Cdat->nowv>=this->m_MainDocument->m_dadanchoice.Per*Cdat->pBaseInfo->Bg))
							{
								IsSend=TRUE;
							}
						}
						else                       
						{
							if(Cdat->pBaseInfo->ltAg>0&&(Cdat->nowv>=this->m_MainDocument->m_dadanchoice.Per*Cdat->pBaseInfo->ltAg))
							{
								IsSend=TRUE;
							}
						}
					}
					else
						return;
			break;
		case 2:     if(Cdat->nowp*Cdat->nowv/100>this->m_MainDocument->m_dadanchoice.Acount)
					{
						IsSend=TRUE;
					}
					break;
		} 
	}
	if(IsSend)
		pMain->m_wndStatusBar.SetBigBill(Cdat->id, strBuySell, Cdat->nowv, Cdat->nowp,TRUE);

}
void CSharesCompute::UpdateHs(CReportData  *Cdat,  short  Curr_Min)                                      //更新该股票每次成交数据
{
	CTaiKlineFileHS* pHsFile ;
	CString s(Cdat->id );
	pHsFile = TSKDatabase()->GetTickFile (s,Cdat->kind);
	pHsFile->WriteHS (Cdat);

}

short CSharesCompute::GetStockMinute(time_t time, int mode)
{
	int tmp;
	struct tm* osTime = new tm;

	CTime m_Time = time;

	osTime = m_Time.GetLocalTm(osTime);

	if (osTime->tm_hour < m_MainDocument->m_nDel_Start_B_hr[mode-1] )
	{   
		tmp=(osTime->tm_hour -m_MainDocument->m_nDel_Start_A_hr[mode-1])*60 + osTime->tm_min  - m_MainDocument->m_nDel_Start_A_min[mode-1] ;
		if (tmp>m_MainDocument->m_nDel_Half_ANT[mode-1])
			tmp=m_MainDocument->m_nDel_Half_ANT[mode-1];
	}
	else 
	{
		tmp=(osTime->tm_hour - m_MainDocument->m_nDel_Start_B_hr[mode-1])*60+osTime->tm_min - m_MainDocument->m_nDel_Start_B_min[mode-1]-1;
		if (tmp<0)
			tmp=m_MainDocument->m_nDel_Half_ANT[mode-1];
		else
		{
			tmp+=m_MainDocument->m_nDel_Half_ANT[mode-1]+1;
			if (tmp>m_MainDocument->m_nMaxANT[mode-1] )
				tmp=m_MainDocument->m_nMaxANT[mode-1] ;
		}
	}

	if (tmp < 0)
	{
		tmp = 0;
		ASSERT(FALSE);
	}

	return(tmp);
}


int CSharesCompute::GetMarketKind(int nKind)
{
	if(nKind==SHZS||nKind==SHBG||nKind==SHAG||nKind==SHZQ)
		return SH_MARKET_EX;
	else if(nKind==SZZS||nKind==SZBG||nKind==SZAG||nKind==SZZQ)
		return SZ_MARKET_EX;
	else 
		return EB_MARKET_EX;
}


CString CSharesCompute::GetIndexSymbol(int nKind)
{
	//lmb11
	ASSERT(nKind>=0 && nKind<4);
	static CString s[4]={"1A0001","399001","399001","100001"};
	static bool bFirst = true;
	if(bFirst == true)
	{
		CFile fl;
		if(fl.Open ("IndexSymbolTxt.txt",CFile::modeRead))
		{
			int n = fl.GetLength ();
			char * pch = new char[n+1];
			fl.Read (pch,n);
			pch[n] = 0;
			CString ss = pch;
			delete [] pch;
			ss.TrimLeft (" ");
			int nStart = 0;
			int nNext=0;
			int i = 0;
			while(i<4)
			{
				nNext = ss.Find (" ",0);
				if(nNext<0) break;

				CString s2 = ss.Left (nNext);
				s2.TrimRight (" ");
				s[i] = s2;
				i++;

				ss = ss.Right (ss.GetLength ()-s2.GetLength ());
				ss.TrimLeft (" ");
			}

		}
		bFirst = false;
	}

	if(nKind >0)
	{
		int stkKind[4] = {SHZS,SZZS,EBZS,EBZS};
		CReportData * pdt;
		if(CMainFrame::m_taiShanDoc->m_sharesInformation.GetStockItem(stkKind[nKind],0,pdt))//.Lookup(s[nKind].GetBuffer (0),pdt,stkKind[nKind] )==0)
		{
			if(pdt) s[nKind] = pdt->id;
		}
	}
	return s[nKind];
}

void CSharesCompute::StockDataUpdate(RCV_REPORT_STRUCTEx* pReport)
{
	CString m_Zqdm;
	CReportData  *Cdat;

	if (strlen(pReport->m_szLabel) <= 0 || strlen(pReport->m_szName) <=0 )
		return;

	int nKind = m_MainDocument->m_sharesInformation.GetStockKind(pReport->m_wMarket, pReport->m_szLabel);
	if (nKind == -1)
		return;

	m_MainDocument->CheckNewReport(pReport->m_time);

	if (m_MainDocument->m_sharesInformation.Lookup(pReport->m_szLabel, Cdat, nKind) != TRUE)
	{
		if (!m_MainDocument->m_sharesInformation.InsertItem(pReport->m_szLabel, Cdat, nKind))
			return;

		m_MainDocument->m_WsStock2000View->LoadShowData(nKind);

		strcpy(Cdat->name, pReport->m_szName);
		strcpy(Cdat->id, pReport->m_szLabel);
		Cdat->kind = nKind;
		Cdat->wMarket = pReport->m_wMarket;

		if (strlen(Cdat->name) > 0)
		{
			m_MainDocument->StockNameConvert(Cdat->name, Cdat->Gppyjc);
			m_MainDocument->AddStockToKeyboard(Cdat->id, Cdat->kind, Cdat->name, Cdat->Gppyjc);
		}

		m_MainDocument->m_WsStock2000View->AddGridRow(nKind);

		STOCK_TYPE_INFO* m_pStock;
		if (m_MainDocument->m_ManagerStockTypeData.Lookup(Cdat->id, Cdat->kind, m_pStock))
		{
			Cdat->pStockTypeInfo = m_pStock;
		}

		BASEINFO* m_pStockBase;
		if (m_MainDocument->m_sharesInformation.LookupBase(Cdat->id, nKind, m_pStockBase))
		{
			Cdat->pBaseInfo = m_pStockBase;
		}
	}

	m_Zqdm = Cdat->id;

	if (Cdat->ystc != 0)
	{
		if (fabs(pReport->m_fNewPrice) < 0.01)
		{
			if (strlen(pReport->m_szName) > 0&& strcmp(Cdat->name, pReport->m_szName) != 0)
			{
				m_MainDocument->StockNameConvert(pReport->m_szName, Cdat->Gppyjc);
				strcpy(Cdat->name, pReport->m_szName);
				Cdat->opnp = pReport->m_fOpen;
				m_MainDocument->AddStockToKeyboard(Cdat->id, Cdat->kind, Cdat->name, Cdat->Gppyjc);
			}

			return;
		}
	}

	if (Cdat->lastclmin < 0)
		Cdat->lastclmin = 0;
	if (Cdat->lastclmin > 240)
		Cdat->lastclmin = 239;

	StockItemUpdate(Cdat, pReport);
}

void CSharesCompute::StockItemUpdate(CReportData* Cdat, RCV_REPORT_STRUCTEx* pReport)
{
	int chg = 0;
	short Select_Market;
	float m_Stockvalue, ltmp;
	float OldBuyP1, OldSellP1;

	if (strcmp(Cdat->id, pReport->m_szLabel) != 0)
		return;

	OldBuyP1 = Cdat->pbuy1;
	OldSellP1 = Cdat->psel1;

	CString StockId = pReport->m_szLabel;

	// 大单提取
	if (m_MainDocument->m_pStockBigBillData && m_MainDocument->m_pStockBigBillData->m_pBigBillHead->Done)
	{
		if (Cdat->IsMxTj)
		{
			m_MainDocument->m_pStockBigBillData->CalcBigBillData(Cdat, pReport);
		}
	}

	// 转换证券拼音，添加到键盘精灵
	if (strlen(pReport->m_szName) > 0 && strcmp(Cdat->name, pReport->m_szName) != 0)
	{
		m_MainDocument->StockNameConvert(pReport->m_szName, Cdat->Gppyjc);
		strcpy(Cdat->name, pReport->m_szName);
		m_MainDocument->AddStockToKeyboard(Cdat->id, Cdat->kind, Cdat->name, Cdat->Gppyjc);
	}


	//
	if (pReport->m_wMarket == SH_MARKET_EX)
	{
		Select_Market = 0;
	}
	else
	{
		Select_Market = 1;
	}

	for (int i = Cdat->lastclmin; i < m_MainDocument->m_nANT[Select_Market]; i++)
	{
		if (Cdat->lastclmin == 0 && Cdat->initdown == FALSE && m_MainDocument->m_nANT[Select_Market] > 0)
		{
			Cdat->m_Kdata1[i].Amount = 0;
			Cdat->m_Kdata1[i].Price = 0;
			Cdat->m_Kdata1[i].Volume = 0;
		}
		else
		{
			Cdat->m_Kdata1[i].Amount = Cdat->m_Kdata1[Cdat->lastclmin].Amount;
			Cdat->m_Kdata1[i].Price = Cdat->m_Kdata1[Cdat->lastclmin].Price;
			Cdat->m_Kdata1[i].Volume = Cdat->m_Kdata1[Cdat->lastclmin].Volume;
		}
	}

	if (Cdat->id[1] == 'A')
	{
		Rsdn1** Nidx2;
		Tidxd** Tidx2;
		m_MainDocument->m_sharesInformation.GetIndexRsdn(Nidx2);
		m_MainDocument->m_sharesInformation.GetIndexTidxd(Tidx2);
		for (short i = Cdat->lastclmin; i < m_MainDocument->m_nANT[Select_Market]; i++)
		{
			if (Cdat->lastclmin == 0 && Cdat->initdown == FALSE && m_MainDocument->m_nANT[Select_Market] > 0)
			{
				Nidx2[Select_Market][i].rsn = 0;
				Nidx2[Select_Market][i].dnn = 0;
			}
			else
			{
				Nidx2[Select_Market][i].rsn = Nidx2[Select_Market][Cdat->lastclmin].rsn;
				Nidx2[Select_Market][i].dnn = Nidx2[Select_Market][Cdat->lastclmin].dnn;
			}
		}
	}

	if (!Cdat->initdown)
	{
		m_MainDocument->InitFiveDaysVolumnForStock(Cdat);
	}
	Cdat->initdown = TRUE;


	// 更新昨日收盘价
	if (pReport->m_fLastClose != 0 && pReport->m_fLastClose != -1)
	{
		Cdat->ystc = pReport->m_fLastClose;
	}


	// 计算涨跌停
	bool bBuySell = false;
	if (Cdat->ystc > 0 && pReport->m_fNewPrice > 0)
	{
		float ff = (pReport->m_fNewPrice - Cdat ->ystc) / Cdat ->ystc;
		if (ff > 0.095 || ff < -0.095)
		{
			bBuySell = true;
		}
		if (Cdat->id[0] == 'S' && Cdat->id[1] == 'T')
		{
			if (ff > 0.047 || ff < -0.047)
			{
				bBuySell = true;
			}
		}
	}


	if (Cdat->kind != SHZS && Cdat->kind != SZZS && Cdat->kind != EBZS)
	{
		if ((pReport->m_fBuyPrice[0] > 0 || bBuySell == true) && pReport->m_fBuyPrice[0] != -1)
		{
			if (pReport->m_fBuyPrice[0] != Cdat->pbuy1)
			{
				if (m_MainDocument->m_nANT[Select_Market] < 0)
				{
					m_MainDocument->m_nANT[Select_Market] = 0;
				}
				Cdat->pbuy1 = pReport->m_fBuyPrice[0];
				chg = 1;
			}
		}

		if ((pReport->m_fSellPrice[0] > 0 || bBuySell == true) && pReport->m_fSellPrice[0] != -1)
		{
			if (pReport->m_fSellPrice[0] != Cdat->psel1)
			{
				if (m_MainDocument->m_nANT[Select_Market] < 0)
				{
					m_MainDocument->m_nANT[Select_Market]=0;
				}
				Cdat->psel1 = pReport->m_fSellPrice[0];
				chg = 1;
			}
		}
	}


	if (pReport->m_fHigh != 0 && pReport->m_fHigh != -1)
		Cdat->higp = pReport->m_fHigh;

	if (pReport->m_fLow != 0 && pReport->m_fLow != -1)
		Cdat->lowp = pReport->m_fLow;

	if (pReport->m_fOpen != 0 && pReport->m_fOpen != -1)
		Cdat->opnp = pReport->m_fOpen;

	if (pReport->m_fNewPrice != 0 && pReport->m_fNewPrice != -1)
	{
		chk_rdp(Cdat, pReport->m_fNewPrice);
		if (pReport->m_fNewPrice != Cdat->nowp)
		{
			if (m_MainDocument->m_nANT[Select_Market] < 0)
			{
				m_MainDocument->m_nANT[Select_Market] = 0;
			}
			Cdat->nowp = pReport->m_fNewPrice;
			chg = 2;
		}
	}


	bool bValid = false;
	for (int k = 0; k < 3; k++)
	{
		if (fabs(pReport->m_fBuyVolume[k]) >= 0.5)
		{
			bValid = true;
			break;
		}
		if (fabs(pReport->m_fSellVolume[k]) >= 0.5)
		{
			bValid = true;
			break;
		}
	}


	// 非指数类型证券
	if (Cdat->kind != SHZS && Cdat->kind != SZZS && Cdat->kind != EBZS)
	{
		if ((pReport->m_fBuyVolume[0] > 0 || bValid == TRUE) && pReport->m_fBuyVolume[0] != -1)
			Cdat->vbuy1 = pReport->m_fBuyVolume[0];

		if ((pReport->m_fBuyPrice[1] > 0.01 || pReport->m_fBuyPrice[1] == 0 && bBuySell) && pReport->m_fBuyPrice[1] != -1)
			Cdat->pbuy2 = pReport->m_fBuyPrice[1];

		if ((pReport->m_fBuyVolume[1] > 0 || bValid == TRUE) && pReport->m_fBuyVolume[1] != -1)
			Cdat->vbuy2 = pReport->m_fBuyVolume[1];

		if ((pReport->m_fBuyPrice[2] > 0.01 || pReport->m_fBuyPrice[2] == 0 && bBuySell) && pReport->m_fBuyPrice[2] != -1)
			Cdat->pbuy3 = pReport->m_fBuyPrice[2];

		if ((pReport->m_fBuyVolume[2] > 0 || bValid == TRUE) && pReport->m_fBuyVolume[2] != -1)
			Cdat->vbuy3 = pReport->m_fBuyVolume[2];

		if ((pReport->m_fBuyPrice[3] > 0.01 || pReport->m_fBuyPrice[3] == 0 && bBuySell) && pReport->m_fBuyPrice[3] != -1)
			Cdat->pbuy4 = pReport->m_fBuyPrice4;

		if ((pReport->m_fBuyVolume[3] > 0 || bValid == TRUE) && pReport->m_fBuyVolume[3] != -1)
			Cdat->vbuy4 = pReport->m_fBuyVolume4;

		if ((pReport->m_fBuyPrice[4] > 0.01 || pReport->m_fBuyPrice[4] == 0 && bBuySell) && pReport->m_fBuyPrice[4] != -1)
			Cdat->pbuy5 = pReport->m_fBuyPrice5;

		if ((pReport->m_fBuyVolume[4] > 0 || bValid == TRUE) && pReport->m_fBuyVolume[4] != -1)
			Cdat->vbuy5 = pReport->m_fBuyVolume5;


		if ((pReport->m_fSellVolume[0] > 0 || bValid == TRUE) && pReport->m_fBuyVolume[0] != -1)
			Cdat->vsel1 = pReport->m_fSellVolume[0];

		if ((pReport->m_fSellPrice[1] > 0.001 || pReport->m_fSellPrice[1] == 0 && bBuySell) && pReport->m_fSellPrice[1] != -1)
			Cdat->psel2 = pReport->m_fSellPrice[1];          

		if ((pReport->m_fSellVolume[1] > 0 || bValid == TRUE) && pReport->m_fSellVolume[1] != -1)
			Cdat->vsel2 = pReport->m_fSellVolume[1];

		if ((pReport->m_fSellPrice[2] > 0.001 || pReport->m_fSellPrice[1] == 0 && bBuySell) && pReport->m_fSellPrice[2] != -1)
			Cdat->psel3 = pReport->m_fSellPrice[2];

		if ((pReport->m_fSellVolume[2] > 0 || bValid == TRUE) && pReport->m_fSellVolume[2] != -1)
			Cdat->vsel3 = pReport->m_fSellVolume[2];

		if ((pReport->m_fSellPrice[3] > 0.001 || pReport->m_fSellPrice[1] == 0 && bBuySell) && pReport->m_fSellPrice[3] != -1)
			Cdat->psel4 = pReport->m_fSellPrice4;

		if ((pReport->m_fSellVolume[3] > 0 || bValid == TRUE) && pReport->m_fSellVolume[3] != -1)
			Cdat->vsel4 = pReport->m_fSellVolume4;

		if ((pReport->m_fSellPrice[4] > 0.001 || pReport->m_fSellPrice[1] == 0 && bBuySell) && pReport->m_fSellPrice[4] != -1)
			Cdat->psel5 = pReport->m_fSellPrice5;

		if ((pReport->m_fSellVolume[4] > 0 || bValid == TRUE) && pReport->m_fSellVolume[4] != -1)
			Cdat->vsel5 = pReport->m_fSellVolume5;


		Cdat->accb=0;
		ltmp = Cdat->nowp*0.97;                                            
		if ( Cdat->pbuy1>=ltmp )                                            
			Cdat->accb=Cdat->vbuy1;
		if ( Cdat->pbuy2>=ltmp )
			Cdat->accb+=Cdat->vbuy2;
		if ( Cdat->pbuy3>=ltmp )
			Cdat->accb+=Cdat->vbuy3;                                 
		Cdat->accs=0;
		ltmp = Cdat->nowp*1.03;
		if ( Cdat->psel1<=ltmp )
			Cdat->accs=Cdat->vsel1;
		if ( Cdat->psel2<=ltmp )
			Cdat->accs+=Cdat->vsel2;
		if ( Cdat->psel3<=ltmp )
			Cdat->accs+=Cdat->vsel3;                         
	}


	m_Stockvalue=pReport->m_fVolume  ;                 
	if (pReport->m_fVolume>0)
	{
		if(m_Stockvalue==0 && Cdat->totv>0 && m_MainDocument->m_nANT[0]>0)
			m_Stockvalue=Cdat->totv;                        
		if (m_Stockvalue!=Cdat->totv)
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			if (m_Stockvalue>Cdat->totv)                                     
			{
				Cdat->nowv=m_Stockvalue - Cdat->totv;                         

				if(Cdat->kind!=SHZS&&Cdat->kind!=SZZS&&Cdat->kind!=EBZS)
				{
					if(Cdat->totv==0)
					{
						Cdat->dvol+=Cdat->nowv/2;                           
						Cdat->rvol+=Cdat->nowv/2;                         
						Cdat->InOut=2;
						SendCuoHeDataToDialog(Cdat);                                
					}
					else
					{
						if (Cdat->nowp >=OldSellP1&&pReport->m_fSellPrice[0]!=0)
						{
							Cdat->rvol+=Cdat->nowv;                           
							Cdat->InOut=0;
							SendCuoHeDataToDialog(Cdat);                     
						}
						else if (Cdat->nowp <=OldBuyP1&&pReport->m_fBuyPrice[0]!=0)
						{
							Cdat->dvol+=Cdat->nowv;                             
							Cdat->InOut=1;
							SendCuoHeDataToDialog(Cdat);                                
						}
						else 
						{
							Cdat->dvol+=Cdat->nowv/2;                     
							Cdat->rvol+=Cdat->nowv/2;                   
							Cdat->InOut=2;
							SendCuoHeDataToDialog(Cdat);                               
						}
					}
					chg=3;                                            
				}
			}
		}
	}

	if (m_Stockvalue != 0)
		Cdat->totv = m_Stockvalue;

	if (pReport->m_fAmount != 0 && pReport->m_fAmount != -1)
		Cdat->totp = pReport->m_fAmount;

	Cdat->time = pReport->m_time;

	if (Cdat->nowv < 0)
		Cdat->nowv = 0;

	if (pReport->m_fNewPrice > 0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Price = pReport->m_fNewPrice;
	if (pReport->m_fAmount > 0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Amount = pReport->m_fAmount;
	if (pReport->m_fVolume > 0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Volume = pReport->m_fVolume;

	if (Cdat->id[1] < 65)
		Cdat->lastclmin = m_MainDocument->m_nANT[Select_Market];

	AddDataCdat1(Cdat);

	//if (chg == 2 || chg == 3)
	{
		UpdateHs(Cdat, m_MainDocument->m_nANT[Select_Market]);
	}
}

void CSharesCompute::AddDataCdat1(CReportData *p)
{

	if(p->kind == SHZS || p->kind == SZZS)
		return;

	int nMarket = 0;
	if(p->kind == SZAG ||p->kind == SZZQ ||p->kind == SZJIJIN)
		nMarket = 1;

	int n = m_dataArr[nMarket].GetSize();
	SymbolKind sym;
	sym.m_nSymbolKind = p->kind ;
	strcpy(sym.m_chSymbol,p->id);

	for(int i = 0;i<n;i++)
	{
		if(strcmp(m_dataArr[nMarket][i].m_chSymbol,p->id) ==0 && p->kind == m_dataArr[nMarket][i].m_nSymbolKind)
		{
			m_dataArr[nMarket].RemoveAt(i);
			m_dataArr[nMarket].Add(sym);
			return;
		}
	}

	if(n>=MAX_COUNT_CDAT1)
		m_dataArr[nMarket].RemoveAt(0);
	m_dataArr[nMarket].Add(sym);


}

int CSharesCompute::CheckStockDaytime(RCV_HISTORY_STRUCTEx* pHistory, int nCount, int nIndex)
{
	int nPos = nIndex;
	do
	{
		nPos++;
	}
	while (pHistory[nPos].m_head.m_dwHeadTag != EKE_HEAD_TAG && nPos < nCount);
	nPos--;

	return nPos;
}

void CSharesCompute::StockDataDayUpdate(RCV_HISTORY_STRUCTEx* pHistory, int nCount)
{
	int nIndex = 0;

	if (pHistory->m_head.m_dwHeadTag != EKE_HEAD_TAG)
	{
		return;
	}

	do
	{
		int nEnd = CheckStockDaytime(pHistory, nCount, nIndex);
		if (nEnd <= nIndex && nIndex < nCount)
		{
			nIndex++;
			continue;
		}
		else if (nEnd <= nIndex)
		{
			break;
		}

		int KLineCount = 0;
		Kline* pDay = NULL;
		WORD wMarket = 0;
		CString StockId;
		int stkKind = -1;

		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc(GPTR, (nEnd - nIndex) * sizeof(Kline));
		hp = GlobalLock(hMem);
		if (hp)
		{
			pDay = (Kline*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
		}

		if (pHistory[nIndex].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
			wMarket = pHistory[nIndex].m_head.m_wMarket;
			StockId = pHistory[nIndex].m_head.m_szLabel;
			stkKind = CSharesInformation::GetStockKind(pHistory[nIndex].m_head.m_wMarket, pHistory[nIndex].m_head.m_szLabel);
		}

		nIndex++;

		do
		{
			pDay[KLineCount].day = pHistory[nIndex].m_time;
			pDay[KLineCount].open = pHistory[nIndex].m_fOpen;
			pDay[KLineCount].high = pHistory[nIndex].m_fHigh;
			pDay[KLineCount].low = pHistory[nIndex].m_fLow;
			pDay[KLineCount].close = pHistory[nIndex].m_fClose;
			pDay[KLineCount].vol = pHistory[nIndex].m_fVolume;
			pDay[KLineCount].amount = pHistory[nIndex].m_fAmount;
			pDay[KLineCount].advance = pHistory[nIndex].m_wAdvance;
			pDay[KLineCount].decline = pHistory[nIndex].m_wDecline;
			KLineCount++;
			nIndex++;
		}
		while (pHistory[nIndex].m_head.m_dwHeadTag != EKE_HEAD_TAG && nIndex < nCount);

		std::string symbol(StockId);

		CTaiKlineFileKLine* pFile = TSKDatabase()->GetKLineFile(wMarket);
		if (pFile)
		{
			pFile->WriteKLine(symbol, pDay, KLineCount, 0);
		}

		((CMainFrame*)AfxGetMainWnd())->UpdateData(83, (WPARAM)2, (LPARAM)StockId.GetBuffer());

		GlobalUnlock((HGLOBAL)pDay);
		GlobalFree((HGLOBAL)pDay);
	}
	while (nIndex < nCount);
}

void CSharesCompute::StockDataMinUpdate(RCV_MINUTE_STRUCTEx* pMinute, int nCount)                      
{
	if (pMinute->m_head.m_dwHeadTag != EKE_HEAD_TAG)
	{
		return;
	}

	int nIndex = 0;

	do
	{
		CReportData* Cdat = NULL;
		WORD wMarket = 0;
		CString StockId = _T("");
		int nKind = 0;

		if (pMinute[nIndex].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
			wMarket = pMinute[nIndex].m_head.m_wMarket;
			StockId = pMinute[nIndex].m_head.m_szLabel;
			StockId.MakeUpper();
			nKind = m_MainDocument->m_sharesInformation.GetStockKind(wMarket, StockId.GetBuffer(0));
			if (m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0), Cdat, nKind) != TRUE)
			{
				Cdat = NULL;
				nIndex++;
				continue;
			}
		}

		if (Cdat == NULL)                       
		{
			nIndex++;
			continue;
		}

		nIndex++;

		if (pMinute[nIndex].m_time < 0)
		{
			Cdat = NULL;
			nIndex++;
			continue;
		}

		int mode = 0;
		if (pMinute[nIndex].m_head.m_wMarket == SZ_MARKET_EX)
		{
			mode = 1;
		}

		short MinuteCount = 0;

		do
		{
			if (pMinute[nIndex].m_time > 240)
			{
				MinuteCount = GetStockMinute(pMinute[nIndex].m_time, mode + 1);
			}
			else
			{
				MinuteCount = pMinute[nIndex].m_time;
			}

			if (MinuteCount <= m_MainDocument->m_nANT[0])
			{
				if (pMinute[nIndex].m_fPrice != 0)
				{
					Cdat->m_Kdata1[MinuteCount].Price = pMinute[nIndex].m_fPrice;
					Cdat->m_Kdata1[MinuteCount].Amount = pMinute[nIndex].m_fAmount;
					Cdat->m_Kdata1[MinuteCount].Volume = pMinute[nIndex].m_fVolume;
				}
			}
			nIndex++;
		}
		while (pMinute[nIndex].m_head.m_dwHeadTag != EKE_HEAD_TAG && nIndex < nCount && MinuteCount < 240);

		if (MinuteCount < m_MainDocument->m_nANT[0] && MinuteCount > m_MainDocument->m_nANT[0] - 3)
		{
			for (int k = MinuteCount; k < m_MainDocument->m_nANT[0]; k++)
			{
				Cdat->m_Kdata1[k].Price = pMinute[nIndex - 1].m_fPrice;
				Cdat->m_Kdata1[k].Amount = pMinute[nIndex - 1].m_fAmount;
				Cdat->m_Kdata1[k].Volume = pMinute[nIndex - 1].m_fVolume;
			}
		}

		if (Cdat->kind == SZZS)
		{
			float max = 0;
			for (int i = 0; i < m_MainDocument->m_nANT[0]; i++)
			{
				if (Cdat->m_Kdata1[i].Volume > max)
				{
					max = Cdat->m_Kdata1[i].Volume;
				}
			}
			if (max > Cdat->totv)
			{
				int rate = 100;
				for (int i = 0; max / 100 > Cdat->totv; i++)
				{
					rate *= 100;
					max /= 100;
				}
				for (int i = 0; i < m_MainDocument->m_nANT[0]; i++)
				{
					Cdat->m_Kdata1[i].Volume /= rate;
				}
			}
		}

		Cdat->lastclmin = m_MainDocument->m_nANT[0] - 1;
		Cdat->initdown = TRUE;
		Cdat = NULL;
	}
	while (nIndex < nCount);
}

void CSharesCompute::StockDataPowerUpdate(RCV_POWER_STRUCTEx* pPower, int nCount)
{
	if (!pPower || pPower->m_head.m_dwHeadTag != EKE_HEAD_TAG || nCount <= 0)
		return;

	int nIndex = 0;

	do
	{
		CReportData* Cdat = NULL;
		WORD wMarket = 0;
		CString StockId = _T("");
		int nKind = 0;

		Split pSplit[80];
		int nNumber = 0;

		if (pPower[nIndex].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
			wMarket = pPower[nIndex].m_head.m_wMarket;
			StockId = pPower[nIndex].m_head.m_szLabel;
			StockId.MakeUpper();
			//nKind = m_MainDocument->m_sharesInformation.GetStockKind(wMarket, StockId.GetBuffer(0));
			//if (m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0), Cdat, nKind) != TRUE)
			//{
			//	Cdat = NULL;
			//	nIndex++;
			//	continue;
			//}
		}

		//if (Cdat == NULL)
		//{
		//	nIndex++;
		//	continue;
		//}

		nIndex++;

		//if (Cdat->pBaseInfo == NULL)
		//{
		//	BASEINFO* pBaseItem;
		//	if (!m_MainDocument->m_sharesInformation.AddBaseinfoPoint(Cdat->id, nKind, pBaseItem))
		//	{
		//		ASSERT(FALSE);
		//		return;
		//	}
		//	Cdat->pBaseInfo = pBaseItem;
		//}
		//else
		//{
		//	Cdat->pBaseInfo->NumSplit = 0;
		//}

		do
		{
			//Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].nTime = pPower[nIndex].m_time;
			//Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Give = pPower[nIndex].m_fGive;
			//Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Allocate = pPower[nIndex].m_fPei;
			//Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].AllocatePrice = pPower[nIndex].m_fPeiPrice;
			//Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Bonus = pPower[nIndex].m_fProfit;
			//Cdat->pBaseInfo->NumSplit++;
			pSplit[nNumber].nTime = pPower[nIndex].m_time;
			pSplit[nNumber].Give = pPower[nIndex].m_fGive;
			pSplit[nNumber].Allocate = pPower[nIndex].m_fPei;
			pSplit[nNumber].AllocatePrice = pPower[nIndex].m_fPeiPrice;
			pSplit[nNumber].Bonus = pPower[nIndex].m_fProfit;
			nNumber++;
			nIndex++;
		}
		while (nNumber < 80 && pPower[nIndex].m_head.m_dwHeadTag != EKE_HEAD_TAG && nIndex < nCount);

		//TSKDatabase()->UpdatePower(stkSymbol, pSplite, nNumber);
	}
	while (nIndex < nCount);
}

void CSharesCompute::StockDataTickUpdate(FOX_TICK* pTick, int nCount)
{
	if (!pTick || pTick->m_head.m_dwHeadTag != EKE_HEAD_TAG || nCount <= 0)
		return;

	CTaiKlineFileHS::WriteHsDataWideNet(pTick, nCount, 0);
}
