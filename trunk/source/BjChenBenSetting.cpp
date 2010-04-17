// BjChenBenSetting.cpp: implementation of the BjChenBenSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "BjChenBenSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BjChenBenSetting::BjChenBenSetting()// NOTE: the ClassWizard will add member functions here
{
	m_colorLine = RGB(0,0,0);
	m_nDays = 0;


}

BjChenBenSetting::~BjChenBenSetting()
{

}
BjChenBenSetting& BjChenBenSetting::operator = (BjChenBenSetting & inObject)
{
	this->m_colorLine = inObject.m_colorLine ;
	this->m_nDays = inObject.m_nDays ;

	return *this;
};
