// ParamEdit.cpp : implementation file
//tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ParamEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamEdit

CParamEdit::CParamEdit()
{
//	m_textPre="";
}

CParamEdit::CParamEdit(CListCtrl *list,int Item)
{
	m_bTestMax = true;
  m_plist = list;
  nItem = Item;
  char tmp[30];
  m_plist->GetItemText(nItem,1,tmp,30);
  m_textPre = tmp;

  bEscapeKey=FALSE;




 //tel:13366898744
}

CParamEdit::~CParamEdit()
{
}


BEGIN_MESSAGE_MAP(CParamEdit, CFloatEdit)
	//{{AFX_MSG_MAP(CParamEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

int CParamEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFloatEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->SetLimitText(10);
	return 0;
}

BOOL CParamEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->wParam == VK_RETURN)
	{
		m_plist->SetFocus ();

		return TRUE;
	}
	else if (pMsg->wParam == VK_ESCAPE)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return bEscapeKey = TRUE;
	}
	
	return CFloatEdit::PreTranslateMessage(pMsg);
}

void CParamEdit::InsertItem()
{
  	   CString temp;
	   this->GetWindowText(temp);
	   if(m_plist == 0)
		   return;
	   if(m_plist && temp.GetLength())
	   {
		   m_plist->SetItemText(nItem,1,temp);
		   m_plist->Invalidate();
		   m_plist->SetFocus();
	   }
}
void CParamEdit::PostNcDestroy() 
{
	CFloatEdit::PostNcDestroy();
	delete this;
}

void CParamEdit::OnKillFocus(CWnd* pNewWnd) 
{
	if (!bEscapeKey)
	{
		if(m_bTestMax == true)
		{
			CString temp;
			this->GetWindowText(temp);
			int tmp=atol(temp.GetBuffer(0) );

		}
		InsertItem();
	}
	PostMessage(WM_CLOSE, 0, 0);
}
BOOL CParamEdit::CheckNumber(CString Num)
{
	return FALSE;
}
