#include <afxdao.h>
#if !defined(AFX_DBZLLBSH_H__08459AC9_CB95_11D1_83F0_0000E8593F1A__INCLUDED_)
#define AFX_DBZLLBSH_H__08459AC9_CB95_11D1_83F0_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBZLLBSH : public CDaoRecordset
{
public:
	CDBZLLBSH(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDBZLLBSH)


	//{{AFX_FIELD(CDBZLLBSH, CDaoRecordset)
	long	m_index;
	CString	m_symbol;
	CString	m_name;
	long	m_time;
	double	m_captialstock;
	double	m_foundermemberstock;
	double	m_nationalstock;
	double	m_innerstock;
	double	m_foreignstock;
	double	m_gathermemberstock;
	double	m_employeestock;
	double	m_corporationmatch;
	double	m_Asection;
	double	m_currencyA;
	double	m_Bsection;
	double	m_Hsection;
	double	m_totalasset;
	double	m_currentassert;
	double	m_moneyassert;
	double	m_fixassert;
	double	m_nofixassert;
	double	m_stockintrade;
	double	m_recievefund;
	double	m_payfund;
	double	m_longtimeinverse;
	double	m_longtimedebt;
	double	m_currentliabilites;
	double	m_capitalaccumulation;
	double	m_surplusaccumulation;
	double	m_legalaccumulation;
	double	m_nodistributeprofit;
	double	m_totalvalue;
	double	m_mainbusinessgain;
	double	m_totalprofit;
	double	m_netprofit;
	double	m_mainbussinessprofit;
	double	m_stockholderinterests;
	double	m_eachstockincrease;
	double	m_eachstockasset;
	//}}AFX_FIELD


	//{{AFX_VIRTUAL(CDBZLLBSH)
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

#endif // !defined(AFX_DBZLLBSH_H__08459AC9_CB95_11D1_83F0_0000E8593F1A__INCLUDED_)
