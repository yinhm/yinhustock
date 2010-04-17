
class CSpz
{
private:
	char (*m_strData)[4];	
	int  m_nCount;		
public:

	CSpz()
	{
		m_strData=NULL;
		m_nCount=0;
	}

	BOOL Open(const char* strFileName=NULL);
	

	UINT GetCount()
	{
		return m_nCount;
	}

	BOOL GetPy(const char* strHz, char &sPy);
	BOOL GetHzpy(int Index, CString &strHz, CString& strPy);


	void Close();


	BOOL QuickSort(int low,int high);

	~CSpz(){}
};

typedef struct _tagDYZSTRUCT
{
	char strHz[9];	
	char strPy[7];	
}DYZSTRUCT, *PDYZSTRUCT;

class CDyz
{
private:
	DYZSTRUCT* m_pData;
	int m_nCount;
	static int m_nSize;
public:
	CDyz()
	{
		m_pData=NULL;
		m_nCount=0;
	}

	
	BOOL Open(const char* strFileName=NULL);
	

	UINT GetCount()
	{
		return m_nCount;
	}

	
	BOOL GetPy(const char* strName, char* strPy);
	BOOL GetHzpy(int Index, CString &strHz, CString& strPy);

	void Close();


	BOOL QuickSort(int low,int high);


	~CDyz(){}
};