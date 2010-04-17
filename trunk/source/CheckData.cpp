// CheckData.cpp: implementation of the CCheckData class.
// by 俞明录
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CheckData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define DLINEHEAD  24
#define DLINESMALLHEAD 4096 * 64
#define DLINECHUQ   32 * 6


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckData::CCheckData()
{

}

CCheckData::~CCheckData()
{

}


BOOL CCheckData::OverlapBlock_SH() //上海版块
{

	return 1;

}

BOOL CCheckData::OverlapBlock_SZ() //深圳版块
{

	return 1;
}

BOOL CCheckData::OverlapSymbol_SH() //上海				
{

	ASSERT(FALSE);
	
	
	return 1;
	
}

BOOL CCheckData::OverlapSymbol_SZ() //深圳			
{


	return 1;
}


BOOL CCheckData::MaxBlock_SH() 
{
	////////////////////////////////////////////////
	return 0;
	
}

BOOL CCheckData::MaxBlock_SZ() 
{
	////////////////////////////////////////////////
	return 0;
	
}
