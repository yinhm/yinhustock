
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineFileKLine.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineTransferKline.h"

#include "StkDatabase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileDaySh = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileDaySz = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileMin5Sh = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileMin5Sz = NULL;

CTaiKlineFileKLine::CTaiKlineFileKLine()
{
	m_kindKline = DAY_KLINE;
	m_nMarket = SZ_MARKET_EX;
	m_bFirstWrite = TRUE;
}

CTaiKlineFileKLine::CTaiKlineFileKLine(int nKind, int nMarket)
{
	m_kindKline = nKind;
	m_nMarket = nMarket;
	m_bFirstWrite = TRUE;

	m_nAddReMap = KlineByteEach * FixedKlinePerBlock * 5;
}

CTaiKlineFileKLine::~CTaiKlineFileKLine()
{
	DeleteMap();
}

BOOL CTaiKlineFileKLine::OpenAll()
{
	if (!m_fileDaySh)
		m_fileDaySh = new CTaiKlineFileKLine(0, SH_MARKET_EX);
	if (!m_fileDaySz)
		m_fileDaySz = new CTaiKlineFileKLine(0, SZ_MARKET_EX);
	if (!m_fileMin5Sh)
		m_fileMin5Sh = new CTaiKlineFileKLine(1, SH_MARKET_EX);
	if (!m_fileMin5Sz)
		m_fileMin5Sz = new CTaiKlineFileKLine(1, SZ_MARKET_EX);

	try
	{

		CreateDirectory("Data\\Sh", NULL);
		CreateDirectory("Data\\Sz", NULL);
		CreateDirectory("Data\\Sh\\F10", NULL);
		CreateDirectory("Data\\Sz\\F10", NULL);
	}
	catch(...)
	{
	}

	BOOL bRtn = TRUE;

	CString sPath2 = g_daysh;
	if (!m_fileDaySh->Open(sPath2, 0, NULL))
	{
		delete m_fileDaySh;
		m_fileDaySh = NULL;
		bRtn = FALSE;
		AfxMessageBox("打开上海日线文件时出错，请关闭程序后，删除文件 data\\sh\\daykline.dat", MB_OK | MB_ICONERROR);
	}

	sPath2 = g_daysz;
	if (!m_fileDaySz->Open(sPath2, 0, NULL))
	{
		delete m_fileDaySz;
		m_fileDaySz = NULL;
		bRtn = FALSE;
		AfxMessageBox("打开深圳日线文件时出错，请关闭程序后，删除文件 data\\sz\\daykline.dat", MB_OK | MB_ICONERROR);
	}

	sPath2 = g_minutesh;
	if (!m_fileMin5Sh->Open(sPath2, 0, NULL))
	{
		delete m_fileMin5Sh;
		m_fileMin5Sh = NULL;
		bRtn = FALSE;
		AfxMessageBox("打开上海5分钟K线文件时出错", MB_OK | MB_ICONERROR);
	}

	sPath2 = g_minutesz;
	if (!m_fileMin5Sz->Open(sPath2, 0, NULL))
	{
		delete m_fileMin5Sz;
		m_fileMin5Sz = NULL;
		bRtn = FALSE;
		AfxMessageBox("打开深圳5分钟K线文件时出错", MB_OK | MB_ICONERROR);
	}

	return bRtn;
}

