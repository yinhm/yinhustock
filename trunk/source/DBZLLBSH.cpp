// DBZLLBSH.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DBZLLBSH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDBZLLBSH

IMPLEMENT_DYNAMIC(CDBZLLBSH, CDaoRecordset)

CDBZLLBSH::CDBZLLBSH(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDBZLLBSH)
	m_index = 0;
	m_symbol = _T("");
	m_name = _T("");
	m_time = 0;
	m_captialstock = 0.0;
	m_foundermemberstock = 0.0;
	m_nationalstock = 0.0;
	m_innerstock = 0.0;
	m_foreignstock = 0.0;
	m_gathermemberstock = 0.0;
	m_employeestock = 0.0;
	m_corporationmatch = 0.0;
	m_Asection = 0.0;
	m_currencyA = 0.0;
	m_Bsection = 0.0;
	m_Hsection = 0.0;
	m_totalasset = 0.0;
	m_currentassert = 0.0;
	m_moneyassert = 0.0;
	m_fixassert = 0.0;
	m_nofixassert = 0.0;
	m_stockintrade = 0.0;
	m_recievefund = 0.0;
	m_payfund = 0.0;
	m_longtimeinverse = 0.0;
	m_longtimedebt = 0.0;
	m_currentliabilites = 0.0;
	m_capitalaccumulation = 0.0;
	m_surplusaccumulation = 0.0;
	m_legalaccumulation = 0.0;
	m_nodistributeprofit = 0.0;
	m_totalvalue = 0.0;
	m_mainbusinessgain = 0.0;
	m_totalprofit = 0.0;
	m_netprofit = 0.0;
	m_mainbussinessprofit = 0.0;
	m_stockholderinterests = 0.0;
	m_eachstockincrease = 0.0;
	m_eachstockasset = 0.0;
	m_nFields = 39;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CDBZLLBSH::GetDefaultDBName()
{
	return _T("C:\\Program Files\\Microsoft Visual Studio\\MyProjects\\vwbase\\历史资料列表.mdb");
}

CString CDBZLLBSH::GetDefaultSQL()
{
	return _T("[上市公司资料列表-上海]");
}

void CDBZLLBSH::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDBZLLBSH)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[index]"), m_index);
	DFX_Text(pFX, _T("[symbol]"), m_symbol);
	DFX_Text(pFX, _T("[name]"), m_name);
	DFX_Long(pFX, _T("[time]"), m_time);
	DFX_Double(pFX, _T("[captialstock]"), m_captialstock);
	DFX_Double(pFX, _T("[foundermemberstock]"), m_foundermemberstock);
	DFX_Double(pFX, _T("[nationalstock]"), m_nationalstock);
	DFX_Double(pFX, _T("[innerstock]"), m_innerstock);
	DFX_Double(pFX, _T("[foreignstock]"), m_foreignstock);
	DFX_Double(pFX, _T("[gathermemberstock]"), m_gathermemberstock);
	DFX_Double(pFX, _T("[employeestock]"), m_employeestock);
	DFX_Double(pFX, _T("[corporationmatch]"), m_corporationmatch);
	DFX_Double(pFX, _T("[Asection]"), m_Asection);
	DFX_Double(pFX, _T("[currencyA]"), m_currencyA);
	DFX_Double(pFX, _T("[Bsection]"), m_Bsection);
	DFX_Double(pFX, _T("[Hsection]"), m_Hsection);
	DFX_Double(pFX, _T("[totalasset]"), m_totalasset);
	DFX_Double(pFX, _T("[currentassert]"), m_currentassert);
	DFX_Double(pFX, _T("[moneyassert]"), m_moneyassert);
	DFX_Double(pFX, _T("[fixassert]"), m_fixassert);
	DFX_Double(pFX, _T("[nofixassert]"), m_nofixassert);
	DFX_Double(pFX, _T("[stockintrade]"), m_stockintrade);
	DFX_Double(pFX, _T("[recievefund]"), m_recievefund);
	DFX_Double(pFX, _T("[payfund]"), m_payfund);
	DFX_Double(pFX, _T("[longtimeinverse]"), m_longtimeinverse);
	DFX_Double(pFX, _T("[longtimedebt]"), m_longtimedebt);
	DFX_Double(pFX, _T("[currentliabilites]"), m_currentliabilites);
	DFX_Double(pFX, _T("[capitalaccumulation]"), m_capitalaccumulation);
	DFX_Double(pFX, _T("[surplusaccumulation]"), m_surplusaccumulation);
	DFX_Double(pFX, _T("[legalaccumulation]"), m_legalaccumulation);
	DFX_Double(pFX, _T("[nodistributeprofit]"), m_nodistributeprofit);
	DFX_Double(pFX, _T("[totalvalue]"), m_totalvalue);
	DFX_Double(pFX, _T("[mainbusinessgain]"), m_mainbusinessgain);
	DFX_Double(pFX, _T("[totalprofit]"), m_totalprofit);
	DFX_Double(pFX, _T("[netprofit]"), m_netprofit);
	DFX_Double(pFX, _T("[mainbussinessprofit]"), m_mainbussinessprofit);
	DFX_Double(pFX, _T("[stockholderinterests]"), m_stockholderinterests);
	DFX_Double(pFX, _T("[eachstockincrease]"), m_eachstockincrease);
	DFX_Double(pFX, _T("[eachstockasset]"), m_eachstockasset);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// by 俞明录

#ifdef _DEBUG
void CDBZLLBSH::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CDBZLLBSH::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif
