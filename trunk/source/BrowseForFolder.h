
#ifndef __SHELLBROWSER_H__
#define __SHELLBROWSER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <memory>
#include <shlobj.h>




class CBrowseForFolder
{
public:
	CBrowseForFolder(const HWND hParent = NULL, const LPITEMIDLIST pidl = NULL, const int nTitleID = 0);
	CBrowseForFolder(const HWND hParent, const LPITEMIDLIST pidl, const CString& strTitle);
	virtual ~CBrowseForFolder() = 0;

	
	void SetOwner(const HWND hwndOwner);

	
	void SetRoot(const LPITEMIDLIST pidl);


	CString GetTitle() const;
	void SetTitle(const CString& strTitle);
	bool SetTitle(const int nTitle);

	
	UINT GetFlags() const;
	void SetFlags(const UINT ulFlags);

	
	CString GetSelectedFolder() const;

	
	int GetImage() const;

	
	bool SelectFolder();

protected:

	virtual void OnInit() const;

	
	virtual void OnSelChanged(const LPITEMIDLIST pidl) const;

	
	void EnableOK(const bool bEnable) const;

	
	void SetSelection(const LPITEMIDLIST pidl) const;
	void SetSelection(const CString& strPath) const;

	
	void SetStatusText(const CString& strText) const;

private:
	static int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

	typedef std::auto_ptr<char> AUTO_STR;
	AUTO_STR m_pchTitle;

	BROWSEINFO m_bi;
	char m_szSelected[MAX_PATH];
	CString m_strPath;
	HWND m_hwnd;
};

inline UINT CBrowseForFolder::GetFlags() const
{
	return m_bi.ulFlags;
}

inline int CBrowseForFolder::GetImage() const
{
	return m_bi.iImage;
}

#endif // __SHELLBROWSER_H__
