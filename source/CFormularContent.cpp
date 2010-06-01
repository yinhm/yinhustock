
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CFormularContent.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"

#include "CTaiScreenParent.h"
#include "KEYBRODEANGEL.h"
#include "CwdOverWriteF.h"
#include "secretdrvhyt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CStringArray CFormularContent::m_strArrayKind[4];

BOOL CFormularContent::m_bVer20 = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CFormularContent

IMPLEMENT_SERIAL(CFormularContent, CObject, 1)

CFormularContent::CFormularContent()
{
	memset(this->btMemData,0,LEN_BYTE);
	for(int i=0;i<16;i++)
		nPeriodsUsed[i]=1;	
	isOfen	= FALSE;		
	pAdditionalBS = NULL;

	{
		name = _T("");		
		password	 = _T("");	
		explainBrief = _T("");	
		explainParam = _T("");	

		for(int i=0;i<8;i++)
			namePara[i]	 = _T("");	
		fomular	 = _T("");		
		help	 = _T("");	

		buyStr   = _T("");      
		sellStr  = _T("");      
		buyStrKong   = _T("");      
		sellStrKong  = _T("");      

		subKindIndex = _T("");		
		subKindIndexTime = _T("");		
		strReserved = _T("");		
	}

	bYH = 0;
}

CFormularContent::~CFormularContent()
{
	if (pAdditionalBS != NULL)
		delete pAdditionalBS;
}

void CFormularContent::SetData(CFormularContent *data)
{// NOTE: the ClassWizard will add member functions here
	for(int i=0;i<LEN_BYTE;i++)
		btMemData[i]=data->btMemData[i];
	if(pAdditionalBS != NULL)
	{
		isAdditionalCond = 1;
	}
	isAdditionalCond = data->isAdditionalCond;
	if(isAdditionalCond == 1)
	{
		if(pAdditionalBS == NULL)
			pAdditionalBS = new ADDITIONAL_BUYSELL;
		memcpy(pAdditionalBS,data->pAdditionalBS,sizeof(ADDITIONAL_BUYSELL));
	}

	defaultValArray.RemoveAll();
	int nArray =data->defaultValArray.GetSize();
	for(int i=0;i<nArray;i++)
	{
		defaultValArray.Add(data->defaultValArray[i]);
	}

	{
		name=data->name ;		
		password	 =data->password	 ;	
		explainBrief=data->explainBrief ;	
		explainParam=data->explainParam ;

		for(int i=0;i<8;i++)
			namePara[i]=data->namePara[i]	 ;	
		fomular=data->fomular	 ;
		help=data->help	 ;		

		buyStr	=data->buyStr		;       
		sellStr	=data->sellStr		;       
		buyStrKong =data->buyStrKong   ;      
		sellStrKong=data->sellStrKong  ;       

		subKindIndex=data->subKindIndex ;	
		subKindIndexTime=data->subKindIndexTime ;	
		strReserved=data->strReserved	;	
	}
}



float CFormularContent::GetParamDataEach(int iIndex, int nKlineType, CFormularContent *pJishu)
{
	if(pJishu->bYH!=0) 
	{
		return pJishu->defaultValYH [iIndex];
	}
	float f = pJishu->defaultVal[iIndex];



	if(((CTaiShanDoc*)(CMainFrame::m_taiShanDoc))->m_propertyInitiate.bSaveParam == TRUE)
	{
		if(pJishu->defaultValArray .GetSize()>iIndex)
		{
			int nInd = CTaiScreenParent::FromKlineKindToFoot(nKlineType);
			if(nInd>=0)
				f = pJishu->defaultValArray[iIndex].fValue[nInd];
		}
	}
	return f;
}

