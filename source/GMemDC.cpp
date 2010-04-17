/////////////////////////////////////////////////////////////////////////////////
//if there have any bugs, pls inform me
// Tel:13366898744
//////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GMemDC.h"

GMemDC::GMemDC() : pOldBitmap(NULL), bPaintStructInitialized(FALSE)
{
}

GMemDC::~GMemDC()
{
   
   ASSERT(pOldBitmap == NULL);  
   ASSERT(bPaintStructInitialized == FALSE);
}

BOOL GMemDC::Create(CDC *pDC, const RECT& rDest, CBrush *pBkBrush, const PAINTSTRUCT *pPS)
{
   BOOL bOK = FALSE;
   pOldBitmap = NULL;

   ASSERT(pDC != NULL);
   ASSERT(m_hDC == NULL);
   
   if(pDC != NULL)
   {
      if(CreateCompatibleDC(pDC) != 0) 
      {
         CRect rDestRect(rDest);

      #ifdef _MFC_DOCS_HAVE_NO_ERRORS_
         int iBitsPerPixel = pDC->GetDeviceCaps(BITSPIXEL);

      #ifdef _DEBUG   
         int iThisBitsPerPixel = GetDeviceCaps(BITSPIXEL);
       
         ASSERT(iThisBitsPerPixel == iBitsPerPixel);
      #endif

         
         int iWidth = rDestRect.Width() * iBitsPerPixel; 
         int iHeight = rDestRect.Height() * iBitsPerPixel; 
      
         CSize sizeBitmap(iWidth, iHeight);
      #endif

         CSize sizeBitmap(rDestRect.Width(), rDestRect.Height());

  
         if(Bitmap.CreateCompatibleBitmap(pDC, sizeBitmap.cx, sizeBitmap.cy) != 0)
         {
	         pOldBitmap = SelectObject(&Bitmap);
	         if(pOldBitmap != NULL)
            {
               CBrush *pBrush = pBkBrush;
               if(pBrush == NULL)
                  pBrush = pDC->GetCurrentBrush();

               ASSERT(pBrush != NULL);
               
             
               if(pBrush != NULL)
                  FillRect(rDestRect, pBrush);

               if(pPS != NULL)
               {
                  bPaintStructInitialized = TRUE;
                  memcpy((void *)&m_ps, (void *)pPS, sizeof(m_ps));
               }

               bOK = TRUE;
            }
         }
      }
   }

   ASSERT(bOK == TRUE);

   return bOK;
}

BOOL GMemDC::Copy(CDC *pDC, const RECT& rDest, const POINT *pptSource)
{
   BOOL bOK = FALSE;
   CRect r(rDest);

   ASSERT(this != NULL);
   ASSERT(pDC != NULL);
 
   ASSERT(pDC->GetDeviceCaps(RASTERCAPS) & RC_BITBLT);
   
   POINT ptSrcTopLeft;
   if(pptSource != NULL)
      ptSrcTopLeft = (*pptSource);
   else
   {
      ptSrcTopLeft.x = r.left;
      ptSrcTopLeft.y = r.top;
   }

   if(pDC != NULL)
      bOK = pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), 
                        this, ptSrcTopLeft.x, ptSrcTopLeft.y, SRCCOPY);

   ASSERT(bOK == TRUE);

   return bOK;
}

BOOL GMemDC::Release()
{
   BOOL bOK = FALSE;

   ASSERT(pOldBitmap != NULL);

   if(pOldBitmap != NULL)
   { 
      SelectObject(pOldBitmap);
      pOldBitmap = NULL;
      bOK = TRUE;            
   }

   bPaintStructInitialized = FALSE;

   return bOK;
}

const PAINTSTRUCT *GMemDC::GetPaintStruct() const
{
   const PAINTSTRUCT *pPS;

   if(bPaintStructInitialized == TRUE)
      pPS = (const PAINTSTRUCT *)(&m_ps);
   else
      pPS = NULL;

   return pPS;
}

