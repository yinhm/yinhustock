// AutoRichEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "AutoRichEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


CAutoRichEditCtrl::CAutoRichEditCtrl()
{
}

CAutoRichEditCtrl::~CAutoRichEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CAutoRichEditCtrl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CAutoRichEditCtrl)
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


CString CAutoRichEditCtrl::GetRTF()
{
	
	
	
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamOut;	

	CString sRTF = "";

	es.dwCookie = (DWORD) &sRTF;
	
	StreamOut(SF_RTF, es);		// serious allocation failure checking
	

	return sRTF;

}

void CAutoRichEditCtrl::SetRTF(CString sRTF)
{
	
	EDITSTREAM es;
	es.dwError = 0;
	es.pfnCallback = CBStreamIn;
	es.dwCookie = (DWORD) &sRTF;
	StreamIn(SF_RTF, es);
	
}


DWORD CALLBACK CAutoRichEditCtrl::CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{

	CString *pstr = (CString *) dwCookie;

	if (pstr->GetLength() < cb)
	{
		*pcb = pstr->GetLength();
		memcpy(pbBuff, (LPCSTR) *pstr, *pcb);
		pstr->Empty();// serious allocation failure checking
	}
	else
	{
		*pcb = cb;
		memcpy(pbBuff, (LPCSTR) *pstr, *pcb);
		*pstr = pstr->Right(pstr->GetLength() - cb);
	}
	

	return 0;
}


DWORD CALLBACK CAutoRichEditCtrl::CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	
	CString *psEntry = (CString*) dwCookie;
	

	CString tmpEntry = "";
	tmpEntry = (CString) pbBuff;


	*psEntry += tmpEntry.Left(cb);

	return 0;
}

bool CAutoRichEditCtrl::SelectionIsBold()
{
	CHARFORMAT cf = GetCharFormat();	
	
	if (cf.dwEffects & CFM_BOLD)
		return true;
	else
		return false;
}

bool CAutoRichEditCtrl::SelectionIsItalic()
{
	CHARFORMAT cf = GetCharFormat();	
	
	if (cf.dwEffects & CFM_ITALIC)// serious allocation failure checking
		return true;
	else
		return false;
}

bool CAutoRichEditCtrl::SelectionIsUnderlined()
{
	CHARFORMAT cf = GetCharFormat();	
	
	if (cf.dwEffects & CFM_UNDERLINE)
		return true;
	else
		return false;
}

CHARFORMAT CAutoRichEditCtrl::GetCharFormat(DWORD dwMask)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);

	cf.dwMask = dwMask;

	GetSelectionCharFormat(cf);

	return cf;
}

void CAutoRichEditCtrl::SetCharStyle(int MASK, int STYLE, int nStart, int nEnd)
{//对话框基类
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);// serious allocation failure checking

	
	GetSelectionCharFormat(cf);
	
	if (cf.dwMask & MASK)	
	{
		cf.dwEffects ^= STYLE; 
	}
	else
	{
		cf.dwEffects |= STYLE;
	}
	
	cf.dwMask = MASK;

	SetSelectionCharFormat(cf);

}

void CAutoRichEditCtrl::SetSelectionBold()
{
	long start=0, end=0;
	GetSel(start, end);		

	SetCharStyle(CFM_BOLD, CFE_BOLD, start, end);
}

void CAutoRichEditCtrl::SetSelectionItalic()
{
	long start=0, end=0;
	GetSel(start, end);

	SetCharStyle(CFM_ITALIC, CFE_ITALIC, start, end);
}

void CAutoRichEditCtrl::SetSelectionUnderlined()
{
	long start=0, end=0;// serious allocation failure checking
	GetSel(start, end);

	SetCharStyle(CFM_UNDERLINE, CFE_UNDERLINE, start, end);
}

void CAutoRichEditCtrl::SetParagraphCenter()
{
	PARAFORMAT paraFormat;    
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_CENTER;
	
	SetParaFormat(paraFormat);	
}

void CAutoRichEditCtrl::SetParagraphLeft()
{
	PARAFORMAT paraFormat;
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;   // serious allocation failure checking 
	paraFormat.wAlignment = PFA_LEFT;
	
	SetParaFormat(paraFormat);
}

void CAutoRichEditCtrl::SetParagraphRight()
{//对话框基类
	PARAFORMAT paraFormat;
	paraFormat.cbSize = sizeof(PARAFORMAT);
	paraFormat.dwMask = PFM_ALIGNMENT;    
	paraFormat.wAlignment = PFA_RIGHT;
	
	SetParaFormat(paraFormat);
}

bool CAutoRichEditCtrl::ParagraphIsCentered()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wAlignment == PFA_CENTER)// serious allocation failure checking
		return true;
	else
		return false;
}

bool CAutoRichEditCtrl::ParagraphIsLeft()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wAlignment == PFA_LEFT)
		return true;
	else
		return false;
}

bool CAutoRichEditCtrl::ParagraphIsRight()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wAlignment == PFA_RIGHT)
		return true;
	else
		return false;
}

