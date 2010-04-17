

#if !defined(AFX_BLOCKTYPEMANAGER_H__88285B9B_F502_4982_8948_95BC91DDD7CC__INCLUDED_)
#define AFX_BLOCKTYPEMANAGER_H__88285B9B_F502_4982_8948_95BC91DDD7CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BlockTypeManager  
{
public:
	static void DeleteSymbols(CString sBlockName, CStringArray &sSymbolsArr);
	static void AddSymbols(CString sBlockName,CStringArray &sSymbolsArr);
	static void ReadSymbols(CString sBlockName,CStringArray &sSymbolsArr);
	void EnumBlockNames(CString sTypeName,CStringArray& sBlockNameArr);
	void EnumAllType(CStringArray& sTypeArr);
	void DeleteBlockType(CString sName);
	int ModifyBlockTypeName(CString sOldName,CString sNewName);
	int AddBlockType(CString sName);
	BlockTypeManager();
	virtual ~BlockTypeManager();
private:
	CString m_sNameFileType;

protected:
	static void DeleteSymbols(CString sBlockName, CStringArray &sSymbolsArr,bool bAppend);
};

#endif // !defined(AFX_BLOCKTYPEMANAGER_H__88285B9B_F502_4982_8948_95BC91DDD7CC__INCLUDED_)
