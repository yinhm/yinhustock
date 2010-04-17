// CBGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "BGridCtrl.h"
#include "CSharesBigBillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



void SwapMx(BIG_LIST &m_pData1,BIG_LIST &m_pData2)
{
     BIG_LIST pData;
	 memcpy(&pData,&m_pData1,sizeof(BIG_LIST));
	 memcpy(&m_pData1,&m_pData2,sizeof(BIG_LIST));
	 memcpy(&m_pData2,&pData,sizeof(BIG_LIST));// NOTE: the ClassWizard will add member functions here
}
BOOL QuickSortStringMx(BIG_LIST *m_pStockData,BOOL bAscending,int low,int high)
{//子窗口框架
    int lo = low;
    int hi = high;
    BIG_LIST pivot;

    if( hi <= lo ) return FALSE;

	int mid=(low +high)/2;
	pivot=m_pStockData[mid];

    while( lo <= hi )
    {
        
        if( bAscending )
		{
            while (lo < high && strcmp(m_pStockData[lo].id  , pivot.id)<0 )
			{
 				++lo;// NOTE: the ClassWizard will add member functions here
			}
		}
        else
		{
            while ((lo < high  && strcmp(m_pStockData[lo].id, pivot.id)>0))
			{
 				++lo;
			}
		
		}

       
        if( bAscending )
		{
			while (hi > low && strcmp(m_pStockData[hi].id, pivot.id)>0 )
			{
 				--hi;
			}
		}
        else
		{
            while (hi > low && strcmp(m_pStockData[hi].id, pivot.id)<0)
			{
                --hi; // standard constructor
			}
		}
       
        if (lo <= hi)
        {
         
            if ( m_pStockData[lo].id != m_pStockData[hi].id  )
            {
				SwapMx(m_pStockData[lo],m_pStockData[hi]);
            }

            ++lo;
            --hi;
        }
    }

   
    if( low < hi )
        QuickSortStringMx(m_pStockData, bAscending, low, hi);

    
    if( lo < high )
        QuickSortStringMx(m_pStockData, bAscending, lo, high);//Do not modify the contents of this file.

    return TRUE;
}

CBGridCtrl::CBGridCtrl()
// 股票列表视图的列变量定义，以及自定义列的表达式计算
{
}

CBGridCtrl::~CBGridCtrl()
{
	m_nRealRow=0;
}


BEGIN_MESSAGE_MAP(CBGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CBGridCtrl)
		
    ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//Do not modify the contents of this file.

