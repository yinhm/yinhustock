// FtpParaClass.h: interface for the FtpParaClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPPARACLASS_H__5CDDA4C7_A9D4_4968_B10D_34333B1B0CCE__INCLUDED_)
#define AFX_FTPPARACLASS_H__5CDDA4C7_A9D4_4968_B10D_34333B1B0CCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS FtpParaClass 
{
public:
	FtpParaClass();
protected:
	HCURSOR m_hDragCursor;
	BOOL m_bDragging;
	CImageList* m_pDragImageList;
	CImageList m_ctImageList;
public:
typedef struct
{
	char szFileName[256];
	char szFileDate[20];
	char szFileSize[20];
	int nType;
}FILE_FTP_INFO;
typedef struct
{
	char fileName[256];
	UINT ufileFlag;
}FILE_COUNT_INFO;
typedef struct
{
	char OldName[256];
	char NewName[256];
}FILE_CHANGE_INFO;

protected:
	void SetPopMenu(int nIndex);
	void OnFileName(CListCtrl* pListCtrl);
	void OnInsertFile(CListCtrl* pListCtrl,LPVOID pIn,LPARAM lParam);
	void GetFileIcon(CString& fileName,int* iIcon,int* iIconSel=NULL);
	BOOL SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo=NULL);
	void SetFileColumns(CListCtrl* pListCtrl);
public:

	virtual ~FtpParaClass();

};


#endif // !defined(AFX_FTPPARACLASS_H__5CDDA4C7_A9D4_4968_B10D_34333B1B0CCE__INCLUDED_)



