// LBsInfoRS.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "LBsInfoRS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LBsInfoRS

IMPLEMENT_DYNAMIC(LBsInfoRS, CDaoRecordset)

LBsInfoRS::LBsInfoRS(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(LBsInfoRS)
	m_symbol = _T("");
	m_content = _T("");
	m_time = (DATE)0;
	m_brief = _T("");
	m_nFields = 4;
	//}}AFX_FIELD_INIT
//	m_content = _T("");
	m_nDefaultType = dbOpenDynaset;
}


CString LBsInfoRS::GetDefaultDBName()
{
	return _T("baseInformation.mdb");
}

CString LBsInfoRS::GetDefaultSQL()
{
	
	return _T("[Sheet1]");
}

void LBsInfoRS::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(LBsInfoRS)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Text(pFX, _T("[symbol]"), m_symbol);
	DFX_Text(pFX, _T("[content]"), m_content);
	DFX_DateTime(pFX, _T("[time]"), m_time);
	DFX_Text(pFX, _T("[brief]"), m_brief);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// LBsInfoRS diagnostics

#ifdef _DEBUG
void LBsInfoRS::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void LBsInfoRS::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif //_DEBUG
