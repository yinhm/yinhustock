// CAlertSystem.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CAlertSystem.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "SelectStock.h"
#include "BGridCtrl.h"
#include "keybrodeangel.h"
#include "SetParam.h"
#include "CSharesBigBillDlg.h"
#include "CSharesInformation.h"
#include "CAlertSet.h"
#include "CTaiKlineFileHS.h"
#include "CAlertPopDlg.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CListAlarmSet CAlertSystem::m_gAlarmArray;
CListAlarmLineSet CAlertSystem::m_gLineAlarmArray;
CList<ALERT_RESULT_ITEM*,ALERT_RESULT_ITEM*&> CAlertSystem::m_gAlarmResultList;

CAlertSystem* CAlertSystem::m_gDlgAlarmSystem = NULL;

#define TJXG_ALERT  400    


CAlertSystem::CAlertSystem(CWnd* pParent /*=NULL*/)
	: CDialog(CAlertSystem::IDD, pParent)
{
	
	pDoc = CMainFrame::m_taiShanDoc ;
}


void CAlertSystem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlertSystem)
	DDX_Control(pDX, IDC_BUTTON7, m_button7);
	DDX_Control(pDX, IDC_BUTTON6, m_button6);
	DDX_Control(pDX, IDC_BUTTON4, m_button4);
	DDX_Control(pDX, IDC_BUTTON3, m_button3);
	DDX_Control(pDX, IDC_LIST2, m_alarmResultList);
	DDX_Control(pDX, IDC_LIST1, m_condArrayList);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_BUTTON1, m_btAdd);
	DDX_Control(pDX, IDC_BUTTON2, m_btDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlertSystem, CDialog)
	//{{AFX_MSG_MAP(CAlertSystem)
	ON_BN_CLICKED(IDC_BUTTON1, OnAdd)
	ON_BN_CLICKED(IDC_BUTTON2, OnDelete)
	ON_BN_CLICKED(IDC_BUTTON3, OnModify)
	ON_BN_CLICKED(IDC_BUTTON4, OnClear)
	ON_BN_CLICKED(IDC_BUTTON6, OnClearAll)
	ON_BN_CLICKED(IDC_BUTTON7, OnWriteToFile)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAlertSystem::OnAdd() 
{

	CAlertSet dlg;
	for(int i=0;i<1000;i++)
	{
		int n = m_gAlarmArray.GetCount ();
		CString s = "";
		s.Format ("预警%d",i);
		bool bExit = false;
		for(int j=0;j<n;j++)
		{
			POSITION pos=m_gAlarmArray.FindIndex (j);
			ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);
			CString s2 = item->m_strAlarmName ;
			if(s==s2)
			{

				bExit = true;
				break;
			}
		}
		if(bExit == false) 
		{
			dlg.m_sTitle = s;
			break;
		}
	}
	if(dlg.DoModal ()==IDOK)
	{
		ALERT_SET_ITEM* item = new ALERT_SET_ITEM;
		dlg.GetItemData (*item);
		m_gAlarmArray.AddTail (item);

		AlarmArrInsertRow(item);
	}

	
}

void CAlertSystem::OnDelete() 
{

    CString Id;
	int temp=m_condArrayList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请首先选中条件！");
		return;
	}
	if(IDYES == AfxMessageBox("真的删除？",MB_YESNO|MB_ICONQUESTION ))
	{
		while(temp>=0)
		{
			m_condArrayList.DeleteItem(temp);
			POSITION pos=m_gAlarmArray.FindIndex (temp);
			POSITION pos2 = pos;
			delete m_gAlarmArray.GetAt(pos);
			m_gAlarmArray.RemoveAt (pos2);

			temp=m_condArrayList.GetNextItem( -1,LVNI_SELECTED   ) ;
		}
		ClearAllResult();
	}

}

void CAlertSystem::OnModify()
{

	CAlertSet dlg;
	int temp=m_condArrayList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请首先选中条件！");
		return;
	}
	if(temp>=m_gAlarmArray.GetCount ())
		return;

	POSITION pos=m_gAlarmArray.FindIndex (temp);
	ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);


	dlg.SetInitData (*item);
	dlg.m_nItem = temp;
	if(dlg.DoModal ()==IDOK)
	{
		dlg.GetItemData (*item);
	}

	AlarmArrInsertRow(item,temp);


	
}

void CAlertSystem::OnClear() 
{

    CString Id;
	int temp=m_alarmResultList.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("请首先选中预警结果！");
		return;
	}
	if(IDYES == AfxMessageBox("删除预警结果？",MB_YESNO|MB_ICONQUESTION ))
		while(temp>=0)
		{
			m_alarmResultList.DeleteItem(temp);
			POSITION pos=m_gAlarmResultList.FindIndex (temp);
			ALERT_RESULT_ITEM* item = m_gAlarmResultList.GetAt(pos);
			delete item;
			m_gAlarmResultList.RemoveAt (pos);
			temp=m_alarmResultList.GetNextItem( -1,LVNI_SELECTED   ) ;
		}
}

void CAlertSystem::OnClearAll() 
{

	if(IDYES == AfxMessageBox("删除全部预警结果？",MB_YESNO|MB_ICONQUESTION ))
	{
		ClearAllResult();
	}
}

void CAlertSystem::OnWriteToFile() 
{

	UpdateData();


	CLongString longStr;

	int nPerLine=0,nOutLine=0;
	nPerLine =  this->m_alarmResultList .GetItemCount();
	nOutLine = 5; 

	int i,j;
	for(j=0;j<nPerLine;j++)
	{
		for(i =0;i<nOutLine;i++)
		{
			CString s ;
			m_alarmResultList.GetItemText (j,i,s.GetBuffer (24),20);//
			s.ReleaseBuffer ();
			if(i==nOutLine-1)
				s+="\r\n";
			else
				s+="\t";
			longStr + s;
		}
	}

    CString     defaultname="AlarmResultData.xls";

	CString sArray="*.xls";        

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,sArray);
	m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
	                                   
	CString filename="";
	if(m_filedia.DoModal()==IDOK)
	{
		filename=m_filedia.GetPathName();

		CFile   file;
		if(file.Open(filename,CFile::modeRead)!=0)
		{
			if(AfxMessageBox(filename+"文件已存在，要覆盖吗？",MB_YESNO|MB_ICONQUESTION)==IDNO)
			{
				file.Close();
				return;
			}
			file.Close();
		}

		if(file.Open(filename,CFile::modeWrite|CFile::modeCreate))
		{
			int n = longStr.GetLength ()+1;
			file.Write(longStr.m_pData ,n);
			file.Close();
		}
	}
}

