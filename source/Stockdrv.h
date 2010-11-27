/****************************************************************************************
*								ͨ�Ӽ������ϣ�δ���������ù���							*	
****************************************************************************************/

/****************************************************************************************
*								 WIN95/98 ��Ʊ��������									*
*								 �����������ݹ㲥�о�����								*
*									     1998.3.5										*
*****************************************************************************************/

/****************************************************************************************
*					��ע���� HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv ��					*
*					Driver  = "����DLLȫ·��"											*
*					Provider= "TongShi"													*
*																						*
*	������� LoadLibrary �� GetProcAddress �� FreeLibrary �ķ�ʽ���ù�Ʊ��������		*
*	��Ʊ�����������ļ��������λ���� ע��� HKEY_LOCAL_MACHINE/SOFTWARE/StockDrv		*
*	�� Driver ָ��																		*
*																						*
*																						*
*																						*
*	==========================		���ֽڶ������		===========================		*
*																						*
*																						*
****************************************************************************************/

#ifndef __STOCKDRV_100_H__
#define __STOCKDRV_100_H__

// ������ʽ���Ͷ���
#define RCV_WORK_MEMSHARE			2					// �ڴ湲��ֱ������
#define RCV_WORK_SENDMSG			4					// �汾 2 ����ʹ�õķ�ʽ	
// ����������ʽ����

#define	TS_StockDrv_IdTxt	"TongShi_StockDrv_1.00"		// ͨ����Ϣ��������


// ֤ȯ�г�
#define SH_MARKET_EX			'HS'		// �Ϻ�
#define SZ_MARKET_EX			'ZS'		// ����
#define HK_MARKET_EX            'KH'		// ����г�
#define EB_MARKET_EX            'BE'		// ���ڶ����г�


// �ļ���������
// �ṹ������ʽ���ļ�����

#define FILE_HISTORY_EX				2			// ����ʷ����
#define FILE_MINUTE_EX				4			// ����ʱ����
#define FILE_POWER_EX				6			// ����Ȩ��Ϣ

#define FILE_BASEINFO_EX			10          //����ȫ����Ʊ��������
#define FILE_DISPARTBARGAINING_EX	12          //���ֱʽ�����ϸ����
#define FILE_STOCKLABELLIST_EX		14          //����ȫ����Ʊ�����
#define FILE_SYSTEMRUN_EX			16          //����ϵͳ���в���
#define FILE_INDEXATTRIBUTE_EX		18          //���̺��̾� 
#define FILE_LOGINAUTH_EX			20          //�ͻ�������Ȩ
#define FILE_TECHNICINDEX			22          //�ͻ��˼���ָ��

#define FILE_HISTORY_MINUTE_EX		81			// ����ʷ���������

#define		FILE_BASE_EX			0x1000		// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
#define		FILE_NEWS_EX			0x1002		// ������,��������m_szFileName����Ŀ¼������
#define     FILE_HTML_EX            0x1004      //HTML �ļ���m_szFileNameΪURL
#define     FILE_ALLNEWSINFO_EX     0x1006      //�г�����ȫ��������Ϣ 
#define     FILE_NOTIFY_EX          0x1008      //֪ͨ��Ϣ
#define     FILE_TECHNIC_EX         0x1010      //����˼���ָ���ļ�    
#define     FILE_SOFTWARE_EX        0x2000      //��������      

#define		FILE_TYPE_RES			-1			// ����

/* ��Ϣ������ */
#define News_Sha_Ex					2			// ��֤��Ϣ
#define News_Szn_Ex					4			// ��֤��Ϣ
#define News_Fin_Ex					6			// �ƾ�����
#define News_TVSta_Ex				8			// ����̨֪ͨ
#define News_SzEb_Ex				10			// ���ڶ���
#define News_Unknown_Ex				-1			// δ֪�ṩ��

//Definition For nInfo of Function GetStockDrvInfo(int nInfo,void * pBuf);
#define		RI_IDSTRING				1			// ��������,����(LPCSTR)������
#define		RI_IDCODE				2			// ����
#define		RI_VERSION				3			// ��������汾
#define		RI_ERRRATE				4			// ȡ�ŵ�����
#define		RI_STKNUM				5			// ȡ���й�Ʊ�ܼ���
#define		RI_SUPPORTEXTHQ			 6

#define		STKLABEL_LEN			10			// �ɺ����ݳ���,�����г��ɺű������Ǯ��
#define		STKNAME_LEN				32			// ��������
#define     STKPINGYING_LEN          6

#pragma pack(1)

//----------------------------------------ϵͳ���в������-----------------------------------------------
typedef  struct  tagRCV_SYSTEMRUN_STRUCTEx
{
	BYTE  m_btBargainingTime[3];      //���̷����� 
	BYTE  m_btTodayInitialize;        //�����Ƿ��ʼ�� 
	BYTE  m_btCloseWork;              //�����Ƿ�����  
	long  m_BargainingDate;           //��������  
	float   m_fNewIndex[3];           //����ָ�� 
	float   m_fVolume[3];             //���³ɽ���
	float   m_fAmount[3];             //���³ɽ����
	float   m_fRedGreenArmy[3];       //���̺��̾�
}RCV_SYSTEMRUN_STRUCTEx;
//--------------------------------------------------------------------------
typedef struct tagRCV_LOGINAUTH_STRUCTEx
{
	BOOL  bAuthorizationState;             
	DWORD dwAuthorizationPurview;                 
}RCV_LOGINAUTH_STRUCTEx;

//----------------------------------------��Ʊ�����-------------------------------------------------------
typedef  struct  tagRCV_STKLABEL_STRUCTEx
{
	WORD m_wMarket;                   //��Ʊ�г�����
	WORD m_wPos;                      //����һ�г������й�Ʊ����λ�� 
	char m_szLabel[STKLABEL_LEN];      //��Ʊ����
	char m_szName[STKNAME_LEN];       //��Ʊ����
	char m_szPingYing[STKPINGYING_LEN]; //��Ʊ����ƴ��
	float m_fLastClose;                    //����
	float m_fAverageVolumn;               //5����� 
}RCV_STKLABEL_STRUCTEx;
//-----------------------------------------------------
typedef struct tagRCV_INDEXATTRIBUTE_STRUCTEx
{
	time_t m_time;         
    short sec5;            
    short rp;              
    short dp;             
	short rsn;             
	short dnn;
}RCV_INDEXATTRIBUTE_STRUCTEx;

//------------------------------------- ������Ϣ ------------------------------
typedef  struct tagRCV_BASEINFO_STRUCTEx
{
	char m_szLabel[STKLABEL_LEN];	// ��Ʊ����
	float m_fZgb;					// �ܹɱ�(���)
	float m_fGjg;					// ���ҹ�(���)
	float m_fFqrfrg;				// �����˷��˹�(���)
	float m_fFrg;					// ���˹�(���)
	float m_fZgg;					// ְ����(���)
	float m_fGzAg;					// ����A��(���)
	float m_fLtAg;					// ��ͨA��(���)
	float m_fHg;					// H��(���)
	float m_fBg;					// B��(���)
	float m_fZpg;					// ת���(���)
	float m_fZzc;					// ���ʲ�(��Ԫ)
	float m_fLdzc;					// �����ʲ�(��Ԫ)
	float m_fCqtz;					// ����Ͷ��(��Ԫ)
	float m_fGdzc;					// �̶��ʲ�(��Ԫ)
	float m_fWxzc;					// �����ʲ�(��Ԫ)
	float m_fLdfz;					// ������ծ(��Ԫ)
	float m_fCqfz;					// ���ڸ�ծ(��Ԫ)
	float m_fGgqy;					// �ɶ�Ȩ��(��Ԫ)
	float m_fZbgjj;					// �ʱ�������(��Ԫ)
	float m_fYygjj;					// ӯ�๫����(��Ԫ)
	float m_fMgjz;					// ÿ�ɾ�ֵ(Ԫ)
	float m_fGdqybl;				// �ɶ�Ȩ�����(%)
	float m_fMggjj;					// ÿ�ɹ�����(Ԫ)
	float m_fZyywsr;				// ��Ӫҵ������(��Ԫ)
	float m_fZyywlr;				// ��Ӫҵ������(��Ԫ)
	float m_fQtywlr;				// ����ҵ������(��Ԫ)
	float m_fLrze;					// �����ܶ�(��Ԫ)
	float m_fJlr;					// ������(��Ԫ)
	float m_fWfplr;					// δ��������(��Ԫ)
	float m_fMgsy;					// ÿ������(Ԫ)
	float m_fJzcsyl;				// ���ʲ�������(%)
	float m_fMgwfplr;				// ÿ��δ��������(Ԫ)
	float m_fMgjzc;					// ÿ�ɾ��ʲ�(Ԫ)
} RCV_BASEINFO_STRUCTEx;

