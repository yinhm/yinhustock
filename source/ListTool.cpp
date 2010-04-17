// ListTool.cpp : implementation file
//
/////////////////////////////////////////////////////////////
// designed 
// 1999.4.10
// if there are any bugs , pls inform me
// Tel:13366898744
//////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "ListTool.h"
 
/////////////////////////////////////////////////////////////////////////////
// CListTool

CListTool::CListTool()
{

}
 
/////////////////////////////////////////////////////////////////////////////

void CListTool::Draw_Button(CDC *pDc,int left_x,int top_y,int right_x,
	      int bottom_y,int linew,COLORREF Color,int Flag)
    {    
    CBrush brush(Color);
	CBrush *OldBrush;
	OldBrush=pDc->SelectObject(&brush);
    RECT rect;
	rect.left=left_x;
	rect.top=top_y;
	rect.right=right_x;
	rect.bottom=bottom_y;
    pDc->FillRect(&rect,&brush);
  
    if(Flag==0) pDc->SelectStockObject(WHITE_PEN); 
    else pDc->SelectStockObject(BLACK_PEN); 
    pDc->MoveTo(left_x+linew,bottom_y-linew);
	pDc->LineTo(right_x-linew,bottom_y-linew);
    pDc->MoveTo(right_x-linew,bottom_y-linew);
	pDc->LineTo(right_x-linew,top_y+linew);
    if(Flag==0) pDc->SelectStockObject(BLACK_PEN); 
    else pDc->SelectStockObject(WHITE_PEN); 
	pDc->MoveTo(left_x+linew,top_y+linew);
	pDc->LineTo(right_x-linew,top_y+linew);
    pDc->MoveTo(left_x+linew,bottom_y-linew);
	pDc->LineTo(left_x+linew,top_y+linew);
	OldBrush=pDc->SelectObject(OldBrush);
}


void CListTool::Show3DText(CDC * pDC, POINT pt, CString str, int dx, COLORREF Color,int flag)
{
     CFont MyFont;
	 int nBackMode;
     nBackMode=pDC->GetBkMode();
     if(nBackMode==OPAQUE) pDC->SetBkMode(TRANSPARENT);
	 MyFont.CreateFont(dx,0,0,0,400,FALSE,FALSE,0,
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_SWISS,
		"Arial");
	 CFont *OldFont;
	 pDC->SetBkColor(RGB(192,192,192));
	 OldFont=pDC->SelectObject(&MyFont);
     if(flag==1){
	    pDC->SetTextColor(RGB(0,0,0));	
	    pDC->TextOut(pt.x,pt.y,str);
	 }
     pDC->SetTextColor(Color);	
	 pDC->TextOut(pt.x-1,pt.y+1,str);
     pDC->SelectObject(OldFont);
	 if(nBackMode==OPAQUE) pDC->SetBkMode(OPAQUE);
}

void CListTool::Draw_Rectangle(CDC * pDc, LPCRECT lpRect, POINT pt, COLORREF Color)
{
    CBrush brush(Color);
	CBrush *oldbush=pDc->SelectObject(&brush);
    CPen pen(PS_SOLID,1,RGB(0,0,0));
	CPen *oldpen=pDc->SelectObject(&pen);
    pDc->RoundRect(lpRect,pt);
    pDc->SelectObject(oldbush);
	pDc->SelectObject(oldpen);
}

void CListTool::DrawPolyLine(CDC * pDc, POINT * pt, int count, int nDrawMode, COLORREF Linecolor)
{
    CPen pen(PS_SOLID,1,Linecolor);
	CPen *oldpen;
	oldpen=pDc->SelectObject(&pen);
	int oldMode=pDc->SetROP2(nDrawMode);
	pDc->Polyline(pt,count);
	pDc->SetROP2(nDrawMode);
	pDc->SelectObject(oldpen);
}


void CListTool::ShowText(CDC *pDC, CString &str, LPRECT lpRect,UINT nFormat )
{
	int nBackMode=pDC->GetBkMode();
    if(nBackMode==OPAQUE) pDC->SetBkMode(TRANSPARENT);
	lpRect->top+=4;
	lpRect->bottom-=4;
	pDC->DrawText(str,lpRect,nFormat);
	if(nBackMode==OPAQUE) pDC->SetBkMode(OPAQUE);
}

void CListTool::Draw_Button(CDC *pDC, CRect rect)
{
     pDC->DrawEdge(&rect, EDGE_ETCHED, BF_RECT | BF_MIDDLE);
}

void CListTool::Draw_Line(CDC *pDC, int x1, int y1, int x2, int y2)
{
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);
}

