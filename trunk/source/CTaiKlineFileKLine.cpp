
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineFileKLine.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineTransferKline.h"

#include "StkDatabase.h"


#define TSK_FILEHEADER_SIZE		sizeof(TSK_FILEHEADER)
#define KlineByteEach			sizeof(Kline)
#define FixedKlinePerBlock		240
#define KLINESMALLHEAD			TSK_FILEHEADER_SIZE + sizeof(TSK_KLINEINDEX) * MAX_STOCK_NUM


CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileDaySh = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileDaySz = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileMin5Sh = NULL;
CTaiKlineFileKLine*	CTaiKlineFileKLine::m_fileMin5Sz = NULL;


CTaiKlineFileKLine::CTaiKlineFileKLine()
{
	m_kindKline = DAY_KLINE;
	m_nMarket = SZ_MARKET_EX;
	m_bFirstWrite = TRUE;
	m_bIndex = FALSE;
}

CTaiKlineFileKLine::CTaiKlineFileKLine(int nKind, int nMarket)
{
	m_kindKline = nKind;
	m_nMarket = nMarket;
	m_bFirstWrite = TRUE;
	m_bIndex = FALSE;

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
int CTaiKlineFileKLine::TimeToFoot(TSK_KLINEINDEX* pKlineIndex,CTime &tm,BOOL& bAdd)//according the time ,to compute the foot of kline in file.
{
	int nCount = pKlineIndex->klineCount ;
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
		int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[i] 
				* KlineByteEach *  FixedKlinePerBlock
					 ;
									 
		this->Seek(nAddr,this->begin);
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
	
		ASSERT(pKlineIndex->symBlock[i]!=0xffff);
		int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[i] 
				* KlineByteEach *  FixedKlinePerBlock
					 ;
									  
		Seek(nAddr,begin);
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
		
			int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[0] 
					* KlineByteEach *  FixedKlinePerBlock
						 ;
										  
			Seek(nAddr,begin);
			Kline *pKline = (Kline *)this->GetFileCurrentPointer ();

			int nMax;

			if(nCount>=FixedKlinePerBlock) nMax = FixedKlinePerBlock;
			else nMax =nCount;
			//end

			rtn = LookTwoPath(tmt ,pKline,  nMax, (bool&)bAdd,(m_kindKline+1)%2);
			if(rtn ==  -1&&bAdd == true) rtn = nMax;

		}

	}


	if(pKlineIndex->klineCount<=rtn)
	{
		rtn =-1;
		bAdd = true;
	}
	
	return rtn;
}

int CTaiKlineFileKLine::ReadKlinePeriod(CString symbol, Kline *&kline, CTime& timeStart, CTime& timeEnd,bool istotaldata,int nAddBlank )//读取一段时间的K线数据
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;

	std::string code(symbol);
	int nIndexStock = GetKlineIndex(code,pKlineIndex);

	if(kline != NULL)
		delete [] kline ;
	BOOL bAdd;

	int nFootB = TimeToFoot(pKlineIndex,timeStart, bAdd);
	if(nFootB ==  -1&&bAdd == true)
	{

		kline = new Kline[1+nAddBlank];
		return 0;
	}
	int nFootE = TimeToFoot(pKlineIndex,timeEnd, bAdd);
	if(nFootE ==  -1&&bAdd == true)
	{

		nFootE = pKlineIndex ->klineCount -1;
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
		int blockIndex= i / FixedKlinePerBlock  ;	 
		int stockIndex=i % FixedKlinePerBlock  ;	  
		int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[blockIndex] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockIndex * KlineByteEach;
									
		this->Seek(nAddr,this->begin);
		Read(kline+i-nFootB,KlineByteEach);
	}
	if(!CTaiShanKlineShowView::IsIndexStock3(symbol))
		TestKlineVolpositive(kline, nCount);
	return nCount;


}

int CTaiKlineFileKLine::CreateOrMoveSmallBlock(TSK_KLINEINDEX *pKlineIndex,int& nBlock)//to create a new block,or move

