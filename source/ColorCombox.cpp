// Tel:13366898744
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorCombox.h"
#include "GMemDC.h"
#include "ListTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


GColorComboBox::GColorComboBox()//色彩
{
   m_cl = RGB(0,0,0);
}

GColorComboBox::~GColorComboBox()
{
}

void GColorComboBox::SetColor(COLORREF clInit)
{
   m_cl = clInit;
}

COLORREF GColorComboBox::GetColor() const
{
   return m_cl;
}

void GColorComboBox::UpdateColor(COLORREF clUpdate)
{
   if(clUpdate != m_cl)
   {
      m_cl = clUpdate;
      Invalidate();
      int nChildID = GetDlgCtrlID();
      WPARAM wParam = MAKEWPARAM((WORD)nChildID, CCN_SELCHANGE);
      GetParent()->SendMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
   }
}


BEGIN_MESSAGE_MAP(GColorComboBox, CWnd)//大图象列表
   ON_WM_LBUTTONDOWN()
   ON_WM_PAINT()
   ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL GColorComboBox::Create(DWORD dwStyle, const RECT& rect,
                            CWnd *pParentWnd, UINT nID)
{
   return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID);
}

void GColorComboBox::OnLButtonDown(UINT nFlags, CPoint pt)
{
   CRect cr;
   GetClientRect(cr);
   cr.right -= CCBDIM_CXEDGE;
   cr.left = cr.right - CCBDIM_CXARROW -
             CCBDIM_CXBORDER - CCBDIM_CXSPACERIGHT;
   cr.top += CCBDIM_CYEDGE;
   cr.bottom -= CCBDIM_CYEDGE;// standard constructor

   if(cr.PtInRect(pt))
   {
      CRect wr;
      GetWindowRect(wr);

      WNDCLASS wndcls;
      memset(&wndcls, 0, sizeof(wndcls));

      wndcls.lpszClassName = "GPopupColorCtrl";
      wndcls.lpfnWndProc = AfxWndProc;
      wndcls.hInstance = AfxGetInstanceHandle();
      
      if(AfxRegisterClass(&wndcls))
      {
         GPopupColorCtrl *pPopup = new GPopupColorCtrl(m_cl, this);
         pPopup->CreateEx(0, wndcls.lpszClassName, NULL, WS_BORDER|WS_POPUP, 
                            wr.left, wr.bottom+2, 0, 0, AfxGetMainWnd()->m_hWnd, 0, NULL);
      }
   }
}

