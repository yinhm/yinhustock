
#pragma once

//#include "Stockdrv.h"
#include "config.h"

enum TDXMarket
{
	tdxMarketShenZhen = 0,
	tdxMarketShangHai = 1,
};


enum TDX_COMMAND
{
	CMD_TEST				= 0x0526,

	CMD_HEART_BEAT			= 0x0523,
	CMD_KLINE				= 0x052C,
	CMD_HIS_TRANS			= 0x0FB5,		// 历史回放，历史分笔
	CMD_INSTANT_TRANS		= 0x0FC5,		// 0x0fc5, 0x0faf

	CMD_0D					= 0x000D,		// 信息
	CMD_LOGIN				= 0x001E,		// 登录


	CMD_STOCK_LIST			= 0x0524,		// 股票列表，按证券版块查询
	CMD_CURRENT_PRICE		= 0x0531,

	CMD_HISREALTIME			= 0x0FB4,		// 历史分时



	CMD_STOCKHOLD_CHANGE	= 0x000F,		// 股本除权结构
	CMD_STOCKPRICE			= 0x0010,		// 列表价格

	CMD_FILEINFO			= 0x02C5,		// 文件信息
	CMD_FILEDOWN			= 0x06B9,		// 访求文件

	CMD_CODELIST			= 0x044E,		// 沪深代码个数
	CMD_CODETABLE			= 0x0450,		// 初始化代码表
};


#define TDX_FILEINFOLENGHT	0x28			// 请求文件信息的长度
#define TDX_FILEDOWNLENGHT	0x64			// 请求下载文件的长度

struct TDX_REQFILE
{
	bool open;
	string filename;
	uint filesize;
	uint current;
	string filemd5;
};


struct TDX_REQKLINE
{
	ushort stock_market;
	char stock_code[6];
	ushort base_period;
	ushort period_count;
	ushort offset_start;
	ushort request_count;
};


struct TDX_REQUESTCMD
{
	uint seq_id;		// 命令 seq_id
	ushort cmd;			// 命令
	TDXMarket market;	// 市场
	char stock[10];		// 代码
};
typedef map<uint, TDX_REQUESTCMD> TdxRequestMap;


typedef struct TDX_STOCKCODE
{
	string	stock_code;				// 证券代码 000001 600000
	string	stock_name;				// 证券名称 深发展 浦发银行
	string	stock_short;			// 拼音简称 SFZ PFYH
	uint16	stock_unit;				// 每手股数 100
	uint8	stock_digit;			// 小数位 
	float	stock_close;			// 前收盘价 
};
typedef map<string, TDX_STOCKCODE> StockCodeMap;


struct TDX_GBBQ
{
	TDXMarket	market;
	string		code;
	long		date;
	uchar		chg_type;
	// 变更类型
	// 1: 正常
	// 2: 与1相关的除权除息日股本变化
	// 3: 股本性质变化(例如职工股上市)
	// 6: 增发
	// 8: 增发上市
	union
	{
		struct 
		{
			float	old_cir;			// 旧流通股
			float	old_ttl;			// 旧总股本
			float	new_cir;			// 新流通股
			float	new_ttl;			// 新总股本
		} gb;
		struct
		{
			float	cash;				// 每10 分红
			float	sell_price;			// 每股配售价格
			float	give_count;			// 每10 股送股数
			float	sell_count;			// 每10 股配售数
		} bonus;
	} data;
};
typedef vector<TDX_GBBQ> DateGBBQ;
typedef map<uint, DateGBBQ> GBBQMap;
