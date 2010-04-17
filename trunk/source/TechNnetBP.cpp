// CTaiKlineNnetBP.cpp: implementation of the CTaiKlineNnetBP class.
//tel:13366898744
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "TechNnetBP.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define kkk	0.05
#define bbb	0.1
#define fMaxInData	0.8
#define fMinInData	-0.8
#define fMaxOutData	20.0
#define fMinOutData	-20.0
const float expRateToTrain = 1;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaiKlineNnetBP::CTaiKlineNnetBP()
{// NOTE: the ClassWizard will add member functions here
	for(int i=0;i<64;i++)
	{
		m_fInRate[i] = 1;
		m_fOutRate[i] = 1;
		m_fInRateBias[i] = 0;
		m_fOutRateBias[i] = 0;
		m_fReservRate[i] = 1;
	}

	m_sFileName = "weightSave.wgt";
	m_nPeriod = 5;
	m_nDays = 1;
	m_nDaysManyDay = 50;

	m_pJishuIn =NULL;
	m_pJishuOut=NULL;

	m_fOut1=NULL;
	m_fOut2=NULL;
	m_fDif=NULL;
	m_fOutDel1=NULL;
	m_fOutDel2=NULL;
	m_fOutDel3=NULL;
	m_fOutput=NULL;

	m_expRateToTrain=20;

	for(int i=0;i<5;i++)
		m_expRate[i] = 1;

}

CTaiKlineNnetBP::~CTaiKlineNnetBP()
{
	if(m_fOut1!=NULL) delete []m_fOut1;
	if(m_fOut2!=NULL) delete []m_fOut2;
	if(m_fDif!=NULL) delete []m_fDif;
	if(m_fOutDel1!=NULL) delete []m_fOutDel1;
	if(m_fOutDel2!=NULL) delete []m_fOutDel2;
	if(m_fOutDel3!=NULL) delete []m_fOutDel3;
	if(m_fOutput!=NULL) delete []m_fOutput;

	if(m_pJishuIn !=NULL)
		delete m_pJishuIn;
	if(m_pJishuOut !=NULL)
		delete m_pJishuOut;
}