void CFormularContent::SecretForm(CString &s,bool bLoad)
{//Default FontName
	if(this->isProtected == TRUE)
	{


		if(bLoad==false)
		{
			int nLen = s.GetLength()+1;
			int nLen2 = (nLen/8+1)*8;
			char* pch = s.GetBuffer(nLen2+1);

			pch[nLen-1] = '\0';
			for(int k=nLen;k<nLen2;k++)
			{
				pch[k] = 'a'+rand()%20;
			}
			CSecretDrv2 secr;
			for(int k=0;k<nLen2;k+=8)
			{
				secr.secret ((BYTE*)(pch+k), (BYTE*)(pch+k));
			}

			CString s2;
			for(int k=0;k<nLen2;k++)
			{
				CString ss;
				ss.Format("%2x",(BYTE)(pch[k]));
				s2+=ss;
			}
			pch[nLen2]='\0';

			s.ReleaseBuffer();
			s = s2;
		}
		else
		{
			int nLen = s.GetLength();
			int nLen2 = (nLen/8)*8;
			char* pch = s.GetBuffer(nLen2);
			ASSERT(nLen2 == nLen);

			CString s2;
			CSecretDrv2 secr;
			for(int k=0;k<nLen2;k+=2)
			{
				BYTE byt;
				byt = (BYTE)secr.From16ToInt(pch+k);
				pch[k/2] = byt;
			}

			for(int k=0;k<nLen2/2;k+=8)
			{
				secr.desecret ((BYTE*)(pch+k), (BYTE*)(pch+k));
			}
			pch[nLen2/2] = '\0';

			s.ReleaseBuffer();
		}

	}

}

