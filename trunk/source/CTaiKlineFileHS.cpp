
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineFileHS.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"

#include "CTaiKlineDoFenshi.h"
#include "ProgressDialog.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineTransferKline.h"


#define TSK_FILEHEADER_SIZE			16
#define HsSmallHeadByteEach			sizeof(TSK_TICKINDEX)
#define FixedHsPerBlock				240
#define HsByteEach					sizeof(TRADE_DETAIL_H_PER)
#define SMALLHEADLENGTH				TSK_FILEHEADER_SIZE + HsSmallHeadByteEach * MAX_STOCK_NUM


CTaiKlineFileHS* CTaiKlineFileHS::m_fileHsSh = NULL;
CTaiKlineFileHS* CTaiKlineFileHS::m_fileHsSz = NULL;


CTaiKlineFileHS::CTaiKlineFileHS()
{
	m_nMarket = SZ_MARKET_EX;
	m_bToday = TRUE;
	m_bIndex = FALSE;
}

CTaiKlineFileHS::CTaiKlineFileHS(int nMarket, BOOL bToday)
{
	ASSERT(nMarket!=0 && nMarket != 1);
	m_nMarket = nMarket;
	m_bToday = bToday;

	m_nAddReMap = HsByteEach * FixedHsPerBlock * 5;

	pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	m_bIndex = FALSE;
}

CTaiKlineFileHS::~CTaiKlineFileHS()
{
	DeleteMap();
}

int CTaiKlineFileHS::ReadHS(CString symbol,CBuySellList& buySellList,bool bClearAll)
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;
	if(CTaiShanKlineShowView::IsIndexStock(symbol )&& bClearAll == false) return buySellList.GetCount();

	int n = ReadHS2( symbol,buySellList,  bClearAll);
	if(bClearAll == true)
	{
		if(CTaiShanKlineShowView::IsIndexStock(symbol ))
		{
			ReadIndexHS(symbol,buySellList,bClearAll);
			return	buySellList.GetCount ();
		}

#ifndef WIDE_NET_VERSION
		TSK_TICKINDEX tickIndex;
		TSK_TICKINDEX* pTickIndex = &tickIndex;

		std::string code(symbol);
		int nIndexStock = GetTickIndex(code,pTickIndex);

		TRADE_DETAIL_H_PER hsFirst;
		TRADE_DETAIL_H_PER* pHs = NULL;
		if(n>0)
		{
			pHs = GetAtBS(&buySellList, 0,n);
		}

		char *strLabel = symbol.GetBuffer (0);
		int nMarket = m_nMarket;
		int stkKind = CSharesInformation::GetStockKind(nMarket,strLabel);

		CReportData* pCdat = NULL;
		CBuySellList buySellList2;
		if(!pDoc->m_sharesInformation .Lookup(symbol.GetBuffer (0),pCdat,stkKind))
			return n;


		bool bNew = false;
		if(pHs ==NULL )
		{
			if(pCdat->nowp <=0 ) return n;
			pHs = new TRADE_DETAIL_H_PER;
			memset(pHs,0,sizeof(TRADE_DETAIL_H_PER));
			CTime tm = CTime::GetCurrentTime ();
			pHs->time = tm.GetTime ();
			if(tm.GetHour ()>=15 || (tm.GetHour ()<9 || tm.GetHour ()==9 && tm.GetMinute ()<26))
			{
				pHs->time = CTime(tm.GetYear (),tm.GetMonth  (),tm.GetDay  (),15,0,0).GetTime();
			}
			pHs->vol = pCdat->totv ;
			pHs->price = pCdat->nowp ;
			buySellList.AddTail (pHs);
			bNew = true;


		}

		CTime tm(pHs->time);
		CTime tmb (tm.GetYear (),tm.GetMonth (),tm.GetDay(),9,30,0);
		int nb = tmb.GetTime ();
		float oldPrice = pCdat->m_Kdata1[0].Price ;

		int footEnd = pDoc->m_nANT [0]+1;
		if(pDoc->m_bInitDone ==FALSE)
			footEnd=pDoc->m_nOldANT [0]+1;
		if(footEnd>240) footEnd = 240;
		for(int i=0;i<footEnd;i++)
		{
			if(pCdat->m_Kdata1[i].Volume<=0.00001 ) continue;
			if(i>0)
				if(pCdat->m_Kdata1[i].Volume == pCdat->m_Kdata1[i-1].Volume) continue;
			;

			if(pCdat->m_Kdata1[i].Price >0 
				&& (pHs->vol )>pCdat->m_Kdata1[i].Volume )
			{
				int i2 = i;
				if(i>=120)
					i2 = i+90;
				if(i2*60+nb>=pHs->time )
					break;

				TRADE_DETAIL_H_PER* pHs2 = new TRADE_DETAIL_H_PER;
				memset(pHs2,0,sizeof(TRADE_DETAIL_H_PER));
				pHs2->time = i2*60+nb;
				pHs2->price  = pCdat->m_Kdata1[i].Price  ;
				pHs2->vol = pCdat->m_Kdata1[i].Volume ;
				if(pHs2->vol <0) pHs2->vol =0;

				oldPrice = pHs2->price;
				buySellList2.AddHead (pHs2);
			}
			else
				break;
		}
		POSITION pos = buySellList2.GetHeadPosition ();
		while(pos)
		{

			buySellList.AddTail (buySellList2.GetNext (pos));
		}


		if(buySellList2.GetCount ()>0 || bNew ==true)
			WriteHS2(symbol,buySellList);
#endif
	}
	return buySellList.GetCount ();
}
int CTaiKlineFileHS::ReadHSPeriod(CString symbol,CBuySellList& buySellList, CTime timeStart, CTime timeEnd,bool bClearAll)//读取一段时间的K线数据

{
	return 0;
}

