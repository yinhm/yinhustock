
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "StkBlockManager.h"

#include "CTaiKlineFileKLine.h"
#include "StkDatabase.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"

#include <share.h>
#include <io.h>
#include "StructTaiShares.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MaxRights		3			// 
#define STOCKADDCOUNT	10			// 

const char g_stocktypeinfo[64] = "Data\\Block.tsk";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStkBlockManager::CStkBlockManager()
{
	m_hFile = NULL;
	m_hFileMap = NULL;
	m_lpvFileBegin = NULL;

	m_pStockTypeHead = NULL;
	m_pStockTypeInfo = NULL;
	m_pStockInfo = NULL;
	m_pStockPoint = NULL;
}

CStkBlockManager::~CStkBlockManager()
{
	BOOL result = TRUE;

	if (m_hFile)
	{
		result = RemoveDeletedStock();

	}

	if (result)
	{
		m_pStockTypeHead->m_lFileExit = 12345678;
	}
	else
	{
		m_pStockTypeHead->m_lFileExit = 87654321;
	}

	if (m_lpvFileBegin)
		UnmapViewOfFile(m_lpvFileBegin);
	if (m_hFileMap) 
		CloseHandle(m_hFileMap);
	if (m_hFile)
		CloseHandle(m_hFile);

	if (m_pStockPoint)
	{
		GlobalUnlock((HGLOBAL)m_pStockPoint);
		GlobalFree((HGLOBAL)m_pStockPoint);
	}
}

BOOL CStkBlockManager::InitStockTypeDataEmpty(CString strPath)
{
	BYTE* temp;

	long nFileLength = sizeof(STOCKTYPEHEAD) + MaxStockTYpe * sizeof(STOCKTYPEINFO) + 4096 * sizeof(STOCK_TYPE_INFO) + 1024 * 2;

	if (CStkFile::Open(g_stocktypeinfo, nFileLength, _T("")) == FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pStockTypeHead = (STOCKTYPEHEAD*)m_lpvFileBegin;
	m_pStockTypeHead->m_lFileTitle = 12345678;
	m_pStockTypeHead->m_lFileExit = 87654321;
	m_pStockTypeHead->m_lStockTypeCount = 0;
	m_pStockTypeHead->m_lStockTypeMaxCount = MaxStockTYpe;
	m_pStockTypeHead->m_lStockCount = 0;
	m_pStockTypeHead->m_lStockMaxCount = 4096;

	SaveDataToFile(m_pStockTypeHead, sizeof(STOCKTYPEHEAD));

	temp = m_lpvFileBegin + sizeof(STOCKTYPEHEAD);
	m_pStockTypeInfo = (STOCKTYPEINFO*)temp;
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		m_pStockTypeInfo[i].m_lIsUse = -1;
	}

	temp += sizeof(STOCKTYPEINFO) * MaxStockTYpe;
	m_pStockInfo = (STOCK_TYPE_INFO*)temp;

	if (!SetMemroyALLOCSize(m_pStockTypeHead->m_lStockMaxCount))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL CStkBlockManager::InitStockTypeDataExist(CString strPath)
{
	BYTE* temp;
	BOOL IsCorrect = FALSE;

	if (CStkFile::Open(g_stocktypeinfo, 0, _T("")) == FALSE)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pStockTypeHead = (STOCKTYPEHEAD*)m_lpvFileBegin;

	if (!((m_pStockTypeHead->m_lFileTitle == 12345678) || (m_pStockTypeHead->m_lFileTitle == 55555555)))
	{
		UnmapViewOfFile(m_lpvFileBegin);
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		return InitStockTypeDataEmpty(strPath);
	}

	if (m_pStockTypeHead->m_lFileExit != 12345678)
	{
		IsCorrect = TRUE;
	}

	m_pStockTypeHead->m_lFileExit = 87654321;

	temp = m_lpvFileBegin + sizeof(STOCKTYPEHEAD);
	m_pStockTypeInfo = (STOCKTYPEINFO*)temp;

	temp += sizeof(STOCKTYPEINFO) * MaxStockTYpe;
	m_pStockInfo = (STOCK_TYPE_INFO*)temp;

	if (!SetMemroyALLOCSize(m_pStockTypeHead->m_lStockMaxCount))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (!IsCorrect)
	{
		for (int j = 0; j < m_pStockTypeHead->m_lStockCount; j++)
		{
			InsertItemPoint(m_pStockInfo + j);
		}
	}
	else
	{
		int temp = m_pStockTypeHead->m_lStockCount;
		m_pStockTypeHead->m_lStockCount = 0;
		for (int j = 0; j < temp; j++)
		{
			InsertStockItemCorrect((m_pStockInfo + j)->m_szSymbol, m_pStockInfo + j);
		}
	}

	SaveDataToFile(m_pStockTypeHead, sizeof(STOCKTYPEHEAD));

	return TRUE;
}

BOOL CStkBlockManager::AddStockTypeDataSize(int AddSizes)
{
	BYTE* temp;

	if (m_pStockTypeHead->m_lStockCount + AddSizes >= m_pStockTypeHead->m_lStockMaxCount)
	{
		long nFileLength = sizeof(STOCKTYPEHEAD) + MaxStockTYpe * sizeof(STOCKTYPEINFO) + (m_pStockTypeHead->m_lStockCount + AddSizes) * sizeof(STOCK_TYPE_INFO) + 1024 * 2;
		m_pStockTypeHead->m_lStockMaxCount = m_pStockTypeHead->m_lStockCount + AddSizes;

		SavePosToFile();

		if (m_lpvFileBegin)
			UnmapViewOfFile(m_lpvFileBegin);
		if (m_hFileMap) 
			CloseHandle(m_hFileMap);

		m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, nFileLength, NULL);
		if (m_hFileMap == NULL)
		{
			ASSERT(FALSE);
			CloseHandle(m_hFile);
			m_hFile = NULL;
			m_hFileMap = NULL;
			return FALSE;
		}

		m_lpvFileBegin = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
		if (m_lpvFileBegin == NULL)
		{
			ASSERT(FALSE);
			CloseHandle(m_hFileMap);
			CloseHandle(m_hFile);
			m_hFile = NULL;
			m_hFileMap = NULL;
			return FALSE;
		}

		m_pStockTypeHead = (STOCKTYPEHEAD*)m_lpvFileBegin;

		temp = m_lpvFileBegin + sizeof(STOCKTYPEHEAD);
		m_pStockTypeInfo = (STOCKTYPEINFO*)temp;

		temp += sizeof(STOCKTYPEINFO) * MaxStockTYpe;
		m_pStockInfo = (STOCK_TYPE_INFO*)temp;

		if (!SetMemroyALLOCSize(m_pStockTypeHead->m_lStockMaxCount))
		{
			ASSERT(FALSE);
			return FALSE;
		}

		for (int j = 0; j < m_pStockTypeHead->m_lStockCount; j++)
		{
			InsertItemPoint(m_pStockInfo + j);
		}

		InitStockPoint();
	}

	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::InsertItemPoint(STOCK_TYPE_INFO* m_pStk)
{
	int InsertPose = m_pStk->m_iPos;
	m_pStockPoint[InsertPose].pStockInfo = m_pStk;

	return TRUE;
}

BOOL CStkBlockManager::SavePosToFile()
{
	for (int j = 0; j < m_pStockTypeHead->m_lStockCount; j++)
	{
		if (m_pStockPoint[j].pStockInfo == NULL)
		{
			return FALSE;
		}

		m_pStockPoint[j].pStockInfo->m_iPos = j;
	}

	return TRUE;
}

BOOL CStkBlockManager::SetMemroyALLOCSize(unsigned int nSize)
{
	STOCK_POINT_INFO* tempData;
	int temp = 0;
	HGLOBAL	hMem;
	LPVOID hp;

	if (!m_pStockPoint)
	{
		hMem = GlobalAlloc(GPTR, (nSize) * sizeof(STOCK_POINT_INFO));
		hp = GlobalLock(hMem);
		if (hp)
		{
			m_pStockPoint = (STOCK_POINT_INFO*)hp;
		}
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	else
	{
		hMem = GlobalAlloc(GPTR, (nSize) * sizeof(STOCK_POINT_INFO));
		hp = GlobalLock(hMem);
		if (hp)
		{
			tempData = (STOCK_POINT_INFO*)hp;
		}
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}

		memcpy(tempData, m_pStockPoint, sizeof(STOCK_POINT_INFO) * m_pStockTypeHead->m_lStockCount);
		GlobalUnlock((HGLOBAL)m_pStockPoint);
		GlobalFree((HGLOBAL)m_pStockPoint);
		m_pStockPoint = tempData;
	}

	return TRUE;
}

BOOL CStkBlockManager::InitStockTypeData(CString strPath)
{
	m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;

	if (_access(g_stocktypeinfo, 0) == -1)
	{
		InitStockTypeDataEmpty(strPath);
	}
	else
	{
		InitStockTypeDataExist(strPath);
	}

	return TRUE;
}

BOOL CStkBlockManager::InitStockPoint()
{
	for (int j = 0; j < m_pStockTypeHead->m_lStockCount; j++)
	{
		CReportData* pCdat;
		CString m_strStockKind = m_pStockInfo[j].m_szSymbol;
		char m_szStockCode[10];
		int nKind;
		strcpy(m_szStockCode, m_strStockKind.Right(m_strStockKind.GetLength() - 2).GetBuffer(0));
		nKind = m_pDoc->GetStockKind(m_strStockKind.Left(2));
		if (nKind < 0)
		{
			continue;
		}

		if (m_pDoc->m_sharesInformation.Lookup(m_szStockCode, pCdat, nKind))
		{
			pCdat->pStockTypeInfo = &m_pStockInfo[j];
		}
	}

	return TRUE;
}

BOOL CStkBlockManager::InitStockTypePoint()
{
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		if (m_pStockTypeInfo[i].m_bIsDelete || m_pStockTypeInfo[i].m_lIsUse == -1)
			continue;

		CReportData* pCdat;
		if (!m_pDoc->m_sharesInformation.Lookup(m_pStockTypeInfo[i].m_szCode, pCdat, STKTYPE))
		{
			m_pDoc->m_sharesInformation.InsertItem(m_pStockTypeInfo[i].m_szCode, pCdat, STKTYPE);
			strcpy(pCdat->name, m_pStockTypeInfo[i].m_szName);
			strcpy(pCdat->id, m_pStockTypeInfo[i].m_szCode);
			pCdat->kind = STKTYPE;

			if (strlen(pCdat->name) > 0)
				m_pDoc->StockNameConvert(pCdat->name, pCdat->Gppyjc);

			m_pDoc->AddStockToKeyboard(pCdat->id, pCdat->kind, pCdat->name, pCdat->Gppyjc);
		}
	}

	return TRUE;
}

