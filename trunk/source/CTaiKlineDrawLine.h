// CTaiKlineDrawLine.h: interface for the CTaiKlineDrawing class.
//		


#if !defined(AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_)
#define AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef CArray<INFO_LINESELF,INFO_LINESELF&> infoLineself;

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiKlineDrawing :public CObject 
{
public:

	bool IsTrendLine();
	void SetAlarmFlag(int nLine,BYTE bAlarm);
	void SetCurrentLineProp(COLORREF rgb=0, int nLineType=0, int nLineThick=0, bool bAlarm=false,int nParam = 0);
	bool GetCurrentLineProp(COLORREF & rgb,int& nLineType,int& nLineThick,bool& bAlarm);
	
	void CaclTwoPointXD(CPoint &p1, CPoint &p2);
	
	void ArcInRect(CDC* pDC,int  x1,int y1,int x2,int y2);

	bool IsCrossSideFloat(int nSide,float x1, float y1, float x2, float y2,CPoint &pOut);
	
	void CaclTwoPointFloat(float x1,float y1,float x2,float y2);
	

	bool IsCrossSide(int nSide,CPoint &p1, CPoint &p2,CPoint &pOut);
	

	void ShowText(CDC* pDC,float in,int y,int nLine = -1);

	void SetPointHuiGui(int foot,int footOld);
	
	int SetPointPos(CPoint& point);
	
	void DrawPointBE(CDC* pDC);
	void DrawXianDuan(CDC* pDC,CPoint& p1,CPoint& p2);

	void DrawXianDuanVirtul(CDC *pDC,CPoint& p1,CPoint& p2,COLORREF* clr = NULL);


	void DeleteLineAll();
	void DeleteLine();
	void AddLine();//
	int FloatToY(float val,float& out,int flag=0);
	
	int TimeToX(int time,int flag=0);
	void DrawTotal(CDC* pDC);//
	int ScreenToFile(int n,bool isToFile,bool bPrinting=false);
	void DrawText(CDC* pDC,CPoint* pp=NULL,CString * s=NULL);
	void DrawArrowUp(UINT id,CDC* pDC,int nPnt=0,int nWidth = 23);
	void DrawHuiGuiTop(CDC* pDC,int flag=0,COLORREF* clr=NULL);
	void CaclTwoPoint(CPoint& p1,CPoint& p2,int flag=0);
	
	int DrawSelfLine(CDC* pDC,bool bSetProp = false);//
	void DrawRect(CDC* pDC,CRect &r);//
	int IsOnPoint(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag);
	int IsOnLine(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag);
	int IsOnSelfLine(CPoint& point);

	int IsOnSelfLineTotal(CPoint& point);
	//
	int SetPointMoved(int cx,int cy);

	void SetDrawLineRect();

	//--- 
	int WriteLineself();//
	int ReadLineself();//


	int m_nPosStock;//
	int m_nLine;//
	int m_nShares;//
	infoLineself	m_info_lineself;//

	int m_nCurrentLineClicked;//
	int m_nLineTypeChanging;// 
	int m_nZhongXianDrawing;//
	CFile* m_fileSh;//
	CFile* m_fileSz;//

	//
	bool m_bSelected;//
	bool m_bLineValidate;//
	int m_nCurrentLine;//

	int m_bMoveZhongXian;//
	int m_nZhongXian;//
	int m_lenTextNow;//
	CString  m_textNow;//
	int m_nPntGroup[30];//
	int m_lineTypeOld;//
	int m_lineType;//
	bool m_bDrawLine;//
	int m_nPointCurrent;
	CPoint m_ptOldPingXing[2];
	CPoint m_ptOldDrawing[3];
	CPoint m_ptOld[3];
	CPoint m_pt[3];
	CRect m_rtDrawLine;
	float m_widthPer;

	CTaiShanDoc* pDoc;	
 	CTaiShanKlineShowView* pView;		

	CTaiKlineDrawing();
	CTaiKlineDrawing(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);
	virtual ~CTaiKlineDrawing();

};

#endif // !defined(AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_)
