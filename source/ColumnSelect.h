#if !defined(AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_)
#define AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const CString g_sBuySell[] = {"买一价","买二价","买三价","买一量","买二量","买三量","卖一价","卖二价","卖三价","卖一量","卖二量","卖三量"
};
enum{

ID_ROWNUM = 5000   
,ID_STOCKID  
,ID_STOCKNAME
,ID_YSTC     
,ID_OPEN     
,ID_HIGH     
,ID_LOW      
,ID_NEW      
,ID_TOTV     
,ID_NOWV     
,ID_TOTP     
,ID_AVGP     
,ID_ADDE     
,ID_HLP      
,ID_RDR      
,ID_BSSPBAS  
,ID_DIFBS    
,ID_VOLBI    
,ID_ACCB     
,ID_ACCS     
,ID_BUY      
,ID_SEL      
,ID_RVOL     
,ID_DVOL     
,ID_EXCHANGE 
,ID_SYL      
,ID_BUYP1     
,ID_BUYP2     
,ID_BUYP3     
 
,ID_BUYV1     
,ID_BUYV2     
,ID_BUYV3     
 
,ID_SELP1     
,ID_SELP2     
,ID_SELP3     
  
,ID_SELV1     
,ID_SELV2     
,ID_SELV3     
 

,ID_ZGB       
,ID_GJG       
,ID_FQRFRG    
,ID_FRG       
,ID_ZGG       
,ID_GZAG      
,ID_LTAG      
,ID_HG        
,ID_BG        
,ID_ZPG       

,ID_ZZC       
,ID_LDZC      
,ID_CQTZ      
,ID_GDZC      
,ID_WXZC      
,ID_LDFZ      
,ID_CQFZ      
,ID_GGQY      
,ID_ZBGJJ     
,ID_YYGJJ     
,ID_MGJZ      
,ID_GDQYBL    
,ID_MGGJJ     

,ID_ZYYWSR    
,ID_ZYYWLR    
,ID_QTYWLR    
,ID_LRZE      
,ID_JLR       
,ID_WFPLR     
,ID_MGSY      
,ID_JZCSYL    
,ID_MGWFPLR   
,ID_MGJZC     


,ID_ZJLS      
,ID_VOLBIFM   
,ID_HLPFM     
,ID_EXCHANGEFM
,ID_ADVSP     

,ID_DELITEM
,ID_DEFAULT

};

struct DynaTechniqueDef{
   char  	m_nString[50];				
   char     m_nExplain[50];
   UINT     m_nID;                 
};

typedef CTypedPtrList<CPtrList, DynaTechniqueDef*> m_DynaTechniqueDef;


class CTaiShanReportView;
class CColumnSelect : public CDialog
{

public:
	int GetId(int nIn);
	void ShowMenu();
	CColumnSelect(CWnd* pParent = NULL);  
	CColumnSelect(UINT nID,CString nStr,BOOL nDel,BOOL IsJS=FALSE,int x=0,int y=0,CWnd* pParent = NULL);   // standard constructor
	~CColumnSelect();   

	int m_nPos_x;           
	int m_nPos_y;             
	UINT m_nSelectID;         
	CString m_sSelectStr;     
	BOOL  m_bIsTechnique;      
	m_DynaTechniqueDef m_DynaTechnique; 

	BOOL m_bIsDelete;          
	BOOL m_bIsFirst;          
	CTaiShanReportView *pView;

	//{{AFX_DATA(CColumnSelect)
	enum { IDD = IDD_COLUMN_SELECT };
	CTreeCtrl	m_MyTree;
	//}}AFX_DATA




	//{{AFX_VIRTUAL(CColumnSelect)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CColumnSelect)
	virtual BOOL OnInitDialog();
	void OnDblclk(NMHDR* wParam, LRESULT* lParam);
	afx_msg void OnClose();
	virtual void OnOK();



	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMNSELECT_H__C0089EA1_E150_11D3_920B_00400552E583__INCLUDED_)
