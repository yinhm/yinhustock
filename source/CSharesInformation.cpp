
#include "stdafx.h"
#include <share.h>
#include  <io.h>
#include "StructTaiShares.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"
#include "CSharesInformation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharesInformation::CSharesInformation()
{
   for(int i=0;i<STOCKTYPE;i++)
   {
	   m_pData[i]=NULL;
   }
    m_hFile=NULL;
	m_hFileMap=NULL;
	m_pbData=NULL;
	m_pMapData=NULL;
	Nidx[0]=NULL;                                  
	Nidx[1]=NULL;                                        
	Nidx[2]=NULL;                                        
    Tidx[0]=NULL;                                         
    Tidx[1]=NULL;                                      
    Tidx[2]=NULL;                                    

	ClearUpDown(0);
	ClearUpDown(1);

}
CSharesInformation::~CSharesInformation()
{
   BOOL result=SavePosToFile();
   RemoveStockInfo();
   if(result)
     m_RealFileHead->FileExitDone=12345678;  
   else
     m_RealFileHead->FileExitDone=88888888;  
   SaveRealDataToFile(m_RealFileHead,0);
   for(int i=0;i<STOCKTYPE;i++)
   {
	   if(m_pData[i])
	   {
		   GlobalUnlock((HGLOBAL)m_pData[i]);   
		   GlobalFree( (HGLOBAL)m_pData[i]);
	   }
   }
   if(m_pbData)
	 UnmapViewOfFile(m_pbData);
   if(m_hFileMap) 
	 CloseHandle(m_hFileMap);
   if(m_hFile)
	 CloseHandle(m_hFile);
}

void CSharesInformation::SavePosToFile(int StockType)
{
	 for(int j=0;j<m_pdwStockCurrentCount[StockType];j++)
	 {
		  if(m_pData[StockType][j].pItem==NULL)
			  continue;
          m_pData[StockType][j].pItem->sel=j; 
	 }
}
BOOL CSharesInformation::SavePosToFile()
{
     for(int i=0;i<STOCKTYPE;i++)
	 {
		 for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		 {
			  if(m_pData[i][j].pItem==NULL)
			  {
				  return FALSE;
			  }
              m_pData[i][j].pItem->sel=j; 
		 }
	 }
	  return TRUE;
}

BOOL CSharesInformation::InsertItem(char* StockId, PCdat1& pStockData, DWORD StockType)
{
	int low = 0;
	int high = m_pdwStockCurrentCount[StockType] - 1;
	int mid = 0;
	int InsertPose = -1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (low == high)
		{
			if (strcmp(StockId, m_pData[StockType][mid].StockId) > 0)
			{
				if (m_pdwStockCurrentCount[StockType] == 0 || m_pdwStockCurrentCount[StockType] == mid)
				{
					InsertPose = mid;
				}
				else
				{
					InsertPose = mid + 1;
				}
				break;
			}
			else if (strcmp(StockId, m_pData[StockType][mid].StockId) < 0)
			{
				InsertPose = mid;
				break;
			}
			else
			{
				pStockData = m_pData[StockType][mid].pItem;
				return TRUE;
			}
		}

		if (strcmp(m_pData[StockType][mid].StockId, StockId) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pData[StockType][mid].StockId, StockId) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			pStockData = m_pData[StockType][mid].pItem;
			return TRUE;
		}
	}

	if (high < low)
	{
		InsertPose = low;
	}

	if (m_RealFileHead->StockCount + 1 > m_RealFileHead->MaxStockCount)
	{
		return FALSE;
	}


	if (m_pdwStockCurrentCount[StockType] + 1 > m_dwStockMaxCount[StockType])
	{
		SetMemroyALLOCSize(StockType, m_pdwStockCurrentCount[StockType]);
	}

	if (m_pdwStockCurrentCount[StockType] > InsertPose)
	{
		STOCKDATASHOW* ptemp;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc(GPTR, (m_pdwStockCurrentCount[StockType] - InsertPose) * sizeof(STOCKDATASHOW));
		hp = GlobalLock(hMem);
		if (hp)
		{
			ptemp = (STOCKDATASHOW*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
			return FALSE;
		}

		memcpy(ptemp, &m_pData[StockType][InsertPose], (m_pdwStockCurrentCount[StockType] - InsertPose) * sizeof(STOCKDATASHOW));
		memcpy(&m_pData[StockType][InsertPose + 1], ptemp, (m_pdwStockCurrentCount[StockType] - InsertPose) * sizeof(STOCKDATASHOW));
		GlobalUnlock((HGLOBAL)ptemp);
		GlobalFree((HGLOBAL)ptemp);
	}

	m_pData[StockType][InsertPose].pItem = m_pMapData + m_RealFileHead->StockCount;
	strcpy(m_pData[StockType][InsertPose].StockId, StockId);
	m_RealFileHead->StockCount++;

	memset(m_pData[StockType][InsertPose].pItem, 0, sizeof(CReportData));

	m_pData[StockType][InsertPose].pItem->pBaseInfo = NULL;
	m_pData[StockType][InsertPose].pItem->pStockTypeInfo = NULL;

	m_pData[StockType][InsertPose].pItem->IsDelete = 0;

	strcpy(m_pData[StockType][InsertPose].pItem->id, StockId);
	m_pData[StockType][InsertPose].pItem->kind = StockType;

	pStockData = m_pData[StockType][InsertPose].pItem;
	m_pdwStockCurrentCount[StockType]++;
	//SavePosToFile(StockType);
	SaveRealDataToFile(m_RealFileHead, sizeof(REALDATA) + sizeof(long) * STOCKTYPE);
	SaveRealDataToFile(m_pData[StockType][InsertPose].pItem, sizeof(CReportData));

	return TRUE;
}

