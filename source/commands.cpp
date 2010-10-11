
#include "stdafx.h"
#include "config.h"


//#include <vector>
//#include <set>
//#include <map>
//#include <queue>

#include <boost/thread/detail/config.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
//#include <boost/smart_assert/assert.hpp>
#include <boost/tuple/tuple.hpp>

#include "tcpSocket.h"
#include "stock1.h"
#include "request.h"
#include "response.h"
#include "commands.h"

namespace StockMarket
{
	static const int max_transaction_per_day = 6000;

	InstantTransShareData g_InstantTransShareData;

	InstantTransReq::InstantTransReq(TDXMarket market, const string& stock_code, int count) : s(market, stock_code, 0, count), finished(false), all_stock(false)
	{
		g_InstantTransShareData.current_stock = current_stock;
	}

	InstantTransReq::InstantTransReq(int count = 1000) : s(tdxMarketShenZhen, "000000", 0, count), finished(true), all_stock(true)
	{
		if ("" != (current_stock = MarketInfo::get_first_stock()))
		{
			g_InstantTransShareData.current_stock = current_stock;
			s.set_stock_code(tdxMarketShenZhen, current_stock);
			finished = false;
		}
		else
		{
			finished = true;
		}
	}

	InstantTransReq::InstantTransStruct::InstantTransStruct()
	{
	}

	InstantTransReq::InstantTransStruct::InstantTransStruct(TDXMarket market, const string& stock_code, ushort record_offset, ushort record_count)
		: header(CMD_INSTANT_TRANS, sizeof(InstantTransStruct)), offset(record_offset), count(record_count)
	{
		set_stock_code(market, stock_code);
	}

	void InstantTransReq::InstantTransStruct::set_stock_code(TDXMarket market, const string& stock_code)
	{
		memcpy(code, stock_code.c_str(), MarketInfo::StocksCodeLen);
		location = /*MarketInfo::get_market_location(stock_code)*/(ushort)market;
	}

	InstantTransReq& InstantTransReq::operator++()
	{
		if ("" != (current_stock = MarketInfo::find_next_stock(current_stock)))
		{
			g_InstantTransShareData.current_stock = current_stock;
			s.set_stock_code(tdxMarketShenZhen, current_stock);
			finished = false;
		}
		else
		{
			finished = true;
		}

		return *this;
	}

	InstantTransReq::operator bool()
	{
		return !finished;
	}

	char* InstantTransReq::buff()
	{
		return (char*)&s;
	}

	ulong InstantTransReq::len()
	{
		return sizeof(InstantTransStruct);
	}

	recursive_mutex InstantTransRes::inst_trans_res_map_mutex;
	DateTransact 	today_transact;

	void InstantTransRes::operator()()
	{
		recursive_mutex::scoped_lock guard(inst_trans_res_map_mutex);

		const int recent_data_buffer = 1000;

		DailyTransact& today_t = today_transact[g_InstantTransShareData.current_stock];
		static StockTransact temp_daily_t[recent_data_buffer];	// 如果连续 多个 个相同，就认为是同一序列
		bool is_the_first_get = today_t.empty();

		StockTransact t;
		uint priceBase = 0;
		int first = 1;

		ushort count = get_ushort();

		int new_data_count = 0;

		while(count-- > 0)
		{
			t.minute = get_ushort();
			// relatively loose assert
			if(t.minute < 9 * 60 || t.minute > 16 * 60)
			{
				throw StockResWrongData(5);
			}
			if(first)
			{
				priceBase = t.price = parse_data2();
				first = 0;
			}
			else
			{
				priceBase = t.price = (uint)(parse_data() + priceBase);
			}
			t.vol = parse_data2();
			t.count = get_uchar();		// 成交笔数。 0: 未知
			t.bs = get_uchar();
			skip_byte(1);			// the ending zero

			if(is_the_first_get)
			{
				today_t.push_back(t);
			}
			else
			{
				temp_daily_t[new_data_count++] = t;
			}
		}
		if(is_the_first_get) return;

		// 确定两序列中要比较的记录数
		int to_compare_count = 5 < today_t.size() ? 5 : today_t.size();

		if(new_data_count < to_compare_count) return;	// 太少数据，肯定出错了。

		// 匹配两序列中相同的部分
		bool matched = true; int i = 0, j = 0;
		for(; i < new_data_count; ++i)
		{
			matched = true;
			for(uint compare_index = today_t.size() - to_compare_count, j = i;
				compare_index < today_t.size(); ++compare_index, ++j)
			{
				if((today_t[compare_index].price != temp_daily_t[j].price) 
					|| (today_t[compare_index].vol != temp_daily_t[j].vol)
					|| (today_t[compare_index].minute != temp_daily_t[j].minute)
					|| (today_t[compare_index].bs != temp_daily_t[j].bs))
				{
					matched = false;
					break;
				}
			}
			if(matched)
			{
				break;
			}
		}
		// 将不相同即新增部分，添加到原有数据后。
		if(matched)
		{
			for(++j; j < new_data_count; ++j)
			{
				today_t.push_back(temp_daily_t[j]);
			}
		}
		return;

	}

