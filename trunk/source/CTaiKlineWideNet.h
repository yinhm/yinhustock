// CTaiKlineWideNet.h: interface for the CTaiKlineWideNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_)
#define AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CTaiShanKlineShowView;

class CTaiKlineWideNet  
{
public:
	void AddStockKlineDataAll(CString symbol,int stkKind, int nKlineType,bool bAll = false);
	void AddStockHsFileDataAll(CString symbol,int stkKind,bool bAll = false);
	void AddStockHsHistoryFileData(CString symbol,int stkKind,CString sDate);
	void SetHsHistoryRequestFlag(bool bRequested);
	void Request();
	void ReleaseID();
	
	void RegisterID();
	
	int GetRequestKlineCount(CString symbol,int stkKind,bool bDay = true);
	
	void AddStockFirst(CString symbol,int stkKind,bool m_bAutoDeletePre = true);

	void AddStockHsFileData(CString symbol,int stkKind);
	
	void AddStockKlineData(CString symbol,int stkKind,int nKlineType);
	
	void AddStockHs(CString symbol,int stkKind,bool m_bAutoDeletePre = true);
	CTaiKlineWideNet();
	CTaiKlineWideNet(CTaiShanKlineShowView * pView);
	virtual ~CTaiKlineWideNet();

private:
	int GetRequestHsCount(CString symbol,int stkKind);
	int GetRequestMin1Count(CString symbol,int stkKind);
	int GetDaysToToday(Kline* pKline);
	CTaiShanKlineShowView* m_pView;

	CString				m_symbolPre;
	int					m_stkKindPre;

	bool				m_bDayRequested;
	bool				m_bMin5Requested;
	bool				m_bMin1Requested;
	bool				m_bHsRequested;
	bool				m_bHsHistoryRequested;
	CString				m_sDateHistory;

	static Kline*		m_pKline1A0001;
	static int			m_nKline1A0001;
	static int			m_nKlineToToday;

	
	SOCKET_ID	g_socketID[3];
};

#endif // !defined(AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_)
