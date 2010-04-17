//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StructTaiShares.h"
#include "CFormularComputeParent.h"
#include "CTaiShanApp.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineTransferKline.h"
#include "TechNnetBP.h"
#include "CTaiKlineMoveChenben.h"
#include "CTaiKlineDlgHistorySelect.h"
#include "XgnExtDataDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define StrToLong StrToInt

CMapStringToPtr CFormularComputeParent::m_LetterTable;
CMap<int,int,int,int> CFormularComputeParent::m_ParameterTable;
CMapStringToPtr CFormularComputeParent::m_WordTable;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFormularComputeParent::CFormularComputeParent()
{
	m_bRightbox = false;
	m_MaxPoint =0;
	m_bShowColor = false;
	m_stkKind=0;
	m_nCurrentFoot = 0;
}

CFormularComputeParent::CFormularComputeParent(int pmin,int pmax,Kline* pp,CString& str)
{ 
	m_bShowColor = false;
	m_stkKind = 0;
	InitEquation( pmin,  pmax, pp, str);
}


CFormularComputeParent::CFormularComputeParent(int pmin,int pmax,Kline* pp,CFormularContent* pIndex,CString symbol,int stkKind,CBuySellList* pBuySellList,int nKindKline,bool bAddBuySell)
//子窗口框架

{ 
	m_stkKind = stkKind;
	m_bShowColor = false;
	CString str = pIndex->fomular ;

	{
	}
	str.MakeLower ();
	InitEquation( pmin,  pmax, pp, str);
	if(pp == NULL)
	{
		m_MinPoint = 0;
		m_MaxPoint = pBuySellList->GetCount ();
	}

	m_pIndex = pIndex;
	m_symbol =symbol;
	m_pBuySellList = pBuySellList;
	m_nKindKline =nKindKline;
	pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,stkKind);
	m_symbol.ReleaseBuffer();
}

CFormularComputeParent::~CFormularComputeParent()

{
	if(m_pKindPos) delete [] m_pKindPos;

	while(m_table.pop_it()){};
	

	if(	m_bCreateBsList == true )
	{
		CTaiKlineFileHS::RemoveHs(*m_pBuySellList);
		delete m_pBuySellList;
	}

	if(m_pKlineIndex!=NULL)
		delete [] m_pKlineIndex;

	m_bTotalData=false;

	for(int i=0;i<m_RlineNum;i++)
		delete[]  m_Rline[i].line;
	for(int i=0;i<m_VarNum;i++)

		delete[]  m_VarArray[i].line;
	for(int i=0;i<m_MidNum;i++)
		delete[]  m_MidArray[i].line;
	m_exec     =0;

	if(m_pKlineEx != NULL) delete [] m_pKlineEx;

}
int CFormularComputeParent::CheckWord(CString eqm)
{
	if(m_bShowColor == true)
	{
		if(m_pKindPos == NULL) m_pKindPos = new KindPos[MAXB_ALL*4];
		if(m_pKindPos == NULL) return 0;

	}

	CString temp;
	int Bm =0;
	double x;
	char ch,chs;
	char chl[20],*chj;
	int  cl,hj,j,cls,fl;
	int  ck;
	int  mj=eqm.GetLength();
	int rtn;//lmb
	int i;
	for(i=0;i<mj;i++)
	{
		if(m_pKindPos != NULL)
		{
			m_pKindPos[m_nSizeKindPos].m_nPos = i+m_nPosPre;
		}

		ch=eqm.GetAt(i);
		cl=GetCharClass(ch);
		switch(cl)
		{
		case 1:
			m_errmsg="非法字符";
			return i;
			break;
		case 2:
			break;
		case 3:
			m_Post[Bm] = i;
			fl=0;
			if(ch=='.')
			{
				fl = 1;

			}
			for(ck=0;ck<20;ck++)	chl[ck] = ' ';
			j=i;
			chl[0]=ch;
			while(++j<mj)
			{
				chs=eqm.GetAt(j);
				if (chs=='.') fl=1;
				cls=GetCharClass(chs);
				if(cls!=3)  break;
				chl[j-i]=chs;				
			};
			i=--j;
			if(fl==1)
			{
				if(m_floatNum <= CEFE-CEFB)
				{
					x = strtod(chl, &chj);
					m_floatArray[m_floatNum]=(float)x;
					m_table.push_b(m_floatNum+CEFB);
					m_floatNum++;
					Bm++;
				}
				else
				{
					m_errmsg="浮点数个数超出";
					return i;
				}
			}
			else
			{
				if(m_intNum <= CEIE -CEIB)
				{
					x = strtod(chl, &chj);			
					m_intArray[m_intNum]=(int)x;
					m_table.push_b(m_intNum+CEIB);
					m_intNum++;
					Bm++;
				}
				else
				{
					m_errmsg="整数个数超出";
					return i;
				}
			}
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 0;
			}

			break;
		case 4:
			m_Post[Bm] = i;
			temp.Empty();
			temp.Insert(0,ch);
			j=i;
			while(++j<mj)
			{
				chs=eqm.GetAt(j);
				cls=GetCharClass(chs);
				if(cls!=3 && cls!=4) 
					break;
				temp.Insert(j-i,chs);				
			};
			
			if(!LookupWordTable(temp,hj))
			{

				while(j<mj&&eqm.GetAt(j)==' ')
					j++;

				if(j<mj)
				{
					if(Bm==0  && eqm.GetAt(j) == ':') 
					{
						if(temp.GetLength()>36)
						{
							m_errmsg="中间变量名不能大于36位";
							return i;
						}
						if(j == mj-1)
						{
							m_errmsg="结束字符为非法字符！";
							return i;
						}
						else
						{
							if(eqm.GetAt(j+1) == '=')
							{
								if(m_VarNum <= MIDE-MIDB)
								{
									m_WordTableNew[temp]=(void*)(MIDB+m_VarNum);
									m_VarArray[m_VarNum].line = new float[m_MaxPoint];
									m_table.push_b(MIDB+m_VarNum);
									m_VarName[m_VarNum] = temp;					
									m_VarNum++;
									Bm++;
								}
								else
								{
									m_errmsg="中间变量个数超出";
									return i;
								}
							}
							else
							{
								if(m_RlineNum <= RLINEE-RLINEB)
								{
									m_WordTableNew[temp]=(void*)(RLINEB+m_RlineNum);
									m_Rline[m_RlineNum].line = new float[m_MaxPoint];
									m_table.push_b(RLINEB+m_RlineNum);
									m_RlineName[m_RlineNum] = temp;
									m_RlineNum++;
									Bm++;
								}
								else
								{
									m_errmsg="返回变量个数超出";
									return i;
								}
							}
						}
					}
					else
					{
						if(temp.GetLength () == 11 && temp.Left(5) == "color")
						{
							m_table.push_b(m_strColorNum+CONSCOLORB);
							CString s2 = temp;
							s2.Delete (0,5);
							m_strColor[m_strColorNum] = s2;
							m_strColorNum++;
							Bm++;
							i=j;
							if(m_pKindPos != NULL)
							{
								m_pKindPos[m_nSizeKindPos].m_nKind = 4;
							}
						}
						else
						{
							m_errmsg="非法变量";
							return i;
						}
				}
				}
				else
				{
						if(temp.GetLength () == 11 && temp.Left(5) == "color")
						{
							m_table.push_b(m_strColorNum+CONSCOLORB);
							CString s2 = temp;
							s2.Delete (0,5);
							m_strColor[m_strColorNum] = s2;
							m_strColorNum++;
							Bm++;
							i=j;
							if(m_pKindPos != NULL)
							{
								m_pKindPos[m_nSizeKindPos].m_nKind = 4;
							}
						}
						else
						{
							m_errmsg="非法变量";
							return i;
						}
				}
				if(m_pKindPos != NULL)
				{
					m_pKindPos[m_nSizeKindPos].m_nKind = 2;
				}
				
			}
			else
			{
				while(j<mj&&eqm.GetAt(j)==' ')
					j++;

				if(j<mj && Bm==0  && eqm.GetAt(j) == ':') 
				{
					if(hj>=MIDB)
					{
						m_errmsg="变量重复定义或与参数同名";
						return i;
					}
				}

				if(hj==8034 || hj == 8046 
					|| hj>=FuncDllB+21 && hj<=FuncDllB+24 
					|| hj>=FuncDllB && hj<=FuncDllB+2
					|| hj==FuncDllB+5 || hj==FuncDllB+6
					|| hj>=FuncDllB+10 && hj<=FuncDllB+14
					|| hj == FuncDllB+34
					|| hj == FuncDllB+4)
					m_bTotalData=true;

				

				m_table.push_b(hj);
				Bm++;
				if(m_pKindPos != NULL)
				{
					m_pKindPos[m_nSizeKindPos].m_nKind = 3;
				}
			}
			i=--j;
			break;
		case 5:
			m_Post[Bm] = i;
			switch(ch)
			{
			case '>':
			case '<':
			case '=':
			case '!':
			case ':':
				if(i==eqm.GetLength()-1)
				{
					m_errmsg = "非法结束 ";
					return i;

				}
				chs=eqm.GetAt(i+1);
				if(chs=='=') 
				{
				//	yhh add 995629 
					if(ch=='=')
					{
						m_Post[Bm] = i+1;
						m_errmsg = "非法运算符";
						return i;
					}

					if(ch==':')
					{
						int pre;
						pre=m_table.pop_it();
						if(pre>=MIDB&&pre<=MIDB+m_VarNum)
							m_table.push_b(pre);
						else
						{
							if(pre>=FuncB&&pre<=PAM)
								m_errmsg ="中间变量不能用系统保留字";
							else
								m_errmsg ="中间变量不合法";
							return i;
						}

					}
				//yhh add end 
					temp.Empty();
					i++;
					if(i==mj) 
					{	
						m_errmsg = "非法结束 ";
						return i;
					}
					temp.Insert(0,ch);
					temp.Insert(1,chs);
					LookupWordTable(temp,hj);
					m_table.push_b(hj);
					Bm++;
				}
				else if(chs=='<' ||chs=='>') 
				{
					if(ch!='>' && chs=='<' || ch!='<' &&chs=='>')
					{
						m_Post[Bm] = i+1;
						m_errmsg = "非法运算符";
						return i;
					}

					temp.Empty();
					i++;
					if(i==mj) 
					{	
						m_errmsg = "非法结束 ";
						return i;
					}
					temp.Insert(0,ch);
					temp.Insert(1,chs);
					LookupWordTable(temp,hj);
					m_table.push_b(hj);
					Bm++;
				}
				else
				{
					if(ch=='!')  
					{	
						m_errmsg = "非法结束 ";
						return i;
					}
					else
					{
						//	yhh add 995629 
						if(ch==':')
						{
							int pre;
							pre=m_table.pop_it();
							if(pre>=RLINEB&&pre<=RLINEB+m_RlineNum)
								m_table.push_b(pre);
							else
							{
								if(pre>=FuncB&&pre<=PAM)
									m_errmsg ="中间变量不能用系统保留字";
								else
									m_errmsg ="中间变量不合法";
								return i;
							}

						}
					// yhh add 995629 end
						temp.Empty();
						temp.Insert(0,ch);
						LookupWordTable(temp,hj);
						m_table.push_b(hj);
						Bm++;
					}
				}
				break;
			case '-':
			case '+':
				rtn=m_table.pop_it();
				if( rtn==0 || (rtn==9001) || (rtn==9005)
					|| (rtn>=1001&&rtn<=4008))//lmb6.13
				{
					if(rtn>0)
						m_table.push_b(rtn);
					m_table.push_b(TOZERO);
					Bm++;
				}
				else if( (rtn==6001 || rtn==6002) && ch == '-' )//lmb6.13
				{
					temp.Empty();
					if(rtn == 6001)
						temp = "*-";
					else
						temp = "/-";
					if(!LookupWordTable(temp,hj))
					{
						m_errmsg="非法字符";
						return i;
					}
					m_table.push_b(hj);
					break;
				}
				else
				{
					m_table.push_b(rtn);
				}
			default:
					temp.Empty();
					temp.Insert(0,ch);
					if(!LookupWordTable(temp,hj))
					{
						m_errmsg="非法字符";
						return i;
					}
					m_table.push_b(hj);
					Bm++;
					break;
			}
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 1;
			}

			break;	
		case 6:
		
			m_Post[Bm] = i+1;
			m_QuoteArray[m_QuoteNum].Empty();
			j=i;
			fl=0;
			while(++j<mj)
			{
				chs=eqm.GetAt(j);
				cls=GetCharClass(chs);
				if(cls>1 && cls<6)
				{
					m_QuoteArray[m_QuoteNum].Insert(j-i,chs);
					continue;
				}
				if(cls==6)
				{
					fl=1;
					break;
				}
				if(cls==1)
				{
					m_errmsg="非法字符";
					return i;
				}
			};
			if(fl==0)
			{
				m_errmsg="引用无匹配符";
				return i;
			}

			m_table.push_b(m_QuoteNum+QUB);
			m_QuoteNum++;
			Bm++;
			i=j;
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 4;
			}
			break;
		case 7:
		
			m_Post[Bm] = i+1;
			m_stringArray[m_stringNum].Empty();
			j=i;
			fl=0;
			while(++j<mj)
			{
				chs=eqm.GetAt(j);
				cls=GetCharClass(chs);
				if(cls>1 && cls<6)
				{
					m_stringArray[m_stringNum].Insert(j-i,chs);
					continue;
				}
				if(cls==7)
				{
					fl=1;
					break;
				}
				if(cls==1)
				{
					m_errmsg="非法字符";
					return i;
				}
			};
			if(fl==0)
			{
				m_errmsg="无匹配符";
				return i;
			}

			m_table.push_b(m_stringNum+CECB);
			m_stringNum++;
			Bm++;
			i=j;
			if(m_pKindPos != NULL)
			{
				m_pKindPos[m_nSizeKindPos].m_nKind = 4;
			}
			break;
		}

		m_nSizeKindPos++;
	}

	m_SentNum=-1;
	do 
	{	m_SentNum++;
		m_Sent[m_SentNum]=m_table.rec_it();
		i=m_Sent[m_SentNum];
	}while(i);
	m_SentNum--;

	IsDaPan();
//yhh add
	FindLineType()  ;
	return -1;
}

int CFormularComputeParent::GetCharClass(char ch)
{   //子窗口框架
	int hj;
	CString tj(ch);

    if(m_LetterTable.Lookup(tj,(void *&)hj)) 
		return hj;
	else 
	{

			return 4;
	}
}
int CFormularComputeParent::YhhCheck(int j, int k)
{
	int tj=0,tu=0;
	int k0=k;
	int i;
  for(i=j;i<=m_SentNum;i++)
	{
		if(tu<0) 
		{
			m_errmsg="缺少参数";    
			return i;
		}
		if(tu>1) 
		{
			m_errmsg="缺少运算符";    
			return i;
		}

		tj=m_Sent[i];
		if(tj==9001)
		{
			k++;   continue;
		}
		if(tj==9002) 
		{
			k--;

     		if((k==0||k==k0) && tu!=1)      
			{
				m_errmsg="非法运算";             
				return i;
			}
				continue;
		}
		if(tj==9005)           
		{
			if(tu!=1) 
			{
				m_errmsg="非法字符";
				return i;
			}
 			return -1;          
		}
		if( (tj>FuncB && tj<=FuncE))     
		{
			if(tj>=QUB&&tj<=FuncE&&i<=m_SentNum)
			{
				if(m_Sent[i+1]!=9001)
					tu+=1;
			}
			continue;
		}
   	   if(tj<FuncB)
			tu-=1;
		if(tj>CONSE||tj==TOZERO)
			tu+=1;
		
	}
	if(tu!=1 && j!=m_SentNum)
	{
		if(tu>1)
			m_errmsg="缺少运算符";
		else
			m_errmsg="缺少参数";

		return i-1;
	}
	else 
		return -1;

}

int CFormularComputeParent::CheckSent()
{//子窗口框架
	int i=0,k=0;
	int tj=0,tu=0;              
	CString str,cname,cline;

	int start;
	for(start=0;start<=m_SentNum;start++)
	{
		tj=m_Sent[start];
		if(tj==9001)
			k++;
		else if(tj==9002)
		{
			k--;
			if(k<0)
			{
				m_errmsg="括号不匹配,多)";
				return start;
			}
		}
	}
	if(start>=m_SentNum&&k>0)
	{
		m_errmsg="括号不匹配,多(";
		return start;	
	}
	int check=YhhCheck(0,k);
	if(check!=-1)
		return check;
	for(i=0;i<=m_SentNum;i++)
	{	tj=m_Sent[i];
		int m;

		if( tj==9005) 
		{
			int check=YhhCheck(i+1,k);
			if(check==-1) continue;
			else 
				return check;
		}

	                        	
		if(tj==9001 ) 
		{
		   k++;
		  int check=YhhCheck(i+1,k);
		  if(check==-1) continue;
		  else 
				return check;

		}
		if(tj==9002)
		{
		k--;
		}
					
		if(tj>FuncB && tj<=FuncE)
		{
		
				
			if(tj>=QUB)            
			{
				str =m_QuoteArray[tj-QUB];
				int nKindE;
				int rtn= GetQuote(str,cname,cline,tu,nKindE,nKindE);
				if(rtn==1)
				{
					m_errmsg="引用的指标不存在";
					return i;	
				}

				if(rtn==2)
				{
					m_errmsg="引用的指标的子项不存在";
					return i;	
				}

				m_ParameterTableNew[tj] = tu;
			

			}
			else
			{
					if(!LookupParameterTable(tj,tu)) 
					tu=1;
			}

					
			 int   kk=0;
			 if(m_Sent[i+1]!=9001) 
			 {
				 if(tj<QUB)
				 {
					m_errmsg="缺括号";
					return i;
				 }
				 //lmb 2000.3.8
				 else
				 {
					 m_ParameterTableNew[tj] = 0;
					 tu =0;
					 continue;
				 }
			 }
			 else 
					kk++;
			 m=i+2;   
			 do{   
				tj=m_Sent[m];
				if(tj==9005 && kk==1) 
					tu-=1;
				if(tj==9001) {kk+=1;} 
				if(tj==9002)         
				{
					kk-=1;
					if(kk==0 )   
					{
						if(tu==1)	break;  
						else
						{
							m_errmsg="参数个数有误";
							return i;
						}
					}
				}
				m++;
			 
			 }while(m<=m_SentNum);
		
		}          
		                           
		
	}
	return -1;
							
		

}