void CTaiKlineFileHS::Cdat1ToHs(CReportData *pCdat, TRADE_DETAIL_H_PER *pHs,bool bToHs, TRADE_DETAIL_H_PER *pHsPre)
{
	float* pPriceVol[6][2];
	int i;
	pPriceVol[0][0] = &(pCdat->pbuy1) ;pPriceVol[1][0] = &(pCdat->pbuy2) ;pPriceVol[2][0] = &(pCdat->pbuy3) ;
	pPriceVol[3][0] = &(pCdat->psel1) ;pPriceVol[4][0] = &(pCdat->psel2) ;pPriceVol[5][0] = &(pCdat->psel3) ;
	pPriceVol[0][1] = &(pCdat->vbuy1) ;pPriceVol[1][1] = &(pCdat->vbuy2) ;pPriceVol[2][1] = &(pCdat->vbuy3) ;
	pPriceVol[3][1] = &(pCdat->vsel1) ;pPriceVol[4][1] = &(pCdat->vsel2) ;pPriceVol[5][1] = &(pCdat->vsel3) ;

	if(bToHs==true)
	{
		pHs->price = pCdat->nowp ;
		pHs->vol = pCdat->totv  ;

		float maxFloat = 0;
		for(i=0;i<6;i++)
		{
			if(*pPriceVol[i][0]>=0.001 && FABSMY(*pPriceVol[i][0]- pHs->price)>maxFloat)
				maxFloat = FABSMY(*pPriceVol[i][0]- pHs->price);
		}
		pHs->buySellOrIndex .buySellInfo .unitCountPrc = maxFloat ;
		float temp ;


		maxFloat = pCdat->vsel1 ;
		for(i=0;i<6;i++)
		{
			if(*pPriceVol[i][1]>maxFloat)	maxFloat = *pPriceVol[i][1];
		}
		pHs->buySellOrIndex .buySellInfo .unitCountVol = maxFloat ;

		if(pHs->buySellOrIndex .buySellInfo .unitCountPrc<=0)
		{
			for(i=0;i<6;i++)
				pHs->buySellOrIndex .buySellInfo .buyP [i] =0;
		}
		else
		{
			for(i=0;i<6;i++)
			{
				if(*pPriceVol[i][0]<0.01)
				{
					pHs->buySellOrIndex .buySellInfo .buyP [i] =(BYTE)127;
				}
				else
					pHs->buySellOrIndex .buySellInfo .buyP [i] 
				=(pCdat->nowp  - *pPriceVol[i][0])*125
					/ pHs->buySellOrIndex .buySellInfo .unitCountPrc;
			}
		}
		if(pHs->buySellOrIndex .buySellInfo .unitCountVol<=0)
		{
			for(i=0;i<6;i++)
				pHs->buySellOrIndex .buySellInfo .buyV [i] =0;
		}
		else
		{
			for(i=0;i<6;i++)
				pHs->buySellOrIndex .buySellInfo .buyV [i] =(BYTE)( (*pPriceVol[i][1])*255/ pHs->buySellOrIndex .buySellInfo .unitCountVol+0.5);;
		}

		CTime tm = CTime::GetCurrentTime ();
		pHs->time  = (int)tm.GetTime ()  ;

		int nYear = tm.GetYear();
		int nMon = tm.GetMonth();
		int nDay = tm.GetDay();
		CTime tm9 = CTime(nYear,nMon,nDay,9,25,0);
		CTime tm11 = CTime(nYear,nMon,nDay,11,31,0);
		if(tm<tm9)
		{
			pHs->time = tm9.GetTime ();
		}
		else if (tm>=tm11)
		{
			CTime tm13 = CTime(nYear,nMon,nDay,13,0,0);
			CTime tm15 = CTime(nYear,nMon,nDay,15,0,0);
			if(tm<tm13)
			{
				CTime tm12 = CTime(nYear,nMon,nDay,12,0,0);
				if(tm<tm12)
					pHs->time = CTime(nYear,nMon,nDay,11,30,0).GetTime();
				else
					pHs->time = tm13.GetTime();
			}
			else if(tm>tm15)
			{
				pHs->time = tm15.GetTime();
			}
		}

	}
	else
	{
		pCdat->nowp =pHs->price  ;
		pCdat->totv =(pHs->vol);
		int nFlag ;
		pCdat->nowv = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
		pCdat->InOut = 1;
		if(nFlag == 0) pCdat->InOut = 2;
		if(nFlag > 0) pCdat->InOut = 0;


		float maxFloat;

		maxFloat= pHs->buySellOrIndex .buySellInfo .unitCountPrc ;

		if(maxFloat<=0)
		{
			for(i=0;i<6;i++)
				*pPriceVol[i][0]=0;
		}
		else
		{
			for(i=0;i<6;i++)
			{
				if(pHs->buySellOrIndex .buySellInfo .buyP [i] != 127)
					*pPriceVol[i][0]=pCdat->nowp-(float)((char)(pHs->buySellOrIndex .buySellInfo .buyP [i] )
					* pHs->buySellOrIndex .buySellInfo .unitCountPrc)/(float)(125);
				else *pPriceVol[i][0] = 0;
			}

		}


		float max= (float)pHs->buySellOrIndex .buySellInfo .unitCountVol ;
		if(max<=0)
		{
			for(i=0;i<6;i++)
				*pPriceVol[i][1]=0;
		}
		else
		{
			for(i=0;i<6;i++)
				*pPriceVol[i][1]=(float)((int)(pHs->buySellOrIndex .buySellInfo .buyV [i]*max/255 +0.5));;
		}

	}
}
int CTaiKlineFileHS::TransferHsToMin1(CBuySellList& buySellList,TRADE_DETAIL_H_PER* pHs,int nTotal)//transfer hs to 1 minute line
{

	if(nTotal==0)
		return 0;
	memset(pHs,0,sizeof(TRADE_DETAIL_H_PER)*nTotal);

	int n = buySellList.GetCount ();
	if(n==0)
		return 0;

	TRADE_DETAIL_H_PER* pHsIn=GetAtBS (&buySellList,0,n);

	int tmt = (int)pHsIn->time ;
	CTime tm2(tmt);


	int k =0;
	ASSERT(nTotal<=240);

	TRADE_DETAIL_H_PER* pHsLast=GetAtBS (&buySellList,n-1,n);
	for(int j=0;j<n;j++)
	{
		pHsIn = GetAtBS (&buySellList,j,n);
		int k = GetMinuteSecond(pHsIn->time);
		if(k<930)
			k = 930;
		if(k>=1500)
			k = 1459;

		if(k>=1130&&k<1200)
			k=1129;
		if(k>=1200&&k<1300)
			k=1300;

		if(k<=1129)
			k=(k-900)/100*60+(k-900)%100-30;
		else
			k=(k-1300)/100*60+(k-1300)%100+120;

		if(k==112)
			k=112;
		if(k==113)
			k=113;

		memcpy(pHs+k,pHsIn,sizeof(TRADE_DETAIL_H_PER));
	}
	pHsIn=GetAtBS (&buySellList,0,n);
	if(pHs->price ==0)  
	{
		pHs->price = pHsIn->price ;
	}
	int nYear = tm2.GetYear ();
	int nMon = tm2.GetMonth ();
	int nDay = tm2.GetDay ();
	for(int j=0;j<nTotal;j++)
	{
		int day  = 900+(30+j)/60*100 +(30+j)%60;
		if(j>=120)
			day  = 1300+(j-120)/60*100+ (j-120)%60;
		if(pHs[j].price <=0 && j>0)
		{
			memcpy(pHs+j,pHs+j-1,sizeof(TRADE_DETAIL_H_PER));
		}
		pHs[j].time = CTime(nYear,nMon,nDay,day/100,day%100,0).GetTime();
	}
	return nTotal;

}