bool CAlertSystem::LoadFile(bool bLine)
{
	CFile fl;
	if(fl.Open("data\\AlarmResult.alm",CFile::shareDenyNone |CFile::modeRead))
	{

		POSITION pos=m_gAlarmArray.GetHeadPosition ();
		while(pos)
		{
			ALERT_SET_ITEM* item = m_gAlarmArray.GetNext (pos);
			if(item!=NULL) delete item;
		}
		m_gAlarmArray.RemoveAll ();



		char ch[32]="W2YJ";
		fl.Read(ch,4);
		CString sHead = ch;
		if(sHead != "W2YJ") return false;

		int n;
		fl.Read(&n,4);

		for(int i=0;i<n;i++)
		{

			ALERT_SET_ITEM* item = new ALERT_SET_ITEM;
			fl.Read(item->m_strAlarmName ,16);

			BYTE bt ;
			fl.Read(&bt,1);
			fl.Read(&bt,1);
			item->m_bAnd = (bool)bt;

			int n2 ;
			fl.Read(&n2,4);

			for(int k=0;k<n2;k++)
			{
				IndexDataInfo ind;
				fl.Read(&ind,sizeof(IndexDataInfo));
				item->m_conditionArray.Add (ind);
			}

			fl.Read(item->m_btAddtional ,4);

			fl.Read(&n2,4);
			for(int k=0;k<n2;k++)
			{
				SharesNameArr3 sym;
				fl.Read(&sym,sizeof(SharesNameArr3));//
				item->m_symbolTypeArray.Add (sym);
			}
			fl.Read(&n2,4);
			for(int k=0;k<n2;k++)
			{
				SYMBOL_ISALERTED sym;
				fl.Read(&sym,sizeof(SYMBOL_ISALERTED));
				item->m_bAlarmedArray.Add (sym);
			}

			
			m_gAlarmArray.AddTail (item);
		}

		
		fl.Read(&n,4);

		{


			pos=m_gLineAlarmArray.GetHeadPosition ();
			while(pos)
			{
				ALERT_LINE_ITEM* item = m_gLineAlarmArray.GetNext (pos);
				if(item!=NULL) delete item;
			}
			m_gLineAlarmArray.RemoveAll ();

			for(int i=0;i<n;i++)
			{
				ALERT_LINE_ITEM* item = new ALERT_LINE_ITEM;

				SYMBOL_ISALERTED sym;
				fl.Read(&sym,sizeof(SYMBOL_ISALERTED));
				item->m_symbolAlarm = (sym);
				;

				DATA_PER_LINE line;
				int k=0;
				fl.Read(&k,4);
				for(int j=0;j<k;j++)
				{
					fl.Read(&line,sizeof(DATA_PER_LINE));
					item->m_lineInfoArr .Add (line);
	
				}

		
				if(item->m_lineInfoArr .GetSize ()<=0)
				{
					delete item;continue;
				}
				else
				{
					for(int j=0;j<item->m_lineInfoArr .GetSize ();j++)
					{
					
						if(item->m_lineInfoArr[j].m_btAlarmed == 1)
						{
							item->m_lineInfoArr.RemoveAt(j);
							j--;
						}
					}
				}
				m_gLineAlarmArray.AddTail (item);
			}
		}


		n = 0;
		int n2 = fl.Read(&n,sizeof(int));
		if(n2 == 4)
		{

			for(int i=0;i<n;i++)
			{
				ALERT_RESULT_ITEM* itm = new ALERT_RESULT_ITEM;
				if(fl.Read(itm,sizeof(ALERT_RESULT_ITEM)) == sizeof(ALERT_RESULT_ITEM))
				{
					m_gAlarmResultList.AddTail(itm);
				}
				else
					break;
			}
		}
	}
	return true;
}

bool CAlertSystem::SaveFile()
{
	
	CFile fl;
	if(fl.Open("data\\AlarmResult.alm",CFile::modeCreate|CFile::shareDenyNone |CFile::modeWrite))
	{
		char ch[32]="W2YJ";
		fl.Write(ch,4);
		int n = m_gAlarmArray.GetCount ();
		fl.Write(&n,4);

		for(int i=0;i<n;i++)
		{
			POSITION pos=m_gAlarmArray.FindIndex (i);
			ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);
			fl.Write(item->m_strAlarmName ,16);

			BYTE bt = (BYTE)item->m_bAnd ;
			fl.Write(&bt,1);
			fl.Write(&bt,1);

			int n2 = item->m_conditionArray .GetSize ();
			fl.Write(&n2,4);
			fl.Write(item->m_conditionArray .GetData (),sizeof(IndexDataInfo)*n2);

			fl.Write(item->m_btAddtional ,4);

			n2= item->m_symbolTypeArray.GetSize();
			fl.Write(&n2,4);
			fl.Write(item->m_symbolTypeArray .GetData () ,sizeof(SharesNameArr3)* n2);
			n2= item->m_bAlarmedArray .GetSize();
			fl.Write(&n2,4);
			fl.Write(item->m_bAlarmedArray .GetData () ,sizeof(SYMBOL_ISALERTED)* n2);
		}


		n = m_gLineAlarmArray.GetCount ();
		fl.Write(&n,4);

		for(int i=0;i<n;i++)
		{
			POSITION pos=m_gLineAlarmArray.FindIndex (i);
			ALERT_LINE_ITEM* item = m_gLineAlarmArray.GetAt(pos);

			fl.Write(&(item->m_symbolAlarm ),sizeof(SYMBOL_ISALERTED));

			int k=item->m_lineInfoArr.GetSize ();
			fl.Write(&k,4);
			for(int j=0;j<k;j++)
			{
				fl.Write(&(item->m_lineInfoArr [j]),sizeof(DATA_PER_LINE));

			}
		}
	
		n = m_gAlarmResultList.GetCount();
		fl.Write(&n,sizeof(int));
		{
			for(int i=0;i<n;i++)
			{
				POSITION pos=m_gAlarmResultList.FindIndex (i);
				ALERT_RESULT_ITEM* itm = m_gAlarmResultList.GetAt(pos);
				fl.Write(itm,sizeof(ALERT_RESULT_ITEM));
			}
		}

	}
	return true;
}

