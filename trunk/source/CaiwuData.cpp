
#include "stdafx.h"
#include "CTaiShanApp.h"
#include "CaiwuData.h"

#include "KEYBRODEANGEL.h"
#include "mainfrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCaiwuData property page

IMPLEMENT_DYNCREATE(CCaiwuData, CPropertyPage)

CCaiwuData::CCaiwuData() : CPropertyPage(CCaiwuData::IDD)
{
	pDoc = CMainFrame::m_taiShanDoc;

	//{{AFX_DATA_INIT(CCaiwuData)
	m_ShowStatus = _T("");
	//}}AFX_DATA_INIT
}

CCaiwuData::~CCaiwuData()
{
}

void CCaiwuData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CCaiwuData)
	DDX_Control(pDX, IDC_ZDTQ, m_zdtq);
	DDX_Control(pDX, 1006, m_button6);
	DDX_Control(pDX, 1005, m_button5);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_LISTCW, m_listcw);
	DDX_Text(pDX, IDC_SHOWSTA, m_ShowStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCaiwuData, CPropertyPage)
	ON_WM_HELPINFO()
	//{{AFX_MSG_MAP(CCaiwuData)
	ON_EN_CHANGE(1008, OnChangeStockSymbol)
	ON_BN_CLICKED(1006, OnExport)
	ON_BN_CLICKED(1005, OnImport)
	ON_BN_CLICKED(IDC_ZDTQ, OnZdtq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CCaiwuData::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	LVCOLUMN listColumn;
	listColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	listColumn.fmt = LVCFMT_CENTER;

	CRect Rect;
	m_listcw.GetClientRect(Rect);
	int nWidth = (Rect.Width() - 16) / 2;

	CString arColumn[] = { "名称", "数值" };
	int nColumn;
	for (nColumn = 0; nColumn < 2; nColumn++)
		m_listcw.InsertColumn(nColumn, arColumn[nColumn], LVCFMT_CENTER, nWidth, nColumn);

	UpdateData(FALSE);

	return TRUE;  
}

BOOL CCaiwuData::PreTranslateMessage(MSG* pMsg) 
{
	if (GetFocus() == GetDlgItem(1008) && pMsg->message == WM_CHAR &&
		((pMsg->wParam >= '0' && pMsg->wParam <= '9') || (pMsg->wParam >= 'A' && pMsg->wParam <= 'z')))
	{
		CTaiKeyBoardAngelDlg KeyboardWizard;

		if (pMsg->wParam >= 'a' && pMsg->wParam <= 'z')
			pMsg->wParam -= 32;

		KeyboardWizard.input = pMsg->wParam;
		KeyboardWizard.ischar = (pMsg->wParam >= '0' && pMsg->wParam <= '9') ? false : true;
		KeyboardWizard.DoModal();

		if (KeyboardWizard.isresultstock)
		{
			CString szSymbol = KeyboardWizard.result;
			m_nKind = KeyboardWizard.m_stkKind;

			CReportData* pDat;
			if ((CMainFrame::m_taiShanDoc)->m_sharesInformation.Lookup(szSymbol.GetBuffer(0), pDat, m_nKind))
			{
				GetDlgItem(1008)->SetWindowText(szSymbol);
			}
			else
			{
				GetDlgItem(1008)->SetWindowText("");
			}
		}

		return TRUE;
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CCaiwuData::OnHelpInfo(HELPINFO* pHelpInfo)
{
	DoHtmlHelp(this);
	return TRUE;
}

void CCaiwuData::OnChangeStockSymbol() 
{
	CString strStockCode;
	GetDlgItem(1008)->GetWindowText(strStockCode);

	if (strStockCode.GetLength() == 4 || strStockCode.GetLength () ==6)
	{
		CReportData* Cdat = NULL;

		if (m_listcw.GetItemCount() > 0)
			m_listcw.DeleteAllItems();

		pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0), Cdat, m_nKind);
		if (Cdat == NULL)
		{
			GetDlgItem(IDC_STANAME)->SetWindowText("");
			return;
		}

		CString sName = Cdat->name;
		GetDlgItem(IDC_STANAME)->SetWindowText(sName);

		BASEINFO* pBase = Cdat->pBaseInfo;
		if (pBase == NULL)
			return;

		CString s;
		float* pf = &(pBase->zgb);
		s.LoadString(IDS_ZGB);

		for (int i = 0; i < 33; i++)
		{
			s.LoadString(IDS_ZGB + i);
			m_listcw.InsertItem(i, s);
			s.Format("%15.2f", (float)pf[i]);
			m_listcw.SetItemText(i, 1, s);
		}
	}
	else 
	{
		GetDlgItem(IDC_STANAME)->SetWindowText("");
		if (m_listcw.GetItemCount() > 0)
		{
			m_listcw.DeleteAllItems();
		}
	}
}

