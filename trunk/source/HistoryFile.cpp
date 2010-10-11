// HistoryFile.cpp: implementation of the CHistoryFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "HistoryFile.h"
#include "MainFrm.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineTransferKline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistoryFile::CHistoryFile()
{
	m_pKlineIndex = NULL;
	CString symbol = "1A0001" ;
	CTaiKlineFileKLine* pFileK = CTaiKlineFileKLine::GetFilePointer (symbol,0,true);

	m_pKlineIndex = NULL;
	std::string code(symbol);
	m_nKlineIndex = pFileK->ReadKLine (code,m_pKlineIndex,-1);

	m_strHistoryDate = "";
	BOOL bOpen = this->Open ("HistoryRealData.dat",0, 0);
	ASSERT(bOpen );
}

CHistoryFile::~CHistoryFile()
{

}

CReportData * CHistoryFile::WriteOne(CReportData *pdt, int nFoot)
{
	if(m_strHistoryDate == "") return NULL;
	if(nFoot>10000 || nFoot<0) return NULL;
	CReportData dt = *pdt;
	dt.vbuy1 = 0;
	dt.vbuy2 = 0;
	dt.vbuy3 = 0;
	dt.vsel1 = 0;
	dt.vsel2 = 0;
	dt.vsel3 = 0;
	dt.pbuy1 = 0;
	dt.pbuy2 = 0;
	dt.pbuy3 = 0;
	dt.psel1 = 0;
	dt.psel2 = 0;
	dt.psel3 = 0;
	dt.volume5 = 0;
	dt.accb = 0;
	dt.accs = 0;
	dt.dvol = 0;
	dt.rvol =0;

	
	CString symbol = dt.id ;
	Kline * pKline = NULL;
	CTaiKlineFileKLine* pFileK = CTaiKlineFileKLine::GetFilePointer (symbol,dt.kind ,true);
	int nd = atoi(m_strHistoryDate);
	CTime tm(nd/10000,nd%10000/100,nd%100,0,0,0);
	int n = pFileK->ReadKlineAccordingTime(symbol, pKline, tm,2); 

	dt.opnp = 0;
	dt.nowp  =0;
	dt.higp  =0;
	dt.lowp  =0;
	dt.totv = 0;
	dt.totp  = 0;
	dt.ystc =0;
	dt.nowp =0;
	if(n>=2) 
	{
	dt.opnp = pKline[1].open ;
	dt.nowp  = pKline[1].close ;
	dt.higp  = pKline[1].high ;
	dt.lowp  = pKline[1].low ;
	dt.totv = pKline[1].vol ;
	dt.totp  = pKline[1].amount ;
	dt.ystc = pKline->close ;
	}

	if(pKline!=NULL) delete []pKline;


	this->Seek (nFoot*sizeof(CReportData),this->begin);
	this->Write (&dt,sizeof(CReportData));
	this->Seek (nFoot*sizeof(CReportData),this->begin);
	return (CReportData *)(this->GetFileCurrentPointer ());
}

CReportData * CHistoryFile::GetOne(int nFoot)
{
	if(m_strHistoryDate == "") return NULL;
	if(nFoot>10000 || nFoot<0) return NULL;
	this->Seek (nFoot*sizeof(CReportData),this->begin);
	return (CReportData *)(this->GetFileCurrentPointer ());
}

void CHistoryFile::GetDateNavigated(bool bMovePre)
{
	if(m_pKlineIndex ==NULL || m_nKlineIndex <=0 ||m_strHistoryDate == "") 
	{
		ASSERT(FALSE);
		return;
	}

	for(int i = m_nKlineIndex-1;i>=0;i--)
	{
		CString s = CTime(m_pKlineIndex[i].day ).Format ("%Y%m%d");
		if(bMovePre == true)
		{
			if(s<m_strHistoryDate)
			{
				m_strHistoryDate = s;
				break;
			}
		}
		else
		{
			if(s<=m_strHistoryDate)
			{
				if(i<m_nKlineIndex-1)
				{
					s = CTime(m_pKlineIndex[i+1].day ).Format ("%Y%m%d");
					m_strHistoryDate = s;
				}
				break;
			}
		}
	}

}

void CHistoryFile::MoveToDate(CString sDate)
{
	if(m_pKlineIndex ==NULL || m_nKlineIndex <=0 ) 
	{
		ASSERT(FALSE);
		return;
	}

	for(int i = m_nKlineIndex-1;i>=0;i--)
	{
		CString s = CTime(m_pKlineIndex[i].day ).Format ("%Y%m%d");
		if(s<=sDate)
		{
			m_strHistoryDate = s;
			break;
		}
	}

}
