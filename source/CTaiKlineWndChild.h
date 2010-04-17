#if !defined(AFX_LMBWNDCHILD_H__73FA6A44_A3DC_11D3_96E0_0080C8E1242B__INCLUDED_)
#define AFX_LMBWNDCHILD_H__73FA6A44_A3DC_11D3_96E0_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineWndChild.h : header file


class CTaiKlineWndChild : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTaiKlineWndChild)
protected:
	CTaiKlineWndChild();           


public:
		CCreateContext* m_pContext ;


public:


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineWndChild)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTaiKlineWndChild();

	
	//{{AFX_MSG(CTaiKlineWndChild)
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBWNDCHILD_H__73FA6A44_A3DC_11D3_96E0_0080C8E1242B__INCLUDED_)
