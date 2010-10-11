
#pragma once

#include <boost/tuple/tuple.hpp>
using namespace boost;

#include "response.h"

namespace StockMarket
{
	struct InstantTransShareData
	{
		TDXMarket market;
		string current_stock;
	};

	class InstantTransReq : public Request 
	{
	public:
		InstantTransReq(int count);
		InstantTransReq(TDXMarket market, const string& stock_code, int count);

	protected:
#pragma pack(push)
#pragma pack(1)
		struct InstantTransStruct
		{
		public:
			InstantTransStruct();
			InstantTransStruct(TDXMarket market, const string& stock_code,  ushort record_offset, ushort record_count);

		public:
			void set_stock_code(TDXMarket market, const string& stock_code);

		protected:
			TDX_REQUESTHEAD	header;
			ushort location;
			char code[MarketInfo::StocksCodeLen];
			ushort offset;
			ushort count;
		} s;
#pragma pack(pop)

	private:
		bool finished;
		string current_stock;
		bool all_stock;
		uint dt;

	public:
		InstantTransReq& operator++();
		operator bool();

	public:
		virtual char* buff();
		virtual ulong len();
	};

	class InstantTransRes:  public StockRes
	{
	public:
		static recursive_mutex inst_trans_res_map_mutex;
		void operator()();
	};

	struct HisTransShareData
	{
		uint date;
		string current_stock;
		ushort to_get_offset;
		ushort total_got_count;
		bool got_one_stock;
	};

	class HisTransReq : public Request
	{
	public:
		HisTransReq(const string& stock_code,  uint record_date);
		HisTransReq(uint record_date);
		char* buff();
		ulong len();
		HisTransReq& operator++();
		operator bool ();
		static const ushort to_get_count = 180;
	protected:
#pragma pack(push)
#pragma pack(1)
		struct HisTransStruct		//0x0fb5
		{
		public:
			HisTransStruct();
			HisTransStruct(const string& stock_code,  uint record_date, ushort record_offset, ushort record_count);
			void set_stock_code(const string& stock_code);
			void set_offset(ushort off);
			TDX_REQUESTHEAD	header;		
			uint date;			// ƒÍ‘¬»’ 20050411 = 0x0131f1e8
			ushort location;		// ????
			char code[6];
			ushort offset;
			ushort count;
		}s;
#pragma pack(pop)
	private:
		bool finished;
		bool all_stock;
		string current_stock;
	};

	class HisTransRes : public StockRes
	{
	public:
		static recursive_mutex his_trans_res_map_mutex;
		void operator()();
	};
}
