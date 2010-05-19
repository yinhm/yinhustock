
#include "stdafx.h"
#include "StructTaiShares.h"
#include "CSharesBaseInfo.h"

#include <share.h>
#include  <io.h>

#include "StockDrv.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharesBaseInfo::CSharesBaseInfo()
{
	m_hFile = NULL;
	m_hFileMap = NULL;
	m_pbData = NULL;
	m_pBaseInfoPoint = NULL;
}

CSharesBaseInfo::~CSharesBaseInfo()
{
	BOOL result = TRUE;
	if (m_hFile)
	{
		result = SavePosToFile();
	}
	if (result)
	{
		m_BaseFileHead->FileExitDone = 12345678;
	}
	else
	{
		m_BaseFileHead->FileExitDone = 87654321;
	}

	if (m_pbData)
	{
		UnmapViewOfFile(m_pbData);
	}
	if (m_hFileMap)
	{
		CloseHandle(m_hFileMap);
	}
	if (m_hFile)
	{
		CloseHandle(m_hFile);
	}
}

BOOL CSharesBaseInfo::InitBaseInfoData(CString Path)
{
	strcpy(m_sFilePath, Path.GetBuffer(0));
	if (_access(g_baseinfo, 0) == -1)
	{
		return InitBaseInfoDataEmpty();
	}
	else
	{
		return InitBaseInfoDataExist();
	}
}

BOOL CSharesBaseInfo::InitBaseInfoDataEmpty()
{
	BYTE* temp;
	long m_filelength = sizeof(BASEINFOHEAD) + (sizeof(BASEINFO)) * 2000;

	m_hFile = CreateFile(g_baseinfo, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开基本资料数据文件出错");
		return FALSE;
	}

	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, m_filelength, NULL);
	if (m_hFileMap == NULL)
	{
		AfxMessageBox("创立基本资料文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFileMap = NULL;
		m_hFile = NULL;
		return FALSE;
	}

	m_pbData = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (m_pbData == NULL)
	{
		AfxMessageBox("将基本资料文件数据映射入内存时出错");
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		m_hFileMap = NULL;
		m_hFile = NULL;
		return FALSE;
	}

	m_BaseFileHead = (BASEINFOHEAD*)m_pbData;
	m_BaseFileHead->filetitle = 12345678;
	m_BaseFileHead->MaxStockCount = 2000;
	m_BaseFileHead->StockCount = 0;
	m_BaseFileHead->FileExitDone = 87654321;
	m_BaseFileHead->FixedNumSplit = 80;

	temp = m_pbData + sizeof(BASEINFOHEAD);
	m_pBaseInfo = (BASEINFO*)temp;
	if (!SetMemroyALLOCSize(m_BaseFileHead->MaxStockCount))
	{
		AfxMessageBox("初始化数据变量出错");
		return FALSE;
	}

	SaveBaseInfoToFile(m_BaseFileHead, sizeof(BASEINFOHEAD));

	return TRUE;
}