//////////////////////////////////////////////////////////////////////////////////
//��������ͷ
//����ͷ m_dwHeadTag == EKE_HEAD_TAG 
#define EKE_HEAD_TAG	0xffffffff

typedef struct	tagRCV_EKE_HEADEx
{
	DWORD	m_dwHeadTag;								// = EKE_HEAD_TAG
	WORD	m_wMarket;									// �г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����
}RCV_EKE_HEADEx;

//------------------------------------------------����ϸ����-----------------------------------------------
typedef  union  tagRCV_DISPBARGAINING_STRUCTEx
{
	struct
	{
		time_t m_time;                      //�ɽ�ʱ�� 
		float  m_fNewPrice;                 // ����
		float  m_fVolume;                  // �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
		float  m_fAmount;                  //�ɽ���,��ԪΪ��λ
		float  m_fBuyPrice[3];              // �����1,2,3
		float  m_fBuyVolume[3];            // ������1,2,3
		float  m_fSellPrice[3];              // ������1,2,3
		float m_fSellVolume[3];             // ������1,2,3
	};
	RCV_EKE_HEADEx  m_head;
}RCV_DISPBARGAINING_STRUCTEx;



///////////////////////////////////////////////////////////////////////////////
// ��������
// ע:
//    ĳЩ�ֶο���Ϊ0,���ϴε����ݴ���
typedef struct tagRCV_REPORT_STRUCTEx
{
	WORD	m_cbSize;									// �ṹ��С
	time_t	m_time;										// ����ʱ��
	WORD	m_wMarket;									// ��Ʊ�г�����
	char	m_szLabel[STKLABEL_LEN];					// ��Ʊ����,��'\0'��β
	char	m_szName[STKNAME_LEN];						// ��Ʊ����,��'\0'��β

	float	m_fLastClose;								// ����
	float	m_fOpen;									// ��
	float	m_fHigh;									// ���
	float	m_fLow;										// ���
	float	m_fNewPrice;								// ����
	float	m_fVolume;									// �ɽ���,�Թ�Ϊ��λ,�ɵĺ�������Ʊ���Ͷ���
	float	m_fAmount;									// �ɽ���,��ԪΪ��λ

	float	m_fBuyPrice[3];								// �����1,2,3
	float	m_fBuyVolume[3];							// ������1,2,3
	float	m_fSellPrice[3];							// ������1,2,3
	float	m_fSellVolume[3];							// ������1,2,3

	float	m_fBuyPrice4;								// �����4
	float	m_fBuyVolume4;								// ������4
	float	m_fSellPrice4;								// ������4
	float	m_fSellVolume4;								// ������4
	float	m_fBuyPrice5;								// �����5
	float	m_fBuyVolume5;								// ������5
	float	m_fSellPrice5;								// ������5
	float	m_fSellVolume5;								// ������5
} RCV_REPORT_STRUCTEx;