void CStkBlockManager::CalcAllStockTypeData()         
{
	for(int i=0;i<MaxStockTYpe;i++)    
	{
		if(m_pStockTypeInfo[i].m_lIsUse==-1)
			break;
		if(m_pStockTypeInfo[i].m_bIsDelete )
			continue;
		m_pStockTypeInfo[i].m_fOpenIndex=0;
		m_pStockTypeInfo[i].m_fYstIndex=0;
		m_pStockTypeInfo[i].m_fTotalRight=0;
	}
	for(int i=0;i<m_pDoc->m_sharesInformation.GetTotalStockTypeCount();i++)   
	{
		if(!CheckKind( i))
			continue;
		float fTotal = 0;
		float fLtp = 0;
		for(int j=0;j<m_pDoc->m_sharesInformation.GetStockTypeCount(i);j++)
		{
			CReportData *pCdat;
			int index=0;
			m_pDoc->m_sharesInformation.GetStockItem(i,j,pCdat);
			if(pCdat==NULL||pCdat->pStockTypeInfo==NULL)
				continue;



			while(pCdat->pStockTypeInfo->m_btStockType[index]!=255&&index<MaxStockTYpe)
			{
				BYTE l_StockTyep=pCdat->pStockTypeInfo->m_btStockType[index];
				switch(m_pStockTypeInfo[l_StockTyep].m_iRightType)
				{
				case ZGB: if(pCdat->opnp!=0&&pCdat->ystc!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->opnp; 
							  m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->pStockTypeInfo->m_ClosePrice;
						  }else if(pCdat->ystc!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->pStockTypeInfo->m_ClosePrice;
						  }else
						  {
							  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
							  m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->pStockTypeInfo->m_ClosePrice;
						  }
						  break; 
				case LTG: if(pCdat->opnp!=0&&pCdat->ystc!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->opnp; 
							  m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->pStockTypeInfo->m_ClosePrice;
						  }
						  else if(pCdat->ystc!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->pStockTypeInfo->m_ClosePrice;
						  }else
						  {
							  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
							  m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->pStockTypeInfo->m_ClosePrice;
						  }
						  break; 
				case OTHER: if(pCdat->opnp!=0&&pCdat->ystc!=0)
							{
								m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->opnp; 
								m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
							}
							else if(pCdat->ystc!=0)
							{
								m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
							}
							else
							{
								pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								m_pStockTypeInfo[l_StockTyep].m_fOpenIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fYstIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
							}
							break; 
				}
				index++;
			}

		}
		//20021218 b
		BASEINFO baseinfo;
		{


		}

	}
	for(int i=0;i<m_pDoc->m_sharesInformation.GetStockTypeCount(STKTYPE);i++)  
	{
		CReportData *pCdat;
		PSTOCKTYPEINFO m_pStktype;
		m_pDoc->m_sharesInformation.GetStockItem(STKTYPE,i,pCdat);
		if(pCdat==NULL)
			continue;
		GetStockTypePoint(m_pStktype,pCdat->id);
		if(m_pStktype->m_fTotalRight==0)
			continue;
		pCdat->ystc=m_pStktype->m_fYstIndex/m_pStktype->m_fTotalRight*1000;
		if(pCdat->lowp>pCdat->ystc)
			pCdat->lowp=pCdat->ystc;
		if(pCdat->higp<pCdat->ystc)
			pCdat->higp=pCdat->ystc;
		pCdat->opnp=m_pStktype->m_fOpenIndex/m_pStktype->m_fTotalRight*1000;
		if(pCdat->lowp>pCdat->opnp)
			pCdat->lowp=pCdat->opnp;
		if(pCdat->higp<pCdat->opnp)
			pCdat->higp=pCdat->opnp;
	}
}

