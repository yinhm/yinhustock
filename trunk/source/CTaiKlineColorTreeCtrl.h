#if !defined(AFX_TECHCOLORTREECTRL_H__53229A59_7CF8_4CF7_9C76_99CBC6224977__INCLUDED_)
#define AFX_TECHCOLORTREECTRL_H__53229A59_7CF8_4CF7_9C76_99CBC6224977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineColorTreeCtrl.h : header file
//


/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

class CTaiKlineColorTreeCtrl : public CTreeCtrl
{

public:
	CTaiKlineColorTreeCtrl();


public:


public:
	void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
	void SetItemBold(HTREEITEM hItem, BOOL bBold);
	void SetItemColor(HTREEITEM hItem, COLORREF color);
	BOOL GetItemFont(HTREEITEM hItem, LOGFONT * plogfont);
	BOOL GetItemBold(HTREEITEM hItem);
	COLORREF GetItemColor(HTREEITEM hItem);

protected:
	CImageList*	m_pDragImage;
	BOOL		m_bLDragging;
	HTREEITEM	m_hitemDrag,m_hitemDrop;
	CWnd*			m_wndTarget;
	HTREEITEM CopyBranch(HTREEITEM oldItem, HTREEITEM newParent, HTREEITEM iPosition);
	void SetNewParent(HTREEITEM newPosition, HTREEITEM oldPosition);

	virtual bool CanBeginDrag();
	virtual bool CanDroped(HTREEITEM	hitem);
	virtual void DoDropOut(CWnd* pWnd,HTREEITEM hItem);
	virtual void DoDropedSelf(HTREEITEM hItem);

public:
	virtual ~CTaiKlineColorTreeCtrl();

protected:
	struct Color_Font
	{
		COLORREF color;
		LOGFONT  logfont;
	};
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;

	
protected:
	//{{AFX_MSG(CTaiKlineColorTreeCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHCOLORTREECTRL_H__53229A59_7CF8_4CF7_9C76_99CBC6224977__INCLUDED_)
