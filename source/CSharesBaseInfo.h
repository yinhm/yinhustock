
#if !defined(AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StkFile.h"

class CSharesBaseInfo : public CStkFile
{
public:
	CSharesBaseInfo();
	virtual ~CSharesBaseInfo();

protected:
	BASEINFOHEAD*	m_BaseFileHead;
	BASEINFO*		m_pBaseInfo;
	PBASEINFO*		m_pBaseInfoPoint;

protected:
	BOOL InitBaseInfoDataEmpty();
	BOOL InitBaseInfoDataExist();
	BOOL SavePosToFile();
	BOOL SetMemroyALLOCSize(unsigned int nSize);
	BOOL AddStockTypeDataSize();
	BOOL InsertItemPoint(BASEINFO* m_pStk);

public:
	BOOL InitBaseInfoData(CString Path);
	BOOL AddStockItem(char* pStockCode, int nKind, PBASEINFO& pBaseItem);
	BOOL AddStockItemCorrect(char* pStockCode, PBASEINFO pBaseItem);
	BOOL Lookup(char* m_szStockId, int nKind, PBASEINFO& m_pStock);
	BOOL ReadBaseInfoData(char* StockCode, int nKind, PBASEINFO& pBaseItem);
	void SaveBaseInfoToFile(LPCVOID lpBaseAddress, DWORD dwNumberOfBytesToFlush);

public:
	LONG GetStockCount() { return m_BaseFileHead->StockCount; };
	BOOL GetStockItem(int ls_pos, PBASEINFO& pBaseItem);
};

inline void CSharesBaseInfo::SaveBaseInfoToFile(LPCVOID lpBaseAddress, DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush);
}

inline BOOL CSharesBaseInfo::GetStockItem(int nPos, PBASEINFO& pBaseItem)
{
	if (nPos< m_BaseFileHead->StockCount)
	{
		pBaseItem = &m_pBaseInfo[nPos];
		return TRUE;
	}

	return FALSE;
}

#endif // !defined(AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