BOOL CSharesInformation::InitRealTimeDataEmpty()
{
	BYTE *temp;

	long m_FileLength = sizeof(REALDATA) + STOCKTYPE * 4 + 240 * 4 * 3 + 240 * 8 * 3 + sizeof(CReportData) * 2000;

	m_hFile = CreateFile(g_realtime, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开实时行情数据出错");
		return FALSE; 
	}

	m_hFileMap=CreateFileMapping(m_hFile,
		NULL,
		PAGE_READWRITE,
		0,
		m_FileLength,
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
	CTime m_Time = CTime::GetCurrentTime();
	long currDay=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;

	m_RealFileHead=(REALDATA *)m_pbData;
	m_RealFileHead->filetitle =12345678;
	m_RealFileHead->MaxStockCount=2000; 
	m_RealFileHead->StockCount =0;
	m_RealFileHead->FileExitDone=87654321;
	m_RealFileHead->Day=currDay;
	m_RealFileHead->CloseWorkDone=FALSE;
	m_RealFileHead->OldANT[0]=0;
	m_RealFileHead->OldANT[1]=0;
	m_RealFileHead->OldANT[2]=0;

	temp=m_pbData+sizeof(REALDATA);
	m_pdwStockCurrentCount=(DWORD *)temp;
	temp +=sizeof(int)*STOCKTYPE;
	for(int j=0;j<STOCKTYPE;j++)
		m_pdwStockCurrentCount[0]=0;

	Nidx[0]=(Rsdn1 *)temp;		             
	memset(Nidx[0],0,240*sizeof(Rsdn1));
	temp +=sizeof(Rsdn1)*240;

	Nidx[1]=(Rsdn1 *)temp;		         
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	temp +=sizeof(Rsdn1)*240;

	Nidx[2]=(Rsdn1 *)temp;		               
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	temp +=sizeof(Rsdn1)*240;

	Tidx[0]=(Tidxd *)temp;                       
	temp +=sizeof(Tidxd)*240;
	memset(Tidx[0],0,240*sizeof(Tidxd));

	Tidx[1]=(Tidxd *)temp;                       
	temp +=sizeof(Tidxd)*240;
	memset(Tidx[1],0,240*sizeof(Tidxd));

	Tidx[2]=(Tidxd *)temp;                       
	temp +=sizeof(Tidxd)*240;
	memset(Tidx[2],0,240*sizeof(Tidxd));

	for (int j=0; j<240; j++)
	{
		Tidx[0][j].sec5=Tidx[1][j].sec5=Tidx[2][j].sec5=9911;
	}
	m_pMapData=(CReportData *)temp;	
	for(int j=0;j<STOCKTYPE;j++)
	{
		if(!SetMemroyALLOCSize(j,5))
		{
			AfxMessageBox("初始化数据变量出错");
			return FALSE;
		} 
	}
	InitEmptyDatabase();
	SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)); 
	return TRUE;
}

