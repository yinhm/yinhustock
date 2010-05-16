
#pragma once

//class CStkReportFile;
class CTaiKlineFileKLine;

class CStkDatabase
{
public:
	CStkDatabase();
	virtual ~CStkDatabase();

public:
	static CStkDatabase* s_pInstance;

protected:
	CString m_strPath;
	//CStkReportFile* m_pStkReport;

public:
	BOOL InitInstance();

public:
	CString GetAppPath();

public:
	//CStkReportFile* GetReportFile(WORD wMarket);
	CTaiKlineFileKLine* GetKLineFile(WORD wMarket);
};

CStkDatabase* TSKDatabase();

inline CStkDatabase* TSKDatabase()
{
	if (CStkDatabase::s_pInstance == NULL)
	{
		static CStkDatabase sInstance;
		return &sInstance;
	}

	return CStkDatabase::s_pInstance;
}
