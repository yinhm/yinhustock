#if !defined(AFX_LBSINFORS_H__244F0325_A171_11D2_91E5_0080C8E1242B__INCLUDED_)
#define AFX_LBSINFORS_H__244F0325_A171_11D2_91E5_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxDao.h>

class LBsInfoRS : public CDaoRecordset
{
public:
	CString m_sSql;
	LBsInfoRS(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(LBsInfoRS)


	//{{AFX_FIELD(LBsInfoRS, CDaoRecordset)
	CString	m_symbol;
	CString	m_content;
	COleDateTime	m_time;
	CString	m_brief;
	//}}AFX_FIELD

	//{{AFX_VIRTUAL(LBsInfoRS)
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

#endif // !defined(AFX_LBSINFORS_H__244F0325_A171_11D2_91E5_0080C8E1242B__INCLUDED_)
