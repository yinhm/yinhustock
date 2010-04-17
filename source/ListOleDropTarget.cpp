// ListOleDropTarget.cpp: implementation of the CListOleDropTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "ListOleDropTarget.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOleDropTarget::CListOleDropTarget()
{

}

CListOleDropTarget::~CListOleDropTarget()
{

}

DROPEFFECT CListOleDropTarget::OnDragEnter(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	return OnDragOver(pWnd, pDataObject, dwKeyState, point);
}
DROPEFFECT CListOleDropTarget::OnDragOver(CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT dropE = DROPEFFECT_NONE;

	if(pDataObject->IsDataAvailable (CF_TEXT))
	{
		dropE = (dwKeyState&MK_CONTROL)?DROPEFFECT_COPY:DROPEFFECT_MOVE;
	}

	return dropE;
}
BOOL CListOleDropTarget::OnDrop(CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	FORMATETC fmt = {CF_TEXT,NULL,DVASPECT_CONTENT,-1,TYMED_HGLOBAL};

	HGLOBAL hG = pDataObject->GetGlobalData (CF_TEXT,&fmt);

	if(hG == NULL)
		return FALSE;

	LPCSTR pText = (LPCSTR)GlobalLock(hG);

	if(AfxIsValidString(pText))
	{
		::SendMessage (pWnd->m_hWnd ,WM_USER_DRAGDROP,0,(LPARAM)(LPCSTR)pText);
		GlobalFree(hG);
		return TRUE;
	}

	return FALSE;
}
