// CTaiKlineFileKLine.cpp: implementation of the CTaiKlineFileKLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineTransferKline.h"
#include "GetSetReg.h"
#include "ChooseReceiveDataForm.h"
#include <io.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define KLINEHEAD  16
#define KLINESMALLHEAD 4096 * 64 + 16
#define FixedKlinePerBlock	240

#define KlineByteEach	32
#define DAY_KLINE 	0
#define MIN5_KLINE	1

CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileDaySh = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileDaySz = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileMin5Sh= NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileMin5Sz= NULL;



CTaiKlineFileKLine::CTaiKlineFileKLine()
{
	m_nMarket = SZ_MARKET_EX;
	m_kindKline = DAY_KLINE;
	m_bFirstWrite = true;
}
CTaiKlineFileKLine::CTaiKlineFileKLine(int nKind,int nMarket)
{

	m_bFirstWrite = true;
	m_nAddReMap = KlineByteEach *  FixedKlinePerBlock*5;
	m_kindKline = nKind;
	m_nMarket = nMarket;

}

CTaiKlineFileKLine::~CTaiKlineFileKLine()
{
	DeleteMap();
}


int CTaiKlineFileKLine::ReadKLine(CString symbol,Kline*& pKline,int nRead,int nAddBlank)
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);

	int n = nRead;
	if(n<=-1||n>pKlineSmallHead->numKln)
		n=pKlineSmallHead->numKln ;
	if(pKline != NULL)
		delete [] pKline;
	ASSERT(nAddBlank>=0);
	pKline = new Kline[n+nAddBlank+1];
	if(n==0)
		return 0;

	this->SeekToBegin ();
	CString ss = pKlineSmallHead->StockSign ;
	ASSERT(n<=0?TRUE:KLINESMALLHEAD + pKlineSmallHead->symBlock[(n-1) / FixedKlinePerBlock ] 
				* KlineByteEach *  FixedKlinePerBlock
					  < this->GetLength ());

	
	for(int i=pKlineSmallHead->numKln-n;i<pKlineSmallHead->numKln ;i++)
	{
		int blkCount= i / FixedKlinePerBlock  ;	  
		int stockCount=i % FixedKlinePerBlock  ;	 
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockCount * KlineByteEach;
									  
		this->Seek(addr,this->begin);
		Read(pKline+i-pKlineSmallHead->numKln+n,KlineByteEach);
	}

	if(!CTaiShanKlineShowView::IsIndexStock3(symbol))
		TestKlineVolpositive(pKline, n);

	return n;

}


int CTaiKlineFileKLine::LookTwoPath(time_t& tmt ,Kline *pKline, int nMax, bool &bAdd,bool bDay)//look up using 2 path
{
	int l,u,mid;
	l=0;
	bAdd=false;
	u=nMax-1;
	int keyVlu = (int)tmt;
	keyVlu = RegularTime(keyVlu,bDay);

	if(keyVlu<RegularTime(pKline[0].day,bDay))
	{
		bAdd=true;
		return 0;
	}
	if(keyVlu>RegularTime(pKline[nMax-1].day,bDay))
	{
		bAdd = true;

		return -1;
	}

	do{
		mid=(l+u)/2;
		if(keyVlu<RegularTime(pKline[mid].day,bDay))
			u=mid;
		else if(keyVlu>RegularTime(pKline[mid].day,bDay))
			l=mid;
		else
			break;
	}while(l<u-1);

	if(keyVlu!=RegularTime(pKline[mid].day,bDay))
	{
		if(u-l==1)
		{
			if(RegularTime(pKline[l].day,bDay)<keyVlu)
			{
				if(RegularTime(pKline[u].day,bDay)>keyVlu)
				{
				mid=l+1;
				bAdd=true;
				}
				else if(RegularTime(pKline[u].day,bDay)== keyVlu)
				{
				mid=l+1;
				}
				else
					ASSERT(FALSE);
			}
			else if(RegularTime(pKline[l].day,bDay)==keyVlu)
			{
				mid=l;
			}
			else
				ASSERT(FALSE);
		}
		else
			ASSERT(FALSE);
	}

	return mid;


}
int CTaiKlineFileKLine::TimeToFoot(KLINE_SMALLHEAD* pKlineSmallHead,CTime &tm,bool& bAdd)//according the time ,to compute the foot of kline in file.
{
	int nCount = pKlineSmallHead->numKln ;
	bAdd = false;
	if(nCount<=0)
	{
		bAdd =true;
		return 0;
	}

	time_t tmt = tm.GetTime ();
	int i = 0;

	
	bool bFound = false;
	bool bDay  = false;
	if((m_kindKline+1)%2==1)
		bDay = true;
	for(i = (nCount-1)/FixedKlinePerBlock ; i>=0 ;i--)
	{
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[i] 
				* KlineByteEach *  FixedKlinePerBlock
					 ;
									 
		this->Seek(addr,this->begin);
		int tmFile;
		Read(&tmFile,4);
		if(RegularTime(tmFile,bDay)<(int)RegularTime(tmt,bDay))
		{
			bFound = true;
			break;
		}
		else if(RegularTime(tmFile,bDay)==(int)RegularTime(tmt,bDay))
			return i*FixedKlinePerBlock;

	}

	
	int rtn;
	if(i>0)
	{
	
		ASSERT(pKlineSmallHead->symBlock[i]!=0xffff);
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[i] 
				* KlineByteEach *  FixedKlinePerBlock
					 ;
									  
		Seek(addr,begin);
		Kline *pKline = (Kline *)this->GetFileCurrentPointer ();
		int nMax;

		//  20001110
		if(nCount>=(i+1)*FixedKlinePerBlock) nMax = FixedKlinePerBlock;
		else nMax =(nCount)%FixedKlinePerBlock;


		rtn = LookTwoPath(tmt ,pKline,  nMax, bAdd,(m_kindKline+1)%2);
		if(i<(nCount-1)/FixedKlinePerBlock)
		{
			if(rtn ==  -1&&bAdd == true)
			{

				rtn = FixedKlinePerBlock;
			}
			rtn += i* FixedKlinePerBlock;
		}
		else
		{
			if(rtn !=  -1)
			{
				rtn += i* FixedKlinePerBlock;
			}
		}
	}
	else
	{
		if(i<0)
			i = 0;
		if(bFound == false)
		{
			bAdd = true;
			rtn =0;
		}
		else
		{
		
			int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[0] 
					* KlineByteEach *  FixedKlinePerBlock
						 ;
										  
			Seek(addr,begin);
			Kline *pKline = (Kline *)this->GetFileCurrentPointer ();

			int nMax;

			if(nCount>=FixedKlinePerBlock) nMax = FixedKlinePerBlock;
			else nMax =nCount;
			//end

			rtn = LookTwoPath(tmt ,pKline,  nMax, bAdd,(m_kindKline+1)%2);
			if(rtn ==  -1&&bAdd == true) rtn = nMax;

		}

	}


	if(pKlineSmallHead->numKln<=rtn)
	{
		rtn =-1;
		bAdd = true;
	}
	
	return rtn;
}

int CTaiKlineFileKLine::ReadKlinePeriod(CString symbol, Kline *&kline, CTime& timeStart, CTime& timeEnd,bool istotaldata,int nAddBlank )//读取一段时间的K线数据
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);

	if(kline != NULL)
		delete [] kline ;
	bool bAdd;

	int nFootB = TimeToFoot(pKlineSmallHead,timeStart, bAdd);
	if(nFootB ==  -1&&bAdd == true)
	{

		kline = new Kline[1+nAddBlank];
		return 0;
	}
	int nFootE = TimeToFoot(pKlineSmallHead,timeEnd, bAdd);
	if(nFootE ==  -1&&bAdd == true)
	{

		nFootE = pKlineSmallHead ->numKln -1;
		bAdd = false;
	}
	int nCount = nFootE - nFootB+1;
	if(bAdd == true)
		nCount = nFootE - nFootB;
	if(nCount<=0)
	{

		kline = new Kline[1+nAddBlank];
		return 0;
	}

	kline = new Kline[nCount+nAddBlank];

	for(int i=nFootB;i<nFootB+nCount ;i++)
	{
		int blkCount= i / FixedKlinePerBlock  ;	 
		int stockCount=i % FixedKlinePerBlock  ;	  
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockCount * KlineByteEach;
									
		this->Seek(addr,this->begin);
		Read(kline+i-nFootB,KlineByteEach);
	}
	if(!CTaiShanKlineShowView::IsIndexStock3(symbol))
		TestKlineVolpositive(kline, nCount);
	return nCount;


}