bool CTaiKlineNnetBP::InitNnet(int nInputValue,int nOutputValue, float fError,int nRepeat,float a,float n,bool bLoadFile )
{// NOTE: the ClassWizard will add member functions here
	m_nCountAdd=0;
	m_e_av = 0;
	m_diffPre = 10000000;
	m_nInputValue = nInputValue;
	m_nOutputValue = nOutputValue;
	m_fError = fError;
	m_nRepeat=nRepeat;
	m_a = a;
	m_n= n;
	m_bTellExpRate = true;

	
	for(int i=0;i<COUNT_NEURAL_LAY1;i++)
	{

		for(int j=0;j<nInputValue+1;j++)
		{
			m_pWeight[0][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPre[0][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPost[0][i][j]=(rand()%1000-500)/(float)500;
			m_pStudyRate[0][i][j]=m_n;
			m_pOldWeightRate[0][i][j]=0;
			m_pWeightMinSave[0][i][j]=m_pWeight[0][i][j];
		}
	}

	for(int i=0;i<COUNT_NEURAL_LAY2;i++)
	{

		for(int j=0;j<COUNT_NEURAL_LAY1+1;j++)
		{
			m_pWeight[1][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPre[1][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPost[1][i][j]=(rand()%1000-500)/(float)500;
			m_pStudyRate[1][i][j]=m_n;
			m_pOldWeightRate[1][i][j]=0;
			m_pWeightMinSave[1][i][j]=m_pWeight[1][i][j];
		}
	}

	for(int i=0;i<m_nOutputValue;i++)
	{

		for(int j=0;j<COUNT_NEURAL_LAY2+1;j++)
		{
			m_pWeight[2][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPre[2][i][j]=(rand()%1000-500)/(float)500;
			m_pWeightPost[2][i][j]=(rand()%1000-500)/(float)500;
			m_pStudyRate[2][i][j]=m_n;
			m_pOldWeightRate[2][i][j]=0;
			m_pWeightMinSave[2][i][j]=m_pWeight[2][i][j];
		}
	}
	if(bLoadFile == true)
		LoadWeight();

	if(m_fOut1!=NULL)
		delete m_fOut1;
	if(m_fOut2!=NULL)
		delete m_fOut2;
	if(m_fDif!=NULL)
		delete m_fDif;
	if(m_fOutDel1!=NULL)
		delete m_fOutDel1;
	if(m_fOutDel2!=NULL)
		delete m_fOutDel2;
	if(m_fOutDel3!=NULL)
		delete m_fOutDel3;
	m_fOut1 = new float[COUNT_NEURAL_LAY1];
	m_fOut2 = new float[COUNT_NEURAL_LAY2];
	m_fDif = new float[m_nOutputValue+1];
	m_fOutDel1 = new float[m_nOutputValue+1];
	m_fOutDel2 = new float[COUNT_NEURAL_LAY2+1];
	m_fOutDel3 = new float[COUNT_NEURAL_LAY1+1];
	m_fOutput = new float[m_nOutputValue];
	return true;
}

bool CTaiKlineNnetBP::FormularCompute(float *fInput, float *fOutputReal,bool bTestSucc)
{// NOTE: the ClassWizard will add member functions here

	if(m_bTellExpRate)
		TestExpRate(fInput,false);
	float max,fRtn;
	{

	max = GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	max = FABSMY(max);
	
	fRtn = GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	if(FABSMY(fRtn)>max)
		max = FABSMY(fRtn);
	
	fRtn = GetOutValue(2, m_fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);
	if(FABSMY(fRtn)>max)
		max = FABSMY(fRtn);

	
	for(int i=0;i<m_nOutputValue;i++)
		m_fDif[i] = fOutputReal[i]-m_fOutput[i];

	
	if(bTestSucc == false)
		if(TellDiffOne(m_fDif) == true)
			return false;


	m_fOutDel1[0] = 0;
	for(int i=0;i<m_nOutputValue;i++)
	{
		m_fOutDel1[i]=m_expRate[2]*m_fDif[i]*(m_fOutput[i])*(1-m_fOutput[i]);
	}
	ModifyWeight(2, m_fOutDel1, m_nOutputValue, m_fOut2, COUNT_NEURAL_LAY2);
	
	GetOutValueDel(2, m_fOutDel2,COUNT_NEURAL_LAY2, m_fOut2 ,m_nOutputValue,  m_fOutDel1);
	ModifyWeight(1, m_fOutDel2, COUNT_NEURAL_LAY2, m_fOut1, COUNT_NEURAL_LAY1);
	
	GetOutValueDel(1, m_fOutDel3,COUNT_NEURAL_LAY1, m_fOut1 ,COUNT_NEURAL_LAY2,  m_fOutDel2);
	ModifyWeight(0, m_fOutDel3, COUNT_NEURAL_LAY1, fInput, m_nInputValue);
	
	memcpy(m_pWeightPre,m_pWeight,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	memcpy(m_pWeight,m_pWeightPost,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	}


	
	{
		if(TellDiff(m_fDif,bTestSucc))
		{
			return true;
		}
		else
			return false;
	}
	
	m_bTellExpRate = false;
	if(max>=49)
		m_bTellExpRate = true;

	return false;

}
bool CTaiKlineNnetBP::Compute222(float *fInput, float &fOutput)
{// NOTE: the ClassWizard will add member functions here
	return true;
}


float CTaiKlineNnetBP::GetOutValue(int nLay, float *fOut, int nOut, float *fIn, int nIn)
{
	float max;
	for(int i=0;i<nOut;i++)
	{
		fOut[i]=-1*m_pWeight[nLay][i][0];
		for(int j=1;j<nIn+1;j++)
		{
			fOut[i]+=(m_pWeight[nLay][i][j]*fIn[j-1]);
		}

		float f2 = FABSMY(fOut[i]);
		if(i==0)
			max = f2;
		else if(max<f2)
			max = f2;

		if(fOut[i]>50)
			fOut[i] = 50;
		if(fOut[i]<-50)
			fOut[i] = -50;

	}

	for(int i=0;i<nOut;i++)
	{
		fOut[i] = 1/(1+exp(-m_expRate[nLay]*fOut[i]));
	}

	return max;
}

void CTaiKlineNnetBP::GetOutValueDel(int nLay, float *fOut,int nOut,  float *fIn,int nIn,  float *fPre)
{
	for(int i=0;i<nOut;i++)
	{
		float f = 0;
		for(int j=0;j<nIn;j++)
		{
			f+=(fPre[j]*m_pWeight[nLay][j][i+1]);
		}
		fOut[i]=m_expRate[nLay]*f*(fIn[i])*(1-fIn[i]);
	}
}



void CTaiKlineNnetBP::ModifyWeight(int nLay, float *fDelIn, int nDelIn, float *fYi, int nYi)
{// NOTE: the ClassWizard will add member functions here
	for(int i=0;i<nDelIn;i++)
	{
		m_pWeightPost[nLay][i][0] = m_pWeight[nLay][i][0] +m_a*(m_pWeight[nLay][i][0] 
			-m_pWeightPre[nLay][i][0])+m_n*fDelIn[i]* (-1);//?
		for(int j=1;j<nYi+1;j++)
		{

			float f = fDelIn[i]* fYi[j-1];
			m_pWeightPost[nLay][i][j] = m_pWeight[nLay][i][j] 
				+m_a*(m_pWeight[nLay][i][j] -m_pWeightPre[nLay][i][j])
				+m_pStudyRate[nLay][i][j]*f;//?

			if(m_pOldWeightRate[nLay][i][j]*f>0)
				m_pStudyRate[nLay][i][j] += kkk;
			else if(m_pOldWeightRate[nLay][i][j]*f<0)
				m_pStudyRate[nLay][i][j] += (-bbb*m_pStudyRate[nLay][i][j]);
			if(m_pStudyRate[nLay][i][j]>=1)
				m_pStudyRate[nLay][i][j] = 0.999;
			if(m_pStudyRate[nLay][i][j]<=0)
				m_pStudyRate[nLay][i][j] = 0.001;
				
			m_pOldWeightRate[nLay][i][j] = f;
		}
	}

}

bool CTaiKlineNnetBP::TellDiff(float *fDif,bool bTestSucc)
{
	if(bTestSucc==true)
	{
		m_nCountAdd=0;
		m_e_av = 0;
	}
	m_nCountAdd++;
	if(m_nCountAdd==0)
		m_nCountAdd++;
	float ee = 0;
	for(int i=0;i<this->m_nOutputValue ;i++)
	{
		ee+=(fDif[i]*fDif[i]);
	}
	ee/=2;

	m_e_av+=ee;
	if(bTestSucc==true)
	{
		m_diffLast=(m_e_av/m_nCountAdd);

		static int nJump = 0;

 		if(m_diffLast<m_diffPre)
		{
			memcpy(m_pWeightMinSave,m_pWeightPre,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
			m_diffPre = m_diffLast;
			nJump = 0;
		}
 		if(m_diffLast<=this->m_fError )
			return true;

		nJump++;
		if(nJump>this->m_nRepeat /10)
		{
			if(m_diffPre<m_fError*100)
				return true;
		}
	}

	return false;
}
int CTaiKlineNnetBP::SimulatNow(float *fInput,float *fOutput)
{
	
	{
		for(int j=0;j<m_nInputValue;j++)
		{
			fInput[j] = m_fInRate[j]*fInput[j]+m_fInRateBias[j] ;
		}
	}


	GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	
	GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	
	GetOutValue(2, fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);

	
	for(int i=0;i<m_nOutputValue;i++)
	{
		fOutput[i] = RegularData(fOutput[i],false);
	}
	for(int i=0;i<m_nOutputValue;i++)
	{
		if(m_fOutRate[i]!=0)
			fOutput[i] = (fOutput[i]-m_fOutRateBias[i])/m_fOutRate[i] ;
	}

	return m_nOutputValue;
}

bool CTaiKlineNnetBP::Test()
{

	return true;

}

void CTaiKlineNnetBP::SaveWeight()
{
	if(m_pJishuIn ==NULL)
		return;
	if(m_pJishuOut ==NULL)
		return;

	CFile fl;
	CString sPath = "知识数据库\\"+m_sFileName;
	if(fl.Open (sPath,CFile::modeWrite|CFile::modeCreate)==FALSE)
		return ;


	int n = 65798809;
	fl.Write (&n,4);
	fl.Write (&m_nInputValue,4);
	n=COUNT_NEURAL_LAY1;
	fl.Write (&n,4);
	n=COUNT_NEURAL_LAY2;
	fl.Write (&n,4);
	fl.Write (&m_nOutputValue,4);
	n = 3;
	fl.Write (&n,4);

	fl.Write (&m_nPeriod,4);
	fl.Write (&m_nDays,4);
	fl.Write (&m_nDaysManyDay,4);


	char ch[32];
	strcpy(ch,this->m_pJishuIn ->name );
	ch[31]=0;
	fl.Write (ch,32);
	strcpy(ch,this->m_pJishuOut ->name );
	ch[31]=0;
	fl.Write (ch,32);

	n = this->m_pJishuIn ->numPara ;
	if(n>8)
		n=0;
	fl.Write (&n,4);
	int n2 = this->m_pJishuOut ->numPara ;
	if(n2>8)
		n2=0;
	fl.Write (&n2,4);

	for(int i=0;i<m_pJishuIn ->numPara;i++)
	{
		fl.Write (&(m_pJishuIn ->defaultVal  [i]),4);
	}
	for(int i=0;i<m_pJishuOut ->numPara;i++)
	{
		fl.Write (&(m_pJishuOut ->defaultVal  [i]),4);
	}
	for(int i=0;i<n;i++)
	{
		strcpy(ch,this->m_pJishuIn ->namePara [i] );
		ch[31]=0;
		fl.Write (ch,32);
	}
	for(int i=0;i<n2;i++)
	{
		strcpy(ch,this->m_pJishuOut ->namePara [i] );
		ch[31]=0;
		fl.Write (ch,32);
	}

	n = this->m_pJishuIn ->fomular .GetLength () ;
	if(n<0) n=0;
	fl.Write (&n,4);
	n2 = this->m_pJishuOut ->fomular .GetLength () ;
	if(n2<0) n2=0;
	fl.Write (&n2,4);
	fl.Write (this->m_pJishuIn ->fomular .GetBuffer (n),n);
	m_pJishuIn ->fomular .ReleaseBuffer ();
	fl.Write (this->m_pJishuOut ->fomular .GetBuffer (n2),n2);
	m_pJishuOut ->fomular .ReleaseBuffer ();


	fl.Write (this->m_pWeight ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	fl.Write (this->m_pStudyRate ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

	{
		fl.Write (m_fInRate,64*4);
		fl.Write (m_fOutRate,64*4);
		fl.Write (m_fInRateBias,64*4);
		fl.Write (m_fOutRateBias,64*4);
		fl.Write (m_fReservRate,64*4);
	}

	fl.Close();


	if(m_diffPre < m_diffLast)
		memcpy(m_pWeight,m_pWeightMinSave,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

}

void CTaiKlineNnetBP::LoadWeight()
{
	CFile fl;
	CString sPath = "知识数据库\\"+m_sFileName;
	if(fl.Open (sPath,CFile::modeRead)==FALSE)
		return ;


	int n = 3;
	fl.Read (&n,4);
	if(n!=65798809)
		return;

	fl.Read (&n,4);
	m_nInputValue = n;

	fl.Read (&n,4);
	if(n!=COUNT_NEURAL_LAY1)
		return;
	fl.Read (&n,4);
	if(n!=COUNT_NEURAL_LAY2)
		return;
	fl.Read (&n,4);
	m_nOutputValue=n;

	fl.Read (&n,4);
	if(n!=3)
		return;

	fl.Read (&m_nPeriod,4);
	fl.Read (&m_nDays,4);
	fl.Read (&m_nDaysManyDay,4);


	if(m_pJishuIn ==NULL)
		m_pJishuIn = new CFormularContent;
	if(m_pJishuOut ==NULL)
		m_pJishuOut = new CFormularContent;
	char ch[32];
	fl.Read (ch,32);
	m_pJishuIn ->name = ch;
	fl.Read (ch,32);
	m_pJishuOut ->name = ch;

	fl.Read (&m_pJishuIn ->numPara,4);
	if(m_pJishuIn ->numPara>8)
		m_pJishuIn ->numPara=0;
	fl.Read (&m_pJishuOut ->numPara,4);
	if(m_pJishuOut ->numPara>8)
		m_pJishuOut ->numPara=0;

	for(int i=0;i<m_pJishuIn ->numPara;i++)
	{
		fl.Read (&(m_pJishuIn ->defaultVal  [i]),4);
	}
	for(int i=0;i<m_pJishuOut ->numPara;i++)
	{
		fl.Read (&(m_pJishuOut ->defaultVal  [i]),4);
	}
	for(int i=0;i<m_pJishuIn ->numPara;i++)
	{
		fl.Read (m_pJishuIn ->namePara [i].GetBuffer (32),32);
		m_pJishuIn ->namePara [i].ReleaseBuffer ();
	}
	for(int i=0;i<m_pJishuOut ->numPara;i++)
	{
		fl.Read (m_pJishuOut ->namePara [i].GetBuffer (32),32);
		m_pJishuOut ->namePara [i].ReleaseBuffer ();
	}


	fl.Read (&n,4);
	if(n<0) n=0;
	int n2;
	fl.Read (&n2,4);
	if(n2<0) n2=0;

	char* pch = new char[n+1];
	pch[n]=0;
	fl.Read (pch,n);
	this->m_pJishuIn ->fomular = pch;
	delete []pch;
	pch = new char[n2+1];
	pch[n2]=0;
	fl.Read (pch,n2);
	this->m_pJishuOut ->fomular = pch;
	delete []pch;
	

	fl.Read (this->m_pWeight ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);
	fl.Read (this->m_pStudyRate ,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

	{
		fl.Read (m_fInRate,64*4);
		fl.Read (m_fOutRate,64*4);
		fl.Read (m_fInRateBias,64*4);
		fl.Read (m_fOutRateBias,64*4);
		fl.Read (m_fReservRate,64*4);
	}

	fl.Close();
	memcpy(m_pWeightPre,m_pWeight,COUNT_LAYYER*COUNT_LAYYER_Max*COUNT_LAYYER_Max*4);

}

bool CTaiKlineNnetBP::TellDiffOne(float *fDif)
{
	float ee = 0;
	for(int i=0;i<this->m_nOutputValue ;i++)
	{
		ee+=(fDif[i]*fDif[i]);
	}
	ee/=2;

	if(ee<=this->m_fError )
		return true;

	return false;

}

void CTaiKlineNnetBP::TestExpRate(float *fInput,bool bChangeExp)
{
	
	if(m_pWeight[0][0][m_nInputValue/2]>10E5||m_pWeight[0][0][m_nInputValue/2]<-10E5)
	{
		for(int k=0;k<1;k++)
			for(int i=0;i<COUNT_LAYYER_Max;i++)
			{
				for(int j=0;j<COUNT_LAYYER_Max;j++)
				{
					m_pWeight[k][i][j]=m_pWeight[k][i][j]/(10);
					m_pWeightPre[k][i][j]=m_pWeightPre[k][i][j]/(10);
				}
			}
	}

	
	float max = GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	if(max>=8)
	{
		m_expRate[0] = 8/max;
		GetOutValue(0, m_fOut1, COUNT_NEURAL_LAY1,fInput, m_nInputValue);
	}
	
	max = GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	if(max>=8)
	{
		m_expRate[1] = 8/max;
		GetOutValue(1, m_fOut2, COUNT_NEURAL_LAY2,m_fOut1, COUNT_NEURAL_LAY1);
	}
	
	max = GetOutValue(2, m_fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);
	if(max>=8)
	{
		m_expRate[2] = 8/max;
		GetOutValue(2, m_fOutput, m_nOutputValue,m_fOut2, COUNT_NEURAL_LAY2);
	}



}

bool CTaiKlineNnetBP::PrepareKlineData(inKlineArray &inArray, float& fDiff, int maxOut,int nIn,int nOut,bool bAverageOut,bool bStatistic )
{
	propArray propArr;
	if(inArray.GetSize() > 8000)
	{
		while(inArray.GetSize ()>8000)
		{
			inArray.RemoveAt(8000,inArray.GetSize ()-8000);
		}
	}
	if(bStatistic == true)
	{
		for(int i=0;i<inArray.GetSize ();i++)
		{
			if(i==0)
			{
				inKlineArray * inTemp = new inKlineArray;
				inArray[i].differ =0;
				inTemp->Add (inArray[i]);
				propArr.Add (inTemp);
				continue;
			}

	
			bool bNewClass = true;
			bool bContinue = false;
			for(int j=0;j<propArr.GetSize ();j++)
			{
				float ee = 0,eeOut =0;
				for(int k=0;k<nIn;k++)
				{
					float f= (inArray[i].dataIn [k]-propArr.GetAt (j)->GetAt (0).dataIn [k]);
					ee+=(f*f);
				}
				ee=ee/(nIn*2);
				for(int k=0;k<nOut;k++)
				{
					float f= (inArray[i].dataOut [k]-propArr.GetAt (j)->GetAt (0).dataOut [k]);
					eeOut+=(f*f);
				}
				eeOut=eeOut/(nOut*2);

				//
				inArray[i].differOut = eeOut;
				if(ee<fDiff)
				{
					inArray[i].differ = ee;
					propArr.GetAt (j)->Add (inArray[i]);
					bNewClass = false;
					break;
				}
			}

		
			if(bNewClass == true)
			{
				if(propArr.GetSize () >maxOut)
				{
				
					for(int j=0;j<propArr.GetSize ();j++)
					{
						propArr.GetAt (j)->RemoveAll ();
						delete propArr.GetAt (j);
					}
					propArr.RemoveAll ();

			
					fDiff = fDiff*2.0;
					i=-1;
					continue;
				}
				inKlineArray * inTemp = new inKlineArray;
				inArray[i].differ =0;
				inTemp->Add (inArray[i]);
				propArr.Add (inTemp);
			}
		}

	
		for(int i=0;i<propArr.GetSize ();i++)
		{
			int j=0,m=0,min =0;
			if(propArr[i]->GetSize ()>=3)
			{
				float fMax = propArr[i]->GetAt (0).differOut ;
				float fMin = fMax;
				for(j=1;j<propArr[i]->GetSize ();j++)
				{
					if(propArr[i]->GetAt (j).differOut >fMax)
					{
						m=j;
						fMax = propArr[i]->GetAt (j).differOut;
					}
				}
				propArr[i]->RemoveAt(m);
				for(j=1;j<propArr[i]->GetSize ();j++)
				{
					if(propArr[i]->GetAt (j).differOut <fMin)
					{
						min=j;
						fMin = propArr[i]->GetAt (j).differOut;
					}
				}
				propArr[i]->RemoveAt(min);
			}

		
			for(int k = 0;k< nOut;k++)
			{
				float out = 0;
				for(j=0;j<propArr[i]->GetSize ();j++)
				{
					out+=propArr[i]->GetAt (j).dataOut [k];
				}
				out = out/propArr[i]->GetSize ();
				propArr[i]->GetAt (0).dataOut [k] = out;
			}
		}

		

	
		inArray.RemoveAll ();
		for(int i=0;i<propArr.GetSize ();i++)
		{
			inArray.Add(propArr[i]->GetAt (0));
		}
		for(int j=0;j<propArr.GetSize ();j++)
		{
			propArr.GetAt (j)->RemoveAll ();
			delete propArr.GetAt (j);
		}
	}


	float fMax[64];
	float fMin[64];
	float fOutMax[64];
	float fOutMin[64];
	int nData = inArray.GetSize();
	int j=0;
	for(int i=0;i<nData;i++)
	{
		if(i==0)
		{
			for(j=0;j<nIn;j++)
			{
				fMax[j] = inArray[0].dataIn [j];
				fMin[j] = inArray[0].dataIn [j];
			}
			for(j=0;j<nOut;j++)
			{
				fOutMax[j] = inArray[0].dataOut [j];
				fOutMin[j] = inArray[0].dataOut [j];
			}
			
		}
		else
		{
		//
			for(j=0;j<nIn;j++)
			{
				if(fMax[j] < inArray[i].dataIn [j]) fMax[j] = inArray[i].dataIn [j];
				if(fMin[j] > inArray[i].dataIn [j]) fMin[j] = inArray[i].dataIn [j];
			}
			for(j=0;j<nOut;j++)
			{
				if(fOutMax[j] < inArray[i].dataOut [j]) fOutMax[j] = inArray[i].dataOut [j];
				if(fOutMin[j] > inArray[i].dataOut [j]) fOutMin[j] = inArray[i].dataOut [j];
			}
		}
	}
	//
	if(nData>1)
	{
		for(j=0;j<nIn;j++)
		{

			float fDf = fMax[j]-fMin[j];
			if(fDf!=0)
			{
				m_fInRate[j] = (fMaxInData-fMinInData)/fDf;
				m_fInRateBias[j] = -(fMax[j]+fMin[j])*(fMaxInData-fMinInData)/2/fDf;
			}
			else
			{
				m_fInRate[j] = 1;
				m_fInRateBias[j] = 0;
			}
		}
		for(j=0;j<nOut;j++)
		{
			float fDf = fOutMax[j]-fOutMin[j];
			if(fDf!=0)
			{
				m_fOutRate[j] = (fMaxOutData-fMinOutData)/fDf;
				m_fOutRateBias[j] = -(fOutMax[j]+fOutMin[j])*(fMaxOutData-fMinOutData)/2/fDf;
			}
			else
			{
				m_fOutRate[j] = 1;
				m_fOutRateBias[j] = 0;
			}
		}


		for(int i=0;i<nData;i++)
		{
			for(j=0;j<nIn;j++)
			{
				float f = m_fInRate[j]*inArray[i].dataIn [j]+m_fInRateBias[j] ;
				inArray[i].dataIn [j] = f;
			}
			for(j=0;j<nOut;j++)
			{
				float f = m_fOutRate[j]*inArray[i].dataOut [j]+m_fOutRateBias[j] ;
				inArray[i].dataOut [j] = f;
			}
		}

	}
	


	for(int i=0;i<nData;i++)
	{
		for(j=0;j<nOut;j++)
		{
			float f = inArray[i].dataOut [j] ;
			inArray[i].dataOut [j] = RegularData(f);
		}
	}
	return true;
}


float CTaiKlineNnetBP::RegularData(float fIn, bool bExp)
{
	float f;
	if(bExp == true)
	{
		f = 1/(1+exp(expRateToTrain	*fIn));
	}
	else
	{
		if(fIn>=0.99999)
			fIn = 0.99999;
		f = log(1/fIn-1);
	}
	return f;
}
