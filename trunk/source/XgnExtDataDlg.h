#if !defined(AFX_XGNEXTDATADLG_H__89285CED_7FAA_11D6_968D_E35CA698EE75__INCLUDED_)
#define AFX_XGNEXTDATADLG_H__89285CED_7FAA_11D6_968D_E35CA698EE75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XgnExtDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XgnExtDataDlg dialog
#include "indicatorextendData.h"
#include "FileExtendData.h"
#include "FileInOutData.h"
#include "FtpGetSend.h"
#include "FtpParaClass.h"
#include "HttpGetData.h"
#include "CTaiScreenParent.h"	
class CProgressDialog;

class XgnExtDataDlg : public CDialog
{

public:
	static bool IsValidData(int nID);
	static void SetProductIdToExtdata(CString sFileName);
	static DWORD GetTimegap();
	static int ReadDownLoadInfoFile(CString sFileName , int *& pData,CString & sDate,int& nTimesNow);
	static int Transfer5MinTimet(int tmIn);
	static bool ReadWriteDownLoadInfoFile(int nTimesNow,bool bWrite);
	static bool RemoveFiles(CString &sPath);
	static CString GetCurrentPath();
	static void ComputeOnce();
	static bool GetCountValid(int * pOut);
	static void RefreshDataAuto(IndicatorInfo &infoData, int nCountValid,int nTimeNow,bool bCloseWork=false,CProgressDialog* pDlg=NULL);
	static CString FormatStr(CReportData * pdt);
	bool IsValidID(int nID);
	int GetAValidID();
	XgnExtDataDlg(CWnd* pParent = NULL);   // standard constructor
	static bool GetLineBE(Kline* pKline,int nKline,ARRAY_BE & lineOut,int nID,CReportData* pdt) ;
	static void DoCloseWork(CProgressDialog* pDlg);

	static indicatorInfoArr m_infoArr;
	static int m_nValidCount[256];
	static DWORD	m_dwTimer;
// Dialog Data
	//{{AFX_DATA(XgnExtDataDlg)
	enum { IDD = IDD_XGN_EXTDATA_PROP };
	CButtonST	m_ok;
	CButtonST	m_cancel;
	CButtonST	m_modify;
	CButtonST	m_refresh2;
	CButtonST	m_refresh;
	CButtonST	m_delete;
	CButtonST	m_add;
	CProgressCtrl	m_Progress;
	CListCtrl	m_listData;
	UINT	m_nDaysLast;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XgnExtDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static CString GetTodayPath(int nID,bool bOnlyPath = false);

	// Generated message map functions
	//{{AFX_MSG(XgnExtDataDlg)
	afx_msg void OnButtonFresh();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonDelete();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonAdd();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFreshAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static IndexDataInfo ChangeInfo(IndicatorInfo & infoData);
	void RefreshData(IndicatorInfo &infoData,bool bClear,bool bTime = false);
	void InsertListInfo();
	static int GetSerInt() ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XGNEXTDATADLG_H__89285CED_7FAA_11D6_968D_E35CA698EE75__INCLUDED_)
