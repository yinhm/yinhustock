// CTaiKlineDoKline.h: interface for the CTaiKlineShowKline class.
//		


#if !defined(AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
#define AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_

#include "LBsInfoRS.h"	
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define Total_Count_Line						64

#include "StructKlineView.h"
#define BUY_CASE 107
#define SELL_CASE 108

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CFormularCompute;
class CTaiKlineDialogShiDuanTJ ;
class CTaiKlineMoveChenben;
class CTaiKlineTransferKline;

class CTaiKlineShowKline : public CObject
{
	DECLARE_DYNCREATE(CTaiKlineShowKline)
public:
	void GetRect(CRect &r,int nFiguer);
	int m_nValidIndicator[5];
	void SetRectCurrent(int nFiguer);
	void DrawIndicatorAdded(CDC* pDC);
	CString m_sIndicatorAdded[5];
	int m_nLineIndicator1[5];

	CString m_sOldSymbol;
	int m_sOldStkKind;

	int m_nKlineType;
	int m_numOldKline;
	bool ValidDoKline(bool bMakeUp = false);
	CTaiKlineTransferKline* m_pLmbTransferKline;

	int m_nBeginFootTJ;
	int m_nEndFootTJ;//
	bool m_bInvertRect;

	bool m_bSetPageData;

	CString m_zhiBiao0;
	int m_highFoot;//
	int m_lowFoot;//
	int m_bDaPan[4];//
	bool m_bInitFoot;//
	CTaiKlineMoveChenben* m_pMoveCB;//

	int m_kLine_mode ;//
	bool m_bNewStock;//

	bool m_bDrived;//
	Kline* m_pKlineAdd[6];
	int m_nKlineAdd[6];
	int m_nKlineFileAdd[6];
	CString m_nameAdd[6];
	CString m_symbolAdd[6];


	int m_stkKindAdd[6];

	float m_amountAdd[6];//
	float m_volAdd[6];//
	CString m_lineName[5][Total_Count_Line];//
	int m_nKlineCurrent;//
	int m_nTextBox;
	int m_axisType;//
	float m_currentValue;//
	int m_footCurrent;//
	CPoint pt[6];//
	CString m_tipStr;//
	CTaiShanDoc* pDoc;		
	CTaiShanKlineShowView* pView;	
	int	m_lineBgn;	

	int m_step[5];				

	bool	  m_bToCacl;	
	int	  m_klinNumDefault;	

	int   m_footBegin;		
	int   m_footEnd;		
	int	  m_klinNumDefaultOld;
	int   m_footBeginOld;		
	int   m_footEndOld;			
	CRect m_rectDrawLine;	
	ARRAY_JISHU		m_dataFormular[5];	
	powerArr m_PowerArray;//

	bool DrawBTX(CDC* pDC);

	void DrawKlineHS(CDC* pDC);

	void MoveDragMode(CPoint & pPre,CPoint& pNext);


	void OutBitmap(CDC *pDC,int id,CPoint & p);


	bool DrawRulorTextSelf(CDC * pDC,int wid);


	bool ShowFenshiRulor(CDC *pDC,int wid);

	void ShowVirtualLine(CDC* pDC,CPoint& p1,CPoint& p2);


	void ShowTjxgShow(CDC* pDC);
	void DrawPower(CDC* pDC);//
	void DrawKLineAmericaAdded(CDC *pDC,int nKln);//

	void WritePower(CString &symbol,int stkKind);//
	//
	int ReadPower(CString& symbol,int stkKind);//

	CTaiKlineShowKline();

	void DrawKLineAddedPage(CDC *pDC);//
	void CaclMaxMin(Kline* pKline,int b,int e);

	int CaclEndFoot(int nKline);

	void CaclMaxAdded();//
	int CaclBeginFoot(int nKline);

	int FindTimeK(int keyVlu,Kline* pKline,int max);


	void DeleteKlineAdded();
	void AddKlineAdded(CString symbol,int stkKind);

	void CaclPercentMaxMin(float& maxPct,float& minPct);
	void DrawBaseInfo(CDC* pDC);
	void RestoreKLine();//
	void ComputePower(CString symbol,int stkKind,Kline* pKline,int nKln);//