BOOL CSharesInformation::InitRealTimeData(CString path)
{
    strcpy(m_sPath,path.GetBuffer(0)); 
	this->m_StockBaseInfo.InitBaseInfoData(path);  
	if(_access(g_realtime,0)==-1)   
	{
       InitRealTimeDataEmpty();
	}
    else
	{
       InitRealTimeDataExist();

	}

    return TRUE;
}
BOOL  CSharesInformation::InsertItemPoint(CReportData *pStockData )          
{     
	int StockType=pStockData->kind; 
	int InsertPose=pStockData->sel; 
	if(m_pdwStockCurrentCount[StockType] + 1 > m_dwStockMaxCount[StockType]) 
	{
        SetMemroyALLOCSize(StockType,m_pdwStockCurrentCount[StockType]);
	}
    m_pData[StockType][InsertPose].pItem=pStockData;
	strcpy(m_pData[StockType][InsertPose].StockId,pStockData->id);
    m_pdwStockCurrentCount[StockType]++;
    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPE); 
    SaveRealDataToFile(m_pData[StockType][InsertPose].pItem,sizeof(CReportData)); 
	return TRUE; 
}
void CSharesInformation::CalcIndexBuyAndSell()
{
	if(m_pdwStockCurrentCount[0]<9)
		return;
	m_pData[0][0].pItem->accb=0;    
	m_pData[0][1].pItem->accb=0;        
	m_pData[0][2].pItem->accb=0;     
	m_pData[0][0].pItem->accs=0;    
	m_pData[0][1].pItem->accs=0;    
	m_pData[0][2].pItem->accs=0;    

	for(int i=0;i<m_pdwStockCurrentCount[1];i++)
	{
		if(m_pData[1][i].pItem==NULL)
			continue;
        m_pData[0][0].pItem->accb +=m_pData[1][i].pItem->accb;
        m_pData[0][0].pItem->accs +=m_pData[1][i].pItem->accs;

        m_pData[0][1].pItem->accb +=m_pData[1][i].pItem->accb;
        m_pData[0][1].pItem->accs +=m_pData[1][i].pItem->accs;
		
	}
	for(int i=0;i<m_pdwStockCurrentCount[2];i++)
	{
		if(m_pData[2][i].pItem==NULL)
			continue;
        m_pData[0][0].pItem->accb +=m_pData[2][i].pItem->accb;
        m_pData[0][0].pItem->accs +=m_pData[2][i].pItem->accs;

        m_pData[0][2].pItem->accb +=m_pData[2][i].pItem->accb;
        m_pData[0][2].pItem->accs +=m_pData[2][i].pItem->accs;
	}
	if(m_pdwStockCurrentCount[3]<9)
		return;
	m_pData[3][0].pItem->accb=0;    
	m_pData[3][1].pItem->accb=0;         
	m_pData[3][2].pItem->accb=0;  
	m_pData[3][0].pItem->accs=0;     
	m_pData[3][1].pItem->accs=0;    
	m_pData[3][2].pItem->accs=0;    
	for(int i=0;i<m_pdwStockCurrentCount[4];i++)
	{
		if(m_pData[4][i].pItem==NULL)
			continue;
        m_pData[3][0].pItem->accb +=m_pData[4][i].pItem->accb;
        m_pData[3][0].pItem->accs +=m_pData[4][i].pItem->accs;

        m_pData[3][1].pItem->accb +=m_pData[4][i].pItem->accb;
        m_pData[3][1].pItem->accs +=m_pData[4][i].pItem->accs;
	}
	for(int i=0;i<m_pdwStockCurrentCount[5];i++)
	{
		if(m_pData[5][i].pItem==NULL)
			continue;
        m_pData[3][0].pItem->accb +=m_pData[5][i].pItem->accb;
        m_pData[3][0].pItem->accs +=m_pData[5][i].pItem->accs;

        m_pData[3][2].pItem->accb +=m_pData[5][i].pItem->accb;
        m_pData[3][2].pItem->accs +=m_pData[5][i].pItem->accs;
	}
	if(m_pdwStockCurrentCount[8]>=1)
	{
		m_pData[8][0].pItem->accb=0;     
		m_pData[8][0].pItem->accs=0;    
		for(int i=0;i<m_pdwStockCurrentCount[9];i++)
		{
			if(m_pData[9][i].pItem==NULL)
				continue;
			m_pData[8][0].pItem->accb +=m_pData[9][i].pItem->accb;
			m_pData[8][0].pItem->accs +=m_pData[9][i].pItem->accs;
		}
	}
}
BOOL CSharesInformation::ClearRealDataMinute()
{
    for(int j=0;j<STOCKTYPE;j++)
	{
		for(int i=0;i<m_pdwStockCurrentCount[j];i++)
		{
			if(m_pData[j][i].pItem==NULL)
				continue;
			m_pData[j][i].pItem->rdp='9';
		    m_pData[j][i].pItem->lastclmin =0;
		    m_pData[j][i].pItem->initdown =FALSE;
            m_pData[j][i].pItem->volume5=0;
            memset(&m_pData[j][i].pItem->ystc,0,8*sizeof(float));
            memset(&m_pData[j][i].pItem->rvol,0,2*sizeof(float));
			memset(&m_pData[j][i].pItem->accb,0,2*sizeof(float));
			memset(&m_pData[j][i].pItem->pbuy1,0,12*sizeof(float));
			memset(m_pData[j][i].pItem->m_Kdata1,0,sizeof(Kdata1)*240);
		}
	}
	memset(Nidx[0],0,240*sizeof(Rsdn1));
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	memset(Nidx[2],0,240*sizeof(Rsdn1));
	for(int j=0;j<240;j++)
	{
       Tidx[0][j].sec5=Tidx[1][j].sec5=Tidx[2][j].sec5=9911;
	}
	return TRUE;
}
void CSharesInformation::InitBaseInfoData()
{
	int BaseCount=m_StockBaseInfo.GetStockCount(); 
	if(BaseCount<0)
		return;
	for(int i=0;i<BaseCount;i++)
	{
		BASEINFO *pBase;
		CReportData *Cdat;
	    CTaiShanDoc	*m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
        m_StockBaseInfo.GetStockItem(i,pBase);
		CString m_strZqdmKind=pBase->Symbol;
		CString m_strZqdm=m_strZqdmKind.Right(m_strZqdmKind.GetLength()-2);

        int nKind=m_pDoc->GetStockKind(m_strZqdmKind.Left(2));
		if(nKind<0)
			continue;
		if(this->Lookup(m_strZqdm.GetBuffer(0),Cdat,nKind))
		{
           Cdat->pBaseInfo=pBase;  
		}
	}
}
void CSharesInformation::ReadBaseInfoData(PCdat1 &pStockData)      
{
     if(pStockData->pBaseInfo==NULL)
	 {
	    BASEINFO *pBaseItem;
        if(!m_StockBaseInfo.AddStockItem(pStockData->id,pStockData->kind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
		{
			AfxMessageBox("增加股票基本资料空间区域时出错！");
			return;
		}
        pBaseItem->NumSplit=0;
		pStockData->pBaseInfo=pBaseItem;
	 }
	 m_StockBaseInfo.ReadBaseInfoData(pStockData->id,pStockData->kind,pStockData->pBaseInfo); //读入数据
     m_StockBaseInfo.SaveBaseInfoToFile(pStockData->pBaseInfo,sizeof(BASEINFO));
}
void CSharesInformation::ReadAllBaseInfo()
{
     for(int i=0;i<STOCKTYPE;i++)
	 {
		 if(i==SHZS||i==SZZS||i==STKTYPE)
			 continue;
		 for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		 {
			 ReadBaseInfoData(this->m_pData[i][j].pItem);  
		 }
	 }
}

BOOL CSharesInformation::AddOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind)
{
	CReportData *pCdat;
     if(InsertItem(strStockCode.GetBuffer(0) ,pCdat,nKind ))
	 {
         strcpy(pCdat->name,strStockName.GetBuffer (0));
		 strcpy(pCdat->Gppyjc, strStockPyjc);
		 return TRUE;
	 }
	 return FALSE;
}
BOOL CSharesInformation::MOdifyOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind)
{
	CReportData *pCdat;
	if(this->Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
         strcpy(pCdat->name,strStockName.GetBuffer (0));
		 strcpy(pCdat->Gppyjc, strStockPyjc);
		 return TRUE;
	}
	return FALSE;
}

