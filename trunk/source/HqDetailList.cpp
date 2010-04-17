// HqDetailList.cpp : implementation file
//

#include "stdafx.h"

#include "HqDetailList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHqDetailList

CHqDetailList::CHqDetailList()
{
}

CHqDetailList::~CHqDetailList()
{
}


BEGIN_MESSAGE_MAP(CHqDetailList, CListCtrl)
	//{{AFX_MSG_MAP(CHqDetailList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHqDetailList message handlers

void CHqDetailList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	int nItem = lpDrawItemStruct->itemID;
	CImageList* pImageList;

	// Save dc state
	int nSavedDC = pDC->SaveDC();
    
	///////////Set Back Color/////////////
	pDC->SetBkColor(pDoc->m_colorArray[0]);
    
	CBrush brush;                      
	CBrush *poldbrush;
	brush.CreateSolidBrush(pDoc->m_colorArray[0]);
	poldbrush=pDC->SelectObject(&brush);
	pDC->FillRect(rcItem,&brush);
	pDC->SelectObject(poldbrush);

	//////////////////////////////////
	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	// Get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol( rcBounds );   


	CString sLabel = GetItemText( nItem, 0 );

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character

	pDC->SetTextColor(pDoc->m_colorArray[14]);
	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;

	CRect rcHighlight;
	CRect rcWnd;
    
	rcCol.right = rcCol.left + GetColumnWidth(0);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcCol);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// Draw state icon
	if (lvi.state & LVIS_STATEIMAGEMASK)
	{
		int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		pImageList = GetImageList(LVSIL_STATE);
		if (pImageList)
		{
			pImageList->Draw(pDC, nImage,
				CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
		}
	}
	
	// Draw item label - Column 0
	rcLabel.left += offset/2;
	rcLabel.right -= offset;

	pDC->DrawText(sLabel,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
				| DT_VCENTER );


	// Draw labels for remaining columns
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
	
	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right :
							rcBounds.right;
	rgn.CreateRectRgnIndirect(&rcBounds);
	pDC->SelectClipRgn(&rgn);
				   
	double UpOrDown;
	for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
	   if(nColumn == 1)
	   {
	     sLabel = GetItemText(nItem, 2);
         UpOrDown = atof(sLabel);
		 if(UpOrDown > 0)
		    pDC->SetTextColor(pDoc->m_colorArray[13]);
         if(UpOrDown < 0)
		    pDC->SetTextColor(pDoc->m_colorArray[15]);
		 if(UpOrDown == 0)
		    pDC->SetTextColor(pDoc->m_colorArray[14]);
	   }   
	   if(nColumn == 4)
	      pDC->SetTextColor(pDoc->m_colorArray[13]);
       if(nColumn == 5)
	      pDC->SetTextColor(pDoc->m_colorArray[15]);
	   if(nColumn == 6 || nColumn == 9)
	   {
	     if(UpOrDown > 0)
		    pDC->SetTextColor(pDoc->m_colorArray[13]);
         if(UpOrDown < 0)
		    pDC->SetTextColor(pDoc->m_colorArray[15]);
		 if(UpOrDown == 0)
		    pDC->SetTextColor(pDoc->m_colorArray[14]);
	   }
	   if(nColumn == 7)
		   pDC->SetTextColor(pDoc->m_colorArray[14]);
       if(nColumn == 8)
		   pDC->SetTextColor(pDoc->m_colorArray[14]);
	   rcCol.left = rcCol.right;
	   rcCol.right += lvc.cx;

		// Draw the background if needed
//		if( m_nHighlight == HIGHLIGHT_NORMAL )
//			pDC->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));

		sLabel = GetItemText(nItem, nColumn);
		if (sLabel.GetLength() == 0)
			continue;

		UINT nJustify = DT_LEFT;
		switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_RIGHT:
			nJustify = DT_RIGHT;
			break;
		case LVCFMT_CENTER:
			nJustify = DT_CENTER;
			break;
		default:
			break;
		}

		rcLabel = rcCol;
		rcLabel.left += offset;
		rcLabel.right -= offset;

		pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
					DT_NOPREFIX | DT_VCENTER );
	}
	// Restore dc
	pDC->RestoreDC( nSavedDC );
} 

