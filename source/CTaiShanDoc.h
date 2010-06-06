
#if !defined(AFX_WSSTOCK2000DOC_H__61A31CF1_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_WSSTOCK2000DOC_H__61A31CF1_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFormularContent.h"

#include "StructKlineView.h"
#include <afxmt.h>
#include "CFormularCompute.h"
#include <afxdao.h>
#include "NineShowView.h"
#include "MyTreeView.h"
#include "InfoView.h"

#define  systemcolorlength   34
#define  comblength          9
#define DLINEHEAD  24
#define DLINESMALLHEAD 4096 * 64
#define DLINECHUQ   32 * 6
#ifdef WIDE_NET_VERSION
#define MAXCOLUME  25
#define INITCOLUME 20
#define INITDATAID 8888
#else
#define MAXCOLUME  30
#define INITCOLUME 25
#define INITDATAID 9999
#endif


struct DATA_KEYBOARD {
	char    key[12];              
	char    data[24];           
	BYTE    id;                
	BYTE	m_stkKind;
};


struct  SYSTEM_OPTION {
	int 		kline;		 
	BOOL 		fourgraph;	
	BOOL		autoclose;	
	BOOL		autominute;	
	BOOL		autoday;  
	BOOL        showxline;
	BOOL        showyline;  
	BOOL        organizedata;  
	BOOL        alarmsound;    
};


struct SYSTEMDATA {
	int        changepage;     
	int        AdvSpTimes ;    
	int        fiveminutesave;
	int        noticesave;    
	int        middleprice;   
	float      buyrate;      
	int        AlarmPercent;  
};

struct SETCURRENTDIRECTORY{
	char CurrentDir[255];
	char Free[255];
};
struct SYSTEMHQSET{
	int       capital;     
	int       quangtity;   
	int       swing;       
	int       upspeed;     
	int       changehand;   
};

struct ALERTCHOICE{
	float Vol;
	float Per;
	float Acount;
	int Format;
};


typedef struct{
	int        daysOfDayKline; 
	int        daysOfMin5Kline ;   
	int        daysOfManyKline; 
	int        countOfHistoryFile;
	int		nSecondsAlarm;
	int		nTestUnfinished;

	BOOL       bDoPowerSelectingStock;    
	BOOL       bDoVolPowering;   
	BOOL		bDoPowerAfter;	 
	BOOL       bAutoPower;  
	BOOL		bSaveParam;		
	BOOL		bHideLine;	
	BOOL		bShowCrossDlg;	
	BOOL		bRunAlarmBegin;	 
	BOOL		bUseMin5ChenBen;	
	int        bTjxgZsFs;         
	float		LTP_rate;	
	float		CJL_rate;	
	float      fReserved[1];    

}PROPERTY_INIT;


typedef struct{
	float	    yFloatBottom;		
	char		nameIndex[10]	;
}Index_INIT;


typedef struct{
	int				nCountIndex;		
	Index_INIT		initIndex[4];		

	int				nCountMin1;			
	Index_INIT		initMin1[4];	
	int		flag[4]	;	


	int		flag_dapan[4]	;
}INFOINIT;


typedef struct StockSymbolStr {
	char   sharesymbol[7];
	int    nKind;
}SharesSymbol;


struct  BLOCKSTOCK{
	CString   blockname;
	CList<SharesSymbol*,SharesSymbol*> stocklist;
	BLOCKSTOCK(CString string);
	void AddStockToTail(SharesSymbol* sharesymbol);
	void AddStockToHead(SharesSymbol* sharesymbol);
public:
	SharesSymbol* GetAt(POSITION position);
};

struct	SCREEN_DETAIL           
{
	time_t buytime;
	time_t selltime;
	float  buyprice;
	float  sellprice;
	int    timerange;
	float  profit;
	int   issuccess;
	int	nKindSell;
};

struct  SCREEN_RESULT
{
	CString symbol;
	CString name;
	int		m_stkKind;
	int     choosecount;
	int     successcount;
	float   successpercent;
	BOOL    isuseful;
	int		nKindBuySell;


	CArray <SCREEN_DETAIL,SCREEN_DETAIL> m_detail;


public:
	void CalculateData();
};

enum CUOHETYPE 
{
	None,StockId_ASh,StockId_BSh,StockId_ASz,StockId_BSz,ChooseId,StockTypeId
};


