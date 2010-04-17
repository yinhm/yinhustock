#if !defined(AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_)
#define AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_DRAWLIST WM_USER+131

class CTaiShanDoc;

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


typedef struct
{
	    float 	Price;  
		float 	Volume;    
		float 	Amount;  
        float   accb;     
        float   accs;  
}KdataOld;


struct  CdatOld
{
 BYTE kind;
 BYTE  rdp;        
 char id[8];         
 char name[10];        
 char Gppyjc[6];         
 int   sel;
 float ystc;        
 float opnp;             
 float higp;           
 float lowp;             
 float nowp;              
 float nowv;           
 float totv;              
 float totp;         
 float pbuy1;            
 float vbuy1;           
 float pbuy2;            
 float vbuy2;      
 float pbuy3;          
 float vbuy3;             
 float psel1;            
 float vsel1;           
 float psel2;        
 float vsel2;            
 float psel3;           
 float vsel3;            
 float accb;             
 float accs;          
 float volume5;    
 float rvol;              
 float dvol;            
 
 short lastclmin;      
 short initdown;            
 BOOL  InOut;
 
 float StartP;       
 float EndP;          
 float Res[11];
 KdataOld   m_Kdata[240];
 int      nReservation;
};



class CTaiShanDoc;
class CImportData : public CPropertyPage
{
	DECLARE_DYNCREATE(CImportData)


public:
    CTaiShanDoc * pDoc;
	CArray<BOOL,BOOL> m_stockrange;
	CString DestinationFile;
	CString SourceFileName;
	CArray <Kline,Kline &> m_InstallArray;

	CImportData();
	~CImportData();


	//{{AFX_DATA(CImportData)
	enum { IDD = IDD_DATAIMPORT };
	CButtonST	m_button1;
	CDateTimeCtrl	m_ctrlTimeEnd;
	CDateTimeCtrl	m_ctrlTimeStart;
	CListCtrl	m_listfw;
	CButtonST	m_okbutton;
	CProgressCtrl	m_progress1;
	CComboBox	m_ComboDatatype;
	CComboBox	m_ComboFiletype;
	CEdit	m_DataSource;
	int m_importAll;
	CTime	m_install_end;
	CTime	m_install_start;
	//}}AFX_DATA
public:
    BOOL IsFileExists(CString filename);
    void SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline =true);
    BOOL InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline=true);
	void Produce2FloppyDisk(CString DestFileName,CString FloppyFileName);
 
	CFile m_OutMinuteFile;
	CFile m_LocalFile;
	CFile m_DestFile;
	CFile m_file;

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
	

	//{{AFX_VIRTUAL(CImportData)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL


protected:

	//{{AFX_MSG(CImportData)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedatatype();
	afx_msg void OnSelchangedataformat();
	afx_msg void OnSetfocusfwxz();
	afx_msg void OnSetfocus();
	afx_msg void Onqbyr();
	afx_msg void Onsdyr();
	afx_msg void OnSetfocusup();
	afx_msg void OnSetfocusdown();
	afx_msg void Onimport();
	afx_msg void OnButtonSearchfixdata();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


public:
	int IsFileType(CString Filename);
	BOOL m_bImport;
	static void InstallOneMinStock(CString FilePath);
	CString m_news;
	BOOL m_Shenzhen;
	BOOL m_Shanghai;
	int m_nImportorcancel;
	int m_nFlag;
	int YieldMinuteFile(bool bShangHai = true,bool bAssignDays = true);
	BOOL ShenglongToTianlong(SHENGLONG shenglong, Kline& wanshen );

	void InstallShenglong(CString srcfilename,int BeginDate, int EndDate);
	int InstallFromSeveralDiskfile(CString FileName,bool bDayKline = true);
    void InstallMinteData();
    void InstallMin(CString lpStockSymbol);
	void ProduceMinuteData() ;

private:
	void InstallStockKlineEach(CString symbol,int wMarketType,int BeginDate,int EndDate,bool bDayKline = true);


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTDATA_H__B5C1E14C_A053_4797_845A_C1BA8330EEA6__INCLUDED_)
