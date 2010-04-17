// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiShanApp.h"
#include "MyEdit.h"
#include "CFormularContent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiTestEdit

CTaiTestEdit::CTaiTestEdit()
{
	m_bCreated = false;
	m_jiShu = NULL;	
	m_nPaPre = -1;

}

CTaiTestEdit::~CTaiTestEdit()
{
}


BEGIN_MESSAGE_MAP(CTaiTestEdit, CAutoRichEditCtrl)
	//{{AFX_MSG_MAP(CTaiTestEdit)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
	ON_WM_KILLFOCUS()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiTestEdit message handlers

void CTaiTestEdit::OnMouseMove(UINT nFlags, CPoint point) 
{

	if(m_bCreated == false ||m_editDlg.m_hWnd == NULL)
	{
		m_editDlg.Create(CFxjSetParamDlg::IDD,this);
		m_bCreated = true;
	}
	int nPa = MoveOnWhichParam(point);

	CAutoRichEditCtrl::OnMouseMove(nFlags, point);
}

void CTaiTestEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	int nPa = MoveOnWhichParam(point);
	CAutoRichEditCtrl::OnLButtonDown(nFlags, point);
}

int CTaiTestEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAutoRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;



	return 0;
}

void CTaiTestEdit::OnChange() 
{
	
	
}

HBRUSH CTaiTestEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	
	return NULL;
}

void CTaiTestEdit::OnSetfocus() 
{

	
}

BOOL CTaiTestEdit::PreTranslateMessage(MSG* pMsg) 
{

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
		CPoint p(-1,-1);
		ClientToScreen(&p);
		::SetCursorPos (p.x,p.y);
		int nPa = MoveOnWhichParam(p );
		return TRUE;
		}
	}
	return CAutoRichEditCtrl::PreTranslateMessage(pMsg);
}

BOOL CTaiTestEdit::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{

	BOOL bb = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	return bb;
}

void CTaiTestEdit::OnKillFocus(CWnd* pNewWnd) 
{

	CAutoRichEditCtrl::OnKillFocus(pNewWnd);
	

	
}

void CTaiTestEdit::SetLJishu(CFormularContent *pJis)
{
	if(m_bCreated == true)
		if(m_editDlg.IsWindowVisible ()) 
		{
			m_editDlg.ShowWindow (SW_HIDE);
			this->m_nPaPre = -1;
		}

	m_jiShu = pJis;
	if(m_jiShu) ShowWindow(SW_SHOW);
	else ShowWindow(SW_HIDE);

	if(!m_jiShu) return;


	CString strText = "";//
	
	for(int k=0;k<m_jiShu->numPara ;k++)
	{
		CString ss;
		ss.Format ("  param#%d\r\n",k+1);
		ss = m_jiShu->namePara[k] + ss;
		strText+= ss;
	}
	if(pJis->explainParam!="") strText = pJis->explainParam;
	CRect r ;
	this->GetClientRect (r);

	SetSel (0,-1);
	this->ReplaceSel ("");



	
	int nStart=0;
	int nTotal=strText.GetLength();
	int nCount= 0;
	CString strTemp = "";

	strText.MakeLower ();
	int left = 0;
	COLORREF clrText = RGB(0,0,255);
	COLORREF clrParam =RGB(180,20,0) ;

	int k=0;
	for(k = 1;k<=8;k++)
	{
		this->m_nParamInfo [k-1].m_nParamB = -1;
		CString ss;
		ss.Format ("param#%d",k);
		int nFind = strText.Find (ss,0);
		if(nFind>=0)
		{
			strTemp = strText.Left (nFind);
			strText.Delete(0,nFind);
			this->m_nParamInfo [k-1].m_nParamB = nFind+nStart;
			this->m_nParamInfo [k-1].fVal = CFormularContent::GetParamDataEach(k-1,5, m_jiShu);
		}
		else
		{
			if(k ==1)
				strTemp =  strText;
			else
				strTemp = "";
		}

	
		SetSel(nStart,nStart);
		this->SetSelectColor (clrText);	
		ReplaceSel(strTemp);
		nStart+=strTemp.GetLength ();
		if(nFind>=0)
		{
			
			float ff=CFormularContent::GetParamDataEach(k-1,5, m_jiShu);

			//20030109
			strTemp =  "   "+CLongString::FloatToString(ff);
			strTemp+="   ";
			if(k-1>=m_jiShu->numPara ) strTemp = " 无此参数 ";
			SetSel(nStart,nStart);
			this->SetSelectColor (clrParam);	
			ReplaceSel(strTemp);
			nStart+=strTemp.GetLength ();
			strText.Delete(0,7);
			this->m_nParamInfo [k-1].m_nParamCount = strTemp.GetLength ();
		}

	}


}

