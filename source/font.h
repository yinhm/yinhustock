#if !defined(AFX_FONT_H__0C1E7D84_AB94_11D2_91E0_0080C8E1242B__INCLUDED_)
#define AFX_FONT_H__0C1E7D84_AB94_11D2_91E0_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COleFont : public COleDispatchDriver
{
public:
	COleFont() {}	
	COleFont(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COleFont(const COleFont& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}


public:
	CString GetName();
	void SetName(LPCTSTR);
	CY GetSize();
	void SetSize(const CY&);
	BOOL GetBold();
	void SetBold(BOOL);
	BOOL GetItalic();
	void SetItalic(BOOL);
	BOOL GetUnderline();
	void SetUnderline(BOOL);
	BOOL GetStrikethrough();
	void SetStrikethrough(BOOL);
	short GetWeight();
	void SetWeight(short);
	short GetCharset();
	void SetCharset(short);


public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONT_H__0C1E7D84_AB94_11D2_91E0_0080C8E1242B__INCLUDED_)