void CTaiKlineFileHS::AddNewStockToFile(CString symbol,TSK_TICKINDEX*& pTickIndex)
{
	int nStock = this->GetStockNumber ();

	if(nStock>=MaxNumStock)
	{
		AddSmallHeadBlock();
	}
	nStock = this->GetStockNumber ();
	ASSERT(nStock<=MaxNumStock);
	int addr = 16+nStock*HsSmallHeadByteEach;
	this->Seek(addr,this->begin);
	pTickIndex = (TSK_TICKINDEX*)this->GetFileCurrentPointer();
	memset(pTickIndex,0xff,sizeof(TSK_TICKINDEX));
	if(symbol.GetLength ()!=4&&symbol.GetLength ()!=6)
	{
		symbol = symbol.Left (6);
	}

	Write(symbol.GetBuffer (8),8);

	int nKline = 0;
	Write(&nKline,4);

	this->Seek(addr,this->begin);
	pTickIndex = (TSK_TICKINDEX*)this->GetFileCurrentPointer();


	std::string code(symbol);
	m_mapIndex[code] = nStock;
	nStock++;
	this->SetStockNumber (nStock);



}
int CTaiKlineFileHS::CreateOrMoveSmallBlock(TSK_TICKINDEX* pTickIndex,int& nBlock)
{

	int rtn = 0;
	int nMax = 16;
	ASSERT(nBlock<nMax);

	if(pTickIndex->symBlock[nBlock]==0xffff)
	{
		int nCountBlock = this->GetSmallBlockCount ();
		this->SetSmallBlockCount (nCountBlock+1);
		int addr2 = SMALLHEADLENGTH + nCountBlock 
			* HsByteEach *  FixedHsPerBlock;

		char buff[ HsByteEach *  FixedHsPerBlock];
		memset(buff,255,HsByteEach *  FixedHsPerBlock);
		this->Seek(addr2,this->begin);
		this->Write (buff,HsByteEach *  FixedHsPerBlock);

		ASSERT(nBlock>=0);
		pTickIndex->symBlock[nBlock]=(WORD)nCountBlock;
		nCountBlock++;
	}
	return 0;

}

BOOL CTaiKlineFileHS::Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd,CFileException* pException)
{	
	if(m_bToday == false)
	{
		CFileFind find;
		if(find.FindFile (lpszFileName) == FALSE)
			return FALSE;
	}
	BOOL bOk = TRUE;
	bOk = CStkFile::Open( lpszFileName,  nOpenFlags,  0,
		pException);
	if(bOk == FALSE) return bOk;


	DWORD dwLen = this->GetLength ();
	if(dwLen<=SMALLHEADLENGTH || bOk == FALSE)
	{
		this->ReMapFromBegin (SMALLHEADLENGTH+250* HsByteEach *  FixedHsPerBlock);


		WriteHeaderInfo();
	}
	else
	{

		//
		int bID = 1;
		if(GetID()!=65798809) bID =0;
		int nBlock = this->GetSmallBlockCount();
		if(this->GetStockNumber ()>4096 
			|| nBlock>4096*16 
			|| this->GetStockNumber ()<0 
			|| nBlock<0 
			|| bID ==0)
			WriteHeaderInfo();
		else
		{
			int nLen = this->GetLength ();
			nLen -= SMALLHEADLENGTH;
			if(nLen>0)
			{
				nLen = nLen/( HsByteEach *  FixedHsPerBlock);
				if(nLen%( HsByteEach *  FixedHsPerBlock)!=0)
					nLen++;
				if(nBlock>nLen)
				{
					ASSERT(FALSE);
					this->SetSmallBlockCount (nLen);

					int nStock = GetStockNumber ();
					for(int i=0;i<nStock;i++)
					{
						int addr = 16+i*HsSmallHeadByteEach;
						Seek(addr,CStkFile::begin);
						TSK_TICKINDEX tickIndex;
						TSK_TICKINDEX* pTickIndex = &tickIndex;

						Read(pTickIndex,sizeof(TSK_TICKINDEX));
						pTickIndex->stockCode[6]=0;
						CString symbol (pTickIndex->stockCode);
						for(int j=0;j<16;j++)
						{
							if(pTickIndex->symBlock [j] == 0xffff) break;
							int iData = -1;
							int iIn = pTickIndex->symBlock [j] ;
							if(iIn>=nLen)
							{
								pTickIndex->tickCount  = (j)*FixedHsPerBlock;
								for(;j<16;j++)
									pTickIndex->symBlock [j] = 0xffff;
								this->SetTickIndex  (i,pTickIndex);
								break;
							}
						}
					}
				}
			}
		}
		this->ReMapFromBegin (SMALLHEADLENGTH+(nAddToFileEnd+this->GetSmallBlockCount())* HsByteEach *  FixedHsPerBlock);
	}

	MaxNumStock = GetMaxNumStock();
	ASSERT(MaxNumStock == 4096);

	return TRUE;
}

void CTaiKlineFileHS::WriteHeaderInfo()
{
	SeekToBegin();

	int nStock = 0;
	Write(&nStock, 4);
	Write(&nStock, 4);

	nStock = 65798809;
	Write(&nStock, 4);
	WORD wd = FixedHsPerBlock;
	Write(&wd, 2);
	wd = MaxNumStock;
	Write(&wd, 2);

	char buff[8192];
	memset(buff, 255, MaxNumStock);
	for (int i = 0; i < HsSmallHeadByteEach; i++)
		Write(buff, MaxNumStock);
}

BOOL CTaiKlineFileHS::WriteHS(TSK_TICKINDEX* pTickIndex,TRADE_DETAIL_H_PER *pHs)
{
	CString symbol(pTickIndex->stockCode);
	//if (symbol.GetLength() != 6 && symbol.GetLength() != 4) return 0;

	ASSERT(pTickIndex->tickCount <= 0 ? TRUE : (int)pTickIndex->symBlock[(pTickIndex->tickCount - 1) / FixedHsPerBlock] < 0xFFFF);

	int i = pTickIndex->tickCount;
	int blockIndex = i / FixedHsPerBlock;
	int stockIndex = i % FixedHsPerBlock;
	if (stockIndex == 0)
	{
		CString s2(pTickIndex->stockCode);
		CreateOrMoveSmallBlock(pTickIndex, blockIndex);
	}

	int addr = SMALLHEADLENGTH + pTickIndex->symBlock[blockIndex] * HsByteEach * FixedHsPerBlock + stockIndex * HsByteEach;

	Seek(addr, begin);
	Write(pHs, HsByteEach);
	pTickIndex->tickCount++;

	return TRUE;
}

