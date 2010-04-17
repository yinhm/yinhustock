

#if !defined(AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_)
#define AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <math.h>

#include "StructKlineView.h"
#include "StructTaiShares.h"

#define NUM_LINE_TOT						64

#define MAXB 2000
#define MAXB_ALL 2000
#define PRAMANUM    8
#define MIDB 10000   
#define MIDE 10099

#define RLINEB 11000    
#define RLINEE 11000+NUM_LINE_TOT

#define CEFB 12000    
#define CEFE 12299	

#define CEIB 13000   
#define CEIE 13299

#define CECB 13500   
#define CECE 13520	

#define FuncB 8000	
#define FuncE 8899
#define FuncDllB 8400	
#define FuncDllE 8600	

#define PAM   9900  

#define PCMB  14000	
#define PCME  14199	

#define QUB  8800  
#define QUE  8899 

#define TOZERO  9009  
#define CONSB  9010 
#define CONSE  9498  
#define CONSCOLORB1  9100  
#define CONSCOLORB  9200  
#define CONSCOLORE  9280  
#define CONSLINETHICKB  9300  
#define CONSLINETHICKE  9320  
#define CONSLINEDOT  9350  


template <class TY> class Intem
{
public:
	Intem(TY v=0){val=v;next=0;prev=0;}
	Intem<TY>* next;
	Intem<TY>* prev;
	TY val;
};

template <class TH> class Intst 
{
public:
	Intst(TH val) { list = new Intem(val);tail=0;};
	Intst() {list = 0;tail=0;};

	TH push_b(TH val)
	{
		Intem<TH>* pt;
		pt=new Intem<TH>(val);
		pt->next = list;
		if(list!=0) 
			list->prev = pt;
		else
			tail = pt;
		list = pt;
		return val;
	}

	TH pop_it()
	{
	Intem<TH>* tmp;
	TH val;
	if(list==0)
		return 0;
	tmp=list;
	list=list->next;
	if(list!=0)
		list->prev=0;
	else
		tail=0;
	val = tmp->val;
	delete tmp;
	return val;
	}

	TH rec_it()
	{
		Intem<TH>* tmp;
		TH val;
		if(tail==0)
			return 0;
		tmp=tail;
		tail=tail->prev;
		if(tail!=0) 
			tail->next=0;
		else
			list=0;
		val = tmp->val;
		delete tmp;
		return val;
	}
private:
	Intem<TH>* list;
	Intem<TH>* tail;
};


class CTaiShanDoc ;
class CFormularContent;
struct INPUT_PARAM
{
	int pmax;
	Kline* pp;
	CFormularContent* pIndex;
	CString symbol;
	CBuySellList* pBuySellList;
	int nKindKline;
	bool bAddBuySell;
	int m_stkKind;

};

class CFormularComputeParent  
{
public:
	virtual int FuncInDllExt(INPUT_INFO *pInput, int nFunc,int j);
	static int GetStockKind(char * strLabel);
	static bool FanEach(CFormularContent * pJishu);
	int m_stkKind;
	bool m_bRightbox;
	
	bool m_bShowColor;
	enum ColorKind {
		digit = 0,
		operrator ,
		variable ,
		function,
		quotevar,
		};
	struct KindPos
	{
		int m_nKind;
		int m_nPos;
	};
	int m_nSizeKindPos;
	KindPos* m_pKindPos;
	int m_nPosPre;

	int GetIndexPoint(CString sName,float &nPoint);
	KlineEx *m_pKlineEx ;
	ARRAY_BE  m_Rline[RLINEE-RLINEB];  
	CString m_RlineName[RLINEE-RLINEB];  
	int     m_RlineNum;             

	CString m_formuCompute;  
	CString m_errmsg;  
	int     m_errpos;

	bool m_bTotalData;
	int m_isDaPan;
	int m_RlineType[NUM_LINE_TOT];
	int m_RlineColor[NUM_LINE_TOT];	
	int m_RlineThick[NUM_LINE_TOT];

	CString GetRlineName(int foot);
	
	void IsDaPan();	
	int FindLineType();
	int YhhCheck(int j, int k);
						
	int GetRlineNum();
	int GetNum(int tj);
	
	CFormularComputeParent(int pmin,int pmax,Kline* pp,CString& str); 
	CFormularComputeParent();
	CFormularComputeParent(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell = true);
		

	virtual ~CFormularComputeParent();
	int AddPara(CString pa,int va); 
	int AddPara(CString pa,float va);
	int AddLine(CString& LineName,ARRAY_BE& pline);
									
	void GetMsg(CString& str,int& pos);

	int GetLine(int& index,ARRAY_BE& pline,CString& name);
									  
	int GetIndexPoint(int &index,float &nPoint);

	int GetLine(CString& LineName,ARRAY_BE& pline);
									
