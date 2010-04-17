#if !defined(AFX_SECRETDRV_H__27282D89_4E49_11D3_9200_00400552E583__INCLUDED_)
#define AFX_SECRETDRV_H__27282D89_4E49_11D3_9200_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define KEY 0x59;

class CSecretDrv 
{
public:
	CSecretDrv();	// standard constructor
	void secret(char desecret[20],char source[10],int key);
	void desecret(char source[10],char desecret[20],int key);
protected:
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_SECRETDRV_H__27282D89_4E49_11D3_9200_00400552E583__INCLUDED_)