void CAlertSystem::CaclAlarm(CMainFrame *pFrame)
{
	if(CMainFrame::m_taiShanDoc == 0)
		return;
	if(CMainFrame::m_taiShanDoc ->m_bInitDone ==FALSE)
		return;
	if(pFrame->m_bAlarmExec == false)
		return;

	static int g_nTime = 0;
	int nLineAlarm = m_gLineAlarmArray.GetCount ();
	int nAlarm = m_gAlarmArray.GetCount ();
	if(g_nTime>=nLineAlarm+nAlarm)
		g_nTime = 0;

	if(g_nTime>=nAlarm)
	{
	
		CaclLineAlarm(pFrame,g_nTime-nAlarm);
		g_nTime++;
	    return;
	}
	POSITION pos=m_gAlarmArray.FindIndex (g_nTime);
	ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

	CArray<IndexDataInfo,IndexDataInfo&>& indexSaveArray = item->m_conditionArray ;
	bool bAnd = item->m_bAnd ;
	
	if(indexSaveArray.GetSize() == 0)
	{
		g_nTime++;
	    return;
	}
  
    int stocknum = item->m_bAlarmedArray .GetSize();
	int index_num = indexSaveArray.GetSize();
    Kline *pKline=NULL;
	CBuySellList buySellList ;

	int * pnNeedCount = new int [index_num];
	for(int i=0;i<index_num;i++)
	{
		IndexDataInfo index_save = indexSaveArray.GetAt(i);
		int nDays=index_save.iBeginFoot ;
		pnNeedCount[i]=nDays;
	}

	static int g_nStockCurrent = 0;
	DWORD dwTick = GetTickCount();
    for(int i=g_nStockCurrent;i<stocknum;i++)
    {
		CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
		if(pDoc == NULL)
				break;
		if(pFrame->m_bAlarmExec == false)
			break;


		{
			DWORD dwTick2 = GetTickCount();
			if(dwTick2-dwTick>100)
			{
				g_nStockCurrent = i;
				goto END_2;
			}
		}
		bool bSucc;
		int nRead;
		CString StockId;
		StockId = item->m_bAlarmedArray.GetAt(i).m_sSymbol ;
		int stkKind = item->m_bAlarmedArray.GetAt(i).m_stkKind ;

		int result = 0 ;

		if(item->m_bAlarmedArray.GetAt(i).m_btAlarmed ==1 &&  item->m_btAddtional [2] == 0)
				continue;

	    for(int k=0;k<index_num;k++)
		{
			IndexDataInfo index_save = indexSaveArray.GetAt(k);
			int nDays=pnNeedCount[k];
			if(nDays<-1)
				continue;
			float a = 0;

			if(index_save.iKind == CTaiKlineTreeCtl::nKindPriceAlert 
				|| index_save.iKind == CTaiKlineTreeCtl::nKindUpDownAlert )
			{
				CReportData* dt1;
				if(pDoc->m_sharesInformation.Lookup(StockId.GetBuffer (0),dt1,stkKind))
				{
					CFormularContent* pJs = CTaiScreenParent::GetSpecialLJishu(-index_save.iKind-7);
					switch(index_save.iKind)
					{
					case CTaiKlineTreeCtl::nKindPriceAlert :
						if(pJs!=NULL)
						{
							if(dt1->nowp <= index_save.fParam [0] || dt1->nowp >= index_save.fParam [1] )
								a=1;
						}
						break;
					case CTaiKlineTreeCtl::nKindUpDownAlert :
						if(pJs!=NULL && dt1->ystc > 0 )
						{
							float fUp =( dt1->nowp - dt1->ystc )/dt1->ystc ;
							if(fUp <= index_save.fParam [0] || fUp >= index_save.fParam [1] )
								a=1;
						}
						break;
					}
				}
			}
			else
			{
		
			nRead = CTaiScreenParent::ReadKLine(StockId,stkKind, pKline, nDays,CTaiScreenParent::nLKind[index_save.iDataType],&buySellList, 48 ,TRUE);
			
			if(nRead < nDays || nDays<-1)
			{
				if(bAnd == 1)
				  continue;
				else
					break;
			}
			a = CTaiScreenParent::FormularComputeShares(StockId,stkKind , nRead,&index_save,bSucc,pKline, &buySellList,true);
			}
			  if(a == 1)
				  result++;
			  if(bAnd==1 &&result>0 ||bAnd==0 &&result<k+1)
				  break;
		}
		if(bAnd==0 &&result==index_num || bAnd ==1 &&result>0)
		{
			;
			CReportData* dt1;

		
			{
				if(pDoc->m_sharesInformation.Lookup(StockId.GetBuffer (0),dt1,stkKind))
				{
					AlarmSendMessage(pFrame, dt1,item,g_nTime);

					SYMBOL_ISALERTED symIs; 
					strcpy(symIs.m_sSymbol ,StockId);
					symIs.m_btAlarmed = 1;
					symIs.m_stkKind = stkKind;
					item->m_bAlarmedArray.SetAt(i,symIs);
				}
			}

		}

    }
END_1:	g_nTime++;
		g_nStockCurrent = 0;
END_2:	CTaiKlineFileHS::RemoveHs(buySellList);
	delete [] pnNeedCount;

	if(pKline)
	{
		delete[] pKline;      
		pKline=NULL;
	}


}