int CFormularComputeParent::Sort(int bi,int ej)
{//子窗口框架
	int tj,tu;
	int lb,le;
	int i=0,j=0,k;
	i=0;

	//lmb6.13
	if(bi>ej)
		return 0;


	while(m_Sent[bi]==9001 && m_Sent[ej]==9002)
	{
		i=bi+1;
		k=0;
		do{
			if(m_Sent[i]==9001)  k++;
			if(m_Sent[i]==9002)  k--;
			if(k<0)  break;
		}while(++i<ej);

		if(k==0)
		{
			bi++;
			ej--;  
			if(bi>=ej)  
			{
				if(bi==ej) m_table.push_b(m_Sent[bi]);
				else ASSERT(FALSE);
				return 0;
			}
			else
				continue;
		}
		else
			break;
	}


	j=0; 
	tj = 10;

	for(k=bi;k<=ej;k++)
	{
		if(m_Sent[k]==9001) {j++;continue;}
		if(m_Sent[k]==9002) j--;
		if(j==0)
		{
			tu=m_Sent[k]/1000;
			if(tu<=tj) {tj=tu; i=k;}
		}
	}
	//	lmb6.13
	if(bi==ej)
	{
		tj=m_Sent[bi];
		m_table.push_b(tj);
		return 0;
	}

    
	tj=m_Sent[i];

	if(tj>FuncB && tj<FuncE) 
	{
		m_table.push_b(tj);
		if(tj>=QUB)
		{

			tu=GetNum(tj);
		}
		else
		{
			if(!LookupParameterTable(tj,tu)) tu=1;
		}
		lb=i+2;

		if(tu>0)
		{
			while(tu--)
			{
				k=0;
				le=lb;
				while(1)
				{
					if(m_Sent[le]==9001) k++;
					if(m_Sent[le]==9002) k--;
					if(k<0 || (k==0 && m_Sent[le] ==9005)) 
					{
						le--;
						break;
					}
					le++;
				}
				if(lb==le)
					m_table.push_b(m_Sent[lb]);
				else
					Sort(lb,le);
				lb=le+2;
			}
		}
	}

	//---
	if(tj<FuncB)
	{
		m_table.push_b(tj);
		lb=bi;
		le=i-1;
		if(lb==le)
			m_table.push_b(m_Sent[lb]);
		else
			Sort(lb,le);
		lb=i+1;
		le=ej;
		if(lb==le)
			m_table.push_b(m_Sent[lb]);
		else
			Sort(lb,le);
	}
	return 0;
}


void CFormularComputeParent::Devide()
{
	int i,tu;
	int lb=0,le=0;
	int Rvdev,el;
	char ch;
	CString ts;
	//lmb 20010907
	m_nPosPre = 0;


	if (m_MaxPoint==0) 
		return;
	tu=m_formuCompute.GetLength();
	m_ComSortNum = -1;
	m_errpos = -1;
	m_tempWhich=-1;
	for(i=0;i<tu;i++)
	{
		ch=m_formuCompute.GetAt(i);
		ts.Insert(i-lb,ch);
		if(ch==';' || i==tu-1)
		{
			if(i-lb==0)
			{
				m_errmsg="非法字符";
				m_errpos= i;
				break;
			}
			if(ch==';')	 ts.Delete(i-lb,1);
			Rvdev = CheckWord(ts);
			if(Rvdev != -1)
			{
				m_errpos= Rvdev+lb;
				break;
			}
			Rvdev = CheckSent();

		
			if(Rvdev != -1)
			{
				m_errpos=m_Post[Rvdev]+lb;
				break;
			}



			Rvdev = Sort(0,m_SentNum);
			do {
				m_ComSortNum++;
				m_ComSort[m_ComSortNum]=m_table.pop_it();
				el=m_ComSort[m_ComSortNum];
			}while(el);
			m_ComSortNum--;
			ts.Empty();
			lb=i+1;

			//lmb 20010907
			m_nPosPre = i+1;
		}
	}

	for(i=0;i<this->m_RlineNum ;i++)
		if(GetPeriodKind(this->m_RlineName[i])!=0)
		{
			m_errpos = 0;
			m_errmsg="返回变量名称不能用系统关键字";
			break;
		}
}


int CFormularComputeParent::AddPara(CString pa,float va)
{//子窗口框架
	if(m_NumGs >=PRAMANUM)
		return 1;
//yhh add 990702
	int hj;
	if(LookupWordTable(pa,hj))
		return 2;
		
//yhh add 990702 end
	m_WordTableNew[pa] =(void *)(m_floatNum+CEFB);
	m_floatArray[m_floatNum]=va;
	m_NumName[m_NumGs] = pa;
	m_NumGs++;
	m_floatNum++;
	return 0;
}

int CFormularComputeParent::AddPara(CString pa, int va)
{
	if(m_NumGs >=PRAMANUM)
		return 1;
//yhh add 990702
	int hj;
	if(LookupWordTable(pa,hj))
		return 2;
		
//yhh add 990702 end
	m_WordTableNew[pa] =(void *)(m_intNum+CEIB);
	m_intArray[m_intNum]=va;
	m_NumName[m_NumGs] = pa;
	m_NumGs++;
	m_intNum++;
	return 0;
}


void CFormularComputeParent::GetData(int cl,ARRAY_BE& pp)
{
	pp.b = m_MinPoint; 
	pp.e = m_MaxPoint-1;
	int i;
	float ltp = 0;

	switch(cl)
	{
	case TOZERO:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			 pp.line[i] =0;
		break;
	case 9521:
		if(m_pData!=NULL)
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		   pp.line[i] = m_pData[i].open;
		else GetHsKlineData(cl,pp);
		break;
	case 9522:
		if(m_pData!=NULL)
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = m_pData[i].close;
		else GetHsKlineData(cl,pp);
		break;
	case 9523:	
		if(m_pData!=NULL)
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = m_pData[i].high;
		else GetHsKlineData(cl,pp);
		break;
	case 9524:	
		if(m_pData!=NULL)
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = m_pData[i].low;
		else GetHsKlineData(cl,pp);
		break;
	case 9525:	
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = (float)m_pData[i].advance;
		break;
	case 9526:	
		CTime *ptime;
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)ptime->GetDayOfWeek()-1;
			delete ptime;
		}
		break;
	case 9527:	
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetDay());
			delete ptime;
		}
		break;
	case 9528:	
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetMonth());
			delete ptime;
		}
		break;
	case 9529:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetYear());
			delete ptime;
		}
		break;
	case 9530: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetHour());
			delete ptime;
		}
		break;
	case 9531: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetMinute());
			delete ptime;
		}
		break;
	case 9560: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			pp.line[i] = (float)(ptime->GetMinute()*100+ptime->GetHour()*10000+ptime->GetSecond ());
			delete ptime;
		}
		break;
	case 9561: 	
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			time_t tm = m_pData[i].day;
			ptime = new CTime(tm);
			int nYear = ptime->GetYear();
			if(nYear>1999)
				nYear = 100+nYear%100;
			else
				nYear = nYear%100;
			pp.line[i] = (float)(ptime->GetMonth()*100+nYear*10000+ptime->GetDay ());
			delete ptime;
		}
		break;
	case 9532: 
		if(m_pData!=NULL)
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = m_pData[i].amount;
		else GetHsKlineData(cl,pp);
		break;
	
	case 9533: 
		if(m_pData!=NULL)
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] = m_pData[i].vol;
		else GetHsKlineData(cl,pp);
		break;
	case 9534: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	
			if(m_pData[i].close > m_pData[i].open)
				pp.line[i] = 1;
			else
				pp.line[i] =0;
		}
		break;
	
	case 9535: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	
			if(m_pData[i].close < m_pData[i].open)
				pp.line[i] = 1;
			else
				pp.line[i] =0;
		}
		break;
	
	case 9536: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{	
			if(m_pData[i].close < m_pData[i].open)
				pp.line[i] = 1;
			else
				pp.line[i] =0;
		}
		break;
	case 9537: 
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pData[i].decline;
		break;
	case 9538: 
		ltp = CTaiShanKlineShowView::GetCapital(this->m_cdat1 );
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =ltp;
		break;
	case 9539: 	
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =CTaiShanKlineShowView::GetCapital(this->m_cdat1 );
		break;
	case 9541:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].open;
		break;
	case 9542:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].close;
		break;
	case 9543:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].high;
		break;
	case 9544:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].low;
		break;
	case 9545:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].vol;
		break;
	case 9546:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].amount;
		break;
	case 9547:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].decline;
		break;
	case 9548:
		if(m_pKlineIndex==NULL)	{if(this->GetKlineIndex ()==NULL) break;}
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) m_pKlineIndex[i].advance;
		break;

	
	case 9550:

		if(this->m_nKindKline == HS_K_LINE)
		{
			if( m_pBuySellList!=NULL)
				if(m_pBuySellList->GetCount() == m_MaxPoint)
				{
					if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
					if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , pp, m_MaxPoint, 0,0,m_pKlineEx);
				}
		}
		else
		{
			if(this->m_cdat1==NULL) pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,m_stkKind);
			m_symbol.ReleaseBuffer();
			PCdat1 cdat1 = m_cdat1;
			if(m_cdat1==NULL) break;
			float f=(float)0 ;
			if(cdat1->nowp <= cdat1->pbuy1 )
				f= cdat1->nowv ;
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] =f;
		}

		break;
	case 9551:

		if(this->m_nKindKline == HS_K_LINE)
		{
			if( m_pBuySellList!=NULL)
				if(m_pBuySellList->GetCount() == m_MaxPoint)
				{
					if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
					if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , pp, m_MaxPoint, 1,0,m_pKlineEx);
				}
		}
		else
		{
			if(this->m_cdat1==NULL) pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,m_stkKind);
			m_symbol.ReleaseBuffer();
			PCdat1 cdat1 = m_cdat1;
			if(m_cdat1==NULL) break;
			float f=(float)0 ;
			if(cdat1->nowp >= cdat1->psel1 )
				f= cdat1->nowv ;
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] =f;
		}

		break;
	case 9552:
		if(this->m_nKindKline == HS_K_LINE)
		{
			if( m_pBuySellList!=NULL)
				if(m_pBuySellList->GetCount() == m_MaxPoint)
				{
					if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
					if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , pp, m_MaxPoint, 2,0,m_pKlineEx);
				}
		}
		else
		{
			if(this->m_cdat1==NULL) pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,m_stkKind);
			m_symbol.ReleaseBuffer();
			PCdat1 cdat1 = m_cdat1;
			if(m_cdat1==NULL) break;
			float f=(float)1 ;
			if(cdat1->nowp >= cdat1->psel1 )
				f= 0 ;
			for(i=m_MinPoint;i<m_MaxPoint;i++)
				pp.line[i] =f;
		}

		break;
	case 9553:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =(float) ((KlineGeneral*)(m_pData+i))->volPositive ;
		break;
	case 9554:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =m_pData[i].vol - (float) ((KlineGeneral*)(m_pData+i))->volPositive ;
		break;
	case 9580:
		for(i=m_MinPoint;i<m_MaxPoint;i++)
			pp.line[i] =100;
		break;
	default:
		if( cl>=MIDB && cl<=MIDE )
		{

			pp = m_VarArray[cl-MIDB];
			break;
		}
		if(cl>=RLINEB && cl<=RLINEE)
		{
			pp =m_Rline[cl-RLINEB];

			break;
		}
		if(cl>=CEFB && cl<=CEFE)
		{
			for(i=m_MinPoint;i<m_MaxPoint;i++) 
				pp.line[i] =m_floatArray[cl-CEFB];
			break;
		}
		if(cl>=CEIB && cl<=CEIE)
		{
			for(i=m_MinPoint;i<m_MaxPoint;i++) 
				pp.line[i] =(float)m_intArray[cl-CEIB];
			break;
		}
		if(cl>=PCMB && cl<=PCME)
		{
			pp =m_MidArray[cl-PCMB];

			delete[] m_MidArray[cl-PCMB].line;
			m_MidNum--;
			break;
		}

		if(cl>=QUB && cl <=QUE)
		{
			if(Relolve(cl)==0)
			{
		
				pp =m_MidArray[m_MidNum-1];

				delete[] m_MidArray[m_MidNum-1].line;
				m_MidNum--;
			}
			break;
		}
		if(cl>=CECB && cl <CECE)
		{
			{
				pp.s = this->m_stringArray [cl-CECB];
				pp.kind = ARRAY_BE::StringData;
			}
			break;
		}
		break;
	}
}

int CFormularComputeParent::FormularCompute()
{	


#ifndef _DEBUG
	try
	{
#endif

	if(	m_exec  == 0)
			m_exec     =1;
	else
	{
		m_errmsg = "公式重新调用必须初始化--SETNEW";
		return -100;
	}

	Devide();

	if(m_errpos!=-1) 
		return -100;
	
	int cs,msg=0;
	

	if(ARRAY_Right_Box::m_nCount>0)
	{
		bool bDay = true;
		if(this->m_nKindKline  <DAY_KLINE || m_nKindKline==HS_KLINE || m_nKindKline==MINUTE_1_K_LINE)
			bDay = false;
		int nCurr = CTaiKlineFileKLine::RegularTime(ARRAY_Right_Box::m_timeCompute.GetTime(),bDay);
		for(int i=0;i<m_MaxPoint ;i++)
		{
			if(CTaiKlineFileKLine::RegularTime(this->m_pData[i].day,bDay)>=nCurr)
			{
				m_nCurrentFoot = i;
				break;
			}
		}
	}

	for(int i=0;i<=m_ComSortNum;i++)
	{
		if(msg!=0)
		{
			m_errmsg = "运行错误";
			if(msg!=999) 
				return msg;
		}
		cs = m_ComSort[i];
		if(cs>=TOZERO)     
		{
			m_table.push_b(cs);
			continue;
		}
		if(cs>=QUB && cs<=QUE) 
		{
			msg = Relolve(cs);
			continue;
		}
		if(cs>=FuncDllB && cs<=FuncDllE)
		{
			msg = FuncFromDll(cs);
			continue;
		}
		if(cs>=CECB && cs<CECE)
		{
			m_table.push_b(cs);
			continue;
		}
		switch(cs)
		{
		case 1001:    //==  :   ==//
			msg=Func101();
			break;
		case 1002:    //==  :=  ==//
			msg=Func102();
			break;
		case 3001:
			msg=Func301();// and
			break;
		case 2001:
			msg=Func302();// or
			break;
		case 4001:
			msg=Func401();// >
			break;
		case 4002:
			msg=Func402();// <
			break;
		case 4005:
			msg=Func405();// >=
			break;
		case 4006:
			msg=Func406();// <=
			break;
		case 4008:
			msg=Func408();// ==
			break;
		case 4009:
			msg=Func409();// !=
			break;
		case 5001:
			msg=Func501();// +
			break;
		case 5002:
			msg=Func502();// -
			break;
		case 6001:
			msg=Func601();// *
			break;
		case 6002:
			msg=Func602();// /
			break;
		case 6003:
			msg=Func603();// *
			break;
		case 6004:
			msg=Func604();// /
			break;
		case 8001:
			msg=Func801();// /
			break;
		case 8002:
			msg=Func802();// /
			break;
		case 8003:
			msg=Func803();// /
			break;
		case 8004:
			msg=Func804();// /
			break;
		case 8005:
			msg=Func805();// /
			break;
		case 8006:
			msg=Func806();// /
			break;
		case 8007:
			msg=Func807();// /
			break;
		case 8008:
			msg=Func808();// /
			break;
		case 8009:
			msg=Func809();// /
			break;
		case 8010:
			msg=Func810();// /
			break;
		case 8011:
			msg=Func811();// /
			break;
		case 8012:
			msg=Func812();// /
			break;
		case 8013:
			msg=Func813();// /
			break;
		case 8030:
			msg=Func830();// /
			break;
		case 8031:
			msg=Func831();// /
			break;
		case 8032:
			msg=Func832();// /
			break;
		case 8033:
			msg=Func833();// /
			break;
		case 8034:
			msg=Func834();// /
			break;
		case 8035:
			msg=Func835();// /
			break;
		case 8036:
			msg=Func836();// /
			break;
		case 8037:
			msg=Func837();// /
			break;
		case 8038:
			msg=Func838();// /
			break;
		case 8039:
			msg=Func839();// /
			break;
		case 8040:
			msg=Func840();// /
			break;
		case 8041:
			msg=Func841();
			break;
		case 8042:
			msg=Func842();
			break;
		case 8043:
			msg=Func843();
			break;
		case 8044:
			msg=Func844();
			break;
		case 8045:
			msg=Func845();
			break;
		case 8046:
			msg=Func846();
			break;
		case 8047:
			msg=Func847();
			break;
		case 8048:
		case 8049:
		case 8050:
		case 8051:
			msg=Func848_51(3+cs-8048);
			break;
		case 8052:
			msg=Func852();
			break;
		case 8053:
			msg=Func853();
			break;
		default:
			break;
		}
	}


	ARRAY_BE  lt;
	lt.line= new float[m_MaxPoint];

	while(cs=m_table.pop_it())
	{
		if(m_RlineNum > RLINEE-RLINEB)
			break;
		m_Rline[m_RlineNum].line= new float[m_MaxPoint];

		if(cs< PAM)
		{
			GetData(cs,lt);
			m_Rline[m_RlineNum].b=lt.b;
			m_Rline[m_RlineNum].e=lt.e;
			if(m_Rline[m_RlineNum].b>=0 && m_Rline[m_RlineNum].e<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_Rline[m_RlineNum].e)
			for(int i=lt.b;i<=lt.e;i++)
				m_Rline[m_RlineNum].line[i] = lt.line[i];
		}
		else
		{
			m_Rline[m_RlineNum].b=m_MidArray[cs-PCMB].b;
			m_Rline[m_RlineNum].e=m_MidArray[cs-PCMB].e;
			if(m_Rline[m_RlineNum].b>=0 && m_Rline[m_RlineNum].e<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_MaxPoint-1 && m_Rline[m_RlineNum].b<=m_Rline[m_RlineNum].e)
			for(int i=m_MidArray[cs-PCMB].b;i<=m_MidArray[cs-PCMB].e;i++)
				m_Rline[m_RlineNum].line[i] = m_MidArray[cs-PCMB].line[i];
		
		}
		m_RlineNum++;
	}
#ifdef TEST_Version1
	CTime tm = CTime::GetCurrentTime();
	if(tm>CTime(2003,10,01,9,30,0))
	{	
		float ff = rand()%100/1000.;
		for(int i = 0;i<m_RlineNum;i++)
		{
			for(int j = 0;j<m_MaxPoint;j++)
				m_Rline[m_RlineNum].line[j] += ff;
		}
	}
#endif

	delete lt.line;
	return 0;
#ifndef _DEBUG
	}
	catch(...)
	{
	}
	return -100;
#endif

}