	recursive_mutex HisTransRes::his_trans_res_map_mutex;

	HisTransShareData g_HisTransShareData;

	HisTransReq::HisTransReq(const string& stock_code,  uint record_date)
		: s(stock_code, record_date, 0, to_get_count), current_stock(stock_code), finished(false), all_stock(false)
	{
		g_HisTransShareData.current_stock = current_stock;
		g_HisTransShareData.date = record_date;
		g_HisTransShareData.to_get_offset = 0;
		g_HisTransShareData.total_got_count = 0;
		g_HisTransShareData.got_one_stock = false;
	}
	HisTransReq::HisTransReq(uint record_date)
		: s("000000", record_date, 0, to_get_count), finished(true) ,all_stock(true)
	{
		// set the first stock
		if("" != (current_stock = MarketInfo::get_first_stock()))
		{
			s.set_stock_code(current_stock);
			finished = false;

			g_HisTransShareData.current_stock = current_stock;
			g_HisTransShareData.date = record_date;
			g_HisTransShareData.to_get_offset = 0;
			g_HisTransShareData.total_got_count = 0;
			g_HisTransShareData.got_one_stock = false;
		}
		else
		{
			finished = true;
		}
	}
	char* HisTransReq::buff()
	{
		return (char*)&s;
	}
	ulong HisTransReq::len()
	{
		return sizeof(HisTransStruct);
	}
	HisTransReq& HisTransReq::operator++()
	{
		if(!g_HisTransShareData.got_one_stock)
		{
			g_HisTransShareData.to_get_offset = g_HisTransShareData.total_got_count;
			s.set_offset(g_HisTransShareData.to_get_offset);
			return *this;
		}

		if("" != (current_stock = MarketInfo::find_next_stock(current_stock)))
		{
			g_HisTransShareData.current_stock = current_stock;
			g_HisTransShareData.to_get_offset = 0;
			g_HisTransShareData.total_got_count = 0;
			g_HisTransShareData.got_one_stock = false;			// Next Stock

			s.set_stock_code(current_stock);
			s.set_offset(g_HisTransShareData.to_get_offset);
			finished = false;
		}
		else
		{
			finished = true;
		}
		return *this;
	}
	HisTransReq::operator bool ()
	{
		return !finished;
	}

	HisTransReq::HisTransStruct::HisTransStruct(){}
	HisTransReq::HisTransStruct::HisTransStruct(const string& stock_code,  uint record_date, ushort record_offset, ushort record_count)
		: header(CMD_HIS_TRANS, sizeof(HisTransStruct)), date(record_date), offset(record_offset), count(record_count)
	{
		set_stock_code(stock_code);
	}
	void HisTransReq::HisTransStruct::set_stock_code(const string& stock_code)
	{
		memcpy(code, stock_code.c_str(), MarketInfo::StocksCodeLen);
		location = /*MarketInfo::get_market_location(stock_code)*/0;
	}
	void HisTransReq::HisTransStruct::set_offset(ushort off)
	{
		offset = off;
	}

	void HisTransRes::operator()()
	{
		recursive_mutex::scoped_lock guard(his_trans_res_map_mutex);

		static StockTransact temp_daily_t[max_transaction_per_day];				// 缓冲, 大一些安全一些

		int priceBase = 0;
		int first = 1;

		short count = get_short();
		skip_byte(4);													// do not know what it is

		short got_count = count;
		g_HisTransShareData.total_got_count += count;

		int temp_count = g_HisTransShareData.total_got_count;

		if(g_HisTransShareData.total_got_count > max_transaction_per_day)										// 缓冲满
		{
			cout << __FUNCTION__ << " code= " << g_HisTransShareData.current_stock << " date = " << g_HisTransShareData.date << "count= " << count << endl;
			throw StockResWrongData(6);
		}

		StockTransact *t = &temp_daily_t[max_transaction_per_day - g_HisTransShareData.total_got_count];
		while(count--)
		{
			t->minute = get_short();
			// relatively loose assert
			if(t->minute < 9 * 60 || t->minute > 16 * 60 || priceBase < 0)
			{
				cout << __FUNCTION__ << " code= " << g_HisTransShareData.current_stock << " date = " << g_HisTransShareData.date << "count= " << count << endl;
				throw StockResWrongData(7);
			}
			if(first)
			{
				priceBase = t->price = parse_data2();
				first = 0;
			}
			else
			{
				priceBase = t->price = parse_data2() + priceBase;
			}
			t->vol = parse_data2();
			t->bs = get_uchar();
			t->count = get_uchar();

			++t;
		}
		if(got_count < HisTransReq::to_get_count)				// 结束
		{
			g_HisTransShareData.got_one_stock = true;
			return;
		}
	}


}


