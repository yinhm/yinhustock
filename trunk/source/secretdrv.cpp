
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "secretdrv.h"

int T1[]={6,12,5,9,10,0,13,8,15,3,14,4,2,11,1,7};
int T2[]={5,14,12,9,11,2,0,15,7,3,4,13,1,6,10,8};
char T3[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


CSecretDrv::CSecretDrv()
{
}
void CSecretDrv::secret(char desecret[20],char source[10],int key) 
{
	char A1[100],A2[100];
	int i=0,x,y,index=0;
	int length=strlen(source);
	while(i<length*2)
	{
		A1[i]=source[index];
		A1[i]=A1[i]^key;
		x=A1[i]/16;y=A1[i]%16;
		A2[i++]=T3[T1[x]];
		A2[i++]=T3[T1[y]];
		index++;
	}
	A2[i]='\0';
	strcpy(desecret,A2);
}
void CSecretDrv::desecret(char source[10],char desecret[20],int key)
{
	char A1[100],A2[100];
	int x,y,i=0,p,q,index=0;
	int length=strlen(desecret);
	while(i<length/2)
	{
		A1[i]=desecret[index];
		p=0;
		while((p<16)&&A1[i]!=T3[p++]);
		index++;
		A2[i]=desecret[index];
		q=0;
		while((q<16)&&A2[i]!=T3[q++]);
		A1[i]=T2[p-1]*16+T2[q-1];
		A1[i]=A1[i]^key;
		i++;
		index++;
	}
    A1[i]='\0';
	strcpy(source,A1);
}