// ��ʷ����
// עһ:
//		ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG, �ж��Ƿ�Ϊ m_head, Ȼ����������
typedef union tagRCV_HISTORY_STRUCTEx
{
	struct
	{
		time_t	m_time;				// UCT
		float	m_fOpen;			// ����
		float	m_fHigh;			// ���
		float	m_fLow;				// ���
		float	m_fClose;			// ����
		float	m_fVolume;			// ��
		float	m_fAmount;			// ��
		union
		{
			struct
			{
				WORD m_wAdvance;	// ����,��������Ч
				WORD m_wDecline;	// ����,��������Ч
			};
			float m_fActiveBuyVol;	// ��������
		};
	};
	RCV_EKE_HEADEx m_head;
} RCV_HISTORY_STRUCTEx;

// ��ʱ����
// עһ:
//		ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG, �ж��Ƿ�Ϊ m_head, Ȼ����������
typedef union tagRCV_MINUTE_STRUCTEx
{
	struct
	{
		time_t m_time;				// UCT
		float m_fPrice;				// ��
		float m_fVolume;			// ��
		float m_fAmount;			// ��
	};
	RCV_EKE_HEADEx m_head; 
} RCV_MINUTE_STRUCTEx;

// ��Ȩ����
// עһ:
//		ÿһ���ݽṹ��Ӧͨ�� m_time == EKE_HEAD_TAG, �ж��Ƿ�Ϊ m_head, Ȼ����������
typedef union tagRCV_POWER_STRUCTEx
{
	struct
	{
		time_t m_time;				// UCT
		float m_fGive;				// ÿ����
		float m_fPei;				// ÿ����
		float m_fPeiPrice;			// ��ɼ�
		float m_fProfit;			// ÿ�ɺ���
	};
	RCV_EKE_HEADEx m_head;
} RCV_POWER_STRUCTEx;

//////////////////////////////////////////////////////////////////////////////////
// �ļ��������ݰ�ͷ
//  עһ:
//	m_wDataType == FILE_BASE_EX
//				m_dwAttrib = ��Ʊ֤ȯ�г�,m_szFileName�������ļ���
//				m_FileTime = �����������ļ���������
//  m_wDataType == FILE_NEWS_EX
//				m_dwAttrib = ��Ϣ��Դ,m_szFileName ����Ŀ¼���ļ���,Ŀ¼��Ϊ��Ϣ��Դ
//				m_dwSerialNo = ���к�
//				��:  "�Ͻ�����Ϣ\\0501Z012.TXT","������\\XLD0001.TXT"
//  ע��:
//		�����ļ�ѭ������,ÿ���ļ���Ψһ�����к�,�Ա����ظ�����
typedef struct tagRCV_FILE_HEADEx
{
	DWORD	m_dwAttrib;							// �ļ�������
	DWORD	m_dwLen;							// �ļ�����
	union {
		DWORD	m_dwSerialNo;					// ���к�,�Թ���
		time_t	m_FileTime;						// �ļ�����ʱ��
	};
	char	m_szFileName[_MAX_PATH];			// �ļ��� or URL
} RCV_FILE_HEADEx;

//////////////////////////////////////////////////////////////////////////////////
//  ����֪ͨ��Ϣ
//	wParam = RCV_WPARAM;
//  lParam ָ�� RCV_DATA�ṹ;
//	���� 1 �Ѿ�����, 0 δ�������ܴ���,Ŀǰ�÷���ֵ������

#define RCV_REPORT			0x3f001234
#define RCV_FILEDATA		0x3f001235



