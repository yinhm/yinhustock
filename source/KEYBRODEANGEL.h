
#if !defined(_SJLKEYBOARDANGELDLG_H_)
#define _SJLKEYBOARDANGELDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Resource.h"

#include "CTaiKlineListCtrl.h"



class CTaiKeyBoardAngelDlg : public CDialog
{

public:
	static void DeleteIndicatorName(CString name);
	void AddIndicatorName(CString name);
	void SetStartPos(BOOL bFellowingMouse,int px,int py);
	CTaiKeyBoardAngelDlg(CWnd* pParent = NULL);   
  
	char    input;              
	bool    ischar;             
	char*   result;             
	int		m_stkKind;		
	BOOL    isresultstock;        
	BOOL    isresultok;         
       
    CString inputpre;		


	//{{AFX_DATA(CTaiKeyBoardAngelDlg)
	enum { IDD = IDD_ANGELKEYBRODE};
	CTaiKlineListCtrl	m_ListCtrl;
	CEdit	m_InputEditCtrl;
	CString	m_strInputResult;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTaiKeyBoardAngelDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	//}}AFX_VIRTUAL


protected:
	char* SymbolToName(char * pSymbol);
	void InsertItemToList(int nWhichItem,int nStartItem,int nDefItem);
	void CalculateListRange(CString strInput);

	//{{AFX_MSG(CTaiKeyBoardAngelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeInput();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusInput();
	afx_msg void OnKillfocusInput();
	//}}AFX_MSG
	afx_msg LRESULT OnDragBegin(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void OnReturn();
	BOOL m_bFellowingMouse;
	POINT m_nFellowingPt;

	CWnd* m_wndParent;
	int     m_nStrLength;
	int     m_nNumberLength;
	int     m_nListPosition;          
	int     m_nCurrentItemIndex;    
	BOOL    m_bLastFocusFlag;         
	int     m_nPreStarIndex;        
	int     m_nPreEndIndex;             
	   
	CArray<int,int> m_nStartPosArray; 
	CArray<int,int> m_nEndPosArray;   
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_KEYBOARDANGELDLG_H_)
