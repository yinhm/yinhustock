// ldao.cpp : implementation file
//

#include "stdafx.h"
#include "ldao.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNAMIC(ldao, CDaoRecordset)

ldao::ldao(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(ldao)
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString ldao::GetDefaultDBName()
{
	return _T("");
}

CString ldao::GetDefaultSQL()
{
	return _T("");
}

void ldao::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(ldao)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// ldao diagnostics

#ifdef _DEBUG
void ldao::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void ldao::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif 