bool CTaiKlineFileKLine::WriteKLine(CString symbol,Kline* pKline,int nWrite,int nType)	
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return false;

	{
		CTime tm=CTime::GetCurrentTime ();
		int tmt = tm.GetTime ()+24*60*60;
		for(int i=0;i<nWrite;i++)
		{
			if(pKline[i].day >tmt)
			{
				ASSERT(FALSE);

				return false;
			}
		}
	}
	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);
	int nMaxBlock = CalcMaxCountBlock();
	if(pKlineSmallHead->numKln > nMaxBlock*FixedKlinePerBlock)
	{
		int n1 = pKlineSmallHead->numKln/FixedKlinePerBlock;
		int n2 = pKlineSmallHead->numKln%FixedKlinePerBlock == 0 ? 0:1;
		int nBlockCur = n1 + n2;
		int nMoveBlock = nBlockCur - nMaxBlock ;
		if(nMoveBlock>0)
		{
			for(int i=0;i<nMoveBlock;i++)
			{
				WORD wdVal = pKlineSmallHead->symBlock [0];
				for(int j=0;j<nBlockCur-1;j++)
				{
					pKlineSmallHead->symBlock [j] = pKlineSmallHead->symBlock [j+1];
					ASSERT(pKlineSmallHead->symBlock [j+1]!=0xffff);
				}
				if(nBlockCur-1>=0)
					pKlineSmallHead->symBlock [nBlockCur-1] = wdVal;
				else
					ASSERT(FALSE);
	
			}
			pKlineSmallHead->numKln = pKlineSmallHead->numKln - nMoveBlock*FixedKlinePerBlock;
		}
		else
		{
			pKlineSmallHead->numKln = 0;
			ASSERT(FALSE);
		}
		this->SetKlineSmallHeader (nIndexStock,pKlineSmallHead);
	}

	ASSERT(pKlineSmallHead!=NULL);

	if(pKline == NULL || nWrite<=0) 
		return false;


	if(nWrite>1 && m_kindKline == 0)
	{
		CString tmt = "19800101";
		for(int i=0;i<nWrite;i++)
		{
				
			CTime tm2(pKline[i].day);
			CString s=tm2.Format ("%Y%m%d");
			while(1)
			{	
				if(nWrite<=i)
					break;
				tm2 = CTime(pKline[i].day);
				s=tm2.Format ("%Y%m%d");
				if(s<=tmt)
				{
					if(nWrite-i-1>0)
						memmove(pKline+i,pKline+i+1,(nWrite-i-1)*sizeof(Kline));
					nWrite--;
				}
				else
					break;
			}
			tmt = s;

		}

	}

	if(pKlineSmallHead->numKln <=0)
		pKlineSmallHead->numKln =0;


	CTime timeStart((time_t)pKline[0].day );
	bool bAdd;
	int nFootB = TimeToFoot(pKlineSmallHead,timeStart, bAdd);
	if(nFootB == -1 )
	{
		if(bAdd ==true)
			nFootB = pKlineSmallHead->numKln ;
		else
			ASSERT(FALSE);
	}
	timeStart = CTime((time_t)pKline[nWrite-1].day );
	int nFootE = TimeToFoot(pKlineSmallHead,timeStart, bAdd);
	if(nFootE == -1 )
	{
		if(bAdd ==true)
			nFootE = pKlineSmallHead->numKln ;
		else
			ASSERT(FALSE);
	}
	else if(bAdd == false)
	{
		if(nFootE <pKlineSmallHead->numKln)
			nFootE++;
		else
			ASSERT(FALSE);
	}
	int nKlineToOverWrite = nFootE - nFootB;

	
	Kline* pKlineFile = NULL;
	Kline* pKlineMid = pKline;
	CArray<Kline,Kline&> klineArray;
	int nWrite2=0;

	if(nType == this->overWriteAll)
	{
	
		int nMove = nWrite -nKlineToOverWrite;
		int nRtn;
		if(nMove!=0)
		{
			nRtn = ReadKLine( symbol,pKlineFile,pKlineSmallHead->numKln - nFootE,1);
			ASSERT(nRtn == pKlineSmallHead->numKln - nFootE);
		}

	
		nWrite2 = pKlineSmallHead->numKln-nFootE;
		if(nWrite2<0 || nMove==0)
			nWrite2 =0;

	}
	else
	{
		
		nWrite2 = pKlineSmallHead->numKln-nFootE;
		if(nWrite2<0)
			nWrite2 =0;
		int i ;
		Kline kln;
		if(nWrite2!=0)
		{
			for( i=nFootB;i<nFootE;i++)
			{
				int blkCount= i / FixedKlinePerBlock  ;	  
				int stockCount=i % FixedKlinePerBlock  ;	 
				int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
						* KlineByteEach *  FixedKlinePerBlock
							 + stockCount * KlineByteEach;
											 
				this->Seek(addr,this->begin);
				Read(&kln,KlineByteEach);
				klineArray.Add (kln);
			}
		}

		bool bDay  = false;
		if((m_kindKline+1)%2==1)
			bDay = true;
		for(i=0;i<nWrite;i++)
		{
			int nDay =RegularTime(pKline[i].day ,bDay);
			if(klineArray.GetSize()==0)
			{
				klineArray.Add(pKline[i]);
				continue;
			}
			for(int j=0;j< klineArray.GetSize();j++)
			{
				if(RegularTime(klineArray[j].day ,bDay)<nDay)
				{
					klineArray.InsertAt(j,pKline[i]);
					break;
				}
				else if(RegularTime(klineArray[j].day ,bDay) == nDay)
					break;
			}
		}
		int nMove = klineArray.GetSize () -nKlineToOverWrite;//>0 is 'write from back' ,==0 is need not write,<0 is ' write from begin'
		if(nMove==0)
			nWrite2 =0;
		if(nWrite2>0)
		{
			int nRtn = ReadKLine( symbol,pKlineFile,pKlineSmallHead->numKln - nFootB,1);
			ASSERT(nRtn == pKlineSmallHead->numKln - nFootB);
		}

		pKlineMid = klineArray.GetData ();
	}
	int nDeleted = 0;
	int blkCount= nFootB / FixedKlinePerBlock  ;	  
	for(int i=nFootB;i<nFootB+ nWrite + nWrite2;i++)
	{
		int stockCount=i % FixedKlinePerBlock  ;	 


		if(stockCount == 0)
		{
			blkCount= i / FixedKlinePerBlock  ;	 
			nDeleted += CreateOrMoveSmallBlock(pKlineSmallHead, blkCount);
		}

		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockCount * KlineByteEach;
									  
		this->Seek(addr,this->begin);

		Kline * pKln = pKlineMid+i-nFootB;
		if(i>=nFootB+ nWrite)//from file old
			pKln = pKlineFile+i-nFootB-nWrite;
		if(!CTaiShanKlineShowView::IsIndexStock3(symbol))
			TestKlineVolpositive(pKln, 1);
		Write(pKln,KlineByteEach);

	}
	
	pKlineSmallHead->numKln = nFootB+ nWrite + pKlineSmallHead->numKln-nFootE - nDeleted;
	ASSERT(pKlineSmallHead->numKln>=0);
	if(pKlineFile != NULL)
		delete [] pKlineFile;

	SetKlineSmallHeader(nIndexStock,pKlineSmallHead);
	return true;

}

bool CTaiKlineFileKLine::WriteKlinePeriod(CString symbol, Kline *pKline,int nKline, CTime& timeStart, CTime& timeEnd)//写一段时间的K线数据
{

	return true;
}
bool CTaiKlineFileKLine::SetKlineSmallHeader(int nIndex, KLINE_SMALLHEAD *pKlineSmallHead)
{
	int nStock = this->GetStockNumber ();
	ASSERT(nIndex<nStock);
	if(nIndex>=nStock)
		return false;

	int addr = 16+nIndex*64;
	this->Seek(addr,this->begin);
	CString s(pKlineSmallHead->StockSign );
	KLINE_SMALLHEAD *pKlineSmallHead2 = (KLINE_SMALLHEAD *)this->GetFileCurrentPointer ();
	CString s2(pKlineSmallHead2->StockSign );
	if(s!=s2)
	{
		ASSERT(FALSE);

	}
	this->Write (pKlineSmallHead,sizeof(KLINE_SMALLHEAD));
	return true;
}
int CTaiKlineFileKLine::GetKlineSmallHeader(CString symbol,KLINE_SMALLHEAD* pKlineSmallHead)//get the small header pointer of a stock
{
	if(m_pSymbolToPos == NULL)
		AddIndexToMap();

	KLINE_SMALLHEAD* pKlineSmallHead3 = NULL;

	int i;
	LookUpIndexFromMap( symbol,i);



	if(i==-1)
	{
	
		i = AddNewStockToFile(symbol,pKlineSmallHead3);
		if(i<0)
			ASSERT(FALSE);
	}
	else
	{
		int addr = 16;
		this->Seek(addr+i*64,this->begin);
		pKlineSmallHead3 = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
	}

	memcpy(pKlineSmallHead,pKlineSmallHead3,sizeof(KLINE_SMALLHEAD));
	ASSERT(pKlineSmallHead3!=NULL);

	return i;
}



int CTaiKlineFileKLine::CalcMaxCountBlock()
{
	int rtn = 5 ;
	switch(this->m_kindKline )
	{
	case DAY_KLINE:
		if(CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfDayKline<0)
			rtn = MAX_BLOCK_USE;
		else
			rtn = CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfDayKline/FixedKlinePerBlock+1;//((CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfDayKline%FixedKlinePerBlock==0)?2:1);
		break;
	case MIN5_KLINE:
		if(CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfMin5Kline<0)
			rtn = MAX_BLOCK_USE;
		else
			rtn = 48*CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfMin5Kline/FixedKlinePerBlock+1;//((48*CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfMin5Kline%FixedKlinePerBlock==0)?2:1);
		break;
	}
	if(rtn<2) rtn = 2;

	if(rtn>MAX_BLOCK_USE)
		rtn = MAX_BLOCK_USE;
	return rtn;

}

