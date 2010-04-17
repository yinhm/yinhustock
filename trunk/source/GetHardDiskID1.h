
#if !defined(AFX_GETHARDDISKID1_H__C91B42DC_A5CA_4499_98BF_5C244568FA22__INCLUDED_)
#define AFX_GETHARDDISKID1_H__C91B42DC_A5CA_4499_98BF_5C244568FA22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGetHardDiskID  
{
public:
	static CString GetID(CString &sError);
	CGetHardDiskID();
	virtual ~CGetHardDiskID();

};

#endif // !defined(AFX_GETHARDDISKID1_H__C91B42DC_A5CA_4499_98BF_5C244568FA22__INCLUDED_)
