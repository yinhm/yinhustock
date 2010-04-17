// DIAEDITZBGSHS.cpp : implementation file
//

#include "stdafx.h"
#include "DIAEDITZBGSHS.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CTaiKlineIndicatorsManage.h"
#include "CTaiShanApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDialogEDITZBGSHS::CDialogEDITZBGSHS(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEDITZBGSHS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEDITZBGSHS)
	m_sExplain = _T("");
	//}}AFX_DATA_INIT
}


void CDialogEDITZBGSHS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEDITZBGSHS)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_EDIT1, m_sExplain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEDITZBGSHS, CDialog)
	//{{AFX_MSG_MAP(CDialogEDITZBGSHS)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(NM_RETURN, IDC_TREE1, OnReturnTree1)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


dataFunc stritemsArr[] = 
	{
	"行情函数",-1,"",

	"ACTIVEBUYVOL---主动性买量",0,"主动性买量",
	"ACTIVESELLVOL---主动性卖量",0,"主动性卖量",
	"ADVANCE---上涨股票数(大盘指标)",0,"取得该周期上涨家数。\r\n用法:\r\nADVANCE\r\n(本函数仅对大盘有效)",
	"AMOUNT---成交额",0,"取得该周期成交额。\r\n用法:\r\nAMOUNT",
	"ASKPRICE(1)---委卖价1",0,"委卖价1(本函数仅个股在分笔成交分析周期有效)",
	"ASKPRICE(2)---委卖价2",0,"委卖价2(本函数仅个股在分笔成交分析周期有效)",
	"ASKPRICE(3)---委卖价3",0,"委卖价3(本函数仅个股在分笔成交分析周期有效)",
	"ASKVOL(1)---委卖量1",0,"委卖量1(本函数仅个股在分笔成交分析周期有效)",
	"ASKVOL(2)---委卖量2",0,"委卖量2(本函数仅个股在分笔成交分析周期有效)",
	"ASKVOL(3)---委卖量3",0,"委卖量3(本函数仅个股在分笔成交分析周期有效)",
	"BIDVOL(1)---委买量1",0,"委买量1(本函数仅个股在分笔成交分析周期有效)",
	"BIDVOL(2)---委买量2",0,"委买量2(本函数仅个股在分笔成交分析周期有效)",
	"BIDVOL(3)---委买量3",0,"委买量3(本函数仅个股在分笔成交分析周期有效)",
	"BIDPRICE(1)---委买价1",0,"委买价1(本函数仅个股在分笔成交分析周期有效)",
	"BIDPRICE(2)---委买价2",0,"委买价2(本函数仅个股在分笔成交分析周期有效)",
	"BIDPRICE(3)---委买价3",0,"委买价3(本函数仅个股在分笔成交分析周期有效)",
	"BUYVOL---换手数据的主动性买量",0,"取得主动性买单量。\r\n用法:\r\nBUYVOL\r\n当本笔成交为主动性买盘时,其数值等于成交量,否则为0\r\n(本函数仅个股在分笔成交分析周期有效)\r\n",
	"CLOSE---收盘价",0,"取得该周期收盘价。\r\n用法:\r\nCLOSE\r\n",
	"DECLINE---下跌股票数(大盘指标)",0,"取得该周期下跌家数。\r\n用法:\r\nDECLINE\r\n(本函数仅对大盘有效)",
	"HIGH---最高价",0,"取得该周期最高价。\r\n用法:\r\nHIGH",
	"ISBUYORDER---是否是买单",0,"取得该成交是否为主动性买单。\r\n用法:\r\nISBUYORDER\r\n当本笔成交为主动性买盘时,返回1,否则为0\r\n(本函数仅个股在分笔成交分析周期有效)",
	"LOW---最低价",0,"取得该周期最低价。\r\n用法:\r\nLOW",
	"OPEN---开盘价",0,"取得该周期开盘价。\r\n用法:\r\nOPEN",
	"SELLVOL---换手数据的主动性卖量",0,"取得主动性卖单量。\r\n用法:\r\nBUYVOL\r\n当本笔成交为主动性卖盘时,其数值等于成交量,否则为0\r\n(本函数仅个股在分笔成交分析周期有效)\r\n",
	"VOL---成交量",0,"取得该周期成交量。\r\n用法:\r\nVOL",

	"时间函数",-1,"",
	"DAY---日期",0,"取得该周期的日期。\r\n用法:\r\nDAY\r\n函数返回有效值范围为(1-31)",
	"DATE---年月日,取得该周期从1900以来的年月日",0,"取得该周期从1900以来的年月日。\r\n用法:\r\nDATE\r\n函数返回有效值范围为(700101-1341231),表示19700101-20341231",
	"HOUR---小时",0,"取得该周期的小时数。\r\n用法:\r\nHOUR\r\n函数返回有效值范围为(0-23)，对于日线及更长的分析周期值为0",
	"MINUTE---分钟",0,"取得该周期的分钟数。\r\n用法:\r\nMINUTE\r\n函数返回有效值范围为(0-59)，对于日线及更长的分析周期值为0",
	"MONTH---月份",0,"取得该周期的月份。\r\n用法:\r\mONTH\r\n函数返回有效值范围为(1-12)",
	"TIME---时分秒",0,"取得该周期的时分秒。\r\n用法:\r\nTIME\r\n函数返回有效值范围为(000000-235959)",
	"VALUEDAY(DAY,HOUR,X)---DAY年月或年月日，HOUR日时分或时分的X的值。",0,"如：VALUEDAY(200103,210930,CLOSE) 表示2001年3月21日9点30分时的收盘价格。",
	"WEEK---星期",0,"取得该周期的月份。\r\n用法:\r\wEEK\r\n函数返回有效值范围为(0-6)",
	"YEAR---年份",0,"取得该周期的年份。\r\n用法:\r\yEAR\r\n函数返回有效值范围为(1970-2038)",

	"引用函数",-1,"",
	"BACKSET(X,N)---若X非0,则将当前位置到N周期前的数值设为1",0,"将当前位置到若干周期前的数据设为1。\r\n用法:\r\nBACKSET(X,N),若X非0,则将当前位置到N周期前的数值设为1。\r\n例如：BACKSET(CLOSE>OPEN,2)若收阳则将该周期及前一周期数值设为1,否则为0",
	"BARSCOUNT(X)---第一个有效数据到当前的天数",0,"求总的周期数。\r\n用法:\r\nBARSCOUNT(X)第一个有效数据到当前的天数\r\n例如：BARSCOUNT(CLOSE)对于日线数据取得上市以来总交易日数，对于分笔成交取得当日成交笔数，对于1分钟线取得当日交易分钟数",
	"BARSLAST(X)---上一次X不为0到现在的天数",0,"上一次条件成立到当前的周期数。\r\n用法:\r\nBARSLAST(X):上一次X不为0到现在的天数\r\n例如：BARSLAST(CLOSE/REF(CLOSE,1)>=1.1)表示上一个涨停板到当前的周期数",
	"BARSSINCE(X)---第一次X不为0到现在的天数",0,"例如：BARSSINCE(HIGH>10)表示股价超过10元时到当前的周期数",
	"COUNT(X,N)---统计N周期中满足X条件的周期数,若N=0则从第一个有效值开始",0,"第一个条件成立到当前的周期数。\r\n用法:\r\nBARSSINCE(X):第一次X不为0到现在的天数\r\n例如：BARSSINCE(HIGH>10)表示股价超过10元时到当前的周期数",
	"DMA(X,A)---求X的动态移动平均",0,"求动态移动平均。\r\n用法:\r\nDMA(X,A),求X的动态移动平均。\r\n算法: 若Y=DMA(X,A)\r\n则 Y=A*X+(1-A)*Y',其中Y'表示上一周期Y值,A必须小于1。\r\n例如：DMA(CLOSE,VOL/CAPITAL)表示求以换手率作平滑因子的平均价",
	"EMA(X,N)---计算X的N天指数平滑移动平均值",0,"求指数平滑移动平均。\r\n用法:\r\nEMA(X,N),求X的N日指数平滑移动平均。算法：若Y=EMA(X,N)\r\n则Y=[2*X+(N-1)*Y']/(N+1),其中Y'表示上一周期Y值。\r\n例如：EMA(CLOSE,30)表示求30日指数平滑均价",
	"HHV(X,N)---计算N天中X的最高值",0,"求最高值。\r\n用法:\r\nHHV(X,N),求N周期内X最高值,N=0则从第一个有效值开始。\r\n例如：HHV(HIGH,30)表示求30日最高价",
	"HHVBARS(X,N)---求N周期内X最高值到当前周期数，N=0表示从第一个有效值开始统计",0,"求上一高点到当前的周期数。\r\n用法:\r\nHHVBARS(X,N):求N周期内X最高值到当前周期数，N=0表示从第一个有效值开始统计\r\n例如：HHVBARS(HIGH,0)求得历史新高到到当前的周期数",
	"LLV(X,N)---计算N天中X的最低值",0,"求最低值。\r\n用法:\r\nLLV(X,N),求N周期内X最低值,N=0则从第一个有效值开始。\r\n例如：LLV(LOW,0)表示求历史最低价",
	"LLVBARS(X,N)---求N周期内X最低值到当前周期数，N=0表示从第一个有效值开始统计",0,"求上一低点到当前的周期数。\r\n用法:\r\nLLVBARS(X,N):求N周期内X最低值到当前周期数，N=0表示从第一个有效值开始统计\r\n例如：LLVBARS(HIGH,20)求得20日最低点到当前的周期数",
	"MA(X,N)---计算X的N天移动平均值",0,"求简单移动平均。\r\n用法:\r\nMA(X,N),求X的N日移动平均值。算法：(X1+X2+X3+...+Xn)/N\r\n例如：MA(CLOSE,10)表示求10日均价",
	"REF(X,N)---计算X的N天前的值",0,"引用若干周期前的数据。\r\n用法:\r\nREF(X,A),引用A周期前的X值。\r\n例如：REF(CLOSE,1)表示上一周期的收盘价，在日线上就是昨收",
	"REFBEGIN(X,N)---计算X的所有数据的第N个数据的值",0,"例如：REFBEGIN(CLOSE,1)表示CLOSE的所有数据的第1个数据的值",
	"REFLAST(X,N)---计算X的所有数据的倒数第N个数据的值",0,"例如：REFLAST(CLOSE,1)表示CLOSE的所有数据的倒数第1个数据的值",
	"SMA(X,N,M)---求X的N日移动平均，M为权重",0,"求移动平均。\r\n用法:\r\nSMA(X,N,M),求X的N日移动平均，M为权重。\r\n算法: 若Y=SMA(X,N,M)\r\n则 Y=[M*X+(N-M)*Y')/N,其中Y'表示上一周期Y值,N必须大于M。\r\n例如：SMA(CLOSE,30,1)表示求30日移动平均价",
	"SUM(X,N)---计算N天中X的总和值",0,"求总和。\r\n用法:\r\nSUM(X,N),统计N周期中X的总和,N=0则从第一个有效值开始。\r\n例如：SUM(VOL,0)表示统计从上市第一天以来的成交量总和",
	"SUMBARS(X,A)---将X向前累加直到大于等于A,返回这个区间的周期数",0,"向前累加到指定值到现在的周期数。\r\n用法:\r\nSUMBARS(X,A):将X向前累加直到大于等于A,返回这个区间的周期数\r\n例如：SUMBARS(VOL,CAPITAL)求完全换手到现在的周期数",
	"EXTDATA(N)---引用ID号为N的扩展指标数据",0,"取得日线扩展数据(1-999)。\r\n用法:\r\nEXTDATA(N),N取1--999\r\n(本函数仅在日线分析周期有效)",


	"算术函数",-1,"",
	"ABS(X)---X的绝对值",0,"求绝对值。\r\n用法:\r\nABS(X)返回X的绝对值\r\n例如：ABS(-34)返回34",
	"BETWEEN(A,B,C)---表示A处于B和C之间时返回1，否则返回0",0,"介于两个数之间。\r\n用法:\r\nBETWEEN(A,B,C)表示A处于B和C之间时返回1，否则返回0\r\n例如：BETWEEN(CLOSE,MA(CLOSE,10),MA(CLOSE,5))表示收盘价介于5日均线和10日均线之间",
	"CROSS(A,B)---A线向上穿越B线为1,否则为0",0,"两条线交叉。\r\n用法:\r\nCROSS(A,B)表示当A从下方向上穿过B时返回1，否则返回0\r\n例如：CROSS(MA(CLOSE,5),MA(CLOSE,10))表示5日均线与10日均线交金叉",
	"FILTER(X,N)---X满足条件后，删除其后N周期内的数据置为0",0,"过滤连续出现的信号。\r\n用法:\r\nFILTER(X,N):X满足条件后，删除其后N周期内的数据置为0\r\n例如：FILTER(CLOSE>OPEN,5)查找阳线，5天内再次出现的阳线不被记录在内",
	"LONGCROSS(A,B,N)---表示A在N周期内都小于B，本周期从下方向上穿过B时返回1，否则返回0",0,"例如：LONGCROSS(MA(CLOSE,5),MA(CLOSE,10),5)表示5日均线维持5周期后与10日均线交金叉",
	"MAX(X,N)---最大值函数",0,"求最大值。\r\n用法:\r\nMAX(A,B)返回A和B中的较大值\r\n例如：MAX(CLOSE-OPEN,0)表示若收盘价大于开盘价返回它们的差值，否则返回0",
	"MIN(X,N)---最小值函数",0,"求最小值。\r\n用法:\r\nMIN(A,B)返回A和B中的较小值\r\n例如：MIN(CLOSE,OPEN)返回开盘价和收盘价中的较小值",
	"MOD(A,B)---取得A对B的模",0,"求模运算。\r\n用法:\r\nMOD(A,B)返回A对B求模\r\n例如：MOD(26,10)返回6",
	"NOT(X)---取得逻辑非",0,"求逻辑非。\r\n用法:\r\nNOT(X)返回非X,即当X=0时返回1，否则返回0\r\n例如：NOT(ISUP)表示平盘或收阴\r\n",
	"RANGE(A,B,C)---表示A大于B同时小于C时返回1，否则返回0",0,"介于某个范围之间。\r\n用法:\r\nRANGE(A,B,C)表示A大于B同时小于C时返回1，否则返回0\r\n例如：RANGE(CLOSE,MA(CLOSE,5),MA(CLOSE,10))表示收盘价大于5日均线并且小于10日均线",
	"REVERSE(X)---取得X的相反数-X",0,"求相反数。\r\n用法:\r\nREVERSE(X)返回-X\r\n例如REVERSE(CLOSE)返回-CLOSE",
	"SGN(X)---取得X的符号，返回(1,0,-1)",0,"求符号值。\r\n用法:\r\nSGN(X)，当X>0,X=0,X<0分别返回1,0,-1\r\n",

	"数学函数",-1,"",
	"ACOS(X)---返回X的反余弦值",0,"反余弦值",
	"ASIN(X)---返回X的反正弦值",0,"反正弦值",
	"ATAN(X)---返回X的反正切值",0,"反正切值",
	"CEILING(X)---向上舍入",0,"例如：CEILING(12.3)求得13,CEILING(-3.5)求得-3",
	"COS(X)---返回X的余弦值",0,"余弦值",
	"EXP(X)---以e为底的X次幂",0,"EXP(CLOSE)返回e的CLOSE次幂",
	"FLOOR(X)---向下舍入",0,"例如：FLOOR(12.3)求得12,FLOOR(-3.5)求得-4",
	"INTPART(X)---X的整数",0,"例如：INTPART(12.3)求得12,INTPART(-3.5)求得-3",
	"LN(X)---X的以e为底的对数",0,"例如：LN(CLOSE)求收盘价的对数",
	"LOG(X)---X的以10为底的对数",0,"例如：LOG(100)等于2",
	"POW(A,B)---A的B次幂",0,"例如：POW(CLOSE,3)求得收盘价的3次方",
	"SIN(X)---返回X的正弦值",0,"正弦值",
	"SQRT(X)---X的平方根",0,"例如：SQRT(CLOSE)收盘价的平方根",
	"TAN(X)---返回X的正切值",0,"正切值",

	"逻辑函数",-1,"",
	"IF(X,A,B)---若X则返回A,否则返回B",0,"根据条件求不同的值。\r\n用法:\r\nIF(X,A,B)若X不为0则返回A,否则返回B\r\n例如：IF(CLOSE>OPEN,HIGH,LOW)表示该周期收阳则返回最高值，否则返回最低值",
	"ISDOWN---当日下跌则为1,否则为0",0,"当收盘<开盘时，返回值为1，否则为0",
	"ISEQUAL---当日平盘则为1,否则为0",0,"当收盘=开盘时，返回值为1，否则为0",
	"ISUP---当日上涨则为1,否则为0",0,"当收盘>开盘时，返回值为1，否则为0",

	"统计函数",-1,"",
	"AVEDEV(X,N)---计算X的N天的平均绝对偏差",0,"平均绝对偏差。\r\n用法:\r\nAVEDEV(X,N)\r\n",
	"DEVSQ(X,N)---计算X的N天的偏差平方和",0,"数据偏差平方和。\r\n用法:\r\nDEVSQ(X,N)\r\n",
	"FORCAST(X,N)---X的N周期线性回归预测值",0,"线性回归预测值。\r\n用法:\r\nFORCAST(X,N)为X的N周期线性回归预测值\r\n例如:FORCAST(CLOSE,10)表示求10周期线性回归预测本周期收盘价",
	"STD(X,N)---计算X的N天的估算标准差",0,"估算标准差。\r\n用法:\r\nSTD(X,N)为X的N日估算标准差\r\n",
	"STDP(X,N)---计算X的N天的总体标准差",0,"总体标准差。\r\n用法:\r\nSTDP(X,N)为X的N日总体标准差\r\n",
	"SLOPE(X,N)---X的N周期线性回归线的斜率",0,"线性回归斜率。\r\n用法:\r\nSLOPE(X,N)为X的N周期线性回归线的斜率\r\n例如:SLOPE(CLOSE,10)表示求10周期线性回归线的斜率\r\n",
	"VALUEAVERAGE( PRICE,VOL,PERCENT)---根据VOL、PERCENT计算PRICE的均价",0,"",
	"VAR(X,N)---计算X的N天的估算样本方差",0,"估算样本方差。\r\n用法:\r\nVAR(X,N)为X的N日估算样本方差\r\n",
	"VARP(X,N)---计算X的N天的总体样本方差",0,"总体样本方差。\r\n用法:\r\nVARP(X,N)为X的N日总体样本方差",

	"指标函数",-1,"",
    "COST(N)---表示N%获利盘的价格是多少",0,"成本分布情况。\r\n用法:\r\nCOST(10),表示10%获利盘的价格是多少，即有10%的持仓量在该价格以下，其余90%在该价格以上，为套牢盘\r\n该函数仅对日线分析周期有效",
	"DOWNWARD(X,N)---X连续N日下降",0,"例如：DOWNWARD(CLOSE,5)表示CLOSE连续5日下降",
	"PEAK(X,N,M)---前M个ZIG转向波峰值",0,"前M个ZIG转向波峰值。\r\n用法:\r\nPEAK(K,N,M)表示之字转向ZIG(K,N)的前M个波峰的数值,M必须大于等于1\r\n例如：PEAK(1,5,1)表示%5最高价ZIG转向的上一个波峰的数值",
	"PEAKBARS(X,N,M)---前M个ZIG转向波峰到当前距离",0,"前M个ZIG转向波峰到当前距离。\r\n用法:\r\nPEAKBARS(K,N,M)表示之字转向ZIG(K,N)的前M个波峰到当前的周期数,M必须大于等于1\r\n例如：PEAK(0,5,1)表示%5开盘价ZIG转向的上一个波峰到当前的周期数",
	"SAR(N,S,M)---抛物转向,N:天数 S:步长 M:极值",0,"抛物转向。\r\n用法:\r\nSAR(N,S,M),N为计算周期,S为步长,M为极值\r\n例如SAR(10,2,20)表示计算10日抛物转向，步长为2%，极限值为20%",
	"SARTURN(N,S,M)---抛物转向位置,N:天数 S:步长 M:极值",0,"抛物转向点。\r\n用法:\r\nSARTURN(N,S,M),N为计算周期,S为步长,M为极值,若发生向上转向则返回1，若发生向下转向则返回-1，否则为0\r\n其用法与SAR函数相同",
	"SLOWDOWNCROSS(N, X1,X2)---X1维持N周期后下穿X2",0,"X1维持N周期后下穿X2",
	"SLOWUPCROSS(N, X1,X2)---X1维持N周期后上穿X2",0,"X1维持N周期后上穿X2",
	"TROUGH(X,N,M)---表示之字转向ZIG(X,N)的前M个波谷的数值,M必须大于等于1",0,"前M个ZIG转向波谷值。\r\n用法:\r\nTROUGH(K,N,M)表示之字转向ZIG(K,N)的前M个波谷的数值,M必须大于等于1\r\n例如：TROUGH(2,5,2)表示%5最低价ZIG转向的前2个波谷的数值",
	"TROUGHBARS(X,N,M)---表示之字转向ZIG(X,N)的前M个波谷到当前的周期数,M必须大于等于1",0,"例如：TROUGH(2,5,2)表示%5最低价ZIG转向的前2个波谷到当前的周期数",
	"UPWARD(X,N)---X连续N日上升",0,"X连续N日上升",
	"ZIG(X,N)---当价格变化量超过N%时转向,X表示输入数值",0,"之字转向。\r\n用法:\r\nZIG(K,N),当价格变化量超过N%时转向,K表示0:开盘价,1:最高价,2:最低价,3:收盘价\r\n例如：ZIG(3,5)表示收盘价的5%的ZIG转向",
	"TOWERVALUE(N)---宝塔线的各种数值，N = 1表示宝塔线的最高值，N = 2表示宝塔线的最低值，\r\nN = 3表示宝塔线的中间值，N = 4表示宝塔线的状态（1为红色，2为绿色，3为即有红又有绿色）",0,"宝塔线的各种数值",
	"WINNER(X)---分布在X(指标或行情数据)以上的所有价位的成交量的和占总流通盘的百分比",0,"用法:\r\nWINNER(CLOSE),表示以当前收市价卖出的获利盘比例，例如返回0.1表示10%获利盘；WINNER(10.5)表示10.5元价格的获利盘比例\r\n该函数仅对日线分析周期有效",
	"WINNERTYPE(N)---流通盘分布类函数，N=1为最高峰处的价格，N=2为最高峰处的成交量密度（手/分），N=3为平均成本处的价格",0,"流通盘分布类函数",

	"大盘函数",-1,"",
	"INDEXA---指数成交额",0,"指数成交额",
	"INDEXADV---大盘上涨家数",0,"大盘上涨家数",
	"INDEXC---指数收盘价",0,"指数收盘价",
	"INDEXDEC---大盘下跌家数",0,"大盘下跌家数",
	"INDEXH---指数最高价",0,"指数最高价",
	"INDEXL---指数最低价",0,"指数最低价",
	"INDEXO---指数开盘价",0,"指数开盘价",
	"INDEXV---指数成交量",0,"指数成交量",

	"常数函数",-1,"",
	"CAPITAL---流通盘",0,"流通盘",
	"VOLUNIT---每手股数",0,"每手股数",
	"DYNAINFO(3)---行情(昨天收盘)",0,"昨天收盘",
	"DYNAINFO(4)---行情(今日开盘)",0,"今日开盘",
	"DYNAINFO(5)---行情(今日最高)",0,"今日最高",
	"DYNAINFO(6)---行情(今日最低)",0,"今日最低",
	"DYNAINFO(7)---行情(最新价格)",0,"最新价格",
	"DYNAINFO(8)---行情(总成交量)",0,"总成交量",
	"DYNAINFO(9)---行情(最新成交量)",0,"最新成交量",
	"DYNAINFO(10)---行情(总成交金额)",0,"总成交金额",
	"DYNAINFO(11)---行情(均价)",0,"均价",
	"DYNAINFO(12)---行情(涨跌)",0,"涨跌",
	"DYNAINFO(13)---行情(振幅)",0,"振幅",
	"DYNAINFO(14)---行情(涨幅)",0,"涨幅",
	"DYNAINFO(15)---行情(委比)",0,"委比",
	"DYNAINFO(16)---行情(委差)",0,"委差",
	"DYNAINFO(17)---行情(量比)",0,"量比",
	"DYNAINFO(18)---行情(委买)",0,"委买",
	"DYNAINFO(19)---行情(委卖)",0,"委卖",
	"DYNAINFO(20)---行情(委买价)",0,"委买价",
	"DYNAINFO(21)---行情(委卖价)",0,"委卖价",
	"DYNAINFO(22)---行情(内盘)",0,"内盘",
	"DYNAINFO(23)---行情(外盘)",0,"外盘",
	"DYNAINFO(24)---行情(涨速)",0,"涨速",

	"DYNAINFO(25)---行情(买入量一)",0,"买入量一",
	"DYNAINFO(26)---行情(买入量二)",0,"买入量二",
	"DYNAINFO(27)---行情(买入量三)",0,"买入量三",
	"DYNAINFO(28)---行情(买入价一)",0,"买入价一",
	"DYNAINFO(29)---行情(买入价二)",0,"买入价二",
	"DYNAINFO(30)---行情(买入价三)",0,"买入价三",

	"DYNAINFO(31)---行情(卖出量一)",0,"卖出量一",
	"DYNAINFO(32)---行情(卖出量二)",0,"卖出量二",
	"DYNAINFO(33)---行情(卖出量三)",0,"卖出量三",
	"DYNAINFO(34)---行情(卖出价一)",0,"卖出价一",
	"DYNAINFO(35)---行情(卖出价二)",0,"卖出价二",
	"DYNAINFO(36)---行情(卖出价三)",0,"卖出价三",

	"DYNAINFO(37)---行情(换手率)",0,"换手率",
	"DYNAINFO(38)---行情(5天均量)",0,"5天均量",


	"FINANCE(1)---财务(总股本(万股))",0,"总股本(万股)",
	"FINANCE(2)---财务(国家股(万股))",0,"国家股(万股)",
	"FINANCE(3)---财务(发起人法人股(万股))",0,"发起人法人股(万股)",
	"FINANCE(4)---财务(法人股(万股))",0,"法人股(万股)",
	"FINANCE(5)---财务(B股(万股))",0,"B股(万股)",
	"FINANCE(6)---财务(Ｈ股(万股))",0,"Ｈ股(万股)",
	"FINANCE(7)---财务(流通A股(万股))",0,"流通A股(万股)",
	"FINANCE(8)---财务(职工股(万股))",0,"职工股(万股)",
	"FINANCE(9)---财务(转配股(万股))",0,"转配股(万股)",
	"FINANCE(10)---财务(总资产(千元))",0,"总资产(千元)",
	"FINANCE(11)---财务(流动资产)",0,"流动资产",
	"FINANCE(12)---财务(固定资产)",0,"固定资产",
	"FINANCE(14)---财务(无形资产)",0,"无形资产",
	"FINANCE(15)---财务(长期投资)",0,"长期投资",
	"FINANCE(13)---财务(流动负债)",0,"流动负债",
	"FINANCE(16)---财务(长期负债)",0,"长期负债",
	"FINANCE(17)---财务(资本公积金)    ",0,"(资本公积金)    ",
	"FINANCE(18)---财务(每股公积金(元))",0,"(每股公积金(元))",
	"FINANCE(19)---财务(股东权益)      ",0,"(股东权益)      ",
	"FINANCE(20)---财务(主营业务收入)  ",0,"(主营业务收入)  ",
	"FINANCE(21)---财务(主营业务利润)  ",0,"(主营业务利润)  ",
	"FINANCE(22)---财务(其它业务利润)  ",0,"(其它业务利润)  ",
	"FINANCE(23)---财务(利润总额)      ",0,"(利润总额)      ",
	"FINANCE(24)---财务(投资收益)      ",0,"(投资收益)      ",
	"FINANCE(25)---财务(补贴收入)      ",0,"(补贴收入)      ",
	"FINANCE(26)---财务(营业外收支)    ",0,"(营业外收支)    ",
	"FINANCE(27)---财务(上年损益调整)  ",0,"(上年损益调整)  ",
	"FINANCE(28)---财务(投资收益)      ",0,"(投资收益)      ",
	"FINANCE(29)---财务(利润总额)      ",0,"(利润总额)      ",
	"FINANCE(30)---财务(税后利润)      ",0,"(税后利润)      ",
	"FINANCE(31)---财务(净利润)        ",0,"(净利润)        ",
	"FINANCE(32)---财务(未分配利润)    ",0,"(未分配利润)    ",
	"FINANCE(33)---财务(每股未分配利润(元))",0,"每股未分配利润(元)",
	"FINANCE(34)---财务(每股收益(元))",0,"每股收益(元)",
	"FINANCE(35)---财务(每股净资产(元))",0,"每股净资产(元)",
	"FINANCE(36)---财务(股东权益比率(%))",0,"股东权益比率(%)",
	"FINANCE(37)---财务(净资产收益率(%))",0,"净资产收益率(%)",

	"绘图函数",-1,"",
	"BUYCASE---买入条件",0,"买入条件",
	"COLORSTICK---彩色条状线",0,"彩色条状线",
	"CROSSDOT---圈叉图",0,"圈叉图",
	"CIRCLEDOT---圆点图",0,"圆点图",
	"LINESTICK---点状、柱状均画",0,"点状、柱状均画",
	"SELLCASE---卖出条件",0,"卖出条件",
	"STICK---普通条状线",0,"普通条状线",
	"VOLSTICK---成交量柱状线",0,"成交量柱状线",

	"DRAWICON(CON,PRICE,N)---当条件CON成立时，在价格PRICE处画第N个位图",0,"在图形上绘制小图标。\r\n用法:\r\nDRAWICON(COND,PRICE,TYPE),当COND条件满足时,在PRICE位置画TYPE号图标。\r\n例如：DRAWICON(CLOSE>OPEN,LOW,1)表示当收阳时在最低价位置画1号图标。",
	"DRAWLINE(CON1,PRICE1,CON2,PRICE2,N)---当条件CON1、CON2成立时，画价格PRICE1、PRICE2两点的连线",0,"在图形上绘制直线段。\r\n用法:\r\nDRAWLINE(COND1,PRICE1,COND2,PRICE2,EXPAND),当COND1条件满足时,在PRICE1位置画直线起点，当COND2条件满足时,在PRICE2位置画直线终点，EXPAND为延长类型。\r\n例如：DRAWILINE(HIGH>=HHV(HIGH,20),HIGH,LOW<=LLV(LOW,20),LOW,1)表示在创20天新高与创20天新低之间画直线并且向右延长。",
	"DRAWTEXT(CON,PRICE,TEXT)---当条件CON成立时，在价格PRICE处画文本TEXT",0,"在图形上显示文字。\r\n用法:\r\nDRAWTEXT(COND,PRICE,TEXT),当COND条件满足时,在PRICE位置书写文字TEXT。\r\n例如：DRAWTEXT(CLOSE/OPEN>1.08,LOW,'大阳线')表示当日涨幅大于8%时在最低价位置显示'大阳线'字样。",
	"POLYLINE(CON,PRICE)---当CON条件满足时,以PRICE位置为顶点画折线连接",0,"例如：POLYILINE(HIGH>=HHV(HIGH,20),HIGH)表示在创20天新高点之间画折线。",
	"STICKLINE(COND,PRICE1,PRICE2,WIDTH,EMPTY)---当COND条件满足时,在PRICE1和PRICE2位置之间画柱状线，宽度为WIDTH(10为标准间距),EMPTH不为0则画空心柱",0,"在图形上绘制柱线。\r\n用法:\r\nSTICKLINE(COND,PRICE1,PRICE2,WIDTH,EMPTY),当COND条件满足时,在PRICE1和PRICE2位置之间画柱状线，宽度为WIDTH(10为标准间距),EMPTH不为0则画空心柱。\r\n例如：STICKLINE(CLOSE>OPEN,CLOSE,OPEN,0.8,1)表示画K线中阳线的空心柱体部分。",


	"颜色函数",-1,"",//9
	"COLORRED---红色",0,"如 vol,colorred 表示用红色画成交量",
	"COLORGREEN---绿色",0,"绿色",
	"COLORBLUE---蓝色",0,"蓝色",
	"COLORYELLOW---黄色",0,"黄色",
	"COLORORANGE---橙色",0,"橙色",
	"COLORPURPLE---紫色",0,"紫色",
	"COLORWHITE---白色",0,"白色",
	"COLORBLACK---黑色",0,"黑色",


	"分布展开函数",-1,"",
	"DEPLOYREF(VOL,LOW,HIGH,N)---引用鼠标处的时间的前N周期的，把VOL均匀分布在LOW和HIGH价格之间的数据",0,"把VOL均匀分布在LOW和HIGH价格之间的数据",
	"DEPLOYSUM(VOL,N)---引用鼠标处的时间的前N周期内的，VOL的均匀分布数据的累加和",0,"VOL的均匀分布数据的累加和",
	"DEPLOYWIN(VOL,N,LTP)---引用鼠标处的时间的前N周期内的、以LTP为流通盘的、数值VOL的移动成本分布数据",0,"引用鼠标处的时间的前N周期内的、以LTP为流通盘的、数值VOL的移动成本分布数据",

	};