void CStkBlockManager::CalcRealTimeStockTypeIndex()      
{
	if(!m_pDoc-> m_bInitDone)
		return ;
	for(int i=0;i<MaxStockTYpe;i++)   
	{
		if(m_pStockTypeInfo[i].m_lIsUse==-1 )
			break;
		if(m_pStockTypeInfo[i].m_bIsDelete)
			continue;
		m_pStockTypeInfo[i].m_fNewIndex=0;
		m_pStockTypeInfo[i].m_fTotalRight=0;
		m_pStockTypeInfo[i].m_fTotP=0;     
		m_pStockTypeInfo[i].m_fTotV=0;        
		m_pStockTypeInfo[i].m_fTotRVol=0;     
		m_pStockTypeInfo[i].m_fTotDVol=0;    

	}
	for(int i=0;i<m_pDoc->m_sharesInformation.GetTotalStockTypeCount();i++)     
	{
		if(!CheckKind( i))
			continue;
		for(int j=0;j<m_pDoc->m_sharesInformation.GetStockTypeCount(i);j++)
		{
			CReportData *pCdat;
			int index=0;
			m_pDoc->m_sharesInformation.GetStockItem(i,j,pCdat);
			if(pCdat==NULL)
				continue; 
			if(pCdat->pStockTypeInfo==NULL)
				continue;
			while(pCdat->pStockTypeInfo->m_btStockType[index]!=255&&index<MaxStockTYpe)   
			{
				BYTE l_StockTyep=pCdat->pStockTypeInfo->m_btStockType[index];
				switch(m_pStockTypeInfo[l_StockTyep].m_iRightType)
				{
				case ZGB: if(pCdat->nowp!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->nowp; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];
						  }
						  else if(pCdat->ystc!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];
						  }
						  else
						  {
							  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
							  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];
						  }
						  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->totp;        
						  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->totv; 							
						  m_pStockTypeInfo[l_StockTyep].m_fTotRVol=pCdat->rvol;     
						  m_pStockTypeInfo[l_StockTyep].m_fTotDVol=pCdat->dvol;     
						  break; 
				case LTG: if(pCdat->nowp!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->nowp;
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[LTG];
						  }
						  else if(pCdat->ystc!=0)
						  {
							  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[LTG];
						  }else
						  {
							  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
							  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
							  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];
						  }
						  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->totp;       
						  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->totv;         
						  m_pStockTypeInfo[l_StockTyep].m_fTotRVol=pCdat->rvol;      
						  m_pStockTypeInfo[l_StockTyep].m_fTotDVol=pCdat->dvol;     
						  break; 
				case OTHER: if(pCdat->nowp!=0)
							{
								m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->nowp;
								m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
							}
							else if(pCdat->ystc!=0)
							{
								m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
							} 
							else
							{
								pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
								m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
							}
							m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->totp;      
							m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->totv;     
							m_pStockTypeInfo[l_StockTyep].m_fTotRVol=pCdat->rvol;     
							m_pStockTypeInfo[l_StockTyep].m_fTotDVol=pCdat->dvol;      
							break; 
				}
				index++;
			}
		}
	}
	for(int i=0;i<m_pDoc->m_sharesInformation.GetStockTypeCount(STKTYPE);i++)  
	{
		CReportData *pCdat;
		PSTOCKTYPEINFO m_pStktype;
		m_pDoc->m_sharesInformation.GetStockItem(STKTYPE,i,pCdat);
		if(pCdat==NULL)
			return;
		GetStockTypePoint(m_pStktype,pCdat->id);
		if(m_pStktype->m_fTotalRight==0)
			continue;
		if(pCdat->opnp==0||pCdat->ystc==0)       
			CalcAllStockTypeData();

		pCdat->nowp=m_pStktype->m_fNewIndex/m_pStktype->m_fTotalRight*1000;
		pCdat->nowv=m_pStktype->m_fTotV - pCdat->totv;             

		pCdat->totp=m_pStktype->m_fTotP;
		pCdat->totv=m_pStktype->m_fTotV;

		pCdat->rvol=m_pStktype->m_fTotRVol;                       
		pCdat->dvol=m_pStktype->m_fTotRVol;                      

		pCdat->m_Kdata1[m_pDoc->m_nANT[0]].Price=pCdat->nowp;
		pCdat->m_Kdata1[m_pDoc->m_nANT[0]].Volume=pCdat->totv;     
		pCdat->m_Kdata1[m_pDoc->m_nANT[0]].Amount=pCdat->totp;     

		if(pCdat->nowp>pCdat->higp||pCdat->higp==0)
			pCdat->higp=pCdat->nowp;
		if(pCdat->nowp<pCdat->lowp||pCdat->lowp==0)
			pCdat->lowp=pCdat->nowp;
	}
	m_pStockTypeHead->m_lLastTime=m_pDoc->m_nANT[0];
}

void CStkBlockManager::CalcMinuteRealTimeIndex()     
{
	for(int t=0;t<m_pDoc->m_nANT[0];t++) 
	{
		for(int i=0;i<MaxStockTYpe;i++)       
		{
			if(m_pStockTypeInfo[i].m_lIsUse==-1 )
				break;
			if(m_pStockTypeInfo[i].m_bIsDelete)
				continue;

			m_pStockTypeInfo[i].m_fNewIndex=0;
			m_pStockTypeInfo[i].m_fTotP=0;         
			m_pStockTypeInfo[i].m_fTotV=0;         
			m_pStockTypeInfo[i].m_fTotalRight=0;
		}
		for(int i=0;i<m_pDoc->m_sharesInformation.GetTotalStockTypeCount();i++)  
		{
			if(!CheckKind( i))
				continue;
			for(int j=0;j<m_pDoc->m_sharesInformation.GetStockTypeCount(i);j++)
			{
				CReportData *pCdat;
				int index=0;
				m_pDoc->m_sharesInformation.GetStockItem(i,j,pCdat);
				if(pCdat==NULL||pCdat->pStockTypeInfo==NULL)
					continue;
				while(pCdat->pStockTypeInfo->m_btStockType[index]!=255&&index<MaxStockTYpe)  
				{
					BYTE l_StockTyep=pCdat->pStockTypeInfo->m_btStockType[index];
					switch(m_pStockTypeInfo[l_StockTyep].m_iRightType)
					{
					case ZGB: if(pCdat->m_Kdata1[t].Price!=0)
							  {
								  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->m_Kdata1[t].Price; 
								  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;       
								  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;     
								  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];
							  }
							  else if(pCdat->ystc!=0)
							  {
								  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
								  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;        
								  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;        
								  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];
							  }
							  else
							  {
								  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pCdat->ystc; 
								  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;        
								  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         
								  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[ZGB];

							  }
							  break; 
					case LTG: if(pCdat->m_Kdata1[t].Price!=0)
							  {
								  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->m_Kdata1[t].Price;
								  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         
								  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;       
								  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[LTG];
							  }
							  else if(pCdat->ystc!=0)
							  {
								  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
								  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         
								  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;   
								  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[LTG];
							  }
							  else
							  {
								  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								  m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pCdat->ystc; 
								  m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;      
								  m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;        
								  m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice*pCdat->pStockTypeInfo->m_fRight[LTG];

							  }
							  break; 
					case OTHER: if(pCdat->m_Kdata1[t].Price!=0)
								{
									m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->m_Kdata1[t].Price;
									m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;        
									m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;     
									m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
								}
								else if(pCdat->ystc!=0)
								{
									m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
									m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;       
									m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;        
									m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
								}
								else
								{
									pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
									m_pStockTypeInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
									m_pStockTypeInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;      
									m_pStockTypeInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;        
									m_pStockTypeInfo[l_StockTyep].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;

								}
								break; 
					}
					index++;
				}
			}
		}
		for(int i=0;i<m_pDoc->m_sharesInformation.GetStockTypeCount(STKTYPE);i++)  
		{
			CReportData *pCdat;
			PSTOCKTYPEINFO m_pStktype;
			m_pDoc->m_sharesInformation.GetStockItem(STKTYPE,i,pCdat);
			if(pCdat==NULL)
				continue;
			GetStockTypePoint(m_pStktype,pCdat->id);
			if(m_pStktype->m_fTotalRight==0)
				continue;

			pCdat->m_Kdata1[t].Price=m_pStktype->m_fNewIndex/m_pStktype->m_fTotalRight*1000;
			pCdat->m_Kdata1[t].Volume=m_pStktype->m_fTotV;    
			pCdat->m_Kdata1[t].Amount=m_pStktype->m_fTotP;    

			if(t==0)
				pCdat->higp=pCdat->lowp=pCdat->m_Kdata1[t].Price;
			else
			{
				if(pCdat->m_Kdata1[t].Price>pCdat->higp)
					pCdat->higp=pCdat->m_Kdata1[t].Price;
				if(pCdat->m_Kdata1[t].Price<pCdat->lowp)
					pCdat->lowp=pCdat->m_Kdata1[t].Price;
			}
		}
	}
}

