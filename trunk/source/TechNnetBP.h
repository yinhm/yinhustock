// CTaiKlineNnetBP.h: interface for the CTaiKlineNnetBP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NNETBP_H__1DA6988F_15DD_11D4_96F3_0080C8E1242B__INCLUDED_)
#define AFX_NNETBP_H__1DA6988F_15DD_11D4_96F3_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_DATA_SETTING	32
struct OutResult{
	float data[MAX_DATA_SETTING];
};
struct InKlineData{
	float dataIn[MAX_DATA_SETTING];
	float dataOut[MAX_DATA_SETTING];
	float differ;
	float differOut;
	int		time;
	char symbol[8];
};
typedef CArray<InKlineData,InKlineData> inKlineArray;
typedef CArray<OutResult,OutResult> outResultArray;
typedef CArray<inKlineArray*,inKlineArray*> propArray;
//PrepareKlineData(propArray& inArray,float fDiff,int maxOut)
#define COUNT_NEURAL_LAY1	30
#define COUNT_NEURAL_LAY2	30
#define COUNT_NEURAL_OutMax	10
#define COUNT_LAYYER_Max	36
#define COUNT_LAYYER	3

class CFormularContent;

class CTaiKlineNnetBP  
{
public:
	float m_fInRate[64];
	float m_fInRateBias[64];
	float m_fOutRate[64];
	float m_fOutRateBias[64];
	float m_fReservRate[64];

	static float RegularData(float fIn,bool bExp=true);
	CString m_sFileName;
	CFormularContent* m_pJishuIn;
	CFormularContent* m_pJishuOut;
	int m_nPeriod;
	int m_nDays;
	int m_nDaysManyDay;

	bool PrepareKlineData(inKlineArray& inArray,float& fDiff,int maxOut,int nIn,int nOut,bool bAverageOut = true,bool bStatistic = true);
	void TestExpRate(float *fInput,bool bChangeExp=true);
	void LoadWeight();
	void SaveWeight();
	static bool Test();
	int SimulatNow(float *fInput,float *fOutput);
	bool FormularCompute(float* fInput,float *fOutputReal,bool bTestSucc);
	bool Compute222(float* fInput,float& fOutput);
	bool InitNnet(int nInputValue,int nOutputValue,float fError,int nRepeat = 1000,float a=0.5,float n=0.5,bool bLoadFile = true);
	int m_nInputValue;
	int m_nOutputValue;
	float m_pWeight[COUNT_LAYYER][COUNT_LAYYER_Max][COUNT_LAYYER_Max];
	float m_pWeightPre[COUNT_LAYYER][COUNT_LAYYER_Max][COUNT_LAYYER_Max];
	float m_pWeightPost[COUNT_LAYYER][COUNT_LAYYER_Max][COUNT_LAYYER_Max];
	float m_pWeightMinSave[COUNT_LAYYER][COUNT_LAYYER_Max][COUNT_LAYYER_Max];

	float m_pStudyRate[COUNT_LAYYER][COUNT_LAYYER_Max][COUNT_LAYYER_Max];
	float m_pOldWeightRate[COUNT_LAYYER][COUNT_LAYYER_Max][COUNT_LAYYER_Max];

	float m_fError;
	int m_nRepeat;
	float m_a;
	float m_n;
	float m_e_av;
	int m_nCountAdd;
	float m_expRate[5];
	float m_diffPre;
	float m_diffLast;
	float m_expRateToTrain;

	float* m_fOut1;
	float* m_fOut2;
	float* m_fDif;
	float* m_fOutDel1;
	float* m_fOutDel2;
	float* m_fOutDel3;
	float* m_fOutput;

	bool m_bTellExpRate;
	

	CTaiKlineNnetBP();
	virtual ~CTaiKlineNnetBP();

private:
	bool TellDiffOne(float *fDif);
	bool TellDiff(float* fDif,bool bTestSucc);
	void ModifyWeight(int nLay,float* fDelIn,int nDelIn,float* fYi,int nYi);
	void GetOutValueDel(int nLay, float *fOut,int nOut,  float *fIn,int nIn,  float *fPre);
	float GetOutValue(int nLay,float* fOut,int nOut,float* fIn,int nIn);
};

#endif // !defined(AFX_NNETBP_H__1DA6988F_15DD_11D4_96F3_0080C8E1242B__INCLUDED_)
