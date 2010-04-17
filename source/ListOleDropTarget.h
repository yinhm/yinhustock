
#if !defined(AFX_LISTOLEDROPTARGET_H__1C5CC1F5_4BD3_11D3_91FC_0080C8E1242B__INCLUDED_)
#define AFX_LISTOLEDROPTARGET_H__1C5CC1F5_4BD3_11D3_91FC_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxole.h>


class CListOleDropTarget : public COleDropTarget  
{
public:
	virtual DROPEFFECT OnDragEnter(CWnd * pWnd,COleDataObject * pDataObject,DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd * pWnd,COleDataObject * pDataObject,DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd * pWnd,COleDataObject * pDataObject,DROPEFFECT dropEffect, CPoint point);
	CListOleDropTarget();
	virtual ~CListOleDropTarget();

};

#endif 