int CTaiKlineFileKLine::CreateOrMoveSmallBlock(KLINE_SMALLHEAD *pKlineSmallHead,int& nBlock)//to create a new block,or move

{

	int rtn = 0;
	int nMax = CalcMaxCountBlock();
	if(nBlock>=nMax)
	{
		WORD first = pKlineSmallHead->symBlock[0];
		for(int j = 0;j<nMax-1;j++)
		{
			pKlineSmallHead->symBlock[j] = pKlineSmallHead->symBlock[j+1];
		}
		pKlineSmallHead->symBlock[nMax-1] = first;


		nBlock = nMax-1;
		ASSERT(pKlineSmallHead->symBlock[nBlock]!=0xffff);
		if(nBlock<0)
		{
			ASSERT(FALSE);
			nBlock=0;
		}
		rtn = FixedKlinePerBlock;
		return rtn;
	}

	if(pKlineSmallHead->symBlock[nBlock]==0xffff)
	{
		int nCountBlock = this->GetSmallBlockCount ();
		this->SetSmallBlockCount (nCountBlock+1);
		int addr2 = KLINESMALLHEAD + nCountBlock 
				* KlineByteEach *  FixedKlinePerBlock;
									  
		char buff[ KlineByteEach *  FixedKlinePerBlock];
		memset(buff,255,KlineByteEach *  FixedKlinePerBlock);
		this->Seek(addr2,this->begin);
		this->Write (buff,KlineByteEach *  FixedKlinePerBlock);



		ASSERT(nBlock>=0&&nBlock<26);
		pKlineSmallHead->symBlock[nBlock]=(WORD)nCountBlock;
		nCountBlock++;
	}


	if(m_bFirstWrite == true)
	{
		CMap<int ,int ,int,int> map;
		
		int nStock = GetStockNumber ();
		for(int i=0;i<nStock;i++)
		{
			int addr = 16+i*64;
			Seek(addr,CTaiKlineFileKLine::begin);
			KLINE_SMALLHEAD klineSmallHead ;
			KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
			Read(pKlineSmallHead,sizeof(KLINE_SMALLHEAD));
			CString symbol (pKlineSmallHead->StockSign);
			for(int j=0;j<26;j++)
			{
				if(pKlineSmallHead->symBlock [j] == 0xffff) break;
				int iData = -1;
				int iIn = pKlineSmallHead->symBlock [j] ;
				if(iIn == 3844 || iIn == 3843)
					iIn = iIn;
				if(map.Lookup(iIn,iData))
				{

					int nBolck = j;
					ASSERT(j>=0&&j<26);
					if(pKlineSmallHead->numKln >(j)*FixedKlinePerBlock)
						pKlineSmallHead->numKln = (j)*FixedKlinePerBlock;
					for(int k=j;k<26;k++)
						pKlineSmallHead->symBlock [k] = 0xffff;

					this->SetKlineSmallHeader (i,pKlineSmallHead);
					break;

								;
				}
				else
					map[iIn] = iIn;
			}
		}

		m_bFirstWrite = false;
	}
	return 0;

}
BOOL CTaiKlineFileKLine::Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd,CFileException* pException)
{	
	BOOL bOk = TRUE;
	bOk = CTaiKlineMemFile::Open( lpszFileName,  nOpenFlags,  0,
	 pException);
	if(bOk == FALSE) return bOk;


	DWORD dwLen = this->GetLength ();
	if(dwLen<=KLINESMALLHEAD || bOk == FALSE)
	{
		this->ReMapFromBegin (KLINESMALLHEAD+500* KlineByteEach *  FixedKlinePerBlock);


		WriteHeaderInfo();

	}
	else
	{
		//
		int nAdd;
		switch(this->m_kindKline )
		{
		case DAY_KLINE:
			nAdd = 10;
			break;
		case MIN5_KLINE:
			nAdd = 100;
			break;
		}
		int bID = 1;
		if(this->m_kindKline==MIN5_KLINE && GetID()!=65798809) bID =0;
		int nBlock = this->GetSmallBlockCount();
		if(this->GetStockNumber ()>4096 || nBlock>4096*MAX_BLOCK_USE 
			|| this->GetStockNumber ()<0 
			|| nBlock<0
			|| bID ==0)
			WriteHeaderInfo();
		else
		{
			int nLen = this->GetLength ();
			nLen -= KLINESMALLHEAD;
			if(nLen>0)
			{
				nLen = nLen/( KlineByteEach *  FixedKlinePerBlock);
				if(nLen%( KlineByteEach *  FixedKlinePerBlock)!=0)
					nLen++;
				if(nBlock>nLen)
				{
					ASSERT(FALSE);
					this->SetSmallBlockCount (nLen);

					int nStock = GetStockNumber ();
					for(int i=0;i<nStock;i++)
					{
						int addr = 16+i*64;
						Seek(addr,CTaiKlineFileKLine::begin);
						KLINE_SMALLHEAD klineSmallHead ;
						KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
						Read(pKlineSmallHead,sizeof(KLINE_SMALLHEAD));
						CString symbol (pKlineSmallHead->StockSign);
						for(int j=0;j<26;j++)
						{
							if(pKlineSmallHead->symBlock [j] == 0xffff) break;
							int iData = -1;
							int iIn = pKlineSmallHead->symBlock [j] ;
							if(iIn>=nLen)
							{
								pKlineSmallHead->numKln = (j)*FixedKlinePerBlock;
								for(;j<26;j++)
									pKlineSmallHead->symBlock [j] = 0xffff;
								this->SetKlineSmallHeader (i,pKlineSmallHead);
								break;
							}
						}
					}
				}
			}

		}

	}

	MaxNumStock = GetMaxNumStock();
	ASSERT(MaxNumStock == 4096);

	return TRUE;
}

void CTaiKlineFileKLine::WriteHeaderInfo()
{
	this->SeekToBegin();
	int nStock=0;
	Write(&nStock,4);
	Write(&nStock,4);
	nStock=65798809;
	Write(&nStock,4);
	WORD wd = 240;
	Write(&wd,2);
	wd=4096;
	Write(&wd,2);

	char buff[4096];
	memset(buff,255,4096);
	for(int i=0;i<64;i++)
		Write(buff,4096);



}

int CTaiKlineFileKLine::AddNewStockToFile(CString symbol,KLINE_SMALLHEAD *&pKlineSmallHead)//add new stock to kline file
{
	int nStock = this->GetStockNumber ();
	int nLen = 6;
	if(nStock>=MaxNumStock)
	{
		bool bNeedMove = true;
		if(MaxNumStock == 4096)
		{
			bool bSh = false, bSz = true;
			if(m_nMarket == SH_MARKET_EX)
			{
				bSh = true;
				bSz = false;
			}

			ReorgnizeFile( bSh,  bSz,true);
			ReorgnizeFile( bSh,  bSz,false);
			nStock = this->GetStockNumber ();
			if(nStock<4096) bNeedMove = false;
		}
		if(MaxNumStock > 4096 || bNeedMove == true)
		{
			AddSmallHeadBlock();
			nStock = this->GetStockNumber ();
		}

	}

	int addr = 16+nStock*64;
	this->Seek(addr,this->begin);
	if(!(symbol.GetLength ()==4||symbol.GetLength ()==6))
	{
		symbol = symbol.Left (nLen);
	}
	symbol = CheckSymbolValid(symbol);

	int rValue = 0;
	if(m_pSymbolToPos->Lookup (symbol,(void*&)rValue))
	{
		addr = 16+(int)(rValue)*64;
		this->Seek(addr,this->begin);
		pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
		return rValue;
	}
	
	Write(symbol.GetBuffer (8),8);

	int nKline = 0;
	Write(&nKline,4);

	this->Seek(addr,this->begin);
	pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();

	//
	m_pSymbolToPos->SetAt(symbol,(CObject*)nStock);
	nStock++;
	this->SetStockNumber (nStock);

	return nStock-1;

}


bool CTaiKlineFileKLine::WriteKLineToRepair(CString symbol, Kline *pKline, int nWrite)//to repair day kline in real time
{
	ASSERT(nWrite>0);
	if(nWrite<=0)
		return false;

	bool bAdd;

	CTime timeStart((time_t)pKline[0].day );

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);

	int nFootB = TimeToFoot(pKlineSmallHead,timeStart, bAdd);

	if(nFootB != -1 && bAdd == false)
	{
		timeStart = CTime((time_t)pKline[nWrite-1].day );
		int nFootE = TimeToFoot(pKlineSmallHead,timeStart, bAdd);
		if(nFootE!= -1 && bAdd == false)
		{
			if(nFootE-nFootB+1==nWrite)
				return false;
		}
	}

	WriteKLine( symbol, pKline,nWrite,0);

	return true;

}

