#ifndef __WSSTOCKSTRUCT_1000_H__
#define __WSSTOCKSTRUCT_1000_H__
// by 俞明录
#define CALLOC(n,x)    ((x *) farcalloc((long)(n),sizeof(x)))
#define FREE(x)        farfree(x)

#define MaxRights 3
#define MaxStockTYpe 102
#define MAXJISHUCOUNT 10


typedef struct
{
		float 	Price;     
		float 	Volume;   
		float 	Amount;   
    
}Kdata1;
typedef struct
{
		float 	Price;     
		float 	Volume;    
		float 	Amount;    
        float   accb;      
        float   accs;     
	    float   bsspbas;  
		float   dif_accb;  
		float   dif_accs;   
}DpKdata1;
typedef struct 
{
	int   nFlags;
	int	  nTime;
	float Give;
	float Allocate;
	float AllocatePrice;
	float Bonus;
	BYTE Free[4];
}Split ,*PSplit;


typedef struct
{
	char Symbol[10];  //股票代码
	int  NumSplit;   //已经除权次数
	float zgb;     //总股本(万股)
	float gjg;     //国家股(万股)
	float fqrfrg;  //发起人法人股(万股)
	float frg;     //法人股(万股)
	float zgg;     //职工股(万股)
	float gzAg;    //公众A股(万股)    5
	float ltAg;    //流通A股(万股)
	float Hg;      //Ｈ股(万股)
	float Bg;      //B股(万股)
	float zpg;     //转配股(万股)

	float zzc;     //总资产(万元)    10
	float ldzc;    //流动资产(万元)
	float cqtz;    //长期投资(万元)
	float gdzc;    //固定资产(万元)
	float wxzc;    //无形资产(万元)
	float ldfz;    //流动负债(万元)   15
	float cqfz;    //长期负债(万元)
	float ggqy;    //股东权益(万元)
	float zbgjj;   //资本公积金(万元)
	float yygjj;   //盈余公积金(万元)
	float mgjz;    //每股净值(元)    20
	float gdqybl;  //股东权益比率(%)
	float mggjj;   //每股公积金(元)

	float zyywsr;  //主营业务收入(万元)
	float zyywlr;  //主营业务利润(万元)
	float qtywlr;  //其它业务利润(万元)25
	float lrze;    //利润总额(万元)
	float jlr;     //净利润(万元)
	float wfplr;   //未分配利润(万元)
	float mgsy;    //每股收益(元)
	float jzcsyl;  //净资产收益率(%)  30
	float mgwfplr; //每股未分配利润(元)

	float mgjzc;   //每股净资产(元)

	int m_iPos;
	BYTE  free[8];
	Split m_Split[80];
}BASEINFO;
typedef BASEINFO *PBASEINFO;
typedef struct
{
	char Symbol[8];  //股票代码
	int  NumSplit;   //已经除权次数
	float zgb;     //总股本(万股)
	float gjg;     //国家股(万股)
	float fqrfrg;  //发起人法人股(万股)
	float frg;     //法人股(万股)
	float zgg;     //职工股(万股)
	float gzAg;    //公众A股(万股)
	float ltAg;    //流通A股(万股)
	float Hg;      //Ｈ股(万股)
	float Bg;      //B股(万股)
	float zpg;     //转配股(万股)

	float zzc;     //总资产(万元)
	float ldzc;    //流动资产(万元)
	float cqtz;    //长期投资(万元)
	float gdzc;    //固定资产(万元)
	float wxzc;    //无形资产(万元)
	float ldfz;    //流动负债(万元)
	float cqfz;    //长期负债(万元)
	float ggqy;    //股东权益(万元)
	float zbgjj;   //资本公积金(万元)
	float yygjj;   //盈余公积金(万元)
	float mgjz;    //每股净值(元)
	float gdqybl;  //股东权益比率(%)
	float mggjj;   //每股公积金(元)

	float zyywsr;  //主营业务收入(万元)
	float zyywlr;  //主营业务利润(万元)
	float qtywlr;  //其它业务利润(万元)
	float lrze;    //利润总额(万元)
	float jlr;     //净利润(万元)
	float wfplr;   //未分配利润(万元)
	float mgsy;    //每股收益(元)
	float jzcsyl;  //净资产收益率(%)
	float mgwfplr; //每股未分配利润(元)

	float mgjzc;   //每股净资产(元)

	int m_iPos;
	BYTE  free[8];
	Split m_Split[80];
}BASEINFO_OLD;
typedef BASEINFO_OLD *PBASEINFO_OLD;
typedef struct
{
	int DayAddr;
	int Min5Addr;
	BASEINFO *pBaseInfo;
	int HsAddr;
	char Free[8];
}MapFileAddr;

typedef struct
{
	long m_lFileTitle;        
    long m_lFileExit;          
	long m_lStockTypeCount;   
    long m_lStockTypeMaxCount; 
    long m_lStockCount;      
    long m_lStockMaxCount;    
	long m_lLastTime;         

}STOCKTYPEHEAD;
typedef STOCKTYPEHEAD *PSTOCKTYPEHEAD;


