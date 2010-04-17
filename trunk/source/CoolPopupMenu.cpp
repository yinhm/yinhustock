// CoolPopupMenu.cpp: implementation of the CCoolPopupMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CoolPopupMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define RGB_BUTTON_BLACK    (GetSysColor(COLOR_WINDOWFRAME))
#define RGB_BUTTON_WHITE    (GetSysColor(COLOR_BTNHIGHLIGHT))
#define RGB_BUTTON_LIGHT    (GetSysColor(COLOR_BTNFACE))
#define RGB_BUTTON_DARK     (GetSysColor(COLOR_BTNSHADOW))
#define RGB_MENU_FACE       (GetSysColor(COLOR_MENU))
#define RGB_MENUTEXT_BACK   (GetSysColor(COLOR_HIGHLIGHT))
#define RGB_MENU_TEXT       (GetSysColor(COLOR_MENUTEXT))
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoolPopupMenu::CCoolPopupMenu()//系统窗口配色
{
	m_iMenuWidth=70;
	m_iMenuHeight=20;
	m_iItemNum=0;
	m_iUpHeight=0;
	CreatePopupMenu ();
}

CCoolPopupMenu::~CCoolPopupMenu()
{
	DestroyMenu();
}

void CCoolPopupMenu::SetBackImage(UINT nID,int cx,int row)
{// NOTE: the ClassWizard will add member functions here
	COLORREF  cl=GetSysColor(COLOR_MENU);
	m_BackImageList.Create(nID,m_iMenuHeight,row,cl);
	m_iBackImageHeight=m_iMenuHeight;

	m_iMenuHeight=cx;
	m_iBackImageWidth=21;
}

void CCoolPopupMenu::SetImage(UINT nID, int cx, int row)
{
	COLORREF  cl=GetSysColor(COLOR_MENU);
	m_ImageList.Create(nID,cx,row,cl);
	m_iImageWidth=cx;
}

void CCoolPopupMenu::AppendCoolMenu(UINT nID, CString text)
{	// NOTE: the ClassWizard will add member functions here
	if(nID==MF_SEPARATOR)
		AppendMenu(MF_SEPARATOR|MF_OWNERDRAW, nID,(LPCTSTR)text);
	else
		AppendMenu(MF_ENABLED | MF_OWNERDRAW, nID,(LPCTSTR)text);
	
	m_MenuText.Add(text);

	MenuItem  item;
	item.m_strMenuText=text;
	item.m_iIndex=m_iItemNum;
	m_MapMenu.SetAt(nID,item);

	m_iItemNum+=1;
	
}



void CCoolPopupMenu::MeasureItem(LPMEASUREITEMSTRUCT  lpDrawItemStruct)
{// NOTE: the ClassWizard will add member functions here
	if(lpDrawItemStruct->itemID==MF_SEPARATOR)
		lpDrawItemStruct->itemHeight = 10;
	else
		lpDrawItemStruct->itemHeight = m_iMenuHeight;
	lpDrawItemStruct->itemWidth  = m_iMenuWidth;
}

void CCoolPopupMenu::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect  rc=lpDrawItemStruct->rcItem;
	UINT  ID=lpDrawItemStruct->itemID;
	DWORD  Data=lpDrawItemStruct->itemData;

	MenuItem  item;
	m_MapMenu.Lookup(ID,item);
	

	if(ID==MF_SEPARATOR)
	{
		CRect  rect;
		rect.left=rc.left+m_iBackImageWidth;
		rect.right=rc.right;
		rect.top=rc.top+int((rc.bottom-rc.top)/2.0);
		rect.bottom=rc.bottom;
		pDC->DrawEdge(rect,EDGE_ETCHED,BF_TOP);
		m_iUpHeight+=rc.Height()-rect.Height();
	}
	else
	{


		pDC->SetBkMode(TRANSPARENT);

		CRect  ImageRect,TextRect;
		ImageRect=rc;
		ImageRect.left+=m_iBackImageWidth+2;
	

		TextRect=rc;
		
		TextRect.left=ImageRect.left;
		if(lpDrawItemStruct->itemState & ODS_SELECTED==ODS_SELECTED)
		{
			pDC->Draw3dRect(&ImageRect,RGB_BUTTON_WHITE,RGB_BUTTON_DARK);
			pDC->Draw3dRect(&TextRect,RGB_BUTTON_DARK,RGB_BUTTON_WHITE);
		}
		else
		{
			pDC->Draw3dRect(&ImageRect,RGB_MENU_FACE,RGB_MENU_FACE);
			pDC->Draw3dRect(&TextRect,RGB_MENU_FACE,RGB_MENU_FACE);
		}
		
		TextRect.left+=4;
		
		pDC->DrawText(item.m_strMenuText,TextRect,DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	}


	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)// NOTE: the ClassWizard will add member functions here
		m_BackImageList.Draw(pDC,item.m_iIndex,CPoint(rc.left,rc.top),ILD_NORMAL);

}



