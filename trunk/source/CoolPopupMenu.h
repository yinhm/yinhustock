
#if !defined(AFX_COOLPOPUPMENU_H__50444B2C_1510_11D4_9E06_0050BABF8C7F__INCLUDED_)
#define AFX_COOLPOPUPMENU_H__50444B2C_1510_11D4_9E06_0050BABF8C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"


struct MenuItem
{
	CString  m_strMenuText;
	int      m_iIndex;
};

class CCoolPopupMenu : public CMenu  
{
public:
	CCoolPopupMenu();
	virtual ~CCoolPopupMenu();


public:
	void SetImage(UINT nID,int cx,int row);   
	void AppendCoolMenu(UINT nID,CString text);
	void SetBackImage(UINT nID,int cx,int row);

public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT  lpDrawItemStruct) ;
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

public:
	void  SetMenuWidth(int width)  { m_iMenuWidth=width;   }
	int   GetMenuWidth()           { return  m_iMenuWidth; }

	void  SetMenuHeight(int height){ m_iMenuHeight=height; }
	int   GetMenuHeight()          { return  m_iMenuHeight;}

private:
	int    m_iMenuWidth;   
	int    m_iMenuHeight;   

	int    m_iBackImageWidth; 
	int    m_iBackImageHeight; 

	int    m_iImageWidth;     
	int    m_iImageHeight;   
private:
	CImageList  m_BackImageList;
	CImageList  m_ImageList;
	CArray<CString,CString>m_MenuText;   
	CMap<UINT,UINT,MenuItem,MenuItem>  m_MapMenu;  
	int      m_iItemNum;
	int      m_iUpHeight;
};

#endif // !defined(AFX_COOLPOPUPMENU_H__50444B2C_1510_11D4_9E06_0050BABF8C7F__INCLUDED_)