typedef struct
{
    long m_lIsUse;        
	char m_szCode[7];     
    char m_szName[12];    
	long m_lStockCount;    
    float m_fTotalRight;   
	float m_fNewIndex;     
	float m_fYstIndex;    
	float m_fOpenIndex;
	float m_fHighIndex;   
	float m_fLowIndex;    
	float m_fTotP;      
	float m_fTotV;        
	float m_fTotRVol;   
	float m_fTotDVol;     
    int  m_iRightType;    
    BOOL m_bIsDelete;      
}STOCKTYPEINFO;
typedef STOCKTYPEINFO *PSTOCKTYPEINFO;
typedef struct
{
	char m_szSymbol[9];	
	BOOL m_bDeleted;
	int m_iPos;
	float m_ClosePrice;
    float m_fRight[MaxRights];    
    BYTE  m_btStockType[MaxStockTYpe];    
}STOCK_TYPE_INFO;
typedef STOCK_TYPE_INFO *PSTOCK_TYPE_INFO;
typedef struct
{
	STOCK_TYPE_INFO *pStockInfo;
}STOCK_POINT_INFO;

//======================
typedef struct
{
	char m_szSymbol[7];
	BOOL m_bDeleted;
	int m_iPos;
	float m_ClosePrice;
    float m_fRight[MaxRights];   
    BYTE  m_btStockType[MaxStockTYpe];    
}STOCK_TYPE_INFO_OLD;



typedef struct
{
	BYTE header[32];
	BYTE kind;
	BYTE rdp;			// 用于计算大盘强弱指标
	WORD wMarket;		// 证券市场
	char id[8];			// 证券代码
	char name[10];		// 证券名称 
	char Gppyjc[6];		// 证券名称拼音简称
	int sel;
	float ystc;			// 昨天收盘
	float opnp;			// 今日开盘 
	float higp;			// 今日最高
	float lowp;			// 今日最低
	float nowp;			// 最新价格
	float nowv;			// 最新成交量
	float totv;			// 总成交量
	float totp;			// 总成交金额
	float pbuy1;		// 买入价一
	float vbuy1;		// 买入量一
	float pbuy2;		// 买入价二
	float vbuy2;		// 买入量二
	float pbuy3;		// 买入价三
	float vbuy3;		// 买入量三
	float pbuy4;
	float vbuy4;
	float pbuy5;
	float vbuy5;
	float psel1;		// 卖出价一
	float vsel1;		// 卖出量一
	float psel2;		// 卖出价二
	float vsel2;		// 卖出量二
	float psel3;		// 卖出价三
	float vsel3;		// 卖出量三
	float psel4;
	float vsel4;
	float psel5;
	float vsel5;
	float accb;
	float accs;
	float volume5;
	float rvol;
	float dvol;
	BYTE none[40];

	short lastclmin;
	short initdown;
	int InOut;
	BOOL IsDelete;
	BOOL IsMxTj;

	float Index[10];
	Kdata1 m_Kdata1[244];
	BASEINFO* pBaseInfo;
	STOCK_TYPE_INFO* pStockTypeInfo;

	int	m_serialNumber;
} CReportData;
typedef CReportData* PCdat1;


typedef struct _tagSTOCKDATASHOW
{
	CReportData* pItem;
	WORD wMarket;
	char StockId[7];
	float ItemData;
} STOCKDATASHOW;
typedef STOCKDATASHOW* PSTOCKDATASHOW;


typedef struct
{
 char name[9];          
 float ystc;
 float nowp;            
 float nowb;            
 float nows;           
 float nowv;            
 BOOL InOut;
 char kind;
}CUOHEDATA;


typedef struct
{
 float ystc;                
 float nowi;             
 float opni;             
 float higi;             
 float lowi;              
 float nowp;             
 float nowv;             
 float accb;             
 float accs;           
 float bsspbas;          
 float dif_accb;        
 float dif_accs;         
 float volume5;         

 float StartP;            
 float EndP;         


 float totp;            
 float totv;             
 float lastaccb;         
 float lastaccs;         
 float lasttotb;                        
 float lasttots;
 float lasttotp;                      
 float lasttotv;               

 DpKdata1 *m_DpKdate;           
} Aidx1;

typedef struct
{
 short rsn;       
 short dnn;
} Rsdn1;

typedef Rsdn1 **PRsdn1;



typedef struct
{
 short sec5;              
 short rp;              
 short dp;              
 short res;
} Tidxd;
typedef Tidxd **PTidxd;



typedef struct _tagREALDATA
{
	long filetitle;
	long Day;
	short OldANT[3];
	BOOL CloseWorkDone;
	long StockCount;
	long MaxStockCount;
	long FileExitDone;
	BYTE Res[12];
} REALDATA;
typedef REALDATA* PREALDATA;



