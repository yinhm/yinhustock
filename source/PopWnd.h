
class GPopupWindow : public CWnd
{
   DECLARE_MESSAGE_MAP()

public:
   GPopupWindow();
   
protected:
   
   afx_msg int OnCreate(LPCREATESTRUCT lpcs);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
   afx_msg void OnCaptureChanged(CWnd *pWnd);

   virtual BOOL OnLButtonDownCheck(UINT nFlags, CPoint pt);
   virtual void PostNcDestroy();
public:
   virtual ~GPopupWindow();
};



 