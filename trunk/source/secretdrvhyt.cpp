// by 俞明录
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <wincrypt.h>

#include "secretdrvhyt.h"


#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)

//--------------------------------------------------------------------

#define ENCRYPT_ALGORITHM CALG_RC2 
#define ENCRYPT_BLOCK_SIZE 8 
#define BUFFER_LENGTH 8 

CSecretDrv2::CSecretDrv2()
{
}

void CSecretDrv2::secret(PBYTE desecret,PBYTE source) 
{
	SecretOrDesecret(source,desecret, 0);
}

void CSecretDrv2::desecret(PBYTE source,PBYTE desecret)
{
	SecretOrDesecret( desecret,source,1);
}

void CSecretDrv2::HandleError(char *s)
{
	char StdErr[200];
    sprintf(StdErr,"错误发生在运行程序 . \n");
    sprintf(StdErr,"%s\n",s);
    sprintf(StdErr, "出错号： %x.\n", GetLastError());
	AfxMessageBox(StdErr);
}

bool CSecretDrv2::CreateImpowerID(CString serialID, CString machineID, CString& impowerOut)
{
	ASSERT(serialID.GetLength ()>=8);
	ASSERT(machineID.GetLength ()>=8);
	BYTE bIn[8];
	BYTE bOut[8];

	//
	for(int i=0;i<4;i++)
	{
		char ch[3];
		ch[0]=serialID.GetAt (i*2);
		ch[1]=serialID.GetAt (i*2+1);
		int n = From16ToInt(ch);
		bIn[i]=(BYTE)n;
	}
	for(int i=0;i<4;i++)
	{
		char ch[5];
		ch[0]=machineID.GetAt (i*2);
		ch[1]=machineID.GetAt (i*2+1);
		int n = From16ToInt(ch);
		bIn[i+4]=(BYTE)n;
	}

	
	secret (bOut,bIn);

	
	impowerOut="";
	for(int i=0;i<4;i++)
	{
		int n = (int)bOut[i];
		CString s;
		s.Format ("%2x",n);
		if(s[0]==' ')
			s.SetAt (0,'0');
		if(s[1]==' ')
			s.SetAt (1,'0');

		impowerOut+=s;
	}
	for(int i=0;i<4;i++)
	{
		int n = (int)bOut[i+4];
		CString s;
		s.Format ("%2x",n);
		if(s[0]==' ')
			s.SetAt (0,'0');
		if(s[1]==' ')
			s.SetAt (1,'0');

		impowerOut+=s;
	}
	impowerOut=impowerOut.Left(8);
	return true;

}

CString CSecretDrv2::SecretSerialID(CString strSerial,bool& bSuccess)
{
	bSuccess = true;
	ASSERT(strSerial.GetLength ()==8);
	BYTE bIn[8];
	BYTE bOut[8];
	for(int i =0;i<8;i++)
	{
		char ch = strSerial[i];
		if(ch>'9'||ch<'0')
			bSuccess=false;
	}



	for(int i=0;i<4;i++)
	{
		char ch[5];
		ch[0]=strSerial.GetAt (i*2);
		ch[1]=strSerial.GetAt (i*2+1);
		int n = From16ToInt(ch);
		bIn[2*i]=(BYTE)n;
	}
	bIn[1]=bIn[6]+0x66;
	bIn[3]=bIn[4]+0x55;
	bIn[5]=bIn[6]+0x33;
	bIn[7]=bIn[0]+0x66;

	secret (bOut,bIn);


	CString sOut="";
	for(int i=0;i<8;i++)
	{
		int n = (int)bOut[i];
		CString s;
		s.Format ("%2x",n);
		if(s[0]==' ')
			s.SetAt (0,'0');
		if(s[1]==' ')
			s.SetAt (1,'0');

		sOut+=s;
	}
	return sOut;
}

