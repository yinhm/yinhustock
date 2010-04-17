// MyTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "MyTreeView.h"
#include "CompanyInfoView.h"
#include "ChildFrm.h"
#include "resource.h"
#include "MyRichView.h"
#include "MainFrm.h"
#include "DiaGetUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct tagFindResult
{
 char ResultFiles[100];
}FindResult;
/////////////////////////////////////////////////////////////////////////////
// CTaiTestTreeView

IMPLEMENT_DYNCREATE(CTaiTestTreeView, CTreeView)

CTaiTestTreeView::CTaiTestTreeView()
{
//added by qyp 2001.10.25
#ifdef WIDE_NET_VERSION
	m_bRequestF10=FALSE;
	m_bRequestF9=FALSE;
#endif
}

CTaiTestTreeView::~CTaiTestTreeView()
{

	CTaiShanDoc* pDoc = ((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;

	((CMainFrame *)AfxGetMainWnd())->m_EnableF9 = true;
	((CMainFrame *)AfxGetMainWnd())->m_EnableF10 = true;
}


BEGIN_MESSAGE_MAP(CTaiTestTreeView, CTreeView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CTaiTestTreeView)
	ON_WM_CREATE()
	ON_COMMAND(ID_DELETE_FILE, OnDeleteFile)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_UPDATE_COMMAND_UI(ID_NEW_F10, OnUpdateNewF10)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_NEW_F10,OnGetUpdate)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_MESSAGE(MSG_MYTREEVIEW,OnMsgMyTreeView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiTestTreeView drawing

void CTaiTestTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

}

/////////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
void CTaiTestTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTaiTestTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////


void CTaiTestTreeView::OnInitialUpdate() //信息路径
{
	CTreeView::OnInitialUpdate();
}

int CTaiTestTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CTaiTestTreeView::ShowAll()
{
	// 12.2
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	CTaiShanDoc* pDoc = pFm->m_taiShanDoc ;
    CImageList img;
	img.Create(IDB_BITMAP2,16,0,RGB(192,192,192));
	this->GetTreeCtrl().SetImageList(&img,TVSIL_NORMAL);
	img.Detach();
  	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM hChapter = tree.InsertItem("公告信息");

	
		m_sArrayItemName.RemoveAll ();
		m_sArrayFilePath.RemoveAll ();
		m_sArrayFilePath.Add ("news\\cj");
		m_sArrayFilePath.Add ("news\\shenzhen");
		m_sArrayFilePath.Add ("news\\shanghai");
		m_sArrayItemName.Add ("财经信息");
		m_sArrayItemName.Add ("深圳信息");
		m_sArrayItemName.Add ("上海信息");
		GetDirectorys("news", m_sArrayFilePath);

		CString sOut;

		int nFPre = 0;
		CString filename;
		CFileFind finder;
		BOOL bWorking;

	
		for(int j = 3;j<m_sArrayFilePath.GetSize ();j++)
		{
			sOut = m_sArrayFilePath[j];
			sOut.MakeLower ();
			CString s;
			CString sName = "";
			int nLast = sOut.ReverseFind ('\\');
			if(nLast>=0 && sOut.GetLength ()-nLast>0)
				sName = sOut.Right (sOut.GetLength ()-nLast);
			if(sOut.Find ("news\\shanghai")>=0
				||sOut.Find ("news\\shenzhen")>=0
				||sOut.Find ("news\\cj")>=0 )
			{
				m_sArrayFilePath.RemoveAt (j);
				j--;
			}
		}

		int nFir = m_sArrayFilePath.GetSize ();
		if(nFir>100)
			nFir = 100;
		HTREEITEM hCaijing[100] ;
		for(int j = 0;j<nFir;j++)
		{
			sOut = m_sArrayFilePath[j];
			sOut.MakeLower ();
			CString s;
			CString sName = "";

			if(j>=3)
			{
				int nLast = sOut.ReverseFind ('\\');
				if(nLast>=0 && sOut.GetLength ()-nLast-1>0)
					sName = sOut.Right (sOut.GetLength ()-nLast-1);
				{
					s = "";
					CString s2 = sName;
					if(s2 == "")
						s2 .Format ("%d",j+1);
					s=s+s2;
					hCaijing[j] = tree.InsertItem(s,hChapter);
				}
				m_sArrayItemName.Add(s);
			}
			else
				hCaijing[j] = tree.InsertItem(m_sArrayItemName[j],hChapter);

			bWorking = finder.FindFile(sOut+"\\*.*");
			while(bWorking)
			{

			  bWorking = finder.FindNextFile();
			  if (finder.IsDots())
				 continue;

			  if (finder.IsDirectory())
				  continue;
			  filename = finder.GetFileName();
			  tree.InsertItem(filename,hCaijing[j],TVI_FIRST);
			}
		}
		tree.Expand(hChapter,TVE_EXPAND);

}



BOOL CTaiTestTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= (TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS);
	return CTreeView::PreCreateWindow(cs);
}


void CTaiTestTreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowFileTextNow();
/*	
*/
	*pResult = 0; 
} 

void CTaiTestTreeView::OnContextMenu(CWnd*, CPoint point)
{


}

void CTaiTestTreeView::OnDeleteFile() 
{

    CString filename;
	CString  name;
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM  hItem = tree.GetSelectedItem();
	name = tree.GetItemText(hItem);
	HTREEITEM  hParentItem = tree.GetParentItem(hItem);
	CString kind = tree.GetItemText(hParentItem);
	if(kind.Compare("公告信息") == 0 || name.Compare("公告信息") == 0)
	{
	    MessageBox("请选择所要删除的文件!");
		return;
	}
    
	filename = GetFileFullName(kind, name);

  
	
	CString message;
	message.Format("是否删除 %s ？",name);
	
	if(MessageBox(message,"删除信息文件",MB_YESNO|MB_ICONQUESTION) == 6)
	{
	 ::DeleteFile(filename);    
     tree.DeleteItem(hItem);
	}
    else
	{
	  return;
	} 
}


void CTaiTestTreeView::OnRButtonDown(UINT nFlags, CPoint point) 
{

		{
		if (point.x == -1 && point.y == -1){
		
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_MY_TREE_VIEW));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		CPoint posMouse;
		::GetCursorPos(&posMouse);

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, posMouse.x, posMouse.y,
			pWndPopupOwner);
	    ///////////0708////////////
	    menu.DestroyMenu();
	    ////////////////////////
	}


}


void CTaiTestTreeView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

	if(pTVKeyDown->wVKey == 13)
	{
		CString filename;
		CString  name;
		CTreeCtrl& tree = GetTreeCtrl();
		HTREEITEM  hItem = tree.GetSelectedItem();
		name = tree.GetItemText(hItem);
		HTREEITEM  hParentItem = tree.GetParentItem(hItem);
		CString kind = tree.GetItemText(hParentItem);
		CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
		CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;

		if(((CMainFrame *)AfxGetMainWnd())->m_F9ORF10)
		{
			if(name.Compare("公告信息") == 0 || kind.Compare("公告信息") == 0 )
			{
			  tree.Expand(hItem,TVE_TOGGLE);
			  return;
			}
		}
		ShowFileTextNow();
		*pResult = 0;
		return;
	}
	
	
	if(pTVKeyDown->wVKey == VK_TAB)
	{
	   CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	   CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	   *pResult = 0;
	   CTaiTestRichView *p_Info =(CTaiTestRichView *)(((CChildFrame *)pActiveWnd)->m_SplitterWnd)->GetPane(0,1);
	   ((CChildFrame *)pActiveWnd)->SetActiveView(p_Info,TRUE);
	   p_Info->SetFocus();
	  return;
	}
	if(pTVKeyDown->wVKey == VK_ESCAPE)
	{
	
	   GetParent()->GetParent()->SendMessage(WM_CLOSE);
	   CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	   CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	   CTaiShanDoc* pDoc=(CTaiShanDoc*)((CChildFrame *)pActiveWnd)->m_CurrentDoc;
	   *pResult = 0;
	   return;
	}
	ChangeView = false;
	*pResult = 0;
}