void CTaiKlineFileKLine::CloseAll()
{
	if (m_fileDaySh != NULL)
	{
		m_fileDaySh->Close();
	}
	if (m_fileDaySz != NULL)
	{
		m_fileDaySz->Close();
	}
	if (m_fileMin5Sh != NULL)
	{
		m_fileMin5Sh->Close();
	}
	if (m_fileMin5Sz != NULL)
	{
		m_fileMin5Sz->Close();
	}
	if (m_fileDaySh != NULL)
	{
		delete m_fileDaySh;
		m_fileDaySh = NULL;
	}
	if (m_fileDaySz != NULL)
	{
		delete m_fileDaySz;
		m_fileDaySz = NULL;
	}
	if (m_fileMin5Sh != NULL)
	{
		delete m_fileMin5Sh;
		m_fileMin5Sh = NULL;
	}
	if (m_fileMin5Sz != NULL)
	{
		delete m_fileMin5Sz;
		m_fileMin5Sz = NULL;
	}
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
int CTaiKlineFileKLine::TimeToFoot(KLINE_SMALLHEAD* pKlineSmallHead,CTime &tm,BOOL& bAdd)//according the time ,to compute the foot of kline in file.
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


		rtn = LookTwoPath(tmt ,pKline,  nMax, (bool&)bAdd,(m_kindKline+1)%2);
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

			rtn = LookTwoPath(tmt ,pKline,  nMax, (bool&)bAdd,(m_kindKline+1)%2);
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
	BOOL bAdd;

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
	BOOL bAdd;
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

int CTaiKlineFileKLine::ReadKlinePeriod(int nIndex, Kline *&kline, CTime &timeStart, CTime &timeEnd,int nAddBlank)
{
	CString symbol = GetSymbol( nIndex);
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4)
		return 0;
	return ReadKlinePeriod( symbol, kline, timeStart, timeEnd,true, nAddBlank);
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
		pFileHs=TSKDatabase()->GetTickFile (symbol,stkKind);
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


CTaiKlineFileKLine* CTaiKlineFileKLine::GetFilePointer(CString symbol, int stkKind, BOOL bDayKline)
{
	CTaiKlineFileKLine*	pFile;

	if (bDayKline == TRUE)
	{
		pFile = CTaiKlineFileKLine::m_fileDaySh;
	}
	else
	{
		pFile = CTaiKlineFileKLine::m_fileMin5Sh;
	}

	if (CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX)
	{
		if (bDayKline == TRUE)
		{
			pFile = CTaiKlineFileKLine::m_fileDaySz;
		}
		else
		{
			pFile = CTaiKlineFileKLine::m_fileMin5Sz;
		}
	}

	return pFile;
}

int CTaiKlineFileKLine::GetKlineCount(CString symbol, int stkKind, BOOL bDayKline)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol, stkKind, bDayKline);
	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	pFile->GetKlineSmallHeader(symbol, pKlineSmallHead);

	return klineSmallHead.numKln;
}

float CTaiKlineFileKLine::GetLastClose(CString symbol, int stkKind)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol, stkKind, TRUE);
	KLINE_SMALLHEAD smallHead;
	pFile->GetKlineSmallHeader(symbol, &smallHead);
	if (smallHead.numKln <= 0)
	{
		return -1;
	}

	Kline* pKline = NULL;
	float f = -1;
	int n = pFile->ReadKLine(symbol, pKline, 1, 0);
	if (n > 0)
	{
		if (pKline != NULL)
			f = pKline->close;
	}

	if (pKline != NULL) delete []pKline;

	return f;
}

int CTaiKlineFileKLine::ReadKLineS(CString symbol, int stkKind, Kline*& pKline, int nRead, int nAddBlank, BOOL bDay)
{
	int nCount = 0;
	CTaiKlineFileKLine* pFile = GetFilePointer(symbol, stkKind, bDay);
	if (pFile)
		nCount = pFile->ReadKLine(symbol, pKline, nRead, nAddBlank);

	return nCount;
}

BOOL CTaiKlineFileKLine::WriteKLineS(CString symbol, int stkKind, Kline* pKline, int nWrite, int nType, BOOL bDay)
{
	BOOL bResult = FALSE;
	CTaiKlineFileKLine* pFile = GetFilePointer(symbol, stkKind, bDay);
	if (pFile)
		bResult = pFile->WriteKLine(symbol, pKline, nWrite, nType);

	return bResult;
}

void CTaiKlineFileKLine::ZeroKlineData(CString symbol, int stkKind, BOOL bDay)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol, stkKind, bDay);
	KLINE_SMALLHEAD smallHead;
	int nIndex = pFile->GetKlineSmallHeader(symbol, &smallHead);
	smallHead.numKln = 0;
	pFile->SetKlineSmallHeader(nIndex, &smallHead);
}