void CStkBlockManager::CheckStockCount(int AddStockCount)
{
	if(m_pStockTypeHead->m_lStockCount+AddStockCount>=m_pStockTypeHead->m_lStockMaxCount)
	{
		AddStockTypeDataSize(AddStockCount+50);
	}

}


BOOL CStkBlockManager::ImportStockTypeData(CString m_szPathStr, CString m_szStockTyoeNameStr, PSTOCKTYPEINFO& l_pStockkType, int nRightType)
{
	if (GetStockTypePointFromName(l_pStockkType, m_szStockTyoeNameStr.GetBuffer(0)))
	{
		DeleteStockType(l_pStockkType->m_szCode);
		CReportData* pCdat;
		if (m_pDoc->m_sharesInformation.Lookup(l_pStockkType->m_szCode, pCdat, STKTYPE))
		{
			m_pDoc->DeleteStockFromKeyboard(pCdat->id, pCdat->kind, pCdat->name, pCdat->Gppyjc);
			m_pDoc->m_sharesInformation.DeleteOneStockInfo(l_pStockkType->m_szCode, STKTYPE);
		}
	}

	int l_nStockTypePos = GetInsertStockTypePos();
	if (!InsertStockType(l_pStockkType, l_nStockTypePos))
	{
		return FALSE;
	}

	char temp[3];

	l_pStockkType->m_lIsUse = TRUE;
	l_pStockkType->m_bIsDelete = FALSE;
	strcpy(l_pStockkType->m_szName, m_szStockTyoeNameStr);
	l_pStockkType->m_lStockCount = 0;
	l_pStockkType->m_fTotalRight = 0;
	sprintf(temp, "%2d", l_nStockTypePos);

	for (int i = 0; i < 2; i++)
	{
		if (temp[i] == ' ')
			temp[i] = '0';
	}

	sprintf(l_pStockkType->m_szCode, "%s%s", "8K00", temp);
	l_pStockkType->m_iRightType = nRightType;


	int stocklength;
	char blank[2] = { ' ', ' ' };
	CFile stockfile;
	CString filepath;
	filepath.Format("%s%s%s", m_szPathStr.GetBuffer(0), "\\", m_szStockTyoeNameStr.GetBuffer(0));

	if (stockfile.Open((LPCTSTR)filepath, CFile::modeReadWrite))
	{
		stockfile.Read(&stocklength, 4);
		for (int j = 0; j < stocklength; j++)
		{
			CReportData* l_pCdat;
			SharesSymbol* symbol = new SharesSymbol;
			stockfile.Read(symbol->sharesymbol, 6);
			stockfile.Read(&symbol->nKind, 4);
			symbol->sharesymbol[6] = '\0';
			stockfile.Read(&blank, 2);
			if (m_pDoc->m_sharesInformation.Lookup(symbol->sharesymbol, l_pCdat, symbol->nKind))
			{
				if (l_pCdat->pBaseInfo != NULL)
				{
					InsertStockToType(l_pCdat, l_pStockkType->m_szCode);
				}
			}
			delete symbol;
		}
		stockfile.Close();
	}

	return TRUE;
}





///////////////////////////////

