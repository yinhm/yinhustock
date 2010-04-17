// CTaiShanTesting.cpp: implementation of the CTaiShanTesting class.
// by 俞明录
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "CTaiShanKlineShowView.h"
#include "CFormularContent.h"
#include "CTaiScreenParent.h"
#include "myricheditctrl.h"
#include "CTaiShanTesting.h"
#include "DialogProgressPingce.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifdef _UNICODE
	#define TOCH(strSource,strDest,nCount) WideCharToMultiByte(CP_ACP,WC_DEFAULTCHAR,(LPCWSTR)strDest,0,strSource,nCount,NULL);
#else
	#define TOCH(strSource,strDest,nCount) strcpy(strDest,(LPCSTR)strSource);
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaiShanTesting::CTaiShanTesting()
{
}

CTaiShanTesting::CTaiShanTesting(CTaiShanDoc* pDoc)
{
	m_strStockID="";
	m_bRefresh=TRUE;
	m_bHaveFormula=FALSE;
	m_pDoc=pDoc;
}

CTaiShanTesting::~CTaiShanTesting()
{
}

void CTaiShanTesting::GetChooseFormula(CTaiShanKlineShowView* pView,BOOL bRefresh)	
{
	if (m_bHaveFormula && !bRefresh)
		return;

	
	m_strHjyChooseFormula.RemoveAll();
	m_strChooseFormu.RemoveAll();

	IndexDataInfo strFormula;
	int nFormulaNum;
	bool bSuccess=false;
	CString strMessage="";

	Formu_Array1 *pList = NULL;
	CFormularContent *pFormula;
	char* strSub2="01 指标选股";
	char* strSubTeSe="02 特色选股公式";
	int i,j;



	if (m_pDoc==NULL)
		goto FUN_EXCEPTION;

	pList=&m_pDoc->m_formuar_choose;
	if (pList==NULL)
		goto FUN_EXCEPTION;

	nFormulaNum=pList->GetSize();
	if (nFormulaNum<1)
	{
		strMessage=_T("当前系统没有选股公式，不能选股!");
		goto FUN_EXCEPTION;
	}

	for (i=0;i<nFormulaNum;i++)
	{
		pFormula=pList->GetAt(i);
		if (strcmp(pFormula->subKindIndex.GetBuffer(0),strSub2)==0)
		{
			strFormula.iKind=-2;
			strcpy(strFormula.sIndex_name,pFormula->name);
			strFormula.iLine=0;
			if (pFormula->numPara>8 || pFormula->numPara<0)
				continue;

			if (m_pDoc->m_propertyInitiate.bSaveParam==TRUE)
			{
				for (j=0;j<pFormula->numPara;j++)
				{
					strFormula.fParam[j]=pFormula->defaultVal[j];
					if(pFormula->defaultValArray.GetSize()>j)
					{
						int nIndex = CTaiScreenParent::FromKlineKindToFoot(j);
						if(nIndex>=0)
							strFormula.fParam[j] = pFormula->defaultValArray[j].fValue[nIndex];
					}
				}
			}
			else
			{
				for (j=0;j<pFormula->numPara;j++)
					strFormula.fParam[j]=pFormula->defaultVal[j];
			}


			strFormula.fSubParam1=0;
			strFormula.fSubParam2=0;
			switch (pView->m_nKlineKind2)
			{
			case HS_KLINE:		
				strFormula.iDataType=0;
				break;
			case MANY_DAY_KLINE:	
				strFormula.iDataType=10;
				break;
			case MINUTE1_KLINE:		
				strFormula.iDataType=1;
				break;
			case MIN5_KLINE:	
				strFormula.iDataType=2;
				break;
			case MIN15_KLINE:	
				strFormula.iDataType=3;
				break;
			case MIN30_KLINE:	
				strFormula.iDataType=4;
				break;
			case MIN60_KLINE:	
				strFormula.iDataType=5;
				break;
			case DAY_KLINE:	
				strFormula.iDataType=6;
				break;
			case WEEK_KLINE:		
				strFormula.iDataType=7;
				break;
			case MONTH_KLINE:	
				strFormula.iDataType=8;
				break;
			case YEAR_KLINE:	
				strFormula.iDataType=9;
				break;
			default:
				goto FUN_EXCEPTION;
			}
			strFormula.iSunIndex=0;
			strFormula.bBuyCase=FALSE;
			strFormula.iBeginFoot=CTaiScreenParent::GetCountValid(&strFormula, m_pDoc, 10);
			m_strChooseFormu.Add(strFormula);
		}
		else if (strcmp(pFormula->subKindIndex.GetBuffer(0),strSubTeSe)==0)
		{
			strFormula.iKind=-2;
			strcpy(strFormula.sIndex_name,pFormula->name);
			strFormula.iLine=0;
			if (pFormula->numPara>8 || pFormula->numPara<0)
				continue;

			if (m_pDoc->m_propertyInitiate.bSaveParam==TRUE)
			{
				for (j=0;j<pFormula->numPara;j++)
				{
					strFormula.fParam[j]=pFormula->defaultVal[j];
					if(pFormula->defaultValArray.GetSize()>j)
					{
						int nIndex = CTaiScreenParent::FromKlineKindToFoot(j);
						if(nIndex>=0)
							strFormula.fParam[j] = pFormula->defaultValArray[j].fValue[nIndex];
					}
				}
			}
			else
			{
				for (j=0;j<pFormula->numPara;j++)
					strFormula.fParam[j]=pFormula->defaultVal[j];
			}


			strFormula.fSubParam1=0;
			strFormula.fSubParam2=0;
			switch (pView->m_nKlineKind2)
			{
			case HS_KLINE:	
				strFormula.iDataType=0;
				break;
			case MANY_DAY_KLINE:
				strFormula.iDataType=10;
				break;
			case MINUTE1_KLINE:		
				strFormula.iDataType=1;
				break;
			case MIN5_KLINE:		
				strFormula.iDataType=2;
				break;
			case MIN15_KLINE:	
				strFormula.iDataType=3;
				break;
			case MIN30_KLINE:	
				strFormula.iDataType=4;
				break;
			case MIN60_KLINE:
				strFormula.iDataType=5;
				break;
			case DAY_KLINE:		
				strFormula.iDataType=6;
				break;
			case WEEK_KLINE:	
				strFormula.iDataType=7;
				break;
			case MONTH_KLINE:	
				strFormula.iDataType=8;
				break;
			case YEAR_KLINE:		
				strFormula.iDataType=9;
				break;
			default:
				goto FUN_EXCEPTION;
			}
			strFormula.iSunIndex=0;
			strFormula.bBuyCase=FALSE;
			strFormula.iBeginFoot=CTaiScreenParent::GetCountValid(&strFormula, m_pDoc, 10);
			m_strHjyChooseFormula.Add(strFormula);
		}
	}
	m_bHaveFormula=TRUE;

FUN_EXCEPTION:
	return;
}