BOOL CAlertSystem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_btDelete.SetIcon(IDI_ICON_DELETE);
	m_btAdd.SetIcon(IDI_ICON_ADD2);
	m_btOK.SetIcon(IDI_ICON_OK);
	m_btCancel.SetIcon(IDI_EXIT16);

	SetTimer(1,10000,NULL);
	


	FillListctrlHeader();



	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	CString sArr[10];
	for(int i=0;i<m_gAlarmArray.GetCount ();i++)
	{
		POSITION pos=m_gAlarmArray.FindIndex (i);
		ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

	
		AlarmArrInsertRow(item);
	}
	 

	int nCount = m_gAlarmResultList.GetCount ();
	m_alarmResultList.SetItemCount (nCount);
	for(int i=0;i<nCount;i++)
	{
		ResultListAddRow(&this->m_alarmResultList ,i);
	}


	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame->m_bAlarmExec == true)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->SetWindowText("关闭预警");
	}
	
	return TRUE; 
}

void CAlertSystem::GetSymbolArray(CArrayStockType &typeArr, SymbolKindArr &symbolArr)
{
	CTaiShanDoc *m_pDoc = CMainFrame::m_taiShanDoc ;

	symbolArr.RemoveAll();
	int n = typeArr.GetSize ();

	//
	CTypedPtrMap<CMapStringToPtr,CString,int*> mapStringToInt;
	int* nMap;
	CString s;
	CString s2;
	for(int i=0;i<n;i++)
	{
		s = typeArr[i].m_sSymbol ;
		int k = typeArr[i].m_nType ;
		if(k<0)
		{
			s2 = m_pDoc->GetStockKindString(-k-1);
			s = s2+s;
			if(!mapStringToInt.Lookup (s,nMap))
				mapStringToInt[s] = (int*)0;
		}
		if(k>=0 && k< CHOOSESTK)
		{
			   int temp=m_pDoc->m_sharesInformation.GetStockTypeCount(k);
			   for(int j=0;j<temp;j++)
			   {
			 	    CReportData *Cdat;
				    m_pDoc->m_sharesInformation.GetStockItem(k,j,Cdat);
				    if(Cdat==NULL)
					   continue;
					s=Cdat->id ;
					s2 = m_pDoc->GetStockKindString(Cdat->kind );
					s= s2+s;
				    if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
			   }
		}
		if(k==CHOOSESTK)
		{
			  SymbolKindArr l_StockArray;
			  m_pDoc->m_ManagerStockTypeData.GetChooseStockCode(l_StockArray);
			  int nStk = l_StockArray.GetSize();
			  for(int j=0;j<nStk;j++)
			  {
				  s=l_StockArray[j].m_chSymbol ;
					s2 = m_pDoc->GetStockKindString(l_StockArray[j].m_nSymbolKind );
					s= s2+s;
				  if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
			  }
		}
		if(k>=1000 && k<2000)
		{
				SymbolKindArr l_StockCodeArray;
				CStringArray l_StockTypeNameArray;
		        m_pDoc->m_ManagerStockTypeData.GetAllStockTypeName(l_StockTypeNameArray);
				CString l_sStockTypeName=l_StockTypeNameArray.GetAt(k-1000);
				m_pDoc->m_ManagerStockTypeData.GetStockFromStockTypeName(l_StockCodeArray,l_sStockTypeName.GetBuffer(0));          
				int DisplayCount=l_StockCodeArray.GetSize();
				for(int row=0 ;row <  (int)DisplayCount  ;row++)        
				{    
					 s = l_StockCodeArray[row].m_chSymbol ; 
					s2 = m_pDoc->GetStockKindString(l_StockCodeArray[row].m_nSymbolKind );
					s= s2+s;
					 if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
				}
		}
		if(k>=2000&&k<3000)
		{
			SymbolKindArr sArr;
			GetScreenStockBlockSymbol(sArr, s,k);
			int n=sArr.GetSize ();
			for(int j=0;j<n;j++)
			{
				s = sArr[j].m_chSymbol ; 
				s2 = m_pDoc->GetStockKindString(sArr[j].m_nSymbolKind );
				s= s2+s;
				if(!mapStringToInt.Lookup (s,nMap))
					mapStringToInt[s] = (int*)0;
			}
		}

		if(k==3000)
		{
			POSITION pos=m_pDoc->m_StockTypeMap[2].GetStartPosition ();  
			while(pos)
			{
				CString zqdm;
				CString ss;
				m_pDoc->m_StockTypeMap[2] .GetNextAssoc(pos,zqdm,ss);
				s=zqdm;
				if(!mapStringToInt.Lookup (s,nMap))
						mapStringToInt[s] = (int*)0;
			}
		}

	}

	
	POSITION position=mapStringToInt.GetStartPosition ();
	while(position)
	{
		mapStringToInt.GetNextAssoc (position,s,nMap);
		SymbolKind sm;
		CString s3 = s.Left (2);
		sm.m_nSymbolKind = atoi(s3);
		s.Delete (0,2);
		strcpy(sm.m_chSymbol ,s);
		symbolArr.Add (sm);
	}

}

