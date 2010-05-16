
#if !defined(AFX_LMBMEMFILE_H__3FADA7E1_AE48_11D3_96E2_0080C8E1242B__INCLUDED_)
#define AFX_LMBMEMFILE_H__3FADA7E1_AE48_11D3_96E2_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StkFile.h"

class CTaiKlineMemFile : public CStkFile
{
public:
	CTaiKlineMemFile();
	CTaiKlineMemFile(LPCTSTR lpszFileName, UINT nOpenFlags);
	virtual ~CTaiKlineMemFile();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBMEMFILE_H__3FADA7E1_AE48_11D3_96E2_0080C8E1242B__INCLUDED_)
