

#define DISPOSAL_NO 0
#define DISPOSAL_NOT 4
#define DISPOSAL_RESTBACK 8
#define DISPOSAL_RESTORE 12

typedef struct
{
	BYTE bit;
	WORD previouscode;
	WORD nextcode;
} GIFTABLE;


class CPlayGif
{
private:
	HWND    m_hWnd;
	BOOL    m_bPause;
	BOOL	m_bAutoStart;
	BOOL	m_bEmbed;
	BYTE	m_cCurentByte,m_cPackedField;
	UINT	m_uBitSize,m_uPrimaryBitSize;
	UINT	m_uRemain,m_uReadByte,m_uBlockSize;
	int		m_iWidth,m_iHeight;
	int		m_iTop,m_iLeft;
	int		m_iFinishCode,m_iResetCode;
	int		m_iPass,m_iRow;
	int		m_iWidth1;
	int		m_iBackgroundColor;
	int		m_iGifSize;
	int		m_x,m_y;
	int		m_iGlobalColorSize;
	int		m_iDisposalMethod;
	BOOL	m_bTransparentIndex;
	int		m_iTransparentIndex;
	int		m_iDelayTime;
	int		m_iTotalReadByte;
	int		m_iMaxByte;
	DWORD	m_dwSpeed;
	COLORREF m_TransparentColor;
	HDC		m_hDC;
	BYTE*	m_pcGlobalColorTable;
	BYTE*	m_pcBitmap;
	BYTE*	m_pcGif;
	BYTE*	m_pcGifTrack; 
	BOOL	m_bGlass;
	volatile int m_EndRun;
	HBITMAP m_hRedrawBitmap;
	int		m_iGifWidth,m_iGifHeight;
	volatile BOOL	m_bLockBitmap;
	TCHAR	filename[_MAX_PATH];
	int		flag;
	BOOL	m_bRunMode;
	BOOL	m_bAutoSize1;
	int     m_nPosX;
	int     m_nPosY;

public:	
	CPlayGif(HWND pWnd);
	~CPlayGif();
	void Play();
	void Pause(BOOL status);
	void Stop();
	void SetPosition(int x,int y);
	BOOL Play1(void);
	BOOL Load(LPCTSTR filename);
	HBITMAP FirstImage(void);
	HBITMAP NextImage(void);
	HBITMAP TakeIt(void);

	void Output(BYTE bit)
	{
		int tmp;
		if(m_cPackedField&0x40)
		{
			if(m_x==m_iWidth)
			{
				m_x=0;
				if(m_iPass==1)m_iRow+=8;
				if(m_iPass==2)m_iRow+=8;
				if(m_iPass==3)m_iRow+=4;
				if(m_iPass==4)m_iRow+=2;
				if(m_iRow>=m_iHeight){m_iPass+=1;m_iRow=16>>m_iPass;}
			}
			tmp=m_iRow*m_iWidth1+m_x;
			m_pcBitmap[tmp]=bit;
			m_x++;
		}
		else
		{
			if(m_x==m_iWidth){m_x=0;m_y++;}
			tmp=m_y*m_iWidth1+m_x;
			m_x++;
		}
		if(tmp>m_iMaxByte)return;
		m_pcBitmap[tmp]=bit;
	}

	BYTE GetByte(void)
	{
		if(m_uReadByte>=m_uBlockSize)
		{
			m_uBlockSize=*m_pcGifTrack++;
			m_uReadByte=0;
			m_iTotalReadByte+=m_uBlockSize+1;
			if(m_iTotalReadByte>m_iGifSize){m_iTotalReadByte-=m_uBlockSize+1;return 0xFF;}
			if(m_uBlockSize==0){m_pcGifTrack--;m_iTotalReadByte--;return 0xFF;}
		}
		m_uReadByte++;
		return *m_pcGifTrack++;
	}

	WORD GetCode(void)
	{
		UINT tmp1;
		BYTE tmp;
		tmp=1;
		if(m_uRemain>=m_uBitSize)
		{
			tmp<<=m_uBitSize;
			tmp--;
			tmp1=m_cCurentByte&tmp;
			m_cCurentByte>>=m_uBitSize;
			m_uRemain-=m_uBitSize;
		}
		else
		{
			tmp<<=m_uRemain;
			tmp--;
			tmp1=m_cCurentByte;
			m_cCurentByte=GetByte();
			tmp=1;
			if(8>=(m_uBitSize-m_uRemain))
			{
				tmp<<=(m_uBitSize-m_uRemain);
				tmp--;
				tmp1=(((UINT)(m_cCurentByte&tmp))<<m_uRemain)+tmp1;
				m_cCurentByte>>=(m_uBitSize-m_uRemain);
				m_uRemain=8-(m_uBitSize-m_uRemain);
			}
			else
			{
				tmp1=(((UINT)(m_cCurentByte))<<m_uRemain)+tmp1;
				m_cCurentByte=GetByte();
				tmp<<=m_uBitSize-m_uRemain-8;
				tmp--;
				tmp1=(((UINT)(m_cCurentByte&tmp))<<(m_uRemain+8))+tmp1;
				m_cCurentByte>>=m_uBitSize-m_uRemain-8;
				m_uRemain=8-(m_uBitSize-m_uRemain-8);
			}
		}
		return tmp1;
	}

};

