
#pragma once

#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost;

#include "config.h"
#include "TdxStructDef.h"
#include "stockdrv.h"

namespace StockMarket
{
	typedef vector<string> StringSet;

	struct MarketInfo
	{
		enum MarketType // tdxBlock
		{
			MARKET_SHANGHAI_A	= 0x00,			// 上证A股  60xxxx (600...601...)
			MARKET_SHANGHAI_B	= 0x01,			// 上证B股  90xxxx (900...)
			MARKET_SHENZHEN_A	= 0x02,			// 深证A股  00xxxx 30xxxx (000...002...300...)
			MARKET_SHENZHEN_B	= 0x03,			// 深证B股  20xxxx (200...)
			MARKET_SHANGHAI_Z	= 0x04,			// 上证债券 01...09xxxx 10...19xxxx 20xxxx 70...79xxxx 939xxx
			MARKET_SHENZHEN_Z	= 0x05,			// 深证债券 10...13xxxx 369xxx
			MARKET_STOCK_A		= 0x06,			// 沪深A股  
			MARKET_STOCK_B		= 0x07,			// 沪深B股  
			MARKET_STOCK_Z		= 0x08,			// 沪深债券 
			MARKET_STOCK_J		= 0x09,			// 沪深基金 15.16.18xxxx 50.51.52xxx
			MARKET_STOCK_ALL	= 0x0A,			// 所有商品 
			MARKET_INDEX		= 0x0B,			// 所有指数 39xxxx 00xxxx 88xx (395...399...000...888...)
			MARKET_ZHONGXIAO	= 0x0C,			// 中小企业 
			MARKET_WARRANT		= 0x0D,			// 沪深权证 58xxxx (580...)
			MARKET_CUANGYE		= 0x0E,			// 创业板
			MARKET_UNKNOWN,
		};

		static unsigned int stocks_count[16];
		static StringSet stocks_set[16];
		static StringSet stocks_set_all;

		const static int StocksCodeLen = 6;		// 100  股一手
		const static int StocksPerHand = 100;	// 100  股一手.
		const static float tax;					// 0.3 %

		static short get_market_type_from_code(const char* pCode)
		{
			//if (pCode[0] == '0')
			//{
				//if (pCode[1] == '0')
				//{
				//	return MARKET_SHENZHEN_A;	// 深圳A股  00xxxx 30xxxx (000...002...300...)
				//}
				//else if (pCode[1] == '1')
				//{
				//	return m1;		// 上海债券
				//}
			//}
			//else if (pCode[0] == '2')
			//{
			//	return MARKET_SHENZHEN_B;		// 深圳B股  20xxxx (200...)
			//}
			//else if (pCode[0] == '3')
			//{
			//	if (pCode[1] == '0')
			//	{
			//		return MARKET_SHENZHEN_A;	// 深圳A股  00xxxx 30xxxx (000...002...300...)
			//	}
			//	else if (pCode[1] == '9')
			//	{
			//		return MARKET_INDEX;		// 所有指数 39xxxx 00xxxx 88xx (395...399...000...888...)
			//	}
			//}
			//else if (pCode[0] == '6')
			//{
			//	return MARKET_SHANGHAI_A;		// 上海A股  60xxxx (600...601...)
			//}
			//else if (pCode[0] == '9')
			//{
			//	if (pCode[1] == '0')
			//	{
			//		return MARKET_SHANGHAI_B;	// 上海B股  90xxxx (900...)
			//	}
			//	else
			//	{
			//		return MARKET_INDEX;		// 99xxxx
			//	}
			//}
			//else if (pCode[0] == '1' || (pCode[0] == '3' && pCode[1] == '6'))
			//{
			//	return m2;						// 深圳债券 10...13xxxxxx,36xxxxxx
			//}
			//else if (pCode[0] == '5')
			//{
			//	return MARKET_WARRANT;			// 沪深权证 580xxx
			//}
			//else
			//{
			//	return m1;
			//}

			return 0;
		}

		static short get_market_type(const char* pCode)
		{
			return get_market_type_from_code(pCode);
		}

		static short get_market_type(const string& stock_code)
		{
			const char* pCode = stock_code.c_str();
			return get_market_type_from_code(pCode);
		}

		static char get_market_location_from_code(const char* pCode)
		{
			if(pCode[0] <= '4')
				return 0;					// 深圳 00xxxx, 03xxxx, 02xxxx, 3xxxxx(深圳指数)
			else
				return 1;					//上海 60xxxxx, 58xxxx, 99xxxx (上海指数)
		}

