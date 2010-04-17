// KEYBRODEANGEL.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>

#include "KEYBRODEANGEL.h"

#include "MainFrm.h"
//including this header for getting all data
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKeyBoardAngelDlg dialog

CTaiKeyBoardAngelDlg::CTaiKeyBoardAngelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKeyBoardAngelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKeyBoardAngelDlg)
	m_strInputResult = _T("");
	//}}AFX_DATA_INIT
	//This is for transmit message of Keyboard angel
	m_stkKind = 0;
	m_wndParent = pParent;
	m_bFellowingMouse=FALSE;
	m_nFellowingPt.x=0;
	m_nFellowingPt.y=0;
}

void CTaiKeyBoardAngelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKeyBoardAngelDlg)
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_INPUT, m_InputEditCtrl);
	DDX_Text(pDX, IDC_INPUT, m_strInputResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKeyBoardAngelDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiKeyBoardAngelDlg)
	ON_EN_CHANGE(IDC_INPUT, OnChangeInput)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindragList)
	ON_EN_SETFOCUS(IDC_INPUT, OnSetfocusInput)
	ON_EN_KILLFOCUS(IDC_INPUT, OnKillfocusInput)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_DRAG_BEGIN,OnDragBegin)//
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKeyBoardAngelDlg message handlers

BOOL CTaiKeyBoardAngelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;

  
	m_strInputResult=input;
	inputpre.Empty();
	inputpre.Format("");
	m_nListPosition=0;
	m_nCurrentItemIndex=0;
	m_bLastFocusFlag=FALSE;
	isresultok=false;

	m_ListCtrl.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	m_ListCtrl.m_wndTarget = m_wndParent;
	m_nStrLength=pDoc->m_keychardata.GetSize();
	m_nNumberLength=pDoc->m_keynumberdata.GetSize();

	int length;
	if(ischar)
		length=pDoc->m_keychardata.GetSize();
	else
		length=pDoc->m_keynumberdata.GetSize();
	m_nStartPosArray.Add(0);
	m_nEndPosArray.Add(length);

	CalculateListRange(m_strInputResult);

	int nLength=m_strInputResult.GetLength();
	int starti=m_nStartPosArray.GetAt(nLength);
	int endi=m_nEndPosArray.GetAt(nLength);
	int end=(starti+6<endi?starti+6:endi);
	
	for(int i=starti;i<end;i++)
 		InsertItemToList(i,starti,0);
 
	m_nPreStarIndex=starti;
	m_nPreEndIndex=end;
	m_ListCtrl.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED ,LVIF_STATE);
	m_ListCtrl.SetFocus();
	
	UpdateData(FALSE);
  	m_InputEditCtrl.SetFocus();
	m_InputEditCtrl.SetSel(1,1);	
    if(m_bFellowingMouse)
	{
		CRect rect;
		GetClientRect(rect);
		SetWindowPos(NULL,m_nFellowingPt.x,m_nFellowingPt.y,rect.Width(),rect.Height()+20,SWP_SHOWWINDOW);
	}
	return FALSE;   
}

void CTaiKeyBoardAngelDlg::OnChangeInput() 
{
	UpdateData();
	if(m_strInputResult.GetLength()!=0)
	{
		bool ischarnow;
		char FirstChar=m_strInputResult.GetAt(0);
		if(FirstChar>='0'&&FirstChar<='9')
			ischarnow=false;
		else
			ischarnow=true;
		if(ischarnow!=ischar)
		{
			if(ischarnow)
				m_nEndPosArray.ElementAt(0)=m_nStrLength;
			else
				m_nEndPosArray.ElementAt(0)=m_nNumberLength;
			ischar=ischarnow;
		}
		
		m_InputEditCtrl.SetSel(m_strInputResult.GetLength()+1,m_strInputResult.GetLength()+1);
		CalculateListRange(m_strInputResult);
		int length=m_strInputResult.GetLength();
		int nStart=m_nStartPosArray.GetAt(length);
		int nEnd=m_nEndPosArray.GetAt(length);
		int nEnds=(nStart+6<nEnd?nStart+6:nEnd);

		if((nStart!=m_nPreStarIndex) || (nEnd!=m_nPreEndIndex))
		{
			m_ListCtrl.DeleteAllItems();
			for(int i=nStart;i<nEnds;i++)
				InsertItemToList(i,nStart,0);
			m_nPreStarIndex=nStart;
			m_nPreEndIndex=nEnd;
		}
	}
	else
	{
		m_nStartPosArray.RemoveAt(1);
		m_nEndPosArray.RemoveAt(1);	
		m_ListCtrl.DeleteAllItems();
		m_nPreStarIndex=m_nPreEndIndex=-1;
	}
	m_nListPosition=0;
	inputpre=m_strInputResult;
}