CString CSecretDrv2::DesecretSerialID(CString strSerialSecreted,bool& bSuccess)
{
	bSuccess = true;
	ASSERT(strSerialSecreted.GetLength ()==16);
	BYTE bIn[8];
	BYTE bOut[8];


	for(int i=0;i<8;i++)
	{
		char ch[5];
		ch[0]=strSerialSecreted.GetAt (i*2);
		ch[1]=strSerialSecreted.GetAt (i*2+1);
		int n = From16ToInt(ch);
		bIn[i]=(BYTE)n;
	}

	desecret (bOut,bIn);

	if(bOut[1]-0x66!=bOut[6])
		bSuccess=false;
	if(bOut[3]-0x55!=bOut[4])
		bSuccess=false;
	if(bOut[5]-0x33!=bOut[6])
		bSuccess=false;
	if(bOut[7]-0x66!=bOut[0])
		bSuccess=false;


	CString sOut="";
	for(int i=0;i<4;i++)
	{
		int n = (int)bOut[i*2];
		CString s;
		s.Format ("%2x",n);
		if(s[0]==' ')
			s.SetAt (0,'0');
		if(s[1]==' ')
			s.SetAt (1,'0');

		sOut+=s;
	}
	ASSERT(sOut.GetLength ()==8);
	return sOut;

}

CString CSecretDrv2::ByteToString(BYTE *pByte, int nCount)
{
	CString sOut="";
	for(int i=0;i<nCount;i++)
	{
		int n = (int)pByte[i];
		CString s;
		s.Format ("%2x",n);
		if(s[0]==' ')
			s.SetAt (0,'0');
		if(s[1]==' ')
			s.SetAt (1,'0');

		sOut+=s;
	}
	return sOut;

}

void CSecretDrv2::StringToByte(CString s, BYTE *pByte, int &nCount)
{

	int n = s.GetLength ();
	if(n%2==1)
		n--;

	char ch[5];
	ch[4]=0;
	for(int i=0;i<n/2;i++)
	{
		ch[0]=s.GetAt (i*2);
		ch[1]=s.GetAt (i*2+1);
		int n2 = From16ToInt(ch);
		pByte[i]=(BYTE)n2;
	}
	nCount = n/2;

	return ;

}

int CSecretDrv2::From16ToInt(char *ch)
{
	int n = 0;
	if(ch[1]>='0'&&ch[1]<='9')
		n = ch[1]-'0';
	if(ch[1]>='a'&&ch[1]<='f')
		n = ch[1]-'a'+10;
	if(ch[1]>='A'&&ch[1]<='F')
		n = ch[1]-'A'+10;

	if(ch[0]>='0'&&ch[0]<='9')
		n += (ch[0]-'0')*16;
	if(ch[0]>='a'&&ch[0]<='f')
		n += (ch[0]-'a'+10)*16;
	if(ch[0]>='A'&&ch[0]<='F')
		n += (ch[0]-'A'+10)*16;

	if(n<0)
		ASSERT(FALSE);

	return n;


}
/////////////////////
unsigned short getbit(unsigned short value,int n);


unsigned short getbit(unsigned short value,int n)
{
	unsigned short z;
	z=~0;
	z=(z>>(n-1))&(z<<(16-n));
	z=value&z;
	z=z>>(16-n);
	return(z);
}