bool CTaiKlineFileKLine::CheckSymbol(CString symbol)
{
	return true;
}
void CTaiKlineFileKLine::InitMapOfDoc()
{
}
void CTaiKlineFileKLine::CreateSmallBlock()
{
}
void CTaiKlineFileKLine::AddIndexToMap()
{
	int nStock = this->GetStockNumber();
	m_pSymbolToPos = new CMapStringToPtr((int)(nStock*1.25)+1);
	ASSERT(nStock<=MaxNumStock);
	for(int i=0;i<nStock;i++)
	{
		int addr = 16+i*64;
		this->Seek(addr,this->begin);
		KLINE_SMALLHEAD* pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
		CString symbol (pKlineSmallHead->StockSign);
		m_pSymbolToPos->SetAt(symbol,(CObject*)i);

	}
}

void CTaiKlineFileKLine::LookUpIndexFromMap(CString symbol,int& nIndex)
{
	nIndex = -1;
	if(m_pSymbolToPos==NULL)
		AddIndexToMap();
	if(!m_pSymbolToPos->Lookup(symbol,(void*&)nIndex))
		nIndex = -1;
}



CTaiKlineFileKLine* CTaiKlineFileKLine::GetFilePointer(CString symbol,int stkKind,bool bDayKline)
{
	CTaiKlineFileKLine*	pFile;

	if(bDayKline == true)
		pFile=CTaiKlineFileKLine::m_fileDaySh ;
	else
		pFile=CTaiKlineFileKLine::m_fileMin5Sh ;
	if(SZ_MARKET_EX == CSharesCompute::GetMarketKind(stkKind))
	{
		if(bDayKline == true)
			pFile=CTaiKlineFileKLine::m_fileDaySz ;
		else
			pFile=CTaiKlineFileKLine::m_fileMin5Sz ;
	}

	return pFile;

}

int CTaiKlineFileKLine::ReadKlineAccordingTime(CString symbol, Kline *&kline, CTime &time, int nCount,int nAddBlank)
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);

	if(nCount<=0)
		return 0;

	if(kline!= NULL) delete []kline;
	kline = new Kline[nCount+nAddBlank];

	this->SeekToBegin ();

	int k=0;
	bool bDay = false;
	if((m_kindKline+1)%2==1)
		bDay = true;
	int tm = RegularTime((int)time.GetTime (),bDay);

	
	for(int i=pKlineSmallHead->numKln-1;i>=0 ;i--)
	{
		if(k>=nCount)
			break;
		int blkCount= i / FixedKlinePerBlock  ;	  
		int stockCount=i % FixedKlinePerBlock  ;	  
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockCount * KlineByteEach;
									  
		this->Seek(addr,this->begin);
		Read(kline+nCount-1 - k ,KlineByteEach);
		if(RegularTime(kline[nCount-1 - k].day,bDay) <= tm)
			k++;
	}

	if(k<nCount)
		memmove(kline,kline+nCount-k,k*sizeof(Kline));

	if(!CTaiShanKlineShowView::IsIndexStock3(symbol))
		TestKlineVolpositive(kline, k);
	return k;


}

int CTaiKlineFileKLine::ReadKLine(int nIndex, Kline *&pKline, int nRead)
{
	CString symbol = GetSymbol( nIndex);
	return ReadKLine( symbol,pKline, nRead,0);
}

int CTaiKlineFileKLine::ReadKlinePeriod(int nIndex, Kline *&kline, CTime &timeStart, CTime &timeEnd,int nAddBlank)
{
	CString symbol = GetSymbol( nIndex);
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4)
		return 0;
	return ReadKlinePeriod( symbol, kline, timeStart, timeEnd,true, nAddBlank);
}

CString CTaiKlineFileKLine::GetSymbol(int nIndex)
{
	int addr = 16;
	this->Seek(addr+nIndex*64,this->begin);
	KLINE_SMALLHEAD * pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
	CString symbol(pKlineSmallHead->StockSign );
	return symbol;

}

void CTaiKlineFileKLine::ReorgnizeFile(bool bSh, bool bSz,bool bDayKline)
{
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	CTaiShanDoc* pDoc = pFm->m_taiShanDoc ;

	int nStockTypeCount[2][6] = {{SHZS,SHAG,SHBG,SHZQ,SHJIJIN,STKTYPE},{SZZS,SZAG,SZBG,SZZQ,SZJIJIN,-1}};
	for(int ii = 0;ii<2;ii++)
	{
		if(ii == 0 && bSh==false) continue;
		if(ii == 1 && bSz==false) continue;

		CTaiKlineFileKLine* pFile = ii == 0?CTaiKlineFileKLine::m_fileDaySh: CTaiKlineFileKLine::m_fileDaySz;
		if(bDayKline == false)
			pFile =ii == 0 ? CTaiKlineFileKLine::m_fileMin5Sh: CTaiKlineFileKLine::m_fileMin5Sz;// CTaiKlineFileKLine::m_fileMin5Sh  ;

		
		int n0 ;
		int i;
		KLINE_SMALLHEAD klineSmallHead;
		CArray<KLINE_SMALLHEAD,KLINE_SMALLHEAD> arrayHead;
		CArray<KLINE_SMALLHEAD,KLINE_SMALLHEAD> arrayHeadAll;
		int nStock = pFile->GetStockNumber ();
		int nDelete[8192];
		for(i=0;i<nStock;i++)
		{
			int addr = 16;
			pFile->Seek(addr+i*64,pFile->begin);
			KLINE_SMALLHEAD* pKlineSmallHead3 = (KLINE_SMALLHEAD*)pFile->GetFileCurrentPointer();
			klineSmallHead = * pKlineSmallHead3;
			arrayHeadAll.Add (klineSmallHead);
			nDelete[i]=0;
		}

		PCdat1 pStockData;
		int k;
		for(int kk =0;kk<6;kk++)
		{
			if(nStockTypeCount[ii][kk]>=0) continue;

			k = nStockTypeCount[ii][kk];
			n0 = pDoc->m_sharesInformation .GetStockTypeCount(k);
			for(i=0;i<n0;i++)
			{
				pDoc->m_sharesInformation .GetStockItem(k,i,pStockData);
				ASSERT(pStockData!=NULL);
				if(pStockData==NULL)
					continue;
				CString symbol(pStockData->id);
				int k;
				pFile ->LookUpIndexFromMap( symbol,k);
				if(k==-1)
					continue;
				k = pFile ->GetKlineSmallHeader( symbol,&klineSmallHead);
				if(k<0) k = 4096;
				nDelete[k] = 1;
				if(strlen(klineSmallHead.StockSign )!=4&&strlen(klineSmallHead.StockSign )!=6)
					ASSERT(FALSE);
				arrayHead.Add (klineSmallHead);
			}
		}

		
		pFile ->m_pSymbolToPos->RemoveAll();
		k = arrayHead.GetSize();
		pFile ->SetStockNumber(k);
		for(int j=0;j<k;j++)
		{
			int addr = 16+j*64;
			pFile ->Seek(addr,pFile ->begin);
			pFile ->Write (&arrayHead[j],sizeof(KLINE_SMALLHEAD));
			CString s2(arrayHead[j].StockSign );
			pFile ->m_pSymbolToPos->SetAt(s2, (void*)j);
		}
		for(int j = 0;j<nStock;j++)
		{
			if(nDelete[j] == 0)
			{
				int addr = 16+k*64;
				pFile ->Seek(addr,pFile ->begin);
				pFile ->Write (&arrayHeadAll[j],sizeof(KLINE_SMALLHEAD));
				CString s2(arrayHeadAll[j].StockSign );

				k++;
			}
		}

	}
}

int CTaiKlineFileKLine::RegularTime(int time,bool bDay)
{
	int nRate = 1;
	int nBias = 0;
	if(bDay == true)
	{
		nRate = 24*60*60;
		nBias = 8*60*60;
	}

	return (time+nBias)/nRate;


}

int CTaiKlineFileKLine::ReadKlineTimeSpan(CString symbol,int stkKind, Kline *&pKline, CTime &timeB, CTime &timeE, int nKlineType, CBuySellList *pBuySellList, int nAddBlank)
{
	return ReadKLineAny(symbol,stkKind, pKline,0,  nKlineType, pBuySellList, nAddBlank,  false, &timeE, &timeB);
}

