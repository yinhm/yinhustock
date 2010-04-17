#include "afxtempl.h"
#include "StructTaiShares.h"

#if !defined(AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
#define AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GSGLLSSJ.h : header file


class CTaiShanDoc;

/////////////////////////////////////////////////////////////

struct QIANLONG
{
	DWORD dwDate;        
	DWORD dwOpen;      
	DWORD dwHigh;         
	DWORD dwLow;         
	DWORD dwClose;        
	DWORD dwAmount;    
	DWORD dwVolume;       
	DWORD dwUnknow;      
    WORD  wUp;          
    WORD  wDown;          
    DWORD dwGuben;        
};

/////////////////////////////////////////////////////////////

struct FENXIJIA
{
	DWORD dwDate;   
	float fOpen;     
	float fHigh;    
	float fLow ;   
	float fClose;    
	float fAmount;  
	float fVolume;  
	float fUnknown;  

};

/////////////////////////////////////////////////////////////

struct TAI_SHAN_DATA_cw
{
		int	    nTime;     
		float	fOpen;	
		float	fHigh;		
		float	fLow ;	
		float 	fClose;
		float 	fVolume;  
		float	fAmount;   
		WORD    wAdvance;   
		WORD    wDecline;  
};

/////////////////////////////////////////////////////////////

struct  HUIJIN
{
	int   nDate;        
	float fOpen;        
	float fHigh;        
	float fLow;          
	float fClose;          
	float fMean;       
	int   nVolume;      
	BYTE  chUnknown;    
};


struct SHENGLONG
{
	long  date;
	long  oprice;	
	long  cprice;
	long  hprice;	
	long  lprice;
	long  tovalue;	
	long  tohands;
	long  topiece;
	char  free[8];
};

////////////////////////////////////////////////

struct KLINEHEAD_cw
{
	int FileID;               
	int	NumPerBlk	;			
	int Free1;                 
	int	NumStock	;			
	int	NumBlockAll;		
	int Free2;					
};



struct KLINESMALLHEAD_cw
{
	char    StockSign[8]; 
	short 	numChuQ	;	  
	long    numKln	;	 
	short   symBlock[25]; 
};

struct POWER_cw   
{
	BYTE  byFree[12]; 
	int   nTime;
	float fGive;
	float fAllocate;
	float fAllocatePrice;
	float fDividend;
};

struct MINUTEKLINEHEAD_cw
{
	char    StockSign[8]; 
	long    numKln	;	  
	long    pPos;         
	long    symBlock;     
};

struct MKLINEHEAD_cw
{
	int IDFile;
	int	numPerBlk	;		
	int	numKline_sh	;		

	int unKnown;
};

/////////////////////////////////////////////////////////////////////////////
// CGSGLLSSJ dialog
class CTaiShanDoc;
class CGSGLLSSJ : public CPropertyPage
{
	DECLARE_DYNCREATE(CGSGLLSSJ)


public:
	CTaiShanDoc* pDoc;

	CString DestinationFile;
	CString SourceFileName;
	CArray <Kline,Kline &> m_InstallArray;
	
	CGSGLLSSJ();
	~CGSGLLSSJ();

// Dialog Data
	//{{AFX_DATA(CGSGLLSSJ)
	enum { IDD = IDD_LSSJGL };
	CProgressCtrl	m_ProgressYield;
	CButtonST	m_check_fixtime;
	CEdit	m_destination;
	CButtonST	m_SearchFixData;
	CEdit	m_DataSource;
	CComboBox	m_ComboDatatype;
	CProgressCtrl	m_progress1;
	BOOL	m_sh;
	BOOL	m_sz;
	int		m_Cover;
	int		m_AssignPeriod;
	int		m_NumberOfDays;
	CTime	m_install_start;
	CTime	m_install_end;
	CTime	m_produce_end;
	CTime	m_produce_start;
	//}}AFX_DATA

public:
	BOOL IsFileExists( CString filename);
	void SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline = true);
	BOOL InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline = true);
	void Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);

	CFile m_OutMinuteFile;
	CFile m_LocalFile;
	CFile m_DestFile;
	CFile m_file;
	KLINEHEAD_cw m_BigFileHead;
	char* file_sz;
	char* file_sh;
	void YieldDay(CString filename, int &NumberOfStock);
	void AssignDays(CString filename, int &NumberOfStock);  

	LONG GetStockDay(time_t NumSeconds );
	int myf(CString szStr);

	BOOL GetStockTime(int ymd, LONG& tm);
	void InstallHuijin  (CString srcfilename, int BeginDate, int EndDate); 
	void InstallQianlong(CString srcfilename, int BeginDate, int EndDate); 
	void InstallWanshen (CString srcfilename, int BeginDate, int EndDate); 
	void InstallFenxijia(CString srcfilename, int BeginDate, int EndDate); 

	//{{AFX_VIRTUAL(CGSGLLSSJ)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL


protected:

	//{{AFX_MSG(CGSGLLSSJ)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearchfixdata();
	afx_msg void OnButtonBeginInstall();
	afx_msg void OnRadioRepaire();
	afx_msg void OnButtonDestination();
	afx_msg void OnButtonBeginproduce();
	afx_msg void OnRadioDay();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboDatatype();
	afx_msg void OnRadioInstalldate();
	afx_msg void OnRadioCover();
	afx_msg void OnRadioAssignperiod();
	afx_msg void OnRadioRecentdays();
	afx_msg void OnSelchangeIntype();
	afx_msg void OnCloseupOuttype();
	afx_msg void OnChangeEditAssigndays();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

///////////////////////

public:
	int YieldMinuteFile(bool bShangHai = true,bool bAssignDays = true);
	BOOL ShenglongToTianlong(SHENGLONG shenglong, Kline& wanshen );

	void InstallShenglong(CString srcfilename,int BeginDate, int EndDate);
	int InstallFromSeveralDiskfile(CString FileName,bool bDayKline = true);
    void InstallMinteData();
    void ProduceMinuteData() ;
private:
	void InstallStockKlineEach(CString symbol,int BeginDate,int EndDate,bool bDayKline = true);
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GSGLLSSJ_H__804D1366_B12B_11D1_83E6_0000E8593F1A__INCLUDED_)
