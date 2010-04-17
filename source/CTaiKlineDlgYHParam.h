#if !defined(AFX_TECHDLGYHPARAM_H__EA950764_C4E8_49A6_8E1C_0338E6CEB1E5__INCLUDED_)
#define AFX_TECHDLGYHPARAM_H__EA950764_C4E8_49A6_8E1C_0338E6CEB1E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgYHParam.h : header file
//
#include "CTaiScreenParent.h"
#include "ParamEdit.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgYHParam dialog
class CParamEdit;
struct ParamSetting
{
	union
	{
		float fSetting[64];
		struct
		{
			BOOL	m_bAuto;
			UINT	m_nRepeate;
			UINT	m_nRange;
			int		m_nKlineLastUsed;
		};
	};
	ParamSetting()
	{
		memset(fSetting,0,sizeof(fSetting));
		m_nKlineLastUsed = 1200;
	};
};

class CTaiKlineDlgYHParam : public CDialog
{
// Construction
public:
	bool m_bCaclSucc;
	bool m_bCalcAuto;
	static BOOL CalcYH(IndexDataInfo *pIndexSave, SymbolKindArr *stockArray);
	void SetResultText();
	static int GetIndexLineType(CFormularContent *pIndex,int &nLine);
	bool m_bContinueCacl;
	bool bCancel;
	void SaveSetting1();
	bool OnCalc();
	bool Init(IndexDataInfo* pIndexSave, Kline* pKline, int nKline,CFormularContent* pIndex,int kLineType,CBuySellList *pList ,SymbolKindArr* stockArray);
	static ParamSetting m_gParamSetting;
	static void SaveSetting();
	static void ReadSetting();
	CTaiKlineDlgYHParam(CWnd* pParent = NULL);  
	CParamEdit *pEdit;
	CFont m_Font;


	IndexDataInfo m_IndexSave;
	Kline*		m_pKline;
	int			m_nKline;
	CFormularContent*		m_pIndex;
	int m_nKlineKind2;
	CBuySellList *m_pList ;
	SymbolKindArr		m_stockArray;

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgYHParam)
	enum { IDD = IDD_DIALOG1 };
	CButtonST	m_conOk;
	CButtonST	m_conCanc;
	CButtonST	m_conButt1;
	CFloatEdit	m_con7;
	CFloatEdit	m_con1;
	CProgressCtrl	m_prg;
	CListCtrl	m_param_list;
	BOOL	m_bAuto;
	UINT	m_nRepeate;
	CString	m_sResult;
	UINT	m_nRange;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgYHParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgYHParam)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonAdditional();
	afx_msg void OnDblclkListParam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListParam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGYHPARAM_H__EA950764_C4E8_49A6_8E1C_0338E6CEB1E5__INCLUDED_)