int CTaiTestEdit::MoveOnWhichParam(CPoint &point)
{
	CRect r(0,0,0,0);

	int nPa = -1;
	CHARFORMAT cf;
	this->GetDefaultCharFormat (cf);
	int nHeight = cf.yHeight+cf.yOffset;

	CDC* pDC = GetDC();
	CFont* pOldF = pDC->SelectObject (&m_Font);

	int nOld = pDC->SetMapMode(MM_TWIPS  ); 
	POINT p;
	p.x = 100;
	p.y = nHeight;

	pDC->SetMapMode(nOld  ); 

	CHARFORMAT fmt = GetCharFormat(CFM_SIZE);
	float f2 = GetDeviceCaps( pDC->m_hDC ,  LOGPIXELSX)/1440.0f*fabs((float)fmt.yHeight)+2 ;
	int nLine = point.y /f2;

	nLine = -1;

	int nLine2 = GetLineCount();
	int j = 0;
	int nNow = 0;
	for(;j<nLine2;j++)
	{
		CPoint pt;
		GetPosFormChar(pt, nNow);
		char ch[2048];
		int j2= GetLine( j,ch ) ;
		if(j2<0)
			continue;
		nNow+=j2;
		if(pt.y==point.y && pt.y==0)
		{
			nLine = j;
			break;
		}
		if(pt.y>point.y)
		{
			nLine = j-1;
			if(nLine<0) nLine = 0;
			break;
		}
	}
	if(	nLine == -1)
	{
		nLine = nLine2-1;
	}



	r.top = nLine*fabs(f2)-1;

	CRect r2;
	m_editDlg.GetWindowRect(r2);


	int nx = 0;
	j = 0;
	CString s = "";
	int nRtn = -1;
	if(nLine<=nLine2)
	{
		char ch[1024];
		j= GetLine( nLine,ch ) ;
		if(j>0)
			ch[j] = 0;
		s = ch;

		int i = 0;

		nRtn = GetCharFormPos(point);
		if(nRtn>=0)
		{
			int nTot = 0;
			int j = 0;
			char ch[1024];
			CString sTmp;
			for( i = 0;i<nLine;i++)
			{
				j= GetLine( i,ch ) ;
				if(j>0)
					ch[j] = 0;
				sTmp = ch;
				nTot+=j;
			}

			int nTotOld = nTot;



			TRACE("nRtn = %d\n",nRtn);

			j= GetLine(nLine,ch ) ;
			if(j>0)
				ch[j] = 0;
			CString ss2 = ch;
			if(this->m_jiShu )
			{
				for( i= 0;i<m_jiShu->numPara ;i++)
				{
					if(m_nParamInfo [i].m_nParamB == -1)
						continue;
					if(nRtn>=this->m_nParamInfo [i].m_nParamB && nRtn<=this->m_nParamInfo [i].m_nParamB+this->m_nParamInfo [i].m_nParamCount)
					{
					
						nPa = i;
						int n2 = m_nParamInfo [i].m_nParamB-nTotOld;
						if(n2<0) 
						{
							n2 = 0;

						}
						CString ss3 = ss2.Left (n2);
						r.left = pDC->GetTextExtent(ss3).cx;
						ss3 = "";
						if(n2+m_nParamInfo [i].m_nParamCount<=ss2.GetLength ()) 
						{
							ss3 = ss2.Mid (n2,m_nParamInfo [i].m_nParamCount);
						}
						else
						{
							pDC->SelectObject (pOldF);
							this->ReleaseDC (pDC);

							CString s33 = "n2+m_nParamInfo  = ";
							CString s44;
							s44.Format ("%d,   ",n2+m_nParamInfo [i].m_nParamCount);
							s33+=s44;
							s33+=ss2;
							s44.Format ("=%d,   ",ss2.GetLength ());
							s33+=s44;

							return false;
						}

						r.right = r.left +5+ pDC->GetTextExtent(ss3).cx;
					}
				}
			}
		}
	}
	pDC->SelectObject (pOldF);
	this->ReleaseDC (pDC);

	if(nPa<0 || nPa>=0 && m_nPaPre!=nPa && m_nPaPre >=0 )//
	{
		if(m_editDlg.IsWindowVisible ()) 
		{
			m_editDlg.ShowWindow (SW_HIDE);
			if(m_nPaPre>=0 && m_nPaPre<m_jiShu->numPara )
			{
				CString s;
				s = m_editDlg.GetText ();
				float f = atof(s);

			
				if(f>=m_jiShu->min[m_nPaPre] && f<=m_jiShu->max[m_nPaPre] )
				{
					if(m_jiShu && m_nParamInfo [m_nPaPre].m_nParamB!=-1)
					{
						this->m_nParamInfo [m_nPaPre].fVal = f;
						s = "   " + s;
						s+="   ";
						int nAdd = s.GetLength ()-this->m_nParamInfo [m_nPaPre].m_nParamCount ;
						SetSel(m_nParamInfo [m_nPaPre].m_nParamB,m_nParamInfo [m_nPaPre].m_nParamB+m_nParamInfo [m_nPaPre].m_nParamCount);
						ReplaceSel(s);
						m_nParamInfo [m_nPaPre].m_nParamCount +=nAdd;
						for(int j = m_nPaPre+1;j<m_jiShu->numPara ;j++)
						{
							m_nParamInfo [j].m_nParamB += nAdd;
						}
					}
				}
				else
					AfxMessageBox("参数数值超过最大值或最小值!");

			}
		}
	}

	if(nPa >=0)
	{
		{
			CString s;
			s = CLongString::FloatToString(m_nParamInfo [nPa].fVal);
			if(m_jiShu)
				m_editDlg.SetText (s,m_jiShu->min[nPa],m_jiShu->max[nPa]);
			else
				m_editDlg.SetText (s);

			CPoint pt;
			GetPosFormChar(pt, m_nParamInfo [nPa].m_nParamB );

			r.left = pt.x;
			r.right =r.left+r2.Width ();
			r.top = pt.y;
			r.bottom =r.top+r2.Height ();
			m_editDlg.MoveWindow (r);

			m_editDlg.ShowWindow (SW_SHOW);
			m_editDlg.SetSelEdit ();
		}
	}

	m_nPaPre = nPa;

	return nPa;
}

void CTaiTestEdit::GetParamVal(float *fVal)
{
	for(int i = 0;i<8;i++)
		fVal[i] = this->m_nParamInfo [i].fVal ;
}

void CTaiTestEdit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

	
	if(m_editDlg.IsWindowVisible ()) 
	{
		m_editDlg.ShowWindow (SW_HIDE);
		this->m_nPaPre = -1;
	}
	CAutoRichEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
