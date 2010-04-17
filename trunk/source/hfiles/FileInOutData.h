// FileInOutData.h: interface for the FileInOutData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINOUTDATA_H__54F8E6A1_7ECA_11D6_968D_B381BDAEB375__INCLUDED_)
#define AFX_FILEINOUTDATA_H__54F8E6A1_7ECA_11D6_968D_B381BDAEB375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IndicatorExtendData.h"

class AFX_EXT_CLASS FileInOutData  
{
public:
	static bool RemoveFiles(CString &sPath);
	FileInOutData();
	virtual ~FileInOutData();

	static CArray<CString,CString&> m_stockSymbolArr;
	static int m_stockSymbolTime;

	void WriteSymbolFile(CString sFileName,CString& sArr,int time);
	bool SetSymbolFile(CString sFileName);
	bool ImportData(CString sFileName,CProgressCtrl* pCtrl);
	bool ExportData(CStringArray& sFileName,int* pIndicatorExtendID,int nDays);
	bool ExportData(CStringArray& sFileName,int* pIndicatorExtendID,CTime& tmBegin,CTime& tmEnd);
	bool ExportRealMin5(CStringArray& sSymbol,float * fValue,int nIndicatorExtendID,int nRecord);

private:
	static bool IsBrief(CString sFileName) ;
	static InOutHeadData GetHead(CFile & fl);
	static void SetHead(InOutHeadData& head,CFile & fl);
	bool ExportData(CStringArray& sFileName,int* pIndicatorExtendID,int nDays,CTime* tmBegin,CTime* tmEnd);
};

#endif // !defined(AFX_FILEINOUTDATA_H__54F8E6A1_7ECA_11D6_968D_B381BDAEB375__INCLUDED_)