int CFormularComputeParent::Func101()
{
	int cl,cr;
	ARRAY_BE fcl;
	fcl.line = new float[m_MaxPoint];
	cr=m_table.pop_it();
	cl=m_table.pop_it();
	GetData(cl,fcl);
	if(fcl.b>fcl.e||fcl.e>m_MaxPoint-1||fcl.b<0)
	{
		m_Rline[cr-RLINEB].b =fcl.b;
		m_Rline[cr-RLINEB].e =fcl.e;
		delete[] fcl.line;
		return 999;
	}
	
	m_Rline[cr-RLINEB] = fcl;
	delete[] fcl.line;

	return 0;
}

int CFormularComputeParent::Func102()
{
	int cl,cr;
	ARRAY_BE fcr;
	fcr.line = new float[m_MaxPoint];
	cl=m_table.pop_it();
	cr=m_table.pop_it();
	GetData(cr,fcr);
	if(fcr.b>fcr.e||fcr.e>m_MaxPoint-1||fcr.b<0)
	{
		m_VarArray[cl-MIDB].b =fcr.b;
		m_VarArray[cl-MIDB].e =fcr.e;
		delete[] fcr.line;
		return 999;
	}
	m_VarArray[cl-MIDB] =fcr;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func301()
{
	int cl,cr;
	int bl,be;
	ARRAY_BE fcl,fcr;

	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 

	bl = m_MidArray[m_MidNum].b;
	be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}


	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] && fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func302()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] || fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func401()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] > fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func402()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] < fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func405()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];

	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] >= fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func406()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] <= fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func408()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] == fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func409()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] != fcr.line[i])
			m_MidArray[m_MidNum].line[i] = 1;
		else 
			m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func501()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Add(fcl,fcr);
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}

int CFormularComputeParent::Func502()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Sub(fcl,fcr);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}

int CFormularComputeParent::Func601()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	cl=m_table.pop_it();
	
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Mult(fcl,fcr);
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}

int CFormularComputeParent::Func602()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Div(fcl,fcr);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func603()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	cl=m_table.pop_it();
	
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Mult(fcl,fcr);
	m_MidArray[m_MidNum] .Mult(m_MidArray[m_MidNum],-1);
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;

}

int CFormularComputeParent::Func604()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	m_MidArray[m_MidNum] .Div(fcl,fcr);
	m_MidArray[m_MidNum] .Div(m_MidArray[m_MidNum],-1);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func801()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
			m_MidArray[m_MidNum].line[i] =
					max(fcl.line[i],fcr.line[i]) ;

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func804()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		m_MidArray[m_MidNum].line[i] 
			=	min(fcl.line[i],fcr.line[i]) ;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func805()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		m_MidArray[m_MidNum].line[i]
				=(float)((int)fcl.line[i]/(int)fcr.line[i]) ;
	}
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func806()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	m_MidArray[m_MidNum].Sub (0.0f,fcl );

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func807()
{
	int cl,cr,ce, ret;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];

	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(ce,fce);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].b =max(m_MidArray[m_MidNum].b ,fce.b); 

	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e);
	m_MidArray[m_MidNum].e =min(m_MidArray[m_MidNum].e,fce.e); 
 
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{	
		ret =(int)fcl.line[i];
		if(ret)
			m_MidArray[m_MidNum].line[i] =fcr.line[i];
		else
			m_MidArray[m_MidNum].line[i] =fce.line[i];
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;
	return 0;
}

int CFormularComputeParent::Func802()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
		m_MidArray[m_MidNum].line[i] 
				=(float)FABSMY(fcl.line[i]) ;
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func808()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] > 0)
			m_MidArray[m_MidNum].line[i] 
				=(float)log(fcl.line[i]);
		else 
		{
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
			delete[] fcl.line;
			return 8;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func809()
{
	int cl,ret;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		ret =(int)fcl.line[i];
		if(ret == 0)
			m_MidArray[m_MidNum].line[i] =1 ;
		else
			m_MidArray[m_MidNum].line[i] =0 ;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func810()
{
	int cl,ret;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		ret =(int)fcl.line[i];
		if(ret == 0) 
			m_MidArray[m_MidNum].line[i] =0 ;
		else
		{
			if(ret <0) 	m_MidArray[m_MidNum].line[i] =-1 ;
			else	    m_MidArray[m_MidNum].line[i] =1 ;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func803()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		m_MidArray[m_MidNum].line[i] 
			=(float)exp(fcl.line[i]);
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func811()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] > 0)
			m_MidArray[m_MidNum].line[i] 
				=(float)log10(fcl.line[i]);
		else
		{
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
			delete[] fcl.line;
			return 11;
		}

	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}


int CFormularComputeParent::Func813()
{
	int cl;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e; 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i] >= 0)
			m_MidArray[m_MidNum].line[i]
				=(float)sqrt(fcl.line[i]);
		else
		{
			if(i== bl)
			{
				m_table.push_b(PCMB+m_MidNum);
				m_MidNum++;
				delete[] fcl.line;
				return 12;
			}
			else
				m_MidArray[m_MidNum].line[i]
					=m_MidArray[m_MidNum].line[i-1];
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	return 0;
}

int CFormularComputeParent::Func812()
{
	int cl,cr;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b); 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e); 
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		try
		{
		m_MidArray[m_MidNum].line[i] 
			= (float)pow(fcl.line[i],fcr.line[i]);
		}
		catch(...)
		{
			if(i!=bl)
				m_MidArray[m_MidNum].line[i] 
					= m_MidArray[m_MidNum].line[i-1] ;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func830()
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	if(ret<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 30;
	}
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,x=0;
		for(j=i-ret+1;j<=i;j++)
				ax+=fcl.line[j];
		ax/=ret;
		for(j=i-ret+1;j<=i;j++)
				x=(float)(FABSMY(fcl.line[j]-ax))+x;
		x/=ret;
		m_MidArray[m_MidNum].line[i] = x;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func833()
 {
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 33;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,x=0;
		for(j=i-ret+1;j<=i;j++)
				ax+=fcl.line[j];
		ax/=ret;
		for(j=i-ret+1;j<=i;j++)
				x=(fcl.line[j]-ax)*(fcl.line[j]-ax)+x;
		m_MidArray[m_MidNum].line[i] = x;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}
 
int CFormularComputeParent::Func832() 
{
	int cl,cr;
	ARRAY_BE fcl,fcr;

	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =max(fcl.b,fcr.b)+1; 
	m_MidArray[m_MidNum].e =min(fcl.e,fcr.e);
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		if(fcl.line[i-1] < fcr.line[i-1] &&
						fcl.line[i] > fcr.line[i])
				m_MidArray[m_MidNum].line[i] = 1;
		else
				m_MidArray[m_MidNum].line[i] = 0;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func831() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	if(ret<0) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 31;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	if(ret == 0)
		m_MidArray[m_MidNum].b =fcl.b; 
	else
		m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	if(ret>0)
		for(int i=bl;i<=be;i++)
		{
			float x=0;
			for(j=i-ret+1;j<=i;j++)
				if(fcl.line[j] != 0) x+=1;
			m_MidArray[m_MidNum].line[i] = x;
		}
	else
		for(int i=bl;i<=be;i++)
			m_MidArray[m_MidNum].line[i] = i+1;
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func834() 
{
	int cl,cr,ret;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 34;
	}

	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;

	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;

	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}
	m_MidArray[m_MidNum].line[bl]=fcl.line [bl];

	double db = fcl.line [bl];
	for(int i1=bl+1;i1<=be;i1++)
	{
		//add lmb ;20010301
		ret = fcr.line[i1]+1;
		if(ret<1) ret = 1;

		db = (fcl.line [i1]-db)*2.0/(ret) + db;
		m_MidArray[m_MidNum].line[i1]=db;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func835()
 {
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	
	ret = GetBeginFoot( fcr);
		
	
	if(ret<0)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 35;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 

	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	if(ret > 0)
	{
		for(int i=bl;i<=be;i++)
		{
			//add lmb ;20010301
			ret = fcr.line[i];
			if(ret<1) ret = 1;
			//end add
			float ax=fcl.line[i];
			if(i-bl<ret)
			{
				for(j=bl;j<=i;j++)
					ax=max(ax,fcl.line[j]);
			}
			else
			{
				for(j=i-ret+1;j<=i;j++)
					ax=max(ax,fcl.line[j]);
			}
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}
	else
	{
		float ax=fcl.line[bl];
		for(int i=bl;i<=be;i++)
		{

			ax=max(ax,fcl.line[i]);
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}

	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func836() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	

	ret = GetBeginFoot( fcr);
	
	
	if(ret<0) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 36;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 

	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	if(ret > 0)
	{
		for(int i=bl;i<=be;i++)
		{
			//add lmb ;20010301
			ret = fcr.line[i];
			if(ret<1) ret = 1;
			//end add
			float ax=fcl.line[i];
			if(i-bl<ret)
			{
				for(j=bl;j<=i;j++)
					ax=min(ax,fcl.line[j]);
			}
			else
			{
				for(j=i-ret+1;j<=i;j++)
					ax=min(ax,fcl.line[j]);
			}
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}
	else
	{
		float ax=fcl.line[bl];
		for(int i=bl;i<=be;i++)
		{

			ax=min(ax,fcl.line[i]);
			m_MidArray[m_MidNum].line[i] = ax;
		}
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func837()
{	
	int cl,cr,ce,j;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(ce,fce);


	int nl = fcl.b;
	nl =(int)fcl.line[nl];
	if(nl<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 37;
	}

	int nr = fcr.b;
	float cfr =(float)(fcr.line[nr]/100);
	if(cfr<0.001||cfr>0.05) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 37;
	}

	int ne = fce.b;
	float cfe =(float)(fce.line[ne]/100);
	if(cfe<0.01||cfe>0.8) return 37;

	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+nl-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}
	int fl; 
		
	float hl,cft;
	float newhigh,newlow;
	if((float)((m_pData[bl].close>m_pData[bl-nl+1].close)))
		fl=2;
	else
		fl=3;
	for(int i=bl;i<=be;i++)
	{
		switch(fl)
		{
		case 0:
			if(m_pData[i].high>newhigh)
			{
				newhigh =m_pData[i].high;
			}

				cft+=cfr;

			if(cft>cfe)
				cft =cfe;

			if(m_pData[i].high>m_pData[i-1].high)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1]+cft*
					(m_pData[i-1].high-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close<=m_MidArray[m_MidNum].line[i-1])
				fl=3;
			break;
		case 1:
			if(m_pData[i].low<newlow)
			{
				newlow =m_pData[i].low;
			}

				cft+=cfr;

			if(cft>cfe) 
				cft =cfe;

			if(m_pData[i].low<m_pData[i-1].low)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1] + cft*
					(m_pData[i-1].low-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close>=m_MidArray[m_MidNum].line[i-1])
				fl=2;

			break;
		case 2:
			hl=m_pData[i-nl+1].low;
			newhigh =m_pData[i-nl+1].high;
			for(j=i-nl+2;j<=i;j++)
			{
					if(hl>m_pData[j].low)
						hl=m_pData[j].low;
					if(newhigh<m_pData[j].high)
						newhigh = m_pData[j].high;
			}
			m_MidArray[m_MidNum].line[i] =hl;
			fl = 0;
			cft=0;
			break;
		case 3:
			hl=m_pData[i-nl+1].high;
			newlow =m_pData[i-nl+1].low;
			for(j=i-nl+2;j<=i;j++)
			{
				if(hl<m_pData[j].high)
					hl=m_pData[j].high;
				if(newlow<m_pData[j].low)
					newlow = m_pData[j].low;
			}
			fl=1;
			cft=0;
			m_MidArray[m_MidNum].line[i] =hl;
			break;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;

	return 0;
}
int CFormularComputeParent::Func838()
 {
	int cl,cr,ce,j;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(cr,fce);

	int nl = fcl.b;
	nl =(int)fcl.line[nl];
	if(nl<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 38;
	}

	int nr = fcr.b;
	float cfr =(float)(fcr.line[nr]/100);
	if(cfr<0.01||cfr>0.05) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 38;
	}

	int ne = fce.b;
	float cfe =(float)(fce.line[ne]/100);
	if(cfe<0.1||cfe>0.8) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 38;
	}

	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+nl-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}
	int fl;  
		
	float hl,cft;
	float newhigh,newlow;
	if((float)((m_pData[bl].close>m_pData[bl-nl+1].close)))
		fl=2;
	else
		fl=3;
	for(int i=bl;i<=be;i++)
	{
		switch(fl)
		{
		case 0:
			if(m_pData[i].high>newhigh)
			{
				newhigh =m_pData[i].high;
				cft+=cfr;
			}
			if(cft>cfe)
				cft =cfe;
			if(m_pData[i].high>m_pData[i-1].high)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1]+cft*
					(m_pData[i-1].high-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close<=m_MidArray[m_MidNum].line[i-1])
				fl=3;
			break;
		case 1:
			if(m_pData[i].low>newlow)
			{
				newlow =m_pData[i].low;
				cft+=cfr;
			}
			if(cft>cfe) 
				cft =cfe;
			if(m_pData[i].low<m_pData[i-1].low)
			{
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1]+cft*
					(m_pData[i-1].low-
					m_MidArray[m_MidNum].line[i-1]);
			}
			else
				m_MidArray[m_MidNum].line[i] =
					m_MidArray[m_MidNum].line[i-1];
			if(m_pData[i].close>=m_MidArray[m_MidNum].line[i-1])
				fl=3;

			break;
		case 2:
			hl=m_pData[i-nl+1].low;
			newhigh =m_pData[i-nl+1].high;
			for(j=i-nl+2;j<=i;j++)
			{
					if(hl>m_pData[j].low)
						hl=m_pData[j].low;
					if(newhigh<m_pData[j].high)
						newhigh = m_pData[j].high;
			}
			m_MidArray[m_MidNum].line[i] =hl;
			fl = 0;
			cft=0;
			break;
		case 3:
			hl=m_pData[i-nl+1].high;
			newlow =m_pData[i-nl+1].low;
			for(j=i-nl+2;j<=i;j++)
			{
				if(hl<m_pData[j].high)
					hl=m_pData[j].high;
				if(newlow<m_pData[j].low)
					newlow = m_pData[j].low;
			}
			fl=1;
			cft=0;
			m_MidArray[m_MidNum].line[i] =hl;
			break;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;

	return 0;
}

int CFormularComputeParent::Func840() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
		//change yml ;20010301
	ret = GetBeginFoot( fcr);
	

	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 40;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0;
		//add yml ;20010301
		ret = fcr.line[i];
		if(ret<1) ret = 1;
	

		for(j=i-ret+1;j<=i;j++)
				ax+=fcl.line[j];
		ax/=ret;
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func839() 
{
	int cl,cr,ret;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	

	ret = GetBeginFoot( fcr);
	
	
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 39;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		//add lmb ;20010301
		ret = fcr.line[i];
		if(ret<1) ret = 1;
		//end add
		m_MidArray[m_MidNum].line[i] = fcl.line[i-ret];
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func841()
 {
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<2) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 41;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(float)sqrt(FABSMY(ax*ret-af*af)/ret/(ret-1));
		m_MidArray[m_MidNum].line[i] = ax;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func842() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 42;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(float)sqrt((ax*ret-af*af)/ret/ret);
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func843() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	

	ret = GetBeginFoot( fcr);
		
	if(ret<0) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 43;
	}

	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(ret==0)
	{
		m_MidArray[m_MidNum].b =fcl.b;
		bl = m_MidArray[m_MidNum].b;
	}

	if(be>=bl&&bl>=0&&be>=0)
	{
		if(ret==0)
		{
			m_MidArray[m_MidNum].line[bl]=fcl.line[bl];
			for(int i=bl+1;i<=be;i++)
			{
				m_MidArray[m_MidNum].line[i]=0;
				float ax=0;
				for(j=bl;j<=i;j++)
				{
					ax+=fcl.line[j];
				}
				m_MidArray[m_MidNum].line[i] = fcl.line[i] + m_MidArray[m_MidNum].line[i-1];
			}
		}
		else
		{
			for(int i=bl;i<=be;i++)
			{
				float ax=0;
				//add lmb ;20010301
				ret = fcr.line[i];
				if(ret<1) ret = 1;
				//end add
				for(j=i-ret+1;j<=i;j++)
				{
					ax+=fcl.line[j];
				}
				m_MidArray[m_MidNum].line[i] = ax;
			}
		}
	}	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func844() 
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<2) 
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 44;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(ax*ret-af*af)/ret/ret;
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	delete[] fcl.line;
	delete[] fcr.line;
		return 0;
}

