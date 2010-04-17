#include "stdafx.h"
#include "BtnST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonST

CButtonST::CButtonST()
{//子窗口框架
	m_MouseOnButton = FALSE;

	m_hIconIn = NULL;
	m_hIconOut = NULL;
	m_cxIcon = 0;
	m_cyIcon = 0;
	m_hCursor = NULL;
  
	
	m_bIsFlat = FALSE; 
  

	m_bDrawBorder = TRUE; 
  

	m_nAlign = ST_ALIGN_HORIZ; 
  

	m_bShowText = TRUE; 
  

	m_bDrawFlatFocus = FALSE;


	m_bIsDefault = FALSE;

	SetDefaultInactiveBgColor();
	SetDefaultInactiveFgColor();
	SetDefaultActiveBgColor();
	SetDefaultActiveFgColor();

	m_crActiveFg = RGB(255,0,0);
	m_crActiveBg = RGB(210,210,210);


	m_ToolTip.m_hWnd = NULL;

	
	m_bDrawTransparent = FALSE;
	m_pbmpOldBk = NULL;

	SetBtnCursor(IDC_HAND_1);
	m_bIsFlat = FALSE;
} 


CButtonST::~CButtonST()
{//子窗口框架

	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL) ::DestroyIcon(m_hIconOut);

	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
} 


BEGIN_MESSAGE_MAP(CButtonST, CButton)
    //{{AFX_MSG_MAP(CButtonST)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CButtonST::SetIcon(int nIconInId, int nIconOutId)
{
	return;
	HICON hIconIn;
	HICON hIconOut;
	HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIconInId), RT_GROUP_ICON);

	
	hIconIn = (HICON)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nIconInId), IMAGE_ICON, 0, 0, 0);
  
	hIconOut = (nIconOutId == NULL) ? NULL : (HICON)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nIconOutId), IMAGE_ICON, 0, 0, 0);

	SetIcon(hIconIn, hIconOut);

}

void CButtonST::SetIcon(HICON hIconIn, HICON hIconOut)
{
	return;
	
	if (m_hIconIn != NULL) ::DestroyIcon(m_hIconIn);
	if (m_hIconOut != NULL) ::DestroyIcon(m_hIconOut);


	m_hIconIn = hIconIn;

	m_hIconOut = (hIconOut == NULL) ? m_hIconIn : hIconOut;
  
	ICONINFO ii;


	ZeroMemory(&ii, sizeof(ICONINFO));
	::GetIconInfo(m_hIconIn, &ii);

	m_cxIcon = (BYTE)(ii.xHotspot * 2);
	m_cyIcon = (BYTE)(ii.yHotspot * 2);
    ::DeleteObject(ii.hbmMask);
	::DeleteObject(ii.hbmColor);

	RedrawWindow();
} 

BOOL CButtonST::SetBtnCursor(int nCursorId)
{//对话框基类
	HINSTANCE hInstResource;
	
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;


	if (nCursorId != -1)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId),
											RT_GROUP_CURSOR);
	
		m_hCursor = (HCURSOR)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
	
		if (m_hCursor == NULL) return FALSE;
	}

	return TRUE;
} 


void CButtonST::SetFlat(BOOL bState)
{
  m_bIsFlat = bState;
  Invalidate();
}


BOOL CButtonST::GetFlat()
{
  return m_bIsFlat;
} 


void CButtonST::SetAlign(int nAlign)
{
  switch (nAlign)
  {    
    case ST_ALIGN_HORIZ:
         m_nAlign = ST_ALIGN_HORIZ;
         break;
    case ST_ALIGN_HORIZ_RIGHT:
         m_nAlign = ST_ALIGN_HORIZ_RIGHT;
         break;
    case ST_ALIGN_VERT:
         m_nAlign = ST_ALIGN_VERT;
         break;
  }
  Invalidate();
}


int CButtonST::GetAlign()
{
  return m_nAlign;
} 


void CButtonST::DrawBorder(BOOL bEnable)
{
  m_bDrawBorder = bEnable;
}


const char* CButtonST::GetVersionC()
{
  return "2.6";
} 


const short CButtonST::GetVersionI()
{
  return 26; 
} 