void GColorComboBox::OnPaint()//系统配色
{
   CPaintDC dc(this);
   GMemDC memDC;
   CBrush brush(GetSysColor(COLOR_BTNFACE));
   CRect cr;
   GetClientRect(cr);// standard constructor
   if(memDC.Create(&dc, cr, &brush))
   {
      CDC *pDC = &memDC;
      CRect rectDraw(cr);
      rectDraw.DeflateRect(1,1);
      CPen penBlack(PS_SOLID, 1, RGB(0,0,0));
      CPen penDarkGray(PS_SOLID, 1, GetSysColor(COLOR_BTNSHADOW));
      CPen penWhite(PS_SOLID, 1, RGB(255,255,255));
      CPen penGray(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));

     
      CPen *pOldPen = pDC->SelectObject(&penGray);

      pDC->MoveTo(rectDraw.left, rectDraw.top);
      pDC->LineTo(rectDraw.right, rectDraw.top);
      pDC->MoveTo(rectDraw.left, rectDraw.top);
      pDC->LineTo(rectDraw.left, rectDraw.bottom);

      pDC->SelectObject(&penWhite);

      rectDraw.DeflateRect(1, 1);
      pDC->MoveTo(rectDraw.left, rectDraw.top);
      pDC->LineTo(rectDraw.right, rectDraw.top);
      pDC->MoveTo(rectDraw.left, rectDraw.top);
      pDC->LineTo(rectDraw.left, rectDraw.bottom);

      pDC->SelectObject(&penBlack);// NOTE: the ClassWizard will add member functions here

      rectDraw.InflateRect(1, 1);
      pDC->MoveTo(rectDraw.right, rectDraw.top);
      pDC->LineTo(rectDraw.right, rectDraw.bottom+1);
      pDC->MoveTo(rectDraw.left, rectDraw.bottom);
      pDC->LineTo(rectDraw.right, rectDraw.bottom);

      pDC->SelectObject(&penDarkGray);

      rectDraw.DeflateRect(1, 1);
      pDC->MoveTo(rectDraw.right, rectDraw.top);
      pDC->LineTo(rectDraw.right, rectDraw.bottom+1);
      pDC->MoveTo(rectDraw.left, rectDraw.bottom);
      pDC->LineTo(rectDraw.right, rectDraw.bottom);

   
      pDC->SelectObject(&penBlack);// serious allocation failure checking

      int nEnd = rectDraw.right - CCBDIM_CXEDGE - 
                 CCBDIM_CXBORDER;
      int nStart = nEnd - CCBDIM_CXARROW;
      int nMiddle = cr.top + (cr.Height()/2);

      pDC->MoveTo(nStart, nMiddle-1);
      pDC->LineTo(nEnd, nMiddle-1);

      nStart++;
      nEnd--;
      pDC->MoveTo(nStart, nMiddle);
      pDC->LineTo(nEnd, nMiddle);

      nStart++;
      nEnd--;
      pDC->MoveTo(nStart, nMiddle+1);
      pDC->LineTo(nEnd, nMiddle+1);

     
      nEnd = rectDraw.right - CCBDIM_CXEDGE - 
                 CCBDIM_CXBORDER - CCBDIM_CXARROW -
                 CCBDIM_CXSPACERIGHT - 1;

      pDC->SelectObject(&penWhite);

      int nTop = cr.top + CCBDIM_CYEDGE + CCBDIM_CYBORDER;
      int nBottom = (cr.bottom - CCBDIM_CYEDGE) - CCBDIM_CYBORDER;
      pDC->MoveTo(nEnd, nTop);
      pDC->LineTo(nEnd, nBottom);

      pDC->SelectObject(&penDarkGray);

      nEnd--;
      pDC->MoveTo(nEnd, nTop);
      pDC->LineTo(nEnd, nBottom);

    
      nEnd -= CCBDIM_CXSPACELEFT;

      CRect rectColor(cr);
      rectColor.DeflateRect(1,1);

      rectColor.right = nEnd;
      rectColor.left += cr.left + CCBDIM_CXEDGE + CCBDIM_CXBORDER;
      rectColor.top = nTop;
      rectColor.bottom = nBottom;

      pDC->SelectObject(&penBlack);

      CBrush brush(m_cl);
      CBrush *pOldBrush = pDC->SelectObject(&brush);

      pDC->Rectangle(rectColor);
  
      pDC->SelectObject(pOldPen);
      pDC->SelectObject(pOldBrush);

      memDC.Copy(&dc, cr);
      memDC.Release();
   }
}

BOOL GColorComboBox::OnEraseBkgnd(CDC *pDC)//系统配色
{
   return TRUE;
}



COLORREF g_clArray[] = {RGB(0,0,255), RGB(0,255,0), 
                        RGB(0,255,255), RGB(255,0,0), 
                        RGB(255,0,255), RGB(255,255,0), 
                        RGB(255,255,255), RGB(0,0,0), 
                        RGB(0,0,127), RGB(0,127,0), 
                        RGB(0,127,127), RGB(127,0,0), 
                        RGB(127,0,127), RGB(127,127,0), 
                        RGB(127,127,127), RGB(63,63,63)}; 



GPopupColorCtrl::GPopupColorCtrl(COLORREF clInit,
                                 GColorComboBox *pBuddyCombo)
{
   m_pBuddyCombo = pBuddyCombo;
   m_nActiveSquare = 0;                           
   m_nCurHoverSquare = 0;

   for(int nColor = 0; nColor < CCDIM_TOTALINDECES; nColor++)
   {
      if(g_clArray[nColor] == clInit)
      {
         m_nActiveSquare = nColor;
         m_nCurHoverSquare = nColor;
         break;
      }
   }

   m_clSelected = g_clArray[m_nActiveSquare];
}


GPopupColorCtrl::~GPopupColorCtrl()
{

}