bool CTaiKlineFileKLine::OutputCloseVal(SymbolKindArr &strSymbolArray, int& nDays,CTime& tmStart,CTime &tmEnd,double*& pdbl,Kline*& pKlineIndex)
{
	
	ASSERT(nDays>=0);
	ASSERT(pdbl == NULL);
	int n= strSymbolArray.GetSize();
	Kline *pKline = NULL;
	if(nDays>0)
	{
		pdbl = new double[n*nDays];
		for(int i=0;i<n;i++)
		{
			int nRtn = ReadKLineAny(strSymbolArray[i].m_chSymbol,strSymbolArray[i].m_nSymbolKind,pKline,nDays,5,NULL, 48 ,false);
			if(nRtn<0)
				nRtn = 0;
			for(int j=0;j<nDays;j++)
			{
				if(nRtn>=nDays)
				{
					pdbl[i+j*n]=pKline[nRtn-nDays+j].close ;
				}
				else
				{
					if(j+nRtn<nDays)
						pdbl[i+j*n]=0;
					else
						pdbl[i+j*n]=pKline[j-nDays+nRtn].close ;
				}
			}

		}
	}
	else
	{
	
		while(1)
		{
			int nD = CTaiKlineFileKLine::ReadKlineTimeSpan(CSharesCompute::GetIndexSymbol(0),0, pKline, tmStart, tmEnd, 5, NULL, 48);
			if(nD<=0)
				break;

			if(pKline ==NULL)
				break;

			ASSERT(pKlineIndex==0);
			pKlineIndex = pKline;
			pKline = NULL ;

			nDays = nD;
			pdbl = new double[n*nDays];
			for(int i=0;i<n;i++)
			{
				int nRtn = CTaiKlineFileKLine::ReadKlineTimeSpan(strSymbolArray[i].m_chSymbol,strSymbolArray[i].m_nSymbolKind, pKline, tmStart, tmEnd, 5, NULL, 48);
				if(nRtn<0)
					nRtn = 0;
				int k=0;
				for(int j=0;j<nDays;j++)
				{
					float f=0;
					CString s = CTime(pKlineIndex [j].day).Format ("%Y%m%d");
					if(k<nRtn)
					{
						CString s2 = CTime(pKline [k].day).Format ("%Y%m%d");
						if( s==s2)
						{
							f = pKline[k].close ;
							k++;
						}
						else 
						{
							CTime t(pKline [k].day);
							s2 = t.Format ("%Y%m%d");
							while(s2 < s)
							{
								if(s==s2)
								{
									f= pKline [k].close;
									k++;
									break;
								}

								k++;
								if(k>=nRtn)
									break;
							}
						}
					}

					
					if(f<=0 || f>10000) 
						f=0;
					pdbl[i+j*n]=f;
				}

			}

			break;
		}
	}
	if(pKline!=NULL)
		delete [] pKline;


	return true;
}

bool CTaiKlineFileKLine::IsNeedHs(int nKlineType)
{
	return nKlineType==HS_K_LINE||nKlineType==MINUTE_1_K_LINE
		||(nKlineType==MANY_DAY_K_LINE&&CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfManyKline<0);

}

int CTaiKlineFileKLine::ReadKLineAny(CString symbol,int stkKind, Kline *&pKline, int nRead, int nKlineType, CBuySellList *pBuySellList, int nAdd,bool bToday, CTime * tmEnd,CTime *tmBegin)
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	int n = 0;

	if(nRead == -1)
		nRead = FixedKlinePerBlock*26;
	//
	switch(nKlineType)
	{
	case 1:
	case 5:
		break;
	case 2:
		nRead*=3;
		break;
	case 3:
		nRead*=6;
		break;
	case 4:
		nRead*=12;
		break;
	case 6:
		nRead*=5;
		break;
	case 7:
		nRead*=23;
		break;
	case 8:
		nRead*=260;
		break;
	case MANY_DAY_K_LINE:
		if(pDoc->m_propertyInitiate.daysOfManyKline>0)
			nRead*=pDoc->m_propertyInitiate.daysOfManyKline;
		break;
	}
	
	if(IsNeedHs(nKlineType))
	{
		
		CTaiKlineFileHS*	pFileHs;
		pFileHs=CTaiKlineFileHS::GetFilePointer (symbol,stkKind);
		pFileHs->ReadHS (symbol,*pBuySellList,true);

		CTaiKlineTransferKline trans(pBuySellList);
		if(tmEnd!=NULL)
		{
			bToday = false;
			if(tmEnd->Format("%Y%m%d")>=CTime::GetCurrentTime ().Format("%Y%m%d"))
				bToday = true;
		}
		trans.TransferKlineHistory(symbol,stkKind,pKline, n,nKlineType,bToday);

		if(pKline == NULL) 
		{
			pKline = new Kline;
			return 0;
		}
		ASSERT(n>=0);
	}
	else
	{

		bool bDay = true;
		if(nKlineType>0&&nKlineType<5)
			bDay = false;
		if(symbol.GetLength()!=4 && symbol.GetLength()!=6)
		{
			return 0;
		}
		CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol,stkKind,bDay);

		//read now
		if(tmEnd ==NULL && tmBegin == NULL)
			n = pFile->ReadKLine(symbol,pKline,nRead,nAdd)	;
		else if(tmEnd!=NULL && tmBegin != NULL)
			n = pFile->ReadKlinePeriod( symbol, pKline, *tmBegin, *tmEnd,false,nAdd );
		else
		{
			if(nRead == -1) nRead = 5000;
			n = pFile->ReadKlineAccordingTime(symbol, pKline, *tmEnd, nRead,nAdd);
		}

	
		CTaiKlineTransferKline trans;
		if(tmEnd!=NULL)
		{
			bToday = false;
			if(tmEnd->Format("%Y%m%d")>=CTime::GetCurrentTime ().Format("%Y%m%d"))
				bToday = true;
		}
		trans.TransferKlineHistory(symbol,stkKind,pKline, n,nKlineType,bToday,true);
	}
	return n;

}


int CTaiKlineFileKLine::GetFirstKline(CString sSymbol,int stkKind,bool bDay,int nFoot ,float * pClose)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(sSymbol,stkKind,bDay);
	KLINE_SMALLHEAD smallHead;
	pFile->GetKlineSmallHeader (sSymbol,&smallHead);
	if(smallHead.numKln <=0)
	{
		if(nFoot!=-1) *pClose = -1;
		return -1;
	}

	Kline kln;
	if(nFoot !=-1)
	{
		if(smallHead.numKln <=nFoot)
		{
			*pClose = -1;
			return -1;
		}
		int blkCount= nFoot / FixedKlinePerBlock  ;	 
		int stockCount=nFoot % FixedKlinePerBlock  ;	  
		int addr = KLINESMALLHEAD + smallHead.symBlock[blkCount] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockCount * KlineByteEach;
									  
		pFile->Seek(addr,pFile->begin);
		pFile->Read(&kln,KlineByteEach);
		*pClose = kln.close ;
	}
	else
	{
		int addr = KLINESMALLHEAD + smallHead.symBlock[0] 
				* KlineByteEach *  FixedKlinePerBlock;
									  
		pFile->Seek(addr,pFile->begin);
		pFile->Read(&kln,KlineByteEach);
	}

	return kln.day ;
}

bool CTaiKlineFileKLine::IsDayKline(int nKlineType)
{
	return nKlineType>=5 && nKlineType<= 8
		||(nKlineType==MANY_DAY_K_LINE&&CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfManyKline>0);

}

void CTaiKlineFileKLine::DeleteKlineData(CString symbol, int nFoot, int nCount)
{
	Kline* pKline = NULL;
	int n = ReadKLine(symbol,pKline,-1,0);	
	if(n<=0) return;
	if(n>nFoot+1) 
		memmove(pKline+nFoot,pKline+nFoot+1,(n-nFoot-1)*sizeof(Kline));
	n--;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);
	pKlineSmallHead->numKln = 0;
	SetKlineSmallHeader(nIndexStock,pKlineSmallHead);

	if(n>0) this->WriteKLine(symbol,pKline,n,0);

	if(pKline == NULL)
		delete [] pKline;
}

void CTaiKlineFileKLine::DeleteMap()
{
	if(m_pSymbolToPos!=NULL)
	{
		m_pSymbolToPos->RemoveAll ();
		delete m_pSymbolToPos;
		m_pSymbolToPos = NULL;
	}

}

void CTaiKlineFileKLine::ZeroKlineData(CString symbol,int stkKind,bool bDay)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol,stkKind,bDay);
	KLINE_SMALLHEAD smallHead;
	int nIndex = pFile->GetKlineSmallHeader (symbol,&smallHead);
	smallHead.numKln = 0;
	pFile->SetKlineSmallHeader (nIndex,&smallHead);
}

CString CTaiKlineFileKLine::CheckSymbolValid(CString symbol)
{
	int nLen = symbol.GetLength ();
	bool bValid = true;
	for(int i=0;i<nLen;i++)
	{
		if(!(symbol[i]>='0' && symbol[i]<='9' 
			|| (i==1&& symbol[i]>='A' && symbol[i]<='Z' )))
		{
			bValid = false;
		}
	}
	if(bValid == false)
	{
		symbol = "0Z00";
		if(nLen == 6)
			symbol+="00";
	}
	return symbol;

}

float CTaiKlineFileKLine::GetLastClose(CString symbol,int stkKind)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol,stkKind,true);//CTaiKlineFileKLine::m_fileDaySh ;
	KLINE_SMALLHEAD smallHead;
	pFile->GetKlineSmallHeader (symbol,&smallHead);
	if(smallHead.numKln <=0)
	{
		return -1;
	}

	Kline* pKline = NULL;
	float f = -1;
	int n=pFile->ReadKLine (symbol,pKline,1,0);
	if(n>0)
	{
		if(pKline!=NULL)
			f = pKline->close ;
	}
	if(pKline!=NULL) delete [] pKline;

	return f ;

}

