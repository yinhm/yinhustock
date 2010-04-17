// BlockTypeManager.cpp: implementation of the BlockTypeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "BlockTypeManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static DWORD dwFileHead = 0x51ffa500;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BlockTypeManager::BlockTypeManager()
{
	m_sNameFileType = "block\\BLOCK.DEF";
}

BlockTypeManager::~BlockTypeManager()
{

}

int BlockTypeManager::AddBlockType(CString sName)
{
	CStringArray sTypeArr;
	EnumAllType(sTypeArr);
	for(int i=0;i<sTypeArr.GetSize ();i++)
	{
		if(sName.CompareNoCase (sTypeArr[i])==0)
			return 0;
	}
	CFile fl;
	int n2 = 0;
	if(fl.Open (m_sNameFileType,CFile::modeRead))
	{
		int n = fl.GetLength ();
		char * pch = new char[n+1];
		fl.Read (pch,n);
		pch[n] = 0;
		CString s = pch;

		int nStart = 0;
		sName = "["+sName;
		sName+="]";

		s.TrimRight ("\r\n");
		s+="\r\n";
		s+=sName;
		s+="\r\n";

		delete [] pch;
		fl.Close ();
		if(fl.Open (m_sNameFileType,CFile::modeWrite|CFile::modeCreate))
		{
			fl.Write (s.GetBuffer (s.GetLength ()),s.GetLength ());
			s.ReleaseBuffer ();
		}

		;
	}

	return n2;
}

int BlockTypeManager::ModifyBlockTypeName(CString sOldName, CString sNewName)
{
	CFile fl;
	int n2 = 0;
	if(fl.Open (m_sNameFileType,CFile::modeRead))
	{
		int n = fl.GetLength ();
		char * pch = new char[n+1];
		fl.Read (pch,n);
		pch[n] = 0;
		CString s = pch;

		int nStart = 0;
		sOldName = "["+sOldName;
		sOldName+="]";
		sNewName = "["+sNewName;
		sNewName+="]";
		while(true)
		{
			int nFind = s.Find (sOldName,nStart);
			if(nFind<0) break;
			n2 = s.Replace (sOldName,sNewName);
			
			fl.Close ();
			if(fl.Open (m_sNameFileType,CFile::modeWrite|CFile::modeCreate))
			{
				fl.Write (s.GetBuffer (s.GetLength ()),s.GetLength ());
				s.ReleaseBuffer ();
			}
			break;
		}


		delete [] pch;
	}

	return n2;
}

void BlockTypeManager::DeleteBlockType(CString sName)
{
	CFile fl;
	if(fl.Open (m_sNameFileType,CFile::modeRead))
	{
		int n = fl.GetLength ();
		char * pch = new char[n+1];
		fl.Read (pch,n);
		pch[n] = 0;
		CString s = pch;

		int nStart = 0;
		sName = "["+sName;
		sName+="]";
		while(true)
		{
			int nFind = s.Find (sName,nStart);
			if(nFind<0) break;
			CString sFirst = s.Left (nFind);

			nStart = nFind+sName.GetLength ();
			int nFind2 = s.Find ("[",nStart);
			if(nFind2<0 ) break;

			CString sType = s.Right(s.GetLength () - nFind2);
			sFirst+=sType;
			fl.Close ();
			if(fl.Open (m_sNameFileType,CFile::modeWrite|CFile::modeCreate))
			{
				fl.Write (sFirst.GetBuffer (sFirst.GetLength ()),sFirst.GetLength ());
				sFirst.ReleaseBuffer ();
			}
			break;
		}


		delete [] pch;
	}

}

