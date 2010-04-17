// LmbDlgHistorySelect.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "LmbDlgHistorySelect.h"
#include "LmbFileHS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LmbDlgHistorySelect dialog


LmbDlgHistorySelect::LmbDlgHistorySelect(CWnd* pParent /*=NULL*/)
	: CDialog(LmbDlgHistorySelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(LmbDlgHistorySelect)
		
	//}}AFX_DATA_INIT
	m_fileName = _T("");
}


void LmbDlgHistorySelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LmbDlgHistorySelect)
	DDX_Control(pDX, IDC_LIST1, m_fileNameList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LmbDlgHistorySelect, CDialog)
	//{{AFX_MSG_MAP(LmbDlgHistorySelect)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LmbDlgHistorySelect message handlers

BOOL LmbDlgHistorySelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	
	CStringArray sArray;

	GetFileNameArray(sArray);

	for(int i=0;i<sArray.GetSize ();i++)
	{
		m_fileNameList.InsertItem (i,sArray[i]);
	}
	
	return TRUE;  
}

void LmbDlgHistorySelect::OnOk() 
{

	m_fileName = "";
	POSITION pos = m_fileNameList.GetFirstSelectedItemPosition();
	if (pos == NULL)
	   TRACE0("No items were selected!\n");
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

void LmbDlgHistorySelect::GetFileNameArray(CStringArray &sArry)
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

}

void LmbDlgHistorySelect::OnDestroy() 
{
	CDialog::OnDestroy();
	

	
}
LmbHistoryFileSelect::LmbHistoryFileSelect() 
{
}
LmbHistoryFileSelect::~LmbHistoryFileSelect() 
{
	CloseAll();
}

bool LmbHistoryFileSelect::OpenAll()
{
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
		LmbFileHS* pFile;
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
		pFile = new LmbFileHS(true);


		filename =sPathSh + title;
		if(!pFile->Open (filename,0))
		{
			delete pFile;
			continue;
		}
		m_fileHsShArray[sTitle] = (pFile);

		pFile = new LmbFileHS(false);
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

void LmbHistoryFileSelect::CloseAll()
{
	POSITION pos=m_fileHsShArray.GetStartPosition ();
	CString s;
	LmbFileHS* pHs;
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

int LmbHistoryFileSelect::FilterFileName(CTime &begin, CTime &end)
{
	int n=0;
	m_fileNameArray.RemoveAll ();
	LmbDlgHistorySelect::GetFileNameArray(this->m_fileNameArray );

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