void CTaiTestTreeView::ShowAllF10()
{
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	CTaiShanDoc* pDoc = pFm->m_taiShanDoc ;
	CImageList img;
	img.Create(IDB_BITMAP2,16,0,RGB(192,192,192));
	this->GetTreeCtrl().SetImageList(&img,TVSIL_NORMAL);
	img.Detach();
    
	CString StockName_File;
	StockName_File.Format("%s.TXT",pDoc->m_sharesSymbol);

  	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM hChapter = tree.InsertItem("上市公司基本资料");
	HTREEITEM hShanghai = tree.InsertItem("沪市基本资料",hChapter);
	HTREEITEM hShenzhen = tree.InsertItem("深市基本资料",hChapter);
	
	HTREEITEM hCurrent = 0;
	CString filename;
	CFileFind finder;
    StockName_File.MakeUpper();
	BOOL bWorking = finder.FindFile(g_sF10sh+"*.txt");
	while(bWorking)
	{
	  bWorking = finder.FindNextFile();
	  filename = finder.GetFileName();
	  filename.MakeUpper();
	  if(filename == StockName_File)
	  {
		  hCurrent = tree.InsertItem(filename,hShanghai,TVI_FIRST);
		 
	  }
	  else
          tree.InsertItem(filename,hShanghai,TVI_FIRST);
	}
	bWorking = finder.FindFile(g_sF10sz+"*.txt");
	while(bWorking)
	{
	  bWorking = finder.FindNextFile();
	  filename = finder.GetFileName();
	  filename.MakeUpper();
	  if(filename == StockName_File)
	  {
		  hCurrent = tree.InsertItem(filename,hShenzhen,TVI_FIRST);
	
	  }
	  else
          tree.InsertItem(filename,hShenzhen,TVI_FIRST);
	}

	tree.SortChildren(hShanghai);
	tree.SortChildren(hShenzhen);
	tree.Expand(hChapter,TVE_EXPAND);
	if( hCurrent)
	{
	  tree.SelectItem(hCurrent);
	  tree.EnsureVisible(hCurrent);
	}
	this->SetFocus();


}

void CTaiTestTreeView::CheckFileTime(int iDays)
{
  CTreeCtrl& tree = GetTreeCtrl();
  HTREEITEM hRoot = tree.GetRootItem();
  tree.Expand(hRoot,TVE_COLLAPSE);
  tree.DeleteAllItems();
  HTREEITEM hChapter = tree.InsertItem("上市公司基本资料");
  HTREEITEM hShanghai = tree.InsertItem("沪市基本资料",hChapter);
  HTREEITEM hShenzhen = tree.InsertItem("深市基本资料",hChapter);
  
  CTime timenow;
  timenow = CTime::GetCurrentTime();
  CTimeSpan ts(iDays,0,0,0);
  timenow = timenow - ts;
  CString filename;
  CFileStatus status;
  CFileFind finder;
  FindResult fr[1000];
  int i = 0;
  BOOL bWorking = finder.FindFile(g_sF10sh+"*.txt"); 
  while(bWorking)
  {
	  bWorking = finder.FindNextFile();
	  if(bWorking)
	  {
		  filename = finder.GetFilePath();
		  if(CFile::GetStatus(filename,status))
		  {
			  if(status.m_mtime >= timenow)
			  {
				if(i>900)
					return;
				tree.InsertItem(finder.GetFileName(),hShanghai,TVI_FIRST);
				strcpy(fr[i].ResultFiles,(LPCTSTR)filename);
				i++;
			  }
		  }
	  }
  }
  finder.Close( );
  bWorking = finder.FindFile(g_sF10sz+"*.txt"); 
  while(bWorking)
  {
	  bWorking = finder.FindNextFile();
	  if(bWorking)
	  {
		  filename = finder.GetFilePath();
		  if(CFile::GetStatus(filename,status))
		  {
			  if(status.m_mtime >= timenow)
			  {
				if(i>900)
					return;
				tree.InsertItem(finder.GetFileName(),hShenzhen,TVI_FIRST);
				strcpy(fr[i].ResultFiles,(LPCTSTR)filename);
				i++;
			  }
		  }
	  }
  }
  finder.Close( );
  if(i>0)
  {
	  CFile UpdateFile;
	  UpdateFile.Open((LPCTSTR)"F10UpDate.txt",CFile::modeCreate|CFile::modeWrite,NULL);
	  CString F10Name;
	  for(int j=0;j<i;j++)
	  {
          F10Name=fr[j].ResultFiles;
          F10Name=F10Name.Mid(F10Name.ReverseFind('\\')+1,F10Name.ReverseFind('.')-F10Name.ReverseFind('\\')-1);
		  F10Name+="\r\n";
		  UpdateFile.Write(F10Name,F10Name.GetLength());
	  }
	  UpdateFile.Close();
  }
}

