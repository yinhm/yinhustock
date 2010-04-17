#if !defined(AFX_CDIBSECTIONLITE_H__35D9F3D4_B960_11D2_A981_2C4476000000__INCLUDED_)
#define AFX_CDIBSECTIONLITE_H__35D9F3D4_B960_11D2_A981_2C4476000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DIBSectionLite.h : header file
//

// Copyright © Dundas Software Ltd. 1999, All Rights Reserved

// //////////////////////////////////////////////////////////////////////////

// Properties:
//	NO	Abstract class (does not have any objects)
//	NO	Derived from CWnd
//	NO	Is a CWnd.                     
//	NO	Two stage creation (constructor & Create())
//	NO	Has a message map
//	NO 	Needs a resource (template)
//	YES	Persistent objects (saveable on disk)      
//	YES	Uses exceptions

// //////////////////////////////////////////////////////////////////////////

// Desciption :         

// CDIBSectionLite is DIBSection wrapper class for win32 and WinCE platforms.
// This class provides a simple interface to DIBSections including loading,
// saving and displaying DIBsections.
//
// Full palette support is provided for Win32 and CE 2.11 and above. 

// Using CDIBSectionLite :

// This class is very simple to use. The bitmap can be set using either SetBitmap()
// (which accepts either a Device dependant or device independant bitmap, or a 
// resource ID) or by using Load(), which allows an image to be loaded from disk. 
// To display the bitmap simply use Draw or Stretch.
//
// eg. 
//
//      CDIBsection dibsection;
//      dibsection.Load(_T("image.bmp"));
//      dibsection.Draw(pDC, CPoint(0,0));  // pDC is of type CDC*
//
//      CDIBsection dibsection;
//      dibsection.SetBitmap(IDB_BITMAP); 
//      dibsection.Draw(pDC, CPoint(0,0));  // pDC is of type CDC*
//



/////////////////////////////////////////////////////////////////////////////
// defines

//#define DIBSECTION_NO_DITHER          // Disallow dithering via DrawDib functions
#define DIBSECTION_NO_MEMDC_REUSE       // Disallow the reuse of memory DC's
//#define DIBSECTION_NO_PALETTE         // Remove palette support

// Only provide palette support for non-CE platforms, or for CE 2.11 and above
#ifdef _WIN32_WCE
#define DIBSECTION_NO_DITHER            
#if (_WIN32_WCE < 211)
#define DIBSECTION_NO_PALETTE          
#endif
#endif


#ifndef DIBSECTION_NO_DITHER 
#include <vfw.h>
#pragma comment(lib, "vfw32")
#endif


#define DS_BITMAP_FILEMARKER  ((WORD) ('M' << 8) | 'B')    

/////////////////////////////////////////////////////////////////////////////


struct DIBINFO : public BITMAPINFO
{
	RGBQUAD	 arColors[255];    

	operator LPBITMAPINFO()          { return (LPBITMAPINFO) this; }
	operator LPBITMAPINFOHEADER()    { return &bmiHeader;          }
	RGBQUAD* ColorTable()            { return bmiColors;           }
};

/////////////////////////////////////////////////////////////////////////////


#ifndef DIBSECTION_NO_PALETTE
struct PALETTEINFO : public LOGPALETTE
{
    PALETTEENTRY arPalEntries[255];               

    PALETTEINFO() 
    {
        palVersion    = (WORD) 0x300;
        palNumEntries = 0;
        ::memset(palPalEntry, 0, 256*sizeof(PALETTEENTRY)); 
    }

    operator LPLOGPALETTE()   { return (LPLOGPALETTE) this;            }
    operator LPPALETTEENTRY() { return (LPPALETTEENTRY) (palPalEntry); }
};
#endif 


/////////////////////////////////////////////////////////////////////////////
// CDIBSectionLite object

class AFX_EXT_CLASS CDIBSectionLite : public CObject
{

public:
	CDIBSectionLite();
	virtual ~CDIBSectionLite();
    void DeleteObject();


public:
    static int BytesPerLine(int nWidth, int nBitsPerPixel);
    static int NumColorEntries(int nBitsPerPixel, int nCompression, DWORD biClrUsed = 0);