int GPopupColorCtrl::CursorToColorIndex(CPoint ptTest)
{// serious allocation failure checking
   CPoint pt(CCDIM_CXSPACE, CCDIM_CYSPACE);
   int nIndex = 0;

   for(int nRow = 0; nRow < 4; nRow++)
   {
      for(int nCol = 0; nCol < 4; nCol++)
      {
         CRect rectSquare(pt, CSize(CCDIM_SQUAREWIDTH, CCDIM_SQUAREHEIGHT));

         if(rectSquare.PtInRect(ptTest))
            return nIndex;

         pt.x += (CCDIM_SQUAREWIDTH + CCDIM_CXSPACE);
         nIndex++;
      }

      pt.x = CCDIM_CXSPACE;
      pt.y += (CCDIM_SQUAREHEIGHT + CCDIM_CYSPACE);
   }

   return -1;
}


void GPopupColorCtrl::DrawColorSquare(CDC *pDC, int nIndex, CPoint pt)
{// serious allocation failure checking
   CRect rectDraw(pt, CSize(CCDIM_SQUAREWIDTH, CCDIM_SQUAREHEIGHT));
   
   _3DPENS pens;

   int nThickness = GfxDraw3DBorder(pDC, &pens, rectDraw);

   rectDraw.DeflateRect(nThickness, nThickness);
   CBrush brush(g_clArray[nIndex]);
   pDC->FillRect(rectDraw, &brush);

   if(nIndex == m_nCurHoverSquare)
   {
      rectDraw.InflateRect(1, 1);
      GfxDrawBorder(pDC, rectDraw, pens.pPenWhite);

      rectDraw.DeflateRect(1,1);
      GfxDrawBorder(pDC, rectDraw, pens.pPenBlack);
   }
}



BEGIN_MESSAGE_MAP(GPopupColorCtrl, GPopupWindow)
   ON_WM_CREATE()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONUP()
   ON_WM_PAINT()
   ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



int GPopupColorCtrl::OnCreate(LPCREATESTRUCT lpcs)
{// NOTE: the ClassWizard will add member functions here
   int nCreate = GPopupWindow::OnCreate(lpcs);

   if(nCreate == 0)
   {
      int cx = CCDIM_CXSPACE * 5;
      cx += GetSystemMetrics(SM_CXBORDER) * 2;
      cx += (CCDIM_SQUAREWIDTH * 4);

      int cy = CCDIM_CYSPACE * 5;
      cy += GetSystemMetrics(SM_CYBORDER) * 2;
      cy += (CCDIM_SQUAREHEIGHT * 4);

      SetWindowPos(&wndTopMost, 0, 0, cx, cy, 
             SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW|SWP_NOACTIVATE);
   }

   return nCreate;
}


void GPopupColorCtrl::OnPaint()
{
   CPaintDC dc(this);
   
   GMemDC memDC;
   CBrush brush(GetSysColor(COLOR_BTNFACE));

   CRect cr;
   GetClientRect(cr);

   if(memDC.Create(&dc, cr, &brush))
   {
      CPoint pt(CCDIM_CXSPACE, CCDIM_CYSPACE);
      int nIndex = 0;

      for(int nRow = 0; nRow < 4; nRow++)
      {
         for(int nCol = 0; nCol < 4; nCol++)
         {
            DrawColorSquare(&memDC, nIndex, pt);
            pt.x += (CCDIM_SQUAREWIDTH + CCDIM_CXSPACE);
            nIndex++;
         }

         pt.x = CCDIM_CXSPACE;
         pt.y += (CCDIM_SQUAREHEIGHT + CCDIM_CYSPACE);
      }

      memDC.Copy(&dc, cr);
      memDC.Release();
   }
}


BOOL GPopupColorCtrl::OnEraseBkgnd(CDC *pDC)
{
   return TRUE;
}


void GPopupColorCtrl::OnMouseMove(UINT nFlags, CPoint pt)
{// NOTE: the ClassWizard will add member functions here
   int nIndex = CursorToColorIndex(pt);

   if(nIndex != -1 && nIndex != m_nCurHoverSquare)
   {
      m_nCurHoverSquare = nIndex;
      Invalidate();
   }
}


void GPopupColorCtrl::OnLButtonUp(UINT nFlags, CPoint pt)
{// NOTE: the ClassWizard will add member functions here
   if(!OnLButtonDownCheck(nFlags, pt))
   {
      int nIndex = CursorToColorIndex(pt);
    
      if(nIndex != -1)
      {
         if(m_pBuddyCombo)
            m_pBuddyCombo->UpdateColor(g_clArray[nIndex]); 

         m_nActiveSquare = nIndex;
         ::ReleaseCapture();
      }
   }
}