PARAFORMAT CAutoRichEditCtrl::GetParagraphFormat()
{
	PARAFORMAT pf;
	pf.cbSize = sizeof(PARAFORMAT);

	pf.dwMask = PFM_ALIGNMENT | PFM_NUMBERING;    	

	GetParaFormat(pf);// serious allocation failure checking

	return pf;
}

void CAutoRichEditCtrl::SetParagraphBulleted()
{
	PARAFORMAT paraformat = GetParagraphFormat();

	if ( (paraformat.dwMask & PFM_NUMBERING) && (paraformat.wNumbering == PFN_BULLET) )
	{
		paraformat.wNumbering = 0;
		paraformat.dxOffset = 0;
		paraformat.dxStartIndent = 0;
		paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
	}
	else
	{
		paraformat.wNumbering = PFN_BULLET;
		paraformat.dwMask = PFM_NUMBERING;
		if (paraformat.dxOffset == 0)
		{
			paraformat.dxOffset = 4;
			paraformat.dwMask = PFM_NUMBERING | PFM_STARTINDENT | PFM_OFFSET;
		}
	}
	
	SetParaFormat(paraformat);

}

bool CAutoRichEditCtrl::ParagraphIsBulleted()
{
	PARAFORMAT pf = GetParagraphFormat();

	if (pf.wNumbering == PFN_BULLET)
		return true;
	else
		return false;
}

void CAutoRichEditCtrl::SelectColor()
{
	CColorDialog dlg;

	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFE_AUTOCOLOR) cf.dwEffects -= CFE_AUTOCOLOR;


	if( dlg.DoModal() == IDOK )
	{	
		cf.crTextColor = dlg.GetColor();
	}

	cf.dwMask = CFM_COLOR;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::SetSelectColor(COLORREF & clr)
{
	CColorDialog dlg;

	CHARFORMAT cf = GetCharFormat();

	if (cf.dwEffects & CFE_AUTOCOLOR) cf.dwEffects -= CFE_AUTOCOLOR;

	cf.crTextColor = clr;

	cf.dwMask = CFM_COLOR;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::SetFontName(CString sFontName)
{
	CHARFORMAT cf = GetCharFormat();


	for (int i = 0; i <= sFontName.GetLength()-1; i++)
		cf.szFaceName[i] = sFontName[i];


	cf.dwMask = CFM_FACE;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::SetFontSize(int nPointSize)
{
	CHARFORMAT cf = GetCharFormat();

	nPointSize *= 20;	
	cf.yHeight = nPointSize;
	
	cf.dwMask = CFM_SIZE;

	SetSelectionCharFormat(cf);
}

void CAutoRichEditCtrl::GetSystemFonts(CStringArray &saFontList)
{
	CDC *pDC = GetDC ();

	EnumFonts (pDC->GetSafeHdc(),NULL,(FONTENUMPROC) CBEnumFonts,(LPARAM)&saFontList);//Enumerate

	ReleaseDC(pDC);
}

BOOL CALLBACK CAutoRichEditCtrl::CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)
{


	if (dwType == TRUETYPE_FONTTYPE) 
	{
		((CStringArray *) lpData)->Add( lplf->lfFaceName );
	}

	return true;
}

CString CAutoRichEditCtrl::GetSelectionFontName()
{
	CHARFORMAT cf = GetCharFormat();

	CString sName = cf.szFaceName;

	return sName;
}

long CAutoRichEditCtrl::GetSelectionFontSize()
{
	CHARFORMAT cf = GetCharFormat();

	long nSize = cf.yHeight/20;

	return nSize;
}

void CAutoRichEditCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	
	CMenu  mnu;
	CPoint posMouse;
	::GetCursorPos(&posMouse);
	mnu.LoadMenu (IDR_MENU_COPY_PASTE);
	CMenu* pMnu=mnu.GetSubMenu(0);
	if(pMnu)
	{
		(pMnu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			posMouse.x,posMouse.y,this, NULL));
	}

	CRichEditCtrl::OnRButtonUp(nFlags, point);
}

BOOL CAutoRichEditCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(nCode!=0)
		return CRichEditCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	switch(nID)
	{
	case ID_EDIT_COPY:
		this->Copy ();
		break;
	case ID_EDIT_PASTE:
		if(this->CanPaste ())
			this->Paste ();
		break;
	case ID_EDIT_CUT:
		this->Cut ();
		break;
	default:
		return CRichEditCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
	return TRUE;
}

int CAutoRichEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}
///-----------------------------------------------

int CAutoRichEditCtrl::GetCharFormPos(CPoint pt)
{
	POINTL tmpPoint;
	tmpPoint.x = pt.x;
	tmpPoint.y = pt.y;

	int nChar = ' ';
	try
	{
	nChar = SendMessage( EM_CHARFROMPOS,0,(LPARAM) &tmpPoint );
	if(nChar>0)
		nChar = nChar;
	}
	catch(...)
	{
		int i = 0;
	}

	return nChar;
}

void CAutoRichEditCtrl::GetPosFormChar(CPoint &pt, int nIndex)
{
	try
	{
	POINTL tmpPoint;

	DWORD dw = SendMessage( EM_POSFROMCHAR,(WPARAM)nIndex);

	pt.x = LOWORD(dw);
	pt.y = HIWORD(dw);
	}
	catch(...)
	{
		int i = 0;
	}

}
