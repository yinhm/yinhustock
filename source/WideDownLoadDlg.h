#if !defined(AFX_WIDEDOWNLOADDLG_H__39DA3222_EC25_4E96_92B4_F868D55F49A0__INCLUDED_)
#define AFX_WIDEDOWNLOADDLG_H__39DA3222_EC25_4E96_92B4_F868D55F49A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WideDownLoadDlg.h : header file
//
#include "ClientTransmitManageWnd.h"

typedef enum WideDownLoadAttributes{ 
	WideTodayDayLine,              //当日日线
	WideTodayAllDayLine,           //当日全部
    WideHistoryDayLine,            //历史日线  
    WideFiveMinuteLine,            //五分钟K线数据 
    WideAllFiveMinuteLine,         //全部五分钟K线数据 
	WideHistoryMinuteLine,         //五分钟历史K线数据 
	WideTodayDetail,               //当天明细数据
	WideAllTodayDetail,            //当天全部明细数据
	WideHistoryDetail,             //历史明细数据
	WideSingleStockBaseInfomation, //个股资料
	WideStockBaseInfomation,       //全部资料
	WideSingleExRight,             //除权数据
	WideExRight,                   //全部除权数据
	WideSingleCaiwuData,           //财务数据 
	WideCaiwuData                  //全部财务数据 
};
typedef struct {
	WideDownLoadAttributes m_DownLoadAttributes;     //下载数据类型
	char m_strDownLoadName[MAX_PATH];                //下载证券名称
	int m_dwDownLoadType;                            //下载数据类型 
	char m_strDownLoadDate[MAX_PATH];                //下载数据时间选择
    DWORD m_dwDownLoadTotalCounts;                   //下载总数
	DWORD m_dwDownLoadCompleteCounts;                //已下载数
    time_t *m_tDownLoadTimes;                        //下载明细数据时间
	DWORD m_dwTimeTotalCounts;                       //下载明细数据总天数
    DWORD m_dwTimeCompleteCounts;                    //已下载明细数据天数
} WIDEREQUEST_CONTEXT,*PWIDEREQUEST_CONTEXT;

/////////////////////////////////////////////////////////////////////////////
// CDownLoadNetDlg dialog
class CTaiKlineWideNet;
class CMainFrame;
class CDownLoadNetDlg : public CDialog
{
// Construction
public:
	CDownLoadNetDlg(CWnd* pParent = NULL);   // standard constructor
    CTaiShanDoc *m_pDoc;
	char m_szCurrentDir[MAX_PATH];
	CImageList * m_pImageList;    //大图象列表
	CImageList * m_pImageListSmall; //小图象列表

    CMainFrame *m_pMainFrm;
	TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
	TransmitStockAttribute m_TransmitStockAttribute;
	SOCKET_ID m_SocketID;
	BOOL m_bDownLoadState;
	CTaiKlineWideNet *m_pTechWideNet;


public:
    void FillToListCtrlHead();    //初始化CListCtrl列名
    void FillToTreeCtrlHead();    //初始化CTreeCtrl标题列
    BOOL InsertDownLoadDataToList(WIDEREQUEST_CONTEXT *pRequestContext );
    void AddDownLoadData(); 
    BOOL CheckList(WideDownLoadAttributes DownLoadAttributes,int dwDownLoadType,
								 char *strDownLoadName=NULL);
    int GetStockMarketCounts(int DownLoadType);
    void GetTimeString(time_t t1,time_t t2,char *strTime);
    void GetStartAndEndTime(time_t &t1,time_t &t2,char *strTime);
    void GetHistoryDetailTimeCounts(time_t t1,time_t t2,PWIDEREQUEST_CONTEXT &pRequestContext);
    void AddDownLoadDataInfo(BOOL bIsSendMsg);
    CReportData *GetCdatPointFromStockTypeAndPos(char strDownLoadName[MAX_PATH],int nStockType,int Pos);
    void DispalyDownLoadInfomation();


// Dialog Data
	//{{AFX_DATA(CDownLoadNetDlg)
	enum { IDD = IDD_WIDEDOWNLOAD };
	CTreeCtrl	m_MyTree;
	CListCtrl	m_MyList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownLoadNetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual void OnCancel();
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownLoadNetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnLoadCollecte();
	afx_msg void OnSaveCollecte();
	afx_msg void OnAddLoaddata();
	afx_msg void OnDeleteLoaddata();
	afx_msg void OnDownloadStart();
	afx_msg void OnStopdownload();
	afx_msg void OnSetupTime();
	afx_msg void OnDblclkSelectLoaddata(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    afx_msg LRESULT OnPostFocus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddStock(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRecvTransmitInfoMation(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WIDEDOWNLOADDLG_H__39DA3222_EC25_4E96_92B4_F868D55F49A0__INCLUDED_)
