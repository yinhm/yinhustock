

#ifndef _BTNST_H
#define _BTNST_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CButtonST : public CButton
{

public:
    CButtonST();
	~CButtonST();
    enum {ST_ALIGN_HORIZ, ST_ALIGN_VERT, ST_ALIGN_HORIZ_RIGHT};


public:


public:

    //{{AFX_VIRTUAL(CButtonST)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


public:
	void DrawTransparent(BOOL bRepaint = FALSE);

	BOOL GetDefault();

	void SetTooltipText(int nId, BOOL bActivate = TRUE);
	void SetTooltipText(CString* spText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);

	BOOL SetBtnCursor(int nCursorId = -1);

	void SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint = FALSE);
	BOOL GetFlatFocus();

	void SetDefaultActiveFgColor(BOOL bRepaint = FALSE);
	void SetActiveFgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetActiveFgColor();
	
	void SetDefaultActiveBgColor(BOOL bRepaint = FALSE);
	void SetActiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetActiveBgColor();
	
	void SetDefaultInactiveFgColor(BOOL bRepaint = FALSE);
	void SetInactiveFgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetInactiveFgColor();

	void SetDefaultInactiveBgColor(BOOL bRepaint = FALSE);
	void SetInactiveBgColor(COLORREF crNew, BOOL bRepaint = FALSE);
	const COLORREF GetInactiveBgColor();

	void SetShowText(BOOL bShow = TRUE);
	BOOL GetShowText();

	void SetAlign(int nAlign);
	int GetAlign();

	void SetFlat(BOOL bState = TRUE);
	BOOL GetFlat();

	void DrawBorder(BOOL bEnable = TRUE);
	void SetIcon(int nIconInId, int nIconOutId = NULL);
	void SetIcon(HICON hIconIn, HICON hIconOut = NULL);

	static const short GetVersionI();
	static const char* GetVersionC();

protected:
    //{{AFX_MSG(CButtonST)
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
private:
	void DrawTheIcon(CDC* pDC, CString* title, RECT* rcItem, CRect* captionRect, BOOL IsPressed, BOOL IsDisabled);
	void InitToolTip();
	void PaintBk(CDC* pDC);

	int m_nAlign;
	BOOL m_bShowText;
	BOOL m_bDrawBorder;
	BOOL m_bIsFlat;
	BOOL m_MouseOnButton;
	BOOL m_bDrawFlatFocus;

	HCURSOR m_hCursor;
	CToolTipCtrl m_ToolTip;

	HICON m_hIconIn;
	HICON m_hIconOut;
	BYTE m_cyIcon;
	BYTE m_cxIcon;

	CDC m_dcBk;
	CBitmap m_bmpBk;
	CBitmap* m_pbmpOldBk;
	BOOL m_bDrawTransparent;

	BOOL m_bIsDefault;

	COLORREF  m_crInactiveBg;
    COLORREF  m_crInactiveFg;
    COLORREF  m_crActiveBg;
    COLORREF  m_crActiveFg;
};


#ifdef ST_USE_MEMDC



class CMemDC : public CDC
{
public:

    
    CMemDC(CDC* pDC) : CDC()
    {
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
              
        if (m_bMemDC)   
        {
            pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else        
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    
    
    ~CMemDC()
    {
        if (m_bMemDC) 
        {    
           
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);

         
            SelectObject(m_pOldBitmap);
        } else {
           
            m_hDC = m_hAttribDC = NULL;
        }
    }


    CMemDC* operator->() {return this;}
        
 
    operator CMemDC*() {return this;}

private:
    CBitmap  m_bitmap;     
    CBitmap* m_pOldBitmap;  
    CDC*     m_pDC;         
    CRect    m_rect;     
    BOOL     m_bMemDC;    
};

#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