bool CSecretDrv2::SecretOrDesecret(BYTE *pIn, BYTE *pOut,int nFlags)
{
	//8888888888888888888888888888888888888888888888888888
	//****************************************************
	


	try
	{
	int i,j,m,n,u,q;
	char key[9] = "shzg1lmb";

	unsigned short keybit[64];
	unsigned short pc1[56]={57, 49, 41, 33, 25, 17, 9,
                 1,  58, 50, 42, 34, 26, 18,
                 10,  2, 59, 51, 43, 35, 27,
                 19, 11,  3, 60, 52, 44, 36,
                 63, 55, 47, 39, 31, 23, 15,
                  7, 62, 54, 46, 38, 30, 22,
                 14,  6, 61, 53, 45, 37, 29,
                 21, 13,5,28,20,12,4};
	unsigned short c[17][28],d[17][28];
	           
	               
    unsigned short leftshift[16]={1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
	             
	unsigned short pc2[48]={ 14, 17, 11,  24,  1,  5,
						  3, 28, 15,  6,  21,  10,
						 23, 19, 12,  4,  26,  8,
						 16, 7,  27, 20,  13,  2,
						 41, 52, 31, 37,  47,  55,
						 30, 40, 51, 45,  33,  48,
						 44, 49, 39, 56,  34,  53,
						 46, 42, 50, 36,  29,  32};
	unsigned short k[16][48];   
//**********************************************************
	//**************************************
//tel:13366898744
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		{
			keybit[8*i+j]=getbit(key[i],(j+9));
		}

//**********************************************************
	//**************************************

    unsigned short mid;
	for(i=0;i<28;i++)
	{
		mid=pc1[i]-1;
		c[0][i]=keybit[mid];
		mid=pc1[i+28]-1;
		d[0][i]=keybit[mid];
	}
 //**********************************************************
	//**************************************



	int cd[56];  
	for(i=0;i<16;i++) 
	{
		if(leftshift[i]==1) 
		{
			c[i+1][27]=c[i][0];
			d[i+1][27]=d[i][0];
			for(j=0;j<27;j++)  
			{
				c[i+1][j]=c[i][j+1];
				d[i+1][j]=d[i][j+1];
			}
			for(m=0;m<28;m++)
			{
				cd[m]=c[i+1][m]; 
				cd[m+28]=d[i+1][m];
			}
			for(n=0;n<48;n++) 
			{
				mid=pc2[n]-1;
				k[i][n]=cd[mid];
			}

		}
		else
		{
			c[i+1][26]=c[i][0];
			c[i+1][27]=c[i][1];
			d[i+1][26]=d[i][0];
			d[i+1][27]=d[i][1];
            for(j=0;j<26;j++)   
			{
				c[i+1][j]=c[i][j+2];
				d[i+1][j]=d[i][j+2];
			}
			for(m=0;m<28;m++)
			{
				cd[m]=c[i+1][m];  
				cd[m+28]=d[i+1][m];
			}
		
			for(n=0;n<48;n++)  
			{
				mid=pc2[n]-1;
				k[i][n]=cd[mid];
			}
		}
	}


	//88888888888888888888888888888888888888888888888888
	//**************************************************





    char data[8];  
	int databit[64]; 
	int ip[64]={   58, 50, 42, 34, 26, 18, 10,  2,
                   60, 52, 44, 36, 28, 20, 12,  4,
                   62, 54, 46, 38, 30, 22, 14,  6,
                   64, 56, 48, 40, 32, 24, 16,  8,
                   57, 49, 41, 33, 25, 17,  9,  1,
                   59, 51, 43, 35, 27, 19, 11,  3,
                   61, 53, 45, 37, 29, 21, 13,  5,
                   63, 55, 47, 39, 31, 23, 15,  7};
	               
	unsigned short e[48]={   32,  1,  2,  3,  4,  5,
							  4,  5,  6,  7,  8,  9,
							  8,  9, 10, 11, 12, 13,
							 12, 13, 14, 15, 16,  17,
							 16, 17, 18, 19, 20,  21,
							 20, 21, 22, 23, 24,  25,
							 24, 25, 26, 27, 28,  29,
							 28, 29, 30, 31, 32,  1}; 
	unsigned short er[16][48]; 
	unsigned short erk[48]; 
	unsigned short b[8][6];  
	              
	unsigned short sb[8]; 
	unsigned short sbbit[32]; 

	unsigned short l[17][32],r[17][32]; 
	                                 

	

	unsigned short s[8][4][16] = {         
        14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7,
         0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8,
         4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0,
        15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13,
 
        15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10,
         3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5,
         0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15,
        13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9,
 
        10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1,
        13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7,
         1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12,
 
         7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15,
        13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9,
        10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4,
         3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14,
 
         2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9,
        14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6,
         4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14,
        11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3,
 
        12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11,
        10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8,
         9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6,
         4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13,
 
         4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1,
        13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6,
         1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2,
         6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12,
 
        13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7,
         1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2,
         7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8,
         2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11,
	};

	unsigned short p[32]={ 16,7,20,21,      
		        29,12,28,17,
				1, 15,23,26,
				5,18,31,10,
				2, 8, 24,14,
				32,27,3, 9,
				19,13,30,6,
				22,11,4,25};

	unsigned short fp[64]={ 40,8,48,16,56,24,64,32,  
							39,7,47,15,55,23,63,31,
							38,6,46,14,54,22,62,30,
							37,5,45,13,53,21,61,29,
							36,4,44,12,52,20,60,28,
							35,3,43,11,51,19,59,27,
							34,2,42,10,50,18,58,26,
							33,1,41,9,49,17,57,25   };

   
	unsigned short rl[64]; 
	unsigned short entext[64];


	int m16,m2345; 
	if(nFlags == 0)
	{
		memcpy(data,pIn,8);
	


		for(i=0;i<8;i++)
			for(j=0;j<8;j++)
			{
				databit[8*i+j]=getbit(data[i],(j+9));
			}

	
		for(i=0;i<32;i++)
		{
			l[0][i]=databit[ip[i]-1];
			r[0][i]=databit[ip[i+32]-1];
		}


	
		for(i=0;i<16;i++)
		{
			for(j=0;j<48;j++)
			{
				er[i][j]=r[i][e[j]-1];  
				erk[j]=er[i][j]^k[i][j]; 
			}
			for(m=0;m<8;m++)
			{
				for(n=0;n<6;n++)
					b[m][n]=erk[m*6+n];  
				
				m16=2*b[m][0]+b[m][5];  
				m2345=2*2*2*b[m][1]+2*2*b[m][2]+2*b[m][3]+b[m][4];
							 
				sb[m]=s[m][m16][m2345];  
				for(u=0;u<4;u++)
					sbbit[m*4+u]=getbit(sb[m],(u+13));
					
			}
			for(q=0;q<32;q++)
			{
				r[i+1][q]=l[i][q]^sbbit[p[q]-1]; 
				l[i+1][q]=r[i][q];
			}
		}


		for(i=0;i<32;i++)
		{
			rl[i]=r[16][i];
			rl[i+32]=l[16][i];
		}
		
		for(i=0;i<64;i++)
			entext[i]=rl[fp[i]-1];

		for(i=0;i<8;i++)
		{
			BYTE b =0;
			int nEach = 1;
			for(int k=0;k<8;k++)
			{
				b+=entext[i*8+7-k]*nEach;
				nEach*=2;
			}
			pOut[i]=b;
		}

		return true;
	}
	else
	{


//88888888888888888888888888888888888888888888888888888888888
//***********************************************************


	memcpy(data,pIn,8);
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		{
			entext[8*i+j]=getbit(data[i],(j+9));
		}

	for(i=0;i<64;i++)
		rl[fp[i]-1]=entext[i];

	for(i=0;i<32;i++)
	{
		r[16][i]=rl[i];
		l[16][i]=rl[i+32];
	}
	


	for(i=15;i>=0;i--)
	{
		for(j=0;j<48;j++)
		{
			er[i][j]=l[i+1][e[j]-1];   
		    erk[j]=er[i][j]^k[i][j]; 
		}
		for(m=0;m<8;m++)
		{
			for(n=0;n<6;n++)
				b[m][n]=erk[m*6+n];  
			
			m16=2*b[m][0]+b[m][5];  
			m2345=2*2*2*b[m][1]+2*2*b[m][2]+2*b[m][3]+b[m][4];
			               
			sb[m]=s[m][m16][m2345];  
			for(u=0;u<4;u++)
				sbbit[m*4+u]=getbit(sb[m],(u+13));
			
		}
		for(q=0;q<32;q++)
		{
			l[i][q]=r[i+1][q]^sbbit[p[q]-1];  
			r[i][q]=l[i+1][q];
		}
	}

    

	unsigned short lr[64]; 

	unsigned short detext[64];  
	for(i=0;i<32;i++)
	{
		lr[i]=l[0][i];
		lr[i+32]=r[0][i];
	}
	
	for(i=0;i<64;i++)
	{
		detext[i]=lr[fp[i]-1];   
	}
	
	
	for(i=0;i<8;i++)
	{
		BYTE b =0;
		int nEach = 1;
		for(int k=0;k<8;k++)
		{
			b+=detext[i*8+7-k]*nEach;
			nEach*=2;
		}
		pOut[i]=b;
	}
	}
	return true;

	}
	catch(...)
	{
		AfxMessageBox("error on secret");
		return false;
	}

}
