// CTaiKlineWideNet.cpp: implementation of the CTaiKlineWideNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineWideNet.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
//#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

Kline*		CTaiKlineWideNet::m_pKline1A0001 = NULL;
int			CTaiKlineWideNet::m_nKline1A0001 = 0;
int			CTaiKlineWideNet::m_nKlineToToday = 0;


CTaiKlineWideNet::CTaiKlineWideNet()
{
}

CTaiKlineWideNet::CTaiKlineWideNet(CTaiShanKlineShowView * pView)
{
	m_pView = pView;

	m_symbolPre = _T("");
	m_sDateHistory = _T("");
	m_stkKindPre = 0;
	
	m_bDayRequested		= false;
	m_bMin5Requested	= false;
	m_bMin1Requested	= false;
	m_bHsRequested		= false;
	m_bHsHistoryRequested = false;


}

CTaiKlineWideNet::~CTaiKlineWideNet()
{

}

void CTaiKlineWideNet::AddStockHs(CString symbol,int stkKind,bool m_bAutoDeletePre)
{
#ifdef WIDE_NET_VERSION
#endif

}

void CTaiKlineWideNet::AddStockKlineData(CString symbol,int stkKind, int nKlineType)
{

	AddStockKlineDataAll( symbol,stkKind,  nKlineType,  false);
}

void CTaiKlineWideNet::AddStockHsFileData(CString symbol,int stkKind)
{
	if(m_bHsRequested	== false && (m_pView->GetDataKind () == HS_K_LINE ||m_pView->pMin1Drawing->m_bHist == true ))
	{
		AddStockHsFileDataAll(symbol,stkKind);
	}
}

void CTaiKlineWideNet::AddStockFirst(CString symbol,int stkKind,bool m_bAutoDeletePre)
{

	if(m_symbolPre == symbol
		&&m_stkKindPre == stkKind
		&&m_bDayRequested	== true
		&&m_bMin5Requested	== true
		&&m_bMin1Requested	== true
		&&m_bHsRequested	== true

		) 
	{
		return;
	}

	if(m_bAutoDeletePre == true)
	{
		if(m_symbolPre != symbol
			&&m_stkKindPre == stkKind
			)
		{
			m_bDayRequested		= false;
			m_bMin5Requested	= false;
			m_bMin1Requested	= false;
			m_bHsRequested		= false;
		}
		m_symbolPre = symbol;
		m_stkKindPre = stkKind;
	}


	{

		AddStockHs(symbol,stkKind,true);
	
		AddStockKlineData( symbol,stkKind, 5);

	
		AddStockHsFileData(symbol,stkKind);

	}

}

int CTaiKlineWideNet::GetRequestKlineCount(CString symbol,int stkKind, bool bDay)
{
	int n = 0;
	Kline * pKline = NULL;
	int nRead = CTaiKlineFileKLine::ReadKLineS(symbol,stkKind,pKline,1,0,bDay);

	int nRate = 1;
	if(bDay == false) nRate = 48;

	if(nRead!=1 ) n = 6000;
	else
	{
		n = GetDaysToToday(pKline)*nRate;
	}

	if(pKline!=NULL) delete[] pKline;
	return n;
}

int CTaiKlineWideNet::GetDaysToToday(Kline *pKline)
{
	int n = 0;
	CTime tm(pKline[0].day );
	CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay (),0,0,0);

	CTime tmCurr = CTime::GetCurrentTime ();
	int nDayOfWeek = tmCurr.GetDayOfWeek ();
	CTimeSpan tmSp = tmCurr - tm2;
	int nDay = tmSp.GetDays();

	nDayOfWeek--;
	if(nDayOfWeek==0) nDayOfWeek = 7;

	if(nDay <= 1)
		n = 0;
	else 
	{
		if(nDay < nDayOfWeek )
			n =  nDayOfWeek - nDay + 1;
		else
		{
			n = nDay-(nDay-nDayOfWeek+5)/7*2;
		}
	}

	if(n == 0) n= -1;
	return n;
}

void CTaiKlineWideNet::RegisterID()
{
#ifdef WIDE_NET_VERSION
#endif
}

void CTaiKlineWideNet::ReleaseID()
{
#ifdef WIDE_NET_VERSION
#endif

}

void CTaiKlineWideNet::Request()
{
	m_bDayRequested		= false;
	m_bMin5Requested	= false;
	m_bMin1Requested	= false;
	m_bHsRequested		= false;


	AddStockFirst(m_pView->m_sharesSymbol,m_pView->m_stkKind );

}

int CTaiKlineWideNet::GetRequestMin1Count(CString symbol,int stkKind)
{
	int n = 0;
	CReportData * pdt = NULL;
	if(CMainFrame::m_taiShanDoc ->m_sharesInformation.Lookup(symbol.GetBuffer (0),pdt,stkKind)==0)
		return 0;
	if(pdt == NULL) return 0;

	n = pdt->lastclmin ;

	return n;

}

int CTaiKlineWideNet::GetRequestHsCount(CString symbol,int stkKind)
{
	int n = 0;
	n = CTaiKlineFileHS::GetCountPre(symbol,stkKind);
	return n;

}

void CTaiKlineWideNet::SetHsHistoryRequestFlag(bool bRequested)
{
	m_bHsRequested = bRequested;
}

void CTaiKlineWideNet::AddStockHsHistoryFileData(CString symbol,int stkKind, CString sDate)
{
#ifdef WIDE_NET_VERSION
#endif

}

void CTaiKlineWideNet::AddStockHsFileDataAll(CString symbol,int stkKind, bool bAll)
{
#ifdef WIDE_NET_VERSION
#endif


}

void CTaiKlineWideNet::AddStockKlineDataAll(CString symbol,int stkKind, int nKlineType, bool bAll)
{
}