bool CFormularContent::InstallIndicator(CString sFilePathName,bool bPromp,bool bFromServer)
{
	CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;
	CString filename=sFilePathName;
	CFile   file;
	if(file.Open(filename,CFile::modeRead)==0)
	{
		if(bPromp == true)
			AfxMessageBox(filename+"文件不存在");

		return false;
	}

	CArchive ar(&file,CArchive::load);
	int selectcount,ncurtab;
	ar>>selectcount;
	if(selectcount > 0)
		CFormularContent::m_bVer20 = false;
	else
	{
		CFormularContent::m_bVer20 = true;
		ar>>selectcount;
	}

	ar>>ncurtab;
	int readpos=0;
	Formu_Array1* pArray[] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose ),&(pDoc->m_formuar_kline )};

	for(int i=0;i<selectcount;i++)
	{
		BOOL isoverwrite=TRUE;
		BOOL isfind=FALSE;

		CFormularContent *jishu=NULL;
		ar>>jishu;


		if(bFromServer == true)// serious allocation failure checking
		{
			jishu->subKindIndex = "服务端特色指标";
			jishu->m_nIsFromServer = 33134999;
			jishu->buyStr = "";
			if(jishu->isProtected == FALSE)
				jishu->isProtected = TRUE;
			if(jishu->password == "")
				jishu->password = "33134999lmb";
		}
		AddKindName(jishu->subKindIndex, ncurtab);


		if(jishu->buyStr .GetLength () >0)
		{
			int nLen2 = jishu->buyStr.GetLength ();
			CString ss = pDoc->m_strSerial;


			if(jishu->buyStr .CompareNoCase ( ss)!=0)
			{
				if(bPromp == true) AfxMessageBox("此公式不能用于您的软件！");
				continue;
			}
			else
			{
				if(jishu->password .GetLength ()<=0)
					jishu ->isProtected = FALSE;
			}
		}

		CString name=jishu->name;
		int k,length;
		length=pArray[ncurtab]->GetSize();
		for( k=0;k<length;k++)
		{
			int result=name.Compare(pArray[ncurtab]->GetAt(k)->name);
			if(result>0)          		 
				continue;
			else
				if(result==0)
				{
					isfind=TRUE;
					if(bPromp == true)
					{
						if(AfxMessageBox(name+"公式已存在，是否要覆盖？",MB_YESNO|MB_ICONQUESTION)==IDNO)
							isoverwrite=FALSE;
					}

					break;
				}
				else
					break;
		}
		if(isfind)
		{
			if(isoverwrite)
			{
				CFormularContent *jishutodel=pArray[ncurtab]->GetAt(k);
				delete jishutodel;
				pArray[ncurtab]->SetAt(k,jishu);
			}


		}// serious allocation failure checking
		else
		{
			pArray[ncurtab]->InsertAt(k,jishu);

			if(ncurtab == 0)
			{

				CString name=jishu->name;
				CTaiKeyBoardAngelDlg dlg;
				dlg.AddIndicatorName(name);

			}
		}
		readpos=k;            
	}
	ar.Close();
	file.Close();

	CFormularContent::m_bVer20 = false;

	return true;

}
bool CFormularContent::InstallIndicatorCwd(CString sFilePathName,bool bPromp,bool bFromServer)
{
	CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;
	CString filename=sFilePathName;
	CFile   file;
	if(file.Open(filename,CFile::modeReadWrite))
	{
		char ch[32] ;
		file.Read (ch,32);
		ch[31] = 0;
		CString ss = ch;

		if(ss == "YBW SECRET FILE1.0")
		{
			int nLen = file.GetLength ();
			if(nLen<=0)
				nLen = 1;
			BYTE * byt = new BYTE[nLen-32];
			file.Read (byt,nLen-32);
			for(int j = 0;j<nLen-7-32;j+=7)
			{
				for(int k = 0;k<7/2;k++)
				{
					BYTE  tmp = byt[j+k];
					byt[j+k] = byt[j+6-k];
					byt[j+6-k] = tmp;
				}
			}
			file.SeekToBegin ();
			file.Write (byt,nLen-32);

		}
	}
	else// serious allocation failure checking
	{
		if(bPromp == true)
			AfxMessageBox("文件不存在或文件只读！请把文件设为可写属性！");
		return false;
	}
	file.SeekToBegin ();
	CArchive ar(&file,CArchive::load);
	int selectcount,ncurtab;
	ar>>selectcount;

	int nTotalKind = 1;
	if(selectcount > 0)
	{
		CFormularContent::m_bVer20 = false;
		ar>>ncurtab;
	}
	else
	{
		nTotalKind = 3;
		CFormularContent::m_bVer20 = true;
		ar>>selectcount;
		ar>>selectcount;
		ar>>ncurtab;
	}

	Formu_Array1* pArray[] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose ),&(pDoc->m_formuar_kline )};

	int isoverwrite=0;
	for(int k = 0;k<nTotalKind;k++)
	{
		int readpos=0;
		for(int i=0;i<selectcount;i++)
		{
			if(isoverwrite == 0)
				isoverwrite=1;
			BOOL isfind=FALSE;

			CFormularContent *jishu=NULL;
			ar>>jishu;


			if(bFromServer == true)
			{
				jishu->subKindIndex = "服务端特色指标";
				jishu->m_nIsFromServer = 33134999;
				jishu->buyStr = "";
				if(jishu->isProtected == FALSE)
					jishu->isProtected = TRUE;
				if(jishu->password == "")
					jishu->password = "33134999lmb";
			}
			AddKindName(jishu->subKindIndex, ncurtab);


			if(jishu->buyStr .GetLength () >0)
			{
				int nLen2 = jishu->buyStr.GetLength ();
				CString ss = pDoc->m_strSerial;


				if(jishu->buyStr .CompareNoCase ( ss)!=0)
				{
					if(bPromp == true) AfxMessageBox("此公式不能用于您的软件！");
					continue;
				}
				else
				{
					if(jishu->password .GetLength ()<=0)
						jishu ->isProtected = FALSE;
				}
			}


			CString name=jishu->name;// serious allocation failure checking
			int k,length;
			length=pArray[ncurtab]->GetSize();
			for( k=0;k<length;k++)
			{
				int result=name.Compare(pArray[ncurtab]->GetAt(k)->name);
				if(result>0)          		 
					continue;
				else
					if(result==0)
					{
						isfind=TRUE;
						if(bPromp == true && isoverwrite <2)
						{
							CFxjOverWriteF dlg;
							dlg.m_s1 = name+"公式已存在，是否要覆盖？";
							dlg.DoModal ();
							isoverwrite = dlg.m_nResponse ;

						}

						break;
					}
					else
						break;
			}
			if(isfind)
			{
				if(isoverwrite)
				{
					CFormularContent *jishutodel=pArray[ncurtab]->GetAt(k);
					delete jishutodel;
					pArray[ncurtab]->SetAt(k,jishu);
				}


			}// serious allocation failure checking
			else
			{
				pArray[ncurtab]->InsertAt(k,jishu);

				if(ncurtab == 0)
				{

					CString name=jishu->name;
					CTaiKeyBoardAngelDlg dlg;
					dlg.AddIndicatorName(name);
				}
			}
			readpos=k;           
		}

		if(k == nTotalKind-1) break;
		//Do not modify the contents of this file.
		ar>>selectcount;
		ar>>ncurtab;
	}
	ar.Close();
	file.Close();

	CFormularContent::m_bVer20 = false;

	return true;

}