// עһ:
//		��¼����ʾ�������ݺͲ�������(���� Header)�����ݰ���, ���ļ���������, = 1
// ע��:
//		�� m_bDISK = FALSE, m_pData Ϊ��Ϣ���������ݻ�����ָ��
//
//		******** ���ݹ���,�����޸����� **********
//
//		m_bDISK = TRUE, m_pData Ϊ���ļ��Ĵ����ļ���, һ�����ֻ��
//		���������ȴ��ļ��ô��̷�ʽ
typedef struct tagRCV_DATA
{
	int					m_wDataType;			// �ļ�����
	int					m_nPacketNum;			// ��¼��, �μ�עһ
	RCV_FILE_HEADEx		m_File;					// �ļ��ӿ�
	BOOL				m_bDISK;				// �ļ��Ƿ��Ѵ��̵��ļ�
	union
	{
		RCV_HISTORY_STRUCTEx* m_pDay;					// ����������
		RCV_MINUTE_STRUCTEx* m_pMinute;					// ����ʱ����   
		RCV_POWER_STRUCTEx* m_pPower;					// ����Ȩ����

		RCV_REPORT_STRUCTEx *m_pReport;                   //��������
		RCV_DISPBARGAINING_STRUCTEx *m_pDispBargaining;  //��ϸ����
		RCV_BASEINFO_STRUCTEx *m_pBaseInfo;               //��������
		RCV_STKLABEL_STRUCTEx *m_pStkLabel;               //������ձ�
		RCV_SYSTEMRUN_STRUCTEx *m_pSystemRun;            //ϵͳ���в���
		RCV_INDEXATTRIBUTE_STRUCTEx *m_pIndexAttribute;   //���̺��̾� 
		RCV_LOGINAUTH_STRUCTEx *m_pLoginAuth;             //������Ȩ״̬       
		void *m_pData;                                    //�ļ��ڴ�ָ��   
	};
} RCV_DATA;


// ===================== �ɺ�����ʦ ===========================================
typedef struct tagFOX_DATA
{
	DWORD	m_dwFlag;
	DWORD	m_dwType;
	int		m_nCount;
} FOX_DATA;



//==================ͬ����ͨѶ��ӿ������ݵ���ṹ=====================
typedef struct RCV_COMMINTERFACE_STRUCTEx
{
	RCV_DATA *m_pRcvData;                       //���鴫������
	DWORD m_dwTransEnvironment;                 //���鴫�ͻ������� 
} RCV_COMMINTERFACE_STRUCTEx;


///////////////////////////////////////////////////////////////////////////////
// �ͻ���Ӧ�ò��������󲿷�
//
typedef enum
{
	SHZS = 0,							// ��ָ֤��
    SHAG = 1,							// ��֤A��
	SHBG = 2,							// ��֤B��
	SZZS = 3,							// ����ָ��
	SZAG = 4,							// ����A��
	SZBG = 5,							// ����B��
	SHZQ = 6,							// ��֤ծȯ
	SZZQ = 7,							// ����ծȯ
	SHJIJIN = 8,						// ��֤����
	SZJIJIN = 9,						// ��֤����
	EBZS = 10,							// ����ָ��
	EBAG = 11,							// �����Ʊ
	STKTYPE = 12,						// ���ָ��
	CHOOSESTK = 20,						// ��ѡ��
	QBGP = 30							// ȫ����Ʊ
};

