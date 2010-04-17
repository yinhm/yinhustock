// msg.cpp : implementation of the CMsg class
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

#include "stdafx.h"
#include "msg.h"
#include "CTaiShanApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsg

IMPLEMENT_DYNCREATE(CMsg, CObject)

/////////////////////////////////////////////////////////////////////////////
// CMsg construction/destruction

CMsg::CMsg()
{
	Init();
}

CMsg::~CMsg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMsg Operations

void CMsg::Init()
{
}

/////////////////////////////////////////////////////////////////////////////
// CMsg serialization

void CMsg::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar<<m_nID;
		ar << (WORD)m_bClose;
		for(int i = 0;i<128;i++)
		{
			ar <<this->m_nData [i];
		}

	}
	else
	{
		ar>>m_nID;
		WORD wd;
		ar >> wd;
		m_bClose = (BOOL)wd;
		for(int i = 0;i<128;i++)
		{
			ar >>this->m_nData [i];
		}

	}
}

/////////////////////////////////////////////////////////////////////////////
// CMsg diagnostics

#ifdef _DEBUG
void CMsg::AssertValid() const
{
	CObject::AssertValid();
}

void CMsg::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif 