//****************************************************
BOOL CStkBlockManager::GetStockTypeStartDate(char * pszStockTypeCode,SymbolKindArr& StockCodeArray,time_t &t)
{
	int FirstKlineData=0;
	if(!GetStockFromStockType(StockCodeArray,pszStockTypeCode))          
	{
		AfxMessageBox("该板块不存在") ;
		return FALSE;
	}
	int StockCount=StockCodeArray.GetSize();
	for(int i=0;i<StockCount;i++)
	{
		SymbolKind StockSymbol=StockCodeArray.GetAt(i);
		FirstKlineData=CTaiKlineFileKLine::GetFirstKline(StockSymbol.m_chSymbol,StockSymbol.m_nSymbolKind ,TRUE);
		if(i==0)
			t= FirstKlineData;
		if(FirstKlineData!=-1&&t>FirstKlineData)
			t= FirstKlineData;
	}
	return TRUE;    
}
void CStkBlockManager::RefreshHistoryDayLineData(char * pszStockTypeCode,CProgressCtrl *pProgressCtrl)
{
	typedef struct
	{
		time_t m_date;
		float m_fOpenIndex;
		float m_fHighIndex;    
		float m_fLowIndex;    
		float m_fCloseIndex;    
		float m_fTotV;         
		float m_fTotP;       
		float m_fTotalRight;
		long ldate;
	}StockTypeInfoTemp;
	MSG message;
	time_t t;
	SymbolKindArr StockCodeArray; 
	STOCKTYPEINFO *m_pStktype;
	CTimeSpan  lDaySpan(1, 0,0, 0 );
	if(!GetStockTypeStartDate(pszStockTypeCode,StockCodeArray,t))
		return;
	GetStockTypePoint(m_pStktype,pszStockTypeCode);
	t+=lDaySpan.GetTotalSeconds();
	//********************************************
	Kline *pKline=NULL;
	int nTotalCount;
	CBuySellList *pBuySellList=NULL;
	CString SymbolCode;
	int nRead=0;
	CTime *tmEnd=NULL;
	CTime *tmBegin=NULL;
	int nKlineType=5;
	time_t now; 
	time( &now );

	SymbolCode=CSharesCompute::GetIndexSymbol(SHZS);
	tmEnd=new CTime(now);
	tmBegin=new CTime(t);

	nTotalCount=CTaiKlineFileKLine::ReadKLineAny(SymbolCode,SHZS,pKline, nRead,nKlineType, pBuySellList, 48,TRUE,  tmEnd,tmBegin);
	nTotalCount=CorrectKline(pKline,nTotalCount);
	if(nTotalCount<0||nTotalCount<-1)
	{
		if(tmEnd) delete tmEnd;
		if(tmBegin) delete tmBegin;
		return;
	}
	//****************************************************************************************
	StockTypeInfoTemp *pStockTypeInfoTemp=NULL;
	HGLOBAL hMem=GlobalAlloc(  GPTR, sizeof(StockTypeInfoTemp)*nTotalCount);
	if(hMem!=NULL)
		pStockTypeInfoTemp=(StockTypeInfoTemp *)GlobalLock(  hMem );
	else
	{
		if(tmEnd) delete tmEnd;
		if(tmBegin) delete tmBegin;
		AfxMessageBox("内存分配出错!");
		return;
	}
	for(int i=0;i<nTotalCount;i++)
	{
		pStockTypeInfoTemp[i].m_date=pKline[i].day;
		pStockTypeInfoTemp[i].ldate=m_pDoc->GetStockDay(pKline[i].day);
	}
	if(pKline)
	{
		delete pKline;        
		pKline=NULL;
	}
	delete tmEnd;
	delete tmBegin;
	//**************************************************************
	if(pProgressCtrl!=NULL)
	{
		pProgressCtrl->SetRange(0,StockCodeArray.GetSize());
		pProgressCtrl->SetPos(0);
	}
	for(int j=0;j<StockCodeArray.GetSize();j++)
	{
		CReportData *pCdat;
		Kline *pStockKline=NULL;
		SymbolKind StockSymbol=StockCodeArray.GetAt(j);
		long StockTotalCount=0;
		if(!m_pDoc->m_sharesInformation.Lookup(StockSymbol.m_chSymbol,pCdat,StockSymbol.m_nSymbolKind))
			continue;
		if(pCdat->pStockTypeInfo==NULL)
			continue;
		StockTotalCount=CTaiKlineFileKLine::ReadKLineAny(StockSymbol.m_chSymbol,StockSymbol.m_nSymbolKind,pStockKline, -1,5, NULL, 48,TRUE,  NULL,NULL);
		StockTotalCount=CorrectKline(pStockKline,StockTotalCount);

		if(StockTotalCount<=0)
			continue; 
		int index=0;
		for(int i=1;i<StockTotalCount&&index<nTotalCount;i++,index++)
		{
			long lDate;
			if(i==1)
			{
				lDate=m_pDoc->GetStockDay(pStockKline[i].day);
				if(lDate>pStockTypeInfoTemp[index].ldate)
					while(index<nTotalCount&&lDate>pStockTypeInfoTemp[index].ldate) index++;
				else if(lDate<pStockTypeInfoTemp[index].ldate)
			 {
				 while(i<StockTotalCount&&lDate<pStockTypeInfoTemp[index].ldate)
				 {
					 lDate=m_pDoc->GetStockDay(pStockKline[i].day);
					 i++;
				 }
			 }
				if(i>=StockTotalCount-1||index>=nTotalCount)
					continue;
			}
			else
			{
				lDate=m_pDoc->GetStockDay(pStockKline[i].day);
				while(i<StockTotalCount&&pStockTypeInfoTemp[index].ldate>lDate) 
			 {
				 lDate=m_pDoc->GetStockDay(pStockKline[i].day);
				 i++;
			 }
				while(index<nTotalCount&&pStockTypeInfoTemp[index].ldate<lDate)     
			 {
				 switch(m_pStktype->m_iRightType)
				 {
				 case ZGB: 
					 pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i-1].open;
					 pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i-1].high;
					 pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i-1].low; 
					 pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i-1].close;
					 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pStockTypeInfo->m_ClosePrice;
					 break; 
				 case LTG: 
					 pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i-1].open;
					 pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i-1].high;
					 pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i-1].low; 
					 pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i-1].close;
					 if(pCdat->kind==SHBG||pCdat->kind==SZBG)
						 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pStockTypeInfo->m_ClosePrice;
					 else
						 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pStockTypeInfo->m_ClosePrice;
					 break; 
				 case OTHER:
					 pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[i-1].open;
					 pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[i-1].high;
					 pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[i-1].low; 
					 pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[i-1].close;
					 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;

					 break; 
				 }
				 pStockTypeInfoTemp[index].m_fTotP +=pStockKline[i-1].amount; 
				 pStockTypeInfoTemp[index].m_fTotV +=pStockKline[i-1].vol;    
				 index++; 
			 } 
			}
			if(i>=StockTotalCount||index>=nTotalCount)
				continue;
			if(pStockTypeInfoTemp[index].ldate!=lDate)
		 {
			 index--;
			 i--;
			 continue;
		 }
			switch(m_pStktype->m_iRightType)
		 {
			case ZGB: 
				pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i].open;
				pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i].high;
				pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i].low; 
				pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[i].close;
				pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pStockTypeInfo->m_ClosePrice;
				break; 
			case LTG: 
				pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i].open;
				pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i].high;
				pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i].low; 
				pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[i].close;
				if(CTaiShanReportView::IsBg( pCdat->kind))
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pStockTypeInfo->m_ClosePrice;
				else
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pStockTypeInfo->m_ClosePrice;
				break; 
			case OTHER:
				pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[i].open;
				pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[i].high;
				pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[i].low; 
				pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[i].close;
				pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;
				break; 
		 }
			pStockTypeInfoTemp[index].m_fTotP +=pStockKline[i].amount; 
			pStockTypeInfoTemp[index].m_fTotV +=pStockKline[i].vol;    
		}
		//*******************************************************************************
		long lDate;
		lDate=m_pDoc->GetStockDay(pStockKline[StockTotalCount-1].day);
		while(index<nTotalCount&&lDate<=pStockTypeInfoTemp[index].ldate)       
		{
			switch(m_pStktype->m_iRightType)
		 {
			case ZGB: 
				pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].open;
				pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].high;
				pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].low; 
				pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pStockTypeInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].close;
				pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pStockTypeInfo->m_ClosePrice;
				break; 
			case LTG: 
				pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].open;
				pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].high;
				pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].low; 
				pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pStockTypeInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].close;
				if(CTaiShanReportView::IsBg( pCdat->kind))
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pStockTypeInfo->m_ClosePrice;
				else
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pStockTypeInfo->m_ClosePrice;
				break; 
			case OTHER:
				pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[StockTotalCount-1].open;
				pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[StockTotalCount-1].high;
				pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[StockTotalCount-1].low; 
				pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[StockTotalCount-1].close;
				pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pStockTypeInfo->m_ClosePrice;

				break; 
		 }
			pStockTypeInfoTemp[index].m_fTotP +=pStockKline[StockTotalCount-1].amount; 
			pStockTypeInfoTemp[index].m_fTotV +=pStockKline[StockTotalCount-1].vol;    
			index++;
		}
		//*******************************************************************************
		if(pStockKline)
			delete pStockKline;
		if(pProgressCtrl!=NULL)
		{
			pProgressCtrl->SetPos(j);
		}
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	if(pProgressCtrl!=NULL)
	{
		pProgressCtrl->SetPos(0);
	}

	hMem=GlobalAlloc(  GPTR, sizeof(Kline)*nTotalCount);
	if(hMem!=NULL)
		pKline=(Kline *)GlobalLock(  hMem );
	else
	{
		AfxMessageBox("内存分配出错!");
		return;
	}
	for(int i=0;i<nTotalCount;i++)
	{
		pKline[i].day=pStockTypeInfoTemp[i].m_date;
		pKline[i].open=pStockTypeInfoTemp[i].m_fOpenIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].high=pStockTypeInfoTemp[i].m_fHighIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].low=pStockTypeInfoTemp[i].m_fLowIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].close=pStockTypeInfoTemp[i].m_fCloseIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].amount=pStockTypeInfoTemp[i].m_fTotP;
		pKline[i].vol=pStockTypeInfoTemp[i].m_fTotV;
	}
	CString strStockTypeCode=pszStockTypeCode;
	CTaiKlineFileKLine::ZeroKlineData(strStockTypeCode,STKTYPE,TRUE);

	std::string symobl(strStockTypeCode);
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (strStockTypeCode,STKTYPE,true);
	pFile ->WriteKLine(symobl,pKline,nTotalCount,CTaiKlineFileKLine::overWriteAll);
	//**************************************************************
	if(pKline)
	{
		GlobalUnlock((HGLOBAL)pKline);
		GlobalFree((HGLOBAL)pKline);
		pKline=NULL;
	}
	if(pStockTypeInfoTemp)
	{
		GlobalUnlock((HGLOBAL)pStockTypeInfoTemp);
		GlobalFree((HGLOBAL)pStockTypeInfoTemp);
	}
}