	int  FormularCompute();   

	int Relolve(int cl);

	void Devide();     
	void GetData(int cl,ARRAY_BE& pp);
	int Sort(int bi,int ej);    
	int CheckWord(CString emp);	
	
	int GetCharClass(char ch);   
	int CheckSent();		


protected:
	int m_nCurrentFoot;
	void FuncZigPre(INPUT_INFO *pInput,int nFunc);
	int CalcMin5InDay(CFormularContent*	pIndex,int nParamInTable,CString linename,int tu);
	int GetBeginFoot(ARRAY_BE& ln2);
	int RelolveNnet(int cl);
	int m_tempWhich;

	int m_MaxPoint;   
	int m_MinPoint;    

	CString  m_QuoteArray[QUE-QUB];	
	int      m_QuoteNum;

	CString  m_stringArray[CECE-CECB];	
	int      m_stringNum;	

	CString m_strColor[CONSCOLORE-CONSCOLORB+1];
	int      m_strColorNum;

	int	 m_Sent[MAXB];  
	int  m_SentNum;     

	int	 m_ComSort[MAXB_ALL];    
	int  m_ComSortNum;		 
	int  m_Post[MAXB];  

	ARRAY_BE  m_VarArray[MIDE-MIDB]; 
	int     m_VarNum;			 
	CString m_VarName[MIDE-MIDB];           

	int   m_floatNum;			
	float m_floatArray[CEFE-CEFB];

	CString  m_NumName[PRAMANUM];    
	int      m_NumGs;		  	

	int   m_intNum;	
	int   m_intArray[CEIE-CEIB]; 

	ARRAY_BE m_MidArray[PCME-PCMB];
	int    m_MidNum;    

	Kline* m_pData;     
	Intst<int> m_table;	

	CMapStringToPtr m_WordTableNew;
	CMap<int,int,int,int> m_ParameterTableNew;
	static CMapStringToPtr m_LetterTable;	
	static CMap<int,int,int,int> m_ParameterTable;
	static CMapStringToPtr m_WordTable;	

	
	static int FuncZigGroup(INPUT_INFO *pInput, void* pArray);
	static int GetFoot(INPUT_INFO *pInput, int nParam);
	int FuncInDll(INPUT_INFO* pInput,int  nFunc);

	int FuncFromDll(int n);
	static void InitStatic();
	bool LookupParameterTable(int nKey,int& nValue);
	bool LookupWordTable(CString sKey,int& n);
	void GetHsKlineData(int cl,ARRAY_BE& pp);
	void TransferRealInfo(int bl,int be,int nKind);
	void KlineToLine(Kline* pKline,float& f,int nFlag);
	int Func847();
	Kline* m_pKlineIndex;

	CString m_symbol;
	CBuySellList * m_pBuySellList; 
	CTaiShanDoc * pDoc;
	
    int Func101();
	int Func102();
	int Func301();
	int Func302();
	int Func401();
	int Func402();
	int Func405();
	int Func406();
	int Func408();
	int Func409();
	int Func501();
	int Func502();
	int Func601();
	int Func602();
	int Func603();
	int Func604();
	int Func801();
	int Func802();
	int Func803();
	int Func804();
	int Func805();
	int Func806();
	int Func807();
	int Func808();
	int Func809();
	int Func810();
	int Func811();
	int Func812();
	int Func813();
	int Func830();
	int Func831();
	int Func832();
	int Func833();
	int Func834();
	int Func835();
	int Func836();
	int Func837();
	int Func838();
	int Func839();
	int Func840();
	int Func841();
	int Func842();
	int Func843();
	int Func844();
	int Func845();
	int Func846();
	
	void GetLineNameArray(CString fml,CStringArray& sArray);
	int GetPeriodKind(CString sIn);
	int ReadKline(CString s,int stkKind,Kline*& pKline);
	CFormularContent*	m_pIndex;
	bool m_bCreateBsList;
	bool GetBuySellList();
	int m_nKindKline;
	CReportData* m_cdat1;
	int Func853();
	int Func852();
	int Func848_51(int nKind);
	Kline* GetKlineIndex();
	void InitEquation(int pmin,int pmax,Kline* pp,CString& str);
	int GetQuote(CString &str, CString &zb, CString &cline, int &tu,int &equationKind,int &nPeriods);//equationKind is the kind of CFormularContent(0 is zhibiao ,1 is xuangu,2 is klinemode,3 is stock system,10 is other stock's kline price, 11 is the string including a periods(such as #week))
	int m_exec;



protected:
	void FanEach(ARRAY_BE& lineIn,int nFoot,ARRAY_BE& lineOut);
};

#endif // !defined(AFX_COM_H__80BFF943_AB2A_11D1_8C43_0000E823CF90__INCLUDED_)