int nItemCountText = 192+35;
BOOL CDialogEDITZBGSHS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

    CTaiShanDoc*	pDoc=CMainFrame::m_taiShanDoc ;

	CString             stritems[FUNCNUM];
	int                 item;


	m_tree.SetImageList(pDoc->m_imagelist,TVSIL_NORMAL);

	int nItem = nItemCountText;
	HTREEITEM hParent = 0;
	if(stritemsArr[0].nFlag  != -1) return TRUE;
	hParent = m_tree.InsertItem("所有函数",16,16);
	for(item=0;item<nItem;item++)
	{
		if(stritemsArr[item].nFlag == 0)
		{
			HTREEITEM h1 = m_tree.InsertItem(stritemsArr[item].s,8,8,hParent);
			m_tree.SetItemData (h1,item);
		}
	}
	m_tree.SortChildren(  hParent );

	for(item=0;item<nItem;item++)
	{
		if(stritemsArr[item].nFlag  == -1)
		{
			hParent=m_tree.InsertItem(stritemsArr[item].s ,16,16);
			m_tree.SetItemData (hParent,0);
		}
		else
		{
			HTREEITEM h1 = m_tree.InsertItem(stritemsArr[item].s ,8,8,hParent);
			m_tree.SetItemData (h1,item);
		}
	}
	
	m_ok.EnableWindow(FALSE);
	return TRUE;  
}



