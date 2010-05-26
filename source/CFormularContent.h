#if !defined(AFX_LJISHU_H__A3E1ADE5_B7F2_11D1_83EC_0000E8593F1A__INCLUDED_)
#define AFX_LJISHU_H__A3E1ADE5_B7F2_11D1_83EC_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define Formu_Array1 CArray<CFormularContent*,CFormularContent*>
#define LEN_BYTE	240
struct FLOAT11
{
	float fValue[11];
};

struct ADDITIONAL_BUYSELL
{
	int nEnterPoint[4];	

	COLORREF nColorPoint[4];	

	BYTE bLose;	
	BYTE bWin;
	BYTE bMaxDays;
	BYTE bSubstractFee;
	BYTE bAddPreProfit;
	BYTE bUseInitFinace;
	BYTE bRes1;
	BYTE bRes2;

	float fMaxLose;
	float fMaxWin;
	float fMaxDays;
	float fInitFinace;
	float fRes1;
	float fRes2;
	float fRes3;
	float fRes4;
	ADDITIONAL_BUYSELL()
	{
		for(int i=0;i<4;i++)
			nEnterPoint[i]=5;
		nColorPoint[0]=0x00ff0000;
		nColorPoint[1]=0x0000ff00;
		nColorPoint[2]=0x00ff0000;
		nColorPoint[3]=0x0000ff00;
		bLose =0;
		bWin =0;
		bMaxDays =0;
		bSubstractFee =0;
		bAddPreProfit =0;
		bUseInitFinace =0;
		fMaxLose = -0.2f;
		fMaxWin = 0.5;
		fMaxDays = 20;
		fInitFinace = 10000;
	};
};


class CTaiShanDoc ;
class CFormularContent : public CObject
{




public:
	CFormularContent();
	union
	{
		struct
		{
			float		max[8]		;	
			float		min[8]	;	
			float		defaultVal[8];	


			float		stepLen[8]	;
			float	posX[8] ;	
			float	fReserved ;	

			int nKindPeriod;	

			BYTE	nPeriodsUsed[16];	

			BOOL	isSystem;		
			BOOL	isOfen	;		
			BOOL	isProtected;		

			BYTE	isMainFiguer;	
			BYTE	numPara		;
			BYTE 	posFlag	;	
			BYTE 	isAdditionalCond	;	

			BYTE	bYHParam[8];
			BYTE	bYH;
			BYTE	bRightBox;


			int		m_nIsFromServer;

		};
		BYTE btMemData[LEN_BYTE];
	};
	ADDITIONAL_BUYSELL * pAdditionalBS;
	CArray<FLOAT11,FLOAT11&>	defaultValArray;	

	CString		name;	
	CString	password	;
	CString	explainBrief;
	CString	explainParam;	

	CString	namePara[8]	;
	CString	fomular	;	
	CString	help	;	

	CString buyStr  ;      
	CString sellStr ;       
	CString buyStrKong  ;      
	CString sellStrKong ;     

	CString		subKindIndex;	
	CString		subKindIndexTime;	
	CString		strReserved;	

	static bool m_bVer20;
	static CStringArray m_strArrayKind[4];
	float		defaultValYH[8];	



public:
	CString GetFormular();
	static bool ExportFormular(Formu_Array1* pArr,int nArr,CString fileName,BOOL bCompleteSecret, BOOL bPassword, CString strPassword);
	static bool InstallIndicator(CString sFilePathName,bool bPromp = false,bool bFromServer = true);
	static bool InstallIndicatorCwd(CString sFilePathName,bool bPromp = false,bool bFromServer = true);
	bool IsValid();
	void SecretForm(CString& s,bool bLoad );
	void AddDefaultValToArray();
	static float GetParamDataEach(int iIndex,int nKlineType,CFormularContent* pJishu);
	static void  DeleteKindName(CString s, int nKind);
	void InitDefaultValArray();
	static void AddKindName(CString s,int nKind = 0 );
	void SerializeData(CArchive& ar);
	void SetData(CFormularContent * data);



	virtual ~CFormularContent();

	//{{AFX_VIRTUAL(CFormularContent)
	//}}AFX_VIRTUAL


protected:

	virtual void Serialize(CArchive& ar);
	DECLARE_SERIAL(CFormularContent)


	//{{AFX_MSG(CFormularContent)

	//}}AFX_MSG

public:
	static void ReadWriteIndex(CTaiShanDoc* pDoc, int nKind = 0, BOOL bRead = TRUE);
	static void ClearIndex(CTaiShanDoc* pDoc, int nKind = 0);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LJISHU_H__A3E1ADE5_B7F2_11D1_83EC_0000E8593F1A__INCLUDED_)