BOOL CBGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{//静态变量
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    if(nRow==0)
	{
        pGridCell->state=0;      
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE;     
		pGridCell->szText=m_pColData[nCol].szText;      
		pGridCell->iImage=-1;     
	    pGridCell->crBkClr=m_pDoc->m_colorArray[18]; 
		
		pGridCell->crFgClr=m_pDoc->m_colorArray[14];    
		pGridCell->lParam=0;      
		pGridCell->lfFont=m_Logfont;    
		return TRUE;
	}

	DWORD key= MAKELONG(nRow, nCol);
    CCellID cell;
    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow))
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
    BIG_LIST *pCjsj=m_pDoc->m_pStockBigBillData->m_pMapData+nRow-1;//Default FontName
	switch(m_pColData[nCol].nID)
	{
	    case 0:
              sprintf(str,"%s",pCjsj->id);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case 1:                                        
              sprintf(str,"%s",pCjsj->name);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case 2:  
			{
			  CString ntime;
			  if(pCjsj->ntime<0)
				  ntime="";
			  else
				  ntime=GetTimeGrid(pCjsj->ntime);
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; //Default FontName
			  pGridCell->szText =ntime;
			}
			break;
		case 3: 
// 股票列表视图的列变量			
              sprintf(str,"%6.2f",pCjsj->ntype  );
			  switch(pCjsj->ntype)
			  {
				  case 0:
					  strcpy(str,"大单买入");
					  break;
				  case 1:
					  strcpy(str,"大单卖出");
					  break;
				  case 2:
					  strcpy(str,"大单挂买");
					  break;
				  case 3:
					  strcpy(str,"大单挂卖");
					  break;
			  }
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case 4:                           
              sprintf(str,"%6.2f",pCjsj->nowv  );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case 5:                             
              sprintf(str,"%6.2f",pCjsj->nowp  );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case 6:                             
              sprintf(str,"%6.2f%s",pCjsj->rdr*100,"%"  );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		}
   return TRUE;
}
BOOL CBGridCtrl::SetRowCount(int nRows)// serious allocation failure checking
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
float CBGridCtrl::GetItemTextV(int nRow,int nCol)
{
    BIG_LIST *pCjsj=m_pDoc->m_pStockBigBillData->m_pMapData+nRow-1;
	// This code may be used in compiled form in any way you 

	switch(m_pColData[nCol].nID)
	{
		case 1:                                      
              return 0;
			break;
		case 2:                                      
              return float(pCjsj->ntime);
			break;
		case 3: 				
			return float(pCjsj->ntype);
			break;
		case 4:                                
			return pCjsj->nowv ;
			break;
		case 5:                                
			return pCjsj->nowp  ;
			break;
		case 6:                                 
              return pCjsj->rdr ;
			break;
		}
	return 0;
}
BOOL CBGridCtrl::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
    if (nCol >= GetColumnCount()) return FALSE;//大图象列表

    if (high == -1) high =m_nRealRow-1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

	double niMid=GetItemTextV((lo+hi)/2, nCol);
    double nistr;

    while( lo <= hi )
    {
       
        if( bAscending )
		{
            nistr=GetItemTextV(lo, nCol);
            while (lo < high && nistr<niMid)
			{
 				++lo;
  				nistr=GetItemTextV(lo, nCol);
			}
		}
        else
		{
            nistr=GetItemTextV(lo, nCol);
            while ((lo < high  && nistr>niMid))
			{
 				++lo;
				nistr=GetItemTextV(lo, nCol);
			}
		
		}

        if( bAscending )
		{
 			nistr=GetItemTextV(hi, nCol);
			while (hi > low && nistr>niMid)
			{
 				--hi;
   				nistr=GetItemTextV(hi, nCol);
			}
		}
        else
		{
			nistr=GetItemTextV(hi,nCol);
            while (hi > low && nistr<niMid)
			{
                --hi;
				nistr=GetItemTextV(hi, nCol);
			}
		}
       
        if (lo <= hi)
        {
          
            if (GetItemTextV(lo, nCol) != GetItemTextV(hi, nCol))
            {
				SwapMx(m_pDoc->m_pStockBigBillData->m_pMapData[lo -1],m_pDoc->m_pStockBigBillData->m_pMapData[hi -1] );
				UINT nRowHeight = m_arRowHeights[lo];
                m_arRowHeights[lo] = m_arRowHeights[hi];
                m_arRowHeights[hi] = nRowHeight;
            }
            ++lo;
            --hi;
        }
    }

    
    if( low < hi )// This code may be used in compiled form in any way you 

        SortTextItems(nCol, bAscending, low, hi);

   
    if( lo < high )
        SortTextItems(nCol, bAscending, lo, high);

    return TRUE;
}
void CBGridCtrl::QuickSortString()
{
    QuickSortStringMx(m_pDoc->m_pStockBigBillData->m_pMapData,TRUE,0,m_nRealRow-2);
}
CString CBGridCtrl::GetTimeGrid(time_t ntime)const
{
  CTime m_Time = ntime;
  return m_Time.Format("%Y%m%d%H%M");
}
void CBGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    HWND hOldFocusWnd = ::GetFocus();

	nFlags=1;
	BOOL m_IsSort=FALSE;
	if(m_SortColumn!=-1)
       m_IsSort=TRUE;
	if(m_idCurrentCell.row>0)
	   m_CurrSortCell=m_idCurrentCell ;
	m_CurrrSortClick=FALSE;
    m_BeforeRow=m_idCurrentCell.row;




    m_LeftClickDownPoint = point;
    m_LeftClickDownCell = GetCellFromPt(point);
    if (!IsValid(m_LeftClickDownCell)) return;

    m_SelectionStartCell = (nFlags & MK_SHIFT)? m_idCurrentCell : m_LeftClickDownCell;

    SetFocus();        
    if (m_LeftClickDownCell == m_idCurrentCell)
    {
        m_MouseMode = MOUSE_PREPARE_EDIT;
		return;
    } else {
        SetFocusCell(-1,-1);
        SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
                    max(m_LeftClickDownCell.col, m_nFixedCols));//Do not modify the contents of this file.
    }

   
    if (m_bAllowDragAndDrop && hOldFocusWnd == GetSafeHwnd() && 
        GetItemState(m_LeftClickDownCell.row, m_LeftClickDownCell.col) & GVNI_SELECTED)
    {
        m_MouseMode = MOUSE_PREPARE_DRAG;
        return;
    }


    SetCapture();
// 股票列表视图的列变量定义，以及自定义列的表达式计算

    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) 
    {
        m_MouseMode = MOUSE_SIZING_COL;
        CPoint start;
        if (!GetCellOrigin(0, m_LeftClickDownCell.col, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);//Do not modify the contents of this file.
        }

        if (point.x - start.x <= m_nResizeCaptureRange)       
            if (!GetCellOrigin(0, --m_LeftClickDownCell.col, &start)) return;

        rect.left = start.x;
        ClientToScreen(rect);
        ClipCursor(rect);
    }
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
    {
        m_MouseMode = MOUSE_SIZING_ROW;
        CPoint start;
        if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.y - start.y <= m_nResizeCaptureRange)           
            if (!GetCellOrigin(--m_LeftClickDownCell.row, 0, &start)) return;

        rect.top = start.y;
        ClientToScreen(rect);
        ClipCursor(rect);//Do not modify the contents of this file.
    }
    else 
    {    
        
		if (m_LeftClickDownCell.row < GetFixedRowCount())
		{
		
	        m_CurrrSortClick=TRUE;
            OnFixedRowClick(m_LeftClickDownCell);// NOTE: the ClassWizard will add member functions here
		}
        else if (m_LeftClickDownCell.col < GetFixedColumnCount())
		{
	        m_CurrrSortClick=FALSE;
			OnFixedColumnClick(m_LeftClickDownCell);
		}
        else
        {
	        m_CurrrSortClick=FALSE;// NOTE: the ClassWizard will add member functions here
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            OnSelecting(m_LeftClickDownCell);
        }

        m_nTimerID = SetTimer(WM_LBUTTONDOWN, m_nTimerInterval, 0);
    }   
    m_LastMousePoint = point;
    m_CurrSortCell=m_idCurrentCell ;
}
