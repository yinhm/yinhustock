

class GMemDC : public CDC
{
public:
 
   BOOL Create(CDC *pDC,            
               const RECT& rClient,  
               CBrush *pBkBrush=NULL,
               const PAINTSTRUCT *pPS=NULL); 
   

   BOOL Copy(CDC *pDC,             
             const RECT& rDraw,   
             const POINT *ppt=NULL);
                                   
 
   BOOL Release();                   


   const PAINTSTRUCT *GetPaintStruct() const;

public:
   GMemDC();
   ~GMemDC();

private:
   PAINTSTRUCT m_ps;               

   BOOL bPaintStructInitialized;    
   CBitmap *pOldBitmap;             
   CBitmap Bitmap;                  

 
   GMemDC( const GMemDC& );
   GMemDC& operator=( const GMemDC& );
};