BOOL CTaiKlineFileHS::WriteHS2(CString symbol, CBuySellList& buySellList)
{
	//if (symbol.GetLength() != 6 && symbol.GetLength() != 4) return 0;

	TSK_TICKINDEX tickIndex;
	TSK_TICKINDEX* pTickIndex = &tickIndex;

	std::string code(symbol);
	int nIndexStock = GetTickIndex(code, pTickIndex);

	int nList = buySellList.GetCount();

	for (int i = 0; i < nList; i++)
	{
		int blockIndex = i / FixedHsPerBlock;
		int stockIndex = i % FixedHsPerBlock;
		if (stockIndex == 0)
		{
			CString s2(pTickIndex->stockCode);
			CreateOrMoveSmallBlock(pTickIndex, blockIndex);
		}

		int addr = SMALLHEADLENGTH + pTickIndex->symBlock[blockIndex] * HsByteEach * FixedHsPerBlock + stockIndex * HsByteEach;

		Seek(addr, begin);
		POSITION pos = buySellList.FindIndex(nList - i - 1);
		TRADE_DETAIL_H_PER* pHs = buySellList.GetAt(pos);
		Write(pHs, HsByteEach);
	}

	pTickIndex->tickCount = nList;
	SetTickIndex(nIndexStock, pTickIndex);

	return TRUE;
}

void CTaiKlineFileHS::TransferHs(CBuySellList *pBuySellList, ARRAY_BE &pp, int nMax, int nKindIn, int nOther, KlineEx *pKlineEx)
{
	ASSERT(nMax==pBuySellList->GetCount ());
	if(nMax!=pBuySellList->GetCount ())
		return ;

	int n = nOther;
	if(nOther<1)
		n = 1;
	if(nOther>3)
		n=3;

	TRADE_DETAIL_H_PER *pHsPre = NULL;
	int nFlag=0;
	float f = 0;
	for (int i=pp.b;i<=pp.e;i++)
	{
		TRADE_DETAIL_H_PER* pHs = GetAtBS(pBuySellList, i,nMax);

		if(nKindIn == BUYVOL||nKindIn == SELLVOL||nKindIn == ISBUYORDER)
			f =  CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
		pHsPre = pHs;

		switch(nKindIn)
		{
		case BUYVOL:
			pp.line[i] = nFlag>0?f:(nFlag==0?f/2:0);
			break;
		case SELLVOL:
			pp.line[i] = nFlag<0?f:(nFlag==0?f/2:0);
			break;
		case ISBUYORDER:
			if(nFlag == -1)
				pp.line[i] = 0;
			else if(nFlag == 0)
				pp.line[i] = 2;
			else 
				pp.line[i] = 1;
			break;
		case ASKPRICE:
			if(n==1)
				pp.line[i] = pKlineEx[i].buyPrice [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].buyPrice [1];
			else
				pp.line[i] = pKlineEx[i].buyPrice [2];
			break;
		case ASKVOL:
			if(n==1)
				pp.line[i] = pKlineEx[i].buyVol [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].buyVol [1];
			else
				pp.line[i] = pKlineEx[i].buyVol [2];
			break;
		case BIDPRICE:
			if(n==1)
				pp.line[i] = pKlineEx[i].sellPrice  [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].sellPrice [1];
			else
				pp.line[i] = pKlineEx[i].sellPrice [2];
			break;
		case BIDVOL:
			if(n==1)
				pp.line[i] = pKlineEx[i].sellVol [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].sellVol [1];
			else
				pp.line[i] = pKlineEx[i].sellVol [2];
			break;
		}
	}

}

TRADE_DETAIL_H_PER* CTaiKlineFileHS::GetAtBS(CBuySellList *pBuySellList, int nFoot,int nCount)
{
	ASSERT(nFoot<nCount);
	POSITION pos=pBuySellList->FindIndex(nCount - nFoot - 1 ) ;
	TRADE_DETAIL_H_PER* pHs=pBuySellList->GetAt( pos );
	return pHs;
}

void CTaiKlineFileHS::DoCloseWorkHs(CProgressDialog* pDlg)
{
	WriteHistoryHsFile(true,pDlg);
	WriteHistoryHsFile(false,pDlg);
}

void CTaiKlineFileHS::WriteHistoryHsFile(bool bSh,CProgressDialog* pDlg)
{
	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc ;


	CString sPath = "data\\historysh\\";
	CString strName = CSharesCompute::GetIndexSymbol(0);
	int nMarket = SH_MARKET_EX;
	if(bSh == false)// ) 
		nMarket = SZ_MARKET_EX;

	if(bSh == false)
	{
		sPath = "data\\historysz\\";
		strName = CSharesCompute::GetIndexSymbol(1);
	}

	CFileFind finder;
	int n = 0;
	BOOL bWorking = finder.FindFile(sPath+"*.hst");
	if(bWorking == FALSE)
	{
		CString s = sPath;
		s.TrimRight ("\\");
		::CreateDirectory (s,NULL);
	}

	CStringArray sArray;
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		CString filename = finder.GetFileTitle();
		int nSize = sArray.GetSize ();
		if(nSize == 0 )
			sArray.Add (filename);
		else
			for(int j=0;j<nSize;j++)
			{
				if(filename <sArray[j])
				{
					sArray.InsertAt (j,filename);
					break;
				}
				if(j==nSize-1)
					sArray.Add (filename);
			};
		n++;
	}

	CString strMax="";
	CString strMin="";
	if(sArray.GetSize ()>0)
	{
		strMax = sArray[0];
		strMin = sArray[0];
	}
	for(int i=1;i<sArray.GetSize ();i++)
	{
		if(strMax<sArray[i])
			strMax=sArray[i];
		if(strMin>sArray[i])
			strMin=sArray[i];
	}



	CTime tm = CTime::GetCurrentTime ();
	CString sToday = tm.Format("%Y%m%d");
	if(sToday == strMax)
	{
		CString s = sPath+strMax+".hst";

		::DeleteFile (s);
	}
	if(n>=pDoc->m_propertyInitiate .countOfHistoryFile)
	{
		for(int k=0;k<n-pDoc->m_propertyInitiate .countOfHistoryFile;k++)
		{
			if(sArray.GetSize()<=0) break;
			CString s = sPath+sArray[0]+".hst";

			::DeleteFile (s);
			sArray.RemoveAt (0);
		}
	}


	CTaiKlineFileHS fileHist(nMarket);
	sToday = sPath+sToday+".hst";
	fileHist.Open (sToday,0);

	CTaiKlineFileHS* pFileToday = m_fileHsSh;
	if(bSh == false) pFileToday = m_fileHsSz;



	fileHist.ReMapFromBegin (SMALLHEADLENGTH+(pFileToday->GetSmallBlockCount())* HsByteEach *  FixedHsPerBlock/2);


	int addr = 16;
	int nStock = pFileToday->GetStockNumber();

	CBuySellList buySellList;
	for(int i=0;i<nStock;i++)
	{

		fileHist.Seek(addr,begin);
		pFileToday->Seek(addr,begin);
		TSK_TICKINDEX* pTickIndex = (TSK_TICKINDEX*)fileHist.GetFileCurrentPointer();
		TSK_TICKINDEX* pHsSmallHeadToday = (TSK_TICKINDEX*)pFileToday->GetFileCurrentPointer();
		CString symbol(pHsSmallHeadToday->stockCode );
		pTickIndex->tickCount =0;
		strcpy(pTickIndex->stockCode ,symbol);


		if(CTaiShanKlineShowView::IsIndexStock3(symbol))
			ASSERT(TRUE);
		pFileToday->ReadHS (symbol,buySellList,true);


		fileHist.WriteHS2( symbol, buySellList);

		addr+=HsSmallHeadByteEach;


		int spos=100*i /nStock   ;
		pDlg->DisplayZqxx(symbol, spos); 

	}

	RemoveHs(buySellList);
	fileHist.Close ();

}

