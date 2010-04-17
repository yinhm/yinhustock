// DiaCopy.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DiaCopy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDiaCopy::CDiaCopy(CWnd* pParent /*=NULL*/)
	: CDialog(CDiaCopy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDiaCopy)
	m_static = _T("");
	//}}AFX_DATA_INIT
}


void CDiaCopy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDiaCopy)
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_STATIC1, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiaCopy, CDialog)
	//{{AFX_MSG_MAP(CDiaCopy)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDiaCopy::OnInitDialog() 
{
	CDialog::OnInitDialog();
    this->m_progress.SetRange(0,100);
	this->m_progress.SetPos(0);	
	CenterWindow(CWnd::GetDesktopWindow());
	return TRUE;  
}

BOOL CDiaCopy::FileCopy(char *filedes,char *filesrc)
{
    this->m_progress.SetPos(0);
	CFileFind finder;
	CString filename;
	BOOL bWorking = finder.FindFile(filesrc);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		filename = finder.GetFilePath();
	}
    if(filename.GetLength() == 0)
	{
	    filename.Format("文件 %s 没有找到",filesrc);
		MessageBox(filename);
		return false;
	}  
	CFile Filesrc(filename,CFile::modeRead|CFile::shareDenyNone);
	long length = Filesrc.GetLength();
	
	 CFile Filedec(filedes,CFile::modeCreate|CFile::modeWrite);
    
	
	long  block = long(length/99);
	char *buf;
	buf = new char[block];
	for(int i = 0;i<100;i++)
	{
	  Filesrc.Read(buf,block);
	  Filedec.Write(buf,block);
	  this->m_progress.SetPos(i);
	  this->m_static.Format("正在备份文件%d%%",i);
	  UpdateData(FALSE);
	}
    
	Filesrc.Read(buf,length - block*99);
	Filedec.Write(buf,length - block*99);
	this->m_progress.SetPos(100);
    this->m_static.Format("100%%");
	UpdateData(FALSE);
	this->m_static.Format("备份完成");
	UpdateData(FALSE);
	return true;
}

void CDiaCopy::OnClose() 
{	
	CDialog::OnClose();
}
