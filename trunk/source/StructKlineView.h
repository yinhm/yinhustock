
#pragma once

#include <afxtempl.h>
#include "CFormularContent.h"
#include "lineRightBox.h"

#define WM_USER_DRAG_BEGIN	WM_USER+ 256

#define VOLSTICK                    1
#define STICK_VOL_FS                2
#define CIRCLE		                3
#define LINESTICK                   4
#define CROSSDOT                    5
#define STICK                       6
#define STICK_LINGXIAN_FS		    7



#define CHUQ_PER_BLK		6
#define KLINE_PER_BLK		256 
#define NUMBYTE_PER_KLINE	32 
#define NUMBYTE_PER_CHUQ	32 
#define NUM_STK_K_TOTAL		4096 

#define	STCKNAMELEN		50
#define	FENSHINUM		240


#define m_midLen_L                  40

#define STATICID                       600667

#define WIDTH_KLINEBODY                    6
#define LEN_KLINE                       10


#define CORLOR_ZUOBIAO                    RGB(0,0,0)
#define RIGHTBOX_PERLINE                    18



#define RIGHTHEIGHT                      250

#define XIAN_DUAN                   0
#define SHUI_PING                   1
#define QU_SHI                      2
#define PING_XING					3
#define SU_DU		                4
#define SHAN_XING                   5
#define GAN_SHI                     6
#define HUANG_JIN					7
#define BO_DUAN		                8
#define BAI_FEN_BI                  9
#define HUI_GUI                     10
#define HUI_GUI_DAI_1               11
#define HUI_GUI_DAI_2		        12
#define ZHOU_QI						13
#define NA_QI                       14
#define ZHONG_XIAN					15
#define NA_QI_HU		            16
#define HUANG_JIN_HU                17
#define UP_ARROW                    18
#define DOWN_ARROW					19
#define RECT_KIND		            20
#define TEXT_KIND		            21
#define DELETE_LINE		            22
#define BAG_LINE		            23
#define SPIRE_LINE		            24

#define  REGION_NO		0


#define  REGION_PICTBORDER1		1
#define  REGION_PICTBORDER2		2
#define  REGION_PICTBORDER3		3
#define  REGION_PICTBORDER4		4


#define  REGION_PICT1		5
#define  REGION_PICT2		6
#define  REGION_PICT3		7
#define  REGION_PICT4		8
#define  REGION_PICT5		9


#define  REGION_RULER		11

#define  REGION_TRANSFER		12


#define  REGION_PICTCAPTION1		14
#define  REGION_PICTCAPTION2		15
#define  REGION_PICTCAPTION3		16
#define  REGION_PICTCAPTION4		17
#define  REGION_PICTCAPTION5		18


#define  REGION_PICTTEXT1		24
#define  REGION_PICTTEXT2		25
#define  REGION_PICTTEXT3		26
#define  REGION_PICTTEXT4		27
#define  REGION_PICTTEXT5		28


#define  REGION_MANY_FIGUER		36

//ÓÒÏÂ½ÇµÄTabÇø
#define  REGION_LITTLE_FIGUER		37

#define  REGION_SUB				46
#define  REGION_ADD				47
#define  REGION_INFO_TRANSFER		48

#define  REGION_SELFLINE_1		58
#define  REGION_SELFLINE_2		59
#define  REGION_SELFLINE_3		60
#define  REGION_SELFLINE_BODY		61
#define  REGION_RIGHT_LINE		70

#define  XIA_DIE_WORD       15
#define  PING_PAN_WORD      14


struct INFO_LINESELF{
	int 	type;
	int     klineType;
	int		time1;
	float	valFlt1;
	int		time2;
	float	valFlt2;
	int		time3;
	float	valFlt3;
	COLORREF	color;
	BYTE	byLineKind;
	BYTE	byLineWidth;
	BYTE	byAlarm;
	BYTE	byWhichFiguer;
	int     nDays;
	int     stored;
	INFO_LINESELF()
	{
		byWhichFiguer = 0;
	}
};


typedef struct{
	char ZhBName[20];	
	int  m_yBottom;			
}Index_BELLOW;

typedef struct{

	int  rightX;	
	Index_BELLOW rtBlw[5];	
}RECTKLINE;

