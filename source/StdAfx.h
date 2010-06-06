// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but

#if !defined(AFX_STDAFX_H__61A31CEB_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_STDAFX_H__61A31CEB_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#define PRO 03

#define _CRT_SECURE_NO_DEPRECATE

#define VC_EXTRALEAN	

#include <afxwin.h>      
#include <afxext.h> 
#include <afxdisp.h>     
#include <afxdtctl.h>	
#include <afxtempl.h>
#include <wininet.h>
#include <afxcmn.h>

#include "resource.h"
#include "stockdrv.h"
#include "StructTaiShares.h"
#include "CSharesInformation.h"
#include "CSharesCompute.h"
#include "StructKlineView.h"
#include "StkBlockManager.h"
#include "JiShuColumnInfo.h"
#include "CSharesBigBillData.h"
#include "CFormularCompute.h"
#include <afxdao.h>
#include "Funclist.h"
#include "HtmlHelp.h"

#include "ClientTransmitManageWnd.h"

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>		
#endif 

const char g_realtime[64] = "Data\\Report.tsk";
const char g_rate[64] = "Data\\stockrate";

const char g_daysh[64] = "Data\\sh\\day";
const char g_daysz[64] = "Data\\sz\\day";
const char g_minutesh[64] = "Data\\sh\\minute5";
const char g_minutesz[64] = "Data\\sz\\minute5";
const char g_buysellsh[64] = "Data\\sz\\BuyEach";
const char g_buysellsz[64] = "Data\\sh\\BuyEach";

const CString g_sF10sh = "Data\\sh\\f10\\";
const CString g_sF10sz = "Data\\sz\\f10\\";

#include "BtnST.h"
enum {	SHPAGE = 0, SZPAGE ,STKINDEXPAGE, CHOOSEPAGE,STKTYPEPAGE
,TJXGPAGE};
enum {	SZEBPAGE=102};//STKINDEXPAGE =100,STKTYPEPAGE=101,
//enum {	SHPAGE = 0, SZPAGE , STKINDEXPAGE ,	CHOOSEPAGE
//		,STKTYPEPAGE,TJXGPAGE};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#define _XTP_STATICLINK
#include <XTToolkitPro.h>



#endif // !defined(AFX_STDAFX_H__61A31CEB_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
