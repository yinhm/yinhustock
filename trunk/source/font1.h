
class COleFont1 : public COleDispatchDriver
{
public:
	COleFont1() {}		
	COleFont1(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	COleFont1(const COleFont1& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}


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
