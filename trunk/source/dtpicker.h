#if !defined(AFX_DTPICKER_H__0C1E7D85_AB94_11D2_91E0_0080C8E1242B__INCLUDED_)
#define AFX_DTPICKER_H__0C1E7D85_AB94_11D2_91E0_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COleFont;
class CPicture;

class CDTPicker : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDTPicker)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x20dd1b9e, 0x87c4, 0x11d1, { 0x8b, 0xe3, 0x0, 0x0, 0xf8, 0x75, 0x4d, 0xa1 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }


public:


public:
	long GetFormat();
	void SetFormat(long nNewValue);
	unsigned long GetCalendarBackColor();
	void SetCalendarBackColor(unsigned long newValue);
	unsigned long GetCalendarForeColor();
	void SetCalendarForeColor(unsigned long newValue);
	unsigned long GetCalendarTitleBackColor();
	void SetCalendarTitleBackColor(unsigned long newValue);
	unsigned long GetCalendarTitleForeColor();
	void SetCalendarTitleForeColor(unsigned long newValue);
	unsigned long GetCalendarTrailingForeColor();
	void SetCalendarTrailingForeColor(unsigned long newValue);
	BOOL GetCheckBox();
	void SetCheckBox(BOOL bNewValue);
	CString GetCustomFormat();
	void SetCustomFormat(LPCTSTR lpszNewValue);
	VARIANT GetDay();
	void SetDay(const VARIANT& newValue);
	VARIANT GetDayOfWeek();
	void SetDayOfWeek(const VARIANT& newValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	COleFont GetFont();
	void SetFont(LPDISPATCH newValue);
	void SetRefFont(LPDISPATCH newValue);
	VARIANT GetHour();
	void SetHour(const VARIANT& newValue);
	long GetHWnd();
	void SetHWnd(long nNewValue);
	DATE GetMaxDate();
	void SetMaxDate(DATE newValue);
	DATE GetMinDate();
	void SetMinDate(DATE newValue);
	VARIANT GetMinute();
	void SetMinute(const VARIANT& newValue);
	VARIANT GetMonth();
	void SetMonth(const VARIANT& newValue);
	CPicture GetMouseIcon();
	void SetMouseIcon(LPDISPATCH newValue);
	void SetRefMouseIcon(LPDISPATCH newValue);
	long GetMousePointer();
	void SetMousePointer(long nNewValue);
	long GetOLEDropMode();
	void SetOLEDropMode(long nNewValue);
	VARIANT GetSecond();
	void SetSecond(const VARIANT& newValue);
	BOOL GetUpDown();
	void SetUpDown(BOOL bNewValue);
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	VARIANT GetYear();
	void SetYear(const VARIANT& newValue);
	void OLEDrag();
	void Refresh();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTPICKER_H__0C1E7D85_AB94_11D2_91E0_0080C8E1242B__INCLUDED_)
