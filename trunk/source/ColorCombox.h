

#include "PopWnd.h"


#define CCN_SELCHANGE  1
#define CCBDIM_CXEDGE    2
#define CCBDIM_CYEDGE    2
#define CCBDIM_CYBORDER  3
#define CCBDIM_CXBORDER  2
#define CCBDIM_CXSPACELEFT  4
#define CCBDIM_CXSPACERIGHT 1
#define CCBDIM_CXCOLORRECT  32
#define CCBDIM_CXDIVIDER     2
#define CCBDIM_CXARROW       5

class GColorComboBox : public CWnd
{
   DECLARE_MESSAGE_MAP()

public:
   GColorComboBox();

   BOOL Create(DWORD dwStyle, const RECT& rect,
               CWnd *pParentWnd, UINT nID);

   void SetColor(COLORREF clInit);
   COLORREF GetColor() const;
 
   virtual void UpdateColor(COLORREF clUpdate);

protected:  

   afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
   afx_msg int OnCreate(LPCREATESTRUCT lpcs);
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnPaint();

   COLORREF m_cl;

public:
   virtual ~GColorComboBox();
};

#define CCDIM_SQUAREWIDTH   16
#define CCDIM_SQUAREHEIGHT  16
#define CCDIM_CYSPACE       3
#define CCDIM_CXSPACE       3
#define CCDIM_TOTALINDECES  16


class GPopupColorCtrl : public GPopupWindow
{
   DECLARE_MESSAGE_MAP();
   
public:
   GPopupColorCtrl(COLORREF clInit, 
                   GColorComboBox *pBuddyCombo);
   
   COLORREF GetSelectedColor();

protected:

   afx_msg void OnPaint();
   afx_msg BOOL OnEraseBkgnd(CDC *pDC);
   afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
   afx_msg int OnCreate(LPCREATESTRUCT lpcs);
 
   void DrawColorSquare(CDC *pDC, int nIndex, CPoint pt);
  
   int CursorToColorIndex(CPoint ptTest);

   COLORREF m_clSelected;
   int m_nActiveSquare; 
   int m_nCurHoverSquare;
 
   GColorComboBox *m_pBuddyCombo;

public:
   virtual ~GPopupColorCtrl();
};



