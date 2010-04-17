// CTaiKlineDoFenshi.h: interface for the CTaiKlineMin1 class.


#if !defined(AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_)
#define AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int  FS_ZOUSHI=		0;
const int  FS_VOL	=		1;
const int  FS_MMLD	=	2;
const int  FS_LB	=		3;
const int  FS_LINXIAN	=	4;
const int  FS_DUOKONG	=	5;
const int  FS_TONGLUO	=	6;
#include "CTaiShanDoc.h"

#include "CTaiKlineDoKline.h"

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiKlineFileHS;
class CTaiKlineDialogShiDuanTJ ;
class CTaiKlineMin1  :public	CTaiKlineShowKline
{
	DECLARE_DYNCREATE(CTaiKlineMin1)
public:
	int TransferX(int x);
	static void GetFenJia(int first,CBuySellList &buySellList,CFJList&	fenjiaUp,CFJList&	fenjiaDown,int nFlag = 0);
	void SetRectDraw(int nFigu);
	CReportData m_dt;	
	CBuySellList m_hsMin1 ;
	TRADE_DETAIL_H_PER	m_hsMin[240];
	bool 	m_bHist;

	Tidxd **m_Tidx;
	Rsdn1 **m_Nidx;
	CTaiKlineFileHS*	m_pFileHs;
	int m_nCnp;
	float m_amount[240];
	float m_vol[240];
	float m_volPre;
	int  num_sun;
	float m_max_dapan[2][2];
	float m_min_dapan[2][2];
	float* m_lineDapan[2][3];
	float m_close_Dapan[2];
	float m_close;//
	int m_pFlg[5];//
	int		m_isDapan;
	CReportData* m_pReportData;	//
	CReportData* m_pS0;	//
	CReportData* m_pS1;	//
	CString m_nameSon[5];//
	bool m_bInvertRect;		


	void ShowCNP(CDC *pDC,int nFlags=0);

	void DrawDapanOther(CDC* pDC);
	

	void CaclMaxAdded();


	void AddFenshiAdded(CString symbol, int nStock,int stkKind);


	void ShowMultiFiguer(CDC *pDC,int flag=0);
	void DrawRulorX(CDC *pDC);//
	int OutString(int num, CPoint p);

	int OnHome();
	int OnEnd();//
	int OnRight();//
	int OnLeft();//
	CString TimeToString(int foot,bool bRight= false);


	void DrawLittleFrame(CDC* pDC,int nMarket=0);
	void DrawLittleDapan(CDC* pDC,int flagLine,int nMarket=0);
	void DrawLittleGegu(CDC *pDC);
	void DrawDapan(CDC *pDC,int nMarket=0,int flagFrame=0);
	int DrawHs(CDC *pDC,int nBegin, BYTE flag);

	
	void InitHs(bool bRemoveAll=true,bool bSkip = true);

	void DrawCapt(CDC* pDC);
	void DrawSon(CDC* pDC);
	void InitMinuteLine();
	

	void DrawMin1Figuer(CDC* pDC);
	CTaiKlineMin1();
	CTaiKlineMin1(CTaiShanDoc* pDocI,CTaiShanKlineShowView* pViewI	);

	float TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags=0);
	

	virtual ~CTaiKlineMin1();

};

#endif // !defined(AFX_LDOFENSHI_H__20183F81_BDC6_11D2_91D9_0080C8E1242B__INCLUDED_)