void CAlertSystem::FillListctrlHeader()
{
	m_condArrayList.SetExtendedStyle(m_condArrayList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

   	CRect           rect;
	CString			strItem1[]= {_T("预警条件"),_T("监控股票"),_T("预警数")};
	int nCol = 3;
	int gap = 2;
	this->m_condArrayList .SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	for(int i=0;i<nCol;i++)
	{
		this->m_condArrayList .InsertColumn(i, strItem1[i], LVCFMT_LEFT,
			125, i);
	}


	m_alarmResultList.SetExtendedStyle(m_alarmResultList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	CString strItem[]= {_T("股票代码"),_T("预警条件"),_T("预警时间"),__T("预警价格"),_T("现价/盈亏")};
//	CString strItem[]= {_T("股票名称"),_T("预警价格"),_T("预警名称"),_T("当前价格"),_T("预警时间")};
	nCol = 5;
	this->m_alarmResultList  .SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int nWid[] = {75,75,120,75,120};
	for(int i=0;i<nCol;i++)
	{
		this->m_alarmResultList .InsertColumn(i, strItem[i], LVCFMT_LEFT,
			nWid[i], i);
	}

}

void CAlertSystem::InsertRow(CListCtrl *pList, CString *strPt, int nStr, int nImag,int nPos,int stkKind )
{
	LV_ITEM  lvitem;
	
	int n = pList->GetItemCount ();

	lvitem.mask = LVIF_TEXT|LVIF_IMAGE ;
	if(nPos == -1)
		lvitem.iItem =n;
	else
	{
		if(nPos >=n)
			return;
		lvitem.iItem =nPos;
	}
	lvitem.iSubItem = 0;
	lvitem.iImage = nImag;
	lvitem.pszText=strPt[0].GetBuffer (0);
	strPt[0].ReleaseBuffer ();
	int k ;
	if(nPos == -1)
		k= pList->InsertItem(&lvitem);
	else
		k= pList->SetItem(&lvitem);
	if(k==-1)
		return;

	for(int i=1;i<nStr;i++)
	{
	lvitem.iSubItem = i;

	lvitem.pszText=strPt[i].GetBuffer (0);;
	strPt[i].ReleaseBuffer ();
	pList->SetItem(&lvitem);
	}

	pList->SetItemData (lvitem.iItem,stkKind);

}

void CAlertSystem::ListCtrlToSymbolType(CArrayStockType &typeArr, CListCtrl *pList)
{
	typeArr.RemoveAll ();


	int  iIcon=0, iItem=0, iSubItem=0;
	
    int n = pList->GetItemCount();
	for(int item = 0;item<n;item++)
	{
		SharesNameArr3 stockType;
		
		pList->GetItemText (item,0,stockType.m_sSymbol ,18);
		stockType.m_nType = pList->GetItemData (item);
		typeArr.Add (stockType);
	} 
}

void CAlertSystem::OnCancel() 
{

	DestroyWindow();

}

void CAlertSystem::OnOK() 
{
	SwitchAlarm();
	
}

void CAlertSystem::StartAlarm(bool bFirst)
{
#ifdef OEM
	return;
#endif

	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();

	CTaiShanDoc *pDoc = CMainFrame::m_taiShanDoc ;


	if(bFirst == true && pDoc->m_propertyInitiate.bRunAlarmBegin != TRUE)
		return ;

	pFm->KillTimer (TJXG_ALERT);
	int nTime = pDoc->m_propertyInitiate.nSecondsAlarm*1000;

	int nItem = m_gAlarmArray.GetCount ();
	if(nItem == 0 )
		return;

	nTime /=nItem;
	if(nTime<500)
		nTime = 500;

	pFm->SetTimer(TJXG_ALERT,nTime,NULL);
	pFm->m_bAlarmExec = true;
	
}

void CAlertSystem::PostNcDestroy() 
{
	
	delete this;
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	pFm->m_pDlgAlarm =NULL;
	CDialog::PostNcDestroy();
}

void CAlertSystem::OnClose() 
{

	DestroyWindow();
	

}

void CAlertSystem::CreateNew()
{
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	if(pFm->m_pDlgAlarm == NULL)
	{
	CAlertSystem* dlg = new CAlertSystem;
	pFm->m_pDlgAlarm = dlg;
	dlg->Create ( IDD_MYALERT_SYSTEM ,pFm);
	}
	pFm->m_pDlgAlarm ->ShowWindow (SW_RESTORE);

}

void CAlertSystem::ResultListAddRow(CListCtrl* pList,int nRow)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	POSITION pos=m_gAlarmResultList.FindIndex (nRow);
	ALERT_RESULT_ITEM* item = m_gAlarmResultList.GetAt(pos);

	CString sArr[10];
	sArr[0]=item->m_symbol ;
	sArr[3].Format ("%.2f",item->m_fPrice);

	if(item->m_nFootAlarmArr >=0)

	{
		pos=m_gAlarmArray.FindIndex (item->m_nFootAlarmArr );
		ALERT_SET_ITEM* itemCond = m_gAlarmArray.GetAt(pos);
		sArr[1]=itemCond->m_strAlarmName  ;
	}
	else
		sArr[1]="趋势线预警" ;


	CReportData* dt1;
	float fPrc = 0;
	if(pDoc->m_sharesInformation.Lookup(sArr[0].GetBuffer (0),dt1,item->m_stkKind ))
		fPrc= dt1->nowp ;

	if(item->m_fPrice!=0)
		sArr[4].Format ("%.2f/%.2f%%",fPrc,(fPrc-item->m_fPrice)*100/item->m_fPrice);
	else
		sArr[4].Format ("%.2f/0.00%%",fPrc);

	

	sArr[2] = CTime(item->m_time ).Format ("%m/%d %H:%M:%S");

	InsertRow(pList  , sArr, 5, 13,-1,dt1->kind );

}

void CAlertSystem::CaclLineAlarm(CMainFrame *pFrame, int nLineAlarm)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;
	POSITION pos=m_gLineAlarmArray.FindIndex (nLineAlarm);
	ALERT_LINE_ITEM* item = m_gLineAlarmArray.GetAt(pos);

	static int g_nToday = 0;
	CTime tmNow = CTime::GetCurrentTime ();
	int nToday = atoi(tmNow.Format ("%Y%m%d"));

    int stocknum = 1;
	int index_num = item->m_lineInfoArr .GetSize ();
    Kline *pKline=NULL;
	CBuySellList buySellList ;
	
	if(pDoc == NULL)
		return;
	int k;
	CString StockId;
	StockId = item->m_symbolAlarm .m_sSymbol ;
	int stkKind =  item->m_symbolAlarm .m_stkKind ;
	int result = 0 ;


	if(nToday != g_nToday)
	{
		GetLineAlarmInitData(item);
		g_nToday = nToday;
	}


	for(k=0;k<index_num;k++)
	{
		if(item->m_lineInfoArr [k] .m_btAlarmed  == 1)
			continue;
		if(item->m_lineInfoArr [k].m_gapFromBeginArr == 0)
			continue;
		int nNow;
		if(item->m_lineInfoArr [k].m_gapFromBeginArr == 0)
			nNow= -item->m_lineInfoArr [k].m_gapFromBeginArr ;
		else
		{
			int nAdd = 0;
			if(item->m_lineInfoArr [k].lineInfo.klineType >0 && item->m_lineInfoArr [k].lineInfo.klineType < 5)
			{
				int isSz=0;
				if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;
				int nFoot;
				if(pDoc->m_bInitDone ==FALSE)
					nFoot=0;
				else
					nFoot=pDoc->m_nANT [isSz];

				int nRate = 5;
				switch (item->m_lineInfoArr [k].lineInfo.klineType )
				{
				case 1:
					nRate = 5;
					break;
				case 2:
					nRate = 15;
					break;
				case 3:
					nRate = 30;
					break;
				case 4:
					nRate = 60;
					break;
				}
				nAdd = (nFoot+1)/nRate + (nFoot+1)%nRate == 0? 0:1;
			}
			nNow += nAdd;
		}

	
		if(item->m_lineInfoArr [k].lineInfo.time2 <=0)
			continue;

		CReportData * pdt;
		if(pDoc->m_sharesInformation.Lookup(StockId.GetBuffer (0),pdt,stkKind))
		{
			float fPriceNow = pdt->nowp ;
			if(fPriceNow>0)
			{
				float fNow = (item->m_lineInfoArr [k].lineInfo.valFlt2 - item->m_lineInfoArr [k].lineInfo.valFlt1 )/item->m_lineInfoArr [k].lineInfo.time2 *nNow+item->m_lineInfoArr [k].lineInfo.valFlt1 ;
				bool bOk = false;
				if(item->m_lineInfoArr [k] .m_bUpwards == true && fPriceNow>=fNow)
					bOk = true;
				if(item->m_lineInfoArr [k] .m_bUpwards == false && fPriceNow<=fNow)
					bOk = true;
				if(bOk == true)
				{
					AlarmSendMessage(pFrame, pdt,NULL,-1);
					item->m_lineInfoArr [k] .m_btAlarmed = 1;
				}
			}
		}
		StockId.ReleaseBuffer ();


	}
  
	CTaiKlineFileHS::RemoveHs(buySellList);

	if(pKline)
	{
		delete[] pKline;     
		pKline=NULL;
	}


}

