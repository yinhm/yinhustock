
#pragma once

#include "config.h"
#include "TdxStructDef.h"


#include <boost/thread/detail/config.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost;


#include "tcpSocket.h"
#include "stock1.h"

class CClientSocket;

namespace StockMarket
{

#pragma pack(push)
#pragma pack(1)

	extern recursive_mutex req_queue_mutex;

	class Request
	{
	public:
		Request();
		virtual ~Request();

	public:
		static bool received;
		static uint seq_id;

		bool first;

	public:
		static void res_seq_id(uint id);
		static bool ready();

	public:
		virtual Request& operator++();
		virtual operator bool();
		virtual void next();
		virtual TDX_REQUESTCMD get_request();

	public:
		virtual char* buff() { return 0; };
		virtual ulong len() { return 0; };
		virtual void send(CClientSocket* soc);
	};



	struct TDX_REQUESTHEAD
	{
	public:
		TDX_REQUESTHEAD() { };
		TDX_REQUESTHEAD(ushort cmd_id, ushort packet_len = sizeof(TDX_REQUESTHEAD));

	protected:
		uchar	zip;			// 标志 0x0C
		uint	seq_id;			// 同一种命令的 seq_id。
		uchar	packet_type;	// 00: 回应 1, 2, 3... Request Count
		ushort	len;			// 数据长度
		ushort	len1;			// 长度重复
		ushort	cmd;			// 命令

	public:
		ushort get_cmd() { return cmd; }

		// 数据长度
		ushort get_len();
		void set_len(ushort payload_len);

		// 数据编号
		uint get_seq_id();
		void set_seq_id(uint id);

		friend class Request;
	};



	class StockHeartBeat : public Request
	{
	public:
		char* buff();
		ulong len();
	private:
		struct StockHeartBeatStruct
		{
			StockHeartBeatStruct() : header(CMD_HEART_BEAT, sizeof(StockHeartBeatStruct)), count(1)
			{
				memcpy(&stock_code[0],"000792", MarketInfo::StocksCodeLen);
				location = MarketInfo::get_market_location(&stock_code[0]);
			}
			TDX_REQUESTHEAD	header;
			ushort count;
			char location;
			char stock_code[MarketInfo::StocksCodeLen];
		}s;
	};



	extern MarketInfo::MarketType request_type;

	class StockListReq : public Request
	{
	public:
		StockListReq(MarketInfo::MarketType market_code, ushort record_offset = 0, ushort record_count = 70, ushort record_total = 0);

	protected:
		struct StockListStruct
		{
			StockListStruct(MarketInfo::MarketType market_code, ushort record_offset, ushort record_count)
				: header(CMD_STOCK_LIST, sizeof(StockListStruct)), offset(record_offset), count(record_count), unknown1(0), unknown2(0)
			{
				block = /*MarketInfo::get_block_from_market_type*/(ushort)market_code;
			}

			TDX_REQUESTHEAD	header;
			ushort block;
			ushort unknown1;
			ushort offset;
			ushort count;
			ushort unknown2;
		} s;

		ushort total;

		MarketInfo::MarketType mtype;

	public:
		operator bool();
		StockListReq& operator++();

	public:
		char* buff();
		ulong len();
		void send(CClientSocket* soc);
	};


	// 同时取多个
	class StockMultiReq : public Request
	{
	public:
		StockMultiReq(ushort cmd_id):s(cmd_id){};
		char* buff();
		ulong len();
		bool add_stock(const string& stock_code);
		bool add_stock(TDXMarket market, const string& stock_code);
		static const int max_stocks_a_request = 30;
	protected:
		struct StockMultiStruct
		{
			char market_locale;
			char stock_code[MarketInfo::StocksCodeLen];
		};
		struct StockMultiReqStruct
		{
			StockMultiReqStruct(ushort cmd_id) : header(cmd_id, 0), count(0){}
			bool add_one_stock(const string& stock_code)
			{
				if(count >= max_stocks_a_request)
					return false;

				s_buff[count].market_locale = MarketInfo::get_market_location(stock_code);
				memcpy(&s_buff[count].stock_code, stock_code.c_str(), MarketInfo::StocksCodeLen);
				count++;
				header.set_len( sizeof(StockMultiStruct) * count + 4);

				return true;
			}

