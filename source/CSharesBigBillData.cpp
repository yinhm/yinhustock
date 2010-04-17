// StockBigBillData.cpp: implementation of the CSharesBigBillData class.
// by 俞明录
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <share.h>
#include  <io.h>
#include "CTaiShanApp.h"
#include "CSharesBigBillData.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define MAXITEMDATABUFFER  70*4+5000*40+200

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharesBigBillData::CSharesBigBillData()//读取数据文件接口定义
{
    m_hFile=NULL;
	m_hFileMap=NULL;                 
	m_pBigBillHead=NULL;                
	m_pbData=NULL;                  
	m_pMapData=NULL;         
	InitRealTimeHsData();
}

CSharesBigBillData::~CSharesBigBillData()
{
  if(m_pbData)
	 UnmapViewOfFile(m_pbData);
   if(m_hFileMap) 
	 CloseHandle(m_hFileMap);
   if(m_hFile)
	 CloseHandle(m_hFile);
}
BOOL CSharesBigBillData::InitRealTimeHsData()
{
	if(_access("RealTimeHs.dat",0)==-1)   
	{
       return InitRealTimeHsDataEmpty();
	}
    else
	{
       return InitRealTimeHsDataExist();
	}
}
BOOL CSharesBigBillData::InitRealTimeHsDataEmpty()
{
	 long l_nFileLength=sizeof(BIG_LISTHEAD)+sizeof(BIG_LIST)*5000+200 ;	//读取数据文件接口定义
	 m_hFile=CreateFile("RealTimeHs.dat",GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开实时行情换手数据文件出错");
		return FALSE; 
	}
	m_hFileMap=CreateFileMapping(m_hFile,
		NULL,
		PAGE_READWRITE,
		0,
		l_nFileLength,
		NULL);
	if(m_hFileMap==NULL)
	{
		AfxMessageBox("创立文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
	m_pbData=(PBYTE)MapViewOfFile(m_hFileMap,
		FILE_MAP_WRITE,
		0,0,0);
	if(m_pbData==NULL)
	{
		AfxMessageBox("将文件数据映射入内存时出错");
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
    m_pBigBillHead=(BIG_LISTHEAD *)m_pbData;                   
    m_pMapData=(BIG_LIST *)(m_pbData+sizeof(BIG_LISTHEAD));         

    memset(m_pBigBillHead,0,sizeof(BIG_LISTHEAD));
	m_pBigBillHead->Done=FALSE;
	m_pBigBillHead->IsGbjg=FALSE;
    m_pBigBillHead->ItemCount=0;
	m_pBigBillHead->IsSaveData=TRUE;
	m_pBigBillHead->MaxItemCount=5000;
    m_pBigBillHead->IsCalc[0]=1;
    m_pBigBillHead->IsCalc[1]=1;
    m_pBigBillHead->IsCalc[2]=1;
    m_pBigBillHead->IsCalc[3]=1;

    m_pBigBillHead->BigSet[0]=100;
    m_pBigBillHead->BigSet[1]=100;
    m_pBigBillHead->BigSet[2]=100;
    m_pBigBillHead->BigSet[3]=100;
	m_pBigBillHead->BigMaxSet[0]=5000;
	m_pBigBillHead->BigMaxSet[1]=5000;
	m_pBigBillHead->BigMaxSet[2]=5000;
	m_pBigBillHead->BigMaxSet[3]=5000;

	m_pBigBillHead->BigBillSetType[0]=10;
	m_pBigBillHead->BigBillSetType[1]=30;
	m_pBigBillHead->BigBillSetType[2]=50;
	m_pBigBillHead->BigBillSetType[3]=70;
	m_pBigBillHead->BigBillSetType[4]=90;
	m_pBigBillHead->BigBillSetType[5]=110;
	m_pBigBillHead->BigBillSetType[6]=130;
	m_pBigBillHead->BigBillSetType[7]=150;
	m_pBigBillHead->BigBillSetType[8]=170;
	m_pBigBillHead->BigBillSetType[9]=200;

	m_pBigBillHead->BigSetGB[0]=0.002f;
	m_pBigBillHead->BigSetGB[1]=0.002f;
	m_pBigBillHead->BigSetGB[2]=0.002f;
	m_pBigBillHead->BigSetGB[3]=0.002f;
	m_pBigBillHead->BigMaxSetGB[0]=0.05f;
	m_pBigBillHead->BigMaxSetGB[1]=0.05f;
	m_pBigBillHead->BigMaxSetGB[2]=0.05f;
	m_pBigBillHead->BigMaxSetGB[3]=0.05f;
	m_pBigBillHead->BigBillSetTypeGB[0]=0.0001f;
	m_pBigBillHead->BigBillSetTypeGB[1]=0.0002f;
	m_pBigBillHead->BigBillSetTypeGB[2]=0.0003f;
	m_pBigBillHead->BigBillSetTypeGB[3]=0.0004f;
	m_pBigBillHead->BigBillSetTypeGB[4]=0.0005f;
	m_pBigBillHead->BigBillSetTypeGB[5]=0.0006f;
	m_pBigBillHead->BigBillSetTypeGB[6]=0.0007f;
	m_pBigBillHead->BigBillSetTypeGB[7]=0.0008f;
	m_pBigBillHead->BigBillSetTypeGB[8]=0.0009f;
	m_pBigBillHead->BigBillSetTypeGB[9]=0.0010f;


    m_pBigBillHead->totalV=0;
	m_pBigBillHead->IsChange=FALSE;
	return TRUE;
}
BOOL CSharesBigBillData::ClearRealTimeHsData()//数据文件读写实现
{
    m_pBigBillHead->ItemCount=0;

    m_pBigBillHead->totalV=0;
	m_pBigBillHead->IsChange=FALSE;

	memset(m_pBigBillHead->totalBigCount,0,sizeof(float)*4);
	memset(m_pBigBillHead->totalBigVolumn,0,sizeof(float)*4);
	memset(m_pBigBillHead->totalCount,0,sizeof(float)*10);
	memset(m_pBigBillHead->totalVolumn,0,sizeof(float)*10);
    m_pBigBillHead->totalV=0;
	return TRUE;
}
BOOL CSharesBigBillData::InitRealTimeHsDataExist()//数据文件读写实现
{
	 m_hFile=CreateFile("RealTimeHs.dat",GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开实时行情换手数据文件出错");
		return FALSE; 
	}
	m_hFileMap=CreateFileMapping(m_hFile,
		NULL,
		PAGE_READWRITE,
		0,
		0,
		NULL);
	if(m_hFileMap==NULL)
	{
		AfxMessageBox("创立文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
	m_pbData=(PBYTE)MapViewOfFile(m_hFileMap,
		FILE_MAP_WRITE,
		0,0,0);
	if(m_pbData==NULL)
	{
		AfxMessageBox("将文件数据映射入内存时出错");
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
    m_pBigBillHead=(BIG_LISTHEAD *)m_pbData;                 
    m_pMapData=(BIG_LIST *)(m_pbData+sizeof(BIG_LISTHEAD));         

	m_pBigBillHead->Done=FALSE;
	if(!m_pBigBillHead->IsSaveData)
	{
		m_pBigBillHead->ItemCount=0;

		m_pBigBillHead->totalV=0;
		m_pBigBillHead->IsChange=FALSE;
		ClearRealTimeHsData();
	}
	return TRUE;
}

BOOL CSharesBigBillData::AddRealTimeHsData(PBIG_LIST &pCjHs)
{
	if(m_pBigBillHead->ItemCount>=m_pBigBillHead->MaxItemCount)
	{
       int temp=70*4+sizeof(BIG_LIST)*(m_pBigBillHead->ItemCount +2000)+200;
       m_pBigBillHead->MaxItemCount +=200; 
	   if(m_pbData)
		 UnmapViewOfFile(m_pbData);
	   if(m_hFileMap) 
		 CloseHandle(m_hFileMap);
		m_hFileMap=CreateFileMapping(m_hFile,
			NULL,
			PAGE_READWRITE,
			0,
			temp,
			NULL);
		if(m_hFileMap==NULL)
		{
			AfxMessageBox("创立文件映射内核时出错");
			CloseHandle(m_hFile);
			m_hFile=NULL;
			m_hFileMap=NULL;
			return FALSE;
		}
		m_pbData=(PBYTE)MapViewOfFile(m_hFileMap,
			FILE_MAP_WRITE,
			0,0,0);
		if(m_pbData==NULL)
		{
			AfxMessageBox("将文件数据映射入内存时出错");
			CloseHandle(m_hFile);
			CloseHandle(m_hFileMap);
			m_hFile=NULL;   
			m_hFileMap=NULL;
			return FALSE;// 释放内存
		}
		m_pBigBillHead=(BIG_LISTHEAD *)m_pbData;                    
		m_pMapData=(BIG_LIST *)(m_pbData+sizeof(BIG_LISTHEAD));         
	}
	pCjHs=m_pMapData+m_pBigBillHead->ItemCount;
    m_pBigBillHead->ItemCount++;
	return TRUE;
}
void CSharesBigBillData::CalcRealTimeHsBigData(BIG_LIST *pCjHs)
{
	switch(pCjHs->ntype)
	{
	case 0:            
		   m_pBigBillHead->totalBigCount[0]++;
		   m_pBigBillHead->totalBigVolumn[0] +=pCjHs->nowv; 
		break;
	case 1:            
		   m_pBigBillHead->totalBigCount[1]++;
 		   m_pBigBillHead->totalBigVolumn[1] +=pCjHs->nowv; 
		break;
	case 2:             
		   m_pBigBillHead->totalBigCount[2]++;
		   m_pBigBillHead->totalBigVolumn[2] +=pCjHs->nowv; 
		break;
	case 3:             
		   m_pBigBillHead->totalBigCount[3]++;
		   m_pBigBillHead->totalBigVolumn[3] +=pCjHs->nowv; 
		break;
	}
}

BOOL CSharesBigBillData::SetBigDataForSetup(float *pBigData,float *pBigMaxData,float *CjmxType)
{// NOTE: the ClassWizard will add member functions here
     memcpy(&m_pBigBillHead->BigSet[0], pBigData,4*sizeof(float));
     memcpy(&m_pBigBillHead->BigMaxSet[0], pBigMaxData,4*sizeof(float));
	 memcpy(&m_pBigBillHead->BigBillSetType[0],CjmxType,10*sizeof(long) );
	 return TRUE;
}
BOOL CSharesBigBillData::SetBigDataForSetupGB(float *pBigData,float *pBigMaxData,float *CjmxType)
{// NOTE: the ClassWizard will add member functions here
     memcpy(&m_pBigBillHead->BigSetGB[0], pBigData,4*sizeof(float));
     memcpy(&m_pBigBillHead->BigMaxSetGB[0], pBigMaxData,4*sizeof(float));
	 memcpy(&m_pBigBillHead->BigBillSetTypeGB[0],CjmxType,10*sizeof(long) );
	 return TRUE;
}
BOOL CSharesBigBillData::CalcBigBillData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport)
{// NOTE: the ClassWizard will add member functions here
	if(!m_pBigBillHead->IsGbjg)
	{
		if(pCdat->totv<m_GpHqReport->m_fVolume)
		{
		  CalcBigBillDataForType(pCdat,m_GpHqReport);
          m_pBigBillHead->IsChange=TRUE;
		}
		CalcBigBillBigData(pCdat,m_GpHqReport);
	}
	else
	{
		float GB;
		if(pCdat->pBaseInfo==NULL)
            return FALSE;
        if(CTaiShanReportView::IsBg(pCdat-> kind))
			GB=pCdat->pBaseInfo->Bg*100;
		else
			GB=pCdat->pBaseInfo->ltAg*100;
		if(GB<=0)
			return FALSE;
		if(pCdat->totv<m_GpHqReport->m_fVolume)
		{
          m_pBigBillHead->IsChange=TRUE;
		  CalcBigBillDataForTypeGB(pCdat,m_GpHqReport,GB);
		}
		CalcBigBillBigDataGB(pCdat,m_GpHqReport,GB);
	}
 	return TRUE;
}
BOOL CSharesBigBillData::CalcBigBillDataForType(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport)
{// NOTE: the ClassWizard will add member functions here
    float temp=m_GpHqReport->m_fVolume - pCdat->totv;
	if(temp<=m_pBigBillHead->BigBillSetType[0])
	{
        m_pBigBillHead->totalCount[0]++;
		m_pBigBillHead->totalVolumn[0] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[0]&&temp<=m_pBigBillHead->BigBillSetType[1])
	{
        m_pBigBillHead->totalCount[1]++;
		m_pBigBillHead->totalVolumn[1] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[1]&&temp<=m_pBigBillHead->BigBillSetType[2])
	{
        m_pBigBillHead->totalCount[2]++;
		m_pBigBillHead->totalVolumn[2] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[2]&&temp<=m_pBigBillHead->BigBillSetType[3])
	{
        m_pBigBillHead->totalCount[3]++;
		m_pBigBillHead->totalVolumn[3] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[3]&&temp<=m_pBigBillHead->BigBillSetType[4])
	{
        m_pBigBillHead->totalCount[4]++;
		m_pBigBillHead->totalVolumn[4] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[4]&&temp<=m_pBigBillHead->BigBillSetType[5])
	{
        m_pBigBillHead->totalCount[5]++;
		m_pBigBillHead->totalVolumn[5] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[5]&&temp<=m_pBigBillHead->BigBillSetType[6])
	{
        m_pBigBillHead->totalCount[6]++;
		m_pBigBillHead->totalVolumn[6] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[6]&&temp<=m_pBigBillHead->BigBillSetType[7])
	{
        m_pBigBillHead->totalCount[7]++;
		m_pBigBillHead->totalVolumn[7] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[7]&&temp<=m_pBigBillHead->BigBillSetType[8])
	{
        m_pBigBillHead->totalCount[8]++;
		m_pBigBillHead->totalVolumn[8] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[8])
	{
        m_pBigBillHead->totalCount[9]++;
		m_pBigBillHead->totalVolumn[9] +=temp;  
	}
    m_pBigBillHead->totalV+=temp;
	return TRUE;
}
BOOL CSharesBigBillData::CalcBigBillBigData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport)
{// NOTE: the ClassWizard will add member functions here
    float temp=m_GpHqReport->m_fVolume - pCdat->totv;


	if(m_GpHqReport->m_fNewPrice >= pCdat->psel1) 
	{    
		if((temp>=m_pBigBillHead->BigSet[0]&&temp<=m_pBigBillHead->BigMaxSet[0])&&m_pBigBillHead->IsCalc[0])
		{
           BIG_LIST *pCjmx;
		   time_t now; 
		   time( &now );
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=0;
			   if(m_GpHqReport->m_fLastClose!=0)
			      pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
           m_pBigBillHead->IsChange=TRUE;
		}
	}else if(m_GpHqReport->m_fNewPrice <=pCdat->pbuy1) 
	{
		if((temp>=m_pBigBillHead->BigSet[1]&&temp<=m_pBigBillHead->BigMaxSet[1])&&m_pBigBillHead->IsCalc[1])
		{
           BIG_LIST *pCjmx;
		   time_t now; 
		   time( &now );
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=1;
			   if(m_GpHqReport->m_fLastClose!=0)
			     pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
           m_pBigBillHead->IsChange=TRUE;
		}
    }
	else
	{
        BIG_LIST *pCjmx;
	    time_t now; 
	    time( &now );
		temp=temp/2;
		if((temp>=m_pBigBillHead->BigSet[0]&&temp<=m_pBigBillHead->BigMaxSet[0])&&m_pBigBillHead->IsCalc[0])
		{
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=0;
			   if(m_GpHqReport->m_fLastClose!=0)
			     pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
		if((temp>=m_pBigBillHead->BigSet[1]&&temp<=m_pBigBillHead->BigMaxSet[1])&&m_pBigBillHead->IsCalc[1])
		{
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=1;
			   if(m_GpHqReport->m_fLastClose!=0)
			     pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
           m_pBigBillHead->IsChange=TRUE;
		}
	}
	BOOL IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[2])
	{
		for(int i=0;i<3;i++)
		{
		   if(m_GpHqReport->m_fBuyPrice[i]==pCdat->pbuy1)
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i] -pCdat->vbuy1;
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }
		   }else if(m_GpHqReport->m_fBuyPrice[i]==pCdat->pbuy2)
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i] -pCdat->vbuy2;
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }

		   }else if(m_GpHqReport->m_fBuyPrice[i]==pCdat->pbuy3)
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i] -pCdat->vbuy3;
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i];
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   time_t now; 
			   time( &now );
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
				   pCjmx->nowp=m_GpHqReport->m_fBuyPrice[i];
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->nowv=temp;
				   pCjmx->ntime=now;
				   pCjmx->ntype=2;
				   if(m_GpHqReport->m_fLastClose!=0)
					  pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   m_pBigBillHead->IsChange=TRUE;
			   IsAdd=FALSE;
			}
		}
	}
	IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[3])
	{
		for(int i=0;i<3;i++)
		{
		   if(m_GpHqReport->m_fSellPrice[i]==pCdat->psel1)
		   {
			   temp=m_GpHqReport->m_fSellVolume[i] -pCdat->vsel1;
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }
		   }else if(m_GpHqReport->m_fSellPrice[i]==pCdat->psel2)
		   {
			   temp=m_GpHqReport->m_fSellVolume[i] -pCdat->vsel2;
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }

		   }else if(m_GpHqReport->m_fSellPrice[i]==pCdat->psel3)
		   {
			   temp=m_GpHqReport->m_fSellVolume[i] -pCdat->vsel3;
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=m_GpHqReport->m_fSellVolume[i];
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   time_t now; 
			   time( &now );
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->nowp=m_GpHqReport->m_fSellPrice[i];
				   pCjmx->nowv=temp;
				   pCjmx->ntime=now;
				   pCjmx->ntype=3;
				   if(m_GpHqReport->m_fLastClose!=0)
					  pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   m_pBigBillHead->IsChange=TRUE;
			   IsAdd=FALSE;
			}
		}
	}
	return TRUE;
}
BOOL CSharesBigBillData::CalcBigBillDataForTypeGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB)
{// NOTE: the ClassWizard will add member functions here
    float temp=m_GpHqReport->m_fVolume - pCdat->totv;
	if(temp<=m_pBigBillHead->BigBillSetTypeGB[0]*GB)
	{
        m_pBigBillHead->totalCount[0]++;
		m_pBigBillHead->totalVolumn[0] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[0]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[1]*GB)
	{
        m_pBigBillHead->totalCount[1]++;
		m_pBigBillHead->totalVolumn[1] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[1]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[2]*GB)
	{
        m_pBigBillHead->totalCount[2]++;
		m_pBigBillHead->totalVolumn[2] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[2]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[3]*GB)
	{
        m_pBigBillHead->totalCount[3]++;
		m_pBigBillHead->totalVolumn[3] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[3]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[4]*GB)
	{
        m_pBigBillHead->totalCount[4]++;
		m_pBigBillHead->totalVolumn[4] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[4]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[5]*GB)
	{
        m_pBigBillHead->totalCount[5]++;
		m_pBigBillHead->totalVolumn[5] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[5]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[6]*GB)
	{
        m_pBigBillHead->totalCount[6]++;
		m_pBigBillHead->totalVolumn[6] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[6]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[7]*GB)
	{
        m_pBigBillHead->totalCount[7]++;
		m_pBigBillHead->totalVolumn[7] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[7]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[8]*GB)
	{
        m_pBigBillHead->totalCount[8]++;
		m_pBigBillHead->totalVolumn[8] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[8]*GB)
	{
        m_pBigBillHead->totalCount[9]++;
		m_pBigBillHead->totalVolumn[9] +=temp;  
	}
    m_pBigBillHead->totalV +=temp;

	return TRUE;
}
BOOL CSharesBigBillData::CalcBigBillBigDataGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB)
{// NOTE: the ClassWizard will add member functions here
    float temp=m_GpHqReport->m_fVolume - pCdat->totv;

	if(m_GpHqReport->m_fNewPrice >= pCdat->pbuy1) 
	{    
		if((temp>=m_pBigBillHead->BigSetGB[0]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[0]*GB)&&m_pBigBillHead->IsCalc[0])
		{
           BIG_LIST *pCjmx;
		   time_t now; 
		   time( &now );
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=0;
			   if(m_GpHqReport->m_fLastClose!=0)
			      pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
           m_pBigBillHead->IsChange=TRUE;
		}
	}else if(m_GpHqReport->m_fNewPrice <=pCdat->psel1) 
	{
		if((temp>=m_pBigBillHead->BigSetGB[1]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[1]*GB)&&m_pBigBillHead->IsCalc[1])
		{
           BIG_LIST *pCjmx;
		   time_t now; 
		   time( &now );
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=1;
			   if(m_GpHqReport->m_fLastClose!=0)
			      pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
           m_pBigBillHead->IsChange=TRUE;
		}
    }
	else
	{
		temp=temp/2;
        BIG_LIST *pCjmx;
	    time_t now; 
	    time( &now );
		if((temp>=m_pBigBillHead->BigSetGB[0]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[0]*GB)&&m_pBigBillHead->IsCalc[0])
		{
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=now;
               pCjmx->ntype=0;
			   if(m_GpHqReport->m_fLastClose!=0)
			     pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
		if((temp>=m_pBigBillHead->BigSetGB[1]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[1]*GB)&&m_pBigBillHead->IsCalc[1])
		{
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=m_GpHqReport->m_fNewPrice;
			   pCjmx->nowv=temp;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->ntime=now;
               pCjmx->ntype=1;
			   if(m_GpHqReport->m_fLastClose!=0)
			     pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
           m_pBigBillHead->IsChange=TRUE;
		}
	}
	BOOL IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[2])
	{
		for(int i=0;i<3;i++)
		{
		   if(m_GpHqReport->m_fBuyPrice[i]==pCdat->pbuy1)
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i] -pCdat->vbuy1;
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else if(m_GpHqReport->m_fBuyPrice[i]==pCdat->pbuy2)
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i] -pCdat->vbuy2;
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }

		   }else if(m_GpHqReport->m_fBuyPrice[i]==pCdat->pbuy3)
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i] -pCdat->vbuy3;
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=m_GpHqReport->m_fBuyVolume[i];
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   time_t now; 
			   time( &now );
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
				   pCjmx->nowp=m_GpHqReport->m_fBuyPrice[i];
				   pCjmx->nowv=temp;
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->ntime=now;
				   pCjmx->ntype=2;
				   if(m_GpHqReport->m_fLastClose!=0)
					  pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   IsAdd=FALSE;
			   m_pBigBillHead->IsChange=TRUE;
			}
		}
	}
	IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[3])
	{
		for(int i=0;i<3;i++)
		{
		   if(m_GpHqReport->m_fSellPrice[i]==pCdat->psel1)
		   {
			   temp=m_GpHqReport->m_fSellVolume[i] -pCdat->vsel1;
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else if(m_GpHqReport->m_fSellPrice[i]==pCdat->psel2)
		   {
			   temp=m_GpHqReport->m_fSellVolume[i] -pCdat->vsel2;
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }

		   }else if(m_GpHqReport->m_fSellPrice[i]==pCdat->psel3)
		   {
			   temp=m_GpHqReport->m_fSellVolume[i] -pCdat->vsel3;
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=m_GpHqReport->m_fSellVolume[i];
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   time_t now; 
			   time( &now );
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
				   pCjmx->nowp=m_GpHqReport->m_fSellPrice[i];
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->nowv=temp;
				   pCjmx->ntime=now;
				   pCjmx->ntype=3;
				   if(m_GpHqReport->m_fLastClose!=0)
					 pCjmx->rdr=(m_GpHqReport->m_fNewPrice - m_GpHqReport->m_fLastClose)/(m_GpHqReport->m_fLastClose);
				   else
					 pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   IsAdd=FALSE;
			   m_pBigBillHead->IsChange=TRUE;
			}
		}
	}
	return TRUE;
}
BOOL CSharesBigBillData::GetBigBillData(int index,PBIG_LIST &pCjmx)
{// NOTE: the ClassWizard will add member functions here
	if(index >=m_pBigBillHead->ItemCount)
	{
        pCjmx=NULL;
		return FALSE;
	}
	pCjmx=m_pMapData+index;
	return TRUE;
}
void CSharesBigBillData::GetParamData(float *pBigData,float *pBigMaxData,float *CjmxType)
{
	memcpy(pBigData,this->m_pBigBillHead->BigSet,sizeof(float)*4);
	memcpy(pBigMaxData,this->m_pBigBillHead->BigMaxSet,sizeof(float)*4);
	memcpy(CjmxType,this->m_pBigBillHead->BigBillSetType,sizeof(float)*10);
}
void CSharesBigBillData::GetParamDataGB(float *pBigData,float *pBigMaxData,float *CjmxType)
{
	memcpy(pBigData,this->m_pBigBillHead->BigSetGB,sizeof(float)*4);
	memcpy(pBigMaxData,this->m_pBigBillHead->BigMaxSetGB,sizeof(float)*4);
	memcpy(CjmxType,this->m_pBigBillHead->BigBillSetTypeGB,sizeof(float)*10);
}
void CSharesBigBillData::SetIsCalcForBig(BOOL *IsCalc)
{
	memcpy(this->m_pBigBillHead->IsCalc,IsCalc,sizeof(BOOL)*4);
}
BOOL CSharesBigBillData::CalcBigBillHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData,int nTotalCount)  //统计成交数据
{
	if(!m_pBigBillHead->IsGbjg)
	{
		for(int i=0;i<nTotalCount-1;i++)
		{
		  CalcBigBillHistoryDataForType(pCdat,&pBigBillData[i+1],&pBigBillData[i]);
		  CalcBigBillBigHistoryData(pCdat,&pBigBillData[i+1],&pBigBillData[i]);
		}
	}
	else
	{
		float GB;
		if(pCdat->pBaseInfo==NULL)
            return FALSE;
        if(CTaiShanReportView::IsBg(pCdat-> kind))
			GB=pCdat->pBaseInfo->Bg*100;
		else
			GB=pCdat->pBaseInfo->ltAg*100;
		for(int i=0;i<nTotalCount-1;i++)
		{
			CalcBigBillHistoryDataForTypeGB(pCdat,&pBigBillData[i+1],&pBigBillData[i],GB);
			CalcBigBillBigHistoryDataGB(pCdat,&pBigBillData[i+1],&pBigBillData[i],GB);
		}
	}
    m_pBigBillHead->IsChange=TRUE;
    return TRUE;
}
void CSharesBigBillData::CalcBigBillHistoryDataForType(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1)
{
    float temp=float(fabs(pBigBillData2->vol) - fabs(pBigBillData1->vol));
	if(temp<=m_pBigBillHead->BigBillSetType[0])
	{
        m_pBigBillHead->totalCount[0]++;
		m_pBigBillHead->totalVolumn[0] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[0]&&temp<=m_pBigBillHead->BigBillSetType[1])
	{
        m_pBigBillHead->totalCount[1]++;
		m_pBigBillHead->totalVolumn[1] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[1]&&temp<=m_pBigBillHead->BigBillSetType[2])
	{
        m_pBigBillHead->totalCount[2]++;
		m_pBigBillHead->totalVolumn[2] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[2]&&temp<=m_pBigBillHead->BigBillSetType[3])
	{
        m_pBigBillHead->totalCount[3]++;
		m_pBigBillHead->totalVolumn[3] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[3]&&temp<=m_pBigBillHead->BigBillSetType[4])
	{
        m_pBigBillHead->totalCount[4]++;
		m_pBigBillHead->totalVolumn[4] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[4]&&temp<=m_pBigBillHead->BigBillSetType[5])
	{
        m_pBigBillHead->totalCount[5]++;
		m_pBigBillHead->totalVolumn[5] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[5]&&temp<=m_pBigBillHead->BigBillSetType[6])
	{
        m_pBigBillHead->totalCount[6]++;
		m_pBigBillHead->totalVolumn[6] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[6]&&temp<=m_pBigBillHead->BigBillSetType[7])
	{
        m_pBigBillHead->totalCount[7]++;
		m_pBigBillHead->totalVolumn[7] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[7]&&temp<=m_pBigBillHead->BigBillSetType[8])
	{
        m_pBigBillHead->totalCount[8]++;
		m_pBigBillHead->totalVolumn[8] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetType[8])
	{
        m_pBigBillHead->totalCount[9]++;
		m_pBigBillHead->totalVolumn[9] +=temp;  
	}
    m_pBigBillHead->totalV+=temp;
}
void CSharesBigBillData::CalcBigBillBigHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1)
{
    float temp=float(fabs(pBigBillData2->vol) - fabs(pBigBillData1->vol));


	if(pBigBillData2->price >= pBigBillData1->sellPrice[0] && (pBigBillData1->sellPrice[0]>0 || pBigBillData1->buyPrice[0]>0)) //大单买入
	{    
		if((temp>=m_pBigBillHead->BigSet[0]&&temp<=m_pBigBillHead->BigMaxSet[0])&&m_pBigBillHead->IsCalc[0])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=0;
			   if(pCdat->ystc!=0)
			      pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
	}else if(pBigBillData2->price <=pBigBillData1->buyPrice[0] && (pBigBillData1->sellPrice[0]>0 || pBigBillData1->buyPrice[0]>0)  ) //大单卖出
	{
		if((temp>=m_pBigBillHead->BigSet[1]&&temp<=m_pBigBillHead->BigMaxSet[1])&&m_pBigBillHead->IsCalc[1])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);//临时数据存储
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=1;
			   if(pCdat->ystc!=0)
			     pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
    }
	else
	{
		temp =temp/2;
		if((temp>=m_pBigBillHead->BigSet[0]&&temp<=m_pBigBillHead->BigMaxSet[0])&&m_pBigBillHead->IsCalc[0])
		{//行情数据结构数组类实现
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=0;
			   if(pCdat->ystc!=0)
			      pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
		if((temp>=m_pBigBillHead->BigSet[1]&&temp<=m_pBigBillHead->BigMaxSet[1])&&m_pBigBillHead->IsCalc[1])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nowv=temp;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=1;
			   if(pCdat->ystc!=0)
			     pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
	}
	BOOL IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[2])
	{
		for(int i=0;i<3;i++)
		{
		   if(pBigBillData2->buyPrice[i]==pBigBillData1->buyPrice[0])
		   {
               temp=pBigBillData2->buyVol[i]- pBigBillData1->buyVol[0];
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }
		   }else if(pBigBillData2->buyPrice[i]==pBigBillData1->buyPrice[1])
		   {
               temp=pBigBillData2->buyVol[i]- pBigBillData1->buyVol[1];
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }

		   }else if(pBigBillData2->buyPrice[i]==pBigBillData1->buyPrice[2])
		   {
               temp=pBigBillData2->buyVol[i]- pBigBillData1->buyVol[2];
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=pBigBillData2->buyVol[i];;
			   if(temp>=m_pBigBillHead->BigSet[2]&&temp<=m_pBigBillHead->BigMaxSet[2])
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
				   pCjmx->nowp=pBigBillData2->buyPrice[i];
				   pCjmx->nowv=temp;
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->ntime=pBigBillData2->time;
				   pCjmx->ntype=2;
				   if(pCdat->ystc!=0)
					  pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   IsAdd=FALSE;
			}
		}
	}
	IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[3])
	{
		for(int i=0;i<3;i++)
		{
		   if(pBigBillData2->sellPrice[i]==pBigBillData1->sellPrice[0])
		   {// NOTE: the ClassWizard will add member functions here
			   temp=pBigBillData2->sellVol[i] -pBigBillData1->sellVol[0];
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }
		   }else if(pBigBillData2->sellPrice[i]==pBigBillData1->sellPrice[1])
		   {
			   temp=pBigBillData2->sellVol[i] -pBigBillData1->sellVol[1];
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }

		   }else if(pBigBillData2->sellPrice[i]==pBigBillData1->sellPrice[2])
		   {
			   temp=pBigBillData2->sellVol[i] -pBigBillData1->sellVol[2];
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=pBigBillData2->sellVol[i];
			   if(temp>=m_pBigBillHead->BigSet[3]&&temp<=m_pBigBillHead->BigMaxSet[3])
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
				   pCjmx->nowp=pBigBillData2->sellPrice[i];
				   pCjmx->nowv=temp;
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->ntime=pBigBillData2->time;
				   pCjmx->ntype=3;
				   if(pCdat->ystc!=0)
					  pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   IsAdd=FALSE;
			}
		}
	}
}
void CSharesBigBillData::CalcBigBillHistoryDataForTypeGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB)
{// NOTE: the ClassWizard will add member functions here
    float temp=float(fabs(pBigBillData2->vol) - fabs(pBigBillData1->vol));
	if(temp<=m_pBigBillHead->BigBillSetTypeGB[0]*GB)
	{
        m_pBigBillHead->totalCount[0]++;
		m_pBigBillHead->totalVolumn[0] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[0]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[1]*GB)
	{
        m_pBigBillHead->totalCount[1]++;
		m_pBigBillHead->totalVolumn[1] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[1]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[2]*GB)
	{
        m_pBigBillHead->totalCount[2]++;
		m_pBigBillHead->totalVolumn[2] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[2]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[3]*GB)
	{
        m_pBigBillHead->totalCount[3]++;
		m_pBigBillHead->totalVolumn[3] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[3]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[4]*GB)
	{
        m_pBigBillHead->totalCount[4]++;
		m_pBigBillHead->totalVolumn[4] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[4]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[5]*GB)
	{
        m_pBigBillHead->totalCount[5]++;
		m_pBigBillHead->totalVolumn[5] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[5]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[6]*GB)
	{
        m_pBigBillHead->totalCount[6]++;
		m_pBigBillHead->totalVolumn[6] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[6]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[7]*GB)
	{
        m_pBigBillHead->totalCount[7]++;
		m_pBigBillHead->totalVolumn[7] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[7]*GB&&temp<=m_pBigBillHead->BigBillSetTypeGB[8]*GB)
	{
        m_pBigBillHead->totalCount[8]++;
		m_pBigBillHead->totalVolumn[8] +=temp;  
	}else if(temp>m_pBigBillHead->BigBillSetTypeGB[8])
	{
        m_pBigBillHead->totalCount[9]++;
		m_pBigBillHead->totalVolumn[9] +=temp;  
	}
    m_pBigBillHead->totalV +=temp;
}
void CSharesBigBillData::CalcBigBillBigHistoryDataGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB)
{// NOTE: the ClassWizard will add member functions here
    float temp=float(fabs(pBigBillData2->vol) - fabs(pBigBillData1->vol));

	if(pBigBillData2->price >= pBigBillData1->buyPrice[0]) 
	{    
		if((temp>=m_pBigBillHead->BigSetGB[0]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[0]*GB)&&m_pBigBillHead->IsCalc[0])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nowv=temp;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=0;
			   if(pCdat->ystc!=0)
			      pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
	}else if(pBigBillData2->price <=pBigBillData1->sellPrice[0])
	{
		if((temp>=m_pBigBillHead->BigSetGB[1]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[1]*GB)&&m_pBigBillHead->IsCalc[1])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=1;
			   if(pCdat->ystc!=0)
			     pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
    }else
	{
		temp=temp/2;
		if((temp>=m_pBigBillHead->BigSetGB[0]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[0]*GB)&&m_pBigBillHead->IsCalc[0])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nowv=temp;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=0;
			   if(pCdat->ystc!=0)
			      pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                  pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
		if((temp>=m_pBigBillHead->BigSetGB[1]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[1]*GB)&&m_pBigBillHead->IsCalc[1])
		{
           BIG_LIST *pCjmx;
		   if(AddRealTimeHsData(pCjmx))
		   {
               strcpy(pCjmx->id,pCdat->id);
			   strcpy(pCjmx->name,pCdat->name);
			   pCjmx->nowp=pBigBillData2->price;
			   pCjmx->nKind=pCdat->kind;
			   pCjmx->nowv=temp;
			   pCjmx->ntime=pBigBillData2->time;
               pCjmx->ntype=1;
			   if(pCdat->ystc!=0)
			     pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
			   else
                 pCjmx->rdr=0;
               CalcRealTimeHsBigData(pCjmx);
		   }
		}
	}
	BOOL IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[2])
	{
		for(int i=0;i<3;i++)
		{
		   if(pBigBillData2->buyPrice[i]==pBigBillData1->buyPrice[0])
		   {
               temp=pBigBillData2->buyVol[i]- pBigBillData1->buyVol[0];
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else if(pBigBillData2->buyPrice[i]==pBigBillData1->buyPrice[1])
		   {
               temp=pBigBillData2->buyVol[i]- pBigBillData1->buyVol[1];
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }

		   }else if(pBigBillData2->buyPrice[i]==pBigBillData1->buyPrice[2])
		   {
               temp=pBigBillData2->buyVol[i]- pBigBillData1->buyVol[2];
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=pBigBillData2->buyVol[i];;
			   if(temp>=m_pBigBillHead->BigSetGB[2]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[2]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->nowp=pBigBillData2->buyPrice[i];
				   pCjmx->nowv=temp;
				   pCjmx->ntime=pBigBillData2->time;
				   pCjmx->ntype=2;
				   if(pCdat->ystc!=0)
					  pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   IsAdd=FALSE;
			}
		}
	}
	IsAdd=FALSE;
	if(m_pBigBillHead->IsCalc[3])
	{
		for(int i=0;i<3;i++)
		{
		   if(pBigBillData2->sellPrice[i]==pBigBillData1->sellPrice[0])
		   {
			   temp=pBigBillData2->sellVol[i] -pBigBillData1->sellVol[0];
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else if(pBigBillData2->sellPrice[i]==pBigBillData1->sellPrice[1])
		   {
			   temp=pBigBillData2->sellVol[i] -pBigBillData1->sellVol[1];
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }

		   }else if(pBigBillData2->sellPrice[i]==pBigBillData1->sellPrice[2])
		   {// NOTE: the ClassWizard will add member functions here
			   temp=pBigBillData2->sellVol[i] -pBigBillData1->sellVol[2];
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }else
		   {
			   temp=pBigBillData2->sellVol[i];
			   if(temp>=m_pBigBillHead->BigSetGB[3]*GB&&temp<=m_pBigBillHead->BigMaxSetGB[3]*GB)
			   {
				   IsAdd=TRUE;
			   }
		   }
			if(IsAdd)
			{
			   BIG_LIST *pCjmx;
			   if(AddRealTimeHsData(pCjmx))
			   {
				   strcpy(pCjmx->id,pCdat->id);
				   strcpy(pCjmx->name,pCdat->name);
				   pCjmx->nowp=pBigBillData2->sellPrice[i];
				   pCjmx->nowv=temp;
			       pCjmx->nKind=pCdat->kind;
				   pCjmx->ntime=pBigBillData2->time;
				   pCjmx->ntype=3;
				   if(pCdat->ystc!=0)
					  pCjmx->rdr=(pBigBillData2->price - pCdat->ystc)/(pCdat->ystc);
				   else
					  pCjmx->rdr=0;
				   CalcRealTimeHsBigData(pCjmx);
			   }
			   IsAdd=FALSE;
			}
		}
	}
}