void CTaiShanTesting::SetStock(CString strStockID)	
{
	if (m_strStockID==strStockID)
		return;
	m_strStockID=strStockID;
	m_strResultData2="";
	m_strResultData="";
	m_bRefresh=TRUE;
}

void CTaiShanTesting::SelectStock(CTaiShanKlineShowView* pView,BOOL bRefresh)			
{
	
	if (!bRefresh && !m_bRefresh)
		return;


	CDialogProgressPingce dlg;		
	IndexDataInfo strFormula;
	bool bSuccess=false;
	int nSelected;
	int nFormulaNum;	
	int i;

	
	nFormulaNum=m_strHjyChooseFormula.GetSize();
	if (nFormulaNum>0)
		dlg.SetRange(0,nFormulaNum-1);	

	m_strResultData2="";
	m_strResultData="";
	dlg.Create(IDD_DIALOG_PROGRESS_PC,pView);
	dlg.ShowWindow(SW_SHOW);
	int j=0;
	for (i=0;i<nFormulaNum;i++)
	{
		dlg.SetPos(i);	
		strFormula=m_strHjyChooseFormula.GetAt(i);
		nSelected = (int)CTaiScreenParent::FormularComputeShares(m_strStockID,pView->m_stkKind ,pView->m_nCountKline,&strFormula,bSuccess,pView->m_pkline, &pView->m_hs,true);
		if (nSelected==1)
		{
			if (j++>0)
				m_strResultData2+="\n";
			m_strResultData2+=strFormula.sIndex_name;
			m_strResultData2+="\n  ";
			CFormularContent* pIndex=(CFormularContent*)CTaiScreenParent::LookUpArray(strFormula.iKind, strFormula.sIndex_name,m_pDoc); 
			m_strResultData2+=pIndex->help;
			m_strResultData2+="\n";
		}
	}

	if (m_strResultData2=="")
		m_strResultData2+="";

	nFormulaNum=m_strChooseFormu.GetSize();
	if (nFormulaNum>0)
	{
		dlg.SetRange(0,nFormulaNum-1,TRUE);	
		dlg.SetPos(0);
	}
	
	j=0;
	for (i=0;i<nFormulaNum;i++)
	{
		dlg.SetPos(0);
		strFormula=m_strChooseFormu.GetAt(i);
		nSelected = (int)CTaiScreenParent::FormularComputeShares(m_strStockID,pView->m_stkKind,pView->m_nCountKline,&strFormula,bSuccess,pView->m_pkline, &pView->m_hs,true);
		if (nSelected==1)
		{
			if (j++>0)
				m_strResultData+="\n";
			m_strResultData+=strFormula.sIndex_name;
			m_strResultData+="\n  ";
			CFormularContent* pIndex=(CFormularContent*)CTaiScreenParent::LookUpArray(strFormula.iKind, strFormula.sIndex_name,m_pDoc); 
			m_strResultData+=pIndex->help;
			m_strResultData+="\n";
		}
	}
	if (m_strResultData=="")
		m_strResultData+="";

	dlg.Complete();
	dlg.Close();

	m_bRefresh=FALSE;	
}