void CDialogEDITZBGSHS::OnOK() 
{

	if(	m_selectedstr == "") 
	{
		AfxMessageBox("请选择函数！");
		return ;
	}
	CDialog::OnOK();
}

void CDialogEDITZBGSHS::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;


	m_selecteditem=m_tree.GetSelectedItem();
	if(m_tree.GetParentItem(m_selecteditem)==NULL)
	{
		if(m_ok.IsWindowEnabled())
			m_ok.EnableWindow(FALSE);
	}
	else
		m_ok.EnableWindow();
	m_selectedstr=m_tree.GetItemText(m_selecteditem);
	int nArr = m_tree.GetItemData (m_selecteditem);
	if(nArr>=0 && nArr<nItemCountText)
	{
		m_sExplain = stritemsArr[nArr].s2;
		UpdateData(FALSE);
	}
	
	*pResult = 0;
}

void CDialogEDITZBGSHS::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	if(m_ok.IsWindowEnabled()) OnOK();
	*pResult = 0;
}

void CDialogEDITZBGSHS::OnReturnTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	
	if(m_ok.IsWindowEnabled()) OnOK();
	*pResult = 0;
}

BOOL CDialogEDITZBGSHS::PreTranslateMessage(MSG* pMsg) 
{
	
	if(pMsg->hwnd == m_tree.m_hWnd)
	{
		if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		{
			if(m_ok.IsWindowEnabled()) OnOK();
				return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDialogEDITZBGSHS::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	
	DoHtmlHelp(this);return TRUE;
	

}