int CFormularComputeParent::Func845()
{
	int cl,cr,ret,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		return 45;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		return 999;
	}

	for(int i=bl;i<=be;i++)
	{
		float ax=0,af=0;
		for(j=i-ret+1;j<=i;j++)
		{
			ax+=fcl.line[j]*fcl.line[j];
			af+=fcl.line[j];
		}
		ax=(ax*ret-af*af)/ret/(ret-1);
		m_MidArray[m_MidNum].line[i] = ax;
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func846()
{
	int cl,cr,ce,ret,j;
	ARRAY_BE fcl,fcr,fce;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	fce.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	ce=m_table.pop_it();
	GetData(ce,fce);
	
	ret = fcr.b;
	ret =(int)fcr.line[ret];
	
	if(ret<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 46;
	}
	int ret2 = fce.b;
	ret2 =(int)fce.line[ret2];
	if(ret2>=ret)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;

		return 46;
	}
	if(ret2<1)
	{
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 46;
	}
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b+ret-1; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		delete[] fcr.line;
		delete[] fce.line;
		return 999;
	}

	if(be>=bl&&bl>=0&&be>=0)
	{
		m_MidArray[m_MidNum].line[bl] = 0;
		for(j=bl-ret+1;j<=bl;j++)
		{
			m_MidArray[m_MidNum].line[bl]+=fcl.line[j];
		}
		m_MidArray[m_MidNum].line[bl] /= ret;

		double db = m_MidArray[m_MidNum].line[bl];
		for(int i=bl+1;i<=be;i++)
		{

			db = (ret2*fcl.line[i]+(ret-ret2)*db)/ret;
			m_MidArray[m_MidNum].line[i] = db ;
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	delete[] fcr.line;
	delete[] fce.line;
	return 0;
}

void CFormularComputeParent::GetMsg(CString& str,int& pos)
{
	if(m_errmsg.IsEmpty ())
		str = " ";
	else
		str=m_errmsg;
	pos=m_errpos;
}


int CFormularComputeParent::GetLine(CString& LineName, ARRAY_BE& pline)
{
	int fl=1;
	for(int i=0;i<m_RlineNum;i++)
	{
		CString s1 = m_RlineName[i];
		s1.MakeLower ();
		CString s2 = LineName;
		s2.MakeLower ();
		if(s1==s2)
		{
			fl=0;
			int bl = m_Rline[i].b;
			int be = m_Rline[i].e;
			if(bl>be||bl<0||be<0||be>=m_MaxPoint)
				return 1;

			pline = m_Rline[i];

			break;
		}
	}
	return fl;
}

int CFormularComputeParent::GetLine(int& index, ARRAY_BE& pline,CString& LineName)
{
	if(index >= m_RlineNum)
		return 1;
	int bl = m_Rline[index].b;
	int be = m_Rline[index].e;
	pline.b = bl;
	pline.e = be;
	LineName = m_RlineName[index];


	if(bl>be||bl<0||be<0||be>=m_MaxPoint)
		return 1;
	pline = m_Rline[index];

	return 0;
}
int CFormularComputeParent::GetIndexPoint(int &index,float &nPoint)
{
	if(index >= m_RlineNum)
		return 1;
	int bl = m_Rline[index].b;
	int be = m_Rline[index].e;

	if(bl>be||bl<0||be<0||be>=m_MaxPoint)
		return 1;
	nPoint=m_Rline[index].line[be];
	return 0;
}

int CFormularComputeParent::AddLine(CString& LineName, ARRAY_BE& pline)
{
	if(m_VarNum >=MIDE)
		return 1;

	m_WordTableNew[LineName]=(void*)(MIDB+m_VarNum);

	m_VarArray[m_VarNum].line = new float[m_MaxPoint];

	m_VarArray[m_VarNum].b = pline.b;
	m_VarArray[m_VarNum].e = pline.e;
	
	for(int i=pline.b;i<=pline.e;i++)
		m_VarArray[m_VarNum].line[i] = pline.line[i];

	m_VarName[m_VarNum] = LineName;					
	m_VarNum++;
	m_exec     =0;
	return 0;
}

int CFormularComputeParent::Relolve(int cl)

{
	CString  str;
	CString  ename;
	CString  linename;

	CString  sl;
	sl=m_QuoteArray[cl-QUB];
	sl.Replace ("$",".");
	sl.Replace ("#",".");

	int tu;
	ARRAY_BE  fcl;
	int equationKind = 0;
	int nPeriods = 0;
	if(GetQuote(sl,ename,linename,tu,equationKind,nPeriods))
	{
		ASSERT(FALSE);
		return 1;
	}
	if(equationKind ==1000)
	{
		return 1;
	}

	int nParamInTable = 0;
	if(!LookupParameterTable(cl,nParamInTable))
	{
		ASSERT(FALSE);
		return 1;
	}

	if(equationKind<10 || equationKind>=100)
	{
		cl = m_MaxPoint ;

		ename.MakeLower();
		linename.MakeLower();
		sl.MakeLower();


		CString zb;
		zb = ename;
		zb.MakeUpper();
		CFormularContent*	pIndex = NULL;
		int nKindEqt = equationKind;
		if(equationKind>=100)
			nKindEqt = equationKind-100;

		switch(nKindEqt)
		{
		case 0:
			pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_index,zb));
			break;
		case 1:
			pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_choose ,zb));
			break;
		case 2:
			pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_kline ,zb));
			break;
		case 3:
			pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_master,zb));
			break;
		}
		if(pIndex==NULL)
		{
			ASSERT(FALSE);
			return 1;
		}
		
		Kline * pKline = NULL;


		int nKlineInOut = m_MaxPoint;
		CString symbol = m_symbol;
		if(equationKind>=100)
		{
			int nFind = sl.Find (".",0);
			if(nFind<=0)
			{
				ASSERT(FALSE);
				return 1;
			}

			symbol= sl.Left (nFind);
			symbol.MakeUpper ();
			pKline = new Kline;
			nKlineInOut=ReadKline(symbol,m_stkKind,pKline);
			cl = nKlineInOut;
		}
		else 
			pKline = m_pData;

	
		Kline * pKlineNew = NULL;
		bool bTransfer = false;

		bool bCalcHs = false;
		bool bCalcMin5 = false;

		if(nPeriods>0 && m_nKindKline>0 &&m_nKindKline!= NUM_MANY_DAY)
	
		{
	
			if( m_nKindKline == HS_K_LINE)
			{
				if(nPeriods==MINUTE_1_K_LINE)
					bTransfer = true;
			}
			else
			{
				if(nPeriods<5)
				{
					if(m_nKindKline<5&&m_nKindKline>0)
					{
						if(nPeriods - m_nKindKline>=1) bTransfer = true;
					}
				}
				else
				{
					if(m_nKindKline<9 && m_nKindKline>4)
					{
						if(nPeriods - m_nKindKline>=1) bTransfer = true;
					}
				}
			}

		}
		if( m_nKindKline == 5)
		{
			if(nPeriods == HS_K_LINE )
				bCalcHs = true;
			if(nPeriods == 1 )
				bCalcMin5 = true;
		}

	
		if(bCalcMin5 == true)
		{
			CalcMin5InDay(pIndex, nParamInTable, linename, tu);
		
			return 0;
		}

	
		if(bTransfer == true)
		{
			CTaiKlineTransferKline trans;
			if(equationKind>=100)
			{
				pKlineNew = pKline;
				trans.TransferKlineHistory(symbol,m_stkKind,pKlineNew, nKlineInOut,nPeriods,true);//used to transfer k line data to another kind of k line
			}
			else
			{
				pKlineNew = new Kline[m_MaxPoint];
				memcpy(pKlineNew,m_pData,m_MaxPoint*sizeof(Kline));
				trans.TransferKlineHistory(symbol,m_stkKind,pKlineNew, nKlineInOut,nPeriods,false);//used to transfer k line data to another kind of k line
			}
			pKline = pKlineNew;
			if(nKlineInOut<=0)
			{
				delete[] pKlineNew;
				ASSERT(FALSE);
				return 1;
			}
			cl = nKlineInOut;
		}

		int nMaxPoint = (m_MaxPoint>cl?m_MaxPoint:cl);
		fcl.line = new float[nMaxPoint];
		memset(fcl.line,0,(m_MaxPoint>cl?m_MaxPoint:cl));

		{
			CFormularComputeParent temp(m_MinPoint,cl-1,pKline,pIndex,symbol,m_stkKind,this->m_pBuySellList ,m_nKindKline);
			
			if(tu>8)
				tu=0;
			if(nParamInTable == tu)
			{
				for(int i=0;i<tu;i++)
				{
					cl=m_table.pop_it();
					GetData(cl,fcl);
					CString sname(pIndex->namePara[i] );
					sname.MakeLower();
					temp.AddPara(sname,fcl.line[fcl.b]);
				}
			}
			else	if(nParamInTable == 0)
			{
				for(int i=0;i<tu;i++)
				{
					CString sname(pIndex->namePara[i] );
					sname.MakeLower();
					temp.AddPara(sname,
						CFormularContent::GetParamDataEach(i,m_nKindKline, pIndex));//
				}
			}
			else
				ASSERT(FALSE);

			if(temp.FormularCompute()!=0)
			{
				delete[] fcl.line;
				if(bTransfer == true)
					if(pKlineNew!=0)
						delete[] pKlineNew;
				return 1;
			}
			if(temp.m_bTotalData != 0) m_bTotalData = temp.m_bTotalData;

			m_MidArray[m_MidNum].line = new float[m_MaxPoint];
			m_MidArray[m_MidNum].b=0;
			m_MidArray[m_MidNum].e=m_MaxPoint-1;

			int nRtn = 0;
			if(bTransfer ==true || equationKind>=100)
			{
		
				nRtn=temp.GetLine(linename,fcl);
				if(nRtn == 1)
				{
					delete[] fcl.line;

					if(bTransfer == true)
						if(pKlineNew!=0)
							delete[] pKlineNew;

					return 1;
				}
				int b = fcl.b;
				int bMidArray=0;
				int j=0;
				{
					int tmb ;
					tmb = pKline[b].day;
					for( j=0;j<m_MaxPoint;j++)
					{
						if(m_pData[j].day >= tmb)
							break;
					}
					m_MidArray[m_MidNum].b=j<m_MaxPoint?j:(m_MaxPoint-1);
				}

				int timeBegin = pKline[b].day;
				int k=m_MidArray[m_MidNum].b;
				for( j=b;j<=fcl.e && k<m_MaxPoint;)
				{
					if(m_pData[k].day < pKline[j].day)
					{
						m_MidArray[m_MidNum].line[k] = fcl.line[j];
						k++;
					}
					else if(m_pData[k].day == pKline[j].day)
					{
						m_MidArray[m_MidNum].line[k] = fcl.line[j];
						k++;
						j++;
					}
					else
						j++;
				}
				if(k<=m_MaxPoint-1)
				{
					int nK=0;
					if(nK>0)
						nK = k-1;
					for(;k<m_MaxPoint;k++)
						m_MidArray[m_MidNum].line[k] =  m_MidArray[m_MidNum].line[nK];
				}

			}
			else
				nRtn = temp.GetLine(linename,m_MidArray[m_MidNum]);
			if(nRtn ==1)
			{
				delete []	m_MidArray[m_MidNum].line;
				delete[] fcl.line;

				if(bTransfer == true)
					if(pKlineNew!=0)
						delete[] pKlineNew;
				return 1;
			}
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
		}

		delete[] fcl.line;

		if(bTransfer == true)
			if(pKlineNew!=0)
				delete[] pKlineNew;
	}
	else
	{
		
		CString symbol ;
		int nFind = sl.Find (".",0);
		if(nFind<=0)
			return 1;

		symbol= sl.Left (nFind);
		symbol.MakeUpper ();
		int n = 0;
		Kline* pKline = NULL;
		 char *strLabel = symbol.GetBuffer (0);

		 int stkKind = GetStockKind(strLabel);

		n=ReadKline(symbol,stkKind,pKline);

	
		m_MidArray[m_MidNum].line = new float[m_MaxPoint];
		int nRate =1;
		if(m_nKindKline>4&&m_nKindKline<9)
			nRate = 24*60*60;
		else
			nRate = 24*60;
		int j =0;
		Kline * pKline2 = new Kline[m_MaxPoint];
		
		int i;
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			if(m_pData[i].day/nRate >= pKline[0].day/nRate)
				break;
		}
		m_MidArray[m_MidNum].e=m_MaxPoint-1;
		if(i<m_MaxPoint)
			m_MidArray[m_MidNum].b=i;
		else
		{
			m_MidArray[m_MidNum].e = -1;
			m_MidArray[m_MidNum].b = 0;
		}

		for(;i<m_MaxPoint && j<n;)
		{
			if(m_pData[i].day/nRate == pKline[j].day/nRate)
			{
			
				memcpy(pKline2+i,pKline+j,sizeof(Kline));// = 
				j++;
				i++;
			}
			else if(m_pData[i].day/nRate > pKline[j].day/nRate)
			{
				j++;
			}
			else
			{
				memcpy(pKline2+i,pKline+j,sizeof(Kline));// = 
				pKline2[i].day = m_pData[i].day;// = 
				i++;
			}
		}
		if(i<=m_MaxPoint-1)
		{
			int nK =0;
			if(i>0)
				nK = i-1;
			for(;i<m_MaxPoint;i++)
				memcpy(pKline2+i,pKline2+nK,sizeof(Kline));
		}


		int nSubMem = equationKind-10;
		for(i=m_MinPoint;i<m_MaxPoint;i++)
		{
			KlineToLine(pKline2+i, m_MidArray[m_MidNum].line[i],nSubMem );
		}

		if(pKline2!=NULL)
			delete [] pKline2;
		if(pKline!=NULL)
			delete [] pKline;

		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		//
	}
	return 0;
}

 int CFormularComputeParent::GetQuote(CString &str, CString &zb, CString &cline, int &tu,int &equationKind,int &nPeriods)//equationKind is the kind of CFormularContent(0 is zhibiao ,1 is xuangu,2 is klinemode,3 is stock system,10 is other stock's kline price)

{

	CStringArray sName;
	str.Replace ("$",".");
	str.Replace ("#",".");
	 int len =str.GetLength(); 
	 if(len<=0)
		 return 1;

	 int lb = 0;
	 while(true)
	 {
		 int le=str.Find('.',lb);
		 CString s;

		 if(le<0)
			 s =  str.Mid(lb,len-lb);
		 else
			s =  str.Mid(lb,le-lb);
		s.TrimLeft (" ");
		s.TrimRight (" ");
		s.MakeUpper();
		sName.Add (s);
		if(le<0)
			break;

		lb = le+1;
	 }
	 if(sName.GetSize ()<=0)
		 return 1;


	 CReportData* pStockData;
	 int nFirst = 0;
	 CString s2 = sName[nFirst];

	 int nAddEquationKind = 0;

	 char *strLabel = s2.GetBuffer (0);
	 int stkKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strLabel);
	 bool bFind = false;
	 if(stkKind>=0)
		 if(pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
			 bFind = true;
		 ;
	 if(bFind == false)
	 {
		 stkKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strLabel);
		 if(stkKind>=0)
			 if(pDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
				 bFind = true;
	 }
	 if(bFind == true)
	 {
		 nFirst++;
		 if(sName.GetSize ()<=nFirst)
			return 2;

		 if(sName.GetSize()==2)
		 {
			 bool bFound = true;
			 if(sName[1]=="OPEN")	equationKind = 0;
			 else if(sName[1]=="HIGH")	equationKind = 1;
			 else if(sName[1]=="LOW")	equationKind = 2;
			 else if(sName[1]=="CLOSE")	equationKind = 3;
			 else if(sName[1]=="VOL")	equationKind = 4;
			 else if(sName[1]=="AMOUNT")	equationKind = 5;
			 else if(sName[1]=="ADVANCE")	equationKind = 6;
			 else if(sName[1]=="DECLINE")	equationKind = 7;
			 else if(sName[1]=="BUYVOL")	equationKind = 8;
			 else if(sName[1]=="SELLVOL")	equationKind = 9;
			 else bFound = false;

			 if(bFound == true)
			 {
				 equationKind = equationKind+10;
				 tu =0;
				 return 0;
			 }
			 else
				 nAddEquationKind = 100;

		 }
		 else if(sName.GetSize()>2)
			 nAddEquationKind = 100;
		 else
			 return 1;
	 }
		s2.ReleaseBuffer();

	CFormularContent*	pIndex;
	 equationKind = 0;
	 if(sName[nFirst] == "STOCKFILTER")
	 {
		 equationKind = 1;
		 nFirst ++;
		 if(sName.GetSize ()<=nFirst)
			return 1;
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_choose,sName[nFirst])); 
	 }
	 else if(sName[nFirst] == "STOCKMODEKLINE")
	 {
		 equationKind = 2;
		 nFirst ++;
		 if(sName.GetSize ()<=nFirst)
			return 1;
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_kline,sName[nFirst])); 
	 }
	 else if(sName[nFirst] == "STOCKSYSTEM")
	 {
		 equationKind = 3;
		 nFirst ++;
		 if(sName.GetSize ()<=nFirst)
			return 1;
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_master,sName[nFirst])); 
	 }
	 else 
	     pIndex=(CFormularContent*)(pDoc->LookUpArray (pDoc->m_formuar_index,sName[nFirst])); 


	 zb = sName[nFirst];
	 equationKind += nAddEquationKind;

	int rtn=0;
	if(pIndex==NULL)
		return 1;


	nPeriods = 0;
	tu=pIndex->numPara ;
	cline = "";
	 nFirst ++;

	CString fml=pIndex->fomular;
	fml.MakeLower();
	CStringArray sArray;
	//add lmb 20001103
	Kline kline;
	CFormularComputeParent equation(0,0,&kline,fml);
	for(int k=0;k<pIndex->numPara ;k++)
	{
		equation.AddPara(pIndex->namePara[k] ,pIndex->defaultVal [k]);
	}
	equation.Devide();
	if(equation.m_bTotalData != 0) m_bTotalData = equation.m_bTotalData;

	int nRtnEqt = equation.GetRlineNum ();

	for(int k=0;k<nRtnEqt;k++)
	{
		sArray.Add (equation.GetRlineName (k));
	}

	 if(sName.GetSize ()<=nFirst)
	 {
	
		for(int i = 0;i<sArray.GetSize ();i++)
		{
			if(sArray[i]==zb)
			{
				cline=zb;
				return 0;
			}
		}
		if(sArray.GetSize ())
		{
			{
				cline=sArray[0];
				return 0;
			}
		}
		return 2;
	 }

	 cline = sName[nFirst];
	 if(sName.GetSize () == nFirst+1)
	 {
		 if(nPeriods = GetPeriodKind(sName[nFirst]))
		 {

			for(int i = 0;i<sArray.GetSize ();i++)
			{
				if(sArray[i]==zb)
				{
					cline=zb;
					return 0;
				}
			}
			if(sArray.GetSize ())
			{
				{
					cline=sArray[0];
					return 0;
				}
			}
			 return 2;
		 }
	 }
	 else if(sName.GetSize () == nFirst+2)
	 {
		 nPeriods = GetPeriodKind(sName[nFirst+1]);
		 if(nPeriods==0)
			 return 2;
	 }
	 else
	 {
		 ASSERT(FALSE);
		 return 1;
	 }



	cline.MakeLower();
	rtn = 2;
	for(int k=0;k<sArray.GetSize ();k++)
	{
		if(cline == sArray[k])
		{
			rtn = 0;
			break;
		}
	}


	return rtn;

}

