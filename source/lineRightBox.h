
#if !defined(AFX_LINERIGHTBOX_H__0BD218B3_B64A_491A_A8E2_8B8A979F9616__INCLUDED_)
#define AFX_LINERIGHTBOX_H__0BD218B3_B64A_491A_A8E2_8B8A979F9616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class ARRAY_Right_Box  
{
public:
	void GetMaxMin(int b,int e,float &max,float &min);
	int GetFirstY(float high, float low ,int top,int bottom,float& fGap,int &nFirstFoot);
	static void FanAllPrice(float fVal, float fInitTotal , ARRAY_Right_Box &out);
	static void FanRect(float fVal,float fLow,float fHigh,ARRAY_Right_Box& out);
	ARRAY_Right_Box();
	virtual ~ARRAY_Right_Box();

	static float m_fPriceLow;
	static float m_fPriceHigh;
	static float m_fRuler;
	static int m_nCount;
	static CTime m_timeCompute;

	static void Reset();
	static void SetTime(CTime& tm);

	float * m_pData;
	static void GetRuler(float fMax, float fMin);

	friend ARRAY_Right_Box  operator +(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2);
	friend ARRAY_Right_Box  operator +(ARRAY_Right_Box& in1,float fIn2);
	friend ARRAY_Right_Box  operator -(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2);
	friend ARRAY_Right_Box  operator -(ARRAY_Right_Box& in1,float fIn2);
	friend ARRAY_Right_Box  operator *(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2);
	friend ARRAY_Right_Box  operator *(ARRAY_Right_Box& in1,float fIn2);
	friend ARRAY_Right_Box  operator /(ARRAY_Right_Box& in1,ARRAY_Right_Box& in2);
	friend ARRAY_Right_Box  operator /(ARRAY_Right_Box& in1,float fIn2);
	friend ARRAY_Right_Box  operator +(float fIn2,ARRAY_Right_Box& in1);
	friend ARRAY_Right_Box  operator -(float fIn2,ARRAY_Right_Box& in1);
	friend ARRAY_Right_Box  operator *(float fIn2,ARRAY_Right_Box& in1);
	friend ARRAY_Right_Box  operator /(float fIn2,ARRAY_Right_Box& in1);
	ARRAY_Right_Box& operator =(ARRAY_Right_Box& in);
	ARRAY_Right_Box& operator =(float fIn);
	static int PriceToFoot(float f);

private:
	static void AssertValid();

protected:
};


#endif // !defined(AFX_LINERIGHTBOX_H__0BD218B3_B64A_491A_A8E2_8B8A979F9616__INCLUDED_)
