// CNineGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "NineGridCtrl.h"
#include "NineShowView.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CNineGridCtrl::CNineGridCtrl()
{
   m_pShowData=NULL;
   m_bGetFocus=FALSE;
   m_ntime=0;
   m_bSync=FALSE;
}

CNineGridCtrl::~CNineGridCtrl()
{
	m_nRealRow=0;
}


BEGIN_MESSAGE_MAP(CNineGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CNineGridCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
    ON_WM_LBUTTONDOWN()
	    ON_WM_KEYDOWN()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNineGridCtrl message handlers
BOOL CNineGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;
    if(m_pDoc==NULL)		
	{
		return TRUE;
	}
    if(nRow==0)
	{
        pGridCell->state=0;      
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE ;    
		pGridCell->szText=m_pColData[nCol].szText;      // Cell text (or binary data if you wish...)
		pGridCell->iImage=-1;      
	    pGridCell->crBkClr=m_pDoc->m_colorArray[18];  
		
		pGridCell->crFgClr=m_pDoc->m_colorArray[14];    
		pGridCell->lParam=0;      
		pGridCell->lfFont=m_Logfont;    
		return TRUE;
	}
	DWORD key= MAKELONG(nRow, nCol);
	if(nRow==1&&nCol==0)
		nCol=0;
    CCellID cell;
    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow)&&m_bGetFocus)
	{
        pGridCell->state=GVIS_SELECTED;       
	}
	else
	{
        pGridCell->state=0;      
	}

    pGridCell->iImage=-1;  
    pGridCell->lParam=0;      
    pGridCell->lfFont=m_Logfont;     

    pGridCell->crBkClr=CLR_DEFAULT;  
    pGridCell->nFormat=m_pColData[nCol].nFormat;
    
	if(nRow >= m_nRealRow)
	{
        pGridCell->szText="";
		return TRUE;
	}
	char str[50];
	float m_temp;
	if(m_pShowData==NULL)
		return FALSE;
	CReportData *pData=m_pShowData[nRow-1].pItem;
	if(pData==NULL )
		return FALSE;
	switch(m_pColData[nCol].nID)
	{
	    case ID_ROWNUM:
              sprintf(str,"%d",nRow);
			  pGridCell->szText =str;
			  pGridCell->crFgClr = m_pDoc->m_colorArray[14]; 
			  break;
		case ID_STOCKID:                                      
              sprintf(str,"%s",pData->id);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case ID_STOCKNAME:                                  
              sprintf(str,"%s",pData->name );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case ID_NEW:                             
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->nowp  );
			else
              sprintf(str,"%6.2f",pData->nowp   );
			  if(pData->nowp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->nowp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;

		case ID_TOTP:                            
			if(pData->kind==SHBG) 
              sprintf(str,"%6.3f",pData->totp /10000  );
			else
              sprintf(str,"%6.2f",pData->totp  /10000 );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;

		case ID_HLP:                               
              if(pData->ystc !=0)
			  {
                  sprintf(str,"%6.2f%s",100 * (pData->higp - pData->lowp) /pData->ystc ,"%" );
			  }
			  else
                 sprintf(str,"%6.2f",0.0f); 
			  pGridCell->szText =str;
			  if(pData->opnp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->opnp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_RDR:                               
			 if(pData->ystc !=0&&pData->nowp!=0)
			 {
               sprintf(str,"%6.2f%s",100 * (pData->nowp - pData->ystc )/ pData->ystc ,"%"  );
			 }
			 else
               sprintf(str,"%6.2f",0.0f);
			  pGridCell->szText =str;
			  if(pData->nowp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->nowp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_BSSPBAS:                           
			{
				float temp=0;
				if (pData->accb+pData->accs>0)
					if(pData->accs==0&&pData->accb!=0)
					   temp=100;
					else if(pData->accs!=0&&pData->accb==0)
					   temp=-100;
					else
					   temp=(float)(pData->accb-pData->accs)/(float)(pData->accb+pData->accs)*100.0;     //¼ÆËãÎ¯±È
				else
					temp=0;
				  sprintf(str,"%6.2f%s",temp ,"%"  );
				  pGridCell->szText =str;
				  if(pData->accb > pData->accs )
					 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				  else if(pData->accb > pData->accs)
					 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			}
			break;
		case ID_VOLBI:                            
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
              sprintf(str,"%6.2f%s",(pData->totv / m_pDoc->m_nANT[0]) / (pData->volume5 /240 ),"%" );
			}
			else
              sprintf(str,"%6.2f",0.0f   );

			  pGridCell->szText =str;
			  if(pData->nowp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->nowp < pData->ystc)
 				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_DVOL:                              
			if(pData->totv!=0)
               sprintf(str,"%6.2f%s",100*pData->rvol/(pData->totv),"%"  );
			else
			   sprintf(str,"%6.2f",0.0f);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			break;

        case ID_EXCHANGE:
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
				  if(CTaiShanReportView::IsBg(pData-> kind))
                     if(pData->pBaseInfo->Bg!=0) 
                        sprintf(str,"%6.2f",pData->totv/pData->pBaseInfo->Bg);
					 else
			            sprintf(str,"%6.2f",0.0f);
				  else
					  if(pData->pBaseInfo->ltAg!=0)
			            sprintf(str,"%6.2f",pData->totv/pData->pBaseInfo->ltAg);
					  else
			            sprintf(str,"%6.2f",0.0f);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
        case ID_SYL:
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
				 if(pData->pBaseInfo->mgsy!=0)
                   sprintf(str,"%6.3f",pData->nowp/pData->pBaseInfo->mgsy); 
				 else
			       sprintf(str,"%6.2f",0.0f);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case ID_ZJLS:        

			  if(m_pDoc->m_nANT[0]>m_ntime)
			  {
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_ntime)
				{
					i--;
				}
                sprintf(str,"%6.2f",(pData->m_Kdata1[i].Amount - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_ntime ].Amount)/10000);
			  }
			  else
			  {
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_ntime)
				{
					i--;
				}
                sprintf(str,"%6.2f",pData->m_Kdata1[i].Amount/10000 );
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case ID_VOLBIFM:  
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_ntime)
				{
				   i--;
				}
                 if(m_pDoc->m_nANT[0] > m_ntime)
				 {
                    sprintf(str,"%6.2f%s",(100*(pData->m_Kdata1[i].Volume - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_ntime ].Volume) / m_ntime) / (pData->volume5 /240 ),"%" );
				 }
				 else
				 {
                    sprintf(str,"%6.2f%s",(100*(pData->m_Kdata1[i].Volume ) / m_ntime) / (pData->volume5 /240 ),"%" );
				 }
			}
			else
                 sprintf(str,"%6.2f",0.0f   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
		    break;
		case ID_HLPFM:    
			{
				float high=0,low=0,totp=0;
				int minute=m_ntime;
				if(m_pDoc->m_nANT[0] < minute)
                    minute=m_pDoc->m_nANT[0];
				for(int i=0;i<minute;i++)
				{   
				    if(pData->m_Kdata1[m_pDoc->m_nANT[0]-i].Price==0)
						continue;
					if(high==0&&low==0)
					{
						high=low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
						continue;
					}
					if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price > high  )
						high=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
					else if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price <low)
						low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
				}
				if(pData->ystc==0)
				  sprintf(str,"%6.2f%s",0.0f,"%");
				else
				  sprintf(str,"%6.2f%s",100*(high-low)/(pData->ystc),"%");
			    pGridCell->szText =str;
			    pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			}
			break;
		case ID_EXCHANGEFM:  
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
				  int i=m_pDoc->m_nANT[0];
				  while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_ntime)
				  {
				 	  i--;
				  }
				  if(CTaiShanReportView::IsBg(pData-> kind))
                     if(pData->pBaseInfo->Bg!=0) 
					 {
						 if(m_pDoc->m_nANT[0] > m_ntime)
                           sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume - pData->m_Kdata1[i-m_ntime].Volume)/pData->pBaseInfo->Bg);
						 else
                           sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume)/pData->pBaseInfo->Bg);
					 }
					 else
			            sprintf(str,"%6.2f",0.0f);
				  else
					  if(pData->pBaseInfo->ltAg!=0)
					  {
						 if(m_pDoc->m_nANT[0] > m_ntime)
			                sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume - pData->m_Kdata1[i - m_ntime].Volume)/pData->pBaseInfo->ltAg);
						 else
			                sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume)/pData->pBaseInfo->ltAg);
					  }
					  else
			            sprintf(str,"%6.2f",0.0f);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case ID_ADVSP:     
			if( m_ntime  <m_pDoc->m_nANT[0])
				if( pData->m_Kdata1[m_pDoc->m_nANT[0] - m_ntime ].Price!=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						  i--;
					}
					m_nowp=pData->m_Kdata1[i].Price;
					m_temp= 100*(m_nowp - 
					pData->m_Kdata1[m_pDoc->m_nANT[0] - m_ntime ].Price)/pData->m_Kdata1[m_pDoc->m_nANT[0] - m_ntime ].Price;
				}   
				else
                    m_temp=0.0f;
			else
				if(pData->opnp !=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						  i--;
					}
                    m_nowp=pData->m_Kdata1[i].Price;

					m_temp= 100*(m_nowp - pData->opnp )/pData->opnp ;
				}
				else
                    m_temp=0.0f;
              sprintf(str,"%6.2f%s",m_temp,"%"   );
			  pGridCell->szText =str;
			  if(m_temp>0 )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(m_temp<0)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 

			break;
		}
   return TRUE;
}
BOOL CNineGridCtrl::SetRowCount(int nRows)
{
    ASSERT(nRows >= 0);
    if (nRows == GetRowCount()) return TRUE;

    if (nRows < m_nFixedRows) 
        m_nFixedRows = nRows;

    if (m_idCurrentCell.row >= nRows)
        SetFocusCell(-1,-1);

    int addedRows = nRows - GetRowCount();
    m_nRows = nRows;
    m_arRowHeights.SetSize(nRows);

  
    if (addedRows > 0)
	{
    
        int startRow = nRows - addedRows;
        for (int row = startRow; row < GetRowCount(); row++)
		{
            m_arRowHeights[row] = m_nDefCellHeight;
        }
    }
    if (GetSafeHwnd() && m_bAllowDraw)
    {
        ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}
BOOL CNineGridCtrl::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
    if (!m_bAllowDraw) return FALSE;
 
    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_STATE | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;


    COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? GetTextBkColor() : Item.crBkClr;
    COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? GetTextColor()   : Item.crFgClr;


    if (TextBkClr != CLR_DEFAULT)
        bEraseBk = TRUE;

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (Item.state & GVIS_FOCUSED && !pDC->IsPrinting()) 
    {
        rect.right++; rect.bottom++;   
        if (bEraseBk) 
        {
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;    
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
        pDC->SetTextColor(TextClr);

        rect.DeflateRect(1,1);

    }
    else if (Item.state & GVIS_SELECTED && !pDC->IsPrinting()) 
    {
        rect.right++; rect.bottom++;   
       
        pDC->FillSolidRect(rect, m_crCursor);
		////////////
		rect.right--; rect.bottom--;
        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    } else {
        rect.right++; rect.bottom++; 
        if (bEraseBk) 
        {
			CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;
        pDC->SetTextColor(TextClr);
    }

    if (Item.state & GVIS_DROPHILITED && !pDC->IsPrinting()) {
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
    }

  
    CFont Font;

    Font.CreateFontIndirect(&m_Logfont);

    pDC->SelectObject(&Font);
    rect.DeflateRect(m_nMargin, 0);

    if (m_pImageList && Item.iImage >= 0) {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info)) {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+m_nMargin;
        }
    }

    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);

	if(!m_bGetFocus&&m_bSync&&nCol==0&&nRow==GetCurrentRow())
	{
		CPoint pPage[3];
		double cxB=rect.top+(rect.bottom - rect.top)/2;
		
		CPen BlackPen(PS_SOLID,1,RGB(0,0,0));
		CPen *OldPen=pDC->SelectObject(&BlackPen);
	
		pPage[0]=CPoint(rect.left,cxB-4);
		pPage[1]=CPoint(rect.left,cxB+4);
		pPage[2]=CPoint(rect.left+12,cxB);
		
		CBrush FillBrush(m_crCursor);
		CBrush FrameBrush(RGB(0,0,0));
		CRgn rgn;
 		rgn.CreatePolygonRgn(pPage,3,WINDING);
		pDC->FillRgn(&rgn,&FillBrush);
		pDC->FrameRgn(&rgn,&FrameBrush,1,1);
		rgn.DeleteObject(); 
		pDC->SelectObject(OldPen);
	}
    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

void CNineGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int l_nRows=GetCurrentRow();
	CGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	if(l_nRows!=GetCurrentRow())
	{
           CWnd *pOwner = GetOwner();
			if (pOwner && IsWindow(pOwner->m_hWnd))
			{
				pOwner->SendMessage(WM_USER_NINESHOW_ROW_CHANGED,0, 0);
				return;
			}
			else
				return ;

	}
}

void CNineGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int l_nRows=GetCurrentRow();
	CGridCtrl::OnLButtonDown(nFlags,point);
	if(l_nRows!=GetCurrentRow())
	{
           CWnd *pOwner = GetOwner();
			if (pOwner && IsWindow(pOwner->m_hWnd))
			{
				pOwner->SendMessage(WM_USER_NINESHOW_ROW_CHANGED,0, 0);
				return;
			}
			else
				return ;

	}
}
