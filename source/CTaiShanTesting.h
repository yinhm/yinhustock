
#if !defined(AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_)
#define AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiShanTesting  
{
private:
	CTaiShanDoc* m_pDoc;
	BOOL m_bRefresh;	
	BOOL m_bHaveFormula;
public:
	CString m_strStockID;	
	CArray<IndexDataInfo,IndexDataInfo> m_strHjyChooseFormula;
	CArray<IndexDataInfo,IndexDataInfo> m_strChooseFormu;	
	
	CString m_strResultData2;	
	CString m_strResultData;	
public:
	CTaiShanTesting();	
	CTaiShanTesting(CTaiShanDoc* pDoc);	
	void SetStock(CString strStockID=_T(""));
	void GetChooseFormula(CTaiShanKlineShowView* pView,BOOL bRefresh=FALSE);
	void SelectStock(CTaiShanKlineShowView* pView,BOOL bRefresh=FALSE);	
	void OnDraw(CTaiTestRichEditCtrl* pRich, COLORREF crBack, COLORREF crText, COLORREF crTitle,CRect &rc);	//»­³ö½á¹û
	virtual ~CTaiShanTesting();	
};

#endif // !defined(AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_)