void BlockTypeManager::EnumAllType(CStringArray &sTypeArr)
{
	sTypeArr.RemoveAll ();
	CFile fl;
	if(fl.Open (m_sNameFileType,CFile::modeRead))
	{
		int n = fl.GetLength ();
		char * pch = new char[n+1];
		fl.Read (pch,n);
		pch[n] = 0;
		CString s = pch;

		int nStart = 0;
		while(true)
		{
			int nFind = s.Find ("[",nStart);
			if(nFind<0) break;
			int nFind2 = s.Find ("]",nStart);
			if(nFind2<0 || nFind2-nFind-1<=0) break;

			CString sType = s.Mid(nFind+1,nFind2-nFind-1);
			sTypeArr.Add (sType);

			nStart = nFind2+1;
		}


		delete [] pch;
	}
}

void BlockTypeManager::EnumBlockNames(CString sTypeName, CStringArray &sBlockNameArr)
{
	sBlockNameArr.RemoveAll ();
	CFile fl;
	if(fl.Open (m_sNameFileType,CFile::modeRead))
	{
		int n = fl.GetLength ();
		char * pch = new char[n+1];
		fl.Read (pch,n);
		pch[n] = 0;
		CString s = pch;

		int nStart = 0;
		sTypeName = "["+sTypeName;
		sTypeName+="]";
		int nFind = s.Find (sTypeName,nStart);
		if(nFind>=0) 
		{
			nStart  = nFind+sTypeName.GetLength ();
			while(true)
			{
				int nFind2 = s.Find ("\r\n",nStart);
				if(nFind2<0)
					break;
				if(nFind2==nStart)
				{
					nStart+=2;
					continue;
				}
				CString sName = s.Mid( nStart ,nFind2-nStart);
				if(sName.Find ("[",0)>=0) break;
				sBlockNameArr.Add (sName);
				nStart=nFind2+2;
			}
		}


		delete [] pch;
	}
}
void BlockTypeManager::ReadSymbols(CString sBlockName,CStringArray &sSymbolsArr)
{
	sSymbolsArr.RemoveAll ();
	CString sFile = "d:\\superstk\\block\\"+sBlockName+".blk";
	CFile fl;
	if(fl.Open (sFile,CFile::modeRead))
	{
		fl.Seek (4,CFile::begin);
		char ch[16];
		for(int i=0;;i+=12)
		{
			int nRead = fl.Read (ch,12);
			if(nRead!=12)
				break;
			ch[8] = 0;
			if(ch[0]== 'S' &&(ch[1] == 'Z' || ch[1] == 'H'))
			{
				CString s = ch;
				if(s.GetLength () == 8)
					sSymbolsArr.Add (s);
			}
		}
	}
}

void BlockTypeManager::AddSymbols(CString sBlockName, CStringArray &sSymbolsArr)
{
	DeleteSymbols( sBlockName, sSymbolsArr, true);
}

void BlockTypeManager::DeleteSymbols(CString sBlockName, CStringArray &sSymbolsArr)
{
	DeleteSymbols( sBlockName, sSymbolsArr, false);
}

void BlockTypeManager::DeleteSymbols(CString sBlockName, CStringArray &sSymbolsArr, bool bAppend)
{
	CStringArray sAllSymbolsArr;
	ReadSymbols( sBlockName,sAllSymbolsArr);
	for(int i=0;i<sAllSymbolsArr.GetSize ();i++)
	{
		for(int j=0;j<sSymbolsArr.GetSize ();j++)
		{
			if(sSymbolsArr[i] == sAllSymbolsArr[j])
			{
				sAllSymbolsArr.RemoveAt (i,1);
				i--;
				break;
			}
		}
	}
	if(bAppend)
		sAllSymbolsArr.Append(sSymbolsArr);


	CString sFile = "block\\"+sBlockName+".blk";
	CFile fl;
	if(fl.Open (sFile,CFile::modeCreate|CFile::modeWrite))
	{
		fl.Write (&dwFileHead,4);
		char ch[16];
		for(int i=0;i<sAllSymbolsArr.GetSize ();i+=12)
		{
			strcpy(ch,sAllSymbolsArr[i]);
			fl.Write (ch,12);
		}
	}
	fl.Close();

}