	int TransferKlineTypeDataToday(Kline*& pKline,CString symbol,int stkKind,int nFlags);//转换今天的数据

	int TransferKlineTypeData(CString symbol,int stkKind,Kline*& pKline,int nFlags=-1,int nKln=-1);//把日线等转换为其他类型

	float TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags=0);//

	float ShowChengBen(CDC *pDC,int nFlags=0);//
	void ShowCNP(CDC* pDC,int nFlags=0);
	void ShowMultiPage(CDC* pDC);
	void ShowMultiFiguer(CDC* pDC,int flag=0);//
	void ShowLittleRulor(CDC* pDC,int wid=0);//
	void DrawRulorX(CDC *pDC);//
	int DrawRulorTextLog(CDC *pDC,BYTE wid=0);//
	int OnHome();//
	int OnEnd();//
	int OnUp();//
	int OnDown();//
	int OnLeft();//
	int OnRight();//
	int TransferX(int x);
	int OutString(int num, CPoint p);

	int MoveDisplays(int num);

	int SetShowTotal(int num);//
	CString TimeToStr(int foot);//
	void DrawCaption(CDC* pDC);//
	void DrawCaptionAdded(CDC *pDC);//
	void CalcSonFiguer(CString name);//
	void DrawSonFiguer(CDC* pDC);	//
	CTaiKlineShowKline(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);

	void DrawRulorText(CDC* pDC,BYTE wid=0);	
	void DrawKlineFiguer(CDC* pDC);
	void DrawKLine(CDC *pDC);		//
	void DrawKLineAdded(CDC *pDC,int nKln);		//
	void DrawKLineAmeric(CDC *pDC);//
	void InitDrawedLineBE();		//
	int InitLineParam();	//
	void CaclMaxFlt(Kline *pK,int footBegin);	



	BOOL m_bClosed;				// 是否停盘

	int m_nSon;					// 当前绘图的区域    0 主图

	float m_min_sun[5];
	float m_max_sun[5];

	void CaclMaxFlt(float* pFlt, int footBegin, int nLine = -1);

	// 画指标线
	void DrawLineIndex(CDC* pDC, bool bFenshi = false);



	float YTransfer(int y);		
	int YTransfer(float y);		//

	void CaclRulorMovable(CPoint point);	

	//-

	virtual ~CTaiKlineShowKline();

private:
	void DrawTextIconLine(CDC *pDC,int i);
	CString GetParamString(CString sName);
	void DrawBTXAdded(CDC *pDC, int nKln);
};
#ifndef _DEBUG 
inline	float CTaiKlineShowKline::YTransfer(int y)		
{
	float rtn;
	if((m_rectDrawLine.top-m_rectDrawLine.bottom)==0)
		return 0;

	if(!(m_nSon==0&&m_axisType==2))
		rtn=((float)(m_max_sun[m_nSon]-m_min_sun[m_nSon]))*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)+m_min_sun[m_nSon];
	else
	{
		rtn=(float)(pow(10,(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))
			*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)
			+log10(m_min_sun[m_nSon])));
	}

	return rtn;
}
inline	int CTaiKlineShowKline::YTransfer(float y)	
{
	int rtn;
	if((m_max_sun[m_nSon]-m_min_sun[m_nSon])==0)
	{
		rtn = -10;
		return rtn;
	}

	if(!(m_nSon==0&&m_axisType==2))
		rtn=(int)((float)(m_rectDrawLine.top-m_rectDrawLine.bottom)*(y-m_min_sun[m_nSon])/(m_max_sun[m_nSon]-m_min_sun[m_nSon])+m_rectDrawLine.bottom+0.5);
	else
	{
		if(m_min_sun[m_nSon]<=0 || m_max_sun[m_nSon]<=0 || y<=0)
			return 0;

		rtn=(int)((m_rectDrawLine.top-m_rectDrawLine.bottom)*(log10(y)-log10(m_min_sun[m_nSon]))/(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))+m_rectDrawLine.bottom+0.5);
	}
	return rtn;
}
#endif
#endif // !defined(AFX_LDOKLINE_H__47E83166_C70C_11D2_91D8_0080C8E1242B__INCLUDED_)
