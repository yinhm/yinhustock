// IndicatorExtendData.h: interface for the IndicatorExtendData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATOREXTENDDATA_H__66FEC7E9_7B03_11D6_968D_A8145FF7C575__INCLUDED_)
#define AFX_INDICATOREXTENDDATA_H__66FEC7E9_7B03_11D6_968D_A8145FF7C575__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct IndicatorInfo{
	int m_nFileID;
	bool m_bServer;
	int m_nKlineKind;
	int m_nKlineKindCompute;
	int m_nID;
	int m_reserved;
	char m_chName[16];
	char m_chSubName[16];
	char m_chDataName[16];
	float fParam[8];

	IndicatorInfo()
	{
		m_nFileID = 'indi';
		m_nKlineKind = 5;
		m_nKlineKindCompute = 5;
		m_bServer = false;
		m_chName[0] = 0;
		m_chSubName[0] = 0;
		m_chDataName[0] = 0;
	};
};
struct ExtHeadData{
	int m_nFileID;
	int m_nCount;
	ExtHeadData()
	{
		m_nFileID = 'head';
		m_nCount = 0;
	};
};
struct RecordIndicator{
	int m_time;
	float m_fValue;
};


struct InOutHeadData{
	IndicatorInfo m_info;
	int m_nStockCount;		
	int m_time;				
	InOutHeadData()
	{
		m_nStockCount = -1;
		m_time= -1;
	}
};

typedef CArray<IndicatorInfo,IndicatorInfo&> indicatorInfoArr;

const char g_chIndicatorPath[] = "data\\indicator\\";
class AFX_EXT_CLASS IndicatorExtendData  
{
public:
	IndicatorExtendData();
	virtual ~IndicatorExtendData();
	static CString m_sCurrentDirctory;
	static int m_nMaxCountRecord;

	static void GetIndicatorInfo(indicatorInfoArr &infoArr);

	bool CreateDirectoryToday(bool bClear);


	void WriteSymbolFile(CString sFileName,CString& sArr,int time);
	bool SetSymbolFile(CString sFileName);
	bool ImportData(CString sFileName,CProgressCtrl* pCtrl);
	bool ExportData(CStringArray& sFileName,int* pIndicatorExtendID,int nDays);
	bool ExportData(CStringArray& sFileName,int* pIndicatorExtendID,CTime& tmBegin,CTime& tmEnd);
	bool ExportRealMin5(CStringArray& sSymbol,float * fValue,int nIndicatorExtendID,int nRecord);


	void SetMaxCountRecord(int nMax);
	int WriteData(RecordIndicator* pData,CString sSymbol,int nIndicatorExtendID,int nCount) ;
	int ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,int nDays);
	int ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,CTime& tmBegin,CTime& tmEnd);

public:
};

#endif // !defined(AFX_INDICATOREXTENDDATA_H__66FEC7E9_7B03_11D6_968D_A8145FF7C575__INCLUDED_)