void CTaiKlineFileKLine::TestKlineVolpositive(Kline *pKline, int nCount)
{
	for(int i=0;i<nCount;i++)
	{
		if(pKline[i].volPositive  >pKline[i].vol 
			||pKline[i].volPositive  <1 && pKline[i].volPositive!=0 )
		{
			pKline[i].volPositive = pKline[i].vol/2;
		}
	}

}

bool CTaiKlineFileKLine::OpenAll()
{
	if(!m_fileDaySh)
	m_fileDaySh = new CTaiKlineFileKLine(0,SH_MARKET_EX);
	if(!m_fileDaySz)
	m_fileDaySz = new CTaiKlineFileKLine(0,SZ_MARKET_EX);
	if(!m_fileMin5Sh)
	m_fileMin5Sh = new CTaiKlineFileKLine(1,SH_MARKET_EX);
	if(!m_fileMin5Sz)
	m_fileMin5Sz = new CTaiKlineFileKLine(1,SZ_MARKET_EX);
	try
	{

		CreateDirectory("DATA\\SH",NULL);
		CreateDirectory("DATA\\SZ",NULL);
		CreateDirectory("DATA\\SH\\f10",NULL);
		CreateDirectory("DATA\\SZ\\f10",NULL);
	}
	catch(...)
	{
	}


	CString sPath2 = g_daysh;
	bool bRtn = true;
	if(!m_fileDaySh->Open(sPath2, 0,NULL))
	{
		delete m_fileDaySh;
		m_fileDaySh=NULL;
		bRtn = false;
		AfxMessageBox("打开上海日线文件时出错，请关闭程序后，删除文件 data\\sh\\daykline.dat ",MB_OK|MB_ICONERROR);
	}
	sPath2 = g_daysz;
	if(!m_fileDaySz->Open(sPath2, 0,NULL))
	{
		delete m_fileDaySz;
		m_fileDaySz=NULL;
		bRtn = false;
		AfxMessageBox("打开深圳日线文件时出错，请关闭程序后，删除文件 data\\sz\\daykline.dat ",MB_OK|MB_ICONERROR);
	}

	sPath2 = g_minutesh;
	if(!m_fileMin5Sh->Open(sPath2, 0,NULL))
	{
		delete m_fileMin5Sh;
		m_fileMin5Sh=NULL;
		bRtn = false;
		AfxMessageBox("打开上海5分钟K线文件时出错",MB_OK|MB_ICONERROR);
	}
	sPath2 = g_minutesz;
	if(!m_fileMin5Sz->Open(sPath2, 0,NULL))
	{
		delete m_fileMin5Sz;
		m_fileMin5Sz=NULL;
		bRtn = false;
		AfxMessageBox("打开深圳5分钟K线文件时出错 ",MB_OK|MB_ICONERROR);
	}
	return bRtn;

}

void CTaiKlineFileKLine::CloseAll()
{
	//  20010910
	if(m_fileDaySh != NULL)
	{
		m_fileDaySh->Close();
	}
	if(m_fileDaySz != NULL)
	{
		m_fileDaySz->Close();
	}
	if(m_fileMin5Sh != NULL)
	{
		m_fileMin5Sh->Close();
	}
	if(m_fileMin5Sz != NULL)
	{
		m_fileMin5Sz->Close();
	}
	if(m_fileDaySh != NULL)
	{

		delete m_fileDaySh;
		m_fileDaySh = NULL;
	}
	if(m_fileDaySz != NULL)
	{

		delete m_fileDaySz;
		m_fileDaySz = NULL;
	}
	if(m_fileMin5Sh != NULL)
	{

		delete m_fileMin5Sh;
		m_fileMin5Sh = NULL;
	}
	if(m_fileMin5Sz != NULL)
	{

		delete m_fileMin5Sz;
		m_fileMin5Sz = NULL;
	}

}

bool CTaiKlineFileKLine::WriteKLineS(CString symbol,int stkKind, Kline *pKline, int nWrite, int nType, bool bDay)
{
	CTaiKlineFileKLine * pFile = GetFilePointer(symbol,stkKind,bDay);
	if(pFile)
		pFile->WriteKLine (symbol, pKline,nWrite, nType);
	return true;
}

int CTaiKlineFileKLine::ReadKLineS(CString symbol,int stkKind, Kline *&pKline, int nRead, int nAddBlank, bool bDay)
{
	int n = 0;
	CTaiKlineFileKLine * pFile = GetFilePointer(symbol,stkKind,bDay);
	if(pFile)
		n = pFile->ReadKLine (symbol, pKline,nRead, nAddBlank);
	return n;
}

void CTaiKlineFileKLine::AddSmallHeadBlock()
{
	ASSERT(FALSE);

	int n = (MaxNumStock - 4096)/(240/2);
	if(n<0) return;

	
	int nStock = GetStockNumber ();
	KLINE_SMALLHEAD klineSmallHead;
	int j = 0;
	int i = 0;
	bool bFound = false;
	for(i=0;i<nStock;i++)
	{
		int addr = 16;
		Seek(addr+i*64,begin);
		Read(&klineSmallHead,sizeof(KLINE_SMALLHEAD));
		for(j = 0;j<26;j++)
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
		Kline kline;
		for(int k=0;k<FixedKlinePerBlock ;k++)
		{
			int blkCount= j ;	 
			int stockCount=k  ;	  
			int addr = KLINESMALLHEAD + klineSmallHead.symBlock[blkCount] 
					* KlineByteEach *  FixedKlinePerBlock
						 + stockCount * KlineByteEach;
										 
			this->Seek(addr,this->begin);
			Read(&kline,KlineByteEach);
			addr = KLINESMALLHEAD + klineSmallHead.symBlock[nNewBlock] 
					* KlineByteEach *  FixedKlinePerBlock
						 + stockCount * KlineByteEach;
			this->Seek(addr,this->begin);
			Write(&kline,KlineByteEach);
		}

	
		klineSmallHead.symBlock [j] = (WORD)nNewBlock;
		this->SetKlineSmallHeader(i, &klineSmallHead);

	
		nNewBlock++;
		SetSmallBlockCount(nNewBlock);
	}

	//2.
	MaxNumStock+=120;
	WORD nMaxNumStock = MaxNumStock;
	SetMaxNumStock(nMaxNumStock);
}