int CFormularComputeParent::GetNum(int tj)
{

	int tu;
	if(!LookupParameterTable(tj,tu))
	{
		AfxMessageBox("引用的公式的参数个数没有找到");
		return 0;
	}
	return tu;
}

int CFormularComputeParent::GetRlineNum()
{
	return m_RlineNum;

}

CString CFormularComputeParent::GetRlineName(int foot)
{
	return m_RlineName[foot];
}
int GetInt(CString s)
{
	if(s.GetLength ()<2)
		return 0;
	int n = 0;
	int nRate[2] = {16,1};
	for(int i = 0;i<2;i++)
	{
		int ntmp = 0;
		if(s[i]>='0' && s[i]<='9')
			ntmp = s[i]-'0';
		else
			ntmp = 10+(s[i]-'a');
		n+=(ntmp*nRate[i]);
	}
	return n;
}
int CFormularComputeParent::FindLineType()
{
	if( m_SentNum<0)
		return 0;

	int flag=0;

	int nRtn = 0;



	m_tempWhich++;
	for(int i=0;i<=m_SentNum;i++)
	{
		if(m_Sent[i]==1002)
		{
			m_tempWhich--;
			flag=2;
			break;
		}
		if(m_Sent[i]==1001)
		{
			flag=1;
			break;
		}

		if(m_Sent[i]>=FuncDllB+50 && m_Sent[i]<= FuncDllB+52)
		{
			m_bRightbox = true;
		}
	}
	if(flag==0 )
	{

		m_SentNum=m_SentNum+2;
		for(int i=m_SentNum-2;i>=0;i--)
		{
			m_Sent[i+2]=m_Sent[i];
		}
		CString temp;

		temp.Format("%d",(int)m_tempWhich);
		temp="l"+temp;
		m_WordTableNew[temp]=(void*)(RLINEB+m_RlineNum);
		m_Rline[m_RlineNum].line = new float[m_MaxPoint];

		m_RlineName[m_RlineNum] = temp;

		m_Sent[0]=RLINEB+m_RlineNum;
		m_Sent[1]=1001;
		m_Sent[m_SentNum+1]=0;

		m_RlineNum++;

	}

	if(flag == 2) return 0;

	m_RlineType[m_tempWhich] = 0;
	for(int i=0;i<=m_SentNum;i++)
	{

		if(m_Sent[i]>=CONSB&&m_Sent[i]<=CONSE && i>0)
		{
			if(m_Sent[i-1]!=9005 )
			{
				for(int k = m_SentNum;k>=i;k--)
					m_Sent[k+1] = m_Sent[k];
				m_Sent[i] = 9005;
				m_SentNum++;
				i--;
				continue;
			}
		}

		if(m_Sent[i+1]>=CONSB&&m_Sent[i+1]<=CONSE)
		{
			if(m_Sent[i]!=9005 && i!=m_SentNum)
			{
				for(int k = m_SentNum;k>i;k--)
					m_Sent[k+1] = m_Sent[k];
				m_Sent[i+1] = 9005;
				m_SentNum++;
				continue;
			}

			if(m_Sent[i+1]>=CONSCOLORB1 && m_Sent[i+1]<CONSLINETHICKB)
			{
				if(m_Sent[i+1]>=CONSCOLORB)
				{
					CString s = m_strColor[m_Sent[i+1]-CONSCOLORB];
					m_RlineColor[m_tempWhich] = RGB(GetInt(s.Right(2)),GetInt(s.Mid(2,2)),GetInt(s.Left (2)));
				}
				else
				{
					switch(m_Sent[i+1])
					{
					case 9100:
						m_RlineColor[m_tempWhich] = RGB(255,0,0);
						break;
					case 9101:
						m_RlineColor[m_tempWhich] = RGB(0,255,0);
						break;
					case 9102:
						m_RlineColor[m_tempWhich] = RGB(0,0,255);
						break;
					case 9103:
						m_RlineColor[m_tempWhich] = RGB(255,255,0);
						break;
					case 9104:
						m_RlineColor[m_tempWhich] = RGB(255,100,0);
						break;
					case 9105:
						m_RlineColor[m_tempWhich] = RGB(255,0,255);
						break;
					case 9106:
						m_RlineColor[m_tempWhich] = RGB(0,0,0);
						break;
					case 9107:
						m_RlineColor[m_tempWhich] = RGB(255,255,255);
						break;
					case 9108:
						m_RlineColor[m_tempWhich] = RGB(255,0,255);
						break;
					case 9109:
						m_RlineColor[m_tempWhich] = RGB(0,255,255);
						break;

					}
				}
			}
			else if(m_Sent[i+1]>=CONSLINETHICKB)
			{
				if(m_Sent[i+1]!= CONSLINEDOT)
					m_RlineThick[m_tempWhich] = m_Sent[i+1]-CONSLINETHICKB+1;
				else
					m_RlineThick[m_tempWhich]+=100;
			}
			else
			{
					m_RlineType[m_tempWhich]=m_Sent[i+1]-CONSB+1;
					if(m_RlineType[m_tempWhich] == 7 || m_RlineType[m_tempWhich] == 8)
						m_RlineType[m_tempWhich] += 100;

			}

			for(int j=i;j<m_SentNum-1;j++)
			{
				m_Sent[j]=m_Sent[j+2];

			}
			m_SentNum-=2;
			m_Sent[m_SentNum+1]=0;

			i--;
	

		}
	}
	return nRtn;
}

void CFormularComputeParent::IsDaPan()
{

	for(int i=0;i<m_SentNum;i++)
	{
		if(m_Sent[i]==9525||m_Sent[i]==9537)
		{
			m_isDaPan=1;
		}
	}

}

void CFormularComputeParent::InitEquation(int pmin, int pmax, Kline *pp, CString &str)
{
	m_bRightbox = false;
	m_nCurrentFoot = 0;
	m_nSizeKindPos = 0;
	m_nPosPre = 0;
	m_pKindPos = NULL;
	m_pKlineEx = NULL;

	m_errmsg = _T(" ");
	m_bTotalData=false;
	m_isDaPan=0;
	for(int k = 0;k<NUM_LINE_TOT;k++)
	{
		m_RlineColor[k] = -1;
		m_RlineThick[k] = 1;
		m_RlineType[k]=0;
	}

	m_pData      = pp;
	m_QuoteNum   =0;
	m_floatNum = 0;
	m_intNum   = 0;
	m_VarNum   = 0;
	m_RlineNum   = 0;
	m_MidNum   = 0;
	m_NumGs    = 0;
	m_stringNum = 0;
	m_strColorNum = 0;


	int b = 0;
	while(true)
	{
		int e = str.Find ("{",b);
		if(e<0)
			break;
		b = e;
		e = str.Find ("}",b);
		if(e<0)
			break;

	
		for(int i=b;i<=e;i++)
		{
			str.SetAt (i,' ');
		}
		b = e;
	}
	
	m_formuCompute   = str;
	m_exec     =0;

	m_MaxPoint = pmax+1;
	m_MinPoint = pmin;
#ifdef TEST_Version1

#endif
	

	InitStatic();

	m_symbol = _T("");
	m_pBuySellList = NULL; 
	pDoc =((CMainFrame*)AfxGetMainWnd())->m_taiShanDoc ;
	m_pKlineIndex = NULL;
	m_cdat1 = NULL;
	m_nKindKline = -1;
	m_bCreateBsList = false;
	m_pIndex = NULL;

}


int CFormularComputeParent::Func848_51(int nKind)
{
	int cl,ret;
	ARRAY_BE fcl;
	
	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	
	ret = fcl.b;
	ret =(int)fcl.line[ret];
	if(ret<1)
		ret = 1;
	if(ret>3)
		ret =3;
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	if(m_nKindKline!=HS_K_LINE)
	{
		int nIn ;
		if(nKind-3<2)
			nIn = 9+(ret-1)*2+nKind-3;
		else
			nIn = 15+(ret-1)*2+nKind-5;
		TransferRealInfo(m_MidArray[m_MidNum].b,m_MidArray[m_MidNum].e, nIn);
	}
	else
	{
		ASSERT(this->m_pBuySellList!=NULL);
		ASSERT(this->m_pBuySellList->GetCount() == m_MaxPoint);
		if(m_MaxPoint!=m_pBuySellList->GetCount ())
		{
			m_table.push_b(PCMB+m_MidNum);
			m_MidNum++;
			delete[] fcl.line;
			return 999;
		}

		{
			if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
			if(m_pKlineEx != NULL) CTaiKlineFileHS::TransferHs(this->m_pBuySellList , m_MidArray[m_MidNum], m_MaxPoint, nKind,ret,m_pKlineEx);
		}
	}
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	if(bl>be||be>m_MaxPoint-1||bl<0)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	return 0;

}



int CFormularComputeParent::Func847()
{
	int cl,cr,ce,j;
	ARRAY_BE fcl,fcr;
	
	fcl.line = new float[m_MaxPoint];
	fcr.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	cr=m_table.pop_it();
	GetData(cr,fcr);
	
	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;
	float fVal = fcr.line [bl];

	if(be>=bl&&bl>=0&&be>=0)
	{
		m_MidArray[m_MidNum].line[bl] = 0;
		for(j=bl;j<=be;j++)
		{
			float f=0;
			int i =0;
			for( i = j;i>=bl;i--)
			{
				f+=fcl.line[i];
				if(f>=fVal)
				{
					i--;
					break;
				}

			}
			m_MidArray[m_MidNum].line[j] =float( j-i-1 );
		}
	}
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;
	delete[] fcr.line;
	return 0;
}

int CFormularComputeParent::Func852()
{
	if(this->m_cdat1==NULL)
		pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,m_stkKind);
	m_symbol.ReleaseBuffer();
	ASSERT(this->m_cdat1!=NULL);
	if(m_cdat1==NULL)
		return 852;

	ASSERT(this->m_symbol.GetLength()>=4);

	int cl,ret,j;
	ARRAY_BE fcl;

	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	ret = int( fcl.line [fcl.b ] );
	if(ret<1)
		ret = 1;
	if(ret>100)
		ret = 1;

	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;

	m_MidArray[m_MidNum].line[bl] = 0;
	BASEINFO* pBase = this->m_cdat1->pBaseInfo;
	if(pBase==NULL)
	{
		m_table.push_b(PCMB+m_MidNum);
		m_MidNum++;
		delete[] fcl.line;
		return 999;
	}


	float fVal[50] = {pBase->zgb ,pBase->gjg ,pBase->fqrfrg ,pBase->frg ,pBase->Bg,
	                  pBase->Hg  ,pBase->ltAg,pBase->zgg    ,pBase->zpg ,pBase->zzc*10 ,
					pBase->ldzc*10  ,pBase->gdzc*10,pBase->wxzc*10   ,pBase->cqtz*10,pBase->ldfz*10 ,
					pBase->cqfz*10  ,pBase->zbgjj*10 ,pBase->mggjj*10,pBase->ggqy*10 ,pBase->zyywsr*10,
					pBase->zyywlr*10,pBase->qtywlr*10,pBase->zyywlr*10,0          ,0,
					0            ,0            ,pBase->lrze*10 ,pBase->jlr*10 ,pBase->jlr*10,
					pBase->wfplr*10 ,pBase->mgwfplr ,pBase->mgsy ,pBase->mgjzc ,0,
					pBase->gdqybl ,pBase->jzcsyl };
	for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] = fVal[ret-1];    


	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;

	return 0;
}

int CFormularComputeParent::Func853()
{
	if(this->m_cdat1==NULL)
	{
		pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,m_stkKind);
		m_symbol.ReleaseBuffer();
	}
	ASSERT(this->m_cdat1!=NULL);
	if(m_cdat1==NULL)
		return 853;

	ASSERT(this->m_symbol.GetLength()>=4);

	int cl,ret;
	ARRAY_BE fcl;

	fcl.line = new float[m_MaxPoint];
	
	cl=m_table.pop_it();
	GetData(cl,fcl);
	ret = int( fcl.line [fcl.b ] );
	if(ret<1)
		ret = 1;
	if(ret>100)
		ret = 1;

	m_MidArray[m_MidNum].line = new float[m_MaxPoint];
	m_MidArray[m_MidNum].b =fcl.b; 
	m_MidArray[m_MidNum].e =fcl.e;
	
	int bl = m_MidArray[m_MidNum].b;
	int be = m_MidArray[m_MidNum].e;

	m_MidArray[m_MidNum].line[bl] = 0;
	TransferRealInfo( bl,  be,  ret);

	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	delete[] fcl.line;

	return 0;
}

Kline* CFormularComputeParent::GetKlineIndex()
{

	if(m_pKlineIndex!=NULL)
		return m_pKlineIndex;
	if(m_nKindKline==-1)
	{
		ASSERT(FALSE);
		return NULL;
	}

	bool bSh = true;
	CString s = CSharesCompute::GetIndexSymbol(0);
	int stkKind = 0;
	if(CSharesCompute::GetMarketKind(m_stkKind) == SZ_MARKET_EX)
	{
		bSh = false;
		s = CSharesCompute::GetIndexSymbol(1);
		stkKind = 3;
	}
	int n = 0;
	Kline* pKline = NULL;
	n=ReadKline(s,stkKind,pKline);

	m_pKlineIndex = new Kline[this->m_MaxPoint];
	memset(m_pKlineIndex,0,sizeof(Kline)*(this->m_MaxPoint));


	int nRate =1;
	if(m_nKindKline>4&&m_nKindKline<9)
		nRate = 24*60*60;
	else
		nRate = 24*60;
	int j =0;
	for(int i=m_MinPoint;i<m_MaxPoint && j<n;)
	{
		if(m_pData[i].day/nRate == pKline[j].day/nRate)
		{
			memcpy(m_pKlineIndex+i,pKline+j,sizeof(Kline));
			j++;
			i++;
		}
		else if(m_pData[i].day/nRate > pKline[j].day/nRate)
		{
			j++;
		}
		else
		{
			memcpy(m_pKlineIndex+i,pKline+j,sizeof(Kline));
			m_pKlineIndex[i].day = m_pData[i].day;
			i++;
		}
	}
	delete [] pKline;

	return m_pKlineIndex;
}