void CTaiTestTreeView::OnGetUpdate()
{
  CDiaGetUpdate dia;
  if(dia.DoModal() == IDOK)
  {
    CheckFileTime(dia.m_days+1);
  }
}

void CTaiTestTreeView::OnUpdateNewF10(CCmdUI* pCmdUI) 
{
  if(m_bF9ORF10)
	  pCmdUI->Enable(FALSE);
  else
	  pCmdUI->Enable();
}
void CTaiTestTreeView::CheckNewsInfomationFileTime()
{
  CMainFrame *pMain=(CMainFrame *)::AfxGetMainWnd() ;
  CTaiShanDoc *pDoc=pMain->m_taiShanDoc;
  int iDays=pDoc->m_systemdata.noticesave;
  CTime timenow;
  timenow = CTime::GetCurrentTime();
  CTimeSpan ts(iDays,0,0,0);
  timenow = timenow - ts;
  CString filename;
  CFileStatus status;
  CFileFind finder;

  int i = 0;
  BOOL bWorking = finder.FindFile("news/shanghai/*.dat"); 
  while(bWorking)
  {
	  bWorking = finder.FindNextFile();
      if(bWorking)
	  {
		  filename = finder.GetFilePath();
		  if(CFile::GetStatus(filename,status))
		  {
			  if(status.m_mtime < timenow)
			  {
				  DeleteFile(filename);	
			  }
		  }
	  }
  }
  
  bWorking = finder.FindFile("news/shenzhen/*.dat"); 
  while(bWorking)
  {
	  bWorking = finder.FindNextFile();
	  if(bWorking)
	  {
		  filename = finder.GetFilePath();
		  if(CFile::GetStatus(filename,status))
		  {
			  if(status.m_mtime < timenow)
			  {
				  DeleteFile(filename);	
			  }
		  }
	  }
  }
  bWorking = finder.FindFile("news/cj/*.dat"); 
  while(bWorking)
  {
	  bWorking = finder.FindNextFile();
	  if(bWorking)
	  {
		  filename = finder.GetFilePath();
		  if(CFile::GetStatus(filename,status))
		  {
			  if(status.m_mtime < timenow)
			  {
				  DeleteFile(filename);	
			  }
		  }
	  }
  }
}
void CTaiTestTreeView::FindStockBaseInfoPos(char *stockcode)
{
	char Filename[MAX_PATH];
	sprintf(Filename,"%s.TXT",stockcode);
    CTreeCtrl& tree = GetTreeCtrl();
    HTREEITEM hRoot;
	hRoot=tree.GetRootItem( );
	if (tree.ItemHasChildren(hRoot))
	{
		HTREEITEM hRootNextItem;
	    HTREEITEM hRootChildItem =tree.GetChildItem(hRoot);
	    while (hRootChildItem != NULL)   
		{
          if(tree.ItemHasChildren(hRootChildItem))
		  {
			  HTREEITEM hNextItem;
			  HTREEITEM hChildItem =tree.GetChildItem(hRootChildItem);
              while(hChildItem != NULL)
			  {
				  CString s = tree.GetItemText(hChildItem);  
				  if(s ==Filename)
				  {
					tree.SelectItem(hChildItem); 
					tree.EnsureVisible(hChildItem);
					return;
				  }
				  hNextItem=tree.GetNextItem(hChildItem, TVGN_NEXT);
		          hChildItem = hNextItem;   
			  }
		  }
		  hRootNextItem = tree.GetNextItem(hRootChildItem, TVGN_NEXT);
		  hRootChildItem = hRootNextItem;   
		}
	}
}
BOOL CTaiTestTreeView::OnHelpInfo(HELPINFO* pHelpInfo) 
{

	DoHtmlHelp(this);return TRUE;

}

void CTaiTestTreeView::GetDirectorys(CString sParen, CStringArray &sArrOut)
{

   CFileFind finder;

  
   CString strWildcard(sParen);
   strWildcard += _T("\\*.*");


   BOOL bWorking = finder.FindFile(strWildcard);

   while (bWorking)
   {
      bWorking = finder.FindNextFile();

      
      if (finder.IsDots())
         continue;

      if (finder.IsDirectory())
      {
         CString str = finder.GetFilePath();
		 sArrOut.Add (str);
      }
   }

   finder.Close();

}

