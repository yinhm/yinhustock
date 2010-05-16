
#pragma once

class CStkFile
{
public:
	CStkFile();
	virtual ~CStkFile();

public:
	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };
	enum { hFileNull = -1 };
	enum OpenFlags
	{
		modeRead =          0x0000,
		modeWrite =         0x0001,
		modeReadWrite =     0x0002,
		shareCompat =       0x0000,
		shareExclusive =    0x0010,
		shareDenyWrite =    0x0020,
		shareDenyRead =     0x0030,
		shareDenyNone =     0x0040,
		modeNoInherit =     0x0080,
		modeCreate =        0x1000,
		modeNoTruncate =    0x2000,
		typeText =          0x4000, 
		typeBinary =   (int)0x8000 
	};
	enum Attribute
	{
		normal =    0x00,
		readOnly =  0x01,
		hidden =    0x02,
		system =    0x04,
		volume =    0x08,
		directory = 0x10,
		archive =   0x20
	};

protected:
	CMapStringToPtr*	m_pSymbolToPos;

	int		MaxNumStock;
	int		m_nAddReMap;
	BOOL	m_bCloseOnDelete;
	BOOL	m_bShareMem;
	CString	m_sNameShareMem;
	CString m_strFileName;

	UINT	m_hFile;
	HANDLE	m_hFileMap;
	BYTE*	m_lpvFileBegin;
	BYTE*	m_lpvFileEnd;
	BYTE*	m_lpvFileCurrent;
	DWORD	m_nLenFile;

public:
	void SeekToBegin();
	DWORD SeekToEnd();
	LONG Seek(LONG lOff, UINT nFrom);
	BYTE* GetFileBeginPointer();
	BYTE* GetFileCurrentPointer();

public:
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd = 0, CFileException* pException = NULL);
	virtual BOOL OpenShare(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd = 0, CString sShareName = "", CFileException* pException = NULL);
	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);
	virtual DWORD GetLength() const;
	virtual void Close();
	virtual void Flush();

public:
	virtual CString GetFileName() { return m_strFileName; }
	virtual int GetStockNumber();
	virtual void SetStockNumber(int nStock);
	virtual WORD GetMaxNumStock();
	virtual void SetMaxNumStock(WORD nMaxNumStock);
	virtual int GetSmallBlockCount();
	virtual void SetSmallBlockCount(int nBlock);

protected:
	virtual int GetID();
	virtual void ReMapFromBegin(int nCount);
	virtual BOOL ReMap(int nAdd = 1024);
};
