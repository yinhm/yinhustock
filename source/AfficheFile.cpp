// AfficheFile.cpp: implementation of the CAfficheFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "AfficheFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXHEADLEN		60	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAfficheFile::CAfficheFile(LPCTSTR lpsz)
{
		sFileName	= lpsz;
		m_bOpen	= FALSE;
}

CAfficheFile::~CAfficheFile()
{
	OnRemoveFileAll();
	if(m_bOpen) m_File.Close();
}

BOOL CAfficheFile::OnOpenFile()
{
	CString sPath = sFileName;

	CFileFind finder;
	if(!finder.FindFile(sPath))
	{
		m_File.Open(sPath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	}
	else
	{
		try
		{
			m_File.Open(sPath, CFile::modeReadWrite | CFile::typeBinary);
		}
		catch(CFileException *e)
		{
			AfxMessageBox("NOT");
			return FALSE;
		}
	}

	m_bOpen = TRUE;
	return TRUE;
}

void CAfficheFile::OnAddNew(char* lpsz, int size)
{
	ASSERT(lpsz != NULL);
	if(!OnOpenFile()) return;
	
	m_File.SeekToEnd();
	m_File.Write(&size, sizeof(int));
	m_File.Write(lpsz, size);

	m_File.Close();
	m_bOpen = FALSE;
}

void CAfficheFile::OnDeleteAll()
{
	if(!OnOpenFile()) return;
	m_File.Remove(sFileName);

	m_File.Close();
	m_bOpen = FALSE;
}

void CAfficheFile::OnRefashFile()
{
	if(!OnOpenFile()) return;
	OnRemoveFileAll();


	int size;
	/////////////////////////////////////////////////////////////////
	m_File.SeekToBegin();
	for(int i = 0; ; i++)
	{
		PAFFICHEFILE pAffiche = new AFFICHEFILE;

		if(m_File.Read(&size, sizeof(int)) != 0)
		{
			char *lp = (char *)malloc(sizeof(char)*(size+1));
			memset(lp, '\0', size + 1);
			m_File.Read(lp, size);
			pAffiche = OnTransStruct(lp, size);
			free(lp);
			m_FileMap[i] = pAffiche;
		}
		else 
		{
			if(pAffiche) delete pAffiche;
			break;
		}
	}

	m_File.Close();
	m_bOpen = FALSE;
}

PAFFICHEFILE CAfficheFile::OnTransStruct(char *lp, int size)
{
	PAFFICHEFILE pAff = new AFFICHEFILE;
	CString str = lp;
	pAff->sContext = str.Left(size);

	pAff->sCode	   = _T("");
	CString s = 0x20;
	int head_count = str.FindOneOf(s);
	if(head_count == -1) head_count = size;
	if(head_count > MAXHEADLEN)	
	{
		head_count = MAXHEADLEN;
		pAff->sHead = str.Left(head_count);
		pAff->sHead += "......";
	}
	{
		pAff->sHead = str.Left(head_count - 4);
		CString str('.', MAXHEADLEN - head_count + 4 );
		pAff->sHead += str;
	}

	int nstart	= str.FindOneOf("(");
	int nend	= str.FindOneOf(")");

	if(nstart != -1 && nend != -1 && nend > nstart)
	{
		pAff->sCode = str.Mid(nstart + 1, nend - nstart - 1);
	}

	
	return pAff;
}

void CAfficheFile::SetFileName(LPCTSTR lpsz)
{
	sFileName = lpsz;
}

void CAfficheFile::OnRemoveFileAll ()
{
	if(!m_FileMap.IsEmpty())
	{
		for(int i= 0; i< m_FileMap.GetCount(); i++)
		{
			PAFFICHEFILE pAffiche = NULL;
			if(m_FileMap.Lookup(i, pAffiche))
			{
				delete pAffiche;
				pAffiche = NULL;
			}
		}
		m_FileMap.RemoveAll();
	}
}