int CSharesInformation::GetChuQuanInfo(CString strStockCode,int nKind,PSplit &pSplit)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		if(pCdat->pBaseInfo!=NULL)
		{
			pSplit=pCdat->pBaseInfo->m_Split;  
			return pCdat->pBaseInfo->NumSplit; 
		}
	}
	pSplit=NULL;
	return -1;
}
BOOL CSharesInformation::AddChuQuanInfo(CString strStockCode,int nKind,Split *pSplit)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			BASEINFO *pBaseItem;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,pCdat->kind,pBaseItem))           
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
		 }

         memcpy(pCdat->pBaseInfo->m_Split+pCdat->pBaseInfo->NumSplit,
		  pSplit,sizeof(Split)); 
		 pCdat->pBaseInfo->NumSplit++ ;  
		 return TRUE; 
	}
	return FALSE;
}
BOOL CSharesInformation::ModifyChuQuanInfo(CString strStockCode,int nWhichItem,Split *pSplit,int nKind)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		if(pCdat->pBaseInfo!=NULL)
		{
		    if(nWhichItem>=pCdat->pBaseInfo->NumSplit)
			   return FALSE;
            memcpy(pCdat->pBaseInfo->m_Split+nWhichItem,
			  pSplit,sizeof(Split)); 
			return TRUE; 
		}
	}
	return FALSE;
}
BOOL CSharesInformation::DeleteChuQuanInfo(CString strStockCode,int nWhichItem,int nKind)
{
	CReportData *pCdat;

	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		if(pCdat->pBaseInfo!=NULL)
		{
   			if(nWhichItem>=pCdat->pBaseInfo->NumSplit)
				return FALSE;
			 memmove(pCdat->pBaseInfo->m_Split+nWhichItem,
			  pCdat->pBaseInfo->m_Split+nWhichItem+1 ,
			  (pCdat->pBaseInfo->NumSplit - nWhichItem -1 )*sizeof(Split)); 
			pCdat->pBaseInfo->NumSplit-- ;  
			return TRUE; 
		}
	}
	return FALSE;
}
BOOL CSharesInformation::ImportChuQuanInfo(CString strStockCode,Split *pSplit,
		int nChuquanTotalTimes,int nKind)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			BASEINFO *pBaseItem;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,nKind,pBaseItem))         
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
		 }

         memcpy(pCdat->pBaseInfo->m_Split,
		  pSplit,sizeof(Split)*nChuquanTotalTimes); 
		 pCdat->pBaseInfo->NumSplit =nChuquanTotalTimes;  
	     m_StockBaseInfo.SaveBaseInfoToFile(pCdat->pBaseInfo,sizeof(BASEINFO)+sizeof(Split)*80);
		 return TRUE; 
	}
	return FALSE;

}
BOOL CSharesInformation::ExportChuQuanInfo(CString strStockCode,PSplit &pSplit,
		int& nChuquanTotalTimes,int nKind)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
	    if(pCdat->pBaseInfo!=NULL)
		{
            pSplit=pCdat->pBaseInfo->m_Split;
   			nChuquanTotalTimes=pCdat->pBaseInfo->NumSplit;  
			return TRUE; 
		}
	}
	return FALSE;
}
BOOL CSharesInformation::RemoveStockInfo()
{
	int index=m_RealFileHead->StockCount;
	int nDelete=0;
	for(int j=0;j<index;j++)
	{
         if((m_pMapData+j)->IsDelete)
		 {
			 memmove(m_pMapData+j,m_pMapData+index -1 ,sizeof(CReportData));
             nDelete++;
             index--;
			 j--;
		 }
	}
	m_RealFileHead->StockCount -=nDelete;
    return TRUE;    
}
BOOL CSharesInformation::RemoveAllStockCjmxTj()
{
     for(int i=0;i<STOCKTYPE;i++)
	 {
		 if(i==SHZS||i==SZZS||i==STKTYPE)
			 continue;
		 for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		 {
			 if(m_pData[i][j].pItem!=NULL)
               m_pData[i][j].pItem->IsMxTj=FALSE;
		 }
	 }
	 return TRUE;
}
BOOL CSharesInformation::RemoveStockCjmxTj(char *code,int nKind,BOOL IsAdd)
{
	CReportData *pCdat;
	if(Lookup(code , pCdat,nKind))
	{
		if(IsAdd)
		   pCdat->IsMxTj=TRUE;
		else
		   pCdat->IsMxTj=FALSE;
		return TRUE;
	}
	return FALSE;
}
int CSharesInformation::GetStockPos(int StockType,char *StockId) 
{
	int low=0;
	int high=m_pdwStockCurrentCount[StockType]-1 ;
	int mid=0;
    while(low <= high)
	{
		 mid=(low+high)/2;
		 if(strcmp(m_pData[StockType][mid].StockId , StockId)>0) high=mid -1;
         else if(strcmp(m_pData[StockType][mid].StockId , StockId)< 0 ) low=mid +1;
		 else 
		 {
			 return mid ;
		 }
	}
	return -1;
}
BOOL CSharesInformation::ImportCaiwuInfo(BASEINFO *pBaseinfo)
{
	CReportData *pCdat;
    CTaiShanDoc	*m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
	CString m_strStockKind=pBaseinfo->Symbol;
    CString m_strZqdm=m_strStockKind.Right(m_strStockKind.GetLength()-2);
	int nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));

	if(Lookup(m_strZqdm.GetBuffer(0),pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			BASEINFO *pBaseItem;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,nKind,pBaseItem))           
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
		 }
         memcpy(&pCdat->pBaseInfo->zgb, &pBaseinfo->zgb,sizeof(float)*33);
	     m_StockBaseInfo.SaveBaseInfoToFile(pCdat->pBaseInfo,sizeof(BASEINFO));
		 return TRUE; 
	}
	return FALSE;

}
BOOL  CSharesInformation::InsertItemCorrect(char *StockId ,PCdat1 pStockData, DWORD StockType)
{
	int low=0;
	int high=m_pdwStockCurrentCount[StockType] -1;
	int mid=0;
	int InsertPose=-1;
    while(low <= high)
	{
		 mid=(low+high)/2;
	     if(low==high)
		    if(strcmp(StockId , m_pData[StockType][mid].StockId )>0)
			{
               if(m_pdwStockCurrentCount[StockType]==0||m_pdwStockCurrentCount[StockType]==mid)
                  InsertPose=mid ;
			   else
                  InsertPose=mid +1;   
			   break;
			}
		    else if(strcmp(StockId , m_pData[StockType][mid].StockId)<0)
			{
			   InsertPose=mid ;
			   break;
			}
			else
			{
             pStockData->IsDelete=TRUE;
			 return TRUE ;
			}
 		 if(strcmp(m_pData[StockType][mid].StockId , StockId)>0) high=mid -1;
         else if(strcmp(m_pData[StockType][mid].StockId , StockId)< 0 ) low=mid +1;
		 else 
		 {
             pStockData->IsDelete=TRUE;
			 return TRUE ;
		 }
	}
	if(high<low)
		InsertPose=low;


	if(m_pdwStockCurrentCount[StockType] + 1 > m_dwStockMaxCount[StockType]) 
	{
        SetMemroyALLOCSize(StockType,m_pdwStockCurrentCount[StockType]);
	}
	if(m_pdwStockCurrentCount[StockType] > InsertPose)
	{
       STOCKDATASHOW *ptemp;
	   HGLOBAL	hMem;
	   LPVOID hp;
	   hMem = GlobalAlloc( GPTR, (m_pdwStockCurrentCount[StockType] - InsertPose )* sizeof( STOCKDATASHOW) );
	   hp=GlobalLock(hMem);
	   if(hp)
	   {
			ptemp= (STOCKDATASHOW *)hp;
	   }
	   else
	   {
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
	   }
	   memcpy(ptemp,&m_pData[StockType][InsertPose],(m_pdwStockCurrentCount[StockType] - InsertPose)*sizeof(STOCKDATASHOW));
	   memcpy(&m_pData[StockType][InsertPose+1],ptemp,(m_pdwStockCurrentCount[StockType] - InsertPose)*sizeof(STOCKDATASHOW));
       GlobalUnlock((HGLOBAL)ptemp);      
	   GlobalFree( (HGLOBAL)ptemp);
    }
    m_pData[StockType][InsertPose].pItem=pStockData;
	strcpy(m_pData[StockType][InsertPose].StockId,StockId);

	m_pData[StockType][InsertPose].pItem->pBaseInfo =NULL;
	m_pData[StockType][InsertPose].pItem->pStockTypeInfo =NULL;
	
    strcpy(m_pData[StockType][InsertPose].pItem->id  ,StockId);
    m_pdwStockCurrentCount[StockType]++;
	m_RealFileHead->StockCount++;

    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPE); 
    SaveRealDataToFile(m_pData[StockType][InsertPose].pItem,sizeof(CReportData)); 

	return TRUE;
}
BOOL CSharesInformation::DeleteAllStockFromStockType()
{
	int m_counts=GetStockTypeCount(STKTYPE);
	for(int i=m_counts-1;  i>=0 ; i--)
       RemoveKey(m_pData[STKTYPE][i].StockId ,STKTYPE);   
	return TRUE;
}