CString CTaiTestTreeView::GetFileFullName(CString sDirect, CString sName)
{
	CString filename = "";
	for(int k=0;k<m_sArrayItemName.GetSize();k++)
	{
		if(m_sArrayItemName[k] == sDirect)
		{
			filename = m_sArrayFilePath[k]+"\\"+sName;
			return filename;
		}
	}
	return filename;


}

void CTaiTestTreeView::ShowFileText(CString filename,CTaiShanDoc* pDoc,CTaiTestRichView* pView)
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
    CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CFile file(filename,CFile::modeRead);
	CString sFileN = file.GetFileName ();
	sFileN.MakeLower ();
	if(sFileN.GetLength ()<5) return;

	int nLen = file.GetLength();
	if(nLen<0) nLen = 0;
	if(sFileN.GetLength ()>5)
	{
		if(sFileN.Right (5) == ".ljwj" )
		{
			char * pCh = new char[nLen+2];
			memset(pCh,0,sizeof(char)*(nLen+2));
			file.SeekToBegin ();
			file.Read(pCh,nLen );
			filename = pCh;
			delete [] pCh;
		}
	}
	if(!(sFileN.Right (4) == ".dat" ||sFileN.Right (4) == ".txt" ))
	{
		ShellExecute(NULL,"open",filename,NULL,NULL,SW_SHOWMAXIMIZED   );
		return;
	}

    char* broadcastmessage=new char[nLen+1];
	memset(broadcastmessage,0,sizeof(broadcastmessage));
	broadcastmessage[nLen ] = 0;
	
	if(sFileN.Right (4) == ".dat" ||sFileN.Right (4) == ".txt" )
	{
		int nLen2 = file.Read(broadcastmessage,nLen );
		for(int k = 0;k<nLen;k++)
			if(broadcastmessage[k] == 0)
				broadcastmessage[k] = ' ' ;
	
		CString s = broadcastmessage;
		bool bDel = false;


	}
	((CTaiTestRichView*)pView)->GetRichEditCtrl().ShowWindow(SW_HIDE);
	((CTaiTestRichView*)pView)->GetRichEditCtrl().SetReadOnly(TRUE);

	((CTaiTestRichView*)pView)->GetRichEditCtrl().SetSel(0,-1);
	((CTaiTestRichView*)pView)->GetRichEditCtrl().ReplaceSel(broadcastmessage);
	((CTaiTestRichView*)pView)->GetRichEditCtrl().SetSel(0,0);
    ((CTaiTestRichView*)pView)->GetRichEditCtrl().ShowWindow(SW_SHOW);
	((CTaiTestRichView*)pView)->GetRichEditCtrl().GetLimitText();

	pDoc->SetModifiedFlag(FALSE);
	((CWnd*)pView)->UpdateData(FALSE);
    ((CChildFrame *)pActiveWnd)->SetActiveView(pView,TRUE);
	pView->SetFocus();
	ChangeView = true;


}

void CTaiTestTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	ShowFileTextNow();
	*pResult = 0;
}