void CTaiKeyBoardAngelDlg::CalculateListRange(CString strInput)
{
	
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
	//
  	int nStrLength=strInput.GetLength();
	int nStrPreLength=inputpre.GetLength();
	int nMinLength=min(nStrLength,nStrPreLength);
	int nChangeIndex;
	BOOL bChangeSort;       
	int i,j;
	//
	for(i=0;i<nMinLength;i++)
	{
		if(strInput[i]!=inputpre[i])
			break;
	}
	if(i>=nMinLength)
		bChangeSort=TRUE;
	else
	{
		nChangeIndex=i;
		bChangeSort=FALSE;
	}

	int nStart,nEnd,nNewStart,nNewEnd;
	char chTest;
	int nArrayLength;
	if(bChangeSort)
	{
 		nStart=m_nStartPosArray.GetAt(nStrLength-1);
		nEnd=m_nEndPosArray.GetAt(nStrLength-1);
		chTest=strInput.GetAt(nStrLength-1);

		for( i=nStart;i<nEnd;i++)
		{
			DATA_KEYBOARD *KeyData;
			if(ischar)
				KeyData=pDoc->m_keychardata.GetAt(i);
			else
				KeyData=pDoc->m_keynumberdata.GetAt(i);

    		if(chTest==(*((KeyData->key)+nStrLength-1)))
			   break;
		}
		if(i<nEnd)
		{
			nNewStart=i;
			for(i=nNewStart;i<nEnd;i++)
			{
				DATA_KEYBOARD *KeyData;
				if(ischar)
					KeyData=pDoc->m_keychardata.GetAt(i);
				else
					KeyData=pDoc->m_keynumberdata.GetAt(i);
				if(chTest!=(*((KeyData->key)+nStrLength-1)))
				   break;
			}
			nNewEnd=i;
		}
		else
		{
			nNewStart=i;
			nNewEnd=i;
		}
		nArrayLength=m_nStartPosArray.GetSize();
		if(nStrLength>nArrayLength-1)
		{
			m_nStartPosArray.Add(nNewStart);
			m_nEndPosArray.Add(nNewEnd);
		}
		else
			if(nStrLength<nArrayLength-1)
			{
				m_nStartPosArray.RemoveAt(nArrayLength-1);
				m_nEndPosArray.RemoveAt(nArrayLength-1);
			}
	}
	else
	{
		nArrayLength=m_nStartPosArray.GetSize();
		for(i=nChangeIndex+1;i<nArrayLength;i++)
		{
			m_nStartPosArray.RemoveAt(nChangeIndex+1);
			m_nEndPosArray.RemoveAt(nChangeIndex+1);
		}
		nArrayLength=m_nStartPosArray.GetSize();
		
		for(j=nArrayLength;j<nStrLength+1;j++)
		{
			nStart=m_nStartPosArray.GetAt(j-1);
			nEnd=m_nEndPosArray.GetAt(j-1);
			chTest=strInput.GetAt(j-1);

			for( i=nStart;i<nEnd;i++)
			{
				DATA_KEYBOARD *KeyData;
				if(ischar)
					KeyData=pDoc->m_keychardata.GetAt(i);
				else
					KeyData=pDoc->m_keynumberdata.GetAt(i);

				if(chTest==(*((KeyData->key)+j-1)))
					break;
			}
			if(i<nEnd)
			{
				nNewStart=i;
				for(i=nNewStart;i<nEnd;i++)
				{
					DATA_KEYBOARD *KeyData;
					if(ischar)
						KeyData=pDoc->m_keychardata.GetAt(i);
					else
						KeyData=pDoc->m_keynumberdata.GetAt(i);
					if(chTest!=(*((KeyData->key)+j-1)))
						break;
				}
				nNewEnd=i;
			}
			else
			{
				nNewStart=i;
				nNewEnd=i;
			}
			m_nStartPosArray.Add(nNewStart);
			m_nEndPosArray.Add(nNewEnd);
		}
	}
}


