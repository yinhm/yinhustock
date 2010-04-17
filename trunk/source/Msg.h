// msg.h : interface of the CMsg class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1998 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
struct INFO_NETCARDID
{
	int m_nID;
	BOOL m_bClose;
	BYTE m_nData[128];
	INFO_NETCARDID()
	{
		m_nID = 33134999;
		m_bClose = FALSE;
		memset(m_nData,0,sizeof(m_nData));
	}
};
class CMsg : public CObject,public INFO_NETCARDID
{
protected:
	DECLARE_DYNCREATE(CMsg)
public:
	CMsg();


public:

public:
	void Init();


public:
	virtual ~CMsg();
	virtual void Serialize(CArchive& ar);   
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
