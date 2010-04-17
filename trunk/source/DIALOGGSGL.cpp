// DIALOGGSGL.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "DIALOGGSGL.h"
#include "DIAEDITZJXT.h"
#include "DIGEDITTJXG.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "DIAGSGLMM.h"
#include "CTaiKlineIndicatorsManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogLOGGSGL::CDialogLOGGSGL(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLOGGSGL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLOGGSGL)
	m_tabedit = _T("");
	m_editexplain = _T("");
	m_sProductCode = _T("");
	m_check = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogLOGGSGL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLOGGSGL)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_SAVE, m_save);
	DDX_Control(pDX, IDC_EDIT1, m_conProduct);
	DDX_Control(pDX, IDC_TABEDIT, m_editcor);
	DDX_Control(pDX, IDC_TABLIST, m_tablist);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
	DDX_Text(pDX, IDC_TABEDIT, m_tabedit);
	DDX_Text(pDX, IDC_BRIEFEXPLAIN, m_editexplain);
	DDX_Text(pDX, IDC_EDIT1, m_sProductCode);
	DDX_Check(pDX, IDC_CHECK1, m_check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLOGGSGL, CDialog)
	//{{AFX_MSG_MAP(CDialogLOGGSGL)
	ON_NOTIFY(NM_CLICK, IDC_TAB1, OnClickTab1)
	ON_BN_CLICKED(IDC_MODIFYGS, OnModifygs)
	ON_BN_CLICKED(IDC_ADDGS, OnAddgs)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TABLIST, OnItemchangedTablist)
	ON_BN_CLICKED(ID_DELETEGS, OnDeletegs)
	ON_NOTIFY(NM_DBLCLK, IDC_TABLIST, OnDblclkTablist)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDialogLOGGSGL::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CTaiShanDoc*   pDoc=CMainFrame::m_taiShanDoc ;

	TC_ITEM tci[3];
	CString str[3]={"技术指标公式","条件选股公式","K线组合公式"};
	for(int i=0;i<3;i++)
	{
	tci[i].mask=TCIF_TEXT;
	tci[i].pszText=(LPSTR)(LPCTSTR)str[i];
	tci[i].cchTextMax=str[i].GetLength();
	m_tab1.InsertItem(i,&tci[i]);
	}
                                
	
	m_tablist.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	
	UpdateIndexData(0);
	UpdateData(FALSE);

	SetWindowText("公式存储");

	this->m_conProduct .EnableWindow(FALSE);


	
	return TRUE; 
}

void CDialogLOGGSGL::OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc ;

    tabitem=m_tab1.GetCurSel();	

	
	switch(tabitem)
	{
	case 0:
		
		UpdateIndexData(0);
		UpdateData(FALSE);
		
		break;
	case 1:
		UpdateChooseData(0);
		UpdateData(FALSE);
	
		break;
	case 2:

		UpdateKlineData(0);
		UpdateData(FALSE);
		break;
    case 3:


		UpdateMasterData(0);
		UpdateData(FALSE);
		break;

	}

	*pResult = 0;
}



void CDialogLOGGSGL::OnModifygs() 
{

}
void CDialogLOGGSGL::OnAddgs() 
{

}