			bool add_one_stock(TDXMarket market, const string& stock_code)
			{
				if (count >= max_stocks_a_request)
					return false;

				s_buff[count].market_locale = market;
				memcpy(&s_buff[count].stock_code, stock_code.c_str(), MarketInfo::StocksCodeLen);
				count++;

				header.set_len(sizeof(StockMultiStruct) * count + 4);

				return true;
			}

			TDX_REQUESTHEAD	header;
			ushort count;
			StockMultiStruct s_buff[max_stocks_a_request];		
		}s;
	};


	class StockCurrentPriceReq : public StockMultiReq
	{
	public:
		StockCurrentPriceReq():StockMultiReq(CMD_CURRENT_PRICE){};	
	};




	struct StockKlineShareData
	{
		char stock_code[MarketInfo::StocksCodeLen];
	};

	static const int max_record_a_request = 0x0118;

	enum TDXKLineType
	{
		tdxKlineTypeMin5		= 0x00,
		tdxKlineTypeMin15		= 0x01,
		tdxKlineTypeMin30		= 0x02,
		tdxKlineTypeMin60		= 0x03,
		tdxKlineTypeDay			= 0x04,
		tdxKlineTypeWeek		= 0x05,
		tdxKlineTypeMonth		= 0x06,
		tdxKlineTypeMin			= 0x07,
		tdxKlineTypeMultiMin	= 0x08,
		tdxKlineTypeMultiDay	= 0x09,
		tdxKlineTypeSeason		= 0x0A,
		tdxKlineTypeYear	 	= 0x0B,
	};

	class StockKLineReq : public Request
	{
	public:
		StockKLineReq(const char* stockcode, TDXMarket market, TDXKLineType type, ushort off_start = 0, ushort off_end = max_record_a_request);

	public:
		struct StockKLineReqStruct
		{
			StockKLineReqStruct() : header(CMD_KLINE, sizeof(StockKLineReqStruct)), period_count(1) { }
			TDX_REQUESTHEAD header;
			ushort market_locale;	// TDXMarket
			char stock_code[MarketInfo::StocksCodeLen];
			ushort base_period;		// TDXKLineType
			ushort period_count;	// 实际周期 default == 1 如 period_count = 10 base_period == ktypeMultiMin 表示 10 分钟 K 线
			ushort offset_start;	// 
			ushort offset_end;		// 
		} s;

	public:
		TDX_REQUESTCMD get_request();

	public:
		char* buff();
		ulong len();
	};


	struct StockKLine
	{
		uint time;			// 时间 20100101

		uint price;			// 现价 
		uint open;			// 开盘
		uint close;			// 收盘
		uint high;			// 最高
		uint low;			// 最低
		uint vol;			// 总手，以股数计算
		uint money;			// 成交资金
	};


#pragma pack(pop)

	typedef shared_ptr<Request> CmdData_t;
	typedef queue<CmdData_t > CmdQueue_t;



	class TdxRequest : public Request
	{
	public:
		TdxRequest(TDX_COMMAND cmd);

	protected:
		char buffer[256];

		int nlen;
		char* pbuff;

	protected:
		uint m_nTotal;
		uint m_nOffset;

	public:
		char* buff();
		ulong len();

	public:
		void WriteData(uint8 data8);
		void WriteData(uint16 data16);
		void WriteData(uint32 data32);
		void WriteData(void* pData, uint16 nLen);

	public:
		void AddStock(uchar market, const string& stock_code);

	public:
		virtual TdxRequest& operator ++();
		virtual operator bool ();

	public:
		virtual TDX_REQUESTCMD get_request();
		virtual void SetTotal(uint nTotal) { m_nTotal = nTotal; };
	};
}
