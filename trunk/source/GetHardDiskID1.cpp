// GetHardDiskID1.cpp: implementation of the CGetHardDiskID class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetHardDiskID1.h"
//#include <windows.h>
#include <iostream>
#include <stdio.h>

#define DFP_GET_VERSION 0x00074080
#define DFP_SEND_DRIVE_COMMAND 0x0007c084
#define DFP_RECEIVE_DRIVE_DATA 0x0007c088

#pragma pack(1)
typedef struct _GETVERSIONOUTPARAMS {
    BYTE bVersion; 
    BYTE bRevision; 
    BYTE bReserved;
    BYTE bIDEDeviceMap; 
    DWORD fCapabilities; 
    DWORD dwReserved[4]; 
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

typedef struct _IDEREGS {
    BYTE bFeaturesReg; 
    BYTE bSectorCountReg; 
    BYTE bSectorNumberReg; 
    BYTE bCylLowReg; 
    BYTE bCylHighReg;
    BYTE bDriveHeadReg;
    BYTE bCommandReg; 
    BYTE bReserved;
} IDEREGS, *PIDEREGS, *LPIDEREGS;

typedef struct _SENDCMDINPARAMS {
    DWORD cBufferSize; 
    IDEREGS irDriveRegs; 
    BYTE bDriveNumber; 
 
    BYTE bReserved[3]; 
    DWORD dwReserved[4]; 
   
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;

typedef struct _DRIVERSTATUS {
    BYTE bDriverError; 
    BYTE bIDEStatus;
    BYTE bReserved[2]; 
    DWORD dwReserved[2]; 
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;

typedef struct _SENDCMDOUTPARAMS {
    DWORD cBufferSize; 
    DRIVERSTATUS DriverStatus; 
    BYTE bBuffer[512]; 
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;

typedef struct _IDSECTOR {
    USHORT wGenConfig;
    USHORT wNumCyls;
    USHORT wReserved;
    USHORT wNumHeads;
    USHORT wBytesPerTrack;
    USHORT wBytesPerSector;
    USHORT wSectorsPerTrack;
    USHORT wVendorUnique[3];
    CHAR sSerialNumber[20];
    USHORT wBufferType;
    USHORT wBufferSize;
    USHORT wECCSize;
    CHAR sFirmwareRev[8];
    CHAR sModelNumber[40];
    USHORT wMoreVendorUnique;
    USHORT wDoubleWordIO;
    USHORT wCapabilities;
    USHORT wReserved1;
    USHORT wPIOTiming;
    USHORT wDMATiming;
    USHORT wBS;
    USHORT wNumCurrentCyls;
    USHORT wNumCurrentHeads;
    USHORT wNumCurrentSectorsPerTrack;
    ULONG ulCurrentSectorCapacity;
    USHORT wMultSectorStuff;
    ULONG ulTotalAddressableSectors;
    USHORT wSingleWordDMA;
    USHORT wMultiWordDMA;
    BYTE bReserved[128];
} IDSECTOR, *PIDSECTOR;



//////////////////////////////////////////////////////////////////////


CGetHardDiskID::CGetHardDiskID()
{

}

CGetHardDiskID::~CGetHardDiskID()
{

}
VOID ChangeByteOrder(PCHAR szString, USHORT uscStrSize)
{
    
    USHORT i;
    CHAR temp;
    
    for (i = 0; i < uscStrSize; i+=2)
    {
        temp = szString[i];
        szString[i] = szString[i+1];
        szString[i+1] = temp;
    }
}
CString gethdidString(CString s)
{
	CString sRtn;
	for(int k=0;k<s.GetLength ();k++)
	{
		if(s[k] != ' ' && s[k] != '\r' && s[k] != '\n' && s[k] != '\t')
		{
			int n = s[k]%10;
			char ch2 = '0'+n ;
			CString sTemp (ch2);
			sRtn+=sTemp;
		}
		if(sRtn.GetLength ()>=8)
			break;
	}
	while(sRtn.GetLength ()<8)
	{
		sRtn+="0";
	}
	return sRtn;

}

CString hdid9x(CString &sError)
{
	CString s = "";
	sError = "";

	GETVERSIONOUTPARAMS vers;
	SENDCMDINPARAMS in;
	SENDCMDOUTPARAMS out;

	HANDLE h;
	DWORD i;
	int j;
    ZeroMemory(&vers,sizeof(vers));
   
    h=CreateFile("\\\\.\\Smartvsd",0,0,0,CREATE_NEW,0,0);
    if (!h){
        sError = "open smartvsd.vxd failed" ;
        return s;
    }
    
    if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){
        sError = "DeviceIoControl failed:DFP_GET_VERSION" ;
        CloseHandle(h);
        return s;
    }
   
    if (!(vers.fCapabilities&1)){
        sError = "Error: IDE identify command not supported.";
        CloseHandle(h);
        return s;
    }

    for (j=0;j<4;j++){
        PIDSECTOR phdinfo;
        char ch[41];
        
        ZeroMemory(&in,sizeof(in));
        ZeroMemory(&out,sizeof(out));
        if (j&1){
            in.irDriveRegs.bDriveHeadReg=0xb0;
        }else{
            in.irDriveRegs.bDriveHeadReg=0xa0;
        }
        if (vers.fCapabilities&(16>>j)){
            
         
			CString sj;
			sj.Format ("Drive %d",(int)(j+1));
            sError = sj + " is a ATAPI device, we don't detect it" ;
            continue;
        }else{
            in.irDriveRegs.bCommandReg=0xec;
        }
        in.bDriveNumber=j;
        in.irDriveRegs.bSectorCountReg=1;
        in.irDriveRegs.bSectorNumberReg=1;
        in.cBufferSize=512;
        if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){
            sError = "DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA" ;
            CloseHandle(h);
            return s;
        }
        phdinfo=(PIDSECTOR)out.bBuffer;
        memcpy(ch,phdinfo->sModelNumber,40);
        ch[40]=0;
        ChangeByteOrder(ch,40);

        memcpy(ch,phdinfo->sFirmwareRev,8);
        ch[8]=0;
        ChangeByteOrder(ch,8);

        memcpy(ch,phdinfo->sSerialNumber,20);
        ch[20]=0;
        ChangeByteOrder(ch,20);

		s = ch;
		s = gethdidString( s);

		break;
    }
    
  
    CloseHandle(h);
	return s;
}

CString hdidnt(CString &sError)
{
	CString sRtn = "";
	sError = "";

	GETVERSIONOUTPARAMS vers;
	SENDCMDINPARAMS in;
	SENDCMDOUTPARAMS out;

	HANDLE h;
	DWORD i;
	int j;
    char hd[80];
    PIDSECTOR phdinfo;
    char s[41];
    
    ZeroMemory(&vers,sizeof(vers));

    for (j=0;j<4;j++){
        sprintf(hd,"\\\\.\\PhysicalDrive%d",j);
        h=CreateFile(hd,GENERIC_READ|GENERIC_WRITE,
            FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_EXISTING,0,0);
        if (!h){
            continue;
        }
        if (!DeviceIoControl(h,DFP_GET_VERSION,0,0,&vers,sizeof(vers),&i,0)){
            CloseHandle(h);
            continue;
        }
       
        if (!(vers.fCapabilities&1)){
            sError = "Error: IDE identify command not supported.";
            CloseHandle(h);
            return sRtn;
        }
     
        ZeroMemory(&in,sizeof(in));
        ZeroMemory(&out,sizeof(out));
        if (j&1){
            in.irDriveRegs.bDriveHeadReg=0xb0;
        }else{
            in.irDriveRegs.bDriveHeadReg=0xa0;
        }
        if (vers.fCapabilities&(16>>j)){
     
			CString sj;
			sj.Format ("Drive %d",(int)(j+1));
            sError = sj + " is a ATAPI device, we don't detect it" ;
            continue;
        }else{
            in.irDriveRegs.bCommandReg=0xec;
        }
        in.bDriveNumber=j;
        in.irDriveRegs.bSectorCountReg=1;
        in.irDriveRegs.bSectorNumberReg=1;
        in.cBufferSize=512;
        if (!DeviceIoControl(h,DFP_RECEIVE_DRIVE_DATA,&in,sizeof(in),&out,sizeof(out),&i,0)){
            sError = "DeviceIoControl failed:DFP_RECEIVE_DRIVE_DATA" ;
            CloseHandle(h);
            return sRtn;
        }
        phdinfo=(PIDSECTOR)out.bBuffer;
        memcpy(s,phdinfo->sModelNumber,40);
        s[40]=0;
        ChangeByteOrder(s,40);

        memcpy(s,phdinfo->sFirmwareRev,8);
        s[8]=0;
        ChangeByteOrder(s,8);

        memcpy(s,phdinfo->sSerialNumber,20);
        s[20]=0;
        ChangeByteOrder(s,20);
		sRtn = s;
		sRtn = gethdidString( sRtn);

        CloseHandle(h);
    }
	return sRtn;
}
;

CString CGetHardDiskID::GetID(CString &sError)
{
	CString s = "";
	sError = "";
    char szAddr[20];

    OSVERSIONINFO VersionInfo;
    
    ZeroMemory(&VersionInfo,sizeof(VersionInfo));
    VersionInfo.dwOSVersionInfoSize=sizeof(VersionInfo);
    GetVersionEx(&VersionInfo);
    
    switch (VersionInfo.dwPlatformId){
    case VER_PLATFORM_WIN32s:
        TRACE("Win32s is not supported by this programm.");
        break;
    case VER_PLATFORM_WIN32_WINDOWS:
        s = hdid9x(sError);
        break;
    case VER_PLATFORM_WIN32_NT:
        s = hdidnt(sError);
        break;
    }
    
	return s;
}
