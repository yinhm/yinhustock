// DetailGridCtrl.cpp: implementation of the CDetailGridCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "DetailGridCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDetailGridCtrl::CDetailGridCtrl()
{

}

CDetailGridCtrl::~CDetailGridCtrl()
{
	m_nRealRow=0;
}
BOOL CDetailGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
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
	CReportData *pCdat;
   	char str[50];
	if(!m_pDoc->m_sharesInformation.Lookup(m_pDoc->m_sharesSymbol.GetBuffer(0),pCdat,m_pDoc->m_stkKind))
		return TRUE;
	Rsdn1 **Nidx2,*tempNidx;
	m_pDoc->m_sharesInformation.GetIndexRsdn(Nidx2); 
    int which_stk;
	if(pCdat->kind==SHZS)
       which_stk=0;
	else if(pCdat->kind==SZZS)
       which_stk=1;
	else if(pCdat->kind==EBZS)
       which_stk=2;
	tempNidx=Nidx2[which_stk];
	int CurrRow;
	int PreRow;
	if(this->m_bIsF1)
	{
        if(nRow-1<=m_pDoc->m_nANT[which_stk])
	        CurrRow=nRow-1;
		else
	        CurrRow=m_pDoc->m_nANT[which_stk];
        PreRow= CurrRow -1;
		if(PreRow<0)
			PreRow=0;
	}
	else
	{
		if((nRow-1)*5<m_pDoc->m_nANT[which_stk])
		{
			if(nRow-1==0)
			{
              CurrRow=0;
              PreRow=0;
			}
			else
			{
              CurrRow=5*(nRow-1);
              PreRow=5*(nRow-2);
              if(PreRow<0) PreRow=0; 
			}
		}
		else
		{
			if(nRow-1==0)
			{
              CurrRow=0;
              PreRow=0;
			}
			else
			{
              CurrRow=m_pDoc->m_nANT[which_stk];
              PreRow=5*(nRow-2);
			}

		}
	}
   	switch(m_pColData[nCol].nID)
	{
	case 0:   GetCurrentDate(CurrRow,pGridCell->szText);
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
              break;
	case 1:   if(pCdat->m_Kdata1[CurrRow].Price==0&&CurrRow!=0)
			  {
		          sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow-1].Price);
				  if(pCdat->m_Kdata1[CurrRow-1].Price>pCdat->ystc )
					 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				  else if(pCdat->m_Kdata1[CurrRow-1].Price < pCdat->ystc)
					 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  }
		      else
			  {
		          sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Price);
				  if(pCdat->m_Kdata1[CurrRow].Price>pCdat->ystc )
					 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				  else if(pCdat->m_Kdata1[CurrRow].Price < pCdat->ystc)
					 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  }
			  pGridCell->szText =str;
              break;
	case 2:   if(pCdat->m_Kdata1[CurrRow].Price==0&&CurrRow!=0)
			  {
		          sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow-1].Price - pCdat->ystc);
				  if(pCdat->m_Kdata1[CurrRow-1].Price>pCdat->ystc )
					 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				  else if(pCdat->m_Kdata1[CurrRow-1].Price < pCdat->ystc)
					 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  }
		      else
			  {
		          sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Price - pCdat->ystc);
				  if(pCdat->m_Kdata1[CurrRow].Price>pCdat->ystc )
					 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				  else if(pCdat->m_Kdata1[CurrRow].Price < pCdat->ystc)
					 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  }
			  pGridCell->szText =str;
             break;
	case 3:  if(pCdat->ystc !=0)
			 {
				  if(pCdat->m_Kdata1[CurrRow].Price==0&&CurrRow>0)
				  {
					  sprintf(str,"%6.2f%s",(pCdat->m_Kdata1[CurrRow-1].Price - pCdat->ystc)/pCdat->ystc*100,"%");
					  if(pCdat->m_Kdata1[CurrRow-1].Price>pCdat->ystc )
						 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
					  else if(pCdat->m_Kdata1[CurrRow-1].Price < pCdat->ystc)
						 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
					  else
						 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
				  }
				  else
				  {
					  sprintf(str,"%6.2f%s",100*(pCdat->m_Kdata1[CurrRow].Price - pCdat->ystc)/pCdat->ystc,"%");
					  if(pCdat->m_Kdata1[CurrRow].Price>pCdat->ystc )
						 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
					  else if(pCdat->m_Kdata1[CurrRow].Price < pCdat->ystc)
						 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
					  else
						 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
				  }
				  pGridCell->szText =str;
			 }
			 else
               sprintf(str,"%6.2f",0.0f);

             break;
	case 4:  if(tempNidx[CurrRow].rsn!=0)
                 sprintf(str,"%d",tempNidx[CurrRow].rsn);
		     else
                 sprintf(str,"%d",tempNidx[CurrRow-1].rsn);
			 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			 pGridCell->szText =str;
             break;
	case 5:  if(tempNidx[CurrRow].dnn!=0)
                 sprintf(str,"%d",tempNidx[CurrRow].dnn);
		     else
                 sprintf(str,"%d",tempNidx[CurrRow-1].dnn);
			 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			 pGridCell->szText =str;
             break;
	case 6:   if(pCdat->m_Kdata1[CurrRow].Amount==0&&CurrRow>0)
		         sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow-1].Amount /10000 );
		      else
		         sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Amount /10000 );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
             break;
	case 7:   if(pCdat->m_Kdata1[CurrRow].Amount==0&&CurrRow>0)
		         sprintf(str,"%6.2f",0 );
		      else
			  {
                 if(CurrRow==0)
		            sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Amount );
                 else
		            sprintf(str,"%6.2f",(pCdat->m_Kdata1[CurrRow].Amount - pCdat->m_Kdata1[PreRow].Amount) /10000 );
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
             break;
	case 8:   if(pCdat->m_Kdata1[CurrRow].Volume==0&&CurrRow>0)
		         sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow-1].Volume);
		      else
		         sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Volume);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
             break;
	case 9:  if(pCdat->m_Kdata1[CurrRow].Volume==0&&CurrRow>0)
		         sprintf(str,"%6.2f",0);
		      else
			  {
                 if(CurrRow==0)
		           sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Volume);
                 else
		           sprintf(str,"%6.2f",pCdat->m_Kdata1[CurrRow].Volume - pCdat->m_Kdata1[PreRow].Volume);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
             break;
	}
    return TRUE;
}
BOOL CDetailGridCtrl::SetRowCount(int nRows)
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
float CDetailGridCtrl::GetItemTextV(int nRow,int nCol)
{
	return 0;
}
void CDetailGridCtrl::GetCurrentDate(int bMenutes,CString &strDate) const
{
    CTime m_NowTime = CTime::GetCurrentTime();
	CTimeSpan  lDaySpan(0, 0,1, 0 );
    long lYear=m_NowTime.GetYear();
	long lMouth=m_NowTime.GetMonth();
	long lDay=m_NowTime.GetDay();
	CTime m_NowTime930=CTime(lYear,lMouth,lDay,9,30,0);
    time_t tTemp=m_NowTime930.GetTime( );
	if(bMenutes<=120)
	{
        tTemp +=bMenutes*lDaySpan.GetTotalSeconds();
	}
	else
	{
        tTemp +=(bMenutes+90)*lDaySpan.GetTotalSeconds();
	}
	char szDate[10];
	CTime ctTime(tTemp);
	sprintf(szDate,"%2d:%2d",ctTime.GetHour(),ctTime.GetMinute());
	for(int i=0;i<5;i++)
	{
		if(szDate[i]==' ')
		   szDate[i]='0';
	}
	szDate[5]='\0';	
    strDate=szDate;   
}