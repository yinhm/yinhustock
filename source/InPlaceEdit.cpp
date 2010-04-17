// InPlaceEdit.cpp : implementation file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. If 
// the source code in  this file is used in any commercial application 
// then acknowledgement must be made to the author of this file 
// (in whatever form you wish).
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
// Modifed 10 May 1998  Uses GVN_ notifications instead of LVN_,
//                      Sends notification messages to the parent, 
//                      instead of the parent's parent.
//
//         15 May 1998  There was a problem when editing with the in-place editor, 
//                      there arises a general protection fault in user.exe, with a 
//                      few qualifications:
//                         (1) This only happens with owner-drawn buttons;
//                         (2) This only happens in Win95
//                         (3) This only happens if the handler for the button does not 
//                             create a new window (even an AfxMessageBox will avoid the 
//                             crash)
//                         (4) This will not happen if Spy++ is running.
//                      PreTranslateMessage was added to route messages correctly.
//                      (Matt Weagle found and fixed this problem)
//         26 Jul 1998  Removed the ES_MULTILINE style - that fixed a few probs!
//          6 Aug 1998  Added nID to the constructor param list
//          6 Sep 1998  Space no longer clears selection when starting edit (Franco Bez)
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCHAR.h"
#include "InPlaceEdit.h"

#include "GridCtrl.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

CInPlaceEdit::CInPlaceEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, CString sInitText, 
                           UINT nFirstChar)
{
    m_sInitText     = sInitText;
    m_nRow          = nRow;
    m_nColumn       = nColumn;
    m_nLastChar     = 0; 
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);   
                                                     

    DWORD dwEditStyle = WS_BORDER|WS_CHILD|WS_VISIBLE| ES_AUTOHSCROLL 
                       | dwStyle;
    if (!Create(dwEditStyle, rect, pParent, nID)) return;

    SetFont(pParent->GetFont());

    SetWindowText(sInitText);
    SetFocus();

    switch (nFirstChar){
        case VK_LBUTTON: 
        case VK_RETURN:   SetSel((int)_tcslen(m_sInitText), -1); return;
        case VK_BACK:     SetSel((int)_tcslen(m_sInitText), -1); break;
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:
        case VK_SPACE:
        case VK_END:      SetSel(0,-1); return;
        default:          SetSel(0,-1);
    }

    SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceEdit::~CInPlaceEdit()
{
}
 
BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
    //{{AFX_MSG_MAP(CInPlaceEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_KEYUP()
    ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
////////////////////////////////////////////////////////////////////////////

void CInPlaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
         nChar == VK_DOWN  || nChar == VK_UP   ||
         nChar == VK_RIGHT || nChar == VK_LEFT) &&
        (m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return;
    }

    if (nChar == VK_ESCAPE) 
    {
        SetWindowText(m_sInitText);   
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return;
    }

    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CInPlaceEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();  
        return;
    }

    CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);
    EndEdit();
}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
     CEdit::OnChar(nChar, nRepCnt, nFlags);
 
   
     CString str;
     GetWindowText( str );

     CWindowDC dc(this);
     CFont *pFontDC = dc.SelectObject(GetFont());
     CSize size = dc.GetTextExtent( str );
     dc.SelectObject( pFontDC );

     size.cx += 5;                
 
  
     CRect rect, parentrect;
     GetClientRect( &rect );
     GetParent()->GetClientRect( &parentrect );
 
 
     ClientToScreen( &rect );
     GetParent()->ScreenToClient( &rect );
 
     
     if (size.cx > rect.Width())
     {
         if( size.cx + rect.left < parentrect.right )
             rect.right = rect.left + size.cx;
         else
             rect.right = parentrect.right;
         MoveWindow( &rect );
     }
}

////////////////////////////////////////////////////////////////////////////

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}	

	
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;

    return CWnd::PreTranslateMessage(pMsg);
}


void CInPlaceEdit::PostNcDestroy() 
{
	CEdit::PostNcDestroy();
	
    delete this;	
}

////////////////////////////////////////////////////////////////////////////

void CInPlaceEdit::EndEdit()
{
    CString str;
    GetWindowText(str);
 
   
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;
 
    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.szText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar; 
 
    CWnd* pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
 

    PostMessage(WM_CLOSE, 0, 0);
}
 
