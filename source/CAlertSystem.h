#if !defined(AFX_ALERTSYSTEM_H__F218F663_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTSYSTEM_H__F218F663_69E0_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CTaiScreenParent.h"

class CMainFrame;
class CTaiShanDoc ;
typedef CList<ALERT_SET_ITEM*,ALERT_SET_ITEM*&> CListAlarmSet;
struct DATA_PER_LINE
{
	INFO_LINESELF lineInfo;
	int m_gapFromBeginArr;
	BYTE m_btAlarmed;
	BYTE m_bUpwards;
};
struct ALERT_LINE_ITEM
{
	SYMBOL_ISALERTED m_symbolAlarm;
	CArray<DATA_PER_LINE,DATA_PER_LINE&>  m_lineInfoArr;

};
typedef CList<ALERT_LINE_ITEM*,ALERT_LINE_ITEM*&> CListAlarmLineSet;

class CAlertSystem : public CDialog
{

public:
	void ClearAllResult();
	static void SwitchAlarm();
	static void GetScreenStockBlockSymbol(SymbolKindArr & sArr,CString sBlockName,int k=-1);

	static void GetLineAlarmInitData(ALERT_LINE_ITEM* item);
	static void DeleteLineAlarmItem(INFO_LINESELF &lineInfo,CString s);
	static void AddLineAlarmItem(INFO_LINESELF& lineInfo,CString s,bool bUpwards);
	static void AlarmSendMessage(CMainFrame* pFrame,CReportData* dt1 ,ALERT_SET_ITEM* item=NULL,int nCondFoot=-1);
	static void CaclLineAlarm(CMainFrame *pFrame,int nLineAlarm);
	static void CreateNew();
	static void StartAlarm(bool bFirst = true);
	static void ListCtrlToSymbolType(CArrayStockType &typeArr,CListCtrl* pList);
	static void InsertRow(CListCtrl* pList,CString *strPt,int nStr,int nImag,int nPos=-1,int stkKind = 0);
	static void GetSymbolArray(CArrayStockType& typeArr,SymbolKindArr & symbolArr);
	static void CaclAlarm(CMainFrame* pFrame);
	static bool SaveFile();
	static bool LoadFile(bool bLine=false);

	CAlertSystem(CWnd* pParent = NULL);  

	CTaiShanDoc * pDoc;
	bool m_bStartAlarm;
	static CListAlarmSet m_gAlarmArray;
	static CListAlarmLineSet m_gLineAlarmArray;
	static CList<ALERT_RESULT_ITEM*,ALERT_RESULT_ITEM*&> m_gAlarmResultList;
	static CAlertSystem* m_gDlgAlarmSystem;

	//{{AFX_DATA(CAlertSystem)
	enum { IDD = IDD_MYALERT_SYSTEM };
	CButtonST	m_button7;
	CButtonST	m_button6;
	CButtonST	m_button4;
	CButtonST	m_button3;
	CListCtrl	m_alarmResultList;
	CListCtrl	m_condArrayList;
	CButtonST m_btOK;
	CButtonST m_btCancel;
	CButtonST m_btAdd;
	CButtonST m_btDelete;
	//}}AFX_DATA



	//{{AFX_VIRTUAL(CAlertSystem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


protected:


	//{{AFX_MSG(CAlertSystem)
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnModify();
	afx_msg void OnClear();
	afx_msg void OnClearAll();
	afx_msg void OnWriteToFile();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void AlarmArrInsertRow(ALERT_SET_ITEM* item ,int nItem = -1);
	static void ResultListAddRow(CListCtrl* pList,int nRow);
	void FillListctrlHeader();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTSYSTEM_H__F218F663_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