		static char get_market_location(const char* pCode)
		{
			return get_market_location_from_code(pCode);
		}

		static char get_market_location(const string& stock_code)
		{
			const char* pCode = stock_code.c_str();
			return get_market_location_from_code(pCode);
		}

		static string get_first_stock();
		static string find_next_stock(const string& stock_code);
	};



	struct StockTransact
	{
		short		minute;
		int			price;
		int 			vol;
		int 			count;	// 此笔成交量的成交次数。 0: 表示未知
		uchar		bs;		// 表示买成交量还是卖成交量0:buy, 1:sell
		bool operator == (const StockTransact& t);
	};
	typedef vector<StockTransact> DailyTransact;
	typedef map<string, DailyTransact> DateTransact;

	extern DateTransact		today_transact;

	struct StockBid			// 盘口
	{
		int			minute;
		int			act;				// 活跃度 active rate

		int			price;			// 现价
		int			y_close;			// 昨收
		int			open;			// 开盘
		int			high;			// 最高
		int			low;				// 最低
		int			buy;			// 买入
		int			sell;			// 卖出

		int			total_vol;		// 总手，以股数计算
		int			avail_vol;		// 现手，以股数计算
		int			inner_vol;		// 内盘
		int			outer_vol;		// 外盘

		int			updown;			// 单笔升跌

		int			buy_price1;	// 买价1
		int			sell_price1;
		int			buy_vol1;	// 买量1
		int			sell_vol1;
		int			buy_price2;
		int			sell_price2;
		int			buy_vol2;
		int			sell_vol2;
		int			buy_price3;
		int			sell_price3;
		int			buy_vol3;
		int			sell_vol3;
		int			buy_price4;
		int			sell_price4;
		int			buy_vol4;
		int			sell_vol4;
		int			buy_price5;
		int			sell_price5;
		int			buy_vol5;
		int			sell_vol5;
	};
	typedef vector<StockBid> DailyBid;




	extern uint date_to_uint(gregorian::date);
	extern gregorian::date uint_to_date(uint);




	struct BaseInfo
	{
		string		stock_code;				// 代码
		uint		update_time;			// 更新时间
		double		ttl_amount;				// 总股本
		double		state_own_amount;		// 国家股
		double		init_amount;			// 发起股本
		double		corp_amount;			// 法人股本
		double		b_amount;				// B 股本
		double		h_amount;				// H 股本
		double		cir_amount;				// 流通 股本
		double		empl_amount;			// 职工 股本
		double		unknown1;				// 
		double		ttl_asset;				// 总资产
		double		varible_asset;			// 流动 资产
		double		firm_asset;				// 固定 资产
		double		invisible_asset;		// 无形 资产
		double		long_term_invest;		// 长期投资
		double		varible_debt;			// 流动负债
		double		long_term_debt;			// 长期负债
		double		accu_fund;				// 公积金
		double		net_asset;				// 净资产
		double		major_income;			// 主营收入
		double		major_profit;			// 主营利润
		double		unknown2;				// 
		double		bussiness_income;		// 营业收入
		double		invest_income;			// 营业收入
		double		allowance;				// 补贴收入
		double		non_bussiness_income;	// 业外收入
		double		income_adjustment;		// 收入调整
		double		ttl_profit;				// 利润总额
		double		unknown3;				// 
		double		net_profit;				// 税后利润
		double		undist_profit;			// 未分配利润
		double		per_net_assert2;		// 每股净资产2

		static const int record_len;
	};

	typedef map<string, BaseInfo> StockBaseInfoMap;

	extern fstream& operator >> (fstream& fs, BaseInfo& bs);

	typedef vector<string> StringVector;
	typedef map<string, StringVector> StockGroupMap;

	class StockBasicInfo
	{
	public:
		StockBaseInfoMap stock_base_info;
		StockGroupMap stock_block_info;

	public:
		// 加载基本资料
		bool load_basic_info();
		bool load_block_info();

		void clear_gbbq_info();
		bool is_gbbq_file_exist(uint tr_date);
		GBBQMap			stock_gbbq_info;
	};




	const int stockocde_len = 0xFA;

	extern uint16 g_marketcount[2];
	extern uint16 g_marketinit[2];
	extern StockCodeMap g_stocklist[2];

	extern TDX_REQFILE g_stockfile;

	extern TdxRequestMap g_stockrequest;

	// 加载证券代码表
	int load_stock_code(TDXMarket market);

	extern void AddStockListRequest(bool bAll);
	extern void AddGBBQRequest();

	typedef deque<RCV_DATA*> sendlistd;
	extern sendlistd g_sendlist;

	void communication();
}