BOOL CStkBlockManager::InsertStockItemCorrect(char *m_szStockId ,PSTOCK_TYPE_INFO m_pStock)        //插入一个股票
{
	int low=0;
	int high=m_pStockTypeHead->m_lStockCount -1 ;
	int mid=0;
	int InsertPose=-1;

	while(low <= high)
	{
		mid=(low+high)/2;
		if(low==high)
		{
			if(strcmp(m_szStockId , m_pStockPoint[mid].pStockInfo->m_szSymbol )>0)
			{
				if(m_pStockTypeHead->m_lStockCount==0||m_pStockTypeHead->m_lStockCount==mid)
					InsertPose=mid ;
				else
					InsertPose=mid +1;   
				break;
			}
			else if(strcmp(m_szStockId , m_pStockPoint[mid].pStockInfo->m_szSymbol)<0)
			{
				InsertPose=mid ;
				break;
			}
			else
			{
				return TRUE ;
			}
		}
		if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol , m_szStockId)>0) high=mid -1;
		else if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol ,m_szStockId)< 0 ) low=mid +1;
		else 
		{
			return TRUE ;
		}
	}
	if(high<low)
		InsertPose=low;


	if(m_pStockTypeHead->m_lStockCount + 1 >m_pStockTypeHead->m_lStockMaxCount)  
	{
		AddStockTypeDataSize();    
	}

	if(m_pStockTypeHead->m_lStockCount > InsertPose)  
	{
		STOCK_POINT_INFO *ptemp;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc( GPTR, (m_pStockTypeHead->m_lStockCount - InsertPose )* sizeof( STOCK_POINT_INFO) );
		hp=GlobalLock(hMem);
		if(hp)
		{
			ptemp= (STOCK_POINT_INFO *)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		}
		memcpy(ptemp,&m_pStockPoint[InsertPose],(m_pStockTypeHead->m_lStockCount - InsertPose)*sizeof(STOCK_POINT_INFO));
		memcpy(&m_pStockPoint[InsertPose+1],ptemp,(m_pStockTypeHead->m_lStockCount - InsertPose)*sizeof(STOCK_POINT_INFO));
		GlobalUnlock((HGLOBAL)ptemp);        
		GlobalFree( (HGLOBAL)ptemp);
	}

	m_pStockPoint[InsertPose].pStockInfo=m_pStock;
	strcpy(m_pStockPoint[InsertPose].pStockInfo->m_szSymbol,m_szStockId);
	m_pStockTypeHead->m_lStockCount++;	
	return TRUE;
}
long CStkBlockManager::CorrectKline(Kline *pKline,long LineCount)
{
	long index=0;
	for(int i=0;i<LineCount-1;i++)
	{
		if(pKline[i].day<pKline[i+1].day)
		{   
			index++;
		}
		else
		{
			memmove(&pKline[i+1],&pKline[i+2],sizeof(Kline)*(LineCount-i-2));
			i--;
			LineCount--;
		}
	}
	return index+1;
}
float CStkBlockManager::GetStockBlockRight(char *StockTypeCode)
{
	PSTOCKTYPEINFO m_pStktype;
	BYTE m_btPos=GetStockTypeNumber(StockTypeCode);
	GetStockTypePoint(m_pStktype,StockTypeCode);
	int index=0;
	float TotalBlockRight=0;
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
		int j=0;
		if(m_pStockPoint[i].pStockInfo==NULL)
			continue;
		while(m_pStockPoint[i].pStockInfo->m_btStockType[j]!=255&&j<MaxStockTYpe)
		{
			if(m_pStockPoint[i].pStockInfo->m_btStockType[j]==m_btPos) 
			{
				CReportData *pCdat;
				CString m_strStockKind=m_pStockPoint[i].pStockInfo->m_szSymbol;
				char m_szStockCode[10];
				int nKind;
				strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
				nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));

				if(this->m_pDoc->m_sharesInformation.Lookup(m_szStockCode,pCdat,nKind))
				{
					if(pCdat->pBaseInfo==NULL)
					{
						j++;
						break;
					}
					switch(m_pStktype->m_iRightType)
					{
					case ZGB:  TotalBlockRight+=pCdat->pBaseInfo->zgb;
						break;
					case LTG:   if(CTaiShanReportView::IsBg( pCdat->kind))
									TotalBlockRight+=pCdat->pBaseInfo->Bg;
								else 
									TotalBlockRight+=pCdat->pBaseInfo->ltAg;
						break;
					case OTHER: TotalBlockRight+=1;
						break;
					}
				}
				break;
			}
			j++;
		};
	}
	return TotalBlockRight;
}

bool CStkBlockManager::CheckKind(int i)
{
	bool b = true;
	if(i==SHZS||i==SZZS||i==EBZS||i==STKTYPE)
		b = false;
	return b;
}



BOOL CStkBlockManager::InsertStockItem(char* m_szStockId, int nKind, PSTOCK_TYPE_INFO& m_pStock)
{
	int low = 0;
	int mid = 0;
	int high = m_pStockTypeHead->m_lStockCount - 1;
	int InsertPose = -1;

	char m_szStockKind[10];
	CString strSymbol = TSKDatabase()->GetStockSymbol(m_szStockId, nKind);
	strcpy_s(m_szStockKind, strSymbol.GetBuffer(0));

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (low == high)
		{
			if (strcmp(m_szStockKind, m_pStockPoint[mid].pStockInfo->m_szSymbol) > 0)
			{
				if (m_pStockTypeHead->m_lStockCount == 0 || m_pStockTypeHead->m_lStockCount == mid)
				{
					InsertPose = mid;
				}
				else
				{
					InsertPose = mid + 1;
				}
				break;
			}
			else if (strcmp(m_szStockKind, m_pStockPoint[mid].pStockInfo->m_szSymbol) < 0)
			{
				InsertPose = mid;
				break;
			}
			else
			{
				m_pStock = m_pStockPoint[mid].pStockInfo;
				return TRUE;
			}
		}

		if (strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			m_pStock = m_pStockPoint[mid].pStockInfo;
			return TRUE;
		}
	}

	if (high < low)
		InsertPose = low;


	if (m_pStockTypeHead->m_lStockCount + 1 > m_pStockTypeHead->m_lStockMaxCount)
	{
		AddStockTypeDataSize();
	}

	if (m_pStockTypeHead->m_lStockCount > InsertPose)
	{
		STOCK_POINT_INFO* ptemp;
		HGLOBAL	hMem;
		LPVOID hp;

		hMem = GlobalAlloc(GPTR, (m_pStockTypeHead->m_lStockCount - InsertPose) * sizeof(STOCK_POINT_INFO));
		hp = GlobalLock(hMem);
		if (hp)
		{
			ptemp = (STOCK_POINT_INFO*)hp;
		}
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}

		memcpy(ptemp, &m_pStockPoint[InsertPose], (m_pStockTypeHead->m_lStockCount - InsertPose) * sizeof(STOCK_POINT_INFO));
		memcpy(&m_pStockPoint[InsertPose + 1], ptemp, (m_pStockTypeHead->m_lStockCount - InsertPose) * sizeof(STOCK_POINT_INFO));
		GlobalUnlock((HGLOBAL)ptemp);
		GlobalFree((HGLOBAL)ptemp);
	}

	m_pStockPoint[InsertPose].pStockInfo = m_pStockInfo + m_pStockTypeHead->m_lStockCount;
	strcpy(m_pStockPoint[InsertPose].pStockInfo->m_szSymbol, m_szStockKind);
	memset(&m_pStockPoint[InsertPose].pStockInfo->m_btStockType[0], 255, sizeof(BYTE) * MaxStockTYpe);
	m_pStockPoint[InsertPose].pStockInfo->m_bDeleted = FALSE;
	m_pStockTypeHead->m_lStockCount++;

	m_pStock = m_pStockPoint[InsertPose].pStockInfo;

	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::DeleteStockItem(char* m_szStockId, int nKind)
{
	int low = 0;
	int mid = 0;
	int high = m_pStockTypeHead->m_lStockCount - 1;
	int DeletePos = -1;

	char m_szStockKind[10];
	CString strSymbol = TSKDatabase()->GetStockSymbol(m_szStockId, nKind);
	strcpy_s(m_szStockKind, strSymbol.GetBuffer(0));

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			DeletePos = mid;
			break;
		}
	}

	if (DeletePos == -1)
	{
		return FALSE;
	}

	memmove(&m_pStockPoint[DeletePos], &m_pStockPoint[DeletePos + 1], (m_pStockTypeHead->m_lStockCount - DeletePos - 1) * sizeof(STOCK_POINT_INFO));
	m_pStockTypeHead->m_lStockCount--;

	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::RemoveDeletedStock()
{
	BOOL result = TRUE;
	int index = m_pStockTypeHead->m_lStockCount;

	for (int j = 0; j < index; j++)
	{
		if ((m_pStockInfo + j)->m_btStockType[0] == 255 && (m_pStockInfo + j)->m_btStockType[CHOOSEPOS] == 255)
		{
			CString strSymbol = (m_pStockInfo + j)->m_szSymbol;
			SymbolKind symbol;
			TSKDatabase()->GetStockSymbol(symbol, strSymbol.GetBuffer(0));
			DeleteStockItem(symbol.m_chSymbol, symbol.m_nSymbolKind);
		}
	}

	if (!SavePosToFile())
		result = FALSE;

	if (index != m_pStockTypeHead->m_lStockCount)
	{
		for (int j = 0; j < index; j++)
		{
			if ((m_pStockInfo + j)->m_btStockType[0] == 255 && (m_pStockInfo + j)->m_btStockType[CHOOSEPOS] == 255)
			{
				memmove(m_pStockInfo + j, m_pStockInfo + index - 1, sizeof(STOCK_TYPE_INFO));
				index--;
				j--;
			}
		}
	}

	SaveDataToFile(m_pStockTypeHead,  0);

	return result;
}