typedef CTypedPtrMap< CMapStringToString , CString , CString > CMapNewsFileName ;
typedef CTypedPtrList<CPtrList,FENJIA*> CFJList; 
class CTaiShanReportView;
class CTaiShanTesting;
class CTaiShanKlineShowView;

typedef CTypedPtrMap<CMapStringToPtr,CString,CPoint*> CMapStringToCP;


#define  fontstructlength    4

const int      listlength=17;
class CProgressDialog;
typedef CArray<CTaiShanKlineShowView*,CTaiShanKlineShowView*> VIEWARRAY;
class CTaiKlineFileKLine;
class CTaiKlineFileHS;
class CSharesBigBillDlg;
class CSharesBigBillData;
class CDownLoadDlg;
class CDownLoadNetDlg;
class CTaiShanDoc : public CDocument
{
protected: 
	CTaiShanDoc();
	DECLARE_DYNCREATE(CTaiShanDoc)

	CString m_CurrentWorkDirectory;


public:

	VIEWARRAY m_lmbViewArray;
	CString	m_sShF10;
	CString	m_sSzF10;
	CString	m_sNews;
	CTaiTestTreeView *m_taiViewF9;
	CTaiTestTreeView *m_taiViewF10;

	static BOOL m_bAppAuthorized;
	CString m_strPName;			
	CString m_strUser;		
	CString m_strCompany;		
	CString m_strSerial;		
	CString m_strCredit;
	CTaiShanTesting* m_pPingCe;	


	INFOINIT	m_infoInit;	
	CTypedPtrArray<CPtrArray,NAMESYMBOL*> m_namesymbol;
	CArray<CString,CString&>		index_keyb_f;
	CArray<CString,CString&>		index_keyb_a;	

	CArray<SCREEN_RESULT*,SCREEN_RESULT*> m_chooseresult;


public:


public:
	void* LookUpArray(Formu_Array1& js,CString& str);
	CString m_sharesSymbol;
	int m_stkKind;
	bool m_GetFocus;      

	CImageList  *m_imagelist;          
	CBitmap       m_bitmap;            

	bool m_bAddKline;

public:

	virtual ~CTaiShanDoc();
public:



	void RefreshKeyBoardAngel(BOOL IsAddStock);
	void AddStockToKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy);
	BOOL DeleteStockFromKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy);
	BOOL ModifyStockOfKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy);
	int GetStockKind(CString strKind);
	CString GetStockKindString(int nKind);

	long m_lDay;                                               
	BOOL m_bInitDone;                                       
	BOOL m_bInitCurrDone;                                   
	int m_BlockCalcTime;

	CMapStringToString m_StockTypeMap[3];

	short m_nANT[3];                                           
	short m_nOldANT[3];                                         
	short m_nMaxANT[3];                                    
	short m_nMaxMaxANT;                                        
	short m_nDel_Half_ANT[3];                                  
	short m_nDel_Start_A_hr[3];                                
	short m_nDel_Start_A_min[3];                                   
	short m_nDel_Start_B_hr[3];                               
	short m_nDel_Start_B_min[3];                               
	short m_nDel_End_A_hr[3];                                  
	short m_nDel_End_A_min[3];                                
	short m_nDel_End_B_hr[3];                                  
	short m_nDel_End_B_min[3];                                   
	short m_nHave_Olddata;                                       
	int m_nNowHour;                                          
	int m_nNowMin;                                           
	int m_nNowSec;                                               
	long m_lStartMinA[3];                                         
	long m_lEndMinA[3];                                          
	long m_lStartMinB[3];                                          
	long m_lEndMinB[3];                                           
	short m_nRN[3],m_nRN1[3],m_nDN[3],m_nRD[3],m_nDN1[3],m_nCalStk[3];            

	int m_nSharesSh;
	int m_nSharesSz;
	int m_nSharesSzEb;
	int m_CalcBlockCounts;


	CArray<SharesSymbol,SharesSymbol&> m_ScreenList;
	struct tm m_week;
	int m_nIsClose;
	int m_nColumnIndex[4][MAXCOLUME];



	CSharesInformation m_sharesInformation;
	CSharesCompute m_sharesCompute;
	TechniqueColumnInfo *m_pTechniqueColumn;
#ifdef WIDE_NET_VERSION
	CDownLoadNetDlg *m_pDownLoadDlg;
#else
	CDownLoadDlg *m_pDownLoadDlg;