// 读取股票代码表
BOOL CSharesInformation::InitEmptyDatabase()
{
	typedef struct 
	{
		char kind;
		char id[7];
		char name[9];
		char Gppyjc[5];
		char group;
		long sel;
	} TempCdat;

	FILE* fp;
	CReportData* pCdat;
	TempCdat Cdat;

	int StockCount[STOCKTYPE];

	if (_access("stockname.dat", 0) == -1)
		return FALSE;

	if ((fp = _fsopen("stockname.dat", "rb", SH_DENYNO)) != NULL)
	{
		fseek(fp, 0, SEEK_SET);
		fread(&StockCount[0], 4, STOCKTYPE, fp);
		while (!feof(fp) && !ferror(fp))
		{
			CString Zqdm;
			long Serl = 0;
			char group = 0;
			memset(&Cdat, 0, sizeof(TempCdat));
			fread(&Cdat.kind, 1, 1, fp);
			fread(Cdat.id, 1, 6, fp);
			Cdat.id[6] = '\0';
			fread(Cdat.name, 1, 8, fp);
			Cdat.name[8] = '\0';
			fread(Cdat.Gppyjc, 1, 4, fp);
			Cdat.Gppyjc[4] = '\0';
			fread(&Cdat.sel, 1, 4, fp);
			char ch123;
			fread(&ch123, 1, 1, fp);

			if (Lookup(Cdat.id, pCdat, Cdat.kind) != TRUE)
			{
				if (strlen(Cdat.id) == 6 || strlen(Cdat.id) == 4)
				{
					int stocktype = Cdat.kind;
					if (stocktype >= 0 && stocktype <= 10)
					{
						if (!InsertItem(Cdat.id, pCdat, Cdat.kind))
							continue;

						strcpy(pCdat->name, Cdat.name);
						strcpy(pCdat->id, Cdat.id);
						pCdat->kind = Cdat.kind;
						strcpy(pCdat->Gppyjc, Cdat.Gppyjc);
					}
				}
			} 
			else
			{
				if (strcmp(pCdat->name, Cdat.name) != 0)
				{
					strcpy(pCdat->name, Cdat.name);
					strcpy(pCdat->Gppyjc, Cdat.Gppyjc);
				}
			}
		}

		fclose(fp);
	}

	return TRUE;
}

BOOL CSharesInformation::AddBaseinfoPoint(CString strStockCode,int nKind,PBASEINFO &pBaseinfo)
{
	pBaseinfo=NULL;
	CReportData *pCdat;

	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			 BASEINFO *pBaseItem=NULL;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,nKind,pBaseItem))              
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
			pBaseinfo=pBaseItem;
		    return TRUE; 
		 }
		 else
		 {
		    pBaseinfo=pCdat->pBaseInfo;
			return TRUE;
		 }
	}
	return FALSE;
}
#ifdef WIDE_NET_VERSION
BOOL CSharesInformation::ClearAllRealTimeMarketData()
{
	BYTE *temp;
	m_RealFileHead->StockCount =0;
	for(int j=0;j<STOCKTYPE;j++)
        m_pdwStockCurrentCount[0]=0;

	memset(Nidx[0],0,240*sizeof(Rsdn1));
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	memset(Tidx[0],0,240*sizeof(Tidxd));
	memset(Tidx[1],0,240*sizeof(Tidxd));
	memset(Tidx[2],0,240*sizeof(Tidxd));
    for (j=0; j<240; j++)
	{
       Tidx[0][j].sec5=Tidx[1][j].sec5=Tidx[2][j].sec5=9911;
	}
	return TRUE;
}

#endif


