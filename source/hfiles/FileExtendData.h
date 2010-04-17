// FileExtendData.h: interface for the FileExtendData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEEXTENDDATA_H__66FEC7EA_7B03_11D6_968D_A8145FF7C575__INCLUDED_)
#define AFX_FILEEXTENDDATA_H__66FEC7EA_7B03_11D6_968D_A8145FF7C575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IndicatorExtendData.h"

class AFX_EXT_CLASS FileExtendData  
{
public:
	void GetIndicatorInfo(indicatorInfoArr & infoArr);
	FileExtendData();
	virtual ~FileExtendData();

	static bool GetIndicatorInfo(IndicatorInfo& data,int nIndicatorExtendID);
	static bool SetIndicatorInfo(IndicatorInfo& data,int nIndicatorExtendID);

	static void Remove(int nIndicatorExtendID);


	int WriteData(RecordIndicator* pData,CString sSymbol,int nIndicatorExtendID,int nCount) ;
	int ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,int nDays);
	int ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,CTime& tmBegin,CTime& tmEnd);

	static int RegularTime(int time,bool bDay);
	static void CloseNow();

private:
	static 	CMap<int,int,IndicatorInfo*,IndicatorInfo*&> m_map;
	static void ClearMap();

	bool	GetFile(CFile& fl,CString sSymbol,int nIndicatorExtendID);
	ExtHeadData	GetFileHead(CFile& fl);
	void	SetFileHead(ExtHeadData &head,CFile& fl);
};

#endif // !defined(AFX_FILEEXTENDDATA_H__66FEC7EA_7B03_11D6_968D_A8145FF7C575__INCLUDED_)
