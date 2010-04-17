// InfoBar.cpp : implementation file
// Tel:13366898744

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "InfoBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoBar dialog


CInfoBar::CInfoBar(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoBar)
		
	//}}AFX_DATA_INIT
}


void CInfoBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar)
	DDX_Control(pDX, IDC_TREE_INFO, m_treeInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoBar, CDialog)
	//{{AFX_MSG_MAP(CInfoBar)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_INFO, OnSelchangedTreeInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


int CInfoBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	
	return 0;
}

BOOL CInfoBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	CImageList imgList; 
	imgList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);

	CBitmap bmp1, bmp2, bmp3;
	bmp1.LoadBitmap(IDB_TREEBMP1);
	bmp2.LoadBitmap(IDB_TREEBMP2);
	bmp3.LoadBitmap(IDB_TREEBMP3);
	imgList.Add(&bmp1, RGB(192, 192, 192));
	imgList.Add(&bmp2, RGB(192, 192, 192));
	imgList.Add(&bmp3, RGB(192, 192, 192));
	m_treeInfo.SetImageList(&imgList, TVSIL_NORMAL);
	
////////////////////////////////////////////////////////
	TV_INSERTSTRUCT TreeCtrlItem;
	HTREEITEM hTreeItem;
	
	TreeCtrlItem.hInsertAfter=TVI_LAST;
	CString FirstTitle;
////////////////////////////////////////////////////////

	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="资讯快递";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "信息备忘");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "公告信息");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "新股上市");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "统计资料");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "财经要闻");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////

	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="新闻综述";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "公司新闻");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "行业动态");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "市场消息");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////

	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="机构论市";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "银河证券");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "国泰君安");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "山东神光");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "申银万国");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "万国测评");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////

	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="市场分析";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "名家论坛");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "大盘分析");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "个股分析");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "热点评述");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "黑马追踪");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////

	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="统计报告";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);

	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "信息备忘");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////

	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="投资必备";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "政策法规");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "投资技巧");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "教研室");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	
	return TRUE;  
}

void CInfoBar::OnSelchangedTreeInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString tempString;
	tempString=m_treeInfo.GetItemText(m_treeInfo.GetSelectedItem());

	CMainFrame* pMainFrm = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

	if(tempString.Compare("信息备忘")==0){pMainFrm->PostMessage(WM_USERDEFINED_INFOMEMORY,   0, 0);	}
	if(tempString.Compare("公告信息")==0){pMainFrm->PostMessage(WM_USERDEFINED_PUBLICINFO,   0, 0);	}
	if(tempString.Compare("新股上市")==0){pMainFrm->PostMessage(WM_USERDEFINED_NEWSTOCK,     0, 0);	}
	if(tempString.Compare("统计资料")==0){pMainFrm->PostMessage(WM_USERDEFINED_STATISTICS,   0, 0);	}
	if(tempString.Compare("财经要闻")==0){pMainFrm->PostMessage(WM_USERDEFINED_FINANCENEWS,  0, 0);	}
	if(tempString.Compare("公司新闻")==0){pMainFrm->PostMessage(WM_USERDEFINED_COMPANYNEWS,  0, 0);	}
	if(tempString.Compare("行业动态")==0){pMainFrm->PostMessage(WM_USERDEFINED_TRADEDYNAMIC, 0, 0);	}
	if(tempString.Compare("市场消息")==0){pMainFrm->PostMessage(WM_USERDEFINED_MARKETNEWS,   0, 0);	}
	if(tempString.Compare("银河证券")==0){pMainFrm->PostMessage(WM_USERDEFINED_YINHESTOCK,   0, 0);	}
	if(tempString.Compare("国泰君安")==0){pMainFrm->PostMessage(WM_USERDEFINED_GUOTAIJUNAN,  0, 0);	}
	if(tempString.Compare("山东神光")==0){pMainFrm->PostMessage(WM_USERDEFINED_SHENGUANG,    0, 0);	}
	if(tempString.Compare("申银万国")==0){pMainFrm->PostMessage(WM_USERDEFINED_SHENYINWANGUO,0, 0);	}
	if(tempString.Compare("万国测评")==0){pMainFrm->PostMessage(WM_USERDEFINED_WANGUOCEPING, 0, 0);	}
	if(tempString.Compare("名家论坛")==0){pMainFrm->PostMessage(WM_USERDEFINED_MINGJIALUNTAN,0, 0);	}
	if(tempString.Compare("大盘分析")==0){pMainFrm->PostMessage(WM_USERDEFINED_DAPANFENXI,   0, 0);	}
	if(tempString.Compare("个股分析")==0){pMainFrm->PostMessage(WM_USERDEFINED_GEGUFENXI,    0, 0);	}
	if(tempString.Compare("热点评述")==0){pMainFrm->PostMessage(WM_USERDEFINED_HOTPOINTCOMMT,0, 0);	}
	if(tempString.Compare("黑马追踪")==0){pMainFrm->PostMessage(WM_USERDEFINED_BLACKHORSE,   0, 0);	}
	if(tempString.Compare("政策法规")==0){pMainFrm->PostMessage(WM_USERDEFINED_POLICY,       0, 0);	}
	if(tempString.Compare("投资技巧")==0){pMainFrm->PostMessage(WM_USERDEFINED_INVESTTECH,   0, 0);	}
	if(tempString.Compare("教研室")  ==0){pMainFrm->PostMessage(WM_USERDEFINED_STAFFROOM,    0, 0);	}
	
	*pResult = 0;
}








































