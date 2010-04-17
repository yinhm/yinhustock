// CTaiKlineDlgHistorySelect.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiKlineDlgHistorySelect.h"
#include "CTaiKlineFileHS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgHistorySelect dialog


CTaiKlineDlgHistorySelect::CTaiKlineDlgHistorySelect(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgHistorySelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgHistorySelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_fileName = _T("");
}


void CTaiKlineDlgHistorySelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgHistorySelect)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_LIST1, m_fileNameList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgHistorySelect, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgHistorySelect)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgHistorySelect message handlers

BOOL CTaiKlineDlgHistorySelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	
	CStringArray sArray;

	GetFileNameArray(sArray);

	int n = sArray.GetSize ();
	for(int i=0;i<n;i++)
	{
		m_fileNameList.InsertItem (i,sArray[n-i-1]);
	}
	if(sArray.GetSize ()>0)
		m_fileNameList.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED  );
;
	
	return TRUE;  
}

void CTaiKlineDlgHistorySelect::OnOk() 
{

	m_fileName = "";
	POSITION pos = m_fileNameList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	   return;
	else
	{  

		{
			  int nItem = m_fileNameList.GetNextSelectedItem(pos);
			  m_fileName = m_fileNameList.GetItemText(nItem,0);
			  TRACE1("Item %d was selected!\n", nItem);
		}
	}
	CDialog::OnOK();
}

void CTaiKlineDlgHistorySelect::GetFileNameArray(CStringArray &sArry)
{
	CString sPath = "data\\historysh\\";
	CFileFind finder;
	int n = 0;
	BOOL bWorking = finder.FindFile(sPath+"*.hst");
	while(bWorking)
	{
	  bWorking = finder.FindNextFile();
	  CString filename = finder.GetFileTitle();
	  int nSize = sArry.GetSize ();
	  for(int j=0;j<nSize;j++)
	  {
		  if(filename <sArry[j])
		  {
			  sArry.InsertAt (j,filename);
			  break;
		  }
		  if(j==nSize-1)
			sArry.Add (filename);
	  }
	  if(nSize == 0)
			sArry.Add (filename);

	  n++;
	}
	sPath = "data\\historysz\\";
	n = 0;
	bWorking = finder.FindFile(sPath+"*.hst");
	while(bWorking)
	{
	  bWorking = finder.FindNextFile();
	  CString filename = finder.GetFileTitle();
	  int nSize = sArry.GetSize ();
	  BOOL bf;
	  bf=false;
	  for(int j=0;j<nSize;j++)
	  {
		  if(filename ==sArry[j])
		  {
			  bf=true;
			  
			  break;
			  
		  }
		  else
		  {
			  bf=false;
			  continue;
		  }
		  if(j==nSize-1)
			sArry.Add (filename);
	  }
	  if((nSize == 0)||(!bf))
			sArry.Add (filename);
	  n++;
	}
}

void CTaiKlineDlgHistorySelect::OnDestroy() 
{
	CDialog::OnDestroy();
	

	
}


CTaiKlineHistorySelect::CTaiKlineHistorySelect() 
{
}
CTaiKlineHistorySelect::~CTaiKlineHistorySelect() 
{
	CloseAll();
}

bool CTaiKlineHistorySelect::OpenAll()
{
	ASSERT(FALSE);
	return false;

	CloseAll();

	CTime tm = CTime::GetCurrentTime ();
	CString sTm = tm.Format ("%Y%m%d");

	bool bToday = false;

	for(int i=0;i<m_fileNameArray.GetSize ();i++)
	{
		CString sPathSh;
		CString sPathSz;
		CString filename ;
		CString sTitle = m_fileNameArray[i];
		CString title = sTitle+".hst";
		sPathSh = "data\\historysh\\";
		sPathSz = "data\\historysz\\";
		CTaiKlineFileHS* pFile;
		if(m_fileNameArray[i]>=sTm)
		{
			if(bToday == false)
			{
				sPathSh = "data\\historysh\\";
				sPathSz = "data\\historysz\\";
				title = "buysell.dat";
				sTitle = "buysell";
				bToday = true;
			}
			else
				continue;
		}
		pFile = new CTaiKlineFileHS(true);


		filename =sPathSh + title;
		if(!pFile->Open (filename,0))
		{
			delete pFile;
			continue;
		}
		m_fileHsShArray[sTitle] = (pFile);

		pFile = new CTaiKlineFileHS(false);
		filename =sPathSz + title;
		if(!pFile->Open (filename,0))
		{
			delete pFile;
			continue;
		}
		m_fileHsSzArray[sTitle] = (pFile);

	}
	return true;
}

void CTaiKlineHistorySelect::CloseAll()
{
	POSITION pos=m_fileHsShArray.GetStartPosition ();
	CString s;
	CTaiKlineFileHS* pHs;
    while(pos)
	{
		m_fileHsShArray.GetNextAssoc (pos,s,pHs);
        if(pHs !=NULL) delete pHs;
	}
    m_fileHsShArray.RemoveAll();

	pos=m_fileHsSzArray.GetStartPosition ();
    while(pos)
	{
        m_fileHsSzArray.GetNextAssoc (pos,s,pHs);
        if(pHs !=NULL) delete pHs;
	}
    m_fileHsSzArray.RemoveAll();
}

int CTaiKlineHistorySelect::FilterFileName(CTime &begin, CTime &end)
{
	int n=0;
	m_fileNameArray.RemoveAll ();
	CTaiKlineDlgHistorySelect::GetFileNameArray(this->m_fileNameArray );

	CString sBegin = begin.Format ("%Y%m%d");
	CString sEnd = end.Format ("%Y%m%d");

	for(int i=0;i<m_fileNameArray.GetSize ();i++)
	{
		if(m_fileNameArray[i]<sBegin||m_fileNameArray[i]>sEnd)
		{
			m_fileNameArray.RemoveAt (i);
			i--;
		}
	}

	return n;

}


void CTaiKlineDlgHistorySelect::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	OnOk() ;
	*pResult = 0;
}
