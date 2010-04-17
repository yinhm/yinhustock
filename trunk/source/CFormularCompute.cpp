// CFormularCompute.cpp: implementation of the CFormularCompute class.
// Tel:13366898744
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFormularCompute.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//应用程序类，主程序初始化，初始数据读取等
CFormularCompute::CFormularCompute()
{

}

CFormularCompute::~CFormularCompute()
{

}
CFormularCompute::CFormularCompute(int pmin,int pmax,Kline* pp,CString& str):CFormularComputeParent(pmin, pmax, pp, str)
{
	InitStatic2();
}
CFormularCompute::CFormularCompute(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell ):CFormularComputeParent(pmin, pmax, pp, pIndex, symbol, stkKind, pBuySellList, nKindKline, bAddBuySell)
{
	InitStatic2();
}

#define BEGIN_NUMBER1	64
void CFormularCompute::InitStatic2()
{
	static bool bExec = false;
	if(bExec==true) return;
	int nParam[]={  1
	};
	CString sName[]={"sin2"
	};
	int nb = FuncDllB+BEGIN_NUMBER1;
	for(int j=0;j<sizeof(nParam)/sizeof(int);j++)
	{
		m_WordTable[sName[j]] =(void *)(nb +j );
		m_ParameterTable[nb + j]         =nParam[j];
	}
	
	
}
int ComputeFormu2(CFormularCompute*& pEqution,INPUT_PARAM& input)
{//子窗口框架
	ASSERT(pEqution == NULL);
	pEqution = new CFormularCompute(0,input. pmax,input. pp,input. pIndex,input. symbol,input.m_stkKind,input. pBuySellList,input. nKindKline,input. bAddBuySell);
		
	int numPa=input. pIndex->numPara ;
	if(numPa>8)
		numPa = 8;
	if(numPa<0)
		numPa = 0;
	for(int j=0;j<numPa;j++)
	{
		input. pIndex->namePara [j].MakeLower();
		pEqution->AddPara (input. pIndex->namePara [j],
			CFormularContent::GetParamDataEach(j,input. nKindKline, input. pIndex));
	}
	
	return pEqution->FormularCompute();
}

int CFormularCompute::FuncInDllExt(INPUT_INFO *pInput, int nFunc, int j)
{//应用程序类
	if(nFunc<BEGIN_NUMBER1) return 0;

	nFunc-=BEGIN_NUMBER1;
	int i= 0;
	switch(nFunc)
	{
	case 0:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = sin(pInput->fInputParam[0].line [i]);
		pInput->fOutputParam.b=j;
		break;
	}
	return 0;
}