tag3DPens::tag3DPens()
{
   pPenBlack = new CPen(PS_SOLID, 1, RGB(0,0,0));
   pPenDarkGray = new CPen(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
   pPenWhite = new CPen(PS_SOLID, 1, RGB(255,255,255));
   pPenGray = new CPen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
}


tag3DPens::~tag3DPens()
{
   delete pPenBlack;
   delete pPenDarkGray;
   delete pPenWhite;
   delete pPenGray;
}

int GfxDraw3DBorder(CDC *pDC, LP3DPENS p3DPens, CRect rectBorder, BOOL bInside)
{
   if(bInside)
   {
      CPen *pOldPen = pDC->SelectObject(p3DPens->pPenDarkGray);

      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.right, rectBorder.top);
      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.left, rectBorder.bottom);

      pDC->SelectObject(p3DPens->pPenBlack);

      rectBorder.DeflateRect(1, 1);
      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.right, rectBorder.top);
      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.left, rectBorder.bottom);

      pDC->SelectObject(p3DPens->pPenWhite);

      rectBorder.InflateRect(1, 1);
      pDC->MoveTo(rectBorder.right-1, rectBorder.top);
      pDC->LineTo(rectBorder.right-1, rectBorder.bottom);
      pDC->MoveTo(rectBorder.left, rectBorder.bottom-1);
      pDC->LineTo(rectBorder.right, rectBorder.bottom-1);

      pDC->SelectObject(p3DPens->pPenGray);

      rectBorder.DeflateRect(1, 1);
      pDC->MoveTo(rectBorder.right-1, rectBorder.top);
      pDC->LineTo(rectBorder.right-1, rectBorder.bottom);
      pDC->MoveTo(rectBorder.left, rectBorder.bottom-1);
      pDC->LineTo(rectBorder.right, rectBorder.bottom-1);

      pDC->SelectObject(pOldPen);
   }
   else
   {
      
      CPen *pOldPen = pDC->SelectObject(p3DPens->pPenGray);

      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.right, rectBorder.top);
      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.left, rectBorder.bottom);

      pDC->SelectObject(p3DPens->pPenWhite);

      rectBorder.DeflateRect(1, 1);
      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.right, rectBorder.top);
      pDC->MoveTo(rectBorder.left, rectBorder.top);
      pDC->LineTo(rectBorder.left, rectBorder.bottom);

      pDC->SelectObject(p3DPens->pPenBlack);

      rectBorder.InflateRect(1, 1);
      pDC->MoveTo(rectBorder.right, rectBorder.top);
      pDC->LineTo(rectBorder.right, rectBorder.bottom+1);
      pDC->MoveTo(rectBorder.left, rectBorder.bottom);
      pDC->LineTo(rectBorder.right, rectBorder.bottom);

      pDC->SelectObject(p3DPens->pPenDarkGray);

      rectBorder.DeflateRect(1, 1);
      pDC->MoveTo(rectBorder.right, rectBorder.top);
      pDC->LineTo(rectBorder.right, rectBorder.bottom+1);
      pDC->MoveTo(rectBorder.left, rectBorder.bottom);
      pDC->LineTo(rectBorder.right, rectBorder.bottom);

      pDC->SelectObject(pOldPen);
   }

   return 2;
}

void GfxDrawBorder(CDC *pDC, CRect rectBorder, 
                   CPen *pPen)
{
   CPen *pOldPen = NULL;
   if(pPen)
      pOldPen = pDC->SelectObject(pPen);

   pDC->MoveTo(rectBorder.left, rectBorder.top);
   pDC->LineTo(rectBorder.right-1, rectBorder.top);
   pDC->LineTo(rectBorder.right-1, rectBorder.bottom-1);
   pDC->LineTo(rectBorder.left, rectBorder.bottom-1);
   pDC->LineTo(rectBorder.left, rectBorder.top);

   if(pOldPen)
      pDC->SelectObject(pOldPen);
}

void GfxDrawBorder(CDC *pDC, CRect rectBorder, 
                   COLORREF clFill, CPen *pPen)
{
   CPen *pOldPen = NULL;
   if(pPen)
      pOldPen = pDC->SelectObject(pPen);

   pDC->MoveTo(rectBorder.left, rectBorder.top);
   pDC->LineTo(rectBorder.right-1, rectBorder.top);
   pDC->LineTo(rectBorder.right-1, rectBorder.bottom-1);
   pDC->LineTo(rectBorder.left, rectBorder.bottom-1);
   pDC->LineTo(rectBorder.left, rectBorder.top);

   if(pOldPen)
      pDC->SelectObject(pOldPen);

   CBrush brush(clFill);
   rectBorder.DeflateRect(1,1);

   pDC->FillRect(rectBorder, &brush);
}