void CButtonST::SetShowText(BOOL bShow)
{
  m_bShowText = bShow;
  Invalidate();
} 


BOOL CButtonST::GetShowText()
{
  return m_bShowText;
} 


void CButtonST::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd* pWnd;  
	CWnd* pParent; 

	CButton::OnMouseMove(nFlags, point);

	
	if (nFlags & MK_LBUTTON && m_MouseOnButton == FALSE) return;



	pWnd = GetActiveWindow();
	pParent = GetOwner();

	if ((GetCapture() != this) && 
		(
#ifndef ST_LIKEIE
		pWnd != NULL && 
#endif
		pParent != NULL)) 
	{
		m_MouseOnButton = TRUE;
	
		SetCapture();
		Invalidate();
	}
	else
	{
		

		POINT p2 = point;
		ClientToScreen(&p2);
		CWnd* wndUnderMouse = WindowFromPoint(p2);

		if (wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd)
		{
		
			if (m_MouseOnButton == TRUE)
			{
				m_MouseOnButton = FALSE;
				Invalidate();
			}
		
			if (!(nFlags & MK_LBUTTON)) ReleaseCapture();
		}
	}
} 


void CButtonST::OnKillFocus(CWnd * pNewWnd)
{//对话框基类
  CButton::OnKillFocus(pNewWnd);

 
  if (m_bIsFlat == FALSE) return;

  if (m_MouseOnButton == TRUE)
  {
    m_MouseOnButton = FALSE;
    Invalidate();
  }
} 


void CButtonST::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_MouseOnButton == TRUE)
	{
		ReleaseCapture();
		Invalidate();
	}

	
	CButton::OnCaptureChanged(pWnd);
} 


void CButtonST::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
#ifdef ST_USE_MEMDC
  CDC  *pdrawDC = CDC::FromHandle(lpDIS->hDC);
  CMemDC memDC(pdrawDC);
  CDC  *pDC = &memDC;
#else	
  CDC* pDC = CDC::FromHandle(lpDIS->hDC);
