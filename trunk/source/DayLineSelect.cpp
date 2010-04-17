// DayLineSelect.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DayLineSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDayLineSelect dialog


CDayLineSelect::CDayLineSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDayLineSelect::IDD, pParent)
{
	m_DayLineTime=0;
}

CDayLineSelect::CDayLineSelect(UINT uiSelectDayLineType,CWnd* pParent /*=NULL*/)
	: CDialog(CDayLineSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDayLineSelect)
	
	m_DayLineTime=0;
	//}}AFX_DATA_INIT
	m_uiSelectDayLineType=uiSelectDayLineType;
}

void CDayLineSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDayLineSelect)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	 DDX_Radio(pDX, ID_DAYLINE,m_nDayLineType);
 	 DDX_Control(pDX, ID_DAYLINE_LIST, m_MyList);
	 DDX_Control(pDX, IDC_MONTHCALENDAR1, m_MonthCalCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDayLineSelect, CDialog)
	//{{AFX_MSG_MAP(CDayLineSelect)
	ON_BN_CLICKED(ID_DAYLINE, OnChangeDayLineType)
	ON_BN_CLICKED(ID_MONTHLINE, OnChangeDayLineType)
	ON_BN_CLICKED(ID_YEARLINE, OnChangeDayLineType)
	ON_BN_CLICKED(IDOK, OnAddDownLoadData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CDayLineSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->CenterWindow();
	m_pImageListSmall = new CImageList();
	ASSERT( m_pImageListSmall != NULL);    
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	this->m_MyList.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	m_nDayLineType=m_uiSelectDayLineType;

    switch(m_nDayLineType)
	{
	case 0:
		{
			CWnd *pWnd=this->GetDlgItem(ID_DAYLINE_LIST);
            pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow(SW_HIDE);
			pWnd=this->GetDlgItem(IDC_MONTHCALENDAR1);
            pWnd->EnableWindow(TRUE);
			pWnd->ShowWindow(SW_SHOW);
		}
		break;
    case 1:
		{
			CWnd *pWnd=this->GetDlgItem(IDC_MONTHCALENDAR1);
            pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow(SW_HIDE);
			pWnd=this->GetDlgItem(ID_DAYLINE_LIST);
            pWnd->EnableWindow(TRUE);
			pWnd->ShowWindow(SW_SHOW);
			InsertDayLineToList(TRUE);
		}
		break;
	case 2:
		{
			CWnd *pWnd=this->GetDlgItem(IDC_MONTHCALENDAR1);
            pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow(SW_HIDE);
			pWnd=this->GetDlgItem(ID_DAYLINE_LIST);
            pWnd->EnableWindow(TRUE);
			pWnd->ShowWindow(SW_SHOW);
			InsertDayLineToList(FALSE);
		}
		break;
	}
	this->UpdateData(FALSE);
	return TRUE;  
}
void CDayLineSelect::InsertDayLineToList(BOOL IsMonthLine)
{
    CTime  Time_Now = CTime::GetCurrentTime();
    if(!IsMonthLine)
	{
		char szData[10];
		for(int i=1990;i<Time_Now.GetYear();i++)
		{
			sprintf(szData,"%d",i);
			InsertDateToList(szData);
		}
	}
	else
	{
		char szData[10];
		for(int i=Time_Now.GetMonth();i<=12;i++)
		{
            sprintf(szData,"%d%2d",Time_Now.GetYear()-1,i);
		    for(int j=0;j<8;j++)
			{
				if(szData[j]==' ')
				   szData[j]='0';
			}
			szData[8]='\0';	
			InsertDateToList(szData);
		}
		for(int i=1;i<=Time_Now.GetMonth();i++)
		{
            sprintf(szData,"%d%2d",Time_Now.GetYear(),i);
		    for(int j=0;j<8;j++)
			{
				if(szData[j]==' ')
				   szData[j]='0';
			}
			szData[8]='\0';	
			InsertDateToList(szData);
		}
	}
}
void CDayLineSelect::FillToListCtrlHead()
{ 
   	CRect           rect;
	CString         strItem1= _T("下载文件");


	this->m_MyList.GetWindowRect(&rect);
	this->m_MyList.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1, 0);
}
BOOL CDayLineSelect::InsertDateToList(char *szData)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LPTSTR   pStrTemp1;
    m_MyList.SetColumnWidth( 0, 60 );
	iItem=this->m_MyList.GetItemCount();
	for (iSubItem = 0; iSubItem <1; iSubItem++)
	{
		if (iSubItem == 0)
			iIcon = 0;  

		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM  : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

	
		CString strIconDesc, strIconShortDesc;
        strIconShortDesc=szData;
		pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		lvitem.pszText = pStrTemp1;

		lvitem.iImage = iIcon;
		iActualItem =this->m_MyList.InsertItem(&lvitem); 
	} 
	return TRUE;
}
void CDayLineSelect::OnChangeDayLineType()
{
	this->UpdateData();
	m_MyList.DeleteAllItems();
    switch(m_nDayLineType)
	{
	case 0:
		{
			CWnd *pWnd=this->GetDlgItem(ID_DAYLINE_LIST);
            pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow(SW_HIDE);
			pWnd=this->GetDlgItem(IDC_MONTHCALENDAR1);
            pWnd->EnableWindow(TRUE);
			pWnd->ShowWindow(SW_SHOW);
		}
		break;
    case 1:
		{
			CWnd *pWnd=this->GetDlgItem(IDC_MONTHCALENDAR1);
            pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow(SW_HIDE);
			pWnd=this->GetDlgItem(ID_DAYLINE_LIST);
            pWnd->EnableWindow(TRUE);
			pWnd->ShowWindow(SW_SHOW);
			InsertDayLineToList(TRUE);
		}
		break;
	case 2:
		{
			CWnd *pWnd=this->GetDlgItem(IDC_MONTHCALENDAR1);
            pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow(SW_HIDE);
			pWnd=this->GetDlgItem(ID_DAYLINE_LIST);
            pWnd->EnableWindow(TRUE);
			pWnd->ShowWindow(SW_SHOW);
			InsertDayLineToList(FALSE);
		}
		break;
	}
  
}
void CDayLineSelect::OnAddDownLoadData()
{
	this->UpdateData();
	ADDDAYLINEDATA *pAddDayLine=NULL;
	HGLOBAL hMem;
	LPVOID  hp;

    switch(m_nDayLineType)
	{
	case 0:
		{
			CTime lStartTime,lEndTime;
			m_MonthCalCtrl.GetSelRange( lStartTime,lEndTime );
            CTimeSpan  lTimeSpan(lEndTime.GetTime()- lStartTime.GetTime( ) );
			CTimeSpan  lDaySpan(1, 0,0, 0 );
			time_t tTemp;

            int listcount=lTimeSpan.GetDays()+1;
			hMem = GlobalAlloc( GPTR, ( listcount)* sizeof( ADDDAYLINEDATA) );
			hp=GlobalLock(hMem);
			if( hp )
			{
				pAddDayLine= (ADDDAYLINEDATA *)hp;
			}
			else
			{
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return;
			}
            tTemp=lStartTime.GetTime();
			int index=0;
			while(tTemp<=lEndTime.GetTime())
			{
				CTime  Time_Now(tTemp);
				char szDate[10];
				sprintf(szDate,"%d%2d%2d",Time_Now.GetYear(),Time_Now.GetMonth(),Time_Now.GetDay());
				for(int i=0;i<8;i++)
				{
					if(szDate[i]==' ')
					   szDate[i]='0';
				}
				szDate[8]='\0';	
			    pAddDayLine[index].DayLineType=0;
				strcpy(pAddDayLine[index].DayLineName,szDate);
				index++;

                tTemp +=lDaySpan.GetTotalSeconds( ) ;
			}
			CWnd *pWnd=this->GetParent();
			::SendMessage(pWnd->m_hWnd,WM_USER_ADDDAYLINE,(long )pAddDayLine,listcount);
		}
		break;
    case 1:
		{
			int listcount=m_MyList.GetSelectedCount( ) ;
			hMem = GlobalAlloc( GPTR, ( listcount)* sizeof( ADDDAYLINEDATA) );
			hp=GlobalLock(hMem);
			if( hp )
			{
				pAddDayLine= (ADDDAYLINEDATA *)hp;
			}
			else
			{
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return;
			}
			int temp=m_MyList.GetNextItem( -1, LVNI_SELECTED) ;
			int index=0;
			while(temp>=0)
			{
			   pAddDayLine[index].DayLineType=1;
			   m_MyList.GetItemText( temp, 0, pAddDayLine[index].DayLineName, 10);
			   temp=m_MyList.GetNextItem( temp, LVNI_SELECTED) ;
			   index++;
			}
			CWnd *pWnd=this->GetParent();
			::SendMessage(pWnd->m_hWnd,WM_USER_ADDDAYLINE,(long )pAddDayLine,listcount);
			m_MyList.SetSelectionMark(-1 );
		}
		break;
	case 2:
		{
			int listcount=m_MyList.GetSelectedCount( ) ;
			hMem = GlobalAlloc( GPTR, ( listcount)* sizeof( ADDDAYLINEDATA) );
			hp=GlobalLock(hMem);
			if( hp )
			{
				pAddDayLine= (ADDDAYLINEDATA *)hp;
			}
			else
			{
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return;
			}
			int temp=m_MyList.GetNextItem( -1, LVNI_SELECTED) ;
			int index=0;
			while(temp>=0)
			{
			   pAddDayLine[index].DayLineType=2;
			   m_MyList.GetItemText( temp, 0, pAddDayLine[index].DayLineName, 10);
			   temp=m_MyList.GetNextItem( temp, LVNI_SELECTED) ;
			   index++;
			}
			CWnd *pWnd=this->GetParent();
			::SendMessage(pWnd->m_hWnd,WM_USER_ADDDAYLINE,(long )pAddDayLine,listcount);
			m_MyList.SetSelectionMark(-1 );
		}
		break;
	}
	if(pAddDayLine)
	{
	   GlobalUnlock(pAddDayLine);       
	   GlobalFree(pAddDayLine);
	}
}
