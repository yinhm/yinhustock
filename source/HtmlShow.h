
#if !defined(AFX_HTMLSHOW_H__34733AD5_764B_11D6_968D_F312CA32DD75__INCLUDED_)
#define AFX_HTMLSHOW_H__34733AD5_764B_11D6_968D_F312CA32DD75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHtmlShow  
{
public:
	CHtmlShow();
	virtual ~CHtmlShow();
public:
	bool Create(CWnd* pWnd,CRect & r);
	void Navigate(LPCTSTR URL, DWORD dwFlags = 0,
		LPCTSTR lpszTargetFrameName = NULL,
		LPCTSTR lpszHeaders = NULL, LPVOID lpvPostData = NULL,
		DWORD dwPostDataLen = 0);
	CWnd m_wndBrowser;
protected:
	IWebBrowser2* m_pBrowserApp;
	bool bCreated;

};

#endif // !defined(AFX_HTMLSHOW_H__34733AD5_764B_11D6_968D_F312CA32DD75__INCLUDED_)