bool CFormularComputeParent::GetBuySellList()
{
	if(m_pBuySellList!=NULL)
		return true;

	m_pBuySellList = new CBuySellList;
	CTaiKlineFileHS*	pFileHs;
	pFileHs=CTaiKlineFileHS::GetFilePointer (m_symbol,m_stkKind);
	pFileHs->ReadHS (m_symbol,*m_pBuySellList,false);
	if(m_bCreateBsList==false)
		m_bCreateBsList = true;
	else
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

int CFormularComputeParent::ReadKline(CString s,int stkKind, Kline *&pKline)
{
	int n = 0;
	if(m_nKindKline<0)
		return 0;

	if(m_nKindKline==HS_K_LINE||m_nKindKline==MINUTE_1_K_LINE
		||(m_nKindKline==MANY_DAY_K_LINE&&pDoc->m_propertyInitiate.daysOfManyKline<0))
	{
		
		CTaiKlineTransferKline trans;
		trans.TransferKlineHistory(s,stkKind,pKline, n,m_nKindKline,true);

		ASSERT(n>=0&&pKline!=NULL);
	}
	else
	{

		bool bDay = true;
		if(m_nKindKline>0&&m_nKindKline<5)
			bDay = false;
		CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(s,stkKind,bDay) ;
		
		n = pFile->ReadKLine(s,pKline,-1,48)	;

		CTaiKlineTransferKline trans;
		trans.TransferKlineHistory(s,stkKind,pKline, n,m_nKindKline,true);
	}
	return n;


}

void CFormularComputeParent::KlineToLine(Kline *pKline, float & f,int nFlag)
{
	switch(nFlag)
	{
	case 0:
		f = pKline->open;
		break;
	case 1:
		f = pKline->high;
		break;
	case 2:
		f = pKline->low;
		break;
	case 3:
		f = pKline->close;
		break;
	case 4:
		f = pKline->vol;
		break;
	case 5:
		f = pKline->amount;
		break;
	case 6:
		f = pKline->advance;
		break;
	case 7:
		f = pKline->decline;
		break;
	case 8:
		f=  *((float*)&(pKline->advance));
		break;
	case 9:
		f =float(  FABSMY(FABSMY(pKline->vol) - *((float*)&(pKline->decline)))    );
		break;
	}

}

int CFormularComputeParent::GetPeriodKind(CString sIn)
{
	sIn.MakeLower ();
	if(sIn == "hsdata")
		return HS_K_LINE;
	if(sIn == "min1")
		return MINUTE_1_K_LINE;
	if(sIn == "min5")
		return 1;
	if(sIn == "min15")
		return 2;
	if(sIn == "min30")
		return 3;
	if(sIn == "min60")
		return 4;
	if(sIn == "day")
		return 5;
	if(sIn == "week")
		return 6;
	if(sIn == "month")
		return 7;
	if(sIn == "year")
		return 8;
	return 0;
}

void CFormularComputeParent::GetLineNameArray(CString fml, CStringArray &sArray)
{
//
	{
		Kline kline[1000];
		CFormularComputeParent equ(0,999,kline,fml);
		equ.FormularCompute () ;
		for(int i=0;i<equ.GetRlineNum ();i++)
		{
			CString s = equ.GetRlineName (i);
			s.MakeUpper ();
			sArray.Add (s);
		}
	}
//
	{
//	
	}

}


void CFormularComputeParent::TransferRealInfo(int bl, int be, int nKind)
{
	int j;
	float * fp = &(m_cdat1->ystc);
	float f = CTaiShanKlineShowView::GetCapital(this->m_cdat1 );
	BASEINFO* pBase = this->m_cdat1->pBaseInfo;
	float f2 = 0;
	if(pBase)
		f2 = (pBase->mgsy!=0?m_cdat1->ystc / pBase->mgsy:0);
	float fVal[50] = {m_cdat1->ystc ,m_cdat1->opnp ,m_cdat1->higp ,
		m_cdat1->lowp ,m_cdat1->nowp,m_cdat1->totv ,m_cdat1->nowv ,m_cdat1->totp,
		(m_cdat1->totv>0? (m_cdat1->totp /m_cdat1->totv *100):0),(m_cdat1->nowp -m_cdat1->ystc ),(m_cdat1->ystc>0?((m_cdat1->higp -m_cdat1->lowp )/m_cdat1->ystc ):0),(m_cdat1->ystc>0?((m_cdat1->nowp -m_cdat1->ystc )/m_cdat1->ystc) :0),((m_cdat1->accb+m_cdat1->accs)>0?((m_cdat1->accb-m_cdat1->accs)/(m_cdat1->accb+m_cdat1->accs)*100.0):0),
		(m_cdat1->accb-m_cdat1->accs),(((CMainFrame::m_taiShanDoc ->m_nANT[0] * (m_cdat1->volume5 ))!=0)?(m_cdat1->totv / (CMainFrame::m_taiShanDoc ->m_nANT[0]) * (m_cdat1->volume5 )):0),m_cdat1->accb,m_cdat1->accs,m_cdat1->pbuy1 ,
		m_cdat1->psel1 ,m_cdat1->dvol ,m_cdat1->rvol,0,m_cdat1->vbuy1 ,
		m_cdat1->vbuy2 ,m_cdat1->vbuy3,m_cdat1->pbuy1,m_cdat1->pbuy2,m_cdat1->pbuy3,
		m_cdat1->vsel1,m_cdat1->vsel2,m_cdat1->vsel3,m_cdat1->psel1,m_cdat1->psel2,
		m_cdat1->psel3,(f>0?m_cdat1->totv /f:0) , m_cdat1->volume5 ,f2,
						};

	
	for(j=bl;j<=be;j++)	 m_MidArray[m_MidNum].line[j] =fVal[nKind-3];

}

void CFormularComputeParent::GetHsKlineData(int cl,ARRAY_BE &pp)
{
	if(m_nKindKline == HS_K_LINE)
	{
		
		CTaiKlineTransferKline trans(this->m_pBuySellList );
		int n = this->m_pBuySellList ->GetCount();
		Kline* pKline = new Kline[n];
		trans.TransferKlineHistory(m_symbol,m_stkKind,pKline, n,HS_K_LINE,false);
		for(int i=0;i<n;i++)
		   pp.line[i] = pKline[i].open;
		delete [] pKline;
	}
}

bool CFormularComputeParent::LookupWordTable(CString sKey, int &n)
{
	if(m_WordTable.Lookup(sKey,(void*&)n))	return true;
	else if(m_WordTableNew.Lookup(sKey,(void*&)n))	return true;
	return false;
}

bool CFormularComputeParent::LookupParameterTable(int nKey, int &nValue)
{
	if(m_ParameterTable.Lookup(nKey,nValue))	return true;
	else if(m_ParameterTableNew.Lookup(nKey,nValue))	return true;
	return false;

}

void CFormularComputeParent::InitStatic()
{
	static bool bExec = false;
	if(bExec==true) return;

	m_WordTable.InitHashTable( 300 );
	m_LetterTable.InitHashTable( 150 );
	m_ParameterTable.InitHashTable( 100 );

	m_WordTable[_T("*")]  =(void *)6001;
	m_WordTable[_T("/")]  =(void *)6002;
	m_WordTable[_T("*-")]  =(void *)6003;
	m_WordTable[_T("/-")]  =(void *)6004;
	m_WordTable[_T("+")]  =(void *)5001;
	m_WordTable[_T("-")]  =(void *)5002;
	m_WordTable[_T(">")]  =(void *)4001;
	m_WordTable[_T("<")]  =(void *)4002;
	m_WordTable[_T(">=")] =(void *)4005;
	m_WordTable[_T("<=")] =(void *)4006;
	m_WordTable[_T("=")] =(void *)4008;
	m_WordTable[_T("and")] =(void *)3001;
	m_WordTable[_T("or")] =(void *)2001;
	m_WordTable[_T(")")]  =(void *)9002;
	m_WordTable[_T("(")]  =(void *)9001;
	m_WordTable[_T(",")]  =(void *)9005;

	m_WordTable[_T(":")]  =(void *)1001;
	m_WordTable[_T(":=")] =(void *)1002;

	m_WordTable[_T("!=")] =(void *)4009;
	m_WordTable[_T("><")] =(void *)4009;
	m_WordTable[_T("<>")] =(void *)4009;

	m_WordTable[_T("volstick")]    =(void *)9010;
	m_WordTable[_T("stick")]	   =(void *)9011;
	m_WordTable[_T("circledot")]   =(void *)9012;
	m_WordTable[_T("linestick")]   =(void *)9013;
	m_WordTable[_T("crossdot")]    =(void *)9014;
	m_WordTable[_T("colorstick")]  =(void *)9015;
	m_WordTable[_T("buycase")]  =(void *)9016;
	m_WordTable[_T("sellcase")]  =(void *)9017;
	m_WordTable[_T("pointdot")]  =(void *)9012;

	m_WordTable[_T("colorred")]  =(void *)9100;
	m_WordTable[_T("colorgreen")]  =(void *)9101;
	m_WordTable[_T("colorblue")]  =(void *)9102;
	m_WordTable[_T("coloryellow")]  =(void *)9103;
	m_WordTable[_T("colororange")]  =(void *)9104;
	m_WordTable[_T("colorpurple")]  =(void *)9105;
	m_WordTable[_T("colorblack")]  =(void *)9106;
	m_WordTable[_T("colorwhite")]  =(void *)9107;
	m_WordTable[_T("colormagenta")]  =(void *)9108;
	m_WordTable[_T("colorcyan")]  =(void *)9109;

	m_WordTable[_T("linethick1")]  =(void *)9300;
	m_WordTable[_T("linethick2")]  =(void *)9301;
	m_WordTable[_T("linethick3")]  =(void *)9302;
	m_WordTable[_T("linethick4")]  =(void *)9303;
	m_WordTable[_T("linethick5")]  =(void *)9304;
	m_WordTable[_T("linethick6")]  =(void *)9305;
	m_WordTable[_T("linethick7")]  =(void *)9306;

	m_WordTable[_T("linedot")]  =(void *)CONSLINEDOT;


	m_WordTable[_T("open")]    =(void *)9521;
	m_WordTable[_T("close")]   =(void *)9522;
	m_WordTable[_T("high")]    =(void *)9523;
	m_WordTable[_T("low")]     =(void *)9524;
	m_WordTable[_T("o")]    =(void *)9521;
	m_WordTable[_T("c")]   =(void *)9522;
	m_WordTable[_T("h")]    =(void *)9523;
	m_WordTable[_T("l")]     =(void *)9524;
	m_WordTable[_T("advance")] =(void *)9525;
	m_WordTable[_T("weekday")] =(void *)9526;
	m_WordTable[_T("day")]     =(void *)9527;
	m_WordTable[_T("month")]   =(void *)9528;
	m_WordTable[_T("year")]    =(void *)9529;
	m_WordTable[_T("hour")]    =(void *)9530;
	m_WordTable[_T("minute")]  =(void *)9531;
	m_WordTable[_T("amount")]  =(void *)9532;
	m_WordTable[_T("vol")]     =(void *)9533;
	m_WordTable[_T("volume")]     =(void *)9533;

	m_WordTable[_T("v")]     =(void *)9533;
	m_WordTable[_T("isup")]    =(void *)9534;
	m_WordTable[_T("isdown")]  =(void *)9535;
	m_WordTable[_T("isequal")] =(void *)9536;
	m_WordTable[_T("decline")] =(void *)9537;
	m_WordTable[_T("capital")] =(void *)9538;

	m_WordTable[_T("indexo")]    =(void *)9541;
	m_WordTable[_T("indexc")]   =(void *)9542;
	m_WordTable[_T("indexh")]    =(void *)9543;
	m_WordTable[_T("indexl")]     =(void *)9544;
	m_WordTable[_T("indexv")]  =(void *)9545;
	m_WordTable[_T("indexa")] =(void *)9546;
	m_WordTable[_T("indexdec")] =(void *)9547;
	m_WordTable[_T("indexadv")] =(void *)9548;

	m_WordTable[_T("buyvol")] =(void *)9550;
	m_WordTable[_T("sellvol")]  =(void *)9551;
	m_WordTable[_T("isbuyorder")]  =(void *)9552;
	m_WordTable[_T("activebuyvol")]  =(void *)9553;
	m_WordTable[_T("activesellvol")]  =(void *)9554;
	m_WordTable[_T("time")]  =(void *)9560;
	m_WordTable[_T("date")]  =(void *)9561;

	m_WordTable[_T("volunit")]  =(void *)9580;


	m_WordTable[_T("max")]     =(void *)8001;
	m_ParameterTable[8001]         =2;
	m_WordTable[_T("min")]     =(void *)8004;
	m_ParameterTable[8004]         =2;
	m_WordTable[_T("mod")]     =(void *)8005;
	m_ParameterTable[8005]         =2;
	m_WordTable[_T("reverse")] =(void *)8006;
	m_ParameterTable[8006]         =1;
	m_WordTable[_T("if")]      =(void *)8007;
	m_ParameterTable[8007]         =3;

	m_WordTable[_T("abs")]     =(void *)8002;
	m_ParameterTable[8002]         =1;
	m_WordTable[_T("ln")]      =(void *)8008;
	m_ParameterTable[8008]         =1;
	m_WordTable[_T("not")]     =(void *)8009;
	m_ParameterTable[8009]         =1;
	m_WordTable[_T("sgn")]     =(void *)8010;
	m_ParameterTable[8010]         =1;

	m_WordTable[_T("exp")]     =(void *)8003;
	m_ParameterTable[8003]         =1;
	m_WordTable[_T("log")]     =(void *)8011;
	m_ParameterTable[8011]         =1;
	m_WordTable[_T("pow")]     =(void *)8012;
	m_ParameterTable[8012]         =2;
	m_WordTable[_T("sqrt")]     =(void *)8013;
	m_ParameterTable[8013]         =1;


	m_WordTable[_T("avedev")]     =(void *)8030;
	m_ParameterTable[8030]         =2;
	m_WordTable[_T("count")]     =(void *)8031;
	m_ParameterTable[8031]         =2;
	m_WordTable[_T("cross")]     =(void *)8032;
	m_ParameterTable[8032]         =2;

	m_WordTable[_T("devsq")]     =(void *)8033;
	m_ParameterTable[8033]         =2;
	m_WordTable[_T("ema")]     =(void *)8034;
	m_ParameterTable[8034]         =2;
	m_WordTable[_T("hhv")]     =(void *)8035;
	m_ParameterTable[8035]         =2;

	m_WordTable[_T("llv")]     =(void *)8036;
	m_ParameterTable[8036]         =2;
	m_WordTable[_T("sar")]     =(void *)8037;
	m_ParameterTable[8037]         =3;
	m_WordTable[_T("sarturn")]     =(void *)8038;
	m_ParameterTable[8038]         =3;


	m_WordTable[_T("ref")]     =(void *)8039;
	m_ParameterTable[8039]         =2;
	m_WordTable[_T("ma")]     =(void *)8040;
	m_ParameterTable[8040]         =2;
	m_WordTable[_T("std")]     =(void *)8041;
	m_ParameterTable[8041]         =2;

	m_WordTable[_T("stdp")]     =(void *)8042;
	m_ParameterTable[8042]         =2;
	m_WordTable[_T("sum")]     =(void *)8043;
	m_ParameterTable[8043]         =2;
	m_WordTable[_T("varp")]     =(void *)8044;
	m_ParameterTable[8044]         =2;


	m_WordTable[_T("var")]     =(void *)8045;
	m_ParameterTable[8045]         =2;
	m_WordTable[_T("sma")]     =(void *)8046;
	m_ParameterTable[8046]         =3;


	m_WordTable[_T("sumbars")]     =(void *)8047;
	m_ParameterTable[8047]         =2;

	m_WordTable[_T("bidprice")]     =(void *)8048;
	m_ParameterTable[8048]         =1;
	m_WordTable[_T("bidvol")]    =(void *)8049;
	m_ParameterTable[8049]         =1;
	m_WordTable[_T("askprice")]  =(void *)8050;
	m_ParameterTable[8050]         =1;
	m_WordTable[_T("askvol")] =(void *)8051;
	m_ParameterTable[8051]         =1;

	m_WordTable[_T("finance")] =(void *)8052;
	m_ParameterTable[8052]         =1;

	m_WordTable[_T("dynainfo")] =(void *)8053;
	m_ParameterTable[8053]         =1;
	int nParam[]={  5,5,5,2,1,
					1,1,2,2,2,
					2,3,3,3,3,
					2,2,2,2,3,
					3,1,1,3,3,
					1,1,1,1,1,
					1,2,1,1,3,
					2,2,3,3,2,
					3,3,3,3,1,
					1,3,5,3,1,
					4,2,4,2,5,
					3,3,3,3,1,
					2,1,3,3
					};
	CString sName[]={"beeline","radial","wirebar","backset","barscount",
					"barslast","barssince","dma","hhvbars","llvbars",
					"zig2","peak2","peakbars2","trough2","troughbars2",
					"forcast","slope","upward","downward","slowupcross",
					"slowdowncross","floatpercent","floattype","valueday","selfadd",
					"sin","cos","tan","asin","acos",
					"atan","backset2","towervalue","intpart","floatcommerce",
					"reflast","refbegin","valueaverage","floatcommercetype","zig",
					"peak","peakbars","trough","troughbars","winner",
					"cost",	"drawicon","drawline","drawtext","extdata",
					"deployref","deploysum","deploywin","polyline","stickline",
					"SetLineWidth","polylinevalue","polylinetime","between","ceiling",
					"filter","floor","longcross","range"
					};
	for(int j=0;j<44+20;j++)
	{
		m_WordTable[sName[j]] =(void *)(FuncDllB+j);
		m_ParameterTable[FuncDllB+j]         =nParam[j];
	}
	m_WordTable["winnertype"] =(void *)(FuncDllB+14);



	m_LetterTable[_T("0")]  =(void *)3;
	m_LetterTable[_T("1")]  =(void *)3;
	m_LetterTable[_T("2")]  =(void *)3;
	m_LetterTable[_T("3")]  =(void *)3;
	m_LetterTable[_T("4")]  =(void *)3;
	m_LetterTable[_T("5")]  =(void *)3;
	m_LetterTable[_T("6")]  =(void *)3;
	m_LetterTable[_T("7")]  =(void *)3;
	m_LetterTable[_T("8")]  =(void *)3;
	m_LetterTable[_T("9")]  =(void *)3;
	m_LetterTable[_T(".")]  =(void *)3;


	m_LetterTable[_T("a")]  =(void *)4;
	m_LetterTable[_T("b")]  =(void *)4;
	m_LetterTable[_T("c")]  =(void *)4;
	m_LetterTable[_T("d")]  =(void *)4;
	m_LetterTable[_T("e")]  =(void *)4;
	m_LetterTable[_T("f")]  =(void *)4;
	m_LetterTable[_T("g")]  =(void *)4;
	m_LetterTable[_T("h")]  =(void *)4;
	m_LetterTable[_T("i")]  =(void *)4;
	m_LetterTable[_T("j")]  =(void *)4;
	m_LetterTable[_T("k")]  =(void *)4;
	m_LetterTable[_T("l")]  =(void *)4;
	m_LetterTable[_T("m")]  =(void *)4;
	m_LetterTable[_T("n")]  =(void *)4;
	m_LetterTable[_T("o")]  =(void *)4;
	m_LetterTable[_T("p")]  =(void *)4;
	m_LetterTable[_T("q")]  =(void *)4;
	m_LetterTable[_T("r")]  =(void *)4;
	m_LetterTable[_T("s")]  =(void *)4;
	m_LetterTable[_T("t")]  =(void *)4;
	m_LetterTable[_T("u")]  =(void *)4;
	m_LetterTable[_T("v")]  =(void *)4;
	m_LetterTable[_T("w")]  =(void *)4;
	m_LetterTable[_T("x")]  =(void *)4;
	m_LetterTable[_T("y")]  =(void *)4;
	m_LetterTable[_T("z")]  =(void *)4;
	m_LetterTable[_T("_")]  =(void *)4;
	m_LetterTable[_T("#")]  =(void *)4;
	m_LetterTable[_T("$")]  =(void *)4;
	m_LetterTable[_T("@")]  =(void *)4;

	m_LetterTable[_T("+")]  =(void *)5;
	m_LetterTable[_T("-")]  =(void *)5;
	m_LetterTable[_T("*")]  =(void *)5;
	m_LetterTable[_T("/")]  =(void *)5;
	m_LetterTable[_T(">")]  =(void *)5;
	m_LetterTable[_T(",")]  =(void *)5;
	m_LetterTable[_T("<")]  =(void *)5;
	m_LetterTable[_T("=")]  =(void *)5;
	m_LetterTable[_T(")")]  =(void *)5;
	m_LetterTable[_T("(")]  =(void *)5;
	m_LetterTable[_T("!")]  =(void *)5;
	m_LetterTable[_T(":")]  =(void *)5;

	m_LetterTable[_T(" ")]  =(void *)2;
	m_LetterTable[_T("\n")] =(void *)2;
	m_LetterTable[_T("\t")] =(void *)2;
	m_LetterTable[_T("\r")] =(void *)2;
	m_LetterTable[_T("{")] =(void *)1;
	m_LetterTable[_T("}")] =(void *)1;

	m_LetterTable[_T("\"")]  =(void *)6;
	m_LetterTable[_T("'")]  =(void *)7;

	bExec=true;
}

int CFormularComputeParent::FuncFromDll(int n)
{
	INPUT_INFO inputInfo;
	int nParam ;
	LookupParameterTable(n, nParam);
	ASSERT(nParam>=0&&nParam<=10);

	if(this->m_cdat1==NULL)
	{
		pDoc->m_sharesInformation .Lookup (this->m_symbol.GetBuffer(0),this->m_cdat1,m_stkKind);
		m_symbol.ReleaseBuffer();
	}
	ASSERT(this->m_cdat1!=NULL);
	if(m_cdat1==NULL)
		return 8500;
	BASEINFO* pBase = this->m_cdat1->pBaseInfo;
	inputInfo.financeData = pBase;
	inputInfo.klineType  = (klineKind)m_nKindKline;
	inputInfo.pDat1   = m_cdat1;
	inputInfo.strSymbol   = m_symbol.GetBuffer(0);
	m_symbol.ReleaseBuffer();
	inputInfo.nNumData    = m_MaxPoint;
	inputInfo.pData    = m_pData;
	inputInfo.pDataEx     = NULL;
	inputInfo.m_stkKind = m_stkKind;

	if(inputInfo.klineType==HS_KLINE)
	{
		if(m_pKlineEx == NULL) CTaiKlineFileHS::TransferDataEx (m_pBuySellList,m_pKlineEx);
		if(m_pKlineEx != NULL) inputInfo.pDataEx = m_pKlineEx;
	}


	int cl;
	for(int i=0;i<nParam;i++)
	{
		inputInfo.fInputParam [i].line = new float[m_MaxPoint];
		cl=m_table.pop_it();
		GetData(cl,inputInfo.fInputParam [i]);
	}
	if(nParam<10)
		inputInfo.fInputParam [nParam].line = NULL;
	inputInfo.fOutputParam .line =new float[m_MaxPoint];

	
	int nRtn;
	nRtn = FuncInDll(&inputInfo, n-FuncDllB);

	

	m_MidArray[m_MidNum].line = inputInfo.fOutputParam .line ;
	m_MidArray[m_MidNum] = inputInfo.fOutputParam  ;
	
	m_table.push_b(PCMB+m_MidNum);
	m_MidNum++;
	
	for(int i=0;i<nParam;i++)
		delete [] inputInfo.fInputParam [i].line;


	return nRtn;
}
struct ValueFoot{
	int nFoot;
	float fValue;
};

int CFormularComputeParent::FuncInDll(INPUT_INFO *pInput, int nFunc)
{
	bool bAdd;
	int i,j,k,l,m,nRtn=0;
	float f,fTemp;
	if(pInput->nNumData <=0)
		return 800;
	int nParam ;
	LookupParameterTable(nFunc+FuncDllB, nParam);
	ASSERT(nParam>=0&&nParam<=10);
	j = 0;
	for(i = 0;i<nParam;i++)
	{
		if(j<pInput->fInputParam[i].b)
			j=pInput->fInputParam[i].b;
		if(pInput->fInputParam[i].b<0 ||pInput->fInputParam[i].b>pInput->fInputParam[i].e)
		{

			nRtn = 800;
			return nRtn;
		}
	}

	switch(nFunc)
	{
	case 0:
	case 1:
	case 2:
		
		f=pInput->fInputParam[4].line [pInput->fInputParam[4].b];
		k=GetFoot(pInput,0);
		l=k+pInput->fInputParam[3].line [pInput->fInputParam[3].b];
		j=max(pInput->fInputParam[4].b,pInput->fInputParam[2].b);
		if(nFunc>1)
			j=max(j,k);
		if(l!=k)
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam .line [i] = (f-pInput->fInputParam[2].line [pInput->fInputParam[2].b])
					*(i-l)/(l-k)+f;
			}
		else
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam .line [i]=f;
			}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	
	case 6:
		j=pInput->fInputParam[0].b;
		k=j;
		l=j;
		for(i=j;i<pInput->nNumData ;i++)
		{
			pInput->fOutputParam .line [i] =i-k;
			if(pInput->fInputParam[0].line[i]!=0)
			{
				if(j==k)
				{
					l=i;
					k=i;
				}
			}
		}
		pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 7:
		j=pInput->fInputParam[0].b;
		if(pInput->fInputParam[1].b>j)
			j=pInput->fInputParam[1].b;
		f=pInput->fInputParam[1].line [pInput->fInputParam[1].b];
		j++;
		pInput->fOutputParam .line [j-1] = pInput->fInputParam[0].line [j];
		for(i=j;i<pInput->nNumData ;i++)
		{
			f=pInput->fInputParam[1].line [i];

			if(f>1)
			{
				f = 1;
				ASSERT(FALSE);
			}
			pInput->fOutputParam .line [i] =f*pInput->fInputParam[0].line [i]+(1-f)*pInput->fOutputParam .line [i-1];
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;

	case 8:
	case 9:
		j=pInput->fInputParam[0].b;
		k=pInput->fInputParam[1].line [pInput->fInputParam[1].b];
		if(k<0)
			break;
		if(k==0)
		{
			l=j;
			f=pInput->fInputParam[0].line [j];
			for(i=j;i<pInput->nNumData ;i++)
			{
				if(pInput->fInputParam[0].line [i]>f&& nFunc==8
					||pInput->fInputParam[0].line [i]<f&& nFunc==9)
				{
					f=pInput->fInputParam[0].line [i];
					l=i;
				}
				pInput->fOutputParam .line [i] = i-l;
			}
		}
		else
		{
			j+=(k-1);
			for(int ii=j;ii<pInput->nNumData ;ii++)
			{
				l=ii;
				f=pInput->fInputParam[0].line [ii];
				for(i=ii-k+1;i<=ii ;i++)
				{
					if(pInput->fInputParam[0].line [i]>f&& nFunc==8
						||pInput->fInputParam[0].line [i]<f&& nFunc==9)
					{
						f=pInput->fInputParam[0].line [i];
						l=i;
					}
				}
				pInput->fOutputParam .line [ii] = ii-l;
			}
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	
	case 11:
	case 12:
	case 13:
	case 14:
	case 40:
	case 41:
	case 42:
	case 43:
		{
			CArray<ValueFoot,ValueFoot&> fArr;
			FuncZigPre(pInput, nFunc);
			FuncZigGroup(pInput, &fArr);
			if(fArr.GetSize ()==0)
				break;
			j=pInput->fOutputParam .b;
			m=pInput->fInputParam [2].line [pInput->fInputParam [2].b];//m is count
			if(m<1)
				break;
			if(j<0||j>=pInput->nNumData) break;

			float* fp=pInput->fOutputParam.line;
			k=0;
			l=j;

			CArray<ValueFoot,ValueFoot&> fArray;
			ValueFoot valueFo;
			fTemp=fArr[0].fValue ;

	
			for(i=1;i<fArr.GetSize ();i++)
			{
				if(nFunc==11||nFunc==12)
				{
					if(fArr[i].fValue <fArr[i-1].fValue )
					{
						fArray.Add ( fArr[i-1]);
					}
				}
				else if(fArr[i].fValue > fArr[i-1].fValue )
						fArray.Add ( fArr[i-1]);
			}
			if(fArray.GetSize ()<1 || fArr.GetSize ()<1)
				break;
			if(fArr[fArr.GetSize ()-1].nFoot > fArray[fArray.GetSize ()-1].nFoot )
				fArray.Add(fArr[fArr.GetSize ()-1]);
			
			if(fArray.GetSize ()<m)
				break;
			l=fArray[m-1].nFoot+1 ;
			for(i=m-1;i<fArray.GetSize ()-1;i++)
			{
				for(j=fArray[i].nFoot+1;j<=fArray[i+1].nFoot ;j++)
				{
					if(nFunc==11||nFunc==13)
						fp[j]=fArray[i-m+1].fValue;
					else
						fp[j]=j-fArray[i-m+1].nFoot;
				}
			}
		}
		pInput->fOutputParam .b=l;
		break;
	
	case 23:
		{
		k=1;

		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		if(CTaiKlineFileKLine::IsDayKline(pInput->klineType ))
			k=0;
		f=0;
		for(i=j;i<pInput->nNumData ;i++)
		{
			f=pInput->fInputParam[2].line[i];
			CString s=CTime(pInput->pData [i].day ).Format ("%Y%m%d");
			int nDay= atoi(s);
			if(k==0)
			{
				if(pInput->fInputParam[0].line[pInput->fInputParam[0].b]<19700101)
				{
					if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b]*100+pInput->fInputParam[1].line[pInput->fInputParam[1].b]/10000)
						break;
				}
				else if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b])
					break;
			}
			else
			{
				CString s2=CTime(pInput->pData [i].day ).Format ("%H%M");
				int nHour= atoi(s2);

				if(pInput->fInputParam[0].line[pInput->fInputParam[0].b]<19700101)
				{
					if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b]*100+pInput->fInputParam[1].line[pInput->fInputParam[1].b]/10000
						&&nHour>= (int)(pInput->fInputParam[1].line[pInput->fInputParam[1].b])%10000)
						break;
				}
				else if(nDay>= pInput->fInputParam[0].line[pInput->fInputParam[0].b]
					&& nHour>= pInput->fInputParam[1].line[pInput->fInputParam[1].b])
					break;
			}

		}
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i]=f;

		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		}
		break;

	case 24:
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		for(i=j;i<pInput->nNumData ;i++)
		{
			if(pInput->fInputParam[2].line [j] == 2 && i==j) 
			{
				pInput->fOutputParam.line [i] = 100;
				continue;
			}
			if(pInput->fInputParam[2].line [i]>0)
			{
				pInput->fOutputParam.line [i]=
					pInput->fInputParam[0].line [i]+pInput->fInputParam[1].line [i]*(i==j?100:pInput->fOutputParam.line [i-1]);
			}
			else
			{
				pInput->fOutputParam.line [i]=(i==j?100:pInput->fOutputParam.line [i-1]);
			}
		}
		pInput->fOutputParam.b=j;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 25:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = sin(pInput->fInputParam[0].line [i]);
		pInput->fOutputParam.b=j;
		break;
	case 26:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = cos(pInput->fInputParam[0].line [i]);
		pInput->fOutputParam.b=j;
		break;
	case 27:
		j=pInput->fInputParam[0].b;
		try
		{
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = tan(pInput->fInputParam[0].line [i]);
		}
		catch(...)
		{
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = 0;
		}
		pInput->fOutputParam.b=j;
		break;
	case 28:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
		{
			f = pInput->fInputParam[0].line [i];
			if(f>1) f=1;
			if(f<-1)f=-1;
			pInput->fOutputParam.line [i] = asin(f);
		}
		pInput->fOutputParam.b=j;
		break;
	case 29:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
		{
			f = pInput->fInputParam[0].line [i];
			if(f>1) f=1;
			if(f<-1)f=-1;
			pInput->fOutputParam.line [i] = acos(f);
		}
		pInput->fOutputParam.b=j;
		break;
	case 30:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = atan(pInput->fInputParam[0].line [i]);
		pInput->fOutputParam.b=j;
		break;
	case 31:
		k=pInput->fInputParam[1].line [pInput->fInputParam[1].b];

		if(k<0) k=0;

		{
			for(int m = pInput->fInputParam[1].b+1;m<pInput->nNumData ;m++)
			{
				int nn = pInput->fInputParam[1].line [m];
				if(nn>m)
					k = nn;
			}
		}
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b) j= pInput->fInputParam[1].b;
		j+=(k+1);

		for(i=j;i<pInput->nNumData ;i++)
		{
			k=pInput->fInputParam[1].line [i];
			if(k<0) k=0;
			for(int l=-1;l<k;l++)
			{
				pInput->fOutputParam .line [i-l-1] =pInput->fInputParam[0].line[i];
			}
		}
		pInput->fOutputParam.b=j;
		break;
		
	case 32:
		j=pInput->fInputParam[0].b;
		j++;
		if(j<0) j=0;
		if(j>=pInput->nNumData) 
		{
			pInput->fOutputParam.b=-1;
			break;
		}

		k=pInput->fInputParam[0].line [j] ;
		if(k>4 || k<1) 
		{
			pInput->fOutputParam.b=-1;
			break;
		}

		{
			float closeYest = pInput->pData [j-1].close;
			float openYest = pInput->pData [j-1].open;
			float f11[4];
			f11[0] = closeYest,f11[1] = closeYest,f11[2] = closeYest,f11[3] = 1;
			for(i=j;i<pInput->nNumData ;i++)
			{
				float close = pInput->pData [i].close;
				if( openYest <= closeYest )
				{
					if(close>=closeYest)
					{
						f11[0] = close;
						f11[1] = closeYest;
						f11[2] = close;
						f11[3] = 1;

						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close>=openYest)
						{
							f11[0] = close;
							f11[1] = closeYest;
							f11[2] = close;
							f11[3] = 1;

							openYest = close;
						}
						else
						{
							f11[0] = closeYest;
							f11[1] = close;
							f11[2] = openYest;
							f11[3] = 3;

							openYest = closeYest;
							closeYest = close;
						}
					}
				}
				else
				{
					if(close<=closeYest)
					{
						f11[0] = closeYest;
						f11[1] = close;
						f11[2] = close;
						f11[3] = 2;

						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close<=openYest)
						{
							f11[0] = close;
							f11[1] = closeYest;
							f11[2] = closeYest;
							f11[3] = 2;

							openYest = close;
						}
						else
						{
							f11[0] = close;
							f11[1] = closeYest;
							f11[2] = openYest;
							f11[3] = 3;

							openYest = closeYest;
							closeYest = close;
						}
					}
				}
				pInput->fOutputParam .line [i] = f11[k-1];
			}
		}
		pInput->fOutputParam.b=j;
		break;
	case 33:
		j=pInput->fInputParam[0].b;
		for(i=j;i<pInput->nNumData ;i++)
			pInput->fOutputParam.line [i] = int(pInput->fInputParam[0].line [i]);
		pInput->fOutputParam.b=j;
		break;
	case 34:
	case 38:
		k=1;
		while(1)
		{
			if(pInput->pData == NULL)
			{
				k=0;
				break;
			}
			j=pInput->fInputParam[0].b;
			l=j;
			float fLTP;
			float fCJL;
			fLTP=pInput->fInputParam[1].line[0];
			fCJL=pInput->fInputParam[2].line[0];
			float fMax=0;
			float fMin=90000000;
			CTaiKlineMoveChenben::GetMaxMin(pInput->pData , pInput->nNumData , fMax, fMin);
			float fRuler;
			int n =CTaiKlineMoveChenben::GetRuler( fMax,  fMin, fRuler);

			if(n<=0 )
			{
				k=0;
				break;
			}
			CReportData* pdt = NULL;
			if(!CMainFrame::m_taiShanDoc ->m_sharesInformation .Lookup (pInput->strSymbol ,pdt,pInput->m_stkKind))
			{
				k=0;
				break;
			}
			float ltp = CTaiShanKlineShowView::GetCapital(pdt);
			ltp=ltp*fLTP;
			if(pdt->id [1] == 'A')
				ltp = ltp/100;
			if( pdt->pBaseInfo!=NULL && ltp>0 )
			{

				float* cnp = new float[n];
				memset(cnp,0,n*4);

				if(nFunc == 34)
				{
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i,fCJL);
						if(i<l)
						{
							continue;
						}
						f=pInput->fInputParam[0].line[i];//
						if(f>fMax )
						{
							f=0;
						}
						else if(f<fMin)
						{
							f=1;
						}
						else
						{
							int nSeprate = (int)(f*fRuler)-(int)(fMin*fRuler);
							f=0;
							for(int ii=0;ii<=nSeprate;ii++)
							{
								f+=cnp[ii];
							}
							f/=ltp;
							f=1-f;
							if(f<0)
								f=0;
						}
						pInput->fOutputParam .line [i] =f;

					}
				}
				else 
				{
					int nIn =pInput->fInputParam[0].line[l];
					for(i=0;i<pInput->nNumData ;i++)
					{
						CTaiKlineMoveChenben::GetMoveCBPerFoot(pInput->pData+i, cnp, ltp, fMax, fMin, i,fCJL);
						if(i<l)
						{
							continue;
						}
						f=cnp[0];
						float amount=0;
						int nFootMax = 0;
						float max = 0;
						int nBeginY=fMin*fRuler;
						fTemp =cnp[0];
						for(int ii=0;ii<n;ii++)
						{
							amount+=(cnp[ii]*(ii+nBeginY)/(float)fRuler);
							if(cnp[ii]>max)
							{
								nFootMax = ii;
								max=cnp[ii];
							}

						}
						switch(nIn)
						{
						case 1:
							f=(nFootMax+nBeginY)/(float)fRuler;
							break;
						case 2:
							f=cnp[nFootMax];
							break;
						case 3:
							f=amount/ltp;
							break;
						default:

							k=0;
							break;
						}
						pInput->fOutputParam .line [i] =f;

					}
				}
				
			}
			else
			{
				for(i=0;i<pInput->nNumData ;i++)
					pInput->fOutputParam .line [i] = 0;
			}
			break;
		}
		if(k==0)
			pInput->fOutputParam.b=-1;
		else
			pInput->fOutputParam.b=l;
		pInput->fOutputParam.e=pInput->nNumData-1;
		break;
	case 35:
		j=pInput->fInputParam[0].b;
		if( pInput->fInputParam[1].b>=0 && pInput->fInputParam[1].b<pInput->nNumData &&pInput->nNumData>0)
		{
			int nMove = pInput->fInputParam[1].line [ pInput->fInputParam[1].b];
			if(nMove<0) nMove = 0;
			if(nMove>= pInput->nNumData) nMove = pInput->nNumData-1;
			int nFoot = pInput->nNumData-1-nMove;
			if(nFoot<j) nFoot = j;
			for(i=j;i<pInput->nNumData ;i++)
				pInput->fOutputParam.line [i] = (pInput->fInputParam[0].line [nFoot]);
			pInput->fOutputParam.b=j;
		}
		break;
	case 36:
		j=pInput->fInputParam[0].b;
		if( pInput->fInputParam[1].b>=0 && pInput->fInputParam[1].b<pInput->nNumData &&pInput->nNumData>0)
		{
			int nMove = pInput->fInputParam[1].line [ pInput->fInputParam[1].b];
			if(nMove<0) nMove = 0;
			if(nMove>= pInput->nNumData) nMove = (pInput->nNumData-1);
			int nFoot = nMove;
			if(nFoot<j) nFoot = j;
			for(i=j;i<pInput->nNumData ;i++)
				pInput->fOutputParam.line [i] = (pInput->fInputParam[0].line [nFoot]);
			pInput->fOutputParam.b=j;
		}
		break;
	case 37:
		j=pInput->fInputParam[0].b;
		if(pInput->fInputParam[1].b>j)
			j =pInput->fInputParam[1].b;
		if(pInput->fInputParam[2].b>j)
			j =pInput->fInputParam[2].b;
		if(pInput->nNumData>0)
		{
			
			float vol = 0;
			for(i=j;i<pInput->nNumData ;i++)
				vol += (pInput->fInputParam[1].line [i]);
			vol *= pInput->fInputParam[2].line [j];

			
			float* fPrice = new float [pInput->nNumData];
			memcpy(fPrice,pInput->fInputParam[0].line ,sizeof(float)*pInput->nNumData);
			float* fVol = new float [pInput->nNumData];
			memcpy(fVol,pInput->fInputParam[1].line ,sizeof(float)*pInput->nNumData);
			for(i=j;i<pInput->nNumData ;i++)
			{
				for(int k=j;k<pInput->nNumData-1-(i-j);k++)
				{
					if(fPrice[k]>fPrice[k+1]) 
					{
						float ff = fPrice[k];
						fPrice[k] = fPrice[k+1];
						fPrice[k+1] = ff;
						ff = fVol[k];
						fVol[k] = fVol[k+1];
						fVol[k+1] = ff;
					}
				}
			}

		
			float fTot = 0;
			for(i=j;i<pInput->nNumData ;i++)
			{
				fTot+=fVol[i];
				if(fTot>=vol)
				{
					fTot = fPrice[i];
					break;
				}
			}
			delete [] fPrice;
			delete [] fVol;
			if(i!=pInput->nNumData)
			{
				for(i=j;i<pInput->nNumData ;i++)
				{
					pInput->fOutputParam.line [i] =fTot;
				}
				pInput->fOutputParam.b=j;
			}
		}
		break;
	case 46:
	case 48:
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		{
			if(nFunc == 46)
			{
				CString s;
				s.Format("%d",(int)(pInput->fInputParam[2].line[j]));
				pInput->fOutputParam.s = s;
			}
			else
				pInput->fOutputParam.s = pInput->fInputParam[2].s;

			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line [i] = (pInput->fInputParam[1].line [i]);
				if(pInput->fInputParam[0].line [i]>0)
				{
					LooseValue val;
					val.nFoot = i;
					val.fVal = pInput->fInputParam[1].line [i];
					pInput->fOutputParam.looseArr.Add(val);
				}
			}
			if(nFunc == 46)
				pInput->fOutputParam.kind = ARRAY_BE::DrawIcon;
			else
				pInput->fOutputParam.kind = ARRAY_BE::StringData;
			pInput->fOutputParam.b=j;
		}
		break;
	case 47:
		j=pInput->fInputParam[0].b;
		if(j<pInput->fInputParam[1].b)
			j=pInput->fInputParam[1].b;
		if(j<pInput->fInputParam[2].b)
			j=pInput->fInputParam[2].b;
		if(j<pInput->fInputParam[3].b)
			j=pInput->fInputParam[3].b;
		if(j<pInput->fInputParam[4].b)
			j=pInput->fInputParam[4].b;
		{
			int np = 0;
			CString s;
			s.Format("%d",pInput->fInputParam[4].line[j]);
			pInput->fOutputParam.s = s;

			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line [i] =  max(pInput->fInputParam[1].line [i],pInput->fInputParam[3].line [i]);
				if(np ==0)
				{
					if( pInput->fInputParam[0].line [i]>0 )
						{
							LooseValue val;
							val.nFoot = i;
							val.fVal =(pInput->fInputParam[1].line [i]);
							pInput->fOutputParam.looseArr.Add(val);

							np=1;
						}
				}
				else
				{
					if( pInput->fInputParam[2].line [i]>0 )
						{
							LooseValue val;
							val.nFoot = i;
							val.fVal = pInput->fInputParam[3].line [i];
							pInput->fOutputParam.looseArr.Add(val);

							np=0;
						}
				}

			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::DrawLine;
		}
		break;
	case 49:
		j=pInput->fInputParam[0].b;
		if( pInput->fInputParam[0].line[j]>=0)
		{
			if(pInput->pDat1 == 0 )
				break;
			CString s = XgnExtDataDlg::FormatStr(m_cdat1);

			pInput->fOutputParam.b=j;
			if(!XgnExtDataDlg::GetLineBE(pInput->pData ,pInput->nNumData,pInput->fOutputParam,pInput->fInputParam[0].line[j],pInput->pDat1 ))
				pInput->fOutputParam.b = -1;
		}
		break;
	case 50:
		{
			j=pInput->fInputParam[0].b;
			if(pInput->fInputParam[1].b>j)
				j =pInput->fInputParam[1].b;
			if(pInput->fInputParam[2].b>j)
				j =pInput->fInputParam[2].b;
			if(pInput->fInputParam[3].b>j)
				j =pInput->fInputParam[3].b;

			int k = pInput->fInputParam[3].line [j];
			if(k<0) break;
			j += k;

			if(m_nCurrentFoot<j) break;
			if(m_nCurrentFoot>=pInput->nNumData) break;
			int k2 = m_nCurrentFoot-k;

			ARRAY_Right_Box::FanRect(pInput->fInputParam[0].line [k2],pInput->fInputParam[1].line [k2],pInput->fInputParam[2].line [k2], pInput->fOutputParam.lineRight);

			{
			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::LineRt;
		
		}
		break;
	case 51:
		{
			j=pInput->fInputParam[0].b;
			if(pInput->fInputParam[1].b>j)
				j =pInput->fInputParam[1].b;

			int k = pInput->fInputParam[1].line [j];
			if(k<0) break;
			j += k;

			if(m_nCurrentFoot<j) break;
			if(m_nCurrentFoot>=pInput->nNumData) break;

			int k2 = m_nCurrentFoot-k+1;
			if(k<=0) k2 = j;

			for(i = k2 ;i<=m_nCurrentFoot ;i++)
			{
				ARRAY_Right_Box::FanRect(pInput->fInputParam[0].line [i],
				
					pInput->pData [i].low , pInput->pData [i].high , pInput->fOutputParam.lineRight);
			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::LineRt;
		}
		break;
	case 52:
		{
			int k = pInput->fInputParam[1].line [j];
			if(k<0) break;
			j += k;

			int nPre = pInput->fInputParam[3].line[j];
			int nCurrentFoot = m_nCurrentFoot - nPre;
			if(nCurrentFoot<j) break;
			if(nCurrentFoot>=pInput->nNumData) break;

			int k2 = nCurrentFoot-k+1;
			if(k<=0) k2 = j;
			float f3 = pInput->fInputParam[2].line[j];

			ARRAY_Right_Box::FanRect(f3,
					pInput->pData [k2].low , pInput->pData [k2].high , pInput->fOutputParam.lineRight);
			for(i = k2+1 ;i<=nCurrentFoot ;i++)
			{
			
				ARRAY_Right_Box::FanAllPrice(pInput->fInputParam[0].line [i],
					f3 ,pInput->fOutputParam.lineRight);
				ARRAY_Right_Box::FanRect(pInput->fInputParam[0].line [i],
					pInput->pData [i].low , pInput->pData [i].high , pInput->fOutputParam.lineRight);
			}
			pInput->fOutputParam.b=j;
			pInput->fOutputParam.kind = ARRAY_BE::LineRt;
		}
		break;
	case 53:
		j=pInput->fInputParam[0].b;
		if(pInput->fInputParam[1].b>j)
			j =pInput->fInputParam[1].b;
		{
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line [i] = (pInput->fInputParam[1].line [i]);
				if(pInput->fInputParam[0].line [i]>0)
				{
					LooseValue val;
					val.nFoot = i;
					val.fVal = pInput->fInputParam[1].line [i];
					pInput->fOutputParam.looseArr.Add(val);
				}
			}
			pInput->fOutputParam.kind = ARRAY_BE::DrawPolyLine;
			pInput->fOutputParam.b=j;
		}
		break;

	case 54:
		{
			float width = pInput->fInputParam[3].line [j];
			int empty = pInput->fInputParam[4].line [j];
			empty%=10;
			CString ss ;
			ss.Format("%.2f %d",width,empty);
			if(!pInput->fOutputParam.lineWidth)  pInput->fOutputParam.lineWidth = new float [pInput->fInputParam[2].e+1];
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line [i] = (pInput->fInputParam[2].line [i]);
				pInput->fOutputParam.lineWidth[i] = pInput->fInputParam[3].line [i];
				if(pInput->fInputParam[0].line [i]>0)
				{
					LooseValue val;
					val.nFoot = i;
					val.fVal = pInput->fInputParam[1].line [i];
					pInput->fOutputParam.looseArr.Add(val);
					TRACE("STICKLINE = %d\n",i);
				}
			}
			pInput->fOutputParam.s = ss;
			pInput->fOutputParam.kind = ARRAY_BE::DrawStickLine;
			pInput->fOutputParam.b=j;
		}
		break;

	case 55:
		{

		}
		break;
	case 56:
		{
			int nFirst = 0;
			float fPre = 0;
			int nTime = (int) pInput->fInputParam[2].line [j];
			if(nTime<=0) break;

			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line [i] = (pInput->fInputParam[1].line [i]);
				if(pInput->fInputParam[0].line [i]>0)
				{
					if(nFirst < nTime)
					{
						nFirst++;
						if(nFirst == nTime)
							j = i+1;
						else 
							continue;
					}
					pInput->fOutputParam.line [i] = fPre;
					fPre =  pInput->fInputParam[1].line [i];
				}
				else
					pInput->fOutputParam.line [i] = fPre;
			}
			if(nFirst < nTime) break;
			pInput->fOutputParam.b=j;
		}
		break;
	case 57:
		{
			int nFirst = 0;
			int nPre = 0;
			int nTime = (int) pInput->fInputParam[2].line [j];
			if(nTime<=0) break;

			CArray<int ,int&> intArr;
			for(i=j;i<pInput->nNumData ;i++)
			{
				pInput->fOutputParam.line [i] = (pInput->fInputParam[1].line [i]);
				nPre++;
				if(pInput->fInputParam[0].line [i]>0)
				{
					intArr.Add (i);

					if(nFirst < nTime)
					{
						nFirst++;
						if(nFirst == nTime)
							j = i+1;
						else 
							continue;
					}
					pInput->fOutputParam.line [i] = nPre;
					ASSERT(intArr.GetSize ()>=nTime);
					nPre =  i - intArr[intArr.GetSize () - nTime];
				}
				else
					pInput->fOutputParam.line [i] = nPre;
			}
			if(nFirst < nTime) break;
			pInput->fOutputParam.b=j;
		}
		break;

	}
	pInput->fOutputParam.e=pInput->nNumData-1;
	if(pInput->fOutputParam.b<0 || pInput->fOutputParam.b>pInput->fOutputParam.e)
		nRtn = 800;
	return nRtn;

}

