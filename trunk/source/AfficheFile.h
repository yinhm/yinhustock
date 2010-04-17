// AfficheFile.h: interface for the CAfficheFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AFFICHEFILE_H__E2EED2F8_9BA5_4CC2_B26A_39B6DA1981E0__INCLUDED_)
#define AFX_AFFICHEFILE_H__E2EED2F8_9BA5_4CC2_B26A_39B6DA1981E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _tagAfficheFile
{
	CString		sCode;						
	CString		sHead;							
	CString		sTime;							
	CString		sContext;						

	int GetSize()
	{
		int size = sHead.GetLength() + sTime.GetLength() + sContext.GetLength();
		return size;
	};
}AFFICHEFILE;
#define PAFFICHEFILE AFFICHEFILE*

class CAfficheFile  
{
public:
	CAfficheFile(LPCTSTR lpsz = NULL);
	virtual ~CAfficheFile();

public:
	CFile m_File;											
	CMap <int, int, PAFFICHEFILE, PAFFICHEFILE> m_FileMap;		

protected:
	CString sFileName;					
	BOOL	m_bOpen;


public:
	void OnAddNew(char* lpsz, int size);
	void OnDeleteAll();
	void OnRefashFile();
	void SetFileName(LPCTSTR lpsz);



protected:
	BOOL OnOpenFile();
	PAFFICHEFILE OnTransStruct(char *lp, int size);
	void OnRemoveFileAll();

};

#endif // !defined(AFX_AFFICHEFILE_H__E2EED2F8_9BA5_4CC2_B26A_39B6DA1981E0__INCLUDED_)