bool CTaiKlineFileKLine::Transfer4To6(bool bFirst)
{

	
	bool bRtn = false;
	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc ;
	if(bFirst == true)
	{
		if(pDoc->m_4or6==0)
		{
		
					
			if(_access("baseinfo.dat",0)==-1)
				return false;
	
			BASEINFOHEAD *FileHead=new BASEINFOHEAD;
			CFile f;
			f.Open("baseinfo.dat",CFile::modeReadWrite|CFile::typeBinary);
			f.Read(FileHead,sizeof(BASEINFOHEAD));
			if(FileHead->filetitle==55555555)
			{
				delete FileHead;
				f.Close();
			}
			else
			{
			BASEINFO *pRecordSet=new BASEINFO[FileHead->StockCount+1000];
			BASEINFO_OLD *pRecordSetOLD=new BASEINFO_OLD;
			for(int k=0;k<FileHead->StockCount;k++)
			{
				DWORD nKind;
				CString strSymbol;
				CString str6;		
				f.Read(pRecordSetOLD,sizeof(BASEINFO_OLD));
				strSymbol=pRecordSetOLD->Symbol;
				if(strSymbol.GetLength()==6)
				{
					nKind=CSharesInformation::GetStockKind(SH_MARKET_EX,strSymbol.GetBuffer(0));
					str6=strSymbol;
				}
				else if(strSymbol.GetLength()==4)
				{
					str6=strSymbol;
					nKind=CSharesInformation::GetStockKind(SZ_MARKET_EX,str6.GetBuffer(0));
				}
				else
				{
					f.Close();
					delete pRecordSetOLD;
					delete FileHead;
					delete []pRecordSet;
					return false;
				}
				CString myNeed=pDoc->GetStockKindString(nKind);
				myNeed+=str6;
				strcpy(pRecordSet[k].Symbol,myNeed.GetBuffer(0));
				memcpy(&(pRecordSet[k].NumSplit),&(pRecordSetOLD->NumSplit),sizeof(BASEINFO_OLD)-8);
				
			}
			FileHead->filetitle=55555555;
			FileHead->FileExitDone=88888888;
			f.SeekToBegin();
			f.Write(FileHead,sizeof(BASEINFOHEAD));
			f.Write(pRecordSet,sizeof(BASEINFO)*(FileHead->StockCount));	
			f.Close();
			delete pRecordSetOLD;
			delete FileHead;
			delete []pRecordSet;
			}


		
			
			if(_access("StockTypeInfo.dat",0)==-1)
				return false;
	
			CFile ff;
			
			
			ff.Open("StockTypeInfo.dat",CFile::modeReadWrite|CFile::typeBinary);
			STOCKTYPEHEAD *pStockTypeHead=new STOCKTYPEHEAD;
			
			ff.Read(pStockTypeHead,sizeof(STOCKTYPEHEAD));
			if(pStockTypeHead->m_lFileTitle==55555555)
			{
				delete pStockTypeHead;
				ff.Close();
			}
			else
			{
			int nMaxType=pStockTypeHead->m_lStockTypeMaxCount;
			int nCurrentStock=pStockTypeHead->m_lStockCount;
			STOCK_TYPE_INFO *pStockInfo=new STOCK_TYPE_INFO[pStockTypeHead->m_lStockCount+1000];
			ff.Seek(sizeof(STOCKTYPEHEAD)+sizeof(STOCKTYPEINFO)*nMaxType,CFile::begin);
			STOCK_TYPE_INFO_OLD *pMyNeed=new STOCK_TYPE_INFO_OLD;
			
			for(int kk=0;kk<nCurrentStock;kk++)
			{		
				DWORD nKind;
				CString str6;
				ff.Read(pMyNeed,sizeof(STOCK_TYPE_INFO_OLD));
				CString strSymbol=pMyNeed->m_szSymbol;
				
				if(strSymbol.GetLength()==6)
				{
					nKind=CSharesInformation::GetStockKind(SH_MARKET_EX,strSymbol.GetBuffer(0));
					str6=strSymbol;
				}
				else if (strSymbol.GetLength()==4)
				{
					str6=strSymbol;
					nKind=CSharesInformation::GetStockKind(SZ_MARKET_EX,str6.GetBuffer(0));
				}
				else
				{
					ff.Close();
					delete pStockTypeHead;
					delete pMyNeed;
					delete []pStockInfo;
					return false;
				}
				CString myNeed=pDoc->GetStockKindString(nKind);
				myNeed+=str6;
				strcpy(pStockInfo[kk].m_szSymbol,myNeed.GetBuffer(0));
				memcpy(&(pStockInfo[kk].m_bDeleted),&(pMyNeed->m_bDeleted),sizeof(STOCK_TYPE_INFO_OLD)-7);
			}	
			pStockTypeHead->m_lFileTitle=55555555;
			pStockTypeHead->m_lFileExit =88888888;
			ff.SeekToBegin();
			ff.Write(pStockTypeHead,sizeof(STOCKTYPEHEAD));
			ff.Seek(sizeof(STOCKTYPEHEAD)+sizeof(STOCKTYPEINFO)*nMaxType,CFile::begin);
			ff.Write(pStockInfo,sizeof(STOCK_TYPE_INFO)*nCurrentStock);
			ff.Close();
		
			delete pStockTypeHead;
			delete pMyNeed;
			delete []pStockInfo;
			}
			//=============
			
			return false;
		}
	}


	CTaiKlineFileKLine*	pFile;

	pFile=CTaiKlineFileKLine::m_fileDaySz ;
	int nStock = pFile->GetStockNumber();

	
	for(int i=0;i<nStock;i++)
	{
		int addr = 16+i*64;
		pFile->Seek(addr,begin);
		char ch[8];
		ch[7]=0;
		pFile->Read (ch,8);
		CString s = ch;

		//lmb11
		for(int k = 0;k<i;k++)
		{
			int addr2 = 16+k*64;
			pFile->Seek(addr2,begin);
			char ch2[8];
			ch2[7]=0;
			pFile->Read (ch2,8);
			CString s2 = ch2;
			if(s == s2 && s2!="0Z0000")
			{
				s2 = "0Z0000";
				pFile->Seek(addr,begin);
				pFile->Write (s2.GetBuffer (8),8);
				s2.ReleaseBuffer ();
				break;
			}
		}
	
		if(s.GetLength () <=4) 
		{

			s = CSharesInformation::Symbol4To6(s);
			strcpy(ch,s);
			pFile->Seek(addr,begin);
			pFile->Write (ch,8);
		}
	}


	pFile=CTaiKlineFileKLine::m_fileMin5Sz ;
	nStock = pFile->GetStockNumber();
	for(int i=0;i<nStock;i++)
	{
		int addr = 16+i*64;
		pFile->Seek(addr,pFile->begin);
		char ch[8];
		ch[7]=0;
		pFile->Read (ch,8);
		CString s = ch;
		if(s.GetLength () <=4) 
		{
			s = CSharesInformation::Symbol4To6(s);
			strcpy(ch,s);
			pFile->Seek(addr,pFile->begin);
			pFile->Write (ch,8);
		}
	}


	CTaiKlineFileHS::Symbol4To6();
	

	if(bFirst == false) 
	{
		return true;
	}
	CString delFile[]={"realtime.dat","stockname.dat","realtimehs.dat","AlarmSetting.ala"};
	CFileFind fd;
	CString lpszFileName;
	for(int km=0;km<4;km++)
	{		
		lpszFileName=delFile[km];
		if(fd.FindFile(lpszFileName)!=0)
		{
			CFile::Remove(lpszFileName);
		}		
	}
if(_access("baseinfo.dat",0)==-1)
		return false;

	BASEINFOHEAD *FileHead=new BASEINFOHEAD;
	CFile f;
	f.Open("baseinfo.dat",CFile::modeReadWrite|CFile::typeBinary);
	f.Read(FileHead,sizeof(BASEINFOHEAD));
	bool myflag;
	myflag=true;
	if(FileHead->filetitle==55555555)
	{
		myflag=false;
	}
	else
	{
		myflag=true;
	}
	BASEINFO *pRecordSet=new BASEINFO[FileHead->StockCount+1000];
	BASEINFO_OLD *pRecordSetOLD=new BASEINFO_OLD;
	for(int k=0;k<FileHead->StockCount;k++)
	{
		DWORD nKind;
		CString strSymbol;
		CString str6;		
		f.Read(pRecordSetOLD,sizeof(BASEINFO_OLD));
	
		if(!myflag)
		{
			CString strMy=pRecordSetOLD->Symbol;
			if (strMy.GetLength()==8)
			{
				strSymbol=strMy.Right(6);
			}
			else if(strMy.GetLength()==6)
			{
				strSymbol=strMy.Right(4);
			}
		}
		else
		{
			strSymbol=pRecordSetOLD->Symbol;
		}

		if(strSymbol.GetLength()==6)
		{
			nKind=CSharesInformation::GetStockKind(SH_MARKET_EX,strSymbol.GetBuffer(0));
			str6=strSymbol;
		}
		else if(strSymbol.GetLength()==4)
		{
			str6=CSharesInformation::Symbol4To6(strSymbol);
			nKind=CSharesInformation::GetStockKind(SZ_MARKET_EX,str6.GetBuffer(0));
		}
		else
		{
			f.Close();
			delete pRecordSetOLD;
			delete FileHead;
			delete []pRecordSet;
			return false;
		}
		CString myNeed=pDoc->GetStockKindString(nKind);
		myNeed+=str6;
		strcpy(pRecordSet[k].Symbol,myNeed.GetBuffer(0));
		memcpy(&(pRecordSet[k].NumSplit),&(pRecordSetOLD->NumSplit),sizeof(BASEINFO_OLD)-8);
		
	}	
	FileHead->FileExitDone=88888888;
	f.SeekToBegin();
	f.Write(FileHead,sizeof(BASEINFOHEAD));
	f.Write(pRecordSet,sizeof(BASEINFO)*(FileHead->StockCount));	
	f.Close();
	delete pRecordSetOLD;
	delete FileHead;
	delete []pRecordSet;

	if(_access("StockTypeInfo.dat",0)==-1)
		return false;

	CFile ff;
	bool bflag;
	bflag=false;

	ff.Open("StockTypeInfo.dat",CFile::modeReadWrite|CFile::typeBinary);
	STOCKTYPEHEAD *pStockTypeHead=new STOCKTYPEHEAD;
	
	ff.Read(pStockTypeHead,sizeof(STOCKTYPEHEAD));
	if(pStockTypeHead->m_lFileTitle==55555555)
	{
		bflag=false;
	}
	else
	{
		bflag=true;
	}

	int nMaxType=pStockTypeHead->m_lStockTypeMaxCount;
	int nCurrentStock=pStockTypeHead->m_lStockCount;
	STOCK_TYPE_INFO *pStockInfo=new STOCK_TYPE_INFO[pStockTypeHead->m_lStockCount+1000];
	ff.Seek(sizeof(STOCKTYPEHEAD)+sizeof(STOCKTYPEINFO)*nMaxType,CFile::begin);
	STOCK_TYPE_INFO_OLD *pMyNeed=new STOCK_TYPE_INFO_OLD;

	for(int kk=0;kk<nCurrentStock;kk++)
	{		
		DWORD nKind;
		CString str6;
		CString strSymbol;
		ff.Read(pMyNeed,sizeof(STOCK_TYPE_INFO_OLD));
		if(!bflag)
		{
			CString strMy=pMyNeed->m_szSymbol;
			if (strMy.GetLength()==8)
			{
				strSymbol=strMy.Right(6);
			}
			else if(strMy.GetLength()==6)
			{
				strSymbol=strMy.Right(4);
			}
		}
		else
		{
			strSymbol=pMyNeed->m_szSymbol;
		}
		
		

		if(strSymbol.GetLength()==6)
		{
			nKind=CSharesInformation::GetStockKind(SH_MARKET_EX,strSymbol.GetBuffer(0));
			str6=strSymbol;
		}
		else if (strSymbol.GetLength()==4)
		{			
			str6=CSharesInformation::Symbol4To6(strSymbol);
			nKind=CSharesInformation::GetStockKind(SZ_MARKET_EX,str6.GetBuffer(0));
		}
		else
		{
			ff.Close();
			delete pStockTypeHead;
			delete pMyNeed;
			delete []pStockInfo;
			return false;
		}
		CString myNeed=pDoc->GetStockKindString(nKind);
		myNeed+=str6;
		strcpy(pStockInfo[kk].m_szSymbol,myNeed.GetBuffer(0));
		memcpy(&(pStockInfo[kk].m_bDeleted),&(pMyNeed->m_bDeleted),sizeof(STOCK_TYPE_INFO_OLD)-7);
	}
	pStockTypeHead->m_lFileTitle=12345678;
	pStockTypeHead->m_lFileExit =88888888;
	ff.SeekToBegin();
	ff.Write(pStockTypeHead,sizeof(STOCKTYPEHEAD));
	ff.Seek(sizeof(STOCKTYPEHEAD)+sizeof(STOCKTYPEINFO)*nMaxType,CFile::begin);
	ff.Write(pStockInfo,sizeof(STOCK_TYPE_INFO)*nCurrentStock);
	ff.Close();

	delete pStockTypeHead;
	delete pMyNeed;
	delete []pStockInfo;
	
	
	
	CFileFind fff;
	CString strdir;
	::GetCurrentDirectory(MAX_PATH,strdir.GetBuffer(MAX_PATH));
	CString strdir1;

	strdir1.Format("%s\\data\\shenzhen\\f10",strdir);
	::SetCurrentDirectory(strdir1);
	if(fff.FindFile("*.*")!=0)
	{
		while(fff.FindNextFile())
		{
			if(!(fff.IsDots()||fff.IsDirectory()))
			{
				CString strFileName=fff.GetFileName();
				int nL=strFileName.Find(".");
				CString str1=strFileName.Left(nL);
				if(str1.GetLength()==4)
				{
					CString strExt=strFileName.Right(4);
					CString str2=CSharesInformation::Symbol4To6(str1);
					CString strNewFileName=str2+strExt;
					if(_access(strNewFileName,0)==-1){
					try{
					CFile::Rename(strFileName,strNewFileName);
					}
					catch(...)
					{
					}
					}
				}
			}
		}
	}
	::SetCurrentDirectory(strdir);
	
	return true;
}