void CCaiwuData::OnExport() 
{
	CString DefExt = "*.cai";
	CString FileName = "CaiWu.cai";
	CString Filter = "数据格式(*.cai)|*.cai||";
	CFileDialog bSaveFileDialog(FALSE, (LPCSTR)DefExt, (LPCTSTR)FileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, (LPCSTR)Filter, this);

	CString FilePathName;
	if (bSaveFileDialog.DoModal() == IDOK)
	{
		FilePathName = bSaveFileDialog.GetPathName();
		OutCaiWuData(FilePathName);
	}
}

void CCaiwuData::OutCaiWuData(CString PathFileName)
{
	m_progress.ShowWindow(SW_HIDE);
	((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_SHOW);

	SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	UpdateData(TRUE);
	CFile OutFile;
	if (!OutFile.Open(PathFileName, CFile::modeCreate | CFile::modeWrite, NULL))
	{
		MessageBox("导出财务信息失败!", "警告", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	int nHeaderFlag = 65792509;
	char nVer[5] = "v2.0";
	int nTotalCount = 0;

	OutFile.Write(&nHeaderFlag, sizeof(int));
	OutFile.Write(nVer, 4);
	OutFile.Write(&nTotalCount, sizeof(int));

	OutFile.Seek(16, CFile::begin);

	m_ShowStatus = "正在导出...";
	UpdateData(FALSE);

	CReportData* pDat1 = NULL;
	for (int nStockType = 0; nStockType < 8; nStockType++)
	{
		for (int i = 0; i < pDoc->m_sharesInformation.GetStockTypeCount(nStockType); i++)
		{
			pDoc->m_sharesInformation.GetStockItem(nStockType, i, pDat1);
			if (pDat1 && pDat1->pBaseInfo)
			{
				OutFile.Write(pDat1->pBaseInfo, sizeof(BASEINFO) - sizeof(pDat1->pBaseInfo->m_Split) - 12);
				nTotalCount++;
			}
		}
	}		

	if (nTotalCount > 0)
	{
		OutFile.Seek(8, CFile::begin);
		OutFile.Write(&nTotalCount, sizeof(int));
		OutFile.Close();

		m_ShowStatus = "导出完毕!";
	}
	else
	{
		OutFile.Close();
		m_ShowStatus = "没有财务数据!";
		OutFile.Remove(PathFileName);
	}

	UpdateData(FALSE);
}

void CCaiwuData::OnImport() 
{
	CString DefExt = "*.cai,*.fin";
	CString FileName = "";
	CString Filter = "数据格式(*.cai) 分析家格式(*.fin)|*.cai;*.fin||";

	CString FilePathName;
	CFileDialog bOpenFileDialog(TRUE, (LPCSTR)DefExt, (LPCTSTR)FileName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, (LPCSTR)Filter, this);
	if (bOpenFileDialog.DoModal() == IDOK)
	{
		m_progress.ShowWindow(SW_HIDE);
		((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_SHOW);
		m_ShowStatus = "正在导入财务数据...";
		FilePathName = bOpenFileDialog.GetPathName();
		UpdateData(FALSE);

		BOOL IsSucc = ImportCwData(FilePathName);

		if (IsSucc)
		{
			m_ShowStatus = "导入完毕!";
		}
		else
		{
			m_ShowStatus = "导入失败!";  
		}

		UpdateData(FALSE);
	}

	UpdateData(FALSE);
}

BOOL CCaiwuData::ImportCwData(CString FileName)
{
	if (FileName.GetLength() < 5)
		return FALSE;

	CString sFxj = FileName;
	sFxj.MakeLower();
	sFxj = sFxj.Right(4);

	if (sFxj == ".fin")
	{
		CFile InFile;
		if (!(InFile.Open(FileName, CFile::modeRead, NULL)))
		{
			AfxMessageBox("引入财务文件不可读!");
			return FALSE;
		}

		int nFlag;
		InFile.Read(&nFlag, 4);
		int nTotal;
		InFile.Read(&nTotal, 4);
		InFile.Seek(8, CFile::begin);

		BASEINFO baseinfo;
		BASEINFO* pBase = &baseinfo;
		char ch[16];
		float fVal[50];
		while (1)
		{
			memset(&baseinfo, 0, sizeof(BASEINFO));

			if (InFile.Read(ch, 4) != 4)
				break;

			ch[2] = '\0';
			CString strSymbol = ch;

			WORD wMarket;
			memcpy(&wMarket, ch, sizeof(WORD));

			if (InFile.Read(ch, 8) != 8)
				break;

			ch[6] = '\0';
			strSymbol += ch;

			strcpy_s(baseinfo.Symbol, strSymbol);

			if (InFile.Read(ch, 2) != 2)
				break;

			float f2 = 0;
			if (InFile.Read(&f2, 4) != 4)
				break;

			if (InFile.Read(fVal, 148) != 148)
				break;

			pBase->zgb = fVal[0];
			pBase->gjg = fVal[1];
			pBase->fqrfrg = fVal[2];
			pBase->frg = fVal[3];
			pBase->Bg = fVal[4];

			pBase->Hg = fVal[5];
			pBase->ltAg = fVal[6];
			pBase->zgg = fVal[7];

			pBase->zzc = fVal[9] / 10;

			pBase->ldzc = fVal[10] /  10;
			pBase->gdzc = fVal[11] / 10;
			pBase->wxzc = fVal[12] / 10;
			pBase->cqtz = fVal[13] / 10;
			pBase->ldfz = fVal[14] / 10;

			pBase->cqfz = fVal[15] / 10;
			pBase->zbgjj = fVal[16] / 10;
			pBase->mggjj = fVal[17];
			pBase->ggqy = fVal[18] / 10;
			pBase->zyywsr = fVal[19] / 10;

			pBase->zyywlr = fVal[20] / 10;
			pBase->qtywlr = fVal[21] / 10;
			pBase->zyywlr = fVal[22] / 10;

			pBase->lrze = fVal[27] / 10;

			pBase->jlr = fVal[29] / 10;

			pBase->wfplr = fVal[30] / 10;
			pBase->mgwfplr = fVal[31];
			pBase->mgsy = fVal[32];
			pBase->mgjzc = fVal[33];

			pBase->gdqybl = fVal[35];
			pBase->jzcsyl = fVal[36];

			int nKind = 0;
			nKind = CMainFrame::m_taiShanDoc->m_sharesInformation.GetStockKind(wMarket, ch);

			if (nKind != SHZS && nKind != SZZS)
				CMainFrame::m_taiShanDoc->m_sharesInformation.ImportCaiwuInfo(&baseinfo);
		}

		InFile.Close();
		return TRUE;
	}

	CFile InFile;
	if (!(InFile.Open(FileName, CFile::modeRead, NULL)))
	{
		AfxMessageBox("引入财务文件不可读!");
		return FALSE;
	}

	int nFlag;
	InFile.Read(&nFlag, 4);
	if (nFlag != 65792509)
	{
		AfxMessageBox("不是" + g_strCompanyName + "财务数据文件!");
		InFile.Close();
		return FALSE;
	}

	char nVer[5];
	InFile.Read(nVer, 4);
	nVer[4] = '\0';
	if (strcmp(nVer, "v2.0") != 0)
	{
		AfxMessageBox("财务数据版本不符!");
		InFile.Close();
		return FALSE;
	}

	int nTotal;
	InFile.Read(&nTotal, 4);
	InFile.Seek(16, CFile::begin);

	BASEINFO baseinfo;
	for (int i = 0; i < nTotal; i++)
	{
		InFile.Read(&baseinfo, sizeof(BASEINFO) - sizeof(baseinfo.m_Split) - 12);
		CMainFrame::m_taiShanDoc->m_sharesInformation.ImportCaiwuInfo(&baseinfo);
	}

	InFile.Close();
	return TRUE;
}

void CCaiwuData::OnZdtq() 
{
	long num = pDoc->m_sharesInformation.GetCount();
	m_progress.ShowWindow(SW_SHOW);
	((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_HIDE);
	m_progress.SetRange32(0, num);
	for (int i = 0; i < 8; i++)
	{
		if (i == 0 || i == 3)
			continue;

		int temp = pDoc->m_sharesInformation.GetStockTypeCount(i);
		for (int j = 0; j < temp; j++)
		{
			CString str;
			CReportData* Cdat;
			pDoc->m_sharesInformation.GetStockItem(i, j, Cdat);
			if (Cdat == NULL)
				continue;

			pDoc->m_sharesInformation.ReadBaseInfoData(Cdat);
			m_progress.OffsetPos(1);
		}
	}

	m_progress.SetPos(num);
	m_progress.SetPos(0);

	m_progress.ShowWindow(SW_HIDE);
	((CComboBox*)GetDlgItem(IDC_SHOWSTA))->ShowWindow(SW_SHOW);
	m_ShowStatus = "财务数据提取完毕!";
	UpdateData(FALSE);
}