void CAlertSystem::AlarmSendMessage(CMainFrame *pFrame, CReportData *dt1, ALERT_SET_ITEM *item,int nCondFoot)
{
	if(dt1==0)
		return ;
	if(dt1->nowp<=0)
		return;

	ALERT_RESULT_ITEM* pItem = new ALERT_RESULT_ITEM;
	
	pItem->m_nFootAlarmArr =nCondFoot;
	CString StockId = dt1->id ;
	strcpy(pItem->m_symbol , dt1->id );
	pItem->m_time = CTime::GetCurrentTime ().GetTime ();
	pItem->m_fPrice = dt1->nowp ;
	pItem->m_stkKind = dt1->kind ;
	m_gAlarmResultList.AddTail (pItem);
	if(m_gAlarmResultList.GetCount ()> 5000)
		delete m_gAlarmResultList.RemoveHead ();



	if(pFrame->m_pDlgAlarm !=NULL)
	{
		int nCount = pFrame->m_pDlgAlarm->m_alarmResultList.GetItemCount( );
		int nCount2 = m_gAlarmResultList.GetCount ();
		if(nCount2>0)
		{
			ResultListAddRow(&pFrame->m_pDlgAlarm->m_alarmResultList ,nCount2-1);
			if(nCount> 2000)
				pFrame->m_pDlgAlarm->m_alarmResultList.DeleteItem(0);
		}
	}

	bool bFirst = true;

	bool bSend = true;
	if(item!=NULL)
	{
		if(item->m_btAddtional [0] ==0)
			bSend = false;
		static int nFoot =-1;
		if(nCondFoot == nFoot)
			bFirst = false;
		nFoot = nCondFoot;
	}
	if(bSend == true)
	{
		static DWORD dw = 0;

		DWORD dw2 = GetTickCount();
		if(dw2-dw>2000)
		{
			::MessageBeep (-1);
			dw = dw2;
		}

	}
	
	if(bSend == true )
	{
		CString s = StockId+": ";
		CAlertPopDlg* dlg;
		if(pFrame->m_pDlgAlarmPop != NULL)
		{
			pFrame->m_pDlgAlarmPop->ShowWindow(SW_SHOW);
		}
		else
		{
			dlg = new CAlertPopDlg;
			dlg->Create(IDD_1UPALERT );
			dlg->ShowWindow(SW_SHOW);
			::MessageBeep (-1);
		}
		if(pFrame->m_pDlgAlarmPop != NULL)
		{
			CString s2;
			POSITION pos=m_gAlarmArray.FindIndex (pItem->m_nFootAlarmArr );
			CString s3 = "";
			if(pos)
			{
				ALERT_SET_ITEM* itemCond = m_gAlarmArray.GetAt(pos);

				s3=itemCond->m_strAlarmName;
				s3 = s3+" "  ;
			}
			s2.Format ("%s%.2f",s3 , pItem->m_fPrice );
			s+=s2;

			pFrame->m_pDlgAlarmPop->m_conInfo.AddString (s);
			int nC = pFrame->m_pDlgAlarmPop->m_conInfo.GetCount();
			if(nC>0) pFrame->m_pDlgAlarmPop->m_conInfo.SetItemData (nC-1,dt1->kind );

			if(pFrame->m_pDlgAlarmPop->m_conInfo.GetCount()>500)
				pFrame->m_pDlgAlarmPop->m_conInfo.DeleteString( 0 );

		}

	}
}