BOOL CStkBlockManager::Lookup(char* m_szStockId, int nKind, PSTOCK_TYPE_INFO& m_pStock)
{
	int low = 0;
	int mid = 0;
	int high = m_pStockTypeHead->m_lStockCount - 1;

	char m_szStockKind[10];
	CString strSymbol = TSKDatabase()->GetStockSymbol(m_szStockId, nKind);
	strcpy_s(m_szStockKind, strSymbol.GetBuffer(0));

	while (low <= high)
	{
		mid = (low + high) / 2;

		if (strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			m_pStock = m_pStockPoint[mid].pStockInfo;
			return TRUE;
		}
	}

	m_pStock = NULL; 
	return FALSE;
}

BOOL CStkBlockManager::InsertStockType(PSTOCKTYPEINFO& m_pStktype, int m_iPos)
{
	if (m_iPos >= MaxStockTYpe)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_pStockTypeInfo[m_iPos].m_bIsDelete || m_pStockTypeInfo[m_iPos].m_lIsUse == -1)
	{
		m_pStktype = m_pStockTypeInfo + m_iPos;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_pStktype->m_fNewIndex = 0;
	m_pStktype->m_fTotP = 0;
	m_pStktype->m_fTotV = 0;
	m_pStktype->m_fYstIndex = 0;
	m_pStktype->m_fOpenIndex = 0;

	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::DeleteStockType(char* m_pszStockTypeCode)
{
	BYTE m_btPos = GetStockTypeNumber(m_pszStockTypeCode);
	for (int i = 0; i < m_pStockTypeHead->m_lStockCount; i++)
	{
		int index = 0;
		BOOL m_bIsFind = FALSE;

		while (m_pStockInfo[i].m_btStockType[index] != 255 && index < MaxStockTYpe && !m_bIsFind)
		{
			if (m_pStockInfo[i].m_btStockType[index] == m_btPos)
			{
				do
				{
					m_pStockInfo[i].m_btStockType[index] = m_pStockInfo[i].m_btStockType[index + 1];
					index++;
				}
				while (m_pStockInfo[i].m_btStockType[index] != 255 && index < MaxStockTYpe);
				m_bIsFind = TRUE;
				break;
			}
			index++;
		};
	}

	m_pStockTypeInfo[m_btPos].m_bIsDelete = TRUE;
	CTaiKlineFileKLine::ZeroKlineData(m_pStockTypeInfo[m_btPos].m_szCode, STKTYPE, TRUE);
	CTaiKlineFileKLine::ZeroKlineData(m_pStockTypeInfo[m_btPos].m_szCode, STKTYPE, FALSE);
	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::CheckBlockName(char* BlockName, char* BlockCode)
{
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		CString m_szTypeName;

		if (m_pStockTypeInfo[i].m_lIsUse == -1)
			break;

		if (m_pStockTypeInfo[i].m_bIsDelete)
			continue;

		if (BlockCode == NULL)
		{
			if (strcmp(m_pStockTypeInfo[i].m_szName, BlockName) == 0)
				return TRUE;
		}
		else
		{
			if (strcmp(m_pStockTypeInfo[i].m_szName, BlockName) == 0 && strcmp(m_pStockTypeInfo[i].m_szCode, BlockCode) != 0)
				return TRUE;
		}
	}

	return FALSE;
}

int CStkBlockManager::GetInsertStockTypePos(int nStart)
{
	for (int i = nStart; i < MaxStockTYpe; i++)
	{
		if (m_pStockTypeInfo[i].m_bIsDelete || m_pStockTypeInfo[i].m_lIsUse == -1)
		{
			return i;
		}
	}

	ASSERT(FALSE);

	return -1;
}

int CStkBlockManager::GetStockTypeCounts()
{
	int index = 0;
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		if (m_pStockTypeInfo[i].m_lIsUse == -1)
			break;

		if (m_pStockTypeInfo[i].m_bIsDelete)
			continue;

		index++;
	}

	return index;
}

void CStkBlockManager::GetAllStockTypeCode(CStringArray& m_StockTypeCodeArray)
{
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		CString m_szTypeCode;
		if (m_pStockTypeInfo[i].m_lIsUse == -1)
			break;

		if (m_pStockTypeInfo[i].m_bIsDelete)
			continue;

		m_szTypeCode = m_pStockTypeInfo[i].m_szCode;
		m_StockTypeCodeArray.Add(m_szTypeCode);
	}
}

void CStkBlockManager::GetAllStockTypeName(CStringArray& m_StockTypeNameArray)
{
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		CString m_szTypeName;
		if (m_pStockTypeInfo[i].m_lIsUse == -1)
			break;

		if (m_pStockTypeInfo[i].m_bIsDelete)
			continue;

		m_szTypeName = m_pStockTypeInfo[i].m_szName;
		m_StockTypeNameArray.Add(m_szTypeName);
	}
}

BOOL CStkBlockManager::GetStockTypePoint(PSTOCKTYPEINFO& m_pStktype, char* m_pszStockTypeCode)
{
	BYTE m_btPos = GetStockTypeNumber(m_pszStockTypeCode);
	m_pStktype = m_pStockTypeInfo + m_btPos;

	return TRUE;
}