bool CFormularContent::ExportFormular(Formu_Array1 *pArr, int nArr,CString fileName,BOOL bCompleteSecret, BOOL bPassword, CString strPassword)
{
	//Do not modify the contents of this file.
	int nTotal = 0;
	for(int i = 0;i<nArr;i++)
	{
		nTotal+=pArr[i].GetSize ();
	}
	if(nTotal==0)
		return false;
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc ;

	CString     defaultname="";              

	int ncurtab=0;
	CString sArray="*.fnc";       
	CFormularContent::m_bVer20 = false;
	{
		CString filename=fileName;
		if(filename.Find('.') == -1)
		{
			CString temp = sArray;
			temp.TrimLeft ("*");
			filename+=temp;
		}

		CFile   file;
		if(file.Open(filename,CFile::modeRead)!=0)
		{
			if(AfxMessageBox(filename+"文件已存在，要覆盖吗？",MB_YESNO|MB_ICONQUESTION)==IDNO)
			{
				file.Close();

				return false;
			}
			file.Close();
		}
		CFormularContent::m_bVer20 = true;
		file.Open(filename,CFile::modeCreate|CFile::modeWrite);
		CArchive ar(&file,CArchive::store);
		int selectcount=0;
		ar<<selectcount;
		ar<<selectcount;

		int nOut = 0;
		Formu_Array1* pArray = pArr;

		int nOutAll = 0;
		for(int j =0;j<nArr;j++)
		{
			nOut = 0;
			for(int i=0;i<pArray[j].GetSize ();i++)
			{
				CFormularContent *jishu;
				jishu=pArray[j].GetAt(i);         		 

				CString buyStr = jishu->buyStr;
				if(buyStr=="" && jishu->sellStr == "")
					nOut++;
			}
			nOutAll+=nOut;

			ar<<nOut;
			ar<<j;
			for(int i=0;i<pArray[j].GetSize ();i++)
			{
				CFormularContent *jishu;
				jishu=pArray[j].GetAt(i);        		 

				CString buyStr = jishu->buyStr;
				if(buyStr=="" && jishu->sellStr == "")
				{
					int isProt = jishu->isProtected;
					CString sP = jishu->password;
					CString sOut = jishu->sellStr ;
					if(bPassword)
					{
						if( jishu->isProtected == 0 )
						{
							jishu->isProtected = 1;
							jishu->password = strPassword;
						}
					}
					if(bCompleteSecret)
					{
						jishu->sellStr = "dls";
						jishu->isProtected = 1;
						if(jishu->password == "")
						{
							for(int k = 0; k<108; k++)
							{
								char ch = (char)((rand()+128)%255+1);
								CString s(ch);

								jishu->password += s;
							}
						}
					}

					ar<<jishu;
					jishu->isProtected = isProt;
					jishu->password  = sP;
					jishu->sellStr  = sOut;
				}
			}
		}

		CString lengthstr;
		lengthstr.Format("%d",nOutAll);
		if(nTotal>nOutAll)
			AfxMessageBox("部分完全加密的公式不允许导出！");
		AfxMessageBox("一共在文件里写了"+lengthstr+"个公式");
		ar.Close();
		//Do not modify the contents of this file.
		file.Close();
		CFormularContent::m_bVer20 = false;

		if(nOutAll<=0) 
			return false;
		if(bCompleteSecret)
		{
			if(file.Open(filename,CFile::modeReadWrite))
			{
				int nLen = file.GetLength ();
				if(nLen<=0)
					nLen = 1;
				BYTE * byt = new BYTE[nLen];

				file.Read (byt,nLen);
				for(int j = 0;j<nLen-7;j+=7)
				{
					for(int k = 0;k<7/2;k++)
					{
						BYTE  tmp = byt[j+k];
						byt[j+k] = byt[j+6-k];
						byt[j+6-k] = tmp;
					}
				}

				char ch[32] = "YBW SECRET FILE1.0";
				file.SeekToBegin ();
				file.Write (ch,32);
				file.Write (byt,nLen);

				file.Close();
			}

		}
	}

	return true;
}

const char g_kxzh_hz[64] = "System\\KlineGroup";
const char g_tixg_hz[64] = "System\\ScreenData";
const char g_zbgs_hz[64] = "System\\IndexData";

