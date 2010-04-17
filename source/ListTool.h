
typedef struct tag3DPens
{
   CPen *pPenBlack;
   CPen *pPenDarkGray;
   CPen *pPenWhite;
   CPen *pPenGray;

   tag3DPens();
   ~tag3DPens();

} _3DPENS, *LP3DPENS;
   
int GfxDraw3DBorder(CDC *pDC, LP3DPENS p3DPens, 
                     CRect rectBorder, BOOL bInside=TRUE);
void GfxDrawBorder(CDC *pDC, CRect rectBorder, CPen *pPen=NULL); 
void GfxDrawBorder(CDC *pDC, CRect rectBorder, 
                  COLORREF clFill, CPen *pPen=NULL);

class CListTool : public CDC
{

public:
	CListTool();

public:
	void Draw_Line(CDC *pDC,int x1,int y1,int x2,int y2);
	void Draw_Button(CDC *pDC,CRect rect);
	void ShowText(CDC *pDC,CString& str, LPRECT lpRect,UINT nFormat );
	void DrawPolyLine(CDC *pDc,POINT *pt,int count,int nDrawMode,COLORREF Linecolor);
	void Draw_Rectangle(CDC *pDc,LPCRECT lpRect,POINT pt,COLORREF Color);
	void Show3DText(CDC *pDC,POINT pt, CString str,int dx,COLORREF Color,int flag);
	void Draw_Button(CDC *pDc,int left_x,int top_y,int right_x,
	        int bottom_y,int linew,COLORREF Color,int Flag);

};


