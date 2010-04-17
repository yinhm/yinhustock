
#if !defined(AFX_GETSETREG_H__2A1FBA1D_E399_11D3_96E7_0080C8E1242B__INCLUDED_)
#define AFX_GETSETREG_H__2A1FBA1D_E399_11D3_96E7_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GetSetReg 
{
	
public:
	GetSetReg();
	virtual ~GetSetReg();

public:
	int GetValue(CString strKey, CString strValueName, DWORD& dwType, BYTE *lpData, DWORD& cbData);
	int SetValue(CString strKey, CString strValueName, DWORD dwType, BYTE *lpData, DWORD cbData);

};

#endif // !defined(AFX_GETSETREG_H__2A1FBA1D_E399_11D3_96E7_0080C8E1242B__INCLUDED_)
