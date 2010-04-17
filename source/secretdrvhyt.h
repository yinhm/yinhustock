
#if !defined(AFX_SECRETDRV_H__27282D89_4E49_11D3_9200_00400552E583__INCLUDED_)
#define AFX_SECRETDRV_H__27282D89_4E49_11D3_9200_00400552E583__INCLUDED_

#define KEY "5989861278";

class CSecretDrv2 
{
public:
	bool SecretOrDesecret(BYTE* pIn,BYTE* pOut,int nFlags);
	static int From16ToInt(char* ch);
	static void StringToByte(CString s,BYTE* pByte,int & nCount);
	static CString ByteToString(BYTE* pByte,int nCount);
	CString DesecretSerialID(CString strSerialSecreted,bool& bSuccess);
	CString SecretSerialID(CString strSerial,bool& bSuccess);
	bool CreateImpowerID(CString serialID,CString machineID,CString& impowerOut);
	PCHAR szPassword;
	CSecretDrv2();
	void secret(PBYTE desecret,PBYTE source);
	void desecret(PBYTE source,PBYTE desecret);
	void HandleError(char *s);

protected:
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_SECRETDRV_H__27282D89_4E49_11D3_9200_00400552E583__INCLUDED_)