DWORD CSharesInformation::GetStockKind(int MarketType, char* strLabel)
{
	char* StockId;
	StockId = strLabel;

	if (MarketType == SH_MARKET_EX)
	{
		if ((StockId[1] == 'A') || (StockId[1] == 'B' ) || (StockId[1] == 'C') || (StockId[0] == '0' && StockId[1] == '0' && StockId[2] == '0'))
		{
			return SHZS;
		}

		if (StockId[0] == '9' && StockId[1] == '0' && StockId[2] == '0')
		{
			return SHBG;
		}

		if (StockId[0] == '5')
		{
			return SHJIJIN; 
		}

		if ((StockId[0] == '0' && StockId[1] == '1') ||
			(StockId[0] == '1' && StockId[1] == '1') ||
			(StockId[0] == '1' && StockId[1] == '2') ||
			(StockId[0] == '1' && StockId[1] == '3') ||
			(StockId[0] == '2'))
		{
			return SHZQ;
		}

		return SHAG;
	}
	else if (MarketType == SZ_MARKET_EX)
	{
		if (StockId[0] == '2' && StockId[1] == '0' && StockId[2] == '0')
		{
			return SZBG;
		}

		return SZAG;
		if (strlen(StockId) == 6)
		{
			if (StockId[0] == '0')
			{
				return SZAG;
			}

			if (StockId[0] == '1')
			{
				if (StockId[1] == '7' || StockId[1] == '8')
				{
					return SZJIJIN;
				}
				else
				{
					return SZZQ;
				}
			}

			if (StockId[0] == '3')
			{
				if (StockId[1] == '9')
				{
					return SZZS;
				}
				else
				{
					return EBAG;
				}
			}
		}
	}

	return -1;
}

CString CSharesInformation::Symbol4To6(CString sIn)
{
		//
	CString Symbol4[]={
		"2A01","2A02","2A03",
		"0696","0896","1990",
		"9905","9906","9907","9908",
		"9911","9912","9913","9914","9915","9916","9917","9918","9919",
		"9920","9921","9922","9923",
		"9931","9932","9933","9934","9935","9936","9937","9938","9939"
	};

	CString Symbol6[]={
		"399001","399002","399003",
		"001696","001896","131990",
		"399305","399106","399107","399108",
		"399110","399120","399130","399140","399150","399160","399170","399180","399190",
		"399200","399210","399220","399230",
		"399131","399132","399133","399134","399135","399136","399137","399138","399139"
	};
	//
	CString sOut = sIn;
	if(strlen(sIn)==4)
	{
		
		for( int i=0;i<sizeof(Symbol4)/sizeof(CString);i++)
		{
			if(Symbol4[i] == sIn )
				return Symbol6[i];
		}
		
	
		if( sIn[0]=='0' )                          
			return  "00"+sIn;

	
		else if( sIn[0]=='4'  )               
		{
			if(sIn[1]=='0' || sIn[1]=='5')
				return  "17"+sIn;
			if(sIn[1]=='6' || sIn[1]=='7')
				return  "18"+sIn;
		}

	
		else if( sIn[0]=='2' && isdigit( sIn[1] ) ) 
		{
			return "200" + sIn.Right(3);
		}

	
		else if(sIn.Left(2) == "16" ||sIn.Left(2) == "17" || sIn.Left(2) == "19" )
			return  "10" + sIn;

	
		else if(sIn.Left(2) == "12" ||sIn.Left(2) == "13" || sIn.Left(2) == "14" ||sIn.Left(2) == "18" ) 
			return  "13" + sIn;

	
		else if(sIn.Left(1) == "5" )  
			return  "12" + sIn;

		
		else if(sIn.Left(1) == "3"||sIn.Left(1) == "7" ||sIn.Left(1) == "8" || sIn.Left(1) == "9") 
		{
			CString ss = "0" + sIn.Left(1) + "0";
			return  ss + sIn.Right (3);
		}

	
		else if(sIn.Left(2) == "10" ) 
			return  "11" + sIn;
	}
	return sOut;
}

void CSharesInformation::AddStocksIfEmpty()
{
	if(this->GetStockTypeCount(0)<=0)
		InitEmptyDatabase();
}

bool CSharesInformation::IsZhiShu(int kind)
{
	bool b = false;
	if( kind==SHZS|| kind==SZZS|| kind==EBZS|| kind==STKTYPE)
		b = true;
	return b;

}

void CSharesInformation::CalcUpDown(int which_stk)
{
	//20021218 b
	{
		int nKind1[] = {SHAG,SHBG,SHJIJIN,SHZQ};
		if(which_stk == 1)
		{
			nKind1[0] = SZAG;
			nKind1[1] = SZBG;
			nKind1[2] = SZJIJIN;
			nKind1[3] = SZZQ;
		}

		ClearUpDown(which_stk);
		for(int i=0;i<4;i++)
		{
			int temp=GetStockTypeCount(nKind1[i]);
			for(int j=0;j<temp;j++)
			{
				CString str;
				CReportData * Cdat;
				GetStockItem(nKind1[i], j, Cdat);
				if(Cdat==NULL)
					continue;
			// skip no use stock
				if (Cdat->ystc>0.001 && Cdat->nowp>0.001)
				{
					float tmp1=(float)(Cdat->nowp-Cdat->ystc);
					if(tmp1>0)
					{
						m_countUp[which_stk][i]++;
					}
					else if(tmp1<0)
					{
						m_countDown[which_stk][i]++;
					}
					else
					{
						m_countEqual[which_stk][i]++;
					}
					m_indexAmount[which_stk][i]+=Cdat->totp ;
				}

			}
		}
	}
	//20021218 e

}

void CSharesInformation::ClearUpDown(int nMarket)
{
	if(nMarket<0 ||nMarket>2)
		return;

		for(int j=0;j<6;j++)
		{
			m_countUp[nMarket][j] =0;
			m_countDown[nMarket][j] = 0;
			m_countEqual[nMarket][j] = 0;
			m_indexAmount[nMarket][j] = 0.0f;
		}

}

float CSharesInformation::GetValueUpDown(int isDown, int whick_stk, int nKind)
{
	if(isDown ==0)
	{
		return (float)m_countUp[whick_stk][nKind];
	}
	else if(isDown ==2)
	{
		return (float)m_countDown[whick_stk][nKind];
	}
	else if(isDown ==1)
	{
		return (float)m_countEqual[whick_stk][nKind];
	}
	else 
	{
		return m_indexAmount[whick_stk][nKind];
	}

}