    static RGBQUAD ms_StdColors[];
#ifndef DIBSECTION_NO_PALETTE
    static BOOL UsesPalette(CDC* pDC) { return (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE); }
    static BOOL CreateHalftonePalette(CPalette& palette, int nNumColors);
#endif 


public:
    HBITMAP      GetSafeHandle() const       { return (this)? m_hBitmap : NULL;        }
    operator     HBITMAP() const             { return GetSafeHandle();                 }
    CSize        GetSize() const             { return CSize(GetWidth(), GetHeight());  }
    int          GetHeight() const           { return m_DIBinfo.bmiHeader.biHeight;    } 
    int          GetWidth() const            { return m_DIBinfo.bmiHeader.biWidth;     }
    int          GetPlanes() const           { return m_DIBinfo.bmiHeader.biPlanes;    }
    int          GetBitCount() const         { return m_DIBinfo.bmiHeader.biBitCount;  }
    LPVOID       GetDIBits()                 { return m_ppvBits;                       }
    LPBITMAPINFO GetBitmapInfo()             { return  (BITMAPINFO*) m_DIBinfo;        }
    DWORD        GetImageSize() const        { return m_DIBinfo.bmiHeader.biSizeImage; }
    LPBITMAPINFOHEADER GetBitmapInfoHeader() { return (BITMAPINFOHEADER*) m_DIBinfo;   }


public:
    LPRGBQUAD GetColorTable()             { return m_DIBinfo.ColorTable();          }
    BOOL      SetColorTable(UINT nNumColors, RGBQUAD *pColors);
    int       GetColorTableSize()         { return m_iColorTableSize;               }
#ifndef DIBSECTION_NO_PALETTE
    CPalette *GetPalette()  { return &m_Palette; }
    BOOL      SetPalette(CPalette* pPalette);
    BOOL      SetLogPalette(LOGPALETTE* pLogPalette);
#endif


public:
    BOOL SetBitmap(UINT nIDResource);
    BOOL SetBitmap(LPCTSTR lpszResourceName);
    BOOL SetBitmap(HBITMAP hBitmap
#ifndef DIBSECTION_NO_PALETTE
                   , CPalette* pPalette = NULL
#endif
                   );
    BOOL SetBitmap(LPBITMAPINFO lpBitmapInfo, LPVOID lpBits);   

    BOOL Load(LPCTSTR lpszFileName);
    BOOL Save(LPCTSTR lpszFileName);
    BOOL Copy(CDIBSectionLite& Bitmap);


public:
    BOOL Draw(CDC* pDC, CPoint ptDest, BOOL bForceBackground = FALSE);
    BOOL Stretch(CDC* pDC, CPoint ptDest, CSize size, BOOL bForceBackground = FALSE);

#ifndef DIBSECTION_NO_DITHER
    BOOL SetDither(BOOL bDither);
    BOOL GetDither();
#endif 

    CDC* GetMemoryDC(CDC* pDC = NULL, BOOL bSelectPalette = TRUE);
    BOOL ReleaseMemoryDC(BOOL bForceRelease = FALSE);




public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
    void _ShowLastError();
#ifndef DIBSECTION_NO_PALETTE
    BOOL CreatePalette();
    BOOL FillDIBColorTable(UINT nNumColors, RGBQUAD *pRGB);
#endif 
    UINT GetColorTableEntries(HDC hdc, HBITMAP hBitmap);
#ifndef DIBSECTION_NO_DITHER
    HDRAWDIB GetDrawDibContext();
#endif

protected:
    HBITMAP  m_hBitmap;    
    DIBINFO  m_DIBinfo;         
    VOID    *m_ppvBits;         
    UINT     m_iColorDataType;  
    UINT     m_iColorTableSize; 

    CDC      m_MemDC;           

#ifndef DIBSECTION_NO_DITHER
    BOOL     m_bDither;          
    HDRAWDIB m_hDrawDib;        
#endif

#ifndef DIBSECTION_NO_MEMDC_REUSE
    BOOL     m_bReuseMemDC;      
#endif

#ifndef DIBSECTION_NO_PALETTE
    CPalette m_Palette;        
    CPalette *m_pOldPalette;
#endif 

private:
    HBITMAP  m_hOldBitmap;     
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDIBSECTIONLITE_H__35D9F3D4_B960_11D2_A981_2C4476000000__INCLUDED_)