BOOL CSharesBaseInfo::InitBaseInfoDataExist()
{
	BYTE* temp;

	m_hFile = CreateFile(g_baseinfo, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开基本资料数据文件出错");
		return FALSE; 
	}

	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	if (m_hFileMap == NULL)
	{
		AfxMessageBox("创立基本资料文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFileMap = NULL;
		m_hFile = NULL;
		return FALSE;
	}

	m_pbData = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (m_pbData == NULL)
	{
		AfxMessageBox("将基本资料文件数据映射入内存时出错");
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		m_hFileMap = NULL;
		m_hFile = NULL;
		return FALSE;
	}

	m_BaseFileHead = (BASEINFOHEAD*)m_pbData;
	if (m_BaseFileHead->filetitle != 12345678 && m_BaseFileHead->filetitle != 55555555)
	{
		UnmapViewOfFile(m_pbData);
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		return InitBaseInfoDataEmpty();
	}

	if (m_BaseFileHead->StockCount + 100 > m_BaseFileHead->MaxStockCount)
	{
		int StockTotalCount = 0;
		long FileLength = 0;
		m_BaseFileHead->MaxStockCount += 100;
		StockTotalCount = m_BaseFileHead->MaxStockCount;
		FileLength = sizeof(BASEINFOHEAD) + (sizeof(BASEINFO)) * StockTotalCount;

		SaveBaseInfoToFile(m_BaseFileHead, sizeof(BASEINFOHEAD));
		UnmapViewOfFile(m_pbData);
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
		m_pbData = NULL;

		m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, FileLength, NULL);
		if (m_hFileMap == NULL)
		{
			AfxMessageBox("创立基本资料文件映射内核时出错");
			CloseHandle(m_hFile);
			m_hFileMap = NULL;
			m_hFile = NULL;
			return FALSE;
		}

		m_pbData = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
		if (m_pbData == NULL)
		{
			AfxMessageBox("将基本资料文件数据映射入内存时出错");
			CloseHandle(m_hFileMap);
			CloseHandle(m_hFile);
			m_hFileMap = NULL;
			m_hFile = NULL;
			return FALSE;
		}

		m_BaseFileHead = (BASEINFOHEAD*)m_pbData;
	}

	temp = m_pbData + sizeof(BASEINFOHEAD);
	m_pBaseInfo = (BASEINFO*)temp;
	if (!SetMemroyALLOCSize(m_BaseFileHead->MaxStockCount))
	{
		AfxMessageBox("初始化数据变量出错");
		return FALSE;
	} 

	if (m_BaseFileHead->FileExitDone == 12345678)
	{
		for (int j = 0; j < m_BaseFileHead->StockCount; j++)
		{
			InsertItemPoint(m_pBaseInfo + j);
		}
	}
	else
	{
		int temp = m_BaseFileHead->StockCount;
		m_BaseFileHead->StockCount = 0;
		for (int j = 0; j < temp; j++)
		{
			AddStockItemCorrect((m_pBaseInfo + j)->Symbol, m_pBaseInfo + j);
		}
	}

	m_BaseFileHead->FileExitDone = 87654321;
	SaveBaseInfoToFile(m_BaseFileHead, sizeof(BASEINFOHEAD));

	return TRUE;
}

BOOL CSharesBaseInfo::SavePosToFile()
{
	for (int j = 0; j < m_BaseFileHead->StockCount; j++)
	{
		if (m_pBaseInfoPoint[j] == NULL)
		{
			return FALSE;
		}

		m_pBaseInfoPoint[j]->m_iPos = j;
	}

	return TRUE;
}