BOOL CSharesInformation::SetMemroyALLOCSize(DWORD StockType, unsigned int nSize)
{
	STOCKDATASHOW* tempData;
	int temp = 0;
	HGLOBAL	hMem;
	LPVOID hp;

	if (!m_pData[StockType])
	{
		hMem = GlobalAlloc(GPTR, (nSize + ADDCOUNT) * sizeof(STOCKDATASHOW));
		hp = GlobalLock(hMem);
		if (hp)
		{
			m_pData[StockType] = (STOCKDATASHOW*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
			return FALSE;
		}
		m_dwStockMaxCount[StockType] = nSize + ADDCOUNT;
		m_pdwStockCurrentCount[StockType] = 0;
	}
	else
	{
		if (m_dwStockMaxCount[StockType] <= nSize)
		{
			hMem = GlobalAlloc(GPTR, (nSize + ADDCOUNT) * sizeof(STOCKDATASHOW));
			hp = GlobalLock(hMem);
			if (hp)
			{
				tempData = (STOCKDATASHOW*)hp;
			}
			else
			{
				AfxMessageBox("分配内存出错", MB_ICONSTOP);
				return FALSE;
			}
			memcpy(tempData, m_pData[StockType], sizeof(STOCKDATASHOW) * m_pdwStockCurrentCount[StockType]);
			GlobalUnlock((HGLOBAL)m_pData[StockType]);
			GlobalFree((HGLOBAL)m_pData[StockType]);
			m_pData[StockType] = tempData;
			m_dwStockMaxCount[StockType] = nSize + ADDCOUNT;
		}
	}

	return TRUE;
}



BOOL CSharesInformation::InitRealTimeDataExist()
{
	BYTE* temp;
	BOOL IsCorrect = FALSE;

	m_hFile = CreateFile(g_realtime, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开实时行情数据出错");
		return FALSE;
	}

	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (m_hFileMap == NULL)
	{
		AfxMessageBox("创立文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFile = NULL;
		return FALSE;
	}

	m_pbData = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (m_pbData == NULL)
	{
		AfxMessageBox("将文件数据映射入内存时出错");
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		m_hFile = NULL;
		m_hFileMap = NULL;
		return FALSE;
	}

	m_RealFileHead = (REALDATA*)m_pbData;
	if (m_RealFileHead->filetitle != 12345678)
	{
		UnmapViewOfFile(m_pbData);
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		return InitRealTimeDataEmpty();
	}

	if (m_RealFileHead->StockCount + 100 > m_RealFileHead->MaxStockCount)
	{
		int StockTotalCount;
		m_RealFileHead->MaxStockCount += 100;
		StockTotalCount = m_RealFileHead->MaxStockCount;
		SaveRealDataToFile(m_RealFileHead, sizeof(REALDATA));
		UnmapViewOfFile(m_pbData);
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
		m_pbData = NULL;

		long FileLength = 0;
		FileLength = sizeof(REALDATA) + STOCKTYPE * 4 + 240 * 4 * 3 + 240 * 8 * 3 + sizeof(CReportData) * StockTotalCount;
		m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, FileLength, NULL);
		if (m_hFileMap == NULL)
		{
			AfxMessageBox("创立文件映射内核时出错");
			CloseHandle(m_hFile);
			m_hFile = NULL;
			return FALSE;
		}

		m_pbData = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
		if (m_pbData == NULL)
		{
			AfxMessageBox("将文件数据映射入内存时出错");
			CloseHandle(m_hFileMap);
			CloseHandle(m_hFile);
			m_hFile = NULL;
			m_hFileMap = NULL;
			return FALSE;
		}

		m_RealFileHead = (REALDATA*)m_pbData;
	}

	if (m_RealFileHead->FileExitDone != 12345678)
	{
		if (m_RealFileHead->FileExitDone != 88888888)
		{
			int rtn = 5;
			if (rtn == 6)
			{
				UnmapViewOfFile(m_pbData);
				CloseHandle(m_hFileMap);
				CloseHandle(m_hFile);
				return InitRealTimeDataEmpty();
			}
		}

		IsCorrect = TRUE;
	}

	m_RealFileHead->FileExitDone = 87654321;
	temp = m_pbData + sizeof(REALDATA);

	m_pdwStockCurrentCount=(DWORD*)temp;
	temp += sizeof(int) * STOCKTYPE;

	Nidx[0] = (Rsdn1*)temp;
	temp += sizeof(Rsdn1) * 240;
	Nidx[1] = (Rsdn1*)temp;
	temp += sizeof(Rsdn1) * 240;
	Nidx[2] = (Rsdn1*)temp;
	temp += sizeof(Rsdn1) * 240;

	Tidx[0] = (Tidxd*)temp;
	temp += sizeof(Tidxd) * 240;
	Tidx[1] = (Tidxd*)temp;
	temp += sizeof(Tidxd) * 240;
	Tidx[2] = (Tidxd*)temp;
	temp += sizeof(Tidxd) * 240;

	m_pMapData = (CReportData*)temp;

	for (int j = 0; j < STOCKTYPE; j++)
	{
		if (!SetMemroyALLOCSize(j, m_pdwStockCurrentCount[j]))
		{
			AfxMessageBox("初始化数据变量出错");
			return FALSE;
		} 
	}

	if (!IsCorrect)
	{
		for (int j = 0; j < m_RealFileHead->StockCount; j++)
		{
			m_pMapData[j].pBaseInfo = NULL;
			m_pMapData[j].pStockTypeInfo = NULL;
			InsertItemPoint(m_pMapData + j);
		}
	}
	else
	{
		int temp = m_RealFileHead->StockCount;
		m_RealFileHead->StockCount = 0;
		for (int j = 0; j < temp; j++)
		{
			if (strlen((m_pMapData + j)->id) == 6 || strlen((m_pMapData + j)->id) == 4)
			{
				int stocktype = (m_pMapData + j)->kind;
				if (stocktype >= 0 && stocktype <= 10 && (m_pMapData + j)->IsDelete == FALSE)
				{
					InsertItemCorrect((m_pMapData + j)->id, m_pMapData + j, (m_pMapData + j)->kind);
				}
				else
				{
					(m_pMapData+j)->IsDelete = TRUE;
				}
			}
			else
			{
				(m_pMapData + j)->IsDelete = TRUE;
			}
		} 
	}

	SaveRealDataToFile(m_RealFileHead, sizeof(REALDATA));
	return TRUE;
}

BOOL CSharesInformation::Lookup(char* StockId, PCdat1& pStockData, int nKind)
{
	int low = 0;
	int high = m_pdwStockCurrentCount[nKind] - 1;
	int mid = 0;
	while (low <= high)
	{
		mid =(low + high) / 2;
		if (strcmp(m_pData[nKind][mid].StockId, StockId) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pData[nKind][mid].StockId, StockId) < 0)
		{
			low = mid + 1;
		}
		else
		{
			pStockData = m_pData[nKind][mid].pItem;
			return TRUE;
		}
	}

	pStockData = NULL;
	return FALSE;
}

