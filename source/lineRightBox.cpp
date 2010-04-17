// ARRAY_Right_Box.cpp: implementation of the ARRAY_Right_Box class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "lineRightBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
float ARRAY_Right_Box::m_fPriceLow = -1;
float ARRAY_Right_Box::m_fPriceHigh = -1;
float ARRAY_Right_Box::m_fRuler = -1;
int   ARRAY_Right_Box::m_nCount = -1;
CTime   ARRAY_Right_Box::m_timeCompute = CTime(2000,1,1,0,0,0);


ARRAY_Right_Box::ARRAY_Right_Box()
{
	if(m_nCount <= -1)
		m_pData = NULL;
	else
	{
		m_pData = new float [m_nCount];
		memset(m_pData,0,m_nCount*sizeof(float));
	}
}

ARRAY_Right_Box::~ARRAY_Right_Box()
{
	if(m_pData) delete [] m_pData;
}

void ARRAY_Right_Box::GetRuler(float fMax, float fMin)
{
	ASSERT(fMax>fMin);

	float fRuler;
	fRuler = 100;
	if(fMax-fMin<100)
		fRuler = 100;
	else if(fMax-fMin<1000)
		fRuler = 10;
	else if(fMax-fMin<10000)
		fRuler = 1;
	else 
		fRuler = 0.1;

	m_fRuler = fRuler;
	m_nCount =(int)( (fMax - fMin)*fRuler+10);
	m_fPriceLow = fMin;
	m_fPriceHigh = fMax;

	if(m_nCount<=0) m_nCount = 1;

	return ;
}
void ARRAY_Right_Box::AssertValid()
{
	ASSERT(m_fPriceLow>=0);
	ASSERT(m_fPriceHigh>=0);
	ASSERT(m_fRuler!=-1);
	ASSERT(m_nCount>0);
}
ARRAY_Right_Box  operator +(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2)
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]+in2.m_pData[i];
	}

	return rtn;
}
ARRAY_Right_Box  operator +(ARRAY_Right_Box& in1,float fIn2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]+fIn2;
	}

	return rtn;
}
ARRAY_Right_Box  operator -(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]-in2.m_pData[i];
	}

	return rtn;
}
ARRAY_Right_Box  operator -(ARRAY_Right_Box& in1,float fIn2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]-fIn2;
	}

	return rtn;
}
ARRAY_Right_Box  operator *(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]*in2.m_pData[i];
	}

	return rtn;
}
ARRAY_Right_Box  operator *(ARRAY_Right_Box& in1,float fIn2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]*fIn2;
	}

	return rtn;
}
ARRAY_Right_Box  operator /(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		if(in2.m_pData[i]!=0)
			rtn.m_pData[i] =in1.m_pData[i]/in2.m_pData[i];
		else
		{
			if(i>0)
				rtn.m_pData[i] = rtn.m_pData[i-1] ;
			else
				rtn.m_pData[i] = 0;
		}
	}

	return rtn;
}
ARRAY_Right_Box  operator /(ARRAY_Right_Box& in1,float fIn2) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		if(fIn2!=0)
			rtn.m_pData[i] =in1.m_pData[i]/fIn2;
		else
		{
			rtn.m_pData[i] = 0;
		}
	}

	return rtn;
}
ARRAY_Right_Box  operator +(float fIn2,ARRAY_Right_Box& in1) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]+fIn2;
	}

	return rtn;
}
ARRAY_Right_Box  operator -(float fIn2,ARRAY_Right_Box& in1) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =fIn2-in1.m_pData[i];
	}

	return rtn;
}
ARRAY_Right_Box  operator *(float fIn2,ARRAY_Right_Box& in1) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		rtn.m_pData[i] =in1.m_pData[i]*fIn2;
	}

	return rtn;
}
ARRAY_Right_Box  operator /(float fIn2,ARRAY_Right_Box& in1) 
{
	ARRAY_Right_Box::AssertValid();
	ARRAY_Right_Box rtn;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		if(in1.m_pData[i]!=0)
			rtn.m_pData[i] =fIn2/in1.m_pData[i];
		else
		{
			if(i>0)
				rtn.m_pData[i] = rtn.m_pData[i-1] ;
			else
				rtn.m_pData[i] = 0;
		}
	}

	return rtn;
}
ARRAY_Right_Box& ARRAY_Right_Box::operator =(ARRAY_Right_Box& in) 
{
	if(m_pData) delete [] m_pData;
	m_pData = new float [ARRAY_Right_Box::m_nCount];
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		m_pData[i] = in.m_pData[i];
	}
	return *this;
}
ARRAY_Right_Box& ARRAY_Right_Box::operator =(float fIn) 
{
	if(m_pData) delete [] m_pData;
	m_pData = new float [ARRAY_Right_Box::m_nCount];
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		m_pData[i] = fIn;
	}
	return *this;
}
void ARRAY_Right_Box::Reset()
{
	m_fPriceLow = -1;
	m_fPriceHigh = -1;
	m_fRuler = -1;
	ARRAY_Right_Box::m_nCount = -1;
	m_timeCompute = CTime(2000,1,1,0,0,0);

}
void ARRAY_Right_Box::SetTime(CTime& tm)
{
	m_timeCompute = tm;
}

void ARRAY_Right_Box::FanRect(float fVal, float fLow, float fHigh, ARRAY_Right_Box &out)
{
	int n1 = PriceToFoot(fLow);
	int n2 = PriceToFoot(fHigh);
	int nAll = n2-n1;
	if(nAll<0) nAll = 0;
	n2 = n1+nAll;
	float f = fVal/(nAll+1);
	for(int i = n1;i<=n2;i++)
	{
		out.m_pData [i]+=f;
	}
}

int ARRAY_Right_Box::PriceToFoot(float f)
{
	int n = 0;
	if(f>=m_fPriceHigh)
		n = ARRAY_Right_Box::m_nCount -1;
	else if(f>m_fPriceLow)
	{
		n = (f-m_fPriceLow)*m_fRuler;
	}
	return n;
}

void ARRAY_Right_Box::FanAllPrice(float fVal, float fInitTotal ,ARRAY_Right_Box &out)
{
	float f =1.0f;
	if(fInitTotal!=0)
		f = 1 - fVal/fInitTotal;
	for(int i = 0;i<ARRAY_Right_Box::m_nCount;i++)
	{
		out.m_pData [i]*=f;
	}
}


int ARRAY_Right_Box::GetFirstY(float high, float low, int top, int bottom, float &fGap, int &nFirstFoot)
{
	if(high-low==0)
	{
		fGap = 1000;
		nFirstFoot = 0;
		return 0;
	}
	int nBegin = bottom;

	float fScale = (top-bottom)/(high-low);
	fGap = -fScale/m_fRuler;

	if(low<m_fPriceLow)
	{
		nBegin = (m_fPriceLow-low)*fScale+bottom;
		low = m_fPriceLow;
	}

	nFirstFoot = PriceToFoot(low);
	return nBegin;
}

void ARRAY_Right_Box::GetMaxMin(int b, int e, float &max, float &min)
{
	if(!m_pData) return;

	max = m_pData [b];
	min = m_pData [b];
	for(int i = b+1;i<=e ;i++)
	{
		if(m_pData [i]>max)
			max = m_pData [i];
		if(m_pData [i]<min)
			min = m_pData [i];
	}

}