void CTaiKeyBoardAngelDlg::InsertItemToList(int nWhichItem,int nStartItem,int nDefItem)
{
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;

	LVITEM   InsertItem,DefaultItem;
	InsertItem.mask=LVIF_TEXT|LVIF_IMAGE;
	InsertItem.iItem=nWhichItem-nStartItem;
	InsertItem.iSubItem=0;

	DefaultItem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	DefaultItem.iItem=nDefItem;
	DefaultItem.iSubItem=0;
	DefaultItem.state=LVIS_SELECTED|LVIS_FOCUSED; 

	char *strData=NULL;
	char *strKey=NULL;
	if(ischar)
	{
		if(pDoc->m_keychardata.GetAt(nWhichItem)->id==3)  
			strData=pDoc->m_keychardata.GetAt(nWhichItem)->data;
		else
			strData=SymbolToName(pDoc->m_keychardata.GetAt(nWhichItem)->data);
	}
	else
		strData=pDoc->m_keynumberdata.GetAt(nWhichItem)->data;

	if(ischar)
		strKey=pDoc->m_keychardata.GetAt(nWhichItem)->key;
	else
		strKey=pDoc->m_keynumberdata.GetAt(nWhichItem)->key;

	CString stringdata=CString(strData,8);
	CString stringkey=CString(strKey);
	int nStrLength=stringkey.GetLength();
	if(nStrLength<6)
	{
		int numofblank=6-nStrLength;
		for(int k=0;k<numofblank;k++)
			stringkey+=" ";
	}
	CString str=stringkey+" "+stringdata;
	InsertItem.pszText=str.GetBuffer(15);
	BYTE  id;
	if(ischar)
		id=pDoc->m_keychardata.GetAt(nWhichItem)->id;
	else
		id=pDoc->m_keynumberdata.GetAt(nWhichItem)->id;
	switch(id)
	{
		case(0):
		case(2):
			InsertItem.iImage=0;
			break;
		case(1):
			InsertItem.iImage=2;
			break;
		case(3):
			InsertItem.iImage=4;
			break;
	}
	if(nWhichItem==nStartItem+nDefItem)
	{
		DefaultItem.iImage=InsertItem.iImage;
		DefaultItem.pszText=InsertItem.pszText;
		m_ListCtrl.InsertItem(&DefaultItem);
	}
	else
		m_ListCtrl.InsertItem(&InsertItem);
}