#endif	
	BOOL m_bCloseReceiver;
	BOOL m_bOpenStockTypeDlg;

	int m_nShowMaxCount;
	CMapNewsFileName m_NewsFileNameMap;                      



	CList<SharesSymbol*,SharesSymbol*> m_choosesymbol;
	CList<BLOCKSTOCK*,BLOCKSTOCK*> m_blocklist;   
	CList<BLOCKSTOCK*,BLOCKSTOCK*> m_Tjxgblocklist;   
	CString          m_cybkname[10];

	Rsdn1 *Nidx[2];                                           
	Tidxd *Tidx[2];                                         
	Aidx1 *Adat[2];   
	int InitSetp;



	CTaiShanReportView *m_WsStock2000View;
	CSharesBigBillDlg  *m_pStockCjMxTj;
	CSharesBigBillData *m_pStockBigBillData;
	CNineShowView *m_NineShowView;
	CTaiTestTreeView   *m_MyTreeView;
	CInfoView     *m_InfoView;
	CTaiShanKlineShowView   *m_VwbaseView;



	void Init_dat();                                          
	void Chk_Ystc();                                          
	void chk_date();                                        
	void Init_EveryDay();                                     
	void Init_StockData(int mode);                            
	int GetStocktime(int mode);                              
	void LoadStockData(int mode);                              
	void CreateFileData(int mode);                              
	void LoadFileData(int mode);                             
	float OnCalcDpTidxdData(int which_stk);                     
	long GetStockDay(time_t time )    ;                     
	void StockCloseWork();
	void SaveFileData();
	void StockNameConvert(char *StockName,char *pyjc);
	void InitTjxg();           
	BOOL IdleProc( LONG lCount );
	void InitStockFiveDaysVolumn();
	void InitFiveDaysVolumnForStock(PCdat1 &pCdat);
	void WriteStockInfoToFile();

#ifdef WIDE_NET_VERSION
	void WideNetTransactSystemRunPara(RCV_SYSTEMRUN_STRUCTEx *pSystemRunPara);
	void WideNetTransactIndexAttribute(RCV_INDEXATTRIBUTE_STRUCTEx *pIndexAttribute, int nParaCounts, int nIndexType);
	void WideNetInitMarketBargainingData(RCV_STKLABEL_STRUCTEx *pStkLabel,int nStkLabelCounts);
#endif


	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_hotkey;
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_hotchar;
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_keychardata; 
	CArray<DATA_KEYBOARD*,DATA_KEYBOARD*> m_keynumberdata;

protected:

	void FreeKeyBoardAngel();

	void InitializeKeyBoardAngle(BOOL IsAddStock);

public:
	// 全局设置
	COLORREF			m_colorArrayDefault[12][34];
	COLORREF		m_colorArray[34];
	LOGFONT			m_fontdefault;
	LOGFONT			m_fontstr[fontstructlength];
	PROPERTY_INIT	m_propertyInitiate;

	SYSTEM_OPTION	m_systemOption;
	SYSTEMDATA		m_systemdata;
	SYSTEMHQSET		m_syshqset;
	ALERTCHOICE		m_dadanchoice;

	SUB_FIGUER_INIT_INFO	m_propertyInitiate2;
	SYSTEMINITDATA			m_SystemInitData;
	SETCURRENTDIRECTORY		m_Directory;

	int				m_nColumnWidth[MAXCOLUME];
	// 设置结束

	Formu_Array1	m_formuar_index;	
	Formu_Array1	m_formuar_choose;	
	Formu_Array1	m_formuar_master;	
	Formu_Array1	m_formuar_kline;	

	CStkBlockManager m_ManagerStockTypeData;

public:
	int		m_nPreDisplayRow;			// 列表显示行数
	int		m_nCurrentPage;				// 当前显示页

	STOCKDATASHOW*	m_pStockDataShow;	// 列表显示数据

public:
	BOOL	m_bCloseWorkDone;			// 是否已经收盘

public:
	//{{AFX_VIRTUAL(CTaiShanDoc)
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

protected:
	void CheckKind();
	void InitChooseAndStockType();

public:
	void ClearRealData();

public:
	void OnCalcHqDataProgress();

protected:
	//{{AFX_MSG(CTaiShanDoc)
	afx_msg void OnOpenDocument();
	afx_msg void OnToolClosework();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_WSSTOCK2000DOC_H__61A31CF1_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
