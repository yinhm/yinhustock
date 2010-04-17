#if !defined(AFX_DATA_INFO_SZ_H__FE46AA1B_4F82_11D3_BD9D_0000E8593F1A__INCLUDED_)
#define AFX_DATA_INFO_SZ_H__FE46AA1B_4F82_11D3_BD9D_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDATA_INFO_SZ : public CDaoRecordset
{
public:
	CDATA_INFO_SZ(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDATA_INFO_SZ)

	//{{AFX_FIELD(CDATA_INFO_SZ, CDaoRecordset)
	long	m_ID;
	CString	m_column1;
	CString	m_column2;
	CString	m_column3;
	double	m_column4;
	double	m_column5;
	double	m_column6;
	double	m_column7;
	double	m_B_;
	double	m_H_;
	double	m___A_;
	double	m_column8;
	double	m_A2___;
	double	m_column9;
	double	m_column10;
	double	m_column11;
	double	m_column12;
	double	m_column13;
	double	m_column14;
	double	m_column15;
	double	m_column16;
	double	m_column17;
	double	m_column18;
	double	m_column19;
	double	m_column20;
	double	m_column21;
	double	m_column22;
	double	m_column23;
	double	m_column24;
	double	m_column25;
	double	m_column26;
	double	m_column27;
	double	m_column28;
	double	m_column29;
	double	m_column30;
	double	m_column31;
	double	m_column32;
	double	m_column33;
	double	m_column34;
	double	m_column35;
	double	m_column36;
	//}}AFX_FIELD

	//{{AFX_VIRTUAL(CDATA_INFO_SZ)
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

#endif // !defined(AFX_DATA_INFO_SZ_H__FE46AA1B_4F82_11D3_BD9D_0000E8593F1A__INCLUDED_)