BOOL CStkBlockManager::GetStockTypePointFromName(PSTOCKTYPEINFO& m_pStktype, char* m_pszStockTypeName)
{
	for (int i = 0; i < MaxStockTYpe; i++)
	{
		CString m_szTypeName;
		if (m_pStockTypeInfo[i].m_lIsUse == -1)
			break;

		if (m_pStockTypeInfo[i].m_bIsDelete)
			continue;

		if (strcmp(m_pStockTypeInfo[i].m_szName, m_pszStockTypeName) == 0)
		{
			m_pStktype = &m_pStockTypeInfo[i];
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStkBlockManager::InsertStockToType(PCdat1& m_pCdat, char* m_pszStockTypeCode)
{
	STOCKTYPEINFO* m_pStktype;
	STOCK_TYPE_INFO* m_pStock;

	int index = 0;

	/* 去掉这两项检测
	if (m_pCdat->pBaseInfo == NULL)
	{
		MessageBox(NULL, "在将股票加入板块时,该股票财务数据不能为空!", "窗口操作提示", MB_OK);
		return FALSE;
	}

	if (!CheckKind(m_pCdat->kind))
	{
		MessageBox(NULL, "指数类将不能增加到板块中!", "窗口操作提示", MB_OK);
		return FALSE;
	}//*/

	BYTE m_btPos = GetStockTypeNumber(m_pszStockTypeCode);

	if (!GetStockTypePoint(m_pStktype, m_pszStockTypeCode))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (!InsertStockItem(m_pCdat->id, m_pCdat->kind, m_pStock))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	for (int i = 0; m_pStock->m_btStockType[i] != 255 && i < MaxStockTYpe; i++)
	{
		if (m_pStock->m_btStockType[i] == m_btPos)
			return TRUE;
	}

	while (m_pStock->m_btStockType[index] != 255 && index < MaxStockTYpe)
	{
		index++;
	};

	m_pStock->m_btStockType[index] = m_btPos;
	m_pStock->m_bDeleted = FALSE;
	m_pStock->m_fRight[ZGB] = m_pCdat->pBaseInfo->zgb;
	if (m_pCdat->kind == SHBG || m_pCdat->kind == SZBG)
	{
		m_pStock->m_fRight[LTG] = m_pCdat->pBaseInfo->Bg;
	}
	else
	{
		m_pStock->m_fRight[LTG] = m_pCdat->pBaseInfo->ltAg;
	}

	CTaiKlineFileKLine::GetFirstKline(m_pCdat->id, m_pCdat->kind, TRUE, 1, &m_pStock->m_ClosePrice);

	m_pCdat->pStockTypeInfo = m_pStock; 

	switch (m_pStktype->m_iRightType)
	{
	case ZGB:
		m_pStktype->m_fTotalRight += m_pStock->m_fRight[ZGB] * m_pStock->m_ClosePrice;
		m_pStktype->m_lStockCount++;
		break;
	case LTG:
		m_pStktype->m_fTotalRight += m_pStock->m_fRight[LTG] * m_pStock->m_ClosePrice;
		m_pStktype->m_lStockCount++;
		break;
	case OTHER:
		m_pStktype->m_fTotalRight += m_pStock->m_ClosePrice;
		m_pStktype->m_lStockCount++;
		break;
	}

	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::DeleteStockFromType(char* m_pszStockCode, int nKind, char* m_pszStockTypeCode)
{
	STOCKTYPEINFO* m_pStktype;

	BYTE m_btPos = GetStockTypeNumber(m_pszStockTypeCode);

	if (!GetStockTypePoint(m_pStktype, m_pszStockTypeCode))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	STOCK_TYPE_INFO* m_pStock;
	if (Lookup(m_pszStockCode, nKind, m_pStock))
	{
		int index = 0;
		while (m_pStock->m_btStockType[index] != 255 && index < MaxStockTYpe)
		{
			if (m_pStock->m_btStockType[index] == m_btPos)
			{
				switch (m_pStktype->m_iRightType)           
				{
				case ZGB:
					m_pStktype->m_fTotalRight -= m_pStock->m_fRight[ZGB] * m_pStock->m_ClosePrice;
					m_pStktype->m_lStockCount--;
					break;
				case LTG:
					m_pStktype->m_fTotalRight -= m_pStock->m_fRight[LTG] * m_pStock->m_ClosePrice;
					m_pStktype->m_lStockCount--;
					break; 
				case OTHER:
					m_pStktype->m_fTotalRight -= m_pStock->m_ClosePrice;
					m_pStktype->m_lStockCount--;
					break;
				}

				do
				{
					m_pStock->m_btStockType[index] = m_pStock->m_btStockType[index + 1];
					index++;
				}
				while (m_pStock->m_btStockType[index] != 255 && index < MaxStockTYpe);
				m_pStock->m_btStockType[index] = 255;
				if (index == 0)
				{
					m_pStock->m_bDeleted = TRUE;
				}
				break;
			}
			index++;
		};
		SaveDataToFile(m_pStockTypeHead, 0);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CStkBlockManager::GetStockCountFromType(char* m_pszCode)
{
	BYTE m_btPos = GetStockTypeNumber(m_pszCode);
	return m_pStockTypeInfo[m_btPos].m_lStockCount;
}

BOOL CStkBlockManager::GetStockFromStockType(SymbolKindArr& m_StockCodeArray, char* m_pszStockTypeCode)
{
	BYTE m_btPos = GetStockTypeNumber(m_pszStockTypeCode);

	int index = 0;
	for (int i = 0; i < m_pStockTypeHead->m_lStockCount; i++)
	{
		int j = 0;

		if (m_pStockPoint[i].pStockInfo == NULL)
			continue;

		while (m_pStockPoint[i].pStockInfo->m_btStockType[j] != 255 && j < MaxStockTYpe)
		{
			if (m_pStockPoint[i].pStockInfo->m_btStockType[j] == m_btPos)
			{
				SymbolKind CodeStr;
				CString strSymbol = m_pStockPoint[i].pStockInfo->m_szSymbol;
				TSKDatabase()->GetStockSymbol(CodeStr, strSymbol.GetBuffer(0));
				m_StockCodeArray.Add(CodeStr);
				index++;
				break;
			}
			j++;
		};
	}

	return TRUE;
}

BOOL CStkBlockManager::GetStockFromStockTypeName(SymbolKindArr& m_StockCodeArray, char* m_pszStockTypeName)
{
	STOCKTYPEINFO* l_pStktype;
	if (!GetStockTypePointFromName(l_pStktype, m_pszStockTypeName))
	{
		return FALSE;
	}

	return GetStockFromStockType(m_StockCodeArray, l_pStktype->m_szCode);
}

/* ============================================================================
	自选板块操作
*/

BOOL CStkBlockManager::InsertStockToChoose(char* m_szStockId, int nKind)
{
	PSTOCK_TYPE_INFO m_pStock;
	InsertStockItem(m_szStockId, nKind, m_pStock);

	m_pStock->m_bDeleted = FALSE;
	m_pStock->m_btStockType[CHOOSEPOS] = CHOOSEPOS;
	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::DeleteStockFromChoose(char* m_szStockId, int nKind)
{
	PSTOCK_TYPE_INFO m_pStock;
	Lookup(m_szStockId, nKind, m_pStock);

	m_pStock->m_btStockType[CHOOSEPOS] = 255;
	SaveDataToFile(m_pStockTypeHead, 0);

	return TRUE;
}

BOOL CStkBlockManager::GetChooseStockCode(SymbolKindArr& m_StockCodeArray)
{
	for (int i = 0; i < m_pStockTypeHead->m_lStockCount; i++)
	{
		if (m_pStockPoint[i].pStockInfo == NULL)
			continue;

		if (m_pStockPoint[i].pStockInfo->m_btStockType[CHOOSEPOS] == CHOOSEPOS)
		{
			SymbolKind CodeStr;
			CString strSymbol = m_pStockPoint[i].pStockInfo->m_szSymbol;
			TSKDatabase()->GetStockSymbol(CodeStr, strSymbol.GetBuffer(0));
			m_StockCodeArray.Add(CodeStr);
			continue;
		}
	}

	return TRUE;
}

int CStkBlockManager::GetChooseStockCounts()
{
	int iCounts = 0;
	for (int i = 0; i < m_pStockTypeHead->m_lStockCount; i++)
	{
		if (m_pStockPoint[i].pStockInfo == NULL)
			continue;

		if (m_pStockPoint[i].pStockInfo->m_btStockType[CHOOSEPOS] == CHOOSEPOS)
		{
			iCounts++;
			continue;
		}
	}

	return iCounts;
}



/* ============================================================================

STOCKTYPEINFO* m_pStockTypeInfo

修改指数代码规则
8K00 XX
前面数字代表类型 1 2 3 
后面三位数字，按板块编号

STOCK_TYPE_INFO 代码表改为 01XXXXXX => SHXXXXXX

char m_szStockKind[10];
CString strSymbol = TSKDatabase()->GetStockSymbol(m_szStockId, nKind);
strcpy_s(m_szStockKind, strSymbol.GetBuffer(0));

SH60000 => SymbolKind
SymbolKind symbol;
TSKDatabase()->GetStockSymbol(symbol, strSymbol);

使用：
InitStockTypeData(strPath);
InitStockPoint()

*/
