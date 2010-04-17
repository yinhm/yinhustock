
typedef struct _GV_ITEM { 
    int      row,col;     
    UINT     mask;      
    UINT     state;       
    UINT     nFormat;     
    CString  szText; 
    int      iImage;   
    COLORREF crBkClr;  
    COLORREF crFgClr; 
    LPARAM   lParam;   
    LOGFONT  lfFont;    
} GV_ITEM; 


#define GVL_NONE                0
#define GVL_HORZ                1
#define GVL_VERT                2
#define GVL_BOTH                3


#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE<<1)
#define GVIF_FGCLR              (GVIF_STATE<<2)
#define GVIF_FORMAT             (GVIF_STATE<<3)
#define GVIF_FONT               (GVIF_STATE<<4)


#define GVIS_FOCUSED            0x0001
#define GVIS_SELECTED           0x0002
#define GVIS_DROPHILITED        0x0004
#define GVIS_READONLY           0x0008


#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW|LVNI_TORIGHT)


#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

typedef struct tagNM_GRIDVIEW { 
    NMHDR hdr; 
    int   iRow; 
    int   iColumn; 
} NM_GRIDVIEW;

typedef struct tagGV_DISPINFO { 
    NMHDR   hdr; 
    GV_ITEM item; 
} GV_DISPINFO;


#define	GVN_BEGINDRAG           LVN_BEGINDRAG        
#define	GVN_BEGINLABELEDIT      LVN_BEGINLABELEDIT  
#define	GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define	GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDLABELEDIT        LVN_ENDLABELEDIT     
#define GVN_SELCHANGING         LVN_BEGINLABELEDIT+1 
#define GVN_SELCHANGED          LVN_BEGINLABELEDIT+2 




class CGridCell : public CObject
{
public:
    CGridCell() 
    {
        state = 0;
        nFormat = 0;
        szText.Empty();
        iImage = -1;
        lParam = 0;
		crBkClr = CLR_DEFAULT;
		crFgClr = CLR_DEFAULT;
    }

    UINT     state;      
    UINT     nFormat;     
    CString  szText;   
    int      iImage;      
    COLORREF crBkClr;     
    COLORREF crFgClr;     
    LPARAM   lParam;     
    LOGFONT  lfFont;     
}; 