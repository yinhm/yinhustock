// FloatEdit.cpp : implementation file
//

#include "stdafx.h"
#include "FloatEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


CFloatEdit::CFloatEdit()
{
}

CFloatEdit::~CFloatEdit()
{
}


BEGIN_MESSAGE_MAP(CFloatEdit, CEdit)
	//{{AFX_MSG_MAP(CFloatEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CFloatEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	static BOOL bDot=TRUE;
	CString szText;
	GetWindowText(szText);
	bDot=(szText.Find('.')==-1)? TRUE:bDot;

	TCHAR tChar=(TCHAR)nChar;

	if( _istdigit(tChar) )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if(tChar =='-' )
	{
		if(szText.Find('-')==-1)	CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if(tChar =='.' && bDot )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		bDot=FALSE;
	}
	else if( nChar== VK_BACK )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
		MessageBeep(MB_ICONASTERISK);

	GetWindowText(szText);
	int nNeg = -1;
	if(nNeg = szText.Find('-')>0)
	{
		szText.Delete (nNeg,1);
		SetWindowText(szText);
		UpdateWindow();
	}


}