BOOL CTaiKlineFileKLine::Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd, CFileException* pException)
{
	BOOL bOk = TRUE;

	bOk = CTaiKlineMemFile::Open(lpszFileName, nOpenFlags, 0, pException);
	if (bOk == FALSE)
	{
		return bOk;
	}

	DWORD dwLen = GetLength();
	if (dwLen <= KLINESMALLHEAD || bOk == FALSE)
	{
		ReMapFromBegin(KLINESMALLHEAD + 500 * KlineByteEach * FixedKlinePerBlock);
		WriteHeaderInfo();
	}
	else
	{
		int nAdd;
		switch (m_kindKline)
		{
		case DAY_KLINE:
			nAdd = 10;
			break;
		case MIN5_KLINE:
			nAdd = 100;
			break;
		}

		int bID = 1;
		if (m_kindKline == MIN5_KLINE && GetID() != 65798809)
		{
			bID = 0;
		}

		int nBlock = GetSmallBlockCount();
		if (GetStockNumber() > 4096 || nBlock > 4096 * MAX_BLOCK_USE || GetStockNumber() < 0 || nBlock < 0 || bID == 0)
		{
			WriteHeaderInfo();
		}
		else
		{
			int nLen = GetLength();
			nLen -= KLINESMALLHEAD;
			if (nLen > 0)
			{
				nLen = nLen / (KlineByteEach * FixedKlinePerBlock);
				if (nLen % (KlineByteEach * FixedKlinePerBlock) != 0)
				{
					nLen++;
				}
				if (nBlock > nLen)
				{
					ASSERT(FALSE);
					SetSmallBlockCount(nLen);

					int nStock = GetStockNumber();
					for (int i = 0; i < nStock; i++)
					{
						int addr = 16 + i * 64;
						Seek(addr, CTaiKlineFileKLine::begin);
						KLINE_SMALLHEAD klineSmallHead;
						KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
						Read(pKlineSmallHead, sizeof(KLINE_SMALLHEAD));
						CString symbol(pKlineSmallHead->StockSign);
						for (int j = 0; j < 26; j++)
						{
							if (pKlineSmallHead->symBlock[j] == 0xFFFF) break;
							int iData = -1;
							int iIn = pKlineSmallHead->symBlock[j];
							if (iIn >= nLen)
							{
								pKlineSmallHead->numKln = (j) * FixedKlinePerBlock;
								for ( ; j < 26; j++)
									pKlineSmallHead->symBlock[j] = 0xFFFF;
								SetKlineSmallHeader(i, pKlineSmallHead);
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

void CTaiKlineFileKLine::AddIndexToMap()
{
	int nStock = GetStockNumber();
	m_pSymbolToPos = new CMapStringToPtr((int)(nStock * 1.25) + 1);
	ASSERT(nStock <= MaxNumStock);
	for (int i = 0; i < nStock; i++)
	{
		int addr = 16 + i * 64;
		Seek(addr, begin);
		KLINE_SMALLHEAD* pKlineSmallHead = (KLINE_SMALLHEAD*)GetFileCurrentPointer();
		CString symbol(pKlineSmallHead->StockSign);
		m_pSymbolToPos->SetAt(symbol,(CObject*)i);
	}
}

void CTaiKlineFileKLine::DeleteMap()
{
	if (m_pSymbolToPos != NULL)
	{
		m_pSymbolToPos->RemoveAll();
		delete m_pSymbolToPos;
		m_pSymbolToPos = NULL;
	}
}

void CTaiKlineFileKLine::LookUpIndexFromMap(CString symbol, int& nIndex)
{
	nIndex = -1;
	if (m_pSymbolToPos == NULL)
		AddIndexToMap();

	if (!m_pSymbolToPos->Lookup(symbol, (void*&)nIndex))
		nIndex = -1;
}

void CTaiKlineFileKLine::WriteHeaderInfo()
{
	SeekToBegin();

	int nStock = 0;
	Write(&nStock, 4);
	Write(&nStock, 4);

	nStock = 65798809;
	Write(&nStock, 4);

	WORD wd = 240;
	Write(&wd, 2);
	wd = 4096;
	Write(&wd, 2);

	char buff[4096];
	memset(buff, 255, 4096);
	for (int i = 0; i < 64; i++)
	{
		Write(buff, 4096);
	}
}

int CTaiKlineFileKLine::CalcMaxCountBlock()
{
	int rtn = 5;
	switch (m_kindKline)
	{
	case DAY_KLINE:
		if (CMainFrame::m_taiShanDoc->m_propertyInitiate.daysOfDayKline <= 0)
		{
			rtn = MAX_BLOCK_USE;
		}
		else
		{
			rtn = CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfDayKline / FixedKlinePerBlock + 1;
			//((CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfDayKline % FixedKlinePerBlock == 0) ? 2 : 1);
		}
		break;
	case MIN5_KLINE:
		if (CMainFrame::m_taiShanDoc->m_propertyInitiate.daysOfMin5Kline <= 0)
		{
			rtn = MAX_BLOCK_USE;
		}
		else
		{
			rtn = 48 * CMainFrame::m_taiShanDoc->m_propertyInitiate.daysOfMin5Kline / FixedKlinePerBlock + 1;
			//((48 * CMainFrame::m_taiShanDoc->m_propertyInitiate.daysOfMin5Kline % FixedKlinePerBlock == 0) ? 2 : 1);
		}
		break;
	}

	if (rtn < 2) rtn = 2;
	if (rtn > MAX_BLOCK_USE) rtn = MAX_BLOCK_USE;

	return rtn;
}

int CTaiKlineFileKLine::AddNewStockToFile(CString symbol, KLINE_SMALLHEAD*& pKlineSmallHead)
{
	int nLen = 6;
	int nStock = GetStockNumber();

	if (nStock >= MaxNumStock)
	{
		BOOL bNeedMove = TRUE;
		if (MaxNumStock == 4096)
		{
			BOOL bSh = FALSE, bSz = TRUE;
			if (m_nMarket == SH_MARKET_EX)
			{
				bSh = TRUE;
				bSz = FALSE;
			}

			ReorgnizeFile(bSh, bSz, TRUE);
			ReorgnizeFile(bSh, bSz, FALSE);

			nStock = GetStockNumber();
			if (nStock < 4096) bNeedMove = FALSE;
		}

		if (MaxNumStock > 4096 || bNeedMove == TRUE)
		{
			AddSmallHeadBlock();
			nStock = GetStockNumber();
		}
	}

	if (!(symbol.GetLength() == 4 || symbol.GetLength() == 6))
	{
		symbol = symbol.Left(nLen);
	}
	symbol = CheckSymbolValid(symbol);

	int addr = 16 + nStock * 64;
	Seek(addr, begin);

	int rValue = 0;
	if (m_pSymbolToPos->Lookup(symbol, (void*&)rValue))
	{
		addr = 16 + (int)(rValue) * 64;
		Seek(addr, begin);
		pKlineSmallHead = (KLINE_SMALLHEAD*)GetFileCurrentPointer();
		return rValue;
	}

	Write(symbol.GetBuffer(8), 8);

	int nKline = 0;
	Write(&nKline, 4);

	Seek(addr, begin);
	pKlineSmallHead = (KLINE_SMALLHEAD*)GetFileCurrentPointer();

	m_pSymbolToPos->SetAt(symbol, (CObject*)nStock);
	nStock++;
	SetStockNumber(nStock);

	return nStock - 1;
}

int CTaiKlineFileKLine::GetKlineSmallHeader(CString symbol, KLINE_SMALLHEAD* pKlineSmallHead)
{
	if (m_pSymbolToPos == NULL)
		AddIndexToMap();

	KLINE_SMALLHEAD* pKlineSmallHead3 = NULL;

	int i;
	LookUpIndexFromMap(symbol, i);
	if (i == -1)
	{
		i = AddNewStockToFile(symbol, pKlineSmallHead3);
		if (i < 0)
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		int addr = 16;
		Seek(addr + i * 64, begin);
		pKlineSmallHead3 = (KLINE_SMALLHEAD*)GetFileCurrentPointer();
	}

	memcpy(pKlineSmallHead, pKlineSmallHead3, sizeof(KLINE_SMALLHEAD));
	ASSERT(pKlineSmallHead3 != NULL);

	return i;
}

BOOL CTaiKlineFileKLine::SetKlineSmallHeader(int nIndex, KLINE_SMALLHEAD* pKlineSmallHead)
{
	int nStock = GetStockNumber();
	ASSERT(nIndex < nStock);
	if (nIndex >= nStock)
		return FALSE;

	int addr = 16 + nIndex * 64;
	Seek(addr, begin);
	CString s(pKlineSmallHead->StockSign);
	KLINE_SMALLHEAD* pKlineSmallHead2 = (KLINE_SMALLHEAD*)GetFileCurrentPointer();
	CString s2(pKlineSmallHead2->StockSign);
	if (s != s2)
	{
		ASSERT(FALSE);

	}

	Write(pKlineSmallHead, sizeof(KLINE_SMALLHEAD));

	return TRUE;
}

CString CTaiKlineFileKLine::GetSymbol(int nIndex)
{
	int addr = 16;
	Seek(addr + nIndex * 64, begin);
	KLINE_SMALLHEAD* pKlineSmallHead = (KLINE_SMALLHEAD*)GetFileCurrentPointer();
	CString symbol(pKlineSmallHead->StockSign);

	return symbol;
}

int CTaiKlineFileKLine::ReadKLine(int nIndex, Kline*& pKline, int nRead)
{
	CString symbol = GetSymbol(nIndex);
	return ReadKLine(symbol, pKline, nRead, 0);
}

int CTaiKlineFileKLine::ReadKLine(CString symbol, Kline*& pKline, int nRead, int nAddBlank)
{
	if (symbol.GetLength() != 6 && symbol.GetLength() != 4) return 0;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol, pKlineSmallHead);

	int nCount = nRead;
	if (nCount <= -1 || nCount > pKlineSmallHead->numKln)
		nCount = pKlineSmallHead->numKln;
	if (pKline != NULL)
		delete []pKline;

	ASSERT(nAddBlank >= 0);
	pKline = new Kline[nCount + nAddBlank + 1];
	if (nCount == 0)
		return 0;

	//SeekToBegin();
	//CString ss = pKlineSmallHead->StockSign;
	ASSERT(nCount <= 0 ? TRUE : KLINESMALLHEAD + pKlineSmallHead->symBlock[(nCount - 1) / FixedKlinePerBlock] * KlineByteEach * FixedKlinePerBlock < GetLength());

	for (int i = pKlineSmallHead->numKln - nCount; i < pKlineSmallHead->numKln; i++)
	{
		int blkCount = i / FixedKlinePerBlock;
		int stockCount = i % FixedKlinePerBlock;
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount]	* KlineByteEach * FixedKlinePerBlock + stockCount * KlineByteEach;

		Seek(addr, begin);
		Read(pKline + i - pKlineSmallHead->numKln + nCount, KlineByteEach);
	}

	if (!CTaiShanKlineShowView::IsIndexStock3(symbol))
		TestKlineVolpositive(pKline, nCount);

	return nCount;
}

void CTaiKlineFileKLine::DeleteKlineData(CString symbol, int nFoot, int nCount)
{
	Kline* pKline = NULL;
	int n = ReadKLine(symbol, pKline, -1, 0);
	if (n <= 0) return;
	if (n > nFoot + 1) 
		memmove(pKline + nFoot, pKline + nFoot + 1, (n - nFoot - 1) * sizeof(Kline));
	n--;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol, pKlineSmallHead);
	pKlineSmallHead->numKln = 0;
	SetKlineSmallHeader(nIndexStock, pKlineSmallHead);

	if (n > 0) WriteKLine(symbol, pKline, n, 0);

	if (pKline == NULL)
		delete []pKline;
}

BOOL CTaiKlineFileKLine::WriteKlinePeriod(CString symbol, Kline* pKline, int nKline, CTime& timeStart, CTime& timeEnd)
{
	return TRUE;
}

BOOL CTaiKlineFileKLine::WriteKLineToRepair(CString symbol, Kline* pKline, int nWrite)
{
	ASSERT(nWrite > 0);
	if (nWrite <= 0)
	{
		return FALSE;
	}

	BOOL bAdd;
	CTime timeStart((time_t)pKline[0].day);

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol, pKlineSmallHead);

	int nFootB = TimeToFoot(pKlineSmallHead, timeStart, bAdd);
	if (nFootB != -1 && bAdd == FALSE)
	{
		timeStart = CTime((time_t)pKline[nWrite - 1].day);
		int nFootE = TimeToFoot(pKlineSmallHead, timeStart, bAdd);
		if (nFootE != -1 && bAdd == FALSE)
		{
			if (nFootE - nFootB + 1 == nWrite)
				return FALSE;
		}
	}

	WriteKLine(symbol, pKline, nWrite, 0);

	return TRUE;
}

/* ============================================================================
int nKind, int nMarket 数据类型 日线、五分、一分 市场类型 上海、深圳 枚举

*/