{

	int rtn = 0;
	int nMax = CalcMaxCountBlock();
	if(nBlock>=nMax)
	{
		WORD first = pKlineIndex->symBlock[0];
		for(int j = 0;j<nMax-1;j++)
		{
			pKlineIndex->symBlock[j] = pKlineIndex->symBlock[j+1];
		}
		pKlineIndex->symBlock[nMax-1] = first;


		nBlock = nMax-1;
		ASSERT(pKlineIndex->symBlock[nBlock]!=0xffff);
		if(nBlock<0)
		{
			ASSERT(FALSE);
			nBlock=0;
		}
		rtn = FixedKlinePerBlock;
		return rtn;
	}

	if(pKlineIndex->symBlock[nBlock]==0xffff)
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
		pKlineIndex->symBlock[nBlock]=(WORD)nCountBlock;
		nCountBlock++;
	}


	if(m_bFirstWrite == true)
	{
		CMap<int ,int ,int,int> map;
		
		int nStock = GetStockNumber ();
		for(int i=0;i<nStock;i++)
		{
			int nAddr = TSK_FILEHEADER_SIZE+i*64;
			Seek(nAddr,CTaiKlineFileKLine::begin);
			TSK_KLINEINDEX klineIndex ;
			TSK_KLINEINDEX* pKlineIndex = &klineIndex;
			Read(pKlineIndex,sizeof(TSK_KLINEINDEX));
			CString symbol (pKlineIndex->stockCode);
			for(int j=0;j<26;j++)
			{
				if(pKlineIndex->symBlock [j] == 0xffff) break;
				int iData = -1;
				int iIn = pKlineIndex->symBlock [j] ;
				if(iIn == 3844 || iIn == 3843)
					iIn = iIn;
				if(map.Lookup(iIn,iData))
				{

					int nBolck = j;
					ASSERT(j>=0&&j<26);
					if(pKlineIndex->klineCount >(j)*FixedKlinePerBlock)
						pKlineIndex->klineCount = (j)*FixedKlinePerBlock;
					for(int k=j;k<26;k++)
						pKlineIndex->symBlock [k] = 0xffff;

					this->SetKlineIndex (i,pKlineIndex);
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

	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;

	std::string code(symbol);
	int nIndexStock = GetKlineIndex(code,pKlineIndex);

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

	
	for(int i=pKlineIndex->klineCount-1;i>=0 ;i--)
	{
		if(k>=nCount)
			break;
		int blockIndex= i / FixedKlinePerBlock  ;	  
		int stockIndex=i % FixedKlinePerBlock  ;	  
		int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[blockIndex] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockIndex * KlineByteEach;
									  
		this->Seek(nAddr,this->begin);
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
		TSK_KLINEINDEX klineIndex;
		CArray<TSK_KLINEINDEX,TSK_KLINEINDEX> arrayHead;
		CArray<TSK_KLINEINDEX,TSK_KLINEINDEX> arrayHeadAll;
		int nStock = pFile->GetStockNumber ();
		int nDelete[8192];
		for(i=0;i<nStock;i++)
		{
			int nAddr = TSK_FILEHEADER_SIZE;
			pFile->Seek(nAddr+i*64,pFile->begin);
			TSK_KLINEINDEX* pKlineSmallHead3 = (TSK_KLINEINDEX*)pFile->GetFileCurrentPointer();
			klineIndex = * pKlineSmallHead3;
			arrayHeadAll.Add (klineIndex);
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
				std::string code(symbol);
				int k;
				pFile ->LookupIndex( code,k);
				if(k==-1)
					continue;
				k = pFile ->GetKlineIndex( code,&klineIndex);
				if(k<0) k = 4096;
				nDelete[k] = 1;
				if(strlen(klineIndex.stockCode )!=4&&strlen(klineIndex.stockCode )!=6)
					ASSERT(FALSE);
				arrayHead.Add (klineIndex);
			}
		}

		
		//pFile ->m_pSymbolToPos->RemoveAll();
		k = arrayHead.GetSize();
		pFile ->SetStockNumber(k);
		for(int j=0;j<k;j++)
		{
			int nAddr = TSK_FILEHEADER_SIZE+j*64;
			pFile ->Seek(nAddr,pFile ->begin);
			pFile ->Write (&arrayHead[j],sizeof(TSK_KLINEINDEX));
			CString s2(arrayHead[j].stockCode );
			//pFile ->m_pSymbolToPos->SetAt(s2, (void*)j);
		}
		for(int j = 0;j<nStock;j++)
		{
			if(nDelete[j] == 0)
			{
				int nAddr = TSK_FILEHEADER_SIZE+k*64;
				pFile ->Seek(nAddr,pFile ->begin);
				pFile ->Write (&arrayHeadAll[j],sizeof(TSK_KLINEINDEX));
				CString s2(arrayHeadAll[j].stockCode );

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
		{
			std::string code(symbol);
			n = pFile->ReadKLine(code,pKline,nRead,nAdd)	;
		}
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


bool CTaiKlineFileKLine::IsDayKline(int nKlineType)
{
	return nKlineType>=5 && nKlineType<= 8
		||(nKlineType==MANY_DAY_K_LINE&&CMainFrame::m_taiShanDoc ->m_propertyInitiate.daysOfManyKline>0);

}

void CTaiKlineFileKLine::TestKlineVolpositive(Kline* pKline, int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		if (pKline[i].volPositive > pKline[i].vol || pKline[i].volPositive < 1 && pKline[i].volPositive != 0)
		{
			pKline[i].volPositive = pKline[i].vol / 2;
		}
	}
}

void CTaiKlineFileKLine::AddSmallHeadBlock()
{
	ASSERT(FALSE);

	int n = (MaxNumStock - 4096)/(240/2);
	if(n<0) return;


	int nStock = GetStockNumber ();
	TSK_KLINEINDEX klineIndex;
	int j = 0;
	int i = 0;
	bool bFound = false;
	for(i=0;i<nStock;i++)
	{
		int nAddr = TSK_FILEHEADER_SIZE;
		Seek(nAddr+i*64,begin);
		Read(&klineIndex,sizeof(TSK_KLINEINDEX));
		for(j = 0;j<26;j++)
		{
			if(klineIndex.symBlock [j] == n)
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
			int blockIndex= j ;
			int stockIndex=k  ;
			int nAddr = KLINESMALLHEAD + klineIndex.symBlock[blockIndex]
			* KlineByteEach *  FixedKlinePerBlock
				+ stockIndex * KlineByteEach;

			this->Seek(nAddr,this->begin);
			Read(&kline,KlineByteEach);
			nAddr = KLINESMALLHEAD + klineIndex.symBlock[nNewBlock]
			* KlineByteEach *  FixedKlinePerBlock
				+ stockIndex * KlineByteEach;
			this->Seek(nAddr,this->begin);
			Write(&kline,KlineByteEach);
		}


		klineIndex.symBlock [j] = (WORD)nNewBlock;
		this->SetKlineIndex(i, &klineIndex);


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

int CTaiKlineFileKLine::GetFirstKline(CString sSymbol,int stkKind,bool bDay,int nFoot ,float * pClose)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(sSymbol,stkKind,bDay);

	std::string code(sSymbol);
	TSK_KLINEINDEX smallHead;
	pFile->GetKlineIndex (code,&smallHead);
	if(smallHead.klineCount <=0)
	{
		if(nFoot!=-1) *pClose = -1;
		return -1;
	}

	Kline kln;
	if(nFoot !=-1)
	{
		if(smallHead.klineCount <=nFoot)
		{
			*pClose = -1;
			return -1;
		}
		int blockIndex= nFoot / FixedKlinePerBlock  ;	 
		int stockIndex=nFoot % FixedKlinePerBlock  ;	  
		int nAddr = KLINESMALLHEAD + smallHead.symBlock[blockIndex] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockIndex * KlineByteEach;
									  
		pFile->Seek(nAddr,pFile->begin);
		pFile->Read(&kln,KlineByteEach);
		*pClose = kln.close ;
	}
	else
	{
		int nAddr = KLINESMALLHEAD + smallHead.symBlock[0] 
				* KlineByteEach *  FixedKlinePerBlock;
									  
		pFile->Seek(nAddr,pFile->begin);
		pFile->Read(&kln,KlineByteEach);
	}

	return kln.day ;
}

int CTaiKlineFileKLine::GetKlineCount(CString symbol, int stkKind, BOOL bDayKline)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol, stkKind, bDayKline);
	if (!pFile)
	{
		return -1;
	}

	std::string code(symbol);
	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;
	pFile->GetKlineIndex(code, pKlineIndex);

	return klineIndex.klineCount;
}

float CTaiKlineFileKLine::GetLastClose(CString symbol, int stkKind)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol, stkKind, TRUE);
	if (!pFile)
	{
		return -1;
	}

	TSK_KLINEINDEX smallHead;
	std::string code(symbol);
	pFile->GetKlineIndex(code, &smallHead);
	if (smallHead.klineCount <= 0)
	{
		return -1;
	}

	float fClose = -1;

	Kline* pKline = NULL;
	int nRead = pFile->ReadKLine(code, pKline, 1, 0);
	if (nRead > 0)
	{
		if (pKline != NULL)
		{
			fClose = pKline->close;
		}
	}

	if (pKline != NULL) delete []pKline;

	return fClose;
}

int CTaiKlineFileKLine::ReadKLineS(CString symbol, int stkKind, Kline*& pKline, int nRead, int nAddBlank, BOOL bDay)
{
	int nCount = 0;

	CTaiKlineFileKLine* pFile = GetFilePointer(symbol, stkKind, bDay);
	if (pFile)
	{
		std::string code(symbol);
		nCount = pFile->ReadKLine(code, pKline, nRead, nAddBlank);
	}

	return nCount;
}

BOOL CTaiKlineFileKLine::WriteKLineS(CString symbol, int stkKind, Kline* pKline, int nWrite, int nType, BOOL bDay)
{
	BOOL bResult = FALSE;

	CTaiKlineFileKLine* pFile = GetFilePointer(symbol, stkKind, bDay);
	if (pFile)
	{
		std::string code(symbol);
		bResult = pFile->WriteKLine(code, pKline, nWrite, nType);
	}

	return bResult;
}

void CTaiKlineFileKLine::ZeroKlineData(CString symbol, int stkKind, BOOL bDay)
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(symbol, stkKind, bDay);
	TSK_KLINEINDEX smallHead;
	std::string code(symbol);
	int nIndex = pFile->GetKlineIndex(code, &smallHead);
	smallHead.klineCount = 0;
	pFile->SetKlineIndex(nIndex, &smallHead);
}

BOOL CTaiKlineFileKLine::Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd, CFileException* pException)
{
	BOOL bOk = TRUE;

	bOk = CStkFile::Open(lpszFileName, nOpenFlags, 0, pException);
	if (bOk == FALSE)
	{
		return bOk;
	}

	m_pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();

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
						int nAddr = TSK_FILEHEADER_SIZE + i * 64;
						Seek(nAddr, CTaiKlineFileKLine::begin);
						TSK_KLINEINDEX klineIndex;
						TSK_KLINEINDEX* pKlineIndex = &klineIndex;
						Read(pKlineIndex, sizeof(TSK_KLINEINDEX));
						CString symbol(pKlineIndex->stockCode);
						for (int j = 0; j < 26; j++)
						{
							if (pKlineIndex->symBlock[j] == 0xFFFF) break;
							int iData = -1;
							int iIn = pKlineIndex->symBlock[j];
							if (iIn >= nLen)
							{
								pKlineIndex->klineCount = (j) * FixedKlinePerBlock;
								for ( ; j < 26; j++)
									pKlineIndex->symBlock[j] = 0xFFFF;
								SetKlineIndex(i, pKlineIndex);
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
	SeekToBegin();

	m_pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	memset(m_pHeader, 0, sizeof(TSK_FILEHEADER));
	m_pHeader->stocksum = 0;
	m_pHeader->blocksum = 0;
	m_pHeader->flag = 65798809;
	m_pHeader->w1 = 240;
	m_pHeader->stockmax = 4096;
	Write(m_pHeader, sizeof(TSK_FILEHEADER));


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

int CTaiKlineFileKLine::AddNewStockToFile(std::string symbol, TSK_KLINEINDEX*& pKlineIndex)
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

	//if (!(symbol.GetLength() == 4 || symbol.GetLength() == 6))
	//{
	//	symbol = symbol.Left(nLen);
	//}
	//symbol = CheckSymbolValid(symbol);

	int nAddr = TSK_FILEHEADER_SIZE + nStock * 64;
	Seek(nAddr, begin);

	int rValue = 0;
	MapIndex::const_iterator stock = m_mapIndex.find(symbol);
	if (stock != m_mapIndex.end())
	{
		rValue = stock->second;
		nAddr = TSK_FILEHEADER_SIZE + (int)(rValue) * 64;
		Seek(nAddr, begin);
		pKlineIndex = (TSK_KLINEINDEX*)GetFileCurrentPointer();
		return rValue;
	}

	Write(symbol.c_str(), 8);

	int nKline = 0;
	Write(&nKline, 4);

	Seek(nAddr, begin);
	pKlineIndex = (TSK_KLINEINDEX*)GetFileCurrentPointer();

	m_mapIndex[symbol] = nStock;
	nStock++;
	SetStockNumber(nStock);

	return nStock - 1;
}

BOOL CTaiKlineFileKLine::WriteKlinePeriod(CString symbol, Kline* pKline, int nKline, CTime& timeStart, CTime& timeEnd)
{
	return TRUE;
}

BOOL CTaiKlineFileKLine::WriteKLineToRepair(CString code, Kline* pKline, int nWrite)
{
	ASSERT(nWrite > 0);
	if (nWrite <= 0)
	{
		return FALSE;
	}

	std::string symbol(code);

	BOOL bAdd;
	CTime timeStart((time_t)pKline[0].day);

	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;
	int nIndexStock = GetKlineIndex(symbol, pKlineIndex);

	int nFootB = TimeToFoot(pKlineIndex, timeStart, bAdd);
	if (nFootB != -1 && bAdd == FALSE)
	{
		timeStart = CTime((time_t)pKline[nWrite - 1].day);
		int nFootE = TimeToFoot(pKlineIndex, timeStart, bAdd);
		if (nFootE != -1 && bAdd == FALSE)
		{
			if (nFootE - nFootB + 1 == nWrite)
				return FALSE;
		}
	}

	WriteKLine(symbol, pKline, nWrite, 0);

	return TRUE;
}



WORD CTaiKlineFileKLine::GetMaxNumStock()
{
	if (!IsOpen() && !m_pHeader)
	{
		return -1;
	}

	TSK_FILEHEADER* pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	return pHeader->stockmax;
}

void CTaiKlineFileKLine::SetMaxNumStock(WORD wStock)
{
	if (!IsOpen() && !m_pHeader && wStock > 8096)
	{
		return;
	}

	TSK_FILEHEADER* pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	pHeader->stockmax = wStock;
	SeekToBegin();
	Write(pHeader, sizeof(TSK_FILEHEADER));
}

int CTaiKlineFileKLine::GetStockNumber()
{
	if (!IsOpen() && !m_pHeader)
	{
		return -1;
	}

	TSK_FILEHEADER* pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	return pHeader->stocksum;
};

void CTaiKlineFileKLine::SetStockNumber(int nStock)
{
	if (!IsOpen() && !m_pHeader && nStock > 8096)
	{
		return;
	}

	TSK_FILEHEADER* pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	pHeader->stocksum = nStock;
	SeekToBegin();
	Write(pHeader, sizeof(TSK_FILEHEADER));
};

int CTaiKlineFileKLine::GetSmallBlockCount()
{
	if (!IsOpen() && !m_pHeader)
	{
		return -1;
	}

	TSK_FILEHEADER* pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	return pHeader->blocksum;
}

void CTaiKlineFileKLine::SetSmallBlockCount(int nBlock)
{
	if (!IsOpen() && !m_pHeader)
	{
		return;
	}

	TSK_FILEHEADER* pHeader = (TSK_FILEHEADER*)GetFileBeginPointer();
	pHeader->blocksum = nBlock;
	SeekToBegin();
	Write(pHeader, sizeof(TSK_FILEHEADER));
}

/* ============================================================================
int nKind, int nMarket 数据类型 日线、五分、一分 市场类型 上海、深圳 枚举

*/

void CTaiKlineFileKLine::AddIndexToMap()
{
	int nStock = GetStockNumber();
	if (nStock <= 0 || nStock > GetMaxNumStock() || m_bIndex)
		return;

	for (int iIndex = 0; iIndex < nStock; iIndex++)
	{
		int nAddr = TSK_FILEHEADER_SIZE + TSK_KLINEINDEX_SIZE * iIndex;
		Seek(nAddr, begin);
		TSK_KLINEINDEX* pKlineIndex = (TSK_KLINEINDEX*)GetFileCurrentPointer();

		std::string symbol(pKlineIndex->stockCode);
		m_mapIndex[symbol] = iIndex;
	}

	m_bIndex = TRUE;
}

void CTaiKlineFileKLine::DeleteMap()
{
	if (m_bIndex && m_mapIndex.size() > 0)
	{
		m_mapIndex.clear();
	}
}

void CTaiKlineFileKLine::LookupIndex(std::string symbol, int& nIndex)
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

int CTaiKlineFileKLine::GetKlineIndex(std::string symbol, TSK_KLINEINDEX* pKlineIndex)
{
	if (!m_bIndex)
	{
		AddIndexToMap();
	}

	TSK_KLINEINDEX* pIndex = NULL;
	int iIndex = -1;

	LookupIndex(symbol, iIndex);
	if (iIndex == -1)
	{
		iIndex = AddNewStockToFile(symbol, pIndex);
		if (iIndex < 0)
			return -1;
	}
	else
	{
		int nAddr = TSK_FILEHEADER_SIZE;
		Seek(nAddr + TSK_KLINEINDEX_SIZE * iIndex, begin);
		pIndex = (TSK_KLINEINDEX*)GetFileCurrentPointer();
	}

	memcpy(pKlineIndex, pIndex, sizeof(TSK_KLINEINDEX));

	return iIndex;
}

BOOL CTaiKlineFileKLine::SetKlineIndex(int nIndex, TSK_KLINEINDEX* pKlineIndex)
{
	int nStock = GetStockNumber();
	if (nIndex >= nStock)
		return FALSE;

	int nAddr = TSK_FILEHEADER_SIZE + TSK_KLINEINDEX_SIZE * nIndex;
	Seek(nAddr, begin);

	Write(pKlineIndex, sizeof(TSK_KLINEINDEX));

	return TRUE;
}

CString CTaiKlineFileKLine::GetSymbol(int nIndex)
{
	int nAddr = TSK_FILEHEADER_SIZE;
	Seek(nAddr + nIndex * TSK_KLINEINDEX_SIZE, begin);
	TSK_KLINEINDEX* pKlineIndex = (TSK_KLINEINDEX*)GetFileCurrentPointer();
	CString symbol(pKlineIndex->stockCode);

	return symbol;
}

int CTaiKlineFileKLine::ReadKLine(int nIndex, Kline*& pKline, int nRead)
{
	CString symbol = GetSymbol(nIndex);
	std::string code(symbol);
	return ReadKLine(code, pKline, nRead, 0);
}

int CTaiKlineFileKLine::ReadKLine(std::string symbol, Kline*& pKline, int nRead, int nAddBlank)
{
	//if (symbol.GetLength() != 6 && symbol.GetLength() != 4) return 0;

	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;
	int nIndexStock = GetKlineIndex(symbol, pKlineIndex);

	int nCount = nRead;
	if (nCount <= -1 || nCount > pKlineIndex->klineCount)
		nCount = pKlineIndex->klineCount;
	if (pKline != NULL)
		delete []pKline;

	ASSERT(nAddBlank >= 0);
	pKline = new Kline[nCount + nAddBlank + 1];
	if (nCount == 0)
		return 0;

	//SeekToBegin();
	//CString ss = pKlineIndex->stockCode;
	ASSERT(nCount <= 0 ? TRUE : KLINESMALLHEAD + pKlineIndex->symBlock[(nCount - 1) / FixedKlinePerBlock] * KlineByteEach * FixedKlinePerBlock < GetLength());

	for (int i = pKlineIndex->klineCount - nCount; i < pKlineIndex->klineCount; i++)
	{
		int blockIndex = i / FixedKlinePerBlock;
		int stockIndex = i % FixedKlinePerBlock;
		int nAddr = KLINESMALLHEAD + KlineByteEach * FixedKlinePerBlock * pKlineIndex->symBlock[blockIndex] + KlineByteEach * stockIndex;

		Seek(nAddr, begin);
		Read(pKline + i - pKlineIndex->klineCount + nCount, KlineByteEach);
	}

	//if (!CTaiShanKlineShowView::IsIndexStock3(symbol))
	//	TestKlineVolpositive(pKline, nCount);

	return nCount;
}

BOOL CTaiKlineFileKLine::WriteKLine(std::string symbol,Kline* pKline,int nWrite,int nType)	
{
	//if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return false;

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
	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;
	int nIndexStock = GetKlineIndex(symbol,pKlineIndex);
	int nMaxBlock = CalcMaxCountBlock();
	if(pKlineIndex->klineCount > nMaxBlock*FixedKlinePerBlock)
	{
		int n1 = pKlineIndex->klineCount/FixedKlinePerBlock;
		int n2 = pKlineIndex->klineCount%FixedKlinePerBlock == 0 ? 0:1;
		int nBlockCur = n1 + n2;
		int nMoveBlock = nBlockCur - nMaxBlock ;
		if(nMoveBlock>0)
		{
			for(int i=0;i<nMoveBlock;i++)
			{
				WORD wdVal = pKlineIndex->symBlock [0];
				for(int j=0;j<nBlockCur-1;j++)
				{
					pKlineIndex->symBlock [j] = pKlineIndex->symBlock [j+1];
					ASSERT(pKlineIndex->symBlock [j+1]!=0xffff);
				}
				if(nBlockCur-1>=0)
					pKlineIndex->symBlock [nBlockCur-1] = wdVal;
				else
					ASSERT(FALSE);
	
			}
			pKlineIndex->klineCount = pKlineIndex->klineCount - nMoveBlock*FixedKlinePerBlock;
		}
		else
		{
			pKlineIndex->klineCount = 0;
			ASSERT(FALSE);
		}
		this->SetKlineIndex (nIndexStock,pKlineIndex);
	}

	ASSERT(pKlineIndex!=NULL);

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

	if(pKlineIndex->klineCount <=0)
		pKlineIndex->klineCount =0;


	CTime timeStart((time_t)pKline[0].day );
	BOOL bAdd;
	int nFootB = TimeToFoot(pKlineIndex,timeStart, bAdd);
	if(nFootB == -1 )
	{
		if(bAdd ==true)
			nFootB = pKlineIndex->klineCount ;
		else
			ASSERT(FALSE);
	}
	timeStart = CTime((time_t)pKline[nWrite-1].day );
	int nFootE = TimeToFoot(pKlineIndex,timeStart, bAdd);
	if(nFootE == -1 )
	{
		if(bAdd ==true)
			nFootE = pKlineIndex->klineCount ;
		else
			ASSERT(FALSE);
	}
	else if(bAdd == false)
	{
		if(nFootE <pKlineIndex->klineCount)
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
			nRtn = ReadKLine( symbol,pKlineFile,pKlineIndex->klineCount - nFootE,1);
			ASSERT(nRtn == pKlineIndex->klineCount - nFootE);
		}

	
		nWrite2 = pKlineIndex->klineCount-nFootE;
		if(nWrite2<0 || nMove==0)
			nWrite2 =0;

	}
	else
	{
		
		nWrite2 = pKlineIndex->klineCount-nFootE;
		if(nWrite2<0)
			nWrite2 =0;
		int i ;
		Kline kln;
		if(nWrite2!=0)
		{
			for( i=nFootB;i<nFootE;i++)
			{
				int blockIndex= i / FixedKlinePerBlock  ;	  
				int stockIndex=i % FixedKlinePerBlock  ;	 
				int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[blockIndex] 
						* KlineByteEach *  FixedKlinePerBlock
							 + stockIndex * KlineByteEach;
											 
				this->Seek(nAddr,this->begin);
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
			int nRtn = ReadKLine( symbol,pKlineFile,pKlineIndex->klineCount - nFootB,1);
			ASSERT(nRtn == pKlineIndex->klineCount - nFootB);
		}

		pKlineMid = klineArray.GetData ();
	}
	int nDeleted = 0;
	int blockIndex= nFootB / FixedKlinePerBlock  ;	  
	for(int i=nFootB;i<nFootB+ nWrite + nWrite2;i++)
	{
		int stockIndex=i % FixedKlinePerBlock  ;	 


		if(stockIndex == 0)
		{
			blockIndex= i / FixedKlinePerBlock  ;	 
			nDeleted += CreateOrMoveSmallBlock(pKlineIndex, blockIndex);
		}

		int nAddr = KLINESMALLHEAD + pKlineIndex->symBlock[blockIndex] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockIndex * KlineByteEach;
									  
		this->Seek(nAddr,this->begin);

		Kline * pKln = pKlineMid+i-nFootB;
		if(i>=nFootB+ nWrite)//from file old
			pKln = pKlineFile+i-nFootB-nWrite;
		//if(!CTaiShanKlineShowView::IsIndexStock3(symbol))
		//	TestKlineVolpositive(pKln, 1);
		Write(pKln,KlineByteEach);

	}
	
	pKlineIndex->klineCount = nFootB+ nWrite + pKlineIndex->klineCount-nFootE - nDeleted;
	ASSERT(pKlineIndex->klineCount>=0);
	if(pKlineFile != NULL)
		delete [] pKlineFile;

	SetKlineIndex(nIndexStock,pKlineIndex);
	return true;

}

void CTaiKlineFileKLine::DeleteKlineData(std::string symbol, int nFoot, int nCount)
{
	Kline* pKline = NULL;

	int nKline = ReadKLine(symbol, pKline, -1, 0);
	if (nKline <= 0)
		return;

	if (nKline > nFoot + 1)
	{
		memmove(pKline + nFoot, pKline + nFoot + 1, KlineByteEach * (nKline - nFoot - 1));
	}

	nKline--;

	TSK_KLINEINDEX klineIndex;
	TSK_KLINEINDEX* pKlineIndex = &klineIndex;
	int nIndexStock = GetKlineIndex(symbol, pKlineIndex);
	pKlineIndex->klineCount = 0;
	SetKlineIndex(nIndexStock, pKlineIndex);

	if (nKline > 0)
	{
		WriteKLine(symbol, pKline, nKline, 0);
	}

	if (pKline == NULL)
		delete []pKline;
}