BOOL CSharesInformation::DeleteOneStockInfo(CString strStockCode, int nKind)
{
    return RemoveKey(strStockCode.GetBuffer(0), nKind);
}

BOOL CSharesInformation::RemoveKey(char* StockId, DWORD StockType)
{
	int low = 0;
	int high = m_pdwStockCurrentCount[StockType] - 1;
	int mid = 0;
	int nPos = -1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (strcmp(m_pData[StockType][mid].StockId, StockId) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pData[StockType][mid].StockId, StockId) < 0)
		{
			low = mid + 1;
		}
		else
		{
			nPos = mid;
			break;
		}
	}

	if (nPos == -1)
	{
		return FALSE;
	}

	m_pData[StockType][nPos].pItem->IsDelete = TRUE;
	memmove(&m_pData[StockType][nPos], &m_pData[StockType][nPos + 1], (m_pdwStockCurrentCount[StockType] - nPos - 1) * sizeof(STOCKDATASHOW));
	m_pdwStockCurrentCount[StockType]--;
	SavePosToFile(StockType);
	SaveRealDataToFile(m_RealFileHead, 0);

	return TRUE;
}

void CSharesInformation::MoveStock(CString strStockCode, int nKind, int nNewKind)
{
	char* StockId = strStockCode.GetBuffer(0);

	int low = 0;
	int high = m_pdwStockCurrentCount[nKind] - 1;
	int mid = 0;
	int nPos = -1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (strcmp(m_pData[nKind][mid].StockId, StockId) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pData[nKind][mid].StockId, StockId) < 0)
		{
			low = mid + 1;
		}
		else
		{
			nPos = mid;
			break;
		}
	}

	if (nPos == -1)
	{
		return;
	}

	//m_pData[nKind][nPos].pItem->IsDelete = TRUE;
	CReportData* p = m_pData[nKind][nPos].pItem;
	memmove(&m_pData[nKind][nPos], &m_pData[nKind][nPos + 1], (m_pdwStockCurrentCount[nKind] - nPos - 1) * sizeof(STOCKDATASHOW));
	m_pdwStockCurrentCount[nKind]--;
	SavePosToFile(nKind);

	low = 0;
	high = m_pdwStockCurrentCount[nNewKind] - 1;
	mid = 0;
	int InsertPose = -1;
	while (low <= high)
	{
		mid = (low + high) / 2;
		if (low == high)
		{
			if (strcmp(StockId, m_pData[nNewKind][mid].StockId) > 0)
			{
				if (m_pdwStockCurrentCount[nNewKind] == 0 || m_pdwStockCurrentCount[nNewKind] == mid)
				{
					InsertPose = mid;
				}
				else
				{
					InsertPose = mid + 1;
				}
				break;
			}
			else if (strcmp(StockId, m_pData[nNewKind][mid].StockId) < 0)
			{
				InsertPose = mid;
				break;
			}
			else
			{
				//pStockData = m_pData[StockType][mid].pItem;
				return;
			}
		}

		if (strcmp(m_pData[nNewKind][mid].StockId, StockId) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pData[nNewKind][mid].StockId, StockId) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			//pStockData = m_pData[StockType][mid].pItem;
			return;
		}
	}

	if (high < low)
	{
		InsertPose = low;
	}

	if (m_pdwStockCurrentCount[nNewKind] + 1 > m_dwStockMaxCount[nNewKind])
	{
		SetMemroyALLOCSize(nNewKind, m_pdwStockCurrentCount[nNewKind]);
	}

	if (m_pdwStockCurrentCount[nNewKind] > InsertPose)
	{
		STOCKDATASHOW* ptemp;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc(GPTR, (m_pdwStockCurrentCount[nNewKind] - InsertPose) * sizeof(STOCKDATASHOW));
		hp = GlobalLock(hMem);
		if (hp)
		{
			ptemp = (STOCKDATASHOW*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
			return;
		}

		memcpy(ptemp, &m_pData[nNewKind][InsertPose], (m_pdwStockCurrentCount[nNewKind] - InsertPose) * sizeof(STOCKDATASHOW));
		memcpy(&m_pData[nNewKind][InsertPose + 1], ptemp, (m_pdwStockCurrentCount[nNewKind] - InsertPose) * sizeof(STOCKDATASHOW));
		GlobalUnlock((HGLOBAL)ptemp);
		GlobalFree((HGLOBAL)ptemp);
	}

	p->kind = nNewKind;
	m_pData[nNewKind][InsertPose].pItem = p;
	m_pdwStockCurrentCount[nNewKind]++;
	//SavePosToFile(nNewKind);
	SaveRealDataToFile(m_RealFileHead, sizeof(REALDATA) + sizeof(long) * STOCKTYPE);
	SaveRealDataToFile(m_pData[nNewKind][InsertPose].pItem, sizeof(CReportData));
}

BOOL CSharesInformation::RecvStockDataForType(PSTOCKDATASHOW& p, BYTE StockType)
{
	if (m_hFile == NULL || m_hFileMap == NULL)
		return FALSE;

	memcpy(p, m_pData[StockType], m_pdwStockCurrentCount[StockType] * sizeof(STOCKDATASHOW));

	return TRUE;
}