void CDialogLOGGSGL::OnSave() 
{

	UpdateData();
	if(this->m_check == TRUE)
	{
		if(this->m_sProductCode .GetLength()!=16)
		{
			AfxMessageBox("您输入的产品序列号可能不正确，请重新输入！");
			return ;
		}
	}

	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc ;

    CString     defaultname="";        

	int ncurtab=m_tab1.GetCurSel();
	CString sArray[]={"*.gs1","*.gs2","*.gs3"};        
	defaultname=sArray[ncurtab];  
	CString     defaultname2 = defaultname+"||";

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,defaultname2);
	m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
	                               
	CFormularContent::m_bVer20 = false;
	if(m_filedia.DoModal()==IDOK)
	{
		CString filename=m_filedia.GetPathName();
		if(filename.Find('.') == -1)
		{
	      CString temp = sArray[ncurtab];
		  temp.TrimLeft ("*");
		  filename+=temp;
		}

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
		CFormularContent::m_bVer20 = true;
		file.Open(filename,CFile::modeCreate|CFile::modeWrite);
		POSITION pos=m_tablist.GetFirstSelectedItemPosition();
		CArchive ar(&file,CArchive::store);
		int selectcount=0;
		ar<<selectcount;
		selectcount=m_tablist.GetSelectedCount();
		int nOut = selectcount;
		Formu_Array1* pArray[] = {&(pDoc->m_formuar_index ),&(pDoc->m_formuar_choose ),&(pDoc->m_formuar_kline )};
		for(int i=0;i<selectcount;i++)
		{
			int item=m_tablist.GetNextSelectedItem(pos);
			CFormularContent *jishu;
			jishu=pArray[ncurtab]->GetAt(item);          		 

			CString buyStr = jishu->buyStr;
			if(buyStr!="")
				nOut--;
		}
		ar<<nOut;
		ar<<ncurtab;
		pos=m_tablist.GetFirstSelectedItemPosition();
		for(int i=0;i<nOut;i++)
		{
			int item=m_tablist.GetNextSelectedItem(pos);
			CFormularContent *jishu;
			jishu=pArray[ncurtab]->GetAt(item);          		 

			CString buyStr = jishu->buyStr;
			if(buyStr!="")
				continue;
			CString password = jishu->password;
			BOOL isProtected = jishu->isProtected;
			if(this->m_check == TRUE)
			{
				jishu->buyStr = this->m_sProductCode ;
				jishu->isProtected = TRUE;
				if(jishu->password .GetLength ()==0)
					jishu->password = "1234567890";
			}
			else
				jishu->buyStr = "";
			ar<<jishu;

		
			jishu->buyStr		= buyStr		;
			jishu->password		= password	;
			jishu->isProtected	= isProtected ;
					
		}
		CString lengthstr;
		lengthstr.Format("%d",nOut);
		if(selectcount>nOut)
			AfxMessageBox("部分公式因加密不允许导出！");
		AfxMessageBox("一共在文件里写了"+lengthstr+"个公式");
		ar.Close();
		file.Close();
		CFormularContent::m_bVer20 = false;

	}
	
	
}

void CDialogLOGGSGL::OnRead() 
{
}

void CDialogLOGGSGL::OnOK() 
{

	UpdateData();
	
	CDialog::OnOK();
}
void CDialogLOGGSGL::OnCancel() 
{
	
	CDialog::OnCancel();
}







void CDialogLOGGSGL::UpdateIndexData(int index)
{
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc ;
	
	m_tablist.DeleteAllItems();
	int arraylength=pDoc->m_formuar_index.GetSize();
	if(arraylength<=index||index<0)
	{
		m_editexplain="";
		m_tabedit="";
		UpdateData(FALSE);
		return;
	}
	LVITEM mydefitem;
	mydefitem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	mydefitem.iItem=index;
	mydefitem.iSubItem=0;
	mydefitem.iImage=4;
	CString name=pDoc->m_formuar_index.GetAt(index)->name;
	mydefitem.pszText=name.GetBuffer(name.GetLength());
	mydefitem.state=LVIS_SELECTED|LVIS_FOCUSED; 

    

	for(int i=0;i<arraylength;i++)
	{
		if(i!=index)
			m_tablist.InsertItem(i,pDoc->m_formuar_index.GetAt(i)->name,4);
		else
			m_tablist.InsertItem(&mydefitem);

	}

	m_editexplain=pDoc->m_formuar_index.GetAt(index)->explainBrief;
	if(pDoc->m_formuar_index.GetAt(index)->isProtected)
		m_tabedit="此指标公式被密码保护";
	else
		m_tabedit=pDoc->m_formuar_index.GetAt(index)->help;
	UpdateData(FALSE);
	m_tablist.EnsureVisible(index,FALSE);
	m_tablist.SetFocus();

	
	

}