void CAlertSystem::AddLineAlarmItem(INFO_LINESELF &lineInfo,CString s,bool bUpwards)
{

	ALERT_LINE_ITEM* item = NULL;
	bool bFound = false;
	for(int i=0;i<m_gLineAlarmArray.GetCount ();i++)
	{
		POSITION pos=m_gLineAlarmArray.FindIndex (i);
		item = m_gLineAlarmArray.GetAt(pos);
		CString s2 = item->m_symbolAlarm .m_sSymbol ;
		if(s==s2)
		{
			bFound = true;
			for(int j=0;j<item->m_lineInfoArr .GetSize ();j++)
			{
				if(item->m_lineInfoArr[j].lineInfo.time1 == lineInfo.time1 &&  item->m_lineInfoArr[j].lineInfo.valFlt1  == lineInfo.valFlt1  )
				{
					if(item->m_lineInfoArr[j].lineInfo.time2 == lineInfo.time2 &&  item->m_lineInfoArr[j].lineInfo.valFlt2  == lineInfo.valFlt2  )
					{
						item->m_lineInfoArr[j] .m_btAlarmed  = 0;
						item->m_lineInfoArr[j] .m_bUpwards  = bUpwards;
						
					
						GetLineAlarmInitData(item);
						return;
					}
				}
			}
			DATA_PER_LINE dataLine;
			dataLine.lineInfo = lineInfo;
			dataLine.m_btAlarmed =0;
			dataLine.m_bUpwards = bUpwards;

			item->m_lineInfoArr .Add (dataLine);
		

			break;
		}
	}

	if(bFound == false)
	{
		item = new ALERT_LINE_ITEM;
		strcpy(item->m_symbolAlarm .m_sSymbol ,s);
		DATA_PER_LINE dataLine;
		dataLine.lineInfo = lineInfo;
		dataLine.m_btAlarmed =0;
		dataLine.m_bUpwards = bUpwards;

		item->m_lineInfoArr .Add (dataLine);
		m_gLineAlarmArray.AddTail (item);
	
	}
	GetLineAlarmInitData(item);
}

void CAlertSystem::DeleteLineAlarmItem(INFO_LINESELF &lineInfo, CString s)
{
	ALERT_LINE_ITEM* item = NULL;
	for(int i=0;i<m_gLineAlarmArray.GetCount ();i++)
	{
		POSITION pos=m_gLineAlarmArray.FindIndex (i);
		item = m_gLineAlarmArray.GetAt(pos);
		CString s2 = item->m_symbolAlarm .m_sSymbol ;
		if(s==s2)
		{
			for(int j=0;j<item->m_lineInfoArr .GetSize ();j++)
			{
				if(item->m_lineInfoArr[j].lineInfo.time1 == lineInfo.time1 &&  FABSMY(item->m_lineInfoArr[j].lineInfo.valFlt1  - lineInfo.valFlt1)<0.1  )
				{
					if(item->m_lineInfoArr[j].lineInfo.time2 == lineInfo.time2 &&  FABSMY(item->m_lineInfoArr[j].lineInfo.valFlt2  - lineInfo.valFlt2)<0.1  )
					{
						item->m_lineInfoArr.RemoveAt(j);
					
						return;
					}
				}
			}
			break;
		}
	}


}

void CAlertSystem::GetLineAlarmInitData(ALERT_LINE_ITEM* item)
{
	CTaiShanDoc* pDoc =CMainFrame::m_taiShanDoc ;

    int stocknum = 1;
	int index_num = item->m_lineInfoArr .GetSize ();
	if(pDoc == NULL)
		return;
	int nRead,k;
	CString StockId;
	StockId = item->m_symbolAlarm .m_sSymbol ;
	int stkKind = item->m_symbolAlarm .m_stkKind ;
	int result = 0 ;


	{
		Kline *pKline=NULL;
		CBuySellList buySellList ;
		CTime tmNow = CTime::GetCurrentTime ();
		int nToday = atoi(tmNow.Format ("%Y%m%d"));

	    for(k=0;k<index_num;k++)
		{
			int nDays=-1;
			int tmLineB = item->m_lineInfoArr [k].lineInfo.time1 ;
			bool bMIn5 = true;
			if(item->m_lineInfoArr [k].lineInfo.klineType >4 && item->m_lineInfoArr [k].lineInfo.klineType < 9)
			{
				tmLineB = atoi(CTime(tmLineB).Format ("%Y%m%d"));
				bMIn5 = false;
			}

		
			nRead = CTaiScreenParent::ReadKLine(StockId,stkKind, pKline, nDays,item->m_lineInfoArr [k].lineInfo.klineType ,&buySellList, 48 ,FALSE);
			
			if(nRead<=0)
			{
				item->m_lineInfoArr [k].m_gapFromBeginArr= 0;
				continue;
			}

			int nFlag = 1;
			if(atoi(CTime(pKline[nRead-1].day).Format ("%Y%m%d")) >= nToday)
				nFlag = -1;

			int nAdd = 0;
			if(nFlag == 1 && bMIn5 == false)
			{
				CTime tm(pKline[nRead-1].day);
				int nYear = tm.GetYear ();
				int nMon = tm.GetMonth  ();
				int nDay = tm.GetDay  ();
				int nDayOfWeek=tmNow.GetDayOfWeek ()-1;

				CTime tm2(nYear,nMon,nDay,0,0,0);
				CTimeSpan tmSpan = tmNow-tm2;
				switch (item->m_lineInfoArr [k].lineInfo.klineType )
				{
				case 5:
					nAdd = 1;
					break;
				case 6:
					if(nDayOfWeek - tmSpan.GetDays ()>0)
						nAdd=1;
					break;
				case 7:
					if(nYear!=tmNow.GetYear () || nMon!=tmNow.GetMonth () )
						nAdd=1;
					break;
				case 8:
					if(nYear!=tmNow.GetYear () )
						nAdd=1;
					break;
				}

			}

			int j=0;
			while(nRead-1-j>=0)
			{
				int tmb = pKline[nRead-1-j].day ;
				if(bMIn5 == false)
					tmb = atoi(CTime(tmb).Format ("%Y%m%d"));

				if(tmb<=tmLineB)
					break;
				j++;
			}
			item->m_lineInfoArr [k].m_gapFromBeginArr=(j+nAdd)*nFlag;
		}
		CTaiKlineFileHS::RemoveHs(buySellList);

		if(pKline)
		{
			delete[] pKline;    
			pKline=NULL;
		}
	}

}