int CTaiKlineFileHS::ReadIndexHS(CString symbol, CBuySellList &buySellList, bool bClearAll)
{

	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	if(bClearAll==true)
	{
		RemoveHs(buySellList);
	}
	else
		return 0;


	int nList = buySellList.GetCount ();
	ASSERT(nList==0);


	char *strLabel = symbol.GetBuffer (0);
	int nMarket = m_nMarket;
	int stkKind = CSharesInformation::GetStockKind(nMarket,strLabel);
	CReportData* pdt=NULL;
	if(pDoc->m_sharesInformation.Lookup(symbol.GetBuffer (0),pdt,stkKind)==0)
		return 0;
	int isSz=0;
	if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;

	Rsdn1 **Nidx;
	pDoc->m_sharesInformation.GetIndexRsdn(Nidx); 
	Tidxd **Tidx;
	pDoc->m_sharesInformation.GetIndexTidxd(Tidx); 

	CTime tm = CTime::GetCurrentTime ();
	int tmt = (int)tm.GetTime ();
	tmt = tmt/(24*60*60)*24*60*60+((9-8)*60+30)*60;

	if(pdt->ystc<=0 || pdt->opnp<=0)
		return 0;

	float f = 0;
	if(pdt->m_Kdata1 [0].Price <=0)
		f = pdt->opnp  ;
	int footEnd = pDoc->m_nANT [0]+1;
	if(pDoc->m_bInitDone ==FALSE)
		footEnd=pDoc->m_nOldANT [0]+1;
	if(footEnd>240) footEnd = 240;
	for(int i=0;i<footEnd ;i++)
	{
		TRADE_DETAIL_H_PER* pHs = new TRADE_DETAIL_H_PER;

		if(pdt->m_Kdata1 [i].Price>0 )
		{
			f = pdt->m_Kdata1 [i].Price;

		}
		pHs->price = f ;

		pHs->vol = pdt->m_Kdata1 [i].Volume  ;
		pHs->time = tmt+60*i ;
		if(i>=120)
			pHs->time  = tmt+60*i+90*60 ;
		pHs->buySellOrIndex .indexInfo .amount   = pdt->m_Kdata1 [i].Amount   ;
		pHs->buySellOrIndex .indexInfo .advance  = Nidx[isSz][i].rsn ;
		pHs->buySellOrIndex .indexInfo .decline  = Nidx[isSz][i].dnn ;
		pHs->buySellOrIndex .indexInfo .tidxd .dp  = Tidx[isSz][i].dp  ;
		pHs->buySellOrIndex .indexInfo .tidxd .res   = Tidx[isSz][i].res  ;
		pHs->buySellOrIndex .indexInfo .tidxd .rp   = Tidx[isSz][i].rp  ;
		pHs->buySellOrIndex .indexInfo .tidxd .sec5   = Tidx[isSz][i].sec5  ;

		if(i==0)
			pHs->buySellOrIndex .indexInfo .faccbaccs    = pdt->accb   ;
		if(i==1)
			pHs->buySellOrIndex .indexInfo .faccbaccs    = pdt->accs   ;

		buySellList.AddHead(pHs);
	}

	return buySellList.GetCount ();
}

bool CTaiKlineFileHS::TransferDataEx(CBuySellList *pBuySellList, KlineEx *&pKlineEx)
{
	if(pKlineEx!=NULL)
		delete [] pKlineEx;
	int nMax = pBuySellList->GetCount ();
	pKlineEx= new KlineEx[nMax];

	int n ;

	TRADE_DETAIL_H_PER *pHsPre = NULL;
	for (int i=0;i<nMax;i++)
	{
		TRADE_DETAIL_H_PER* pHs = GetAtBS(pBuySellList, i,nMax);

		if(pHsPre )
			if(pHs->vol - pHsPre->vol == 430)
			{
				int k = 20;
				k = 90;

			}

			CReportData cdat;
			Cdat1ToHs(&cdat, pHs, false,pHsPre);
			float* pf = &(cdat.pbuy1);

			pHsPre = pHs;
			for(int j=0;j<3;j++)
			{
				pKlineEx[i].fDataEx [j] = pf[j*2];
				pKlineEx[i].fDataEx [j+3] = pf[j*2+1];
				pKlineEx[i].fDataEx [j+6] = pf[j*2+6];
				pKlineEx[i].fDataEx [j+9] = pf[j*2+6+1];
			}
	}

	return true;

}

bool CTaiKlineFileHS::BillTransferDataEx(CTaiKlineFileHS *pFileHs, char *pStockCode, CBuySellList *pBuySellList, KlineEx *&pKlineEx)

{

	if(pFileHs ==NULL)
		return false;

	CString symbol = pStockCode;
	pFileHs->ReadHS2 (symbol,*pBuySellList,true);

	pFileHs->TransferDataEx(pBuySellList, pKlineEx);

	return true;
}

CTaiKlineFileHS* CTaiKlineFileHS::BillOpenFileHs(CString strTime, bool bShangHai, bool bToday)

