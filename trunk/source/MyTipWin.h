#if !defined(AFX_MYTIPWIN_H__4F7C1005_A38A_11D1_B032_000021002C16__INCLUDED_)
#define AFX_MYTIPWIN_H__4F7C1005_A38A_11D1_B032_000021002C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CTaiTestTipWin : public CWnd
{

public:
	CTaiTestTipWin();
	virtual BOOL Create( CWnd *pParentWnd);
	void Show(LPCTSTR lpszTitleText, CPoint p);
	void Hide();

	CPoint m_pOld;
	int max;
	int m_maxHeight;

public:

public:

	//{{AFX_VIRTUAL(CTaiTestTipWin)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
public:
	virtual ~CTaiTestTipWin();

protected:
	CPoint m_pointOld;
	void InitMember();
	int m_height;

	CWnd *m_pParentWnd;

	TEXTMETRIC tm;
	CFont fnt;
	int CalcSize();
	int m_fontSize;
	CStringList m_stringList;

	void SetString(CString& s);

protected:
	//{{AFX_MSG(CTaiTestTipWin)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTIPWIN_H__4F7C1005_A38A_11D1_B032_000021002C16__INCLUDED_)