void CAlertSystem::AlarmArrInsertRow(ALERT_SET_ITEM *item,int nItem)
{



	CString sArr[10];
	sArr[0]=item->m_strAlarmName ;
	sArr[1].Format ("%d",item->m_bAlarmedArray .GetSize () );
	int nNotAlarmed = 0;
	for(int i=0;i<item->m_bAlarmedArray .GetSize();i++)
	{
		if(item->m_bAlarmedArray [i].m_btAlarmed == 0)
			nNotAlarmed ++;
	}
	sArr[2].Format ("%d",item->m_bAlarmedArray .GetSize ()-nNotAlarmed);
	InsertRow(&this->m_condArrayList , sArr, 3, 10,nItem);



}

void CAlertSystem::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == 1)
	{
		for(int i=0;i<m_gAlarmArray.GetCount ();i++)
		{
			POSITION pos=m_gAlarmArray.FindIndex (i);
			ALERT_SET_ITEM* item = m_gAlarmArray.GetAt(pos);

			CString s="";
			int nNotAlarmed = 0;
			for(int j=0;j<item->m_bAlarmedArray .GetSize();j++)
			{
				if(item->m_bAlarmedArray [j].m_btAlarmed == 0)
					nNotAlarmed ++;
			}
			s.Format ("%d",item->m_bAlarmedArray .GetSize()-nNotAlarmed);
			m_condArrayList.SetItemText (i,2,s);
		}


		{
			int n = m_alarmResultList.GetItemCount();
			for(int i=0;i<n;i++)
			{
				CReportData* dt1;
				float fPrc = 0;
				CString s = m_alarmResultList.GetItemText (i,0);

				int stkKind = m_alarmResultList.GetItemData (i);
				if(pDoc->m_sharesInformation.Lookup(s.GetBuffer (0),dt1,stkKind))
				{
					fPrc= dt1->nowp ;
					s.ReleaseBuffer ();
					s.Format ("%.2f",fPrc);
					m_alarmResultList.SetItemText (i,3,s);
				}
			}
		}

	}
	
	CDialog::OnTimer(nIDEvent);
}

void CAlertSystem::GetScreenStockBlockSymbol(SymbolKindArr &sArr, CString sBlockName,int k)
{
	sArr.RemoveAll ();
	CString s = sBlockName;

	CTaiShanDoc *m_pDoc = CMainFrame::m_taiShanDoc ;
	POSITION position=m_pDoc->m_Tjxgblocklist.GetHeadPosition();
	int j=0;
	BLOCKSTOCK * blocknow=NULL;
	bool bFind = false;
	while(position)
	{
		blocknow=m_pDoc->m_Tjxgblocklist.GetNext(position);
		CString s2 = blocknow->blockname;
		s2.MakeLower ();
		s.MakeLower ();
		if(s2 == s || k>=0 && j==k-2000)
		{
			bFind = true;
			break;
		}
		j++;
	}

	if(bFind == true)
	{
		POSITION position=blocknow->stocklist.GetHeadPosition();
		int j=0;

		SharesSymbol* pStockSymbol = NULL;
		while(position)
		{
			SymbolKind sm;
			pStockSymbol=blocknow->stocklist.GetNext(position);
			s = pStockSymbol->sharesymbol; 
			strcpy(sm.m_chSymbol ,s);
			sm.m_nSymbolKind  = pStockSymbol->nKind ;
			CString s2 = s;
			CReportData* Cdat;
		 	if (m_pDoc->m_sharesInformation.Lookup(s.GetBuffer (0), Cdat,sm.m_nSymbolKind ) == TRUE)     //检测该股票是否已记录在内存模板中
				sArr.Add (sm);
		}
	}

}

void CAlertSystem::SwitchAlarm()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_wndStatusBar.InvalidateRect(pFrame->m_wndStatusBar.m_aPaneRect[5]);
	if(pFrame->m_bAlarmExec == false)
	{
		StartAlarm(false);
		if(pFrame->m_pDlgAlarm !=NULL&& pFrame->m_bAlarmExec == true)
		{
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
			pFrame->m_pDlgAlarm ->GetDlgItem(IDOK)->SetWindowText("关闭预警");
		}
	}
	else
	{
		pFrame->KillTimer (TJXG_ALERT);
		if(pFrame->m_pDlgAlarm !=NULL)
		{
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON1)->EnableWindow();
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON2)->EnableWindow();
			pFrame->m_pDlgAlarm ->GetDlgItem(IDC_BUTTON3)->EnableWindow();
			pFrame->m_pDlgAlarm ->GetDlgItem(IDOK)->SetWindowText("启动预警");
		}
		pFrame->m_bAlarmExec = false;
	}

}

BOOL CAlertSystem::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
	

}

void CAlertSystem::ClearAllResult()
{
	CString Id;
	POSITION pos=m_gAlarmResultList.GetHeadPosition ();
	while(pos)
	{
		delete m_gAlarmResultList.GetNext (pos);
	}
	m_gAlarmResultList.RemoveAll();

	m_alarmResultList.DeleteAllItems ();

}