void CTaiShanTesting::OnDraw(CTaiTestRichEditCtrl* pRich, COLORREF crBack, COLORREF crText, COLORREF crTitle, CRect &rc)	//画出结果
{

	CString strText;
	CString strTemp;
	int nLen;
	int nWidth;
	int nCount;
	int nStart=0;
	int nTotal;
	int i;
	int nPos;
	char ch;
	int nHzch;

	CHARFORMAT cf;

	pRich->ShowWindow(SW_HIDE);
	pRich->SetSel(0,-1);
	pRich->SetWindowText (_T(""));
	pRich->SetBackgroundColor(FALSE,crBack);


	cf.dwMask = CFM_COLOR | CFM_SIZE;
	cf.cbSize=sizeof(CHARFORMAT);
	cf.crTextColor=crTitle;
	cf.dwEffects=~CFE_AUTOCOLOR;
	cf.yHeight=220;
	pRich->SetSelectionCharFormat(cf);	

	strText=_T("特色选股评测\r\n");	
	pRich->ReplaceSel(strText);

	cf.dwMask = CFM_COLOR | CFM_SIZE;
	cf.cbSize=sizeof(CHARFORMAT);
	cf.crTextColor=crText;
	cf.dwEffects=~CFE_AUTOCOLOR;
	cf.yHeight=180;
	pRich->SetSelectionCharFormat(cf);	


	nLen=6;
	nWidth=rc.Width();
	nCount=nWidth / nLen-3;

	
	strText.Format(_T("%s\r\n"),m_strResultData2);
	nStart=0;
	nTotal=strText.GetLength();
	do
	{
		strTemp=strText.Mid(nStart,nCount);
		if (strTemp.GetLength()<nCount)
		{
			nStart=nTotal-1;
		}
		else
		{
			nPos=strTemp.FindOneOf(_T("\r\n"));
			if (nPos==-1)	
			{
				if (strTemp[nCount-1]&0x80==0)
				{
					nStart+=nCount;
				}
				else
				{
					nHzch=0;
					for (i=0;i<nCount;i++)
					{
						ch=strTemp[i];
						if ((ch&0x80)==0)
						{
							nHzch=0;
						}
						else
						{
							nHzch++;
						}
					}
					if ((nHzch % 2)==1)
					{
						strTemp=strTemp.Left(nCount-1);
						nStart += (nCount-1);
					}
					else
					{
						nStart += nCount;
					}
				}
				strTemp+=_T("\r\n");
			}
			else
			{
				if (nPos==nCount-1)
				{
				nStart += nCount;
				}
				else
				{
					if (strTemp[nPos]==_T('\r'))
					{
						if (strTemp[nPos+1]!=_T('\n'))
						{
							
							strTemp = strTemp.Left(nPos+1);
							nStart += (nPos+1);
						}
						else
						{
							strTemp =strTemp.Left(nPos+2);
							nStart += (nPos+2);
						}
					}
					else
					{
						if (strTemp[nPos+1]!=_T('\r'))
						{
						
							strTemp = strTemp.Left(nPos+1);
							nStart += (nPos+1);
						}
						else
						{
							strTemp =strTemp.Left(nPos+2);
							nStart += (nPos+2);
						}
					}
				}
			}
		}
		pRich->SetSelectionCharFormat(cf);	
		pRich->ReplaceSel(strTemp);
	}while(nStart<nTotal-1);

	cf.dwMask = CFM_COLOR | CFM_SIZE;
	cf.cbSize=sizeof(CHARFORMAT);
	cf.crTextColor=crTitle;
	cf.dwEffects=~CFE_AUTOCOLOR;
	cf.yHeight=220;
	pRich->SetSelectionCharFormat(cf);	
	strText=_T("选股综合评测\r\n");	
	pRich->ReplaceSel(strText);

	cf.dwMask = CFM_COLOR | CFM_SIZE;
	cf.cbSize=sizeof(CHARFORMAT);
	cf.crTextColor=crText;
	cf.dwEffects=~CFE_AUTOCOLOR;
	cf.yHeight=180;
	pRich->SetSelectionCharFormat(cf);	
	strText.Format(_T("%s"),m_strResultData);
	nStart=0;
	nTotal=strText.GetLength();
	do
	{
		strTemp=strText.Mid(nStart,nCount);
		if (strTemp.GetLength()<nCount)
		{
			nStart=nTotal-1;
		}
		else
		{
			nPos=strTemp.FindOneOf(_T("\r\n"));
			if (nPos==-1)		
			{
				if (strTemp[nCount-1]&0x80==0)
				{
					nStart+=nCount;
				}
				else
				{
					nHzch=0;
					for (i=0;i<nCount;i++)
					{
						ch=strTemp[i];
						if ((ch&0x80)==0)
						{
							nHzch=0;
						}
						else
						{
							nHzch++;
						}
					}
					if ((nHzch % 2)==1)
					{
						strTemp=strTemp.Left(nCount-1);
						nStart += (nCount-1);
					}
					else
					{
						nStart += nCount;
					}
				}
				strTemp+=_T("\r\n");
			}
			else
			{
				if (nPos==nCount-1)
				{
				nStart += nCount;
				}
				else
				{
					if (strTemp[nPos]==_T('\r'))
					{
						if (strTemp[nPos+1]!=_T('\n'))
						{
							
							strTemp = strTemp.Left(nPos+1);
							nStart += (nPos+1);
						}
						else
						{
							strTemp =strTemp.Left(nPos+2);
							nStart += (nPos+2);
						}
					}
					else
					{
						if (strTemp[nPos+1]!=_T('\r'))
						{
							
							strTemp = strTemp.Left(nPos+1);
							nStart += (nPos+1);
						}
						else
						{
							strTemp =strTemp.Left(nPos+2);
							nStart += (nPos+2);
						}
					}
				}
			}
		}
		pRich->SetSelectionCharFormat(cf);	
		pRich->ReplaceSel(strTemp);
	}while(nStart<nTotal-1);

	
	pRich->HideSelection(TRUE,TRUE);
	pRich->MoveWindow(&rc);
	pRich->ShowWindow(SW_SHOW);
}