BOOL CSharesBaseInfo::SetMemroyALLOCSize(unsigned int nSize)
{
	PBASEINFO* tempData;
	int temp = 0;
	HGLOBAL	hMem;
	LPVOID hp;

	if (!m_pBaseInfoPoint)
	{
		hMem = GlobalAlloc(GPTR, (nSize) * sizeof(PBASEINFO));
		hp = GlobalLock(hMem);
		if (hp)
		{
			m_pBaseInfoPoint = (PBASEINFO*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
			return FALSE;
		}
	}
	else
	{
		hMem = GlobalAlloc(GPTR, (nSize) * sizeof(PBASEINFO));
		hp = GlobalLock(hMem);
		if (hp)
		{
			tempData = (PBASEINFO*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
			return FALSE;
		}

		memcpy(tempData, m_pBaseInfoPoint, sizeof(PBASEINFO) * m_BaseFileHead->StockCount);
		GlobalUnlock((HGLOBAL)m_pBaseInfoPoint);
		GlobalFree((HGLOBAL)m_pBaseInfoPoint);
		m_pBaseInfoPoint = tempData;
	}

	return TRUE;
}

BOOL CSharesBaseInfo::AddStockTypeDataSize()
{
	BYTE* temp;

	m_BaseFileHead->MaxStockCount += 100;
	long m_filelength = sizeof(BASEINFOHEAD) + (sizeof(BASEINFO)) * m_BaseFileHead->MaxStockCount;
	SavePosToFile();

	if (m_pbData)
		UnmapViewOfFile(m_pbData);
	if (m_hFileMap)
		CloseHandle(m_hFileMap);

	m_hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, m_filelength, NULL);
	if (m_hFileMap == NULL)
	{
		AfxMessageBox("创立基本资料文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFileMap = NULL;
		m_hFile = NULL;
		return FALSE;
	}

	m_pbData = (PBYTE)MapViewOfFile(m_hFileMap, FILE_MAP_WRITE, 0, 0, 0);
	if (m_pbData == NULL)
	{
		AfxMessageBox("将基本资料文件数据映射入内存时出错");
		CloseHandle(m_hFileMap);
		CloseHandle(m_hFile);
		m_hFileMap = NULL;
		m_hFile = NULL;
		return FALSE;
	}

	m_BaseFileHead = (BASEINFOHEAD*)m_pbData;

	temp = m_pbData + sizeof(BASEINFOHEAD);
	m_pBaseInfo = (BASEINFO*)temp;

	if (!SetMemroyALLOCSize(m_BaseFileHead->MaxStockCount))
	{
		AfxMessageBox("初始化数据变量出错");
		return FALSE;
	} 

	for (int j = 0; j < m_BaseFileHead->StockCount; j++)
	{
		InsertItemPoint(m_pBaseInfo + j);
	}

	CTaiShanDoc* m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	m_pDoc->m_sharesInformation.InitBaseInfoData();

	SaveBaseInfoToFile(m_BaseFileHead, sizeof(BASEINFOHEAD));

	return TRUE;
}

BOOL CSharesBaseInfo::InsertItemPoint(BASEINFO* m_pStk)
{
	int InsertPose = m_pStk->m_iPos;
	m_pBaseInfoPoint[InsertPose] = m_pStk;

	return TRUE;
}

//BOOL CSharesBaseInfo::ReadBaseInfoData(char *StockCode,int nKind,PBASEINFO & pBaseItem)//读入数据
//{
//	HANDLE hFileBase,hFileMapBase;              
//	DWORD nlength=0;
//	BYTE  *pbDataBase;                          
//	char  *pFileData;
//	char ls_path[255];
//	char temp[100];
//	if(nKind==SHZS||nKind==SHAG||nKind==SHBG||nKind==SHZQ)
//	{
//		strcpy(ls_path,this->m_sFilePath);
//		strcat(ls_path,"\\");
//		strcat(ls_path,g_sF10sh);
//		strcat(ls_path,StockCode);
//		strcat(ls_path,".TXT");
//	}
//	else if(nKind==SZZS||nKind==SZAG||nKind==SZBG||nKind==SZZQ)
//	{
//		strcpy(ls_path,this->m_sFilePath);
//		strcat(ls_path,"\\");
//		strcat(ls_path,g_sF10sz);
//		strcat(ls_path,StockCode);
//		strcat(ls_path,".TXT");
//	}
//	if(_access(ls_path,0)==-1)   
//	{
//		return FALSE;
//	}
//	hFileBase=CreateFile(ls_path,GENERIC_READ|GENERIC_WRITE,
//		FILE_SHARE_READ|FILE_SHARE_WRITE,
//		NULL,
//		OPEN_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL);
//	if (hFileBase == INVALID_HANDLE_VALUE)
//	{
//		AfxMessageBox("打开股票基本资料数据文件出错");
//		return FALSE; 
//	}
//	nlength=GetFileSize(hFileBase,NULL);
//	hFileMapBase=CreateFileMapping(hFileBase,
//		NULL,
//		PAGE_READWRITE,
//		0,
//		nlength,
//		NULL);
//	if(hFileMapBase==NULL)
//	{
//		AfxMessageBox("创立股票基本资料文件映射内核时出错");
//		CloseHandle(hFileBase);
//		hFileBase=NULL;
//		hFileMapBase=NULL;
//		return FALSE;
//	}
//	pbDataBase=(PBYTE)MapViewOfFile(hFileMapBase,
//		FILE_MAP_READ|FILE_MAP_WRITE,
//		0,0,nlength);
//	if(pbDataBase==NULL)
//	{
//		AfxMessageBox("将股票基本资料文件数据映射入内存时出错");
//		CloseHandle(hFileBase);
//		CloseHandle(hFileMapBase);
//		hFileBase=NULL;
//		hFileMapBase=NULL;
//		return FALSE;
//	}
//	pFileData=(char *)pbDataBase;
//	pFileData[nlength-1]='\0';
//	strncpy(temp,pFileData,80);
//	temp[80]='\0';
//
//	BOOL bRtn = TRUE;
//	try
//	{
//		if(strstr(temp,"博经闻"))
//			ReadBaseInfoDataBJW(pFileData,pBaseItem);
//		else if(strstr(temp,"万国股市测评"))
//		{
//			ReadBaseInfoDataWG(pFileData,pBaseItem);
//			pBaseItem->mgwfplr=pBaseItem->wfplr/pBaseItem->zgb;
//		}
//		else   
//		{
//			bRtn =  FALSE;
//		}
//	}
//	catch(...)
//	{
//		bRtn =  FALSE;
//	}
//	UnmapViewOfFile(pbDataBase);
//	CloseHandle(hFileMapBase);
//
//	CloseHandle(hFileBase);
//	return bRtn;
//}
//void CSharesBaseInfo::ReadBaseInfoDataBJW(char *pFileData,PBASEINFO & pBaseItem) 
//{// NOTE: the ClassWizard will add member functions here
//	char *fileB,*fileA;
//	char str[100];
//	for(int i=0;i<3;i++)
//	{
//		if(i==0)
//		{
//			fileB=strstr(pFileData,"、股本结构情况");
//			if(fileB==NULL)
//			{
//				fileB=strstr(pFileData,"、\\t股本结构情况");
//				if(fileB==NULL)
//					continue;
//			}
//			fileA=strchr(fileB,'\n');
//			if(fileA==NULL)
//				continue;
//		}
//		else if(i==1)
//		{
//			fileB=strstr(pFileData,"、资产负债表");
//			if(fileB==NULL)
//			{
//				fileB=strstr(pFileData,"、\\t资产负债表");
//				if(fileB==NULL)
//					continue;
//			}
//			fileA=strchr(fileB,'\n');
//			if(fileA==NULL)
//				continue;
//		}
//		else if(i==2)
//		{
//			fileB=strstr(pFileData,"、利润及利润分配表");
//			if(fileB==NULL)
//			{
//				fileB=strstr(pFileData,"、\\t利润及利润分配表");
//				if(fileB==NULL)
//					continue;
//			}
//			fileA=strchr(fileB,'\n');
//			if(fileA==NULL)
//				continue;
//		}
//		while(true)
//		{
//			CString temp;
//			CString mc;
//			int ls_startpos,ls_endpos;
//			float sj;
//			fileA=strchr(fileB,'\n');
//			strncpy(str,fileB,fileA-fileB +1);
//			str[fileA-fileB-1 ]='\0';
//			temp=str;
//			if(temp.IsEmpty())
//			{
//				break;
//			}
//			ls_startpos=temp.Find ("│",0);
//			if(ls_startpos<0)
//			{
//				fileB=fileA+1;
//				continue;
//			}
//			mc=temp.Left(ls_startpos);
//			mc.TrimLeft();
//			mc.TrimRight();
//			if(mc.Compare("名称＼年度")==0)
//			{
//				fileB=fileA+1;
//				continue;
//			}
//			ls_endpos=temp.Find ("│",ls_startpos+1);
//			CString zbsj=temp.Mid(ls_startpos+2,ls_endpos-ls_startpos-2);
//			zbsj.TrimLeft ();
//			zbsj.TrimRight();
//			sj=(float)atof(zbsj.GetBuffer(0) );
//			SetBaseInfoData(mc,sj,pBaseItem);
//			fileB=fileA+1;
//		}
//	}
//	fileB=strstr(pFileData,"年中期指标");
//	if(fileB!=NULL)
//	{
//		pBaseItem->mgsy *=2;
//	}
//
//}
//void CSharesBaseInfo::ReadBaseInfoDataWG(char *pFileData,PBASEINFO & pBaseItem)
//{// NOTE: the ClassWizard will add member functions here
//	char *fileB,*fileA;
//	char str[100];
//	if(pBaseItem->Symbol[2]=='5'&&strlen(pBaseItem->Symbol)==6 )
//		return;
//	if(pBaseItem->Symbol[4]=='4'&&strlen(pBaseItem->Symbol)==6 )
//		return;
//
//	for(int i=0;i<4;i++)
//	{
//		if(i==0)
//		{
//			fileB=strstr(pFileData,"=主要财务指标=");
//			if(fileB==NULL)
//				return;
//		}
//		else if(i==1)
//		{
//			fileB=strstr(fileB,"=股本结构=");
//			if(fileB==NULL)
//				return;
//		}
//		else if(i==2)
//		{
//			fileB=strstr(fileB,"=经营业绩=");
//			if(fileB==NULL)
//				return;
//		}
//		else if(i==3)
//		{
//			fileB=strstr(fileB,"=公司资产负债情况=");
//			if(fileB==NULL)
//				return;
//		}
//		while(true)
//		{
//			CString temp;
//			CString mc;
//			int ls_startpos,ls_endpos;
//			float sj;
//			fileA=strchr(fileB,'\n');
//			strncpy(str,fileB,fileA-fileB +1);
//			str[fileA-fileB-1 ]='\0';
//			temp=str;
//			if(temp.IsEmpty()||temp.Find ("代码:",0)>0)
//			{
//				break;
//			}
//			if(temp.GetLength()<50 )
//			{
//				fileB=fileA+1;
//				continue;
//			}
//			ls_startpos=temp.Find ("|",1);
//			if(ls_startpos<0)
//			{
//				fileB=fileA+1;
//				continue;
//			}
//			mc=temp.Mid(1,ls_startpos-1);
//			mc.TrimLeft();
//			mc.TrimRight();
//			int cc=mc.GetLength(); 
//			if(mc.Compare("指标")==0||strcmp(mc.Left(10).GetBuffer(0),"----------")==0||
//				mc.Compare("指标(单位:千元)")==0||mc.Compare("指标(单位:万股)")==0)
//
//			{
//				fileB=fileA+1;
//				continue;
//			}
//			ls_endpos=temp.Find ("|",ls_startpos+1);
//			CString zbsj=temp.Mid(ls_startpos+2,ls_endpos-ls_startpos-2);
//			zbsj.TrimLeft ();
//			ls_endpos=zbsj.Find (" ",0);
//			zbsj=zbsj.Left(ls_endpos ) ;
//			sj=(float)atof(zbsj.GetBuffer(0) );
//			SetBaseInfoData(mc,sj,pBaseItem,1);
//			fileB=fileA+1;
//		}
//
//	}
//}
//void CSharesBaseInfo::SetBaseInfoData(CString mc,float zbsj,PBASEINFO & pBaseItem,int mode)
//{// NOTE: the ClassWizard will add member functions here
//	if(mc==_T("总股本(万股)")||mc==_T("总股本"))
//	{
//		pBaseItem->zgb=zbsj;
//		return;
//	}else if(mc==_T("国家股(万股)")||mc==_T("国有股"))
//	{
//		pBaseItem->gjg=zbsj;
//		return;
//	}else if(mc==_T("发起人法人股(万股)")||mc==_T("国有法人股"))
//	{
//		pBaseItem->fqrfrg=zbsj;
//		return;
//	}else if(mc==_T("法人股(万股)")||mc==_T("社会法人股"))
//	{
//		pBaseItem->frg=zbsj;
//		return;
//	}else if(mc==_T("职工股(万股)")||mc==_T("内部职工股"))
//	{
//		pBaseItem->zgg=zbsj;
//		return;
//	}else if(mc==_T("公众Ａ股(万股)"))
//	{
//		pBaseItem->gzAg=zbsj;
//		return;
//	}else if(mc==_T("流通Ａ股(万股)")||mc==_T("流通Ａ股"))
//	{
//		pBaseItem->ltAg=zbsj;
//		return;
//	}else if(mc==_T("Ｈ股(万股)")||mc==_T("Ｈ股"))
//	{
//		pBaseItem->Hg=zbsj;
//		return;
//	}else if(mc==_T("Ｂ股(万股)")||mc==_T("流通Ｂ股"))
//	{
//		pBaseItem->Bg=zbsj;
//		return;
//	}else if(mc==_T("转配股(万股)")||mc==_T("转配股"))
//	{
//		pBaseItem->zpg=zbsj;
//		return;
//	}else if(mc==_T("总资产(万元)")||mc==_T("总资产"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->zzc=zbsj;
//		return;
//	}else if(mc==_T("流动资产(万元)")||mc==_T("流动资产"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->ldzc=zbsj;
//		return;
//	}else if(mc==_T("长期投资(万元)")||mc==_T("长期投资"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->cqtz=zbsj;
//		return;
//	}else if(mc==_T("固定资产(万元)")||mc==_T("固定资产"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->gdzc=zbsj;
//		return;
//	}else if(mc==_T("无形及递延资产(万元)"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->wxzc=zbsj;
//		return;
//	}else if(mc==_T("流动负债(万元)")||mc==_T("流动负债"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->ldfz=zbsj;
//		return;
//	}else if(mc==_T("长期负债(万元)")||mc==_T("长期负债"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->cqfz=zbsj;
//		return;
//	}else if(mc==_T("股东权益(万元)")||mc==_T("股东权益"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->ggqy=zbsj;
//		return;
//	}else if(mc==_T("资本公积金(万元)")||mc==_T("资本公积金"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->zbgjj=zbsj;
//		return;
//	}else if(mc==_T("盈余公积金(万元)"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->yygjj=zbsj;
//		return;
//	}else if(mc==_T("每股净值(元)")||mc==_T("每股净资产  (元)"))
//	{
//		pBaseItem->mgjz=zbsj;
//		pBaseItem->mgjzc=zbsj;   
//
//		return;
//	}else if(mc==_T("股东权益比率(％)")||mc==_T("股东权益比率(％)"))
//	{
//		pBaseItem->gdqybl=zbsj;
//		return;
//	}else if(mc==_T("每股资本公积金(元)")||mc==_T("每股公积金  (元)"))
//	{
//		pBaseItem->mggjj=zbsj;
//		return;
//	}else if(mc==_T("主营业务收入(万元)")||mc==_T("主营收入"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->zyywsr=zbsj;
//		return;
//	}else if(mc==_T("主营业务利润(万元)")||mc==_T("主营利润"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->zyywlr=zbsj;
//		return;
//	}else if(mc==_T("其它业务利润(万元)"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->qtywlr=zbsj;
//		return;
//	}else if(mc==_T("利润总额(万元)")||mc==_T("利润总额"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->lrze=zbsj;
//		return;
//	}else if(mc==_T("净利润(万元)")||mc==_T("净利润"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->jlr=zbsj;
//		return;
//	}else if(mc==_T("未分配利润(万元)")||mc==_T("未分配利润"))
//	{
//		if(mode==1)
//			zbsj =zbsj/10;
//		pBaseItem->wfplr=zbsj;
//		return;
//	}else if(mc==_T("每股收益(元)")||mc==_T("每股收益    (元)"))
//	{
//		pBaseItem->mgsy=zbsj;
//		return;
//	}else if(mc==_T("净资产收益率(％)")||mc==_T("净资产收益率(％)"))
//	{
//		pBaseItem->jzcsyl=zbsj;
//		return;
//	}else if(mc==_T("每股未分配利润(元)"))
//	{
//		pBaseItem->mgwfplr=zbsj;
//		return;
//	}
//}

BOOL CSharesBaseInfo::AddStockItem(char* pStockCode, int nKind, PBASEINFO& pBaseItem)
{
	int low = 0;
	int mid = 0;
	int high = m_BaseFileHead->StockCount - 1;
	int InsertPose = -1;

	CTaiShanDoc* m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;

	char strStockKind[10];
	CString strKind = m_pDoc->GetStockKindString(nKind);
	strcpy(strStockKind, strKind.GetBuffer(0));
	strcat(strStockKind, pStockCode);

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (low == high)
		{
			if (strcmp(strStockKind, m_pBaseInfoPoint[mid]->Symbol) > 0)
			{
				if (m_BaseFileHead->StockCount == 0 || m_BaseFileHead->StockCount == mid)
				{
					InsertPose = mid;
				}
				else
				{
					InsertPose = mid + 1;
				}
				break;
			}
			else if (strcmp(strStockKind, m_pBaseInfoPoint[mid]->Symbol) < 0)
			{
				InsertPose = mid;
				break;
			}
			else
			{
				pBaseItem = m_pBaseInfoPoint[mid];
				return TRUE;
			}
		}

		if (strcmp(m_pBaseInfoPoint[mid]->Symbol, strStockKind) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pBaseInfoPoint[mid]->Symbol, strStockKind) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			pBaseItem = m_pBaseInfoPoint[mid];
			return TRUE;
		}
	}

	if (high < low)
		InsertPose = low;


	if (m_BaseFileHead->StockCount + 1 > m_BaseFileHead->MaxStockCount)
	{
		AddStockTypeDataSize();
	}

	if (m_BaseFileHead->StockCount > InsertPose)
	{
		PBASEINFO* ptemp;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc(GPTR, (m_BaseFileHead->StockCount - InsertPose) * sizeof(PBASEINFO));
		hp = GlobalLock(hMem);
		if (hp)
		{
			ptemp = (PBASEINFO*)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错", MB_ICONSTOP);
			return FALSE;
		}

		memcpy(ptemp, &m_pBaseInfoPoint[InsertPose], (m_BaseFileHead->StockCount - InsertPose) * sizeof( PBASEINFO));
		memcpy(&m_pBaseInfoPoint[InsertPose + 1], ptemp, (m_BaseFileHead->StockCount - InsertPose) * sizeof( PBASEINFO));
		GlobalUnlock((HGLOBAL)ptemp);
		GlobalFree((HGLOBAL)ptemp);
	}

	m_pBaseInfoPoint[InsertPose] = m_pBaseInfo + m_BaseFileHead->StockCount;
	strcpy(m_pBaseInfoPoint[InsertPose]->Symbol, strStockKind);
	m_BaseFileHead->StockCount++;
	pBaseItem = m_pBaseInfoPoint[InsertPose];
	memset(pBaseItem, 0, sizeof(BASEINFO));
	strcpy(pBaseItem->Symbol, strStockKind);
	pBaseItem->NumSplit = 0;

	SaveBaseInfoToFile(m_BaseFileHead, sizeof(BASEINFOHEAD));
	SaveBaseInfoToFile(m_pBaseInfoPoint[InsertPose], sizeof(BASEINFO));

	return TRUE;
}

