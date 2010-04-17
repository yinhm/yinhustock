#if !defined(AFX_LMBMEMFILE_H__3FADA7E1_AE48_11D3_96E2_0080C8E1242B__INCLUDED_)
#define AFX_LMBMEMFILE_H__3FADA7E1_AE48_11D3_96E2_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapStringToPtr;
class CTaiKlineMemFile 
{




public:
	CTaiKlineMemFile();         


public:

	CMapStringToPtr* m_pSymbolToPos;
	enum OpenFlags {
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

	enum Attribute {
		normal =    0x00,
		readOnly =  0x01,
		hidden =    0x02,
		system =    0x04,
		volume =    0x08,
		directory = 0x10,
		archive =   0x20
		};

	enum SeekPosition { begin = 0x0, current = 0x1, end = 0x2 };

	enum { hFileNull = -1 };


	CTaiKlineMemFile(LPCTSTR lpszFileName, UINT nOpenFlags);



	void SetSmallBlockCount(int nBlock);
	int GetSmallBlockCount();
	void SetStockNumber(int nStock);
	int GetStockNumber();

	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd =0 ,CFileException* pException = NULL);
	virtual BOOL OpenShare(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd =0 ,CString sShareName = "",CFileException* pException = NULL);

	DWORD SeekToEnd();
	void SeekToBegin();



	LONG Seek(LONG lOff, UINT nFrom);
	virtual DWORD GetLength() const;

	virtual UINT Read(void* lpBuf, UINT nCount);
	virtual void Write(const void* lpBuf, UINT nCount);




	virtual void Flush();
	virtual void Close();


public:
	CString GetFileName(){return m_strFileName;}
protected:
	void SetMaxNumStock(WORD nMaxNumStock);
	WORD GetMaxNumStock();
	int MaxNumStock;
	BOOL m_bCloseOnDelete;
	CString m_strFileName;
	int m_nAddReMap;


	UINT m_hFile;
	HANDLE m_hFileMap;
	BYTE* m_lpvFileBegin;
	BYTE* m_lpvFileEnd;
	BYTE* m_lpvFileCurrent;
	DWORD	m_nLenFile;


	


public:
	int GetID();
	void ReMapFromBegin(int nCount);
	BYTE* GetFileCurrentPointer();
	BYTE * GetFileBeginPointer();
	virtual ~CTaiKlineMemFile();
	bool m_bShareMem;
	CString m_sNameShareMem;

private:
	bool ReMap(int nAdd = 1024);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBMEMFILE_H__3FADA7E1_AE48_11D3_96E2_0080C8E1242B__INCLUDED_)
