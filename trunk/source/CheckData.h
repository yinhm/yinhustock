// by ÓáÃ÷Â¼

#if !defined(AFX_CHECKDATA_H__E99849C2_40E0_11D3_BF3B_0000E86564F7__INCLUDED_)
#define AFX_CHECKDATA_H__E99849C2_40E0_11D3_BF3B_0000E86564F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCheckData  
{
public:
	CCheckData();
	virtual ~CCheckData();
	BOOL MaxBlock_SH(); 
	BOOL MaxBlock_SZ(); 
	BOOL OverlapBlock_SH();
	BOOL OverlapBlock_SZ(); 
	BOOL OverlapSymbol_SH();
	BOOL OverlapSymbol_SZ();



};

#endif // !defined(AFX_CHECKDATA_H__E99849C2_40E0_11D3_BF3B_0000E86564F7__INCLUDED_)