typedef struct{
	int  rightX;		
	int  leftX;		
	Index_BELLOW rtBlw[5];	
}RECTFENSHI;

struct LooseValue
{
	int nFoot;
	float fVal;
};
typedef CArray<LooseValue,LooseValue&> looseValueArray; 
class ARRAY_BE
{ 
public:
	float *line;
	int b;
	int e;
	float *lineWidth;

	enum Kind
	{
		Normal = 0,
		DrawLine,
		DrawPolyLine,
		DrawStickLine,
		DrawIcon,
		StringData,
		LineRt,
	};

	Kind kind;
	WORD nLineWidth;
	WORD isVirtualLine;
	CString s;
	ARRAY_Right_Box lineRight;
	looseValueArray looseArr;

	ARRAY_BE()
	{
		s = "";
		line = NULL;
		lineWidth = NULL;
		kind = Normal;
		b=0;
		e = 0;
		nLineWidth = 1;
		isVirtualLine = 0;
	};
	virtual ~ARRAY_BE()
	{
		if(lineWidth) delete [] lineWidth;
	};
	ARRAY_BE& operator=(ARRAY_BE& in)
	{
		this->b=in.b;
		e = in.e;
		kind = in.kind;
		nLineWidth = in.nLineWidth;
		isVirtualLine = in.isVirtualLine;
		s = in.s;
		if(line&&in.line && b>=0)
		{
			for(int i = b;i<=e;i++)
				this->line[i]=in.line[i];
		}

		if(kind == LineRt)
			lineRight = in.lineRight;
		if(kind	>=	DrawLine)
		{
			looseArr.Copy(in.looseArr );
		}

		if(kind == DrawStickLine)
		{
			if(this->lineWidth) delete [] lineWidth;
			if(in.e>=0)
			{
				lineWidth = new float[in.e+1];
				if(in.lineWidth)
					memcpy(lineWidth,in.lineWidth ,sizeof(float)*(in.e+1));
			}
			else
				lineWidth = NULL;
		}


		return *this;
	};
	bool Add(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		b=max(in1.b,in2.b);
		e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			s = in1.s;
		if(!in2.s.IsEmpty())
			s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			kind = LineRt;
			lineRight = in1.lineRight+in2.lineRight;
		}
		else
		{
			kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<= e;i++)
					line[i]=in1.line[i]+in2.line[i];
			}
		}
		return true;
	};
	bool Add(ARRAY_BE& in1,float fIn2 )
	{
		b=in1.b;
		e=in1.e;
		if(!in1.s.IsEmpty())
			s = in1.s;

		kind = in1.kind;
		if(in1.kind == LineRt )
		{
			lineRight = in1.lineRight+fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					line[i]=in1.line[i]+fIn2;
			}
		}
		return true;
	};
	bool Sub(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		b=max(in1.b,in2.b);
		e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			s = in1.s;
		if(!in2.s.IsEmpty())
			s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			kind = LineRt;
			lineRight = in1.lineRight-in2.lineRight;
		}
		else
		{
			kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					line[i]=in1.line[i]-in2.line[i];
			}
		}
		return true;
	};
	bool Sub(ARRAY_BE& in1,float fIn2 )
	{
		b=in1.b;
		e=in1.e;
		if(!in1.s.IsEmpty())
			s = in1.s;

		kind = in1.kind;
		if(in1.kind == LineRt )
		{
			lineRight = in1.lineRight-fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					line[i]=in1.line[i]-fIn2;
			}
		}
		return true;
	};
	bool Sub(float fIn2,ARRAY_BE& in1 )
	{
		b=in1.b;
		e=in1.e;
		if(!in1.s.IsEmpty())
			s = in1.s;

		kind = in1.kind;
		if(in1.kind == LineRt )
		{
			lineRight = fIn2 -in1.lineRight;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					line[i]=fIn2-in1.line[i];
			}
		}
		return true;
	};
	bool Mult(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		b=max(in1.b,in2.b);
		e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			s = in1.s;
		if(!in2.s.IsEmpty())
			s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			kind = LineRt;
			lineRight = in1.lineRight*in2.lineRight;
		}
		else
		{
			kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					line[i]=in1.line[i]*in2.line[i];
			}
		}
		return true;
	};
	bool Mult(ARRAY_BE& in1,float fIn2 )
	{
		b=in1.b;
		e=in1.e;
		if(!in1.s.IsEmpty())
			s = in1.s;

		kind = in1.kind;
		if(in1.kind == LineRt )
		{
			lineRight = in1.lineRight*fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
					line[i]=in1.line[i]*fIn2;
			}
		}
		return true;
	};
	bool Div(ARRAY_BE& in1,ARRAY_BE& in2 )
	{
		b=max(in1.b,in2.b);
		e=min(in1.e,in2.e);
		if(!in1.s.IsEmpty())
			s = in1.s;
		if(!in2.s.IsEmpty())
			s = in2.s;

		if(in1.kind == LineRt || in2.kind == LineRt)
		{
			kind = LineRt;
			lineRight = in1.lineRight/in2.lineRight;
		}
		else
		{
			kind = in1.kind>in2.kind ? in1.kind : in2.kind ;

			if(in1.line&&in2.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
				{
					if(in2.line[i]!=0)
						line[i]=in1.line[i]/in2.line[i];
					else
					{
						if(i>0)
							line[i] =  line[i-1];
						else
							line[i]= 0;
					}
				}
			}
		}
		return true;
	};
	bool Div(ARRAY_BE& in1,float fIn2 )
	{
		b=in1.b;
		e=in1.e;
		if(!in1.s.IsEmpty())
			s = in1.s;

		kind = in1.kind;
		if(in1.kind == LineRt )
		{
			lineRight = in1.lineRight/fIn2;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
				{
					if(fIn2!=0)
						line[i]=in1.line[i]/fIn2;
					else
					{
						if(i>0)
							line[i] =  line[i-1];
						else
							line[i]= 0;
					}
				}
			}
		}
		return true;
	};
	bool Div(float fIn2,ARRAY_BE& in1 )
	{
		b=in1.b;
		e=in1.e;
		if(!in1.s.IsEmpty())
			s = in1.s;

		kind = in1.kind;
		if(in1.kind == LineRt )
		{
			lineRight = fIn2 /in1.lineRight;
		}
		else
		{
			if(in1.line &&  line &&  b>=0)
			{
				for(int i =  b;i<=e;i++)
				{
					if(in1.line[i] != 0 )
						line[i]=fIn2/in1.line[i];
					else
					{
						if(i>0)
							line[i] =  line[i-1];
						else
							line[i]= 0;
					}
				}
			}
		}
		return true;
	};
};

