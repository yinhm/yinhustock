#if !defined(AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSharesBaseInfo  
{
public:
	CSharesBaseInfo();
	virtual ~CSharesBaseInfo();
private:
	HANDLE m_hFile,m_hFileMap;          
	BYTE  *m_pbData;                      
	BASEINFOHEAD *m_BaseFileHead;             
	BASEINFO *m_pBaseInfo;                    
	PBASEINFO *m_pBaseInfoPoint;

	char m_sFilePath[255];                 
public:
	LONG GetStockCount() { return m_BaseFileHead->StockCount ;} ; 
	BOOL GetStockItem(int ls_pos,PBASEINFO & pBaseItem);        
	BOOL AddStockItem(char *pStockCode,int nKind,PBASEINFO & pBaseItem);      
	BOOL ReadBaseInfoData(char *StockCode,int nKind,PBASEINFO & pBaseItem);
	void ReadBaseInfoDataBJW(char *pFileData,PBASEINFO & pBaseItem); 
	void ReadBaseInfoDataWG(char *pFileData,PBASEINFO & pBaseItem);  
	BOOL Lookup(char *m_szStockId,int nKind,PBASEINFO &m_pStock);
	BOOL InsertItemPoint(BASEINFO *m_pStk);            
	BOOL SavePosToFile();

public:
	BOOL InitBaseInfoData(CString Path); 
	void SaveBaseInfoToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);  
	BOOL InitBaseInfoDataEmpty(); 
	BOOL InitBaseInfoDataExist(); 
	BOOL SetMemroyALLOCSize(unsigned int nSize);
	BOOL AddStockItemCorrect(char *pStockCode,PBASEINFO  pBaseItem);
	BOOL AddStockTypeDataSize();

private:
	void SetBaseInfoData(CString mc,float zbsj,PBASEINFO & pBaseItem,int mode=0);  

};
inline BOOL CSharesBaseInfo::GetStockItem(int ls_pos,PBASEINFO & pBaseItem)
{
	if(ls_pos >= m_BaseFileHead->StockCount )
		return FALSE;
	else
	{
		pBaseItem=&m_pBaseInfo[ls_pos];
		return TRUE;
	}

}
inline void CSharesBaseInfo::SaveBaseInfoToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress,dwNumberOfBytesToFlush);
}
#endif // !defined(AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
