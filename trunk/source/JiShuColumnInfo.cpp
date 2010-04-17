// JiShuColumnInfo.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "Mainfrm.h"
#include "CTaiShanDoc.h"
#include "CTaiShanApp.h"
#include "JiShuColumnInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SYSTEMHEAD     1000

/////////////////////////////////////////////////////////////////////////////
// JiShuColumnInfo

TechniqueColumnInfo::TechniqueColumnInfo(CTaiShanDoc *pDoc)
{
	m_pDoc=pDoc;

}
TechniqueColumnInfo::~TechniqueColumnInfo()
{
}

/////////////////////////////////////////////////////////////////////////////


UINT TechniqueColumnInfo::FindSolt(PTECHNIQUE_COLUMN &pTechniqueIndex)
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(!m_pDoc->m_SystemInitData.TechniqueColumn[i].IsUse)
		{
			pTechniqueIndex=&m_pDoc->m_SystemInitData.TechniqueColumn[i];
			return m_pDoc->m_SystemInitData.TechniqueColumn[i].nID; 
		}
	}
	return -1; 
}
BOOL TechniqueColumnInfo::GetTechniqueFromId(UINT nId,PTECHNIQUE_COLUMN &pTechniqueIndex)
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(m_pDoc->m_SystemInitData.TechniqueColumn[i].nID ==nId)
		{
			pTechniqueIndex=&m_pDoc->m_SystemInitData.TechniqueColumn[i];
			return TRUE; 
		}
	}
	pTechniqueIndex=NULL;
	return FALSE; 
}
BOOL TechniqueColumnInfo::GetTechniqueFromIndexName(CString IndexName ,PTECHNIQUE_COLUMN &pTechniqueIndex)
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(IndexName==m_pDoc->m_SystemInitData.TechniqueColumn[i].nIndex.sIndex_name)
		{
			pTechniqueIndex=&m_pDoc->m_SystemInitData.TechniqueColumn[i];
			return TRUE; 
		}
	}
	pTechniqueIndex=NULL;
	return FALSE; 
}
void TechniqueColumnInfo::SetColumnNotUse(UINT nId)
{ 
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		if(m_pDoc->m_SystemInitData.TechniqueColumn[i].nID ==nId)
		{
			m_pDoc->m_SystemInitData.TechniqueColumn[i].IsUse=FALSE ; 
		}
	}
}
void TechniqueColumnInfo::SetAllColumnNotUse()
{
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
		m_pDoc->m_SystemInitData.TechniqueColumn[i].IsUse=FALSE ; 
	}
}