BOOL CTaiKeyBoardAngelDlg::PreTranslateMessage(MSG* pMsg) 
{
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;
    
	int nLength,i;	
	switch(pMsg->message)
	{
		case(WM_KEYDOWN):
			if(::GetKeyState(VK_CONTROL)&0x8000 && pMsg->wParam =='V')
				return TRUE;
			switch(pMsg->wParam)
			{
				case(40):
					m_ListCtrl.SetFocus();
					m_nCurrentItemIndex=m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
					if(m_nCurrentItemIndex==5&&m_ListCtrl.GetItemCount()>5)
					{
						m_nListPosition++;
						nLength=m_strInputResult.GetLength();
						int nStart=m_nStartPosArray.GetAt(nLength)+m_nListPosition;
						int nEnd=m_nEndPosArray.GetAt(nLength);
						int nEnds=(nStart+6<nEnd?nStart+6:nEnd);
						if (nEnds>nStart)
						{
							m_ListCtrl.DeleteAllItems();
							for(int i=nStart;i<nEnds;i++)
								InsertItemToList(i,nStart,4);
						}
						else
						{
							m_nListPosition--;
						}
					}
					m_bLastFocusFlag=TRUE;
					break;
				case(38):               
					m_ListCtrl.SetFocus();
					m_nCurrentItemIndex=m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
					if(m_nCurrentItemIndex==0&&m_nListPosition>0)
					{
						m_nListPosition--;
						nLength=m_strInputResult.GetLength();
						int nStart=m_nStartPosArray.GetAt(nLength)+m_nListPosition;
						int nEnd=nStart+6;
						m_ListCtrl.DeleteAllItems();
						for(int i=nStart;i<nEnd;i++)
							InsertItemToList(i,nStart,0);
					}
					m_bLastFocusFlag=TRUE;
					break;

				case(8): 
					if(!m_bLastFocusFlag)
						break;
					
				case(46):           
					nLength=m_strInputResult.GetLength();
					if(nLength>0)
					{
						m_strInputResult.Delete(nLength-1);
						m_InputEditCtrl.SetFocus();
						UpdateData(FALSE);
						m_bLastFocusFlag=FALSE;
						OnChangeInput();
						return TRUE;
					}
					break;

				case(13):
					result=NULL;
					m_nCurrentItemIndex=m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
					nLength=m_strInputResult.GetLength();
					if(m_nCurrentItemIndex<0)
					{
						isresultok=false;
						return CDialog::PreTranslateMessage(pMsg);
					}
					OnReturn();
					break;
				
				}
			break;
		case(WM_CHAR):
			if(m_bLastFocusFlag&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||pMsg->wParam>='A'&&pMsg->wParam<='Z'||pMsg->wParam>='a'&&pMsg->wParam<='z'))
			{
				m_InputEditCtrl.SetFocus();
				m_bLastFocusFlag=FALSE;
				m_strInputResult+=(char)pMsg->wParam;
				m_InputEditCtrl.SetSel(m_strInputResult.GetLength(),m_strInputResult.GetLength());
				UpdateData(FALSE);
				OnChangeInput();
				return TRUE;
			}
			else if (pMsg->wParam >= 0xa0)
				return TRUE;
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


char* CTaiKeyBoardAngelDlg::SymbolToName(char *pSymbol)
{
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;
	int nStockLength=pDoc->m_keynumberdata.GetSize();
	int i;
 	for(i=0;i<nStockLength;i++)
	{
		char *pSearchKey=NULL;
		pSearchKey=pDoc->m_keynumberdata.GetAt(i)->key;
		if(strcmp(pSymbol,pSearchKey)==0)
			break;
	}
	if(i<nStockLength)
		return pDoc->m_keynumberdata.GetAt(i)->data;
	else
		return NULL;
}

void CTaiKeyBoardAngelDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;
	UpdateData();
	OnReturn();
    OnOK();
	*pResult = 0;
}

void CTaiKeyBoardAngelDlg::OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;
	UpdateData();
	m_nCurrentItemIndex=m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int length=0;
	int i;	
	length = m_strInputResult.GetLength();
	if(m_nCurrentItemIndex<0)
	{
		isresultok=false;
		m_ListCtrl.m_wParam = 100;
		return;
	}
    
	i=m_nCurrentItemIndex+m_nStartPosArray.GetAt(length)+m_nListPosition;

	isresultok=true;

	if(ischar)
	{
		if(pDoc->m_keychardata.GetAt(i)->id==2)
		{
			result=pDoc->m_keychardata.GetAt(i)->data;
			m_ListCtrl.m_wParam = 0+16;
			isresultstock=true;
		}
		else
		{
			result=pDoc->m_keychardata.GetAt(i)->key;
			m_ListCtrl.m_wParam = 1+16;
			isresultstock=false;
		}
	}
	else
	{
		result=pDoc->m_keynumberdata.GetAt(i)->key;
		if(pDoc->m_keynumberdata.GetAt(i)->id==0)
		{
			m_ListCtrl.m_wParam = 0+16;
			isresultstock=true;
		}
		else
		{
			m_ListCtrl.m_wParam = 1+16;
			isresultstock=false;
		}
	
	}			
	*pResult = 0;
}

