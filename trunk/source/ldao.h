#if !defined(AFX_LDAO_H__2935D1E5_BB4B_11D2_91DF_0080C8E1242B__INCLUDED_)
#define AFX_LDAO_H__2935D1E5_BB4B_11D2_91DF_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxdao.h>

class ldao : public CDaoRecordset
{
public:
	ldao(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(ldao)


	//{{AFX_FIELD(ldao, CDaoRecordset)
	//}}AFX_FIELD


	//{{AFX_VIRTUAL(ldao)
	public:
	virtual CString GetDefaultDBName();		
	virtual CString GetDefaultSQL();	
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);
	//}}AFX_VIRTUAL


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDAO_H__2935D1E5_BB4B_11D2_91DF_0080C8E1242B__INCLUDED_)