{
	CString sPath;
	int nMarket = SH_MARKET_EX;
	sPath = "data\\historysh\\";
	if(bShangHai==false)
	{
		nMarket = SZ_MARKET_EX;
		sPath = "data\\historysz\\";
	}
	sPath+=strTime;
	sPath+=".hst";
	if(bToday==true)
	{
		if(bShangHai==false)
			sPath =g_buysellsz;
		else
			sPath = g_buysellsh;
	}
	CFileFind find;
	if(find.FindFile(sPath)==FALSE) return NULL;

	CTaiKlineFileHS*	pFile = new CTaiKlineFileHS(nMarket,bToday);
	if(!pFile->Open (sPath,0))
	{
		delete pFile;
		return NULL;
	}
	return pFile;
}

int CTaiKlineFileHS::GetMinuteSecond(int timet)
{
	CTime tm(timet);
	return atoi(tm.Format ("%H%M"));
}

BOOL CTaiKlineFileHS::OpenAll()
{
	m_fileHsSh = new CTaiKlineFileHS(SH_MARKET_EX);
	m_fileHsSz = new CTaiKlineFileHS(SZ_MARKET_EX);

	BOOL bRtn = TRUE;

	CString sPath2;
	sPath2 = g_buysellsh;
	if (!m_fileHsSh->Open(sPath2, 0, NULL))
	{
		ASSERT(FALSE);
		delete m_fileHsSh;
		m_fileHsSh = NULL;
		bRtn = FALSE;
	}

	sPath2 = g_buysellsz;
	if (!m_fileHsSz->Open(sPath2, 0, NULL))
	{
		ASSERT(FALSE);
		delete m_fileHsSz;
		m_fileHsSz = NULL;
		bRtn = FALSE;
	}

	return bRtn;
}

void CTaiKlineFileHS::CloseAll()
{
	if (m_fileHsSh != NULL)
	{
		m_fileHsSh->Close();
		delete m_fileHsSh;
		m_fileHsSh = NULL;
	}

	if (m_fileHsSz != NULL)
	{
		m_fileHsSz->Close();
		delete m_fileHsSz;
		m_fileHsSz = NULL;
	}
}

CTaiKlineFileHS* CTaiKlineFileHS::GetFilePointer(CString symbol, int stkKind)
{
	CTaiKlineFileHS* pFileHs;

	pFileHs = CTaiKlineFileHS::m_fileHsSh;

	if (SZ_MARKET_EX == CSharesCompute::GetMarketKind(stkKind))
		pFileHs = CTaiKlineFileHS::m_fileHsSz;

	return pFileHs;
}

CTaiKlineFileHS* CTaiKlineFileHS::GetFilePointer2(int nMarket)
{
	CTaiKlineFileHS* pFileHs = NULL;

	pFileHs = CTaiKlineFileHS::m_fileHsSh;
	if (SH_MARKET_EX == nMarket)
	{
		pFileHs=CTaiKlineFileHS::m_fileHsSh;
	}
	else if (SZ_MARKET_EX == nMarket)
	{
		pFileHs = CTaiKlineFileHS::m_fileHsSz;
	}

	return pFileHs;
}

int CTaiKlineFileHS::GetDataCountAll(CString symbol, int stkKind)
{
	CTaiKlineFileHS* pFile = GetFilePointer(symbol, stkKind);
	std::string code(symbol);
	return pFile->GetDataCount(code);
}

void CTaiKlineFileHS::RemoveHs(CBuySellList& buySellList)
{
	TRADE_DETAIL_H_PER* phs;

	for ( ; buySellList.IsEmpty()==0; )
	{
		phs = buySellList.RemoveHead();
		delete phs;
	}

	buySellList.RemoveAll();
}

int CTaiKlineFileHS::GetCountPre(CString symbol, int stkKind)
{
	TSK_TICKINDEX tickIndex;
	TSK_TICKINDEX* pTickIndex = &tickIndex;
	memset(pTickIndex, 0, sizeof(TSK_TICKINDEX));

	std::string code(symbol);
	CTaiKlineFileHS* pFile = GetFilePointer(symbol, stkKind);
	int nIndexStock = pFile->GetTickIndex(code, pTickIndex);

	return tickIndex.numReserved;
}

bool CTaiKlineFileHS::IsNeedHsHistData(CString symbol,int stkKind, CString sDate)
{
	bool brtn = true;
	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc ;


	CString sPath = "data\\historysh\\";

	bool bSh = true;
	int nMarket = SH_MARKET_EX;
	if(CSharesCompute::GetMarketKind(stkKind) ==SZ_MARKET_EX)
	{
		nMarket = SZ_MARKET_EX;
		bSh = false;
	}
	if(bSh == false)
	{
		sPath = "data\\historysz\\";

	}

	CFileFind finder;


	int n = 0;
	BOOL bWorking = finder.FindFile(sPath+sDate+".hst");
	if(bWorking == FALSE)
	{
		;
	}
	else
	{
		TSK_TICKINDEX tickIndex;
		memset(&tickIndex,0,sizeof(TSK_TICKINDEX));
		TSK_TICKINDEX* pTickIndex = &tickIndex;
		CTaiKlineFileHS fileHist(nMarket);
		CString sName = sPath+sDate+".hst";
		fileHist.Open (sName,0);
		std::string code(symbol);
		int nIndexStock = fileHist.GetTickIndex(code,pTickIndex);
		n = tickIndex.tickCount  ;
		if(n>0)
			brtn = false;

	}
	return brtn;
}