#endif

  CPen *pOldPen;
  BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
  BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
  BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

  CRect itemRect = lpDIS->rcItem;

  pDC->SetBkMode(TRANSPARENT);

  if (m_bIsFlat == FALSE)
  {
    if (bIsFocused || (GetDefault() == TRUE))
    {
      CBrush br(RGB(0,0,0));  
      pDC->FrameRect(&itemRect, &br);
      itemRect.DeflateRect(1, 1);
    }
  }


  COLORREF bgColor;
  if ((m_MouseOnButton == TRUE) || (bIsPressed))
    bgColor = GetActiveBgColor();
  else
    bgColor = GetInactiveBgColor();

	CBrush br(bgColor);

	if (m_bDrawTransparent == TRUE)
	{
		PaintBk(pDC);
	}
	else
	{
		pDC->FillRect(&itemRect, &br);
	}

	
  if (bIsPressed)
  {
    if (m_bIsFlat == TRUE)
    {
      if (m_bDrawBorder == TRUE)
      {
		  pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));

      }
    }
    else    
    {
      CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
      pDC->FrameRect(&itemRect, &brBtnShadow);
    }
  }
  else 
  {
    CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); 
    CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));      
    CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW)); 
    CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); 

    if (m_bIsFlat == TRUE)
    {
      if (m_MouseOnButton == TRUE && m_bDrawBorder == TRUE)
      {
		  pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));

      }
    }
    else
    {
      
      pOldPen = pDC->SelectObject(&penBtnHiLight);
      pDC->MoveTo(itemRect.left, itemRect.bottom-1);
      pDC->LineTo(itemRect.left, itemRect.top);
      pDC->LineTo(itemRect.right, itemRect.top);
    
      pDC->SelectObject(pen3DLight);
      pDC->MoveTo(itemRect.left+1, itemRect.bottom-1);
      pDC->LineTo(itemRect.left+1, itemRect.top+1);
      pDC->LineTo(itemRect.right, itemRect.top+1);
      
      pDC->SelectObject(pen3DDKShadow);
      pDC->MoveTo(itemRect.left, itemRect.bottom-1);
      pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
      pDC->LineTo(itemRect.right-1, itemRect.top-1);
    
      pDC->SelectObject(penBtnShadow);
      pDC->MoveTo(itemRect.left+1, itemRect.bottom-2);
      pDC->LineTo(itemRect.right-2, itemRect.bottom-2);
      pDC->LineTo(itemRect.right-2, itemRect.top);
      
      pDC->SelectObject(pOldPen);
    }
  }

 
  CString sTitle;
  GetWindowText(sTitle);


  if (m_bShowText == FALSE) sTitle.Empty();

  CRect captionRect = lpDIS->rcItem;


  if (m_hIconIn != NULL)
  {
    DrawTheIcon(pDC, &sTitle, &lpDIS->rcItem, &captionRect, bIsPressed, bIsDisabled);
  }


  if (sTitle.IsEmpty() == FALSE)
  {
   
    if (bIsPressed)
      captionRect.OffsetRect(1, 1);
    
    

#ifdef ST_USE_MEMDC

    CFont *pCurrentFont = GetFont(); 
    CFont *pOldFont = pDC->SelectObject(pCurrentFont);
#endif
    if ((m_MouseOnButton == TRUE) || (bIsPressed)) 
	{
      pDC->SetTextColor(GetActiveFgColor());
      pDC->SetBkColor(GetActiveBgColor());
    } 
	else 
	{
      pDC->SetTextColor(GetInactiveFgColor());
      pDC->SetBkColor(GetInactiveBgColor());
    }
  
    CRect centerRect = captionRect;
    pDC->DrawText(sTitle, -1, captionRect, DT_SINGLELINE|DT_CALCRECT);
    captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
	

	pDC->SetBkMode(TRANSPARENT);
    pDC->DrawState(captionRect.TopLeft(), captionRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
                   TRUE, 0, (CBrush*)NULL);
#ifdef ST_USE_MEMDC
    pDC->SelectObject(pOldFont);
#endif
  }

  if (m_bIsFlat == FALSE || (m_bIsFlat == TRUE && m_bDrawFlatFocus == TRUE))
  {
   
    if (bIsFocused)
    {
      CRect focusRect = itemRect;
      focusRect.DeflateRect(3, 3);
      pDC->DrawFocusRect(&focusRect);
    }
  }
} 


void CButtonST::DrawTheIcon(CDC* pDC, CString* title, RECT* rcItem, CRect* captionRect, BOOL IsPressed, BOOL IsDisabled)
{//对话框基类
	CRect iconRect = rcItem;
	CRect btnRect;

  switch (m_nAlign)
  {
    case ST_ALIGN_HORIZ:
         if (title->IsEmpty())
         {
           
           iconRect.left += ((iconRect.Width() - m_cxIcon)/2);
         }
         else
         {
          
           iconRect.left += 3;  
           captionRect->left += m_cxIcon/2 + 3;
         }
         
         iconRect.top += ((iconRect.Height() - m_cyIcon)/2);
         break;
	case ST_ALIGN_HORIZ_RIGHT:
			GetClientRect(&btnRect);
			if (title->IsEmpty())
			{
			
				iconRect.left += ((iconRect.Width() - m_cxIcon)/2);
			}
			else
			{
				
				captionRect->right = captionRect->Width() - m_cxIcon - 3;
				captionRect->left = 3;
				iconRect.left = btnRect.right - m_cxIcon - 3;
			
				iconRect.top += ((iconRect.Height() - m_cyIcon)/2);
			}
	break;
    case ST_ALIGN_VERT:
         
         iconRect.left += ((iconRect.Width() - m_cxIcon)/2);
         if (title->IsEmpty())
         {
     
           iconRect.top += ((iconRect.Height() - m_cyIcon)/2);           
         }
         else
         {
           captionRect->top += m_cyIcon;
         }
         break;
  }
    

  if (IsPressed) iconRect.OffsetRect(1, 1);

  pDC->DrawState(	iconRect.TopLeft(),
					iconRect.Size(), 
					(m_MouseOnButton == TRUE || IsPressed) ? m_hIconIn : m_hIconOut, 
					(IsDisabled ? DSS_DISABLED : DSS_NORMAL), 
					(CBrush*)NULL);
} 


