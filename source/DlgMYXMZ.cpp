// DlgMYXMZ.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DlgMYXMZ.h"
#include "Stockdrv.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDialogMYXMZ::CDialogMYXMZ(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMYXMZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMYXMZ)
	
	//}}AFX_DATA_INIT
	
	
}


void CDialogMYXMZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMYXMZ)
	DDX_Control(pDX, IDC_MYXMZLIST, m_mylist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMYXMZ, CDialog)
	//{{AFX_MSG_MAP(CDialogMYXMZ)
	ON_MESSAGE(WM_MYMSG,DealMsg)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CDialogMYXMZ::DealMsg(WPARAM wParam,LPARAM lParam)
{
	
	int i=0;
	PBYTE pFileBuf = NULL;
	RCV_REPORT_STRUCTEx Buf;
	RCV_DATA *	pHeader;
	CString m_FileName;      
	pHeader = (RCV_DATA *) lParam;
    CString m_File;
	switch( wParam )
	{
	case RCV_REPORT: 	
			
		if(pHeader->m_pReport[i].m_cbSize ==SH_MARKET_EX||pHeader->m_pReport[i].m_cbSize ==SZ_MARKET_EX)//如果是老格式，则进行转化
			{CTime tm;
			CString str;
			
			tm=CTime::GetCurrentTime();
			str=tm.Format("%H:%M:%S");
		
			m_mylist.InsertItem(0,str,0);
			m_mylist.SetItemText(0,1,"实时行情");
				RCV_REPORT_STRUCTEx_OLD *pMyBuf;
				pMyBuf=(RCV_REPORT_STRUCTEx_OLD*)pHeader->m_pReport;
				m_mylist.SetItemText(0,2,pMyBuf[0].m_szLabel);
				m_mylist.SetItemText(0,3,pMyBuf[0].m_szName);
		}
		else
		{
			CTime tm;
			CString str;
		
			tm=CTime::GetCurrentTime();
			str=tm.Format("%H:%M:%S");
			
			m_mylist.InsertItem(0,str,0);
			m_mylist.SetItemText(0,1,"实时行情");
			memset(&Buf,0,sizeof(Buf));
				BYTE *totalmem;
				totalmem=(BYTE *)pHeader->m_pReport;
				BYTE mymem[142];
				memcpy(mymem,totalmem,142);
				char sym[10];
				char name[32];
				
				memcpy(sym,mymem+8,10);
				memcpy(name,mymem+18,32);
			
				strcpy(Buf.m_szLabel,sym);
				strcpy(Buf.m_szName,name);
			
			m_mylist.SetItemText(0,2,Buf.m_szLabel);
			m_mylist.SetItemText(0,3,Buf.m_szName);

			
		}
				
		break;
	case RCV_FILEDATA:
	    if( !pHeader->m_pData || pHeader->m_wDataType == FILE_TYPE_RES)
		{
			TRACE("MSG: CStkDemo::OnStkDataOK, Replaced data \n");
			break;
		}
		int index=0;
		CString StockId;	
		switch(pHeader->m_wDataType)
		{
		case FILE_HISTORY_EX:			
	    case FILE_POWER_EX:
		case FILE_MINUTE_EX:				         		
			do
			{
				if(pHeader->m_pMinute[index].m_time==EKE_HEAD_TAG)
				{
					StockId=pHeader->m_pMinute[index].m_head.m_szLabel;			
					CTime tm;
					CString str;
					int nRow;
					tm=CTime::GetCurrentTime();
					str=tm.Format("%H:%M:%S");
					
					m_mylist.InsertItem(0,str,1);
					if(pHeader->m_wDataType == FILE_HISTORY_EX)
						m_mylist.SetItemText(0,1,"补日K线");
					else
						m_mylist.SetItemText(0,1,"补分时线");
					m_mylist.SetItemText(0,2,StockId);
					m_mylist.SetHotItem(0);
				}
				index++;				 
			}while(index<pHeader->m_nPacketNum)	;	

			break;

		case FILE_BASE_EX:	
		case FILE_NEWS_EX:					
    		m_File.Format("%s",pHeader->m_File.m_szFileName);
		          
			CMainFrame* pMf = (CMainFrame* )AfxGetMainWnd();
			if(pMf->m_taiShanDoc->m_NewsFileNameMap.Lookup( m_File, m_FileName ) != TRUE)
			{
					CTime tm;
					CString str;

					tm=CTime::GetCurrentTime();
					str=tm.Format("%H:%M:%S");
					
					m_mylist.InsertItem(0,str,2);
					if(pHeader->m_wDataType==FILE_BASE_EX)
					{
						m_mylist.SetItemText(0,1,"基本资料");
					}
					else
					{
						m_mylist.SetItemText(0,1,"新闻");
					}
					m_mylist.SetItemText(0,2,m_File);
					m_mylist.SetHotItem(0);
			}		
			break;
		}
	}

	// 删除一些行
	while (m_mylist.GetItemCount() > 1000)
	{
		for (int i = 0; i > 100 || m_mylist.GetItemCount() > 900; i++)
			m_mylist.DeleteItem(m_mylist.GetItemCount() - 1);
	}

	return 0;
}

BOOL CDialogMYXMZ::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIcon;
	CTaiShanApp* pApp = (CTaiShanApp*)AfxGetApp();
	hIcon = pApp->LoadIcon(IDI_MYXMZ);
	SetIcon(hIcon, FALSE);

	m_mylist.SetImageList(&m_imagelist, LVSIL_SMALL);  

	m_mylist.InsertColumn(0, "时间", LVCFMT_LEFT, 80);
	m_mylist.InsertColumn(1, "类型", LVCFMT_LEFT, 80);
	m_mylist.InsertColumn(2, "股票代码", LVCFMT_LEFT, 80);
	m_mylist.InsertColumn(3, "股票名称", LVCFMT_LEFT, 100);
	SetWindowLong(m_mylist.m_hWnd, GWL_STYLE, WS_VISIBLE|WS_CHILD|WS_BORDER|LVS_REPORT|LVS_EDITLABELS);

	return TRUE;  
}
