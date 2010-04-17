// CSharesDetailGridCtrl.cpp: implementation of the CSharesDetailGridCtrl class.
// by ÓáÃ÷Â¼
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineTransferKline.h"
#include "CSharesDetailGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharesDetailGridCtrl::CSharesDetailGridCtrl()
{

}

CSharesDetailGridCtrl::~CSharesDetailGridCtrl()
{
	m_nRealRow=0;
}
BOOL CSharesDetailGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    if(nRow==0)
	{
        pGridCell->state=0;      
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS; 
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
    if((m_idCurrentCell.row==nRow))
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
	CReportData *pCdat;
   	char str[50];
	if(!m_pDoc->m_sharesInformation.Lookup(m_pDoc->m_sharesSymbol.GetBuffer(0),pCdat,m_pDoc->m_stkKind))
		return TRUE;
    UINT ColumnNumber=m_pColData[nCol].nID;
	POSITION pos;
	TRADE_DETAIL_H_PER* pHs=NULL,* pHsPre=NULL;
	FENJIA* pFenJia=NULL;
	int lTotalCount=m_pView->m_hs.GetCount();
	if(this->m_bIsF1)
	{
		pos=m_pView->m_hs.FindIndex(lTotalCount-(nRow-1)-1) ;
		pHs=m_pView->m_hs.GetAt( pos );
		pHsPre = NULL;
		if(nRow-1>0)
		{
			pos=m_pView->m_hs.FindIndex(lTotalCount-(nRow-2)-1  ) ;
			pHsPre=m_pView->m_hs.GetAt( pos );
		}
		if(pHs==NULL)
			return TRUE;
	}
	else
	{
	      pos = m_pView->m_fenjia.FindIndex(nRow-1);
          pFenJia = m_pView->m_fenjia.GetAt( pos );
          ColumnNumber +=4;
		  if(pFenJia==NULL)
			  return TRUE;
	}
   	switch(ColumnNumber)
	{
	case 0:   GetCurrentDate(pHs->time,pGridCell->szText);
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
		    break;
	case 1:   sprintf(str,"%6.2f",pHs->price);
			  if(pHs->price>pCdat->ystc )
				  pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pHs->price < pCdat->ystc)
				  pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			  else
				  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
		      break;
	case 2:   sprintf(str,"%6.2f",pHs->price - pCdat->ystc);
		      if(pHs->price>pCdat->ystc )
				  pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pHs->price < pCdat->ystc)
				  pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			  else
				  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
		    break;
	case 3: {
                int nFlag ;
			    float fVol = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
				if(nFlag>0)
				{
					   sprintf(str,"%6.2f",fVol);
					   pGridCell->szText =str;
					   pGridCell->szText +=_T(" ¡ü");
					   pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				}
				else if(nFlag<0)
				{
					   sprintf(str,"%6.2f",fVol);
					   pGridCell->szText =str;
					   pGridCell->szText +=_T(" ¡ý");
					   pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				}
				else
				{ 
					   sprintf(str,"%6.2f",fVol);
					   pGridCell->szText =str;
					   pGridCell->szText +=_T(" *");
					   pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
				}
			}
		    break;
	case 4: if(pFenJia->price>pCdat->ystc )
			    pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pFenJia->price < pCdat->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
		    else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
            sprintf(str,"%6.2f",pFenJia->price);
			pGridCell->szText =str;
	        break;
	case 5: if(pFenJia->price>pCdat->ystc )
			    pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pFenJia->price < pCdat->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
		    else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
            sprintf(str,"%6.2f",pFenJia->vol);
			pGridCell->szText =str;
	        break;
	}

   return TRUE;
}
BOOL CSharesDetailGridCtrl::SetRowCount(int nRows)
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
float CSharesDetailGridCtrl::GetItemTextV(int nRow,int nCol)
{
	return 0;
}
void CSharesDetailGridCtrl::GetCurrentDate(time_t t,CString &strDate) const
{
	char szDate[10];
	CTime ctTime(t);
	sprintf(szDate,"%2d:%2d",ctTime.GetHour(),ctTime.GetMinute());
	for(int i=0;i<5;i++)
	{
		if(szDate[i]==' ')
		   szDate[i]='0';
	}
	szDate[5]='\0';	
    strDate=szDate;   
}