void CButtonST::PreSubclassWindow() 
{
	UINT nBS;

	nBS = GetButtonStyle();


	if (nBS & BS_DEFPUSHBUTTON) m_bIsDefault = TRUE;


	SetButtonStyle(nBS | BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}


BOOL CButtonST::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
} 


LRESULT CButtonST::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	}
	return CButton::DefWindowProc(message, wParam, lParam);
} 


void CButtonST::SetDefaultInactiveBgColor(BOOL bRepaint)
{
	m_crInactiveBg = ::GetSysColor(COLOR_BTNFACE); 
	if (bRepaint == TRUE) Invalidate();
} 

void CButtonST::SetInactiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveBg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} 


const COLORREF CButtonST::GetInactiveBgColor()
{
	return m_crInactiveBg;
}


void CButtonST::SetDefaultInactiveFgColor(BOOL bRepaint)
{
	m_crInactiveFg = ::GetSysColor(COLOR_BTNTEXT); 
	if (bRepaint == TRUE) Invalidate();
} 

void CButtonST::SetInactiveFgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crInactiveFg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} 


const COLORREF CButtonST::GetInactiveFgColor()
{
	return m_crInactiveFg;
} 


void CButtonST::SetDefaultActiveBgColor(BOOL bRepaint)
{
	m_crActiveBg = ::GetSysColor(COLOR_BTNFACE); 
	if (bRepaint == TRUE) Invalidate();
}


void CButtonST::SetActiveBgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveBg = crNew; 
	if (bRepaint == TRUE) Invalidate();
} 


const COLORREF CButtonST::GetActiveBgColor()
{
	return m_crActiveBg;
} 


void CButtonST::SetDefaultActiveFgColor(BOOL bRepaint)
{
	m_crActiveFg = ::GetSysColor(COLOR_BTNTEXT); 
	if (bRepaint == TRUE) Invalidate();
}


void CButtonST::SetActiveFgColor(COLORREF crNew, BOOL bRepaint)
{
	m_crActiveFg = crNew; 
	if (bRepaint == TRUE) Invalidate();
}


const COLORREF CButtonST::GetActiveFgColor()
{
	return m_crActiveFg;
} 


void CButtonST::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	

	if (bRepaint == TRUE) Invalidate();
}


BOOL CButtonST::GetFlatFocus()
{
	return m_bDrawFlatFocus;
} 

BOOL CButtonST::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{

	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CButton::OnSetCursor(pWnd, nHitTest, message);
} 


void CButtonST::SetTooltipText(CString* spText, BOOL bActivate)
{

	if (spText == NULL) return;

	
	InitToolTip();


	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}


	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
} 


void CButtonST::SetTooltipText(int nId, BOOL bActivate)
{
	CString sText;


	sText.LoadString(nId);

	if (sText.IsEmpty() == FALSE) SetTooltipText(&sText, bActivate);
} 


void CButtonST::ActivateTooltip(BOOL bActivate)
{

	if (m_ToolTip.GetToolCount() == 0) return;


	m_ToolTip.Activate(bActivate);
} 


BOOL CButtonST::GetDefault()
{
	return m_bIsDefault;
} 


void CButtonST::DrawTransparent(BOOL bRepaint)
{
	m_bDrawTransparent = TRUE;


	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	m_bmpBk.Detach();
	m_dcBk.Detach();


	if (bRepaint == TRUE) Invalidate();
} 


void CButtonST::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
	
		m_ToolTip.Create(this);
	
		m_ToolTip.Activate(FALSE);
	}
}


void CButtonST::PaintBk(CDC * pDC)
{
	CClientDC clDC(GetParent());
	CRect rect;
	CRect rect1;

	GetClientRect(rect);

	GetWindowRect(rect1);
	GetParent()->ScreenToClient(rect1);

	if (m_dcBk.m_hDC == NULL)
	{
		m_dcBk.CreateCompatibleDC(&clDC);
		m_bmpBk.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
	}

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
} 


HBRUSH CButtonST::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
}


void CButtonST::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();	
} 


#undef ST_USE_MEMDC
#undef ST_LIKEIE