BOOL CSharesBaseInfo::AddStockItemCorrect(char *pStockCode,PBASEINFO  pBaseItem)
{
	int low=0;
	int high=m_BaseFileHead->StockCount -1 ;
	int mid=0;
	int InsertPose=-1;
	while(low <= high)
	{
		mid=(low+high)/2;
		if(low==high)
		{
			if(strcmp(pStockCode , m_pBaseInfoPoint[mid]->Symbol )>0)
			{
				if(m_BaseFileHead->StockCount==0||m_BaseFileHead->StockCount==mid)
					InsertPose=mid ;
				else
					InsertPose=mid +1;   
				break;
			}
			else if(strcmp(pStockCode , m_pBaseInfoPoint[mid]->Symbol)<0)
			{
				InsertPose=mid ;
				break;
			}
			else
			{
				return TRUE ;
			}
		}
		if(strcmp(m_pBaseInfoPoint[mid]->Symbol , pStockCode)>0) high=mid -1;
		else if(strcmp(m_pBaseInfoPoint[mid]->Symbol ,pStockCode)< 0 ) low=mid +1;
		else 
		{
			return TRUE ;
		}
	}
	if(high<low)
		InsertPose=low;


	if(m_BaseFileHead->StockCount + 1 >m_BaseFileHead->MaxStockCount)  
	{
		AddStockTypeDataSize();
	}
	if(m_BaseFileHead->StockCount > InsertPose)
	{
		PBASEINFO *ptemp;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc( GPTR, (m_BaseFileHead->StockCount - InsertPose )* sizeof( PBASEINFO) );
		hp=GlobalLock(hMem);
		if(hp)
		{
			ptemp= (PBASEINFO *)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		}
		memcpy(ptemp,&m_pBaseInfoPoint[InsertPose],(m_BaseFileHead->StockCount - InsertPose)*sizeof( PBASEINFO));
		memcpy(&m_pBaseInfoPoint[InsertPose+1],ptemp,(m_BaseFileHead->StockCount - InsertPose)*sizeof( PBASEINFO));
		GlobalUnlock((HGLOBAL)ptemp);        
		GlobalFree( (HGLOBAL)ptemp);
	}
	m_pBaseInfoPoint[InsertPose]=pBaseItem;
	m_BaseFileHead->StockCount++;
	strcpy(m_pBaseInfoPoint[InsertPose]->Symbol,pStockCode);
	return TRUE;
}

BOOL CSharesBaseInfo::Lookup(char* m_szStockId, int nKind, PBASEINFO& m_pStock)
{
	int low = 0;
	int high = m_BaseFileHead->StockCount - 1;
	int mid = 0;

	CTaiShanDoc* m_pDoc = ((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc;
	char strStockKind[10];
	CString strKind = m_pDoc->GetStockKindString(nKind);
	strcpy(strStockKind, strKind.GetBuffer(0));
	strcat(strStockKind, m_szStockId);

	while (low <= high)
	{
		mid = (low + high) / 2;
		if (strcmp(m_pBaseInfoPoint[mid]->Symbol, strStockKind) > 0)
		{
			high = mid - 1;
		}
		else if (strcmp(m_pBaseInfoPoint[mid]->Symbol, strStockKind) < 0)
		{
			low = mid + 1;
		}
		else 
		{
			m_pStock = m_pBaseInfoPoint[mid];
			return TRUE;
		}
	}

	m_pStock = NULL;

	return FALSE;
}