//---------------------------------------------------------------------------
typedef enum ClientTransmitAttributes{
	LoginAuthorization=0,            //������Ȩ 
    SystemRunAttribute,              //ϵͳ��������  
	InitStockCodeList,               //ȡ�ó�ʼ��ȫ����Ʊ��������   
    IndexRealtimeData,               //ȡ��ĳһ����ָ����ʱ��������1A0001��2A01   
	StockRealtimeData,               //ȡ��ĳһ��Ʊ��ʱ��������
	StockMinuteData,                 //ȡ��ĳһ��Ʊ�����������
    StockRealtimeDataKLine,          //ȡ��ĳһ��Ʊ��K�߽��漴ʱ��������
	StockDisPartData,                //ȡ��ĳһ��Ʊ�ֱ���ϸ����
    StockTypesBaseRealTimeData,      //ȡ��ĳһ���Ʊ������������ ĳһ���Ʊȫ����ߡ���͡���������  
    StockTypesRealTimeMarketData,    //ȡ��ĳһ���Ʊ��������   ĳһ���Ʊȫ�����¡��ɽ��������
    StockTypesRealTimeMarketWithMinute,      //ȡ��ĳһ���Ʊ��ʱ��������   ĳһ����ȫ����Ʊ��ʱ����
	HistoryDayLineData,              //���ڱ�ʾ��ʷ�������� 
	HistoryMinuteLineData,           //���ڱ�ʾ��ʷ��ʱ���� 
	HistoryDisPartBargainingData,    //���ڱ�ʾ��ʷ��ϸ����
	WideExRightData,                     //���ڱ�ʾ��Ȩ����
	WideFinancingData,                   //���ڱ�ʾ��������
	WideStockBaseInfo,                   //���ڱ�ʾ��������
    WideShInfomation,                    //���ڱ�ʾ��֤����
	WideSzInfomation,                    //���ڱ�ʾ��֤����  
    WideEbInfomation,                    //���ڱ�ʾ���幫��  
	WideZjInfomation,                    //���ڱ�ʾ�ƾ�����
	WideTechnicIndex,                    //���ڱ�ʾ����ָ�� 
	WideNotifyInfomation                 //���ڱ�ʾ֪ͨ��Ϣ
};
typedef struct tagTransmitStockAttribute
{
	WORD m_wMarket;                                  //��Ʊ�г�����
	char m_szTransmitStockLabel[8];                  //���͹�Ʊ����
	DWORD m_dwTransmitCounts;                        //��ʾ��¼��     
}TransmitStockAttribute, *PTransmitStockAttribute;   //���͹�Ʊ����ָ��
//========================================
typedef struct tagTRANS_PACKAGE_STRUCTEx
{
	ClientTransmitAttributes  m_TransmitType;    //�ͻ��������ݰ�����  
	DWORD m_dwTransmitStockType;                 //���ڱ�ʾ�������  
	union
	{
		struct
		{
			TransmitStockAttribute *m_pTransmitStockAttribute;
			DWORD m_dwTransmitStockCount;                        //��ʾ���͹�Ʊ��
		};
		DWORD m_dwTransmitStockTypeCounts;                       //��ʾ��¼��,���ڰ��
	};
} TRANS_PACKAGE_STRUCTEx;
/*�ṹ˵����m_TransmitType���ڱ�ʾ�ͻ����������������
m_dwTransmitStockType���ڱ�ʾ��鴫��
m_dwTransmitStockType== 	SHZS��ʾ��ָ֤�� 
m_dwTransmitStockType== SHAG,��ʾ��֤A�� 
m_dwTransmitStockType==SHBG,��ʾ��֤B�� 
m_dwTransmitStockType==SHZQ��ʾ��֤ծȯ
m_dwTransmitStockType==SZZS��ʾ����ָ��
m_dwTransmitStockType==SZAG,��ʾ����A��
m_dwTransmitStockType==SZBG,��ʾ����B��
m_dwTransmitStockType==SZZQ��ʾ����ծȯ
m_dwTransmitStockType==EBZS��ʾ����ָ��   
m_dwTransmitStockType==EBAG,��ʾ�����Ʊ
������ʾ��һ��Ʊ����Ʊ����
m_pTransmitStockAttributeΪ�ṹ����ָ�룬���ڱ�ʾ��һ��Ʊ����Ʊ��������˵��
����m_szTransmitStockLabelΪ��Ʊ���룬m_dwTransmitCounts��ʾ������˵�����ڲ�ͬ�������ݴ����н���ʾ��ͬ����
m_dw TransmitStockCountΪ��ʾ��Ʊ����
m_dwTransmitStockTypeCounts���ڱ�ʾ��鴫��ʱ��������˵����
*/

/* ��Ϣ�������� DEMO
LONG OnStkDataOK(UINT wParam,LONG lParam)
{
RCV_DATA	*		pHeader;
int			i;

	pHeader = (RCV_DATA *)lParam;

//	���ڴ����ٶ�������������,��ý� pHeader->m_pData ָ������ݱ���,��������

	switch( wParam )
	{
	case RCV_REPORT:						// �����������÷�ʽ,��Ʊ����
		for(i=0; i<pHeader->m_nPacketNum; i++)
		{
			pHeader->m_pReport[i] ...
			// ���ݴ���
		}
		break;

	case RCV_FILEDATA:						// �����������÷�ʽ,�ļ�
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:				// ����������
			break;

		case FILE_MINUTE_EX:				// ������������
			break;

		case FILE_BASE_EX:					// Ǯ�����ݻ��������ļ�,m_szFileName�������ļ���
			break;

		case FILE_NEWS_EX:					// ������,��������m_szFileName����Ŀ¼������
			break;
		}
		break;

	default:
		return 0;							// unknown data
	}
	return 1;
}
*/