int CFormularComputeParent::GetFoot(INPUT_INFO *pInput, int nParam)
{
	int i=(int)(pInput->fInputParam[nParam].line [pInput->fInputParam[nParam].b]+0.5);
	if(i<198001||i>203701)
		return -1;
	int j=pInput->fInputParam[nParam+1].line [pInput->fInputParam[nParam+1].b];
	if(j/10000>31||j%100>=60||j/100%100>24||j<0)
		return -1;

	CTime tm(i/100,i%100,j/10000,j/100%100,j%100,0);
	time_t tmt = tm.GetTime ();
	bool bAdd;
	bool bDay=true;
	if(pInput->klineType <DAY_KLINE || pInput->klineType==HS_KLINE || pInput->klineType==MINUTE_1_K_LINE)
		bDay = false;

	int nRtn = CTaiKlineFileKLine::LookTwoPath((time_t&)tmt ,pInput->pData , pInput->nNumData , bAdd,bDay);

	if(nRtn==-1&&bAdd==true)
		nRtn = 0;
	return nRtn;

}

int CFormularComputeParent::FuncZigGroup(INPUT_INFO *pInput, void* pArray)
{
	int i,j,k,l,m,nMaxFoot,nMinFoot,nRtn=0;
	float f,fTemp,max,min,fOld1,fOld2;
	float* fp;

	CArray<ValueFoot,ValueFoot&>* pArr=(CArray<ValueFoot,ValueFoot&>*)pArray;

	j=pInput->fInputParam[0].b;
	fp=pInput->fInputParam[0].line ;
	fTemp=pInput->fInputParam[1].line [pInput->fInputParam[1].b]/(float)100.0;
	l=0;
	m=j;
	ValueFoot valueFo;
	valueFo.fValue = fp[j];
	valueFo.nFoot  = j;
	pArr->Add(valueFo);
	max = fp[j];
	min = fp[j];
	fOld1=fp[j];
	fOld2=fp[j];
	nMaxFoot = j;
	nMinFoot = j;

	//lmb add 20010326
	for(i=j+1;i<pInput->nNumData ;i++)
	{
	
		if(fOld1==0||fOld2==0)
			break;
	
		if((max-fp[i])/fp[i] >fTemp && fp[i]<max && (l==0 || l==2))
		{
			valueFo.fValue = max;
			valueFo.nFoot  = nMaxFoot;
			pArr->Add(valueFo);

			max = fp[i];
			min = fp[i];
			nMaxFoot = i;
			nMinFoot = i;
			l=1;

		}

		
		if((fp[i]-min)/fp[i] >fTemp && fp[i]>min&& (l==0 || l==1))
		{
			valueFo.fValue = min;
			valueFo.nFoot  = nMinFoot;
			pArr->Add(valueFo);

			max = fp[i];
			min = fp[i];
			nMaxFoot = i;
			nMinFoot = i;
			l=2;

		}
		if(fp[i]>max)
		{
			nMaxFoot = i;
			max= fp[i];
		}
		if(fp[i]<min)
		{
			nMinFoot = i;
			min= fp[i];
		}

	}


	if(pArr->GetSize ()-1>=0)
	{
		valueFo=pArr->GetAt(pArr->GetSize ()-1);
		if(valueFo.nFoot <pInput->nNumData-1)
		{
			valueFo.fValue = fp[pInput->nNumData-1];
			valueFo.nFoot =pInput->nNumData-1;
			pArr->Add(valueFo);
		}
	}
	pInput->fOutputParam.b=j;


	return 0;
}

