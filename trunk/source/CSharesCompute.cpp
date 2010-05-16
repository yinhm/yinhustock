
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
#include "CTaiKlineMemFile.h"
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
	pHsFile = CTaiKlineFileHS::GetFilePointer (s,Cdat->kind);
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

void CSharesCompute::StockDataPowerUpdate(POWER_TOTAL_STRUCTEx * m_GpPower)                               
{
	int index =0;
	static int steps=0;
	int nKind=0;
	if(m_GpPower->m_RcvPower->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
		CReportData * Cdat;
		Cdat=NULL;
		CString StockId;
		if( m_GpPower->m_RcvPower[index].m_head .m_dwHeadTag == EKE_HEAD_TAG )   
		{
			StockId=m_GpPower->m_RcvPower[index].m_head .m_szLabel ;          
            StockId.MakeUpper(); 
			nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpPower->m_RcvPower[index].m_head.m_wMarket,m_GpPower->m_RcvPower[index].m_head.m_szLabel);
			if(m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0) , Cdat,nKind) != TRUE)      
			{
				Cdat=NULL;
				index++;
				continue;
			}
		}
		if(Cdat==NULL)                        
		{
			index++;
			continue;
		}
		index++;
		if(Cdat->pBaseInfo==NULL)
		{
			 if(Cdat->pBaseInfo==NULL)
			 {
				BASEINFO *pBaseItem;
				if(!m_MainDocument->m_sharesInformation.AddBaseinfoPoint(Cdat->id,nKind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
				{
					AfxMessageBox("增加股票基本资料空间区域时出错！");
					return ;
				}
				Cdat->pBaseInfo=pBaseItem;
			 }
		}
		else
			 Cdat->pBaseInfo->NumSplit=0;

		do
		{
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].nTime= m_GpPower->m_RcvPower[index].m_time;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Give= m_GpPower->m_RcvPower[index].m_fGive;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Allocate= m_GpPower->m_RcvPower[index].m_fPei;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].AllocatePrice= m_GpPower->m_RcvPower[index].m_fPeiPrice;
            Cdat->pBaseInfo->m_Split[Cdat->pBaseInfo->NumSplit].Bonus= m_GpPower->m_RcvPower[index].m_fProfit;
            Cdat->pBaseInfo->NumSplit++;
			index++;
		}while(m_GpPower->m_RcvPower[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpPower->Power_Count);
	}while(index<m_GpPower->Power_Count);
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
#ifdef WIDE_NET_VERSION
void CSharesCompute::WideStockDataUpdate(RCV_WIDOFEREPORT_STRUCTEx* m_WideGpHqReport)                           //更新即时行情数据             
{
	CString m_Zqdm;
    CReportData  *Cdat;
	RCV_REPORT_STRUCTEx *m_GpHqReport=m_WideGpHqReport->pMarketReport;
	if(strstr(m_GpHqReport->m_szLabel," "))
		return;
	if(strlen(m_GpHqReport->m_szLabel)!=6&&strlen(m_GpHqReport->m_szLabel)!=4)
		return;
    m_GpHqReport->m_szName[8]='\0';
	int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpHqReport->m_wMarket,m_GpHqReport->m_szLabel);
    if(nKind==-1)
		return;
	if (this->m_MainDocument->m_sharesInformation.Lookup(m_GpHqReport->m_szLabel, Cdat,nKind) != TRUE)     
	{
         return; 
	} 
	// add
    if(Cdat->lastclmin<0 )
       Cdat->lastclmin=0;
	if(Cdat->lastclmin>240)
       Cdat->lastclmin=239;
    WideStockItemUpdate(Cdat, m_WideGpHqReport);                                   
}
void CSharesCompute::WideStockItemUpdate(CReportData  *Cdat,RCV_WIDOFEREPORT_STRUCTEx * m_WideGpHqReport)          
{
    int chg=0;
	short Select_Market;
	float m_Stockvalue;
	float OldBuyP1,OldSellP1;
	RCV_REPORT_STRUCTEx *m_GpHqReport=m_WideGpHqReport->pMarketReport;
	if(strcmp(Cdat->id,m_GpHqReport->m_szLabel)!=0)
		return;
    OldBuyP1=Cdat->pbuy1;
	OldSellP1=Cdat->psel1;
    CString StockId=m_GpHqReport->m_szLabel;
	if(m_GpHqReport->m_wMarket == SH_MARKET_EX)
       Select_Market=0;                                  
	else
	   Select_Market=1;  	 

	Cdat->initdown =TRUE;                                  
	if(m_GpHqReport->m_fLastClose !=0&& m_GpHqReport->m_fLastClose!=-1)
	   Cdat->ystc=m_GpHqReport->m_fLastClose;            

	bool bBuySell = false;
	if(Cdat ->ystc>0 && m_GpHqReport->m_fNewPrice>0)
	{
		float ff =( m_GpHqReport->m_fNewPrice - Cdat ->ystc )/Cdat ->ystc;
		if(ff>0.095 || ff<-0.095)
			bBuySell = true;
		if(Cdat->id[0] == 'S' && Cdat->id[1] == 'T')
			if(ff>0.047 || ff<-0.047)
				bBuySell = true;
	}
	

	if ((m_GpHqReport->m_fBuyPrice[0]>0 || bBuySell == true) && m_GpHqReport->m_fBuyPrice[0]!=-1)
	{
		if (m_GpHqReport->m_fBuyPrice[0]!=Cdat->pbuy1)                   
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->pbuy1 =m_GpHqReport->m_fBuyPrice[0];                     
			chg=1;                                                                                             
		}
	}
	
	if ((m_GpHqReport->m_fSellPrice[0] >0 || bBuySell == true) && m_GpHqReport->m_fSellPrice[0]!=-1)
	{
		if (m_GpHqReport->m_fSellPrice[0]!=Cdat->psel1)
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->psel1=m_GpHqReport->m_fSellPrice[0];                      
			chg=1;                                       
		}
	}
	if (m_GpHqReport->m_fHigh!=0&&m_GpHqReport->m_fHigh!=-1)                       
		Cdat->higp=m_GpHqReport->m_fHigh;          
	
	if (m_GpHqReport->m_fLow!=0&&m_GpHqReport->m_fLow!=-1)
		Cdat->lowp=m_GpHqReport->m_fLow;                
	
	if (m_GpHqReport->m_fOpen!=0&&m_GpHqReport->m_fOpen!=-1)
		Cdat->opnp=m_GpHqReport->m_fOpen;              

	if (m_GpHqReport->m_fNewPrice!=0&&m_GpHqReport->m_fNewPrice!=-1)                   
	{
		chk_rdp(Cdat,m_GpHqReport->m_fNewPrice);
		if (m_GpHqReport->m_fNewPrice!=Cdat->nowp)                 
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->nowp=m_GpHqReport->m_fNewPrice;                    
			chg=2;                                      
		}
	}
	bool bValid = false;
	for(int k = 0;k<3;k++)
	{
		if(m_GpHqReport->m_fBuyVolume[k]!=0) 
		{bValid = true;break;}
		if(m_GpHqReport->m_fSellVolume[k]!=0) 
		{bValid = true;break;}
	}

	if((m_GpHqReport->m_fBuyVolume[0]>0 || bValid == true) &&m_GpHqReport->m_fBuyVolume[0]!=-1 )          //得到买一量
	   Cdat->vbuy1=m_GpHqReport->m_fBuyVolume[0];

	if((m_GpHqReport->m_fBuyPrice[1]>0.01 ||m_GpHqReport->m_fBuyPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fBuyPrice[1] !=-1)           //得到买二价
	   Cdat->pbuy2=m_GpHqReport->m_fBuyPrice[1];          

	if((m_GpHqReport->m_fBuyVolume[1]>0 || bValid == true)&&m_GpHqReport->m_fBuyVolume[1] !=-1)          //得到买二量
	   Cdat->vbuy2=m_GpHqReport->m_fBuyVolume[1];

	if((m_GpHqReport->m_fBuyPrice [2]>0.01||m_GpHqReport->m_fBuyPrice[2] == 0 && bBuySell)  &&m_GpHqReport->m_fBuyPrice[2]!=-1)         //得到买三价
	   Cdat->pbuy3=m_GpHqReport->m_fBuyPrice [2];

	if((m_GpHqReport->m_fBuyVolume[2]>0 || bValid == true) && m_GpHqReport->m_fBuyVolume[2]!=-1)         //得到买三量
	   Cdat->vbuy3=m_GpHqReport->m_fBuyVolume [2];

	if((m_GpHqReport->m_fSellVolume[0]>0 || bValid == true)&& m_GpHqReport->m_fBuyVolume[0]!=-1)      //得到卖一量
	   Cdat->vsel1=m_GpHqReport->m_fSellVolume [0];

	if((m_GpHqReport->m_fSellPrice [1]>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell)   &&m_GpHqReport->m_fSellPrice[1]!=-1)       //得到卖二价
	   Cdat->psel2=m_GpHqReport->m_fSellPrice [1];          

	if((m_GpHqReport->m_fSellVolume[1]>0 || bValid == true)&&m_GpHqReport->m_fSellVolume[1]!=-1)      //得到卖二量
	   Cdat->vsel2=m_GpHqReport->m_fSellVolume [1];

	if((m_GpHqReport->m_fSellPrice [2]>0.001||m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) &&m_GpHqReport->m_fSellPrice [2]!=-1)       //得到卖三价
	   Cdat->psel3=m_GpHqReport->m_fSellPrice [2];

	if((m_GpHqReport->m_fSellVolume[2]>0 || bValid == true) &&m_GpHqReport->m_fSellVolume[2]!=-1)        //得到卖三量
	  Cdat->vsel3=m_GpHqReport->m_fSellVolume[2];

	m_Stockvalue=m_GpHqReport->m_fVolume  ;                    
	if (m_GpHqReport->m_fVolume>0)
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
				chg=3;                                                 
			}
		}
	}
	if(m_Stockvalue!=0)
	   Cdat->totv=m_Stockvalue;                                                
	if (m_GpHqReport->m_fAmount!=0&&m_GpHqReport->m_fAmount!=-1)                         
	   Cdat->totp =m_GpHqReport->m_fAmount;

	Cdat->rvol=m_WideGpHqReport->m_fActiveBuyVolumn;
	Cdat->dvol=Cdat->totv - Cdat->rvol;
	
	if (Cdat->nowv<0)
		Cdat->nowv=0;
}
void CSharesCompute::WideStockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute)                              
{
	int index =0;
	static int steps=0;
	if(m_GpMinute->m_RcvMinute->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
		CReportData * Cdat;
		Cdat=NULL;
		CString StockId;
		if( m_GpMinute->m_RcvMinute[index].m_head .m_dwHeadTag == EKE_HEAD_TAG )   
		{

			StockId=m_GpMinute->m_RcvMinute[index].m_head.m_szLabel ;          
            StockId.MakeUpper(); 
			int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket,m_GpMinute->m_RcvMinute[index].m_head.m_szLabel);
			if(m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0) , Cdat,nKind) != TRUE)       
			{
				Cdat=NULL;
				index++;
				continue;
			}
		}
		if(Cdat==NULL)                      
		{
			index++;
			continue;
		}
		index++;
		int mode=0;
		if(m_GpMinute->m_RcvMinute[index].m_time<0)        
		{
			Cdat=NULL;
			index++;
			continue;
		}
		if(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket ==SZ_MARKET_EX)    
			mode=1;
        short MinuteCount=0;
		do
		{
			if(m_GpMinute->m_RcvMinute[index].m_time>240)
			{
			   MinuteCount=GetStockMinute(m_GpMinute->m_RcvMinute[index].m_time,mode+1   ); 
			}
			else
			{
			   MinuteCount=m_GpMinute->m_RcvMinute[index].m_time;
			}
			if(MinuteCount<=m_MainDocument->m_nANT[0])
			{
                if(m_GpMinute->m_RcvMinute[index].m_fPrice!=0)
				{
					Cdat->m_Kdata1[MinuteCount].Price   =m_GpMinute->m_RcvMinute[index].m_fPrice ;
					Cdat->m_Kdata1[MinuteCount].Amount  =m_GpMinute->m_RcvMinute[index].m_fAmount;
					Cdat->m_Kdata1[MinuteCount].Volume  =m_GpMinute->m_RcvMinute[index].m_fVolume;
				}
			}
			index++;
		}while(m_GpMinute->m_RcvMinute[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpMinute->Min_Count&&MinuteCount < 240);
		Cdat->lastclmin =MinuteCount ;
		Cdat->initdown =TRUE;
		Cdat=NULL;
	}while(index<m_GpMinute->Min_Count);
}
#else
void CSharesCompute::StockDataUpdate(RCV_REPORT_STRUCTEx* m_GpHqReport)
{
	CString m_Zqdm;
	CReportData  *Cdat;
	if(strstr(m_GpHqReport->m_szLabel," "))
		return;

	if(strlen(m_GpHqReport->m_szLabel)!=6&&strlen(m_GpHqReport->m_szLabel)!=4)
		return;


	m_GpHqReport->m_szName[8]='\0';
	int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpHqReport->m_wMarket,m_GpHqReport->m_szLabel);
	if(nKind==-1)
		return;
	if (this->m_MainDocument->m_sharesInformation.Lookup(m_GpHqReport->m_szLabel, Cdat,nKind) != TRUE)     
	{
		if(!m_MainDocument->m_sharesInformation.InsertItem(m_GpHqReport->m_szLabel,Cdat,nKind))
			return;
		m_MainDocument->m_WsStock2000View->LoadShowData(nKind);              
		strcpy(Cdat->name ,m_GpHqReport->m_szName );
		strcpy(Cdat->id ,m_GpHqReport->m_szLabel );
		Cdat->kind=nKind;
		Cdat->wMarket = m_GpHqReport->m_wMarket;
		if(strlen(Cdat->name)>0)
		{
			m_MainDocument->StockNameConvert(Cdat->name,Cdat->Gppyjc) ;
			m_MainDocument->AddStockToKeyboard(Cdat->id,Cdat->kind,Cdat->name,Cdat->Gppyjc);
		}
		m_MainDocument->m_WsStock2000View->AddGridRow(nKind);             

		STOCK_TYPE_INFO *m_pStock;
		if(m_MainDocument->m_ManagerStockTypeData.Lookup(Cdat->id,Cdat->kind,m_pStock))
		{
			Cdat->pStockTypeInfo= m_pStock;
		}
		BASEINFO *m_pStockBase;
		if(m_MainDocument->m_sharesInformation.LookupBase(Cdat->id,nKind,m_pStockBase))
		{
			Cdat->pBaseInfo=m_pStockBase;
		}
	} 
	m_Zqdm=Cdat->id;

	if(Cdat->ystc!=0)
	{
		if(fabs(m_GpHqReport->m_fNewPrice)<0.01 )
		{
			if(strlen(m_GpHqReport->m_szName)>0&&strcmp(Cdat->name,m_GpHqReport->m_szName)!=0 )
			{
				m_MainDocument->StockNameConvert(m_GpHqReport->m_szName,Cdat->Gppyjc) ;
				strcpy(Cdat->name , m_GpHqReport->m_szName );
				Cdat->opnp=m_GpHqReport->m_fOpen;
				m_MainDocument->AddStockToKeyboard(Cdat->id,Cdat->kind,Cdat->name,Cdat->Gppyjc);
			}
			return;
		}
	}
	if(Cdat->lastclmin<0 )
		Cdat->lastclmin=0;
	if(Cdat->lastclmin>240)
		Cdat->lastclmin=239;
	StockItemUpdate(Cdat, m_GpHqReport);                                    
}
void CSharesCompute::StockItemUpdate(CReportData  *Cdat,RCV_REPORT_STRUCTEx* m_GpHqReport)         
{
	int chg=0;
	short Select_Market;
	float m_Stockvalue,ltmp;
	float OldBuyP1,OldSellP1;
	if(strcmp(Cdat->id,m_GpHqReport->m_szLabel)!=0)
		return;
	OldBuyP1=Cdat->pbuy1;
	OldSellP1=Cdat->psel1;
	CString StockId=m_GpHqReport->m_szLabel;
	if(m_MainDocument->m_pStockBigBillData&&m_MainDocument->m_pStockBigBillData->m_pBigBillHead->Done)
	{
		if(Cdat->IsMxTj)
		{ 
			m_MainDocument->m_pStockBigBillData->CalcBigBillData(Cdat, m_GpHqReport);
		}
	}
	if(strlen(m_GpHqReport->m_szName)>0&&strcmp(Cdat->name,m_GpHqReport->m_szName)!=0 )
	{
		m_MainDocument->StockNameConvert(m_GpHqReport->m_szName,Cdat->Gppyjc) ;
		strcpy(Cdat->name , m_GpHqReport->m_szName );
		m_MainDocument->AddStockToKeyboard(Cdat->id,Cdat->kind,Cdat->name,Cdat->Gppyjc);
	}


	//
	if(m_GpHqReport->m_wMarket == SH_MARKET_EX)
		Select_Market=0;                                    
	else
		Select_Market=1;  		    
	for( int i=Cdat->lastclmin   ; i < m_MainDocument->m_nANT[Select_Market]  ;i++ )    
	{
		if(Cdat->lastclmin==0 && Cdat->initdown ==FALSE && m_MainDocument->m_nANT[Select_Market] > 0)     
		{
			Cdat->m_Kdata1[i].Amount =0;
			Cdat->m_Kdata1[i].Price =0 ;
			Cdat->m_Kdata1[i].Volume =0;
		}
		else
		{
			Cdat->m_Kdata1[i].Amount =Cdat->m_Kdata1 [Cdat->lastclmin  ].Amount;
			Cdat->m_Kdata1[i].Price =Cdat->m_Kdata1 [Cdat->lastclmin  ].Price ;
			Cdat->m_Kdata1[i].Volume =Cdat->m_Kdata1 [Cdat->lastclmin ].Volume ;
		}
	}
	if(Cdat->id[1]=='A')
	{
		Rsdn1 **Nidx2;
		Tidxd **Tidx2;
		m_MainDocument->m_sharesInformation.GetIndexRsdn(Nidx2); 
		m_MainDocument->m_sharesInformation.GetIndexTidxd(Tidx2); 
		for( short i=Cdat->lastclmin ; i < m_MainDocument->m_nANT[Select_Market]  ;i++ )     
		{
			if(Cdat->lastclmin==0 && Cdat->initdown ==FALSE&&m_MainDocument->m_nANT[Select_Market] > 0)     
			{
				Nidx2[Select_Market][i].rsn=0;
				Nidx2[Select_Market][i].dnn=0;
			}
			else
			{
				Nidx2[Select_Market][i].rsn=Nidx2[Select_Market][Cdat->lastclmin].rsn;
				Nidx2[Select_Market][i].dnn=Nidx2[Select_Market][Cdat->lastclmin].dnn;
			}
		}        
	}
	if(!Cdat->initdown)
		m_MainDocument->InitFiveDaysVolumnForStock(Cdat);
	Cdat->initdown =TRUE;                                 
	if(m_GpHqReport->m_fLastClose !=0&& m_GpHqReport->m_fLastClose!=-1)
		Cdat->ystc=m_GpHqReport->m_fLastClose;          


	{
	}
	bool bBuySell = false;
	if(Cdat ->ystc>0 && m_GpHqReport->m_fNewPrice>0)
	{
		float ff =( m_GpHqReport->m_fNewPrice - Cdat ->ystc )/Cdat ->ystc;
		if(ff>0.095 || ff<-0.095)
			bBuySell = true;
		if(Cdat->id[0] == 'S' && Cdat->id[1] == 'T')
			if(ff>0.047 || ff<-0.047)
				bBuySell = true;
	}


	if(Cdat->kind!=SHZS&&Cdat->kind!=SZZS&&Cdat->kind!=EBZS)
	{
		if ((m_GpHqReport->m_fBuyPrice[0]>0 || bBuySell == true) && m_GpHqReport->m_fBuyPrice[0]!=-1)
		{
			if (m_GpHqReport->m_fBuyPrice[0]!=Cdat->pbuy1)                   
			{
				if (m_MainDocument->m_nANT[Select_Market]<0)
					m_MainDocument->m_nANT[Select_Market]=0;
				Cdat->pbuy1 =m_GpHqReport->m_fBuyPrice[0];                    
				chg=1;                                                                                             
			}
		}

		if ((m_GpHqReport->m_fSellPrice[0] >0 || bBuySell == true) && m_GpHqReport->m_fSellPrice[0]!=-1)
		{
			if (m_GpHqReport->m_fSellPrice[0]!=Cdat->psel1)
			{
				if (m_MainDocument->m_nANT[Select_Market]<0)
					m_MainDocument->m_nANT[Select_Market]=0;
				Cdat->psel1=m_GpHqReport->m_fSellPrice[0];                  
				chg=1;                                       
			}
		}
	}
	if (m_GpHqReport->m_fHigh!=0&&m_GpHqReport->m_fHigh!=-1)                        
		Cdat->higp=m_GpHqReport->m_fHigh;          

	if (m_GpHqReport->m_fLow!=0&&m_GpHqReport->m_fLow!=-1)
		Cdat->lowp=m_GpHqReport->m_fLow;               

	if (m_GpHqReport->m_fOpen!=0&&m_GpHqReport->m_fOpen!=-1)
		Cdat->opnp=m_GpHqReport->m_fOpen;               

	if (m_GpHqReport->m_fNewPrice!=0&&m_GpHqReport->m_fNewPrice!=-1)                  
	{
		chk_rdp(Cdat,m_GpHqReport->m_fNewPrice);
		if (m_GpHqReport->m_fNewPrice!=Cdat->nowp)                  
		{
			if (m_MainDocument->m_nANT[Select_Market]<0)
				m_MainDocument->m_nANT[Select_Market]=0;
			Cdat->nowp=m_GpHqReport->m_fNewPrice;               
			chg=2;                                    
		}

	}


	bool bValid = false;
	for(int k = 0;k<3;k++)
	{
		if(fabs(m_GpHqReport->m_fBuyVolume[k])>=0.5) 
		{bValid = true;break;}
		if(fabs(m_GpHqReport->m_fSellVolume[k])>=0.5) 
		{bValid = true;break;}
	}

	// 非指数类型证券
	if (Cdat->kind != SHZS && Cdat->kind != SZZS && Cdat->kind != EBZS)
	{
		if ((m_GpHqReport->m_fBuyVolume[0] > 0 || bValid == TRUE) && m_GpHqReport->m_fBuyVolume[0] != -1)
			Cdat->vbuy1 = m_GpHqReport->m_fBuyVolume[0];

		if ((m_GpHqReport->m_fBuyPrice[1] > 0.01 || m_GpHqReport->m_fBuyPrice[1] == 0 && bBuySell) && m_GpHqReport->m_fBuyPrice[1] != -1)
			Cdat->pbuy2 = m_GpHqReport->m_fBuyPrice[1];

		if ((m_GpHqReport->m_fBuyVolume[1] > 0 || bValid == TRUE) && m_GpHqReport->m_fBuyVolume[1] != -1)
			Cdat->vbuy2 = m_GpHqReport->m_fBuyVolume[1];

		if ((m_GpHqReport->m_fBuyPrice[2] > 0.01 || m_GpHqReport->m_fBuyPrice[2] == 0 && bBuySell) && m_GpHqReport->m_fBuyPrice[2] != -1)
			Cdat->pbuy3 = m_GpHqReport->m_fBuyPrice[2];

		if ((m_GpHqReport->m_fBuyVolume[2] > 0 || bValid == TRUE) && m_GpHqReport->m_fBuyVolume[2] != -1)
			Cdat->vbuy3 = m_GpHqReport->m_fBuyVolume[2];

		if ((m_GpHqReport->m_fBuyPrice[3] > 0.01 || m_GpHqReport->m_fBuyPrice[3] == 0 && bBuySell) && m_GpHqReport->m_fBuyPrice[3] != -1)
			Cdat->pbuy4 = m_GpHqReport->m_fBuyPrice4;

		if ((m_GpHqReport->m_fBuyVolume[3] > 0 || bValid == TRUE) && m_GpHqReport->m_fBuyVolume[3] != -1)
			Cdat->vbuy4 = m_GpHqReport->m_fBuyVolume4;

		if ((m_GpHqReport->m_fBuyPrice[4] > 0.01 || m_GpHqReport->m_fBuyPrice[4] == 0 && bBuySell) && m_GpHqReport->m_fBuyPrice[4] != -1)
			Cdat->pbuy5 = m_GpHqReport->m_fBuyPrice5;

		if ((m_GpHqReport->m_fBuyVolume[4] > 0 || bValid == TRUE) && m_GpHqReport->m_fBuyVolume[4] != -1)
			Cdat->vbuy5 = m_GpHqReport->m_fBuyVolume5;


		if ((m_GpHqReport->m_fSellVolume[0] > 0 || bValid == TRUE) && m_GpHqReport->m_fBuyVolume[0] != -1)
			Cdat->vsel1 = m_GpHqReport->m_fSellVolume[0];

		if ((m_GpHqReport->m_fSellPrice[1] > 0.001 || m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) && m_GpHqReport->m_fSellPrice[1] != -1)
			Cdat->psel2 = m_GpHqReport->m_fSellPrice[1];          

		if ((m_GpHqReport->m_fSellVolume[1] > 0 || bValid == TRUE) && m_GpHqReport->m_fSellVolume[1] != -1)
			Cdat->vsel2 = m_GpHqReport->m_fSellVolume[1];

		if ((m_GpHqReport->m_fSellPrice[2] > 0.001 || m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) && m_GpHqReport->m_fSellPrice[2] != -1)
			Cdat->psel3 = m_GpHqReport->m_fSellPrice[2];

		if ((m_GpHqReport->m_fSellVolume[2] > 0 || bValid == TRUE) && m_GpHqReport->m_fSellVolume[2] != -1)
			Cdat->vsel3 = m_GpHqReport->m_fSellVolume[2];

		if ((m_GpHqReport->m_fSellPrice[3] > 0.001 || m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) && m_GpHqReport->m_fSellPrice[3] != -1)
			Cdat->psel4 = m_GpHqReport->m_fSellPrice4;

		if ((m_GpHqReport->m_fSellVolume[3] > 0 || bValid == TRUE) && m_GpHqReport->m_fSellVolume[3] != -1)
			Cdat->vsel4 = m_GpHqReport->m_fSellVolume4;

		if ((m_GpHqReport->m_fSellPrice[4] > 0.001 || m_GpHqReport->m_fSellPrice[1] == 0 && bBuySell) && m_GpHqReport->m_fSellPrice[4] != -1)
			Cdat->psel5 = m_GpHqReport->m_fSellPrice5;

		if ((m_GpHqReport->m_fSellVolume[4] > 0 || bValid == TRUE) && m_GpHqReport->m_fSellVolume[4] != -1)
			Cdat->vsel5 = m_GpHqReport->m_fSellVolume5;


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
	m_Stockvalue=m_GpHqReport->m_fVolume  ;                 
	if (m_GpHqReport->m_fVolume>0)
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
						if (Cdat->nowp >=OldSellP1&&m_GpHqReport->m_fSellPrice[0]!=0)
						{
							Cdat->rvol+=Cdat->nowv;                           
							Cdat->InOut=0;
							SendCuoHeDataToDialog(Cdat);                     
						}
						else if (Cdat->nowp <=OldBuyP1&&m_GpHqReport->m_fBuyPrice[0]!=0)
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
	if(m_Stockvalue!=0)
		Cdat->totv=m_Stockvalue;                                           
	if (m_GpHqReport->m_fAmount!=0&&m_GpHqReport->m_fAmount!=-1)                     
		Cdat->totp =m_GpHqReport->m_fAmount;


	if (Cdat->nowv<0)
		Cdat->nowv=0;


	if(m_GpHqReport->m_fNewPrice >0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Price=m_GpHqReport->m_fNewPrice ;
	if(m_GpHqReport->m_fAmount >0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Amount= m_GpHqReport->m_fAmount ;
	if(m_GpHqReport->m_fVolume >0)
		Cdat->m_Kdata1[m_MainDocument->m_nANT[Select_Market]].Volume =m_GpHqReport->m_fVolume ;
	if(Cdat->id[1] <65) 
		//if (chg==2 || chg==3)                                             
		{
			UpdateHs(Cdat,m_MainDocument->m_nANT[Select_Market]);
		}
		Cdat->lastclmin=m_MainDocument->m_nANT[Select_Market];

		AddDataCdat1(Cdat);
}
void CSharesCompute::StockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute)                      
{
	int index =0;
	static int steps=0;
	if(m_GpMinute->m_RcvMinute->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
		CReportData * Cdat;
		Cdat=NULL;
		CString StockId;
		if( m_GpMinute->m_RcvMinute[index].m_head .m_dwHeadTag == EKE_HEAD_TAG )  
		{

			StockId=m_GpMinute->m_RcvMinute[index].m_head.m_szLabel ;        
			StockId.MakeUpper(); 
			int nKind=m_MainDocument->m_sharesInformation.GetStockKind(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket,m_GpMinute->m_RcvMinute[index].m_head.m_szLabel);
			if(m_MainDocument->m_sharesInformation.Lookup(StockId.GetBuffer(0) , Cdat,nKind) != TRUE)      
			{
				Cdat=NULL;
				index++;
				continue;
			}
		}
		if(Cdat==NULL)                       
		{
			index++;
			continue;
		}
		index++;
		int mode=0;
		if(m_GpMinute->m_RcvMinute[index].m_time<0)        
		{
			Cdat=NULL;
			index++;
			continue;
		}
		if(m_GpMinute->m_RcvMinute[index].m_head.m_wMarket ==SZ_MARKET_EX)   
			mode=1;
		short MinuteCount=0;
		do
		{
			if(m_GpMinute->m_RcvMinute[index].m_time>240)
			{
				MinuteCount=GetStockMinute(m_GpMinute->m_RcvMinute[index].m_time,mode+1   ); 
			}
			else
			{
				MinuteCount=m_GpMinute->m_RcvMinute[index].m_time;
			}
			if(MinuteCount<=m_MainDocument->m_nANT[0])
			{
				if(m_GpMinute->m_RcvMinute[index].m_fPrice!=0)
				{
					Cdat->m_Kdata1[MinuteCount].Price   =m_GpMinute->m_RcvMinute[index].m_fPrice ;
					Cdat->m_Kdata1[MinuteCount].Amount  =m_GpMinute->m_RcvMinute[index].m_fAmount;
					Cdat->m_Kdata1[MinuteCount].Volume  =m_GpMinute->m_RcvMinute[index].m_fVolume;
				}
			}
			index++;
		}while(m_GpMinute->m_RcvMinute[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpMinute->Min_Count&&MinuteCount < 240);
		if(MinuteCount < m_MainDocument->m_nANT[0] && MinuteCount > m_MainDocument->m_nANT[0]-3)
		{
			for(int k=MinuteCount ;k<m_MainDocument->m_nANT[0];k++)
			{
				Cdat->m_Kdata1[k].Price   =m_GpMinute->m_RcvMinute[index - 1].m_fPrice ;
				Cdat->m_Kdata1[k].Amount  =m_GpMinute->m_RcvMinute[index - 1].m_fAmount;
				Cdat->m_Kdata1[k].Volume  =m_GpMinute->m_RcvMinute[index - 1].m_fVolume;
			}
		}
		if( Cdat->kind == SZZS)
		{
			float max = 0;
			for(int i = 0 ;i<m_MainDocument->m_nANT[0];i++)
			{
				if(Cdat->m_Kdata1[i].Volume >max)
					max = Cdat->m_Kdata1[i].Volume;
			}
			if(max>Cdat->totv )
			{
				int rate = 100;
				for(int i = 0;max/100>Cdat->totv;i++)
				{
					rate*=100;
					max/=100;
				}
				for(int i = 0 ;i<m_MainDocument->m_nANT[0];i++)
				{
					Cdat->m_Kdata1[i].Volume /= rate;
				}
			}
		}
		Cdat->lastclmin =m_MainDocument->m_nANT[0] - 1 ;
		Cdat->initdown =TRUE;
		Cdat=NULL;
	}while(index<m_GpMinute->Min_Count);
}
#endif	

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

int CSharesCompute::CheckStockDaytime(DAY_TOTAL_STRUCTEx* m_GpDay, int index)
{
	int ls_pos = index;
	do
	{
		ls_pos++;
	}
	while (m_GpDay->m_RcvDay[ls_pos].m_head.m_dwHeadTag != EKE_HEAD_TAG && ls_pos < m_GpDay->Day_Count);
	ls_pos--;

	return ls_pos;
}

void CSharesCompute::StockDataDayUpdate(DAY_TOTAL_STRUCTEx* m_GpDay)
{
	int index = 0;
	if (m_GpDay->m_RcvDay->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;

	do
	{
		int endindex = CheckStockDaytime(m_GpDay, index);
		if (endindex <= index && index < m_GpDay->Day_Count)
		{
			index++;
			continue;
		}
		else if (endindex <= index)
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
		hMem = GlobalAlloc(GPTR, (endindex - index) * sizeof(Kline));
		hp = GlobalLock(hMem);
		if (hp)
		{
			pDay = (Kline*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
		}

		if (m_GpDay->m_RcvDay[index].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
			wMarket = m_GpDay->m_RcvDay[index].m_head.m_wMarket;
			StockId = m_GpDay->m_RcvDay[index].m_head.m_szLabel;
			stkKind = CSharesInformation::GetStockKind(m_GpDay->m_RcvDay[index].m_head.m_wMarket, m_GpDay->m_RcvDay[index].m_head.m_szLabel);
		}

		index++;

		do
		{
			pDay[KLineCount].day = m_GpDay->m_RcvDay[index].m_time;
			pDay[KLineCount].open = m_GpDay->m_RcvDay[index].m_fOpen;
			pDay[KLineCount].high = m_GpDay->m_RcvDay[index].m_fHigh;
			pDay[KLineCount].low = m_GpDay->m_RcvDay[index].m_fLow;
			pDay[KLineCount].close = m_GpDay->m_RcvDay[index].m_fClose;
			pDay[KLineCount].vol = m_GpDay->m_RcvDay[index].m_fVolume;
			pDay[KLineCount].amount = m_GpDay->m_RcvDay[index].m_fAmount;
			pDay[KLineCount].advance = m_GpDay->m_RcvDay[index].m_wAdvance;
			pDay[KLineCount].decline = m_GpDay->m_RcvDay[index].m_wDecline;
			KLineCount++;
			index++;
		}
		while (m_GpDay->m_RcvDay[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index < m_GpDay->Day_Count);

		//CTaiKlineFileKLine::GetFilePointer(StockId, stkKind, TRUE)->WriteKLine(StockId, pDay, KLineCount, 0);
		CTaiKlineFileKLine* pFile = TSKDatabase()->GetKLineFile(wMarket);
		ASSERT(pFile);
		if (pFile)
			pFile->WriteKLine(StockId, pDay, KLineCount, 0);

		GlobalUnlock((HGLOBAL)pDay);
		GlobalFree((HGLOBAL)pDay);
	}
	while (index < m_GpDay->Day_Count);
}