#pragma pack()



///////////////////////////////////////////////////////////////////////////////
// APIs

#ifdef __cplusplus
extern "C"
{
#endif

///////////////////////////////////////////////////////////////////////////////
// ע�ắ��

// ��Ʊ��ʼ��
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��
//		Msg				�û��Զ�����Ϣ
//		nWorkMode		�ӿڹ�����ʽ,Ŀǰֻ��Ϊ RCV_WORK_MEMSHARE
// ���ز���:
//		 1				�ɹ�
//		-1				ʧ��
// ע:
//		ע���,����������������ڷ�����Ϣ; ����ע��,ͨ����ѯ��ʽ���ȡ������
//		����Ʊ����û����,��������Ʊ���ճ���
int WINAPI Stock_Init(HWND hWnd, UINT Msg, int nWorkMode);

// �˳�,ֹͣ������Ϣ
// ��ڲ���:
//		hWnd			������Ϣ�Ĵ��ھ��,ͬ Stock_Init �ĵ�����ڲ���
//	���ز���:
//		 1				�ɹ�
//		-1				ʧ��
int WINAPI Stock_Quit(HWND hWnd);

///////////////////////////////////////////////////////////////////////////////
// ����ӿ�

// ȡ�ѽ��յ��Ĺ�Ʊ����
int WINAPI GetTotalNumber();

// �����ȡ��Ʊ����(��չ)
// ��ڲ���:
//			nNo			���
//			pBuf		��Ź�Ʊ���ݵĻ�����
// ���ز���:
//		    NoStockData	�޹�Ʊ����
// ע:
//			�ú����ṩ��Ʊ���ݵ���Ҫ����;��������������ʱ,���Կ��ٽ������ݿ��
int WINAPI GetStockByNoEx(int nNo, RCV_REPORT_STRUCTEx* pBuf);

// �ɹɺ�ȡ��Ʊ����(��չ)
// ��ڲ���:	
//			pszStockCode��Ʊ����
//			nMarket		֤ȯ�г�
//			pBuf		��Ź�Ʊ���ݵĻ�����
// ���ز���:
//		    NoStockData	�޹�Ʊ����
// ע:
//			�ú����ṩ��Ʊ���ݵ���Ҫ����;��������������ʱ,���Կ��ٽ������ݿ��
int WINAPI GetStockByCodeEx(char* pszStockCode, int nMarket, RCV_REPORT_STRUCTEx* pBuf);

// ������ճ���,��������
// ��ڲ���:
//			bSetup		TRUE		��ʾ����,��������
// ���ز���:
//			 1			�ɹ�
//			-1			ʧ��
int	WINAPI SetupReceiver(BOOL bSetup);

//	ȡ�ù�Ʊ������Ϣ
//	��ڲ���:
//			nInfo		����
//			pBuf		������
//	���ڲ���:
//			nInfo == RI_IDSTRING,	���������ַ�������, pBuf Ϊ�����ַ���
//									��:	"TongShi_StockDrv_1.00"
//			nInfo == RI_IDCODE,		������Ϣ�� ID ��, pBuf Ϊ�ַ�����ʽ�� ID ��
//									��:	0x78001234	  "78001234"
//			nInfo == RI_VERSION,	������Ϣ���汾��, pBuf Ϊ�ַ����汾
//									��:  1.00		  "1.00"
//			nInfo == RI_ERRRATE,	ȡ�ŵ�����,
//			nInfo == RI_STKNUM,		ȡ���й�Ʊ�ܼ���
DWORD WINAPI GetStockDrvInfo(int nInfo, void* pBuf);

#ifdef __cplusplus
}
#endif



#endif // __STOCKDRV_100_H__