bool CTaiKlineFileHS::WriteHsDataWideNet(RCV_DISPBARGAINING_STRUCTEx *pData,int nPacketNum,int nRequest)
{
	int index=0;
	if(pData->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return false;

	bool bHistory = false;
	if(nRequest>19800101)
		bHistory = true;

	while(index<nPacketNum)
	{

		CString StockId;
		int nMarket;
		if(pData[index].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
			StockId=pData[index].m_head.m_szLabel;
			nMarket=pData[index].m_head.m_wMarket;
		}
		else break;
		index++;


		int  endindex= index; 
		while(pData[endindex].m_head.m_dwHeadTag != EKE_HEAD_TAG && endindex<nPacketNum)
		{
			endindex++;
		}


		{
			int nDataCount = endindex - index;
			if(nDataCount<=0) continue;


			CString sDate ="";
			if(bHistory == false)
			{
			}
			else
			{
				CTime tm(pData[index].m_time );
				sDate = tm.Format ("%Y%m%d");
			}
			WriteHsToFileWideNet(StockId,nMarket, sDate, pData+index, nDataCount,nRequest);
		}


		index = endindex;
	}
	return true;
}

void CTaiKlineFileHS::WriteHsToFileWideNet(CString symbol,int nMarket, CString sDate, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord,int nRequest)
{
	int n = 0;
	if(sDate == "")
	{
		CTaiKlineFileHS* pFile = CTaiKlineFileHS::GetFilePointer2(nMarket);
		n = pFile->WriteHsToFile( symbol, pData,  nRecord);


		if(nRequest < 2000 )
		{
			TSK_TICKINDEX tickIndex;
			memset(&tickIndex,0,sizeof(TSK_TICKINDEX));
			TSK_TICKINDEX* pTickIndex = &tickIndex;
			std::string code(symbol);
			int nIndexStock = pFile->GetTickIndex(code,pTickIndex);
			tickIndex.numReserved = n;
			pFile->SetTickIndex (nIndexStock,pTickIndex);
		}

		if(nRequest == 0 )
		{
			CTime tm(pData->m_time);
			CTime tm2 = CTime::GetCurrentTime ();
			if(tm.GetDay ()==tm2.GetDay ()
				&&tm.GetMonth ()==tm2.GetMonth ()
				&&tm.GetYear ()==tm2.GetYear ())
				sDate=tm.Format ("%Y%m%d");
		}
	}
	if(sDate!="")
	{
		CString sPath = FILE_PATH_HSH_SH;
		bool bSh = true;
		char *strLabel = symbol.GetBuffer (0);
		int stkKind2 = CSharesInformation::GetStockKind(nMarket,strLabel);
		int nMarket = SH_MARKET_EX;
		if(CSharesCompute::GetMarketKind(stkKind2) !=SH_MARKET_EX)
		{
			nMarket = SZ_MARKET_EX;
			bSh = false;
		}

		if(bSh == false)
		{
			sPath = FILE_PATH_HSH_SZ;
		}
		CTaiKlineFileHS fileHsSh(bSh);

		{
			CString filename =sPath + sDate +".hst";
			if(fileHsSh.Open (filename,0))
			{
				n = fileHsSh.WriteHsToFile( symbol, pData,  nRecord);
			}
		}
	}



}

int CTaiKlineFileHS::WriteHsToFile(CString symbol, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord)
{

	int n = 0;
	TSK_TICKINDEX tickIndex;
	memset(&tickIndex,0,sizeof(TSK_TICKINDEX));
	TSK_TICKINDEX* pTickIndex = &tickIndex;
	std::string code(symbol);
	int nIndexStock = GetTickIndex(code,pTickIndex);
	n = tickIndex.numReserved ;


	CBuySellList buySellList;
	ReadHS2(symbol, buySellList,false);

	bool bWriteDirect = false;
	int nCountFile = buySellList.GetCount();
	if(nCountFile>0)
	{
		TRADE_DETAIL_H_PER* pHs = CTaiKlineFileHS::GetAtBS(&buySellList, nCountFile-1,nCountFile);
		if(pData[0].m_time>=pHs->time && pData[0].m_fVolume >=pHs->vol )
			bWriteDirect = true;
	}
	else
		bWriteDirect = true;

	int k=0;
	for(int i = 0;i<nRecord;i++)
	{
		TRADE_DETAIL_H_PER * pHs = new TRADE_DETAIL_H_PER;
		CReportData dat1;
		dat1.nowp = pData[i].m_fNewPrice ;
		dat1.totv  = pData[i].m_fVolume ;
		dat1.pbuy1  = pData[i].m_fBuyPrice [0];
		dat1.pbuy2  = pData[i].m_fBuyPrice [1];
		dat1.pbuy3  = pData[i].m_fBuyPrice [2];
		dat1.psel1  = pData[i].m_fSellPrice [0];
		dat1.psel2  = pData[i].m_fSellPrice [1];
		dat1.psel3  = pData[i].m_fSellPrice [2];
		dat1.vbuy1   = pData[i].m_fBuyVolume   [0];
		dat1.vbuy2   = pData[i].m_fBuyVolume   [1];
		dat1.vbuy3   = pData[i].m_fBuyVolume   [2];
		dat1.vsel1   = pData[i].m_fSellVolume  [0];
		dat1.vsel2   = pData[i].m_fSellVolume  [1];
		dat1.vsel3   = pData[i].m_fSellVolume  [2];
		Cdat1ToHs(&dat1, pHs,true, NULL);
		pHs->time = pData[i].m_time ;

		if(bWriteDirect)
		{
			buySellList.AddHead (pHs);
		}
		else
		{
			int j;
			for(j=0;j<buySellList.GetCount ();j++)
			{
				TRADE_DETAIL_H_PER* pHs2 = 
					CTaiKlineFileHS::GetAtBS(&buySellList, j,buySellList.GetCount ());
				if(pHs->vol > pHs2->vol )
				{
					break;							
				}
			}
			if(j<buySellList.GetCount ())
			{
				ASSERT(buySellList.GetCount ()>0);
				j++;
				while(j<buySellList.GetCount ())
				{
					delete buySellList.RemoveHead ();
				}

			}
			bWriteDirect = true;
			buySellList.AddHead (pHs);

		}
	}


	WriteHS2(symbol, buySellList);
	int nRtn = buySellList.GetCount ();
	RemoveHs (buySellList);
	return nRtn;

}

void CTaiKlineFileHS::AddSmallHeadBlock()
{
	ASSERT(FALSE);
	int n = (MaxNumStock - 4096)/(240/2);
	if(n<0) return;


	int nStock = GetStockNumber ();

	TSK_TICKINDEX klineSmallHead;
	int j = 0;
	int i = 0;
	bool bFound = false;
	for(i=0;i<nStock;i++)
	{
		int addr = 16;
		Seek(addr+i*HsSmallHeadByteEach,begin);
		Read(&klineSmallHead,sizeof(TSK_TICKINDEX));
		for(j = 0;j<16;j++)
		{
			if(klineSmallHead.symBlock [j] == n)
			{
				bFound = true;
				break;
			}
		}

		if(bFound == true)
			break;
	}

	int nNewBlock = this->GetSmallBlockCount();
	if(bFound == true)
	{
		TRADE_DETAIL_H_PER kline;
		for(int k=0;k<FixedHsPerBlock ;k++)
		{
			int blockIndex= j ;	  
			int stockIndex=k  ;	  
			int addr = SMALLHEADLENGTH + klineSmallHead.symBlock[blockIndex] 
			* HsByteEach *  FixedHsPerBlock
				+ stockIndex * HsByteEach;

			this->Seek(addr,this->begin);
			Read(&kline,HsByteEach);
			addr = SMALLHEADLENGTH + klineSmallHead.symBlock[nNewBlock] 
			* HsByteEach *  FixedHsPerBlock
				+ stockIndex * HsByteEach;
			this->Seek(addr,this->begin);
			Write(&kline,HsByteEach);
		}

		//write small block
		klineSmallHead.symBlock [j] = (WORD)nNewBlock;
		this->SetTickIndex(i, &klineSmallHead);

		//
		nNewBlock++;
		SetSmallBlockCount(nNewBlock);
	}

	//2.
	MaxNumStock+=120;
	WORD nMaxNumStock = MaxNumStock;
	SetMaxNumStock(nMaxNumStock);

}




void CTaiKlineFileHS::AddIndexToMap()
{
	int nStock = GetStockNumber();
	if (nStock <= 0 || nStock > GetMaxNumStock() || m_bIndex)
		return;

	for (int iIndex = 0; iIndex < nStock; iIndex++)
	{
		//modify
		int addr = 16 + iIndex * 48;
		Seek(addr, begin);
		TSK_TICKINDEX* pTickIndex = (TSK_TICKINDEX*)GetFileCurrentPointer();

		string symbol(pTickIndex->stockCode);
		m_mapIndex[symbol] = iIndex;
	}

	m_bIndex = TRUE;
}

void CTaiKlineFileHS::DeleteMap()
{
	if (m_bIndex && m_mapIndex.size() > 0)
	{
		m_mapIndex.clear();
	}
}

void CTaiKlineFileHS::LookupIndex(std::string symbol, int& nIndex)
{
	if (!m_bIndex)
	{
		AddIndexToMap();
	}

	nIndex = -1;

	MapIndex::iterator stock = m_mapIndex.find(symbol);
	if (stock != m_mapIndex.end())
	{
		nIndex = stock->second;
	}
}

int CTaiKlineFileHS::GetTickIndex(std::string symbol, TSK_TICKINDEX* pTickIndex)
{
	if (!m_bIndex)
	{
		AddIndexToMap();
	}

	TSK_TICKINDEX* pIndex = NULL;
	int iIndex = -1;

	LookupIndex(symbol, iIndex);
	if (iIndex == -1)
	{
		//modify
		CString code(symbol.c_str());
		AddNewStockToFile(code, pIndex);
		if (GetStockNumber() > 0)
		{
			iIndex = GetStockNumber() - 1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		//modify
		int addr = 16;
		Seek(addr + iIndex * 48, begin);
		pIndex = (TSK_TICKINDEX*)GetFileCurrentPointer();
	}

	memcpy(pTickIndex, pIndex, sizeof(TSK_TICKINDEX));

	return iIndex;
}

BOOL CTaiKlineFileHS::SetTickIndex(int nIndex, TSK_TICKINDEX* pTickIndex)
{
	int nStock = GetStockNumber();
	if (nIndex >= nStock)
		return FALSE;

	//modify
	int addr = 16 + nIndex * HsSmallHeadByteEach;
	Seek(addr, begin);

	Write(pTickIndex, sizeof(TSK_TICKINDEX));

	return TRUE;
}

int CTaiKlineFileHS::GetDataCount(std::string symbol)
{
	TSK_TICKINDEX tickIndex;
	TSK_TICKINDEX* pTickIndex = &tickIndex;

	int nIndex = GetTickIndex(symbol, pTickIndex);
	if (pTickIndex == NULL)
		return 0;

	return pTickIndex->tickCount;
}

void CTaiKlineFileHS::ZeroHsCountEach()
{
	int addr = 16;
	int nStock = GetStockNumber();

	for (int iIndex = 0; iIndex < nStock; iIndex++)
	{
		//modify
		Seek(addr, begin);
		TSK_TICKINDEX* pTickIndex = (TSK_TICKINDEX*)GetFileCurrentPointer();
		memset(pTickIndex, 0xFF, sizeof(TSK_TICKINDEX));
		pTickIndex->tickCount = 0;
		addr += HsSmallHeadByteEach;
	}

	DeleteMap();

	SetStockNumber(0);
	SetSmallBlockCount(0);
}

BOOL CTaiKlineFileHS::WriteHS(CReportData* pCdat, BOOL bFirstOne)
{
	std::string symbol(pCdat->id);

	if (CTaiShanKlineShowView::IsIndexStock(symbol.c_str()))
		return FALSE;


	TSK_TICKINDEX tickIndex;
	TSK_TICKINDEX* pTickIndex = &tickIndex;

	int nIndexStock = GetTickIndex(symbol, pTickIndex);

	if (pCdat->m_serialNumber < 0)
	{
		pCdat->m_serialNumber = 0;
	}

	Seek(16 + pCdat->m_serialNumber * sizeof(TSK_TICKINDEX), begin);

	//tickIndex = *((TSK_TICKINDEX*)GetFileCurrentPointer());
	//CString symb(tickIndex.stockCode);
	//if (symb != symbol)
	//{
	//	nIndexStock = GetTickIndex(symbol, pTickIndex);
	//	pCdat->m_serialNumber = nIndexStock;
	//}


	TRADE_DETAIL_H_PER hs;
	Cdat1ToHs(pCdat, &hs);
	WriteHS(pTickIndex, &hs);

	SetTickIndex(nIndexStock, pTickIndex);

	return TRUE;
}

int CTaiKlineFileHS::ReadHS2(CString symbol, CBuySellList& buySellList, BOOL bClearAll)
{
	TSK_TICKINDEX tickIndex;
	TSK_TICKINDEX* pTickIndex = &tickIndex;

	std::string code(symbol);
	int nIndexStock = GetTickIndex(code, pTickIndex);

	if (bClearAll == TRUE)
	{
		RemoveHs(buySellList);
	}

	int nList = buySellList.GetCount();
	if (nList > pTickIndex->tickCount)
		return nList;

	SeekToBegin();
	//ASSERT(pTickIndex->tickCount <= 0 ? TRUE : (int)pTickIndex->symBlock[(pTickIndex->tickCount - 1) / FixedHsPerBlock] < 0xFFFF);

	for (int iIndex = nList; iIndex < pTickIndex->tickCount; iIndex++)
	{
		int blockIndex = iIndex / FixedHsPerBlock;
		int stockIndex = iIndex % FixedHsPerBlock;
		int nAddr = SMALLHEADLENGTH + pTickIndex->symBlock[blockIndex] * HsByteEach * FixedHsPerBlock + stockIndex * HsByteEach;

		Seek(nAddr, begin);

		TRADE_DETAIL_H_PER* pHs = new TRADE_DETAIL_H_PER;
		Read(pHs, HsByteEach);
		if (pHs->price == 0)
		{
			delete pHs;
		}
		else
		{
			buySellList.AddHead(pHs);
		}
	}

	return buySellList.GetCount();
}
