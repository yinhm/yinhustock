// DATA_INFO_SH.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "INFORMATION_SH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDATA_INFO_SH

IMPLEMENT_DYNAMIC(CDATA_INFO_SH, CDaoRecordset)

CDATA_INFO_SH::CDATA_INFO_SH(CDaoDatabase* pdb)
	: CDaoRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDATA_INFO_SH)
	m_ID = 0;
	m_column1 = _T("");
	m_column2 = _T("");
	m_column3 = _T("");
	m_column4 = 0.0;
	m_column5 = 0.0;
	m_column6 = 0.0;
	m_column7 = 0.0;
	m_B_ = 0.0;
	m_H_ = 0.0;
	m___A_ = 0.0;
	m_column8 = 0.0;
	m_A2___ = 0.0;
	m_column9 = 0.0;
	m_column10 = 0.0;
	m_column11 = 0.0;
	m_column12 = 0.0;
	m_column13 = 0.0;
	m_column14 = 0.0;
	m_column15 = 0.0;
	m_column16 = 0.0;
	m_column17 = 0.0;
	m_column18 = 0.0;
	m_column19 = 0.0;
	m_column20 = 0.0;
	m_column21 = 0.0;
	m_column22 = 0.0;
	m_column23 = 0.0;
	m_column24 = 0.0;
	m_column25 = 0.0;
	m_column26 = 0.0;
	m_column27 = 0.0;
	m_column28 = 0.0;
	m_column29 = 0.0;
	m_column30 = 0.0;
	m_column31 = 0.0;
	m_column32 = 0.0;
	m_column33 = 0.0;
	m_column34 = 0.0;
	m_column35 = 0.0;
	m_column36 = 0.0;
	m_nFields = 41;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dbOpenDynaset;
}


CString CDATA_INFO_SH::GetDefaultDBName()
{
	return _T("基本资料.mdb");
}

CString CDATA_INFO_SH::GetDefaultSQL()
{
	return _T("[上海基本资料]");
}

void CDATA_INFO_SH::DoFieldExchange(CDaoFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDATA_INFO_SH)
	pFX->SetFieldType(CDaoFieldExchange::outputColumn);
	DFX_Long(pFX, _T("[ID]"), m_ID);
	DFX_Text(pFX, _T("[代码]"), m_column1);
	DFX_Text(pFX, _T("[名称]"), m_column2);
	DFX_Text(pFX, _T("[更新日期]"), m_column3);
	DFX_Double(pFX, _T("[总股本(万股)]"), m_column4);
	DFX_Double(pFX, _T("[国家股]"), m_column5);
	DFX_Double(pFX, _T("[发起人法人股]"), m_column6);
	DFX_Double(pFX, _T("[法人股]"), m_column7);
	DFX_Double(pFX, _T("[B股]"), m_B_);
	DFX_Double(pFX, _T("[H股]"), m_H_);
	DFX_Double(pFX, _T("[流通A股]"), m___A_);
	DFX_Double(pFX, _T("[职工股]"), m_column8);
	DFX_Double(pFX, _T("[A2转配股]"), m_A2___);
	DFX_Double(pFX, _T("[总资产(千元)]"), m_column9);
	DFX_Double(pFX, _T("[流动资产]"), m_column10);
	DFX_Double(pFX, _T("[固定资产]"), m_column11);
	DFX_Double(pFX, _T("[无形资产]"), m_column12);
	DFX_Double(pFX, _T("[长期投资]"), m_column13);
	DFX_Double(pFX, _T("[流动负债]"), m_column14);
	DFX_Double(pFX, _T("[长期负债]"), m_column15);
	DFX_Double(pFX, _T("[资本公积金]"), m_column16);
	DFX_Double(pFX, _T("[每股公积金]"), m_column17);
	DFX_Double(pFX, _T("[股东权益]"), m_column18);
	DFX_Double(pFX, _T("[主营收入]"), m_column19);
	DFX_Double(pFX, _T("[主营利润]"), m_column20);
	DFX_Double(pFX, _T("[其他利润]"), m_column21);
	DFX_Double(pFX, _T("[营业利润]"), m_column22);
	DFX_Double(pFX, _T("[投资收益]"), m_column23);
	DFX_Double(pFX, _T("[补贴收入]"), m_column24);
	DFX_Double(pFX, _T("[营业外收支]"), m_column25);
	DFX_Double(pFX, _T("[上年损益调整]"), m_column26);
	DFX_Double(pFX, _T("[利润总额]"), m_column27);
	DFX_Double(pFX, _T("[税后利润]"), m_column28);
	DFX_Double(pFX, _T("[净利润]"), m_column29);
	DFX_Double(pFX, _T("[未分配利润]"), m_column30);
	DFX_Double(pFX, _T("[每股未分配]"), m_column31);
	DFX_Double(pFX, _T("[每股收益]"), m_column32);
	DFX_Double(pFX, _T("[每股净资产]"), m_column33);
	DFX_Double(pFX, _T("[调整每股净资]"), m_column34);
	DFX_Double(pFX, _T("[股东权益比]"), m_column35);
	DFX_Double(pFX, _T("[净资收益率]"), m_column36);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
void CDATA_INFO_SH::AssertValid() const
{
	CDaoRecordset::AssertValid();
}

void CDATA_INFO_SH::Dump(CDumpContext& dc) const
{
	CDaoRecordset::Dump(dc);
}
#endif 