void CFormularContent::ReadWriteIndex(CTaiShanDoc* pDoc, int nKind, BOOL bRead)
{
	if (pDoc == NULL)
		return;

	m_bVer20 = FALSE;

	CString sName = g_zbgs_hz;
	Formu_Array1* pArray = &(pDoc->m_formuar_index);
	if (nKind == 1)
	{
		sName = g_tixg_hz;
		pArray = &(pDoc->m_formuar_choose);
	}
	else if (nKind == 2)
	{
		sName = g_kxzh_hz;
		pArray = &(pDoc->m_formuar_kline);
	}

	try
	{
		if (bRead == FALSE)
		{
			m_bVer20 = TRUE;

			CFile gsfile;
			gsfile.Open(sName + ".tst", CFile::modeCreate | CFile::modeWrite);
			CArchive ar(&gsfile, CArchive::store);

			int length = -1;
			ar << length;
			length = pArray->GetSize();

			int ii = 0;
			for (int i = 0; i < length; i++)
			{
				if (pArray->GetAt(i)->m_nIsFromServer != 33134999)
					ii++;
			}
			ar << ii;

			for (int i = 0; i < length; i++)
			{
				CFormularContent* jishu;
				jishu = pArray->GetAt(i);

				if (jishu->m_nIsFromServer != 33134999)
					ar << jishu;
			}

			ar.Close();
			gsfile.Close();

			DeleteFile(sName);

			CFile::Rename(sName + "tst", sName);
		}
		else
		{
			CFile gsfile;
			gsfile.Open(sName, CFile::modeRead);
			CArchive ar(&gsfile, CArchive::load);

			int length;
			ar >> length;
			if (length == -1)
			{
				m_bVer20 = TRUE;
				ar >> length;
			}

			m_strArrayKind[nKind].RemoveAll();
			m_strArrayKind[nKind].Add("其它");

			for (int i = 0; i < length; i++)
			{
				CFormularContent* jishu;
				ar >> jishu;
				pArray->Add(jishu);

				AddKindName(jishu->subKindIndex, nKind);
			}

			ar.Close();
			gsfile.Close();
		}
	}
	catch (...)
	{
	}

	m_bVer20 = FALSE;
}

void CFormularContent::AddKindName(CString strName, int nKind)
{
	if (strName == "")
		return;

	BOOL bFind = FALSE;

	for (int j = 0; j < m_strArrayKind[nKind].GetSize(); j++)
	{
		if (m_strArrayKind[nKind][j] == strName)
		{
			bFind = TRUE;
			break;
		}
	}

	if (bFind == FALSE)
	{
		if (m_strArrayKind[nKind].GetSize() <= 0)
		{
			m_strArrayKind[nKind].Add(strName);
			return;
		}

		for (int j = 0; j < m_strArrayKind[nKind].GetSize(); j++)
		{
			int k = strName.Compare(m_strArrayKind[nKind][j]);
			if (k < 0)
			{
				m_strArrayKind[nKind].InsertAt(j, strName);
				return;
			}
		}

		m_strArrayKind[nKind].Add(strName);
	}
}

void CFormularContent::DeleteKindName(CString strName, int nKind)
{
	if (strName == "")
		return;

	for (int j = 0; j < m_strArrayKind[nKind].GetSize(); j++)
	{
		if (m_strArrayKind[nKind][j] == strName)
		{
			m_strArrayKind[nKind].RemoveAt(j);
			break;
		}
	}
}

void CFormularContent::ClearIndex(CTaiShanDoc* pDoc, int nKind)
{
	if (pDoc == NULL)
		return;

	Formu_Array1* pArray = &(pDoc->m_formuar_index);
	if (nKind == 1)
	{
		pArray = &(pDoc->m_formuar_choose);
	}
	else if (nKind == 2)
	{
		pArray = &(pDoc->m_formuar_kline);
	}

	int nIndex = pArray->GetSize();
	for (int i = 0; i < nIndex; i++)
	{
		CFormularContent* jishu = NULL;
		jishu = pArray->GetAt(i);

		if (jishu)
		{
			delete jishu;
		}
	}

	pArray->RemoveAll();
}

void CFormularContent::AddDefaultValToArray()
{
	defaultValArray.RemoveAll();
	FLOAT11 f11;

	for (int i = 0; i < numPara; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			f11.fValue[j] = defaultVal[i];
		}

		defaultValArray.Add(f11);
	}
}

void CFormularContent::InitDefaultValArray()
{
	if (defaultValArray.GetSize() != numPara)
	{
		defaultValArray.RemoveAll();
		for (int j = 0; j < numPara; j++)
		{
			FLOAT11 f11;
			for (int i = 0; i < 11; i++)
			{
				f11.fValue[i] = defaultVal[j];
			}
			defaultValArray.Add(f11);
		}
	}
}

