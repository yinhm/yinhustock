// HtmlShow.cpp: implementation of the CHtmlShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "HtmlShow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHtmlShow::CHtmlShow()
{
	bCreated = false;
}

CHtmlShow::~CHtmlShow()
{
	if(	bCreated == true)
		m_wndBrowser.DestroyWindow();
	if (m_pBrowserApp != NULL)
		m_pBrowserApp->Release();

}

bool CHtmlShow::Create(CWnd *pWnd, CRect &r)
{
	CString s;
	pWnd->GetWindowText (s);
	if (!m_wndBrowser.CreateControl(CLSID_WebBrowser, s.GetBuffer(0),
				WS_VISIBLE | WS_CHILD, r, pWnd, AFX_IDW_PANE_FIRST))
	{
		return false;
	}
	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED(hr))
	{
		m_pBrowserApp = NULL;
		m_wndBrowser.DestroyWindow();
		return false;
	}
	bCreated = true;
	return true;
}
void CHtmlShow::Navigate(LPCTSTR lpszURL, DWORD dwFlags /* = 0 */,
	LPCTSTR lpszTargetFrameName /* = NULL */ ,
	LPCTSTR lpszHeaders /* = NULL */, LPVOID lpvPostData /* = NULL */,
	DWORD dwPostDataLen /* = 0 */)
{
	CString strURL(lpszURL);
	BSTR bstrURL = strURL.AllocSysString();

	COleSafeArray vPostData;
	if (lpvPostData != NULL)
	{
		if (dwPostDataLen == 0)
			dwPostDataLen = lstrlen((LPCTSTR) lpvPostData);

		vPostData.CreateOneDim(VT_UI1, dwPostDataLen, lpvPostData);
	}

	m_pBrowserApp->Navigate(bstrURL,
		COleVariant((long) dwFlags, VT_I4),
		COleVariant(lpszTargetFrameName, VT_BSTR),
		vPostData,
		COleVariant(lpszHeaders, VT_BSTR));
}