void CDialogLOGGSGL::UpdateChooseData(int index)
{
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc ;

	m_tablist.DeleteAllItems();
	int arraylength=pDoc->m_formuar_choose.GetSize();
	
	if(arraylength<=index||index<0)
	{
		m_editexplain="";
		m_tabedit="";
		UpdateData(FALSE);
		return;
	}
	LVITEM mydefitem;
	mydefitem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	mydefitem.iItem=index;
	mydefitem.iSubItem=0;
	mydefitem.iImage=3;
	CString name=pDoc->m_formuar_choose.GetAt(index)->name;
	mydefitem.pszText=name.GetBuffer(name.GetLength());
	mydefitem.state=LVIS_SELECTED|LVIS_FOCUSED; 

	
	for(int i=0;i<arraylength;i++)
	{
		if(i!=index)
			m_tablist.InsertItem(i,pDoc->m_formuar_choose.GetAt(i)->name,3);
		else
			m_tablist.InsertItem(&mydefitem);
	}

	m_editexplain=pDoc->m_formuar_choose.GetAt(index)->explainBrief;
	if(pDoc->m_formuar_choose.GetAt(index)->isProtected)
		m_tabedit="此条件选股公式被密码保护";
	else
		m_tabedit=pDoc->m_formuar_choose.GetAt(index)->help;
	UpdateData(FALSE);
	m_tablist.EnsureVisible(index,FALSE);
	m_tablist.SetFocus();
	

}


void CDialogLOGGSGL::UpdateMasterData(int index)
{

}


void CDialogLOGGSGL::UpdateKlineData(int index)
{
	CTaiShanDoc*   pDoc=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->m_taiShanDoc ;

	m_tablist.DeleteAllItems();
	
	int arraylength=pDoc->m_formuar_kline.GetSize();
	if(arraylength<=index||index<0)
	{
		m_editexplain="";
		m_tabedit="";
		UpdateData(FALSE);
		return;
	}
	LVITEM mydefitem;
	mydefitem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	mydefitem.iItem=index;
	mydefitem.iSubItem=0;
	mydefitem.iImage=5;
	CString name=pDoc->m_formuar_kline.GetAt(index)->name;
	mydefitem.pszText=name.GetBuffer(name.GetLength());
	mydefitem.state=LVIS_SELECTED|LVIS_FOCUSED; 

	
	
	for(int i=0;i<arraylength;i++)
	{
		if(i!=index)
			m_tablist.InsertItem(i,pDoc->m_formuar_kline.GetAt(i)->name,5);
		else
			m_tablist.InsertItem(&mydefitem);
	}
	m_editexplain=pDoc->m_formuar_kline.GetAt(index)->explainBrief;
	if(pDoc->m_formuar_kline.GetAt(index)->isProtected)
		m_tabedit="此K线组合公式被密码保护";
	else
	m_tabedit=pDoc->m_formuar_kline.GetAt(index)->help;
	UpdateData(FALSE);
	m_tablist.EnsureVisible(index,FALSE);
	m_tablist.SetFocus();	

}


 



void CDialogLOGGSGL::OnItemchangedTablist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	*pResult = 0;
}


void CDialogLOGGSGL::OnDeletegs() 
{
}




void CDialogLOGGSGL::OnDblclkTablist(NMHDR* pNMHDR, LRESULT* pResult) 
{

	
	*pResult = 0;
}

BOOL CDialogLOGGSGL::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	
	DoHtmlHelp(this);return TRUE;

}

void CDialogLOGGSGL::OnCheck1() 
{
	
	UpdateData();
	this->m_conProduct .EnableWindow(this->m_check);
	
}
