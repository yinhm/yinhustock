// CTaiKlineListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineListCtrl.h"
#include "CTaiShanKlineShowView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CTaiKlineListCtrl::CTaiKlineListCtrl()
{
	m_bDragging = FALSE;
	m_pimageListDrag = NULL;
	m_wParam = 100;

	m_wndTarget = NULL;
	m_strResult = "";
	m_stkKind = 0;
}

CTaiKlineListCtrl::~CTaiKlineListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaiKlineListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTaiKlineListCtrl)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTaiKlineListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	long        lStyle;

	lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	lStyle &= LVS_TYPEMASK;  
	if (m_bDragging)
	{
		ASSERT(m_pimageListDrag != NULL);

		CPoint p2(point.x,point.y);
		ClientToScreen(&p2);
		CPoint p(point.x,point.y-18);
		ClientToScreen(&p);
		if(m_wndTarget != NULL)
		{

			if(m_wndTarget ->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
			{
				CRect r;
				GetParent()->GetWindowRect (r);
				CRect r2;
				m_wndTarget->GetClientRect (r2);
				m_wndTarget->ClientToScreen(r2);

				CPoint p(point);
				ClientToScreen(&p);



				if((r.PtInRect (p) == FALSE ) && r2.PtInRect (p))
				{
					if(::GetKeyState(VK_CONTROL)&0x8000)

						SetCursor(LoadCursor( AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_5) )); 
					else
						SetCursor(LoadCursor( AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_4) )); 
				}
				else
					SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_DISABLE ))); // name string or cursor resource identifier);
			}
		}
		else
			SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_DISABLE ))); // name string or cursor resource identifier);

		m_pimageListDrag->DragMove(p); 

	}
}

void CTaiKlineListCtrl::OnButtonUp(CPoint point)
{
	if (m_bDragging)  
	{
		long        lStyle;
		CString     cstr;

		lStyle = GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK;
		m_bDragging = FALSE;

		ASSERT(m_pimageListDrag != NULL);
		m_pimageListDrag->DragLeave(this);
		m_pimageListDrag->EndDrag();
		delete m_pimageListDrag;
		m_pimageListDrag = NULL;

	
		
		cstr =m_strResult; 

		CPoint p2(point.x,point.y);
		ClientToScreen(&p2);

		if(m_wndTarget != NULL)
		{
			if(m_wndTarget ->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
			{
				CRect r;
				GetParent()->GetWindowRect (r);
				CRect r2;
				m_wndTarget->GetClientRect (r2);
				m_wndTarget->ClientToScreen(r2);

				CPoint p(point);
				ClientToScreen(&p);

				if((!r.PtInRect (p)) && r2.PtInRect (p))
				{
					::SendMessage(GetParent()->m_hWnd ,WM_USER_DRAG_BEGIN,1,0);
					::SendMessage(m_wndTarget->m_hWnd,WM_USER_DRAGDROP,this->m_wParam ,(long)((LPSTR)cstr.GetBuffer (0)) );
				}
			}
		}


		::ReleaseCapture();
	}
}

void CTaiKlineListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	OnButtonUp(point);
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CTaiKlineListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	OnButtonUp(point);
	CListCtrl::OnRButtonUp(nFlags, point);
}

void CTaiKlineListCtrl::OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	LV_DISPINFO  *plvDispInfo = (LV_DISPINFO *)pnmhdr;
	LV_ITEM      *plvItem = &plvDispInfo->item;

	if (plvItem->pszText != NULL)
		SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
}

void CTaiKlineListCtrl::OnBeginDrag(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	CPoint          ptItem, ptAction, ptImage;
	NM_LISTVIEW     *pnmListView = (NM_LISTVIEW *)pnmhdr;



	ASSERT(!m_bDragging);
	m_bDragging = TRUE;
	m_iItemDrag = pnmListView->iItem;
	ptAction = pnmListView->ptAction;
	GetItemPosition(m_iItemDrag, &ptItem);  
	GetOrigin(&m_ptOrigin);
	m_ptOrigin = CPoint(0,0);

	ASSERT(m_pimageListDrag == NULL);
	m_pimageListDrag = CreateDragImage(m_iItemDrag, &ptImage);
	m_sizeDelta = ptAction - ptImage;  
	m_ptHotSpot = ptAction - ptItem + m_ptOrigin; 
	m_pimageListDrag->DragShowNolock(TRUE); 

	m_ptHotSpot = CPoint (0,18);

	m_pimageListDrag->BeginDrag(0, CPoint(0, 0));
	ptAction -= m_sizeDelta;
	CPoint p(ptAction);
	ClientToScreen(&p);
	m_pimageListDrag->DragEnter(NULL, p);
	m_pimageListDrag->DragMove(p);  
	SetCapture();
	::SendMessage(GetParent()->m_hWnd ,WM_USER_DRAG_BEGIN,0,0);
}