typedef struct 
{
	int     day;
	float	open;
	float   high;
	float   low;
	float   close;
	float   vol;
	float   amount;
	union
	{
		struct
		{
			WORD    advance;
			WORD    decline;
		};
		float   volPositive;
	};
}Kline;
typedef struct 
{
	int     day;
	float	open;
	float   high;
	float   low;
	float   close;
	float   vol;
	float   amount;
	float   volPositive;
}KlineGeneral;

struct ARRAY_BE_SHOW
{ 
	ARRAY_BE m_arrBE;
	int bNow;
	int type;
	int lineThick;
	COLORREF	clr;
	ARRAY_BE_SHOW()
	{
		clr = -1;
		lineThick = 1;
	};
	//	char name[10];
};
typedef struct{
	int numLine;
	ARRAY_BE_SHOW	line[64];
}ARRAY_JISHU;

typedef struct{
	long    date;
	long	open;
	long	high;
	long	low;
	long	close;
	long    trade;
	long    volume;
	long    amout_1;
	char    pri_1;
	char    pri_2;
	WORD    pri_3;
	int    guben;
}DATA_KLINE2;

typedef struct{
	float price;
	float vol;
}FENJIA;

struct POWER    
{
	int   nFlags; 
	int   nTime;
	float fGive;    
	float fAllocate;
	float fAllocatePrice;//
	float fDividend;
	BYTE  byFree[4];
};
struct	SCREEN_DETAIL_L           
{
	int buytime;
	int selltime;
	float  buyprice;
	float  sellprice;
	int    timerange;
	float  profit;
	BOOL   issuccess;
};
struct  SCREEN_RESULT_L
{
	CString symbol;
	CString name;
	int     choosecount;
	int     successcount;
	float   successpercent;
	BOOL    isuseful;
	CArray <SCREEN_DETAIL_L*,SCREEN_DETAIL_L*> m_detail;
};

#define IDM_POP_TJXG_SHOW		300//
#define IDM_POP_KLINE_ADDED		301//
#define IDM_POP_CHANGE_PARAM		302//
#define IDM_POP_DRAG_MODE		303//


#define MANY_DAY_K_LINE		15
#define HS_K_LINE		25
#define MINUTE_1_K_LINE		35
#define NUM_MANY_DAY		50



struct INDEX_INFO
{
	WORD	advance;
	WORD	decline;
	float	amount;
	Tidxd	tidxd;
	float	faccbaccs;
};

struct TRADE_DETAIL_INFO
{
	float	unitCountVol;
	float	unitCountPrc;
	BYTE	buyP[3];
	BYTE	sellP[3];
	BYTE	buyV[3];
	BYTE	sellV[3];
};

union TRADE_DETAIL_INDEX
{
	INDEX_INFO			indexInfo;
	TRADE_DETAIL_INFO	buySellInfo;
};

struct TRADE_DETAIL_H_PER
{
	int		time;
	float	price;
	float	vol;	
	float	amount;
	TRADE_DETAIL_INDEX buySellOrIndex;
};

typedef CTypedPtrList<CPtrList, TRADE_DETAIL_H_PER*> CBuySellList;


typedef CArray<POWER,POWER&> powerArr; 
typedef union tagKlineEx
{
	struct
	{
		float buyPrice[3];	
		float buyVol[3];	
		float sellPrice[3];	
		float sellVol[3];	
	};
	float fDataEx[12];		
} KlineEx;

enum klineKind {
	HS_KLINE = 25,
	MANY_DAY_KLINE	=	15,
	MINUTE1_KLINE	=	35,
	MIN5_KLINE = 1,
	MIN15_KLINE = 2,
	MIN30_KLINE = 3,
	MIN60_KLINE = 4,
	DAY_KLINE = 5,
	WEEK_KLINE = 6,
	MONTH_KLINE = 7,
	YEAR_KLINE = 8
};
typedef struct tagINPUT_INFO
{
	char*			  strSymbol;	
	bool			  bIndex;			

	int			  nNumData;	
	Kline*		  pData;			
	KlineEx*	      pDataEx;			

	ARRAY_BE 		fInputParam[10]	;
	ARRAY_BE 		fOutputParam;
	klineKind	  klineType;		
	BASEINFO*    financeData;		
	CReportData* pDat1;
	int nReserved;
	void* pBuySellList;
	int m_stkKind;
} INPUT_INFO;

struct SUB_FIGUER_INIT{
	float	    yFloatBottom;		
	char		nameIndex[20]	;
};


typedef struct{
	int				nCountIndex;			
	SUB_FIGUER_INIT		initIndex[5];	

	int				nCountMin1;			
	SUB_FIGUER_INIT		initMin1[5];	
	int		flag[5]	;


	int		flag_dapan[5]	;

	int nIDRightSheet[10];
}SUB_FIGUER_INIT_INFO;

struct  TRADE_DETAIL_BIG_LIST_PER
{
	float vol;	
	float price;
	int   time;
	float buyPrice[3];	
	float buyVol[3];	
	float sellPrice[3];		
	float sellVol[3];	
};
#define FABSMY(x) ((x)<0?-(x):(x))
extern CString g_strCompanyName;






typedef struct tagSymbolKind
{
	WORD m_wMarket;
	char m_chSymbol[8];
	int m_nSymbolKind;
	char m_szSymbol[10];
	tagSymbolKind()
	{
		m_wMarket = 0;
		memset(m_chSymbol, 0, 8);
		m_nSymbolKind = 0;
		memset(m_szSymbol, 0, 10);
	};
	void tarSymbolKind(WORD wMarket, char* szCode)
	{
		m_wMarket = wMarket;
		strcpy_s(m_chSymbol, szCode);
		m_nSymbolKind = 0;
		memcpy(m_szSymbol, &m_wMarket, sizeof(WORD));
		strcat(m_szSymbol, m_chSymbol);
	};
} SymbolKind;

typedef CArray<SymbolKind, SymbolKind&> SymbolKindArr;