LRESULT CTaiKeyBoardAngelDlg::OnDragBegin(WPARAM wParam, LPARAM lParam)
{
	if(wParam == 0)
	{
	    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;
		UpdateData();
		m_nCurrentItemIndex=m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
		int length=0,i;	

		length = m_strInputResult.GetLength();
		if(m_nCurrentItemIndex<0)
		{
			isresultok=false;
			m_ListCtrl.m_wParam = 100;
			m_ListCtrl.m_strResult = "";
			return 0;
		}
    
		i=m_nCurrentItemIndex+m_nStartPosArray.GetAt(length)+m_nListPosition;

		isresultok=true;

		if(ischar)
		{
			if(pDoc->m_keychardata.GetAt(i)->id==2)
			{
				result=pDoc->m_keychardata.GetAt(i)->data;
				m_stkKind = pDoc->m_keychardata.GetAt(i)->m_stkKind ;
				m_ListCtrl.m_wParam = 0+16;
				CString s2;
				s2.Format("%2d",m_stkKind);
				m_ListCtrl.m_strResult = s2+result;
				m_ListCtrl.m_stkKind = this->m_stkKind ;
				isresultstock=true;
			}
			else
			{
				result=pDoc->m_keychardata.GetAt(i)->key;
				m_stkKind = pDoc->m_keychardata.GetAt(i)->m_stkKind ;
				m_ListCtrl.m_wParam = 1+16;
				m_ListCtrl.m_strResult = result;
				m_ListCtrl.m_stkKind = this->m_stkKind ;
				isresultstock=false;
			}
		}
		else
		{
			result=pDoc->m_keynumberdata.GetAt(i)->key;
			m_stkKind = pDoc->m_keynumberdata.GetAt(i)->m_stkKind ;
			if(pDoc->m_keynumberdata.GetAt(i)->id==0)
			{
				m_ListCtrl.m_wParam = 0+16;
				CString s2;
				s2.Format("%2d",m_stkKind);
				m_ListCtrl.m_strResult = s2+result;
				m_ListCtrl.m_stkKind = m_stkKind;
				isresultstock=true;
			}
			else
			{
				m_ListCtrl.m_wParam = 100;
				isresultstock=false;
				m_ListCtrl.m_strResult = result;
				m_ListCtrl.m_stkKind = m_stkKind;
			}
		}	
	}
	else
	    this->PostMessage(WM_CLOSE);

	return 0;
}

void CTaiKeyBoardAngelDlg::SetStartPos(BOOL bFellowingMouse,int px,int py)
{
	m_bFellowingMouse=bFellowingMouse;
	m_nFellowingPt.x=px;
	m_nFellowingPt.y=py;
}

void CTaiKeyBoardAngelDlg::OnSetfocusInput() 
{
	m_bLastFocusFlag=FALSE;
}

void CTaiKeyBoardAngelDlg::OnKillfocusInput() 
{ 
	m_bLastFocusFlag=TRUE;	
}

void CTaiKeyBoardAngelDlg::OnReturn()
{
	result=NULL;
    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;
	m_nCurrentItemIndex=m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int length=m_strInputResult.GetLength();
	int i;	
	if(m_nCurrentItemIndex<0)
	{
		isresultok=false;
		return;
	}
    
	i=m_nCurrentItemIndex+m_nStartPosArray.GetAt(length)+m_nListPosition;

	isresultok=true;

	if(ischar)
	{
		if(pDoc->m_keychardata.GetAt(i)->id==2)
		{
			result=pDoc->m_keychardata.GetAt(i)->data;
			m_stkKind = pDoc->m_keychardata.GetAt(i)->m_stkKind;
			isresultstock=true;
		}
		else
		{
			result=pDoc->m_keychardata.GetAt(i)->key;
			isresultstock=false;
		}
	}
	else
	{
		result=pDoc->m_keynumberdata.GetAt(i)->key;
		if(pDoc->m_keynumberdata.GetAt(i)->id==0)
		{
			isresultstock=true;
			m_stkKind = pDoc->m_keynumberdata.GetAt(i)->m_stkKind;
		}
		else
			isresultstock=false;
	}			
}

void CTaiKeyBoardAngelDlg::AddIndicatorName(CString name)
{
	CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;
	DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
	strcpy(keytoadd->key,name);
	if(name.GetLength()<=4)
		name+="Ö¸±ê";
	else
		name+="Ïß";
	strcpy(keytoadd->data,name.GetBuffer(name.GetLength()));
	keytoadd->id=3;
	int keylength=pDoc->m_keychardata.GetSize();
	int j;
	for(j=0;j<keylength;j++)
	{
		if(strcmp(pDoc->m_keychardata.GetAt(j)->key,name)>0)
			break;
	}
	pDoc->m_keychardata.InsertAt(j,keytoadd);

}

void CTaiKeyBoardAngelDlg::DeleteIndicatorName(CString name)
{
	CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;
	CString namepre=name;
	int keylength=pDoc->m_keychardata.GetSize();
	int j;
	for(j=0;j<keylength;j++)
	{
			if(strcmp(pDoc->m_keychardata.GetAt(j)->key,namepre)==0)
				break;
	}
	if(j<keylength)
		pDoc->m_keychardata.RemoveAt(j);

}