int CTaiKlineFileKLine::GetKlineCount(CString symbol, int stkKind,bool bDayKline)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol,stkKind,bDayKline);//CTaiKlineFileKLine::m_fileDaySh ;
	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	pFile->GetKlineSmallHeader (symbol,pKlineSmallHead);
	return klineSmallHead.numKln ;
}

void CTaiKlineFileKLine::ChooseReceiveDataForm()
{
	GetSetReg reg;
	char ch[512];
	int nWhich=1;
	memset(ch,0,512);
	DWORD ncb = sizeof(ch);
	DWORD typ = REG_SZ;
	if(reg.GetValue("software\\WsStock\\ReceiveDataForm","NotShow",typ,(unsigned char *)ch,ncb) == ERROR_SUCCESS)
	{
		CString strShow = ch;
		if(strShow=="OK")
			return;
		else
		{
			char mych[512];
			memset(mych,0,512);
			ncb = sizeof(mych);
			if(reg.GetValue("software\\WsStock\\ReceiveDataForm","SelectForm",typ,(unsigned char *)mych,ncb) == ERROR_SUCCESS)
			{
				CString s = mych;
				if(s=="OLD")
					nWhich=0;
				else if(s=="NEW")
					nWhich=1;
				else
					nWhich=2;
			}
			CChooseReceiveDataForm mydlg;
			mydlg.nWhich=nWhich;
			mydlg.DoModal();
		}
	}
	else
	{
		CChooseReceiveDataForm mydlg;
		mydlg.nWhich=0;
		mydlg.DoModal();
	}
}

bool CTaiKlineFileKLine::ConvertBaseInfoFile()
{
	if(_access("baseinfo.dat",0)==-1)
		return false;
	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc ;
	BASEINFOHEAD *FileHead=new BASEINFOHEAD;
	CFile f;
	f.Open("baseinfo.dat",CFile::modeReadWrite|CFile::typeBinary);
	f.Read(FileHead,sizeof(BASEINFOHEAD));
	BASEINFO *pRecordSet=new BASEINFO[FileHead->StockCount];
	BASEINFO_OLD *pRecordSetOLD=new BASEINFO_OLD;
	for(int k=0;k<FileHead->StockCount;k++)
	{
		DWORD nKind;
		CString strSymbol;
		CString str6;		
		f.Read(pRecordSetOLD,sizeof(BASEINFO_OLD));
		strSymbol=pRecordSetOLD->Symbol;
		if(strSymbol.GetLength()==6)
		{
			nKind=CSharesInformation::GetStockKind(SH_MARKET_EX,strSymbol.GetBuffer(0));
			str6=strSymbol;
		}
		else if(strSymbol.GetLength()==4)
		{
			str6=CSharesInformation::Symbol4To6(strSymbol);
			nKind=CSharesInformation::GetStockKind(SZ_MARKET_EX,str6.GetBuffer(0));
		}
		else
		{
			f.Close();
			delete pRecordSetOLD;
			delete FileHead;
			delete []pRecordSet;
			return false;
		}
		CString myNeed=pDoc->GetStockKindString(nKind);
		myNeed+=str6;
		strcpy(pRecordSet[k].Symbol,myNeed.GetBuffer(0));
		memcpy(&(pRecordSet[k].NumSplit),&(pRecordSetOLD->NumSplit),sizeof(BASEINFO_OLD)-8);
		
	}	
	FileHead->FileExitDone=88888888;
	f.SeekToBegin();
	f.Write(FileHead,sizeof(BASEINFOHEAD));
	f.Write(pRecordSet,sizeof(BASEINFO)*(FileHead->StockCount));	
	f.Close();
	delete pRecordSetOLD;
	delete FileHead;
	delete []pRecordSet;
	return true;
}

bool CTaiKlineFileKLine::ConvertStockTypeInfo()
{
	if(_access("StockTypeInfo.dat",0)==-1)
		return false;
	CTaiShanDoc * pDoc = CMainFrame::m_taiShanDoc ;
	CFile ff;


	ff.Open("StockTypeInfo.dat",CFile::modeReadWrite|CFile::typeBinary);
	STOCKTYPEHEAD *pStockTypeHead=new STOCKTYPEHEAD;
	try{
	ff.Read(pStockTypeHead,sizeof(STOCKTYPEHEAD));
	}
	catch(CFileException e)
	{
		e.ReportError();
	}
	int nMaxType=pStockTypeHead->m_lStockTypeMaxCount;
	int nCurrentStock=pStockTypeHead->m_lStockCount;
	STOCK_TYPE_INFO *pStockInfo=new STOCK_TYPE_INFO[pStockTypeHead->m_lStockCount+1000];
	ff.Seek(sizeof(STOCKTYPEHEAD)+sizeof(STOCKTYPEINFO)*nMaxType,CFile::begin);
	STOCK_TYPE_INFO_OLD *pMyNeed=new STOCK_TYPE_INFO_OLD;

	for(int kk=0;kk<nCurrentStock;kk++)
	{		
		DWORD nKind;
		CString str6;
		ff.Read(pMyNeed,sizeof(STOCK_TYPE_INFO_OLD));
		CString strSymbol=pMyNeed->m_szSymbol;

		if(strSymbol.GetLength()==6)
		{
			nKind=CSharesInformation::GetStockKind(SH_MARKET_EX,strSymbol.GetBuffer(0));
			str6=strSymbol;
		}
		else if (strSymbol.GetLength()==4)
		{
			str6=CSharesInformation::Symbol4To6(strSymbol);
			nKind=CSharesInformation::GetStockKind(SZ_MARKET_EX,str6.GetBuffer(0));
		}
		else
		{
			ff.Close();
			delete pStockTypeHead;
			delete pMyNeed;
			delete []pStockInfo;
			return false;
		}
		CString myNeed=pDoc->GetStockKindString(nKind);
		myNeed+=str6;
		strcpy(pStockInfo[kk].m_szSymbol,myNeed.GetBuffer(0));
		memcpy(&(pStockInfo[kk].m_bDeleted),&(pMyNeed->m_bDeleted),sizeof(STOCK_TYPE_INFO_OLD)-7);
	}	
	pStockTypeHead->m_lFileExit =88888888;
	ff.SeekToBegin();
	ff.Write(pStockTypeHead,sizeof(STOCKTYPEHEAD));
	ff.Seek(sizeof(STOCKTYPEHEAD)+sizeof(STOCKTYPEINFO)*nMaxType,CFile::begin);
	ff.Write(pStockInfo,sizeof(STOCK_TYPE_INFO)*nCurrentStock);
	ff.Close();

	delete pStockTypeHead;
	delete pMyNeed;
	delete []pStockInfo;
	return true;
}