typedef struct
{
	long filetitle;            
	long FileExitDone;
	long StockCount;
	long MaxStockCount;
    long FixedNumSplit;
	BYTE Res[20];
}BASEINFOHEAD;
typedef BASEINFOHEAD *PBASEINFOHEAD;

typedef struct _BIG_LIST
{
	char id[8];
	char name[10];
	int nKind;
	time_t ntime;
	int  ntype; 
	float nowv; 
	float nowp;  
	float rdr;
}BIG_LIST;
typedef BIG_LIST *PBIG_LIST;
typedef struct _BIG_LISTHEAD
{
	BOOL Done;
	BOOL IsGbjg;
	BOOL IsChange;
	float BigSet[4];
	float BigMaxSet[4];
	float BigBillSetType[10];
	float BigSetGB[4];
	float BigMaxSetGB[4];
	float BigBillSetTypeGB[10];
    long ItemCount ;
	long MaxItemCount;
	long totalBigCount[4];
	float totalBigVolumn[4];
	long totalCount[10];
	float totalVolumn[10];
    float totalV;
	BOOL IsCalc[4];
	BOOL IsSaveData;
}BIG_LISTHEAD;
typedef BIG_LISTHEAD *PBIG_LISTHEAD;
//
typedef struct _SHARES_DATA_ADD
{
	char name[10];
	int  StockType;
}SHARES_DATA_ADD;



typedef struct  NameSymbolStr 
{
        char   stocksymblo[7];
		char   stockname[9];
		char   stockhypy[5];
		char   group;
}NAMESYMBOL;


typedef struct  tagMIN_TOTAL_STRUCTEx 
{
	RCV_MINUTE_STRUCTEx *m_RcvMinute;    
	long Min_Count;                      

}MIN_TOTAL_STRUCTEx;


typedef struct  tagDAY_TOTAL_STRUCTEx 
{
	RCV_HISTORY_STRUCTEx *m_RcvDay;   
	long Day_Count;                      

}DAY_TOTAL_STRUCTEx;
typedef struct  tagPOWER_TOTAL_STRUCTEx 
{
	RCV_POWER_STRUCTEx *m_RcvPower;   
	long Power_Count;                   

}POWER_TOTAL_STRUCTEx;

/////////////////////////////////////////////////////////////

struct TAI_SHAN_DATA
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

////////////////////////////////////////////////

struct KLINEHEAD
{
	int	NumChuQPerBlk;	   	
	int	numPerBlk	;		
	int	numKline_sh	;		
	int	numBlkAll_sh;		
	int numKline_sz	;	       
	int numBlkAll_sz;	     
};

struct KLINESMALLHEAD
{
	char    StockSign[8]; 
	short 	numChuQ	;	  
	long    numKln	;	
	short   symBlock[25]; 
	long    index;
};


struct MINUTEKLINEHEAD
{
	char    StockSign[8];
	long    numKln	;	  
	long    pPos;        
	long    symBlock;    
};

struct MKLINEHEAD
{
	int	numPerBlk	;		
	int	numKline_sh	;	
	int numKline_sz	;
};

typedef struct
{
  time_t day;
  float opn;
  float hig;
  float low;
  float clo;
  float vol;
  float amt;
  short adv;
  short dec;
} Misd;

typedef struct
{
  char  m_zqdm[7]; 
  float SortID;
} SORTSTRUCT;


typedef struct
{
	long day;
	Misd misd[48];
}MINUTEData;


typedef struct _ADD_DATA_DAY_LINE
{
	char DayLineName[10];
	int  DayLineType;
}ADDDAYLINEDATA;

typedef struct _tagIndex_Technique
{
       int  iKind;                   
       char sIndex_name[50];          
       int  iLine;                   
       int  iDataType;                 
       float fParam[8];               
       float fSubParam1;            
       float fSubParam2;              
       int  iSunIndex;               
	   char SubLineType[10];
}Index_Technique;

typedef struct _TECHNIQUE_COLUMN{ 
    UINT     nID ;      
	Index_Technique nIndex;
	BOOL IsUse;
} TECHNIQUE_COLUMN ,*PTECHNIQUE_COLUMN; 

typedef struct _NineViewData
{
	int minutecount;
	BOOL IsDay;
	UINT showtype;
}NineViewData;

typedef struct _TotalNineViewData
{
	int NineInitDone;
    NineViewData m_NineViewDate[9];
}TotalNineViewData;


typedef struct _SystemInitData
{
	int InitComplete;
    char StockTypeName[20];     
	char ScreenStockName[20];    
	int  ColumnIndex[30]; 
	int  ColumnType;               
	int  ColumnWidth[30];  
	TECHNIQUE_COLUMN TechniqueColumn[MAXJISHUCOUNT];     
    char NetAddr[255];               
	char KswtAddr[255];          
    int  RealTime;                
	int  BlockTime;                    
	TotalNineViewData m_TotalNineViewData;
}SYSTEMINITDATA;	 







#endif 
