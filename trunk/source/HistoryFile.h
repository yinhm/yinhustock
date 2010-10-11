
#if !defined(AFX_HISTORYFILE_H__7E61FC02_C873_11D1_968C_8B4A225F8378__INCLUDED_)
#define AFX_HISTORYFILE_H__7E61FC02_C873_11D1_968C_8B4A225F8378__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CTaiKlineMemFile.h"

class CHistoryFile : public CStkFile  
{
public:
	void GetDateNavigated(bool bMovePre);
	CString m_strHistoryDate;
	CReportData * GetOne( int nFoot);
	CReportData * WriteOne(CReportData * pdt,int nFoot);
	CHistoryFile();
	virtual ~CHistoryFile();
private:
	Kline * m_pKlineIndex;
	int m_nKlineIndex;

protected:
	void MoveToDate(CString sDate);
};

#endif // !defined(AFX_HISTORYFILE_H__7E61FC02_C873_11D1_968C_8B4A225F8378__INCLUDED_)