CString CFormularContent::GetFormular()
{
	CString sFormular;

	if (buyStr != "")
		sFormular = "";
	else if (sellStr != "" )
		sFormular = "";
	else if (isProtected )
		sFormular = "此公式被密码保护！";
	else 
		sFormular = fomular;

	return sFormular;

}

BOOL CFormularContent::IsValid()
{
	CTaiShanDoc* pDoc = CMainFrame::m_taiShanDoc;

	if (buyStr.GetLength() > 0)
	{
		if (buyStr.CompareNoCase(pDoc->m_strSerial) != 0)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CFormularContent::SerializeData(CArchive &ar)
{// NOTE: the ClassWizard will add member functions here
	int i;
	if(ar.IsStoring())
	{
		for(i=0;i<LEN_BYTE;i++)
			ar<<btMemData[i];

		int nTemp = 62332442;
		ar<<nTemp;

		if(pAdditionalBS != NULL)
		{
			isAdditionalCond = 1;
		}
		ar<<isAdditionalCond;
		if(isAdditionalCond == 1)
		{

			BYTE * pb = (BYTE*)pAdditionalBS;
			for(i=0;i<sizeof(ADDITIONAL_BUYSELL);i++)
			{
				ar<<pb[i];
			}
		}

		BYTE nArray = (BYTE)defaultValArray.GetSize();
		if(nArray!=this->numPara) nArray = 0;

		ar<<nArray;
		for(i=0;i<nArray;i++)
		{
			float*  pf11 = (float*)(&(defaultValArray[i]));
			for(int j=0;j<11;j++)
			{
				ar<<pf11[j];
			}
		}

		{
			ar<<name ;		
			CString s2 = password;


			SecretForm(s2,false);
			ar<<s2	 ;
			ar<<explainBrief ;	
			ar<<explainParam ;	

			for(i=0;i<8;i++)
				ar<<namePara[i]	 ;


			s2 = fomular;
			SecretForm(s2,false);

			ar<<s2	 ;		
			ar<<help	 ;		

			ar<<buyStr   ;     
			ar<<sellStr  ;   
			ar<<buyStrKong   ;      
			ar<<sellStrKong  ;      

			ar<<subKindIndex ;		
			ar<<subKindIndexTime ;		
			ar<<strReserved ;	
		}
	}
	else
	{
		for(i=0;i<LEN_BYTE;i++)
			ar>>btMemData[i];
		int nTemp = 62332442;
		ar>>nTemp;


		ar>>isAdditionalCond;
		if(isAdditionalCond == 1)
		{
			if(pAdditionalBS == NULL)
				pAdditionalBS = new ADDITIONAL_BUYSELL;
			BYTE * pb = (BYTE*)pAdditionalBS;
			for(i=0;i<sizeof(ADDITIONAL_BUYSELL);i++)
			{
				ar>>pb[i];
			}
		}

		BYTE nArray ;
		ar>>nArray;
		defaultValArray.RemoveAll();
		for(i=0;i<nArray;i++)
		{
			FLOAT11 f11;
			float*  pf11 = (float*)&f11;
			for(int j=0;j<11;j++)
			{
				ar>>pf11[j];
			}
			defaultValArray.Add(f11);
		}

		if(nArray != this->numPara)
		{
			AddDefaultValToArray();
		}


		for(int iIndex = 0;iIndex< numPara ;iIndex++)
		{
			if( defaultValYH [iIndex]> max [iIndex]||
				defaultValYH [iIndex]< min [iIndex])
				defaultValYH [iIndex] =  defaultVal [iIndex];
		}



		{
			ar>>name ;	

			ar>>password	 ;

			CString s2 = password;
			SecretForm(s2,true);
			password = s2;

			ar>>explainBrief ;
			ar>>explainParam ;	

			for(i=0;i<8;i++)
				ar>>namePara[i]	 ;
			ar>>fomular	 ;	
			s2 = fomular;
			SecretForm(s2,true);
			fomular = s2;

			ar>>help	 ;	

			ar>>buyStr   ;      
			ar>>sellStr  ;    
			ar>>buyStrKong   ;     
			ar>>sellStrKong  ;      

			ar>>subKindIndex ;		
			ar>>subKindIndexTime ;	
			ar>>strReserved ;	




		}
	}

}

void CFormularContent::Serialize(CArchive& ar)
{
	SerializeData(ar);
}
