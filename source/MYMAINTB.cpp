// MYMAINTB.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "MYMAINTB.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMYMAINTB

CMYMAINTB::CMYMAINTB()
{
}

CMYMAINTB::~CMYMAINTB()
{
}


BEGIN_MESSAGE_MAP(CMYMAINTB, CToolBarCtrl)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMYMAINTB message handlers

BOOL CMYMAINTB::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{

	buttonnumber=ID_PALETTEOVAL-ID_PALETTEERASE+1;
		
	VERIFY(AddBitmap(buttonnumber,IDB_PALETTE) != -1);

	m_tbbutton = new TBBUTTON[buttonnumber];

	for (int nIndex = 0; nIndex < buttonnumber; nIndex++)
	{
		CString string;
		string.LoadString(nIndex + ID_PALETTEERASE);

		// Add second '\0'
		int nStringLength = string.GetLength() + 1;
		TCHAR* pString = string.GetBufferSetLength(nStringLength);
		pString[nStringLength] = 0;

		VERIFY((m_tbbutton[nIndex].iString = AddStrings(pString)) != -1);

		string.ReleaseBuffer();


		m_tbbutton[nIndex].fsState = TBSTATE_ENABLED;
		m_tbbutton[nIndex].fsStyle = TBSTYLE_CHECKGROUP;
		m_tbbutton[nIndex].dwData = 0;
		m_tbbutton[nIndex].iBitmap = nIndex;
		m_tbbutton[nIndex].idCommand = nIndex + ID_PALETTEERASE;

	}


	for (int nIndex = 0; nIndex < buttonnumber; nIndex++)
	{
		VERIFY(AddButtons(1,&m_tbbutton[nIndex]));
	}


	
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