void CTaiTestTreeView::ShowFileTextNow()
{
	CString filename;
	CString  name;
	CTreeCtrl& tree = GetTreeCtrl();
	HTREEITEM  hItem = tree.GetSelectedItem();
	name = tree.GetItemText(hItem);
	HTREEITEM  hParentItem = tree.GetParentItem(hItem);
	CString kind = tree.GetItemText(hParentItem);
    CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
    CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;

	if(((CMainFrame *)AfxGetMainWnd())->m_F9ORF10)
	{
		if(name.Compare("公告信息") == 0 || kind.Compare("公告信息") == 0 )
		{

		  return;
		}
    
		filename = GetFileFullName(kind, name);
	}
	else
	{
	   	if(kind.Compare("上市公司基本资料") == 0 || name.Compare("上市公司基本资料") == 0)
		{
		  tree.Expand(hItem,TVE_TOGGLE);
		  return;
		}

	    if(kind.Compare("沪市基本资料") == 0)
		{
		  filename.Format("%s%s",g_sF10sh,name);
		}
		
		if(kind.Compare("深市基本资料") == 0)
		{
		  filename.Format("%s%s",g_sF10sz,name);
		}
	}
	CView* pView;
	POSITION pos = pDoc->GetFirstViewPosition();   
	while (pos != NULL)   
	{
      pView = pDoc->GetNextView(pos);
      if(pView->IsKindOf(RUNTIME_CLASS(CTaiTestRichView)))
	  {
            if(this->m_bF9ORF10 == ((CTaiTestRichView *)(pView))->m_bF9ORF10)
		    break;
	  }
	}   
//added by qyp 2001.10.25	
#ifdef WIDE_NET_VERSION
	if(CMainFrame::m_bLogonSuccess)
	{	
		CFile file;
		file.Open("data/loading.txt",CFile::modeCreate|CFile::modeWrite);
		CString buf;
		buf=_T("正在从服务器请求数据,请等待...");
		
		file.Write(buf.GetBuffer(0),buf.GetLength()+1);
		file.Close();
		ShowFileText("data/loading.txt", pDoc,(CTaiTestRichView* )pView);
		
		DeleteFile("data/loading.txt");

		m_filename=filename;
		m_pView=pView;
		
		if(!(((CMainFrame *)AfxGetMainWnd())->m_F9ORF10))
		{
			
			TRANS_PACKAGE_STRUCTEx trans;
			TransmitStockAttribute attr[1];
			trans.m_TransmitType = WideStockBaseInfo;
			trans.m_dwTransmitStockType=-1;
			strcpy(attr[0].m_szTransmitStockLabel ,pDoc->m_sharesSymbol);
			attr[0].m_wMarket=pDoc->m_stkKind;
			attr[0].m_dwTransmitCounts = 1;
			trans.m_pTransmitStockAttribute = attr;
			
			CMainFrame::m_pClientTransmitManageWnd->GetMarketData(&trans,0,socket_id);
			m_bRequestF10=TRUE;
		}
		else
		{
					
			m_bRequestF9=TRUE;
		}
	}
	else
	{
		if(MessageBox("没有登录到服务器,是否装载旧的资料?","装载提示",MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			ShowFileText( filename, pDoc,(CTaiTestRichView* )pView);
		}
		else
		{
			CFile file;
			file.Open("data/loading.txt",CFile::modeCreate|CFile::modeWrite);
			CString buf;
			buf=_T("没有登录到服务器,无法得到最新的资料.");
			
			file.Write(buf.GetBuffer(0),buf.GetLength()+1);
			file.Close();
			ShowFileText("data/loading.txt", pDoc,(CTaiTestRichView* )pView);
			
			DeleteFile("data/loading.txt");
		}
	}
#else
	ShowFileText( filename, pDoc,(CTaiTestRichView* )pView);
#endif


}

void CTaiTestTreeView::ClearAll()
{
		CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
		CTaiShanDoc* pDoc = pFm->m_taiShanDoc ;

		CStringArray arrPaths;
		GetDirectorys("news", arrPaths);

		CString sOut;
		for(int i=0;i<arrPaths.GetSize ();i++)
		{
			sOut = arrPaths[i];
			sOut.MakeLower ();
			CString s;
			CString sName = "";
			int nLast = sOut.ReverseFind ('\\');
			if(nLast>=0 && sOut.GetLength ()-nLast>0)
				sName = sOut.Right (sOut.GetLength ()-nLast);
			if(sOut.Find ("news\\shanghai")>=0
				||sOut.Find ("news\\shenzhen")>=0
				||sOut.Find ("news\\cj")>=0 
				||sOut.Find ("news\\网上证券咨询")>=0)
			{
				continue;
			}

			CFileFind finder;
			BOOL bWorking = finder.FindFile(sOut+"\\*.*");
			while(bWorking)
			{

			  bWorking = finder.FindNextFile();
			  if (finder.IsDots())
				 continue;

			  if (finder.IsDirectory())
				  continue;
			  CString filename = finder.GetFilePath ();
			  CFile::Remove (filename);
			}
			;
		}

}

LRESULT CTaiTestTreeView::OnMsgMyTreeView(WPARAM wParam,LPARAM lParam)
{
#ifdef WIDE_NET_VERSION
	if(m_bRequestF10)
	{
		CTaiShanDoc* pDoc=((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;
		ShowFileText(m_filename, pDoc,(CTaiTestRichView* )m_pView);
		m_bRequestF10=FALSE;
	}
	if(m_bRequestF9)
	{
		CTaiShanDoc* pDoc=((CMainFrame *)AfxGetMainWnd())->m_taiShanDoc;
		ShowFileText(m_filename, pDoc,(CTaiTestRichView* )m_pView);
		m_bRequestF9=FALSE;
	}
#endif
	return 0;
}