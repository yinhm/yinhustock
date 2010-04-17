// by ÓáÃ÷Â¼
#define MAXDATANUM	( ALLTIME * 60 / TIMERDELAY )	
#define		MAXKLINEITEMS	48
typedef struct{
	float current;
	float volume;
	float trade;
}DATA;
typedef struct{
	float sell_cur;
	float buy_cur;
}DATAMM;
typedef struct{
	long    date;
	long	open;
	long	high;
	long	low;
	long	close;
	long    trade;
	long    volume;
}DATA_KLINE1;


typedef	struct{
	char	symbol[8];
	char	name[16];

	float	close;
	float	open;
	
	float	high;
	float	low;

	float	buy1price;
	float	buy1volume;

	float	buy2price;
	float	buy2volume;

	float	buy3price;
	float	buy3volume;

	float	sale1price;
	float	sale1volume;

	float	sale2price;
	float	sale2volume;

	float	sale3price;
	float	sale3volume;

	
	
	DATA	data[MAXDATANUM];
	DATA_KLINE1   klndata5[48];

	float        volume_per_num;
	float        volume_per[20];

}DATA_INFO;
typedef	struct{
	float	close;
	float	high;
	float	low;
	float	price[MAXDATANUM];
	float	volume[MAXDATANUM];
}DATA_INFO_DAPAN;

typedef struct{
	int		nTimes;
	int		nStocknum;
	DATA_INFO*	pInfo;
}SENDPARAM;

typedef struct{
	float	open;
	float	close;
	float	high;
	float	low;

	int		volume;		
	
	WORD		date;
}DATA_KLINE;

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
	WORD     pri_3;
	int    guben;
}DATA_KLINE2;

typedef struct{
	char		symbol[8];
	char		name[16];

	DATA_KLINE	data[MAXKLINEITEMS];
}KLINESEND;

typedef struct{
	int		nStocknum;
	int		nItems;

	KLINESEND*		pSend;
}SENDKLINE;

typedef struct{
	SENDPARAM*	pSH;
	SENDPARAM*	pSZ;
	SENDKLINE*	pKline;
}SENDTOTAL;

typedef struct{
	float current_sh;
	float volume_sh;///////////   
	float trade_sh;  //////////  
	float zhangdie_sh;  //////////  
	float rp_sh;  //////////  
	float dp_sh;  //////////  
	
	float current_sz;
	float volume_sz;///////////  
	float trade_sz;  //////////     
	float zhangdie_sz;  //////////  
	float rp_sz;  //////////  
	float dp_sz;  //////////  
}STATUSBAR_ALL;

typedef struct{
	float A_gu;
	float A_guzhd;///////////   
	float B_gu;  //////////  
	float B_guzhd;  //////////  

	float buy_hand;  //////////  
	float sell_hand;  //////////  

	float kaipan ;
	float zuigao ;
	float zuidi ;
	
	float zhang_num;
	float ping_num;///////////    /100
	float die_num;  //////////     
}RIGHTBOX_DAPAN;
typedef struct{
	float zongzhi_sh;
	float A_gu_sh;
	float B_gu_sh;  //////////  
	float sh30;  //////////  

	float zongzhi_zhd_sh;///////////    
	float A_guzhd_sh;///////////    
	float B_guzhd_sh;  //////////  
	float zhd_sh30;  //////////  

	float volume_sh ;
	float A_volume_sh ;
	float B_volume_sh ;
	float zhaiquan_volume_sh ;
	
	float trade_sh;
	float A_trade_sh;
	float B_trade_sh;///////////    /100
	float zhaiquan_trade_sh;  //////////     

	float gu_sz;
	float A_gu_sz;
	float B_gu_sz;  //////////  
	float zonghe_sz;  //////////  
	float guzhd_sz;///////////    /100
	float A_guzhd_sz;///////////    /100
	float B_guzhd_sz;  //////////  
	float zhd_zonghe_sz;  //////////  

	float volume_sz ;
	float A_volume_sz ;
	float B_volume_sz ;
	float zhaiquan_volume_sz ;
	
	float trade_sz;
	float A_trade_sz;
	float B_trade_sz;///////////    /100
	float zhaiquan_trade_sz;  //////////     

}SHSZ_TONGJI;

typedef struct{
 float lingxian_sh[MAXDATANUM];
 float ADL_sh[MAXDATANUM];
 float duokong_sh[MAXDATANUM];
 float mmlidao_sh[2][MAXDATANUM];
 float lingxian_sz[MAXDATANUM];
 float ADL_sz[MAXDATANUM];
 float duokong_sz[MAXDATANUM];
 float mmlidao_sz[2][MAXDATANUM];

 DATA_INFO_DAPAN zhibiao_array[2][6];
 DATA_INFO_DAPAN shAndZh_array[2];

 RIGHTBOX_DAPAN rightbox[2];

 STATUSBAR_ALL statursbar_all;

 SHSZ_TONGJI   shsz_tongji;
}DAPAN_STRUCT;
typedef struct{
	UCHAR s1[8];
	SDWORD   ws1;
	UCHAR s2[16];
	SDWORD   ws2;

	float s3;
	float s4;
	float s6;
	float s7;

	float s8;
	float s11;
	float s13;
	float s5;
	
	float s9;
	float s15;

	float s16;
	float s17;

	float s18;
	float s19;

	float s10;
	float s21;
	float s22;

	float s23;
	float s24;
	float s25;
	SDWORD   ws3;
	SDWORD   ws4;
	SDWORD   ws5;
	SDWORD   ws6;
	SDWORD   ws7;
	SDWORD   ws8;
	SDWORD   ws9;
	SDWORD   ws10;
	SDWORD   ws11;
	SDWORD   ws13;
	SDWORD   ws15;
	SDWORD   ws16;
	SDWORD   ws17;
	SDWORD   ws18;
	SDWORD   ws19;
	SDWORD   ws21;
	SDWORD   ws22;
	SDWORD   ws23;
	SDWORD   ws24;
	SDWORD   ws25;
}TABLESHOW128;
//////////////////////////////
typedef struct{
	RECT    TR;
	long	open;
	long	high;
	long	low;
	long	close;
	long    trade;
	long    volume;
}RECTREGION;

