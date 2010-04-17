// CSharesDetailList.cpp : implementation file
// by ÓáÃ÷Â¼

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CSharesDetailList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DiaStockdetail.h"
/////////////////////////////////////////////////////////////////////////////
// CSharesDetailList

CSharesDetailList::CSharesDetailList()
{

  num = 0;
}

CSharesDetailList::~CSharesDetailList()
{
}


BEGIN_MESSAGE_MAP(CSharesDetailList, CListCtrl)
	//{{AFX_MSG_MAP(CSharesDetailList)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSharesDetailList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	int nItem = lpDrawItemStruct->itemID;
	CImageList* pImageList;


	int nSavedDC = pDC->SaveDC();

	pDC->SetBkColor(pDoc->m_colorArray[0]);
    
	CBrush brush;                      
	CBrush *poldbrush;
	brush.CreateSolidBrush(pDoc->m_colorArray[0]);
	poldbrush=pDC->SelectObject(&brush);
	pDC->FillRect(rcItem,&brush);
	pDC->SelectObject(poldbrush);

	
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;	
	GetItem(&lvi);


	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol( rcBounds );   


	CString sLabel = GetItemText( nItem, 0 );
    CString UpDown = GetItemText( nItem, 3);

	if(this->m_IsF1 == false)
	{
	  if(RedOrGreen[nItem] == 1)
		 pDC->SetTextColor(pDoc->m_colorArray[13]);
      if(RedOrGreen[nItem] == 2)
         pDC->SetTextColor(pDoc->m_colorArray[15]);
      if(RedOrGreen[nItem] == 3)
         pDC->SetTextColor(pDoc->m_colorArray[14]);
	}
	else
	   pDC->SetTextColor(pDoc->m_colorArray[14]);
	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;

	CRect rcHighlight;
	CRect rcWnd;
	int nExt;
    
	rcCol.right = rcCol.left + GetColumnWidth(0);
	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcCol);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();


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
	

	rcLabel.left += offset/2;
	rcLabel.right -= offset;

	pDC->DrawText(sLabel,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
				| DT_VCENTER | DT_END_ELLIPSIS);



	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	
	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right :
							rcBounds.right;
	rgn.CreateRectRgnIndirect(&rcBounds);
	pDC->SelectClipRgn(&rgn);
				   
	for(int nColumn = 1; GetColumn(nColumn, &lvc); nColumn++)
	{
		sLabel = GetItemText(nItem, nColumn);
		if(nColumn == 3 && m_IsF1 == true)
		{
		    if(atoi(UpDown) > 0)
			   pDC->SetTextColor(pDoc->m_colorArray[13]);
			if(atoi(UpDown) == 0)
			   pDC->SetTextColor(pDoc->m_colorArray[14]);
			if(atoi(UpDown) < 0)
			   pDC->SetTextColor(pDoc->m_colorArray[15]);
		}
		else
		{
		  pDC->SetTextColor(pDoc->m_colorArray[14]);
		}
		rcCol.left = rcCol.right;
		rcCol.right += lvc.cx;

	

		sLabel = GetItemText(nItem, nColumn);
		if(nColumn == 3 && m_IsF1 == true)
		{
		  int t = atoi(UpDown);
		  if(t > 0)
		    sLabel.Format("%d¡ü",abs(t));
		  if(t < 0)
		    sLabel.Format("%d¡ý",abs(t));
		  if(t == 0)
		    sLabel.Format("%d",abs(t));

		}

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

		if(nColumn == 3 && m_IsF1 == true)
           pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
					DT_NOPREFIX | DT_RIGHT | DT_END_ELLIPSIS);
		else
			pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE | 
					DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
		
	}

	pDC->RestoreDC( nSavedDC );
}
