// PopWnd.cpp
//this class only support a popup window frame
/////////////////////////////////////////////////////////////////////////////////
//if there have any bugs, pls inform me
//tel:13366898744
//////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PopWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GPopupWindow::GPopupWindow()
{
}

GPopupWindow::~GPopupWindow()
{
}

void GPopupWindow::PostNcDestroy()
{
   delete this;
}

BOOL GPopupWindow::OnLButtonDownCheck(UINT nFlags, CPoint pt)
{
   CRect cr;
   GetClientRect(cr);

   if(!cr.PtInRect(pt))
      return TRUE;

   return FALSE;
}

BEGIN_MESSAGE_MAP(GPopupWindow, CWnd)
   ON_WM_CREATE()
   ON_WM_DESTROY()
   ON_WM_CAPTURECHANGED()
   ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void GPopupWindow::OnCaptureChanged(CWnd *pWnd)
{
   PostMessage(WM_CLOSE);
}

int GPopupWindow::OnCreate(LPCREATESTRUCT lpcs)
{
   int nCreate = CWnd::OnCreate(lpcs);

   SetCapture();
   return nCreate;
}

void GPopupWindow::OnLButtonDown(UINT nFlags, CPoint pt)
{
   if(OnLButtonDownCheck(nFlags, pt))
   {
      ::ReleaseCapture();
   }
}