int CFormularComputeParent::RelolveNnet(int cl)
{
	return 1;
}

int CFormularComputeParent::GetBeginFoot(ARRAY_BE &ln2)
{
	int ret =ln2.b;
	if(ret<0) return -1;
	int nFoot = (int)ln2.line[ret];
	ret =(int)ln2.line[ret];
	for(int i=ln2.b;i<=ln2.e;i++)
	{
		if((int)ln2.line[i]>ret) 
			ret = (int)ln2.line[i];
	}
	if(ret == nFoot) return ret;

	if(ret<1) ret = 1;

	return ret;
}

int CFormularComputeParent::GetIndexPoint(CString sName, float &nPoint)
{
	int fl=1;
	for(int i=0;i<m_RlineNum;i++)
	{
		CString s1 = m_RlineName[i];
		s1.MakeLower ();
		CString s2 = sName;
		s2.MakeLower ();
		if(s1==s2)
		{
			fl=0;
			int bl = m_Rline[i].b;
			int be = m_Rline[i].e;
			if(bl>be||bl<0||be<0||be>=m_MaxPoint)
				return 1;

			nPoint = m_Rline[i].line[be];
			break;
		}
	}
	return fl;

}

int CFormularComputeParent::CalcMin5InDay(CFormularContent *pIndex,int nParamInTable,CString linename,int tu)
{
	return 1;
}

int CFormularComputeParent::GetStockKind(char *strLabel)
{
	 CReportData *pStockData;
	 int stkKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strLabel);
	 bool bFind = false;
	 {
		 if(stkKind>=0)
			 if(CMainFrame::m_taiShanDoc ->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
				 bFind = true;
	 }
	 if(bFind == false)
	 {
		 stkKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strLabel);
		 if(stkKind>=0)
			 if(CMainFrame::m_taiShanDoc->m_sharesInformation .Lookup (strLabel,pStockData,stkKind))
				 bFind = true;
	 }
	 return stkKind;

}

void CFormularComputeParent::FuncZigPre(INPUT_INFO *pInput, int nFunc)
{
	if(nFunc>20)
	{
		int j=pInput->fInputParam[0].b;
		int f2 = (int)(pInput->fInputParam[0].line [j]);
		if(f2<0)f2 = 0;
		if(f2>3)f2 = 3;

		for(int i=j;i<pInput->nNumData ;i++)
		{
			switch(f2)
			{
			case 0:
				pInput->fInputParam[0].line [i] = pInput->pData [i].open ;
				break;
			case 1:
				pInput->fInputParam[0].line [i] = pInput->pData [i].high ;
				break;
			case 2:
				pInput->fInputParam[0].line [i] = pInput->pData [i].low ;
				break;
			case 3:
				pInput->fInputParam[0].line [i] = pInput->pData [i].close ;
				break;
			}
		}
	}
}

void CFormularComputeParent::FanEach(ARRAY_BE &lineIn, int nFoot, ARRAY_BE &lineOut)
{
}
bool CFormularComputeParent::FanEach(CFormularContent * pJishu)
{
	CString formal = pJishu->fomular ;
	formal.MakeLower();
	Kline kline;
	CFormularComputeParent equation(0,0,&kline,formal);
	equation.Devide ();
	bool b = equation.m_bRightbox ;
	pJishu->bRightBox = 0;
	if(b)
		pJishu->bRightBox = 1;
	return b;
}

int CFormularComputeParent::FuncInDllExt(INPUT_INFO *pInput, int nFunc, int j)
{
	return 0;
}
