// RichEditFormular.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "RichEditFormular.h"
#include "DIAEDITZBGSHS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern dataFunc stritemsArr[];
extern int nItemCountText;
/////////////////////////////////////////////////////////////////////////////
// CRichEditFormular

CRichEditFormular::CRichEditFormular()
{
}

CRichEditFormular::~CRichEditFormular()
{
}


BEGIN_MESSAGE_MAP(CRichEditFormular, CAutoRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditFormular)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


void CRichEditFormular::OnMouseMove(UINT nFlags, CPoint point) 
{//子窗口框架


	CAutoRichEditCtrl::OnMouseMove(nFlags, point);

	if(m_TitleTip.m_hWnd == NULL )
	{
		CRect rect(0,0,1,1);
		m_TitleTip.Create (this);
		return;
	}
	int nCharOffset = GetCharFormPos( point );
	if( nCharOffset < 0 )
	{
		m_TitleTip.Hide ();
		return;
	}

	CString strText;
	GetWindowText( strText );
	if( nCharOffset >= strText.GetLength() )
	{
		m_TitleTip.Hide ();
		return;
	}
	CString strTmp = GetWordByOffset(nCharOffset);
	strTmp.TrimRight();
	if( strTmp.IsEmpty() )
	{
		m_TitleTip.Hide ();
		return;
	}

	TRACE("%s\n",strTmp);

	strTmp.MakeUpper ();
	int nLen = strTmp.GetLength ();

	int i;
	for(i = 0;i<nItemCountText;i++)
	{
		CString s = stritemsArr[i].s ;
		s = s.Left (nLen);
		if(s == strTmp)
		{
			if(stritemsArr[i].s[nLen] == '-' ||stritemsArr[i].s[nLen] == '(')
				break;
		}
	}
	if(i>=nItemCountText)
	{
		m_TitleTip.Hide ();
		return;
	}
	CString s2 = stritemsArr[i].s;
	int nFind = s2.Find ("---",0);
	if(nFind>=0)
	{
		strTmp = s2.Left(nFind);
		s2.Delete (0,nFind+3);
	}
	strTmp = strTmp+"\n"+s2+"\n";

	TRACE("22 == %s\n",strTmp);

	m_TitleTip.Show(strTmp,point);

}
//子窗口框架
/// 2002-11-8

CString CRichEditFormular::GetWordByOffset(int nOffset)
{
	CString strText;
	GetWindowText( strText );
	if( nOffset >= strText.GetLength() )
	{
		strText.Empty();
		return strText;
	}
	int nLeftPos = nOffset;
	while( nLeftPos>=0 )
	{
		char cLeft = strText[ nLeftPos ];
		if( FALSE == ::isalnum(cLeft) )
		{
			nLeftPos ++;
			break;				
		}
		nLeftPos --;
	}
	
	if( nLeftPos < 0 )
		nLeftPos = 0;

	int nMaxLen = strText.GetLength();
	int nRightPos = nOffset;
	while( nRightPos < nMaxLen )
	{
		char cRight = strText[ nRightPos ];
		if( FALSE == ::isalnum(cRight) )
		{
			nRightPos --;
			CString ss = strText.Mid( nLeftPos, nRightPos-nLeftPos+1 );
			ss.MakeLower ();
			if(ss == "finance" || ss == "dynainfo"
				|| ss == "askvol"
				|| ss == "askprice"
				|| ss == "bidvol"
				|| ss == "bidorice"
				)
			{
				bool bFind = false;
				while( nRightPos < nMaxLen )
				{
					cRight = strText[ nRightPos ];
					if(cRight == ')')
					{
						bFind = true;

						break;
					}
					nRightPos ++;
				}
			}
			break;
		}
		nRightPos ++;
	}
	if( nRightPos >= nMaxLen )
		nRightPos = nMaxLen-1;

	CString ss2 = strText.Mid( nLeftPos, nRightPos-nLeftPos+1 );

	ss2.Replace (" ","");
	
	return ss2;
}

int CRichEditFormular::OnCreate(LPCREATESTRUCT lpCreateStruct) //子窗口框架
{
	if (CAutoRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect(0,0,1,1);
	m_TitleTip.Create (this);

	return 0;
}
