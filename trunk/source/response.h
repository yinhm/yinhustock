
#pragma once

#include "stock1.h"

namespace StockMarket
{
	typedef vector<string> StringSet;
	typedef map<int, StringSet> IntStringSetMap;

	class StockRes
	{
	public:
		StockRes();
		StockRes(uchar* pData, ulong data_len);
		virtual ~StockRes();

	public:
		const uchar* raw_data;		// the raw data passed in
		ulong raw_len;				// the raw data len
		const uchar* data;			// payload data
		ulong len;					// payload len
		const uchar* current;		// pointer to parse
		ulong left;					// len left unparsed
		bool dataNew;				// data is uncompass

	public:
		virtual ulong buff_left();
		virtual bool end();
		virtual void reset();
		virtual void operator()() = 0;

	protected:
		void skip_byte(ulong count);
		void skip_data(ulong count);

		char get_char();
		uchar get_uchar();
		short get_short();
		ushort get_ushort();
		int get_int();
		uint get_uint();
		float get_float();

		int parse_data();
		int parse_data2();

	private:
		void umcompress_if();

	public:
		uint read(const uchar* pData, ulong data_len);

	public:
		ushort get_packet_len();
		static ushort get_packet_len(const uchar*);

		uint get_seq_id();
		static uint get_seq_id(const uchar*);

		ushort get_cmd_id();
		static ushort get_cmd_id(const uchar*);
	};



	class StockListRes: public StockRes
	{
	public:
		StockListRes() { };
		StockListRes(uchar* pData, ulong data_len) : StockRes(pData, data_len) { };
		void operator()();
	};

	// 股本除权结构
	class StockHoldChgRes : public StockRes
	{
	public:
		StockHoldChgRes() { };
		StockHoldChgRes(uchar* pData, ulong data_len) : StockRes(pData, data_len) { };
		void operator()();
	};

	class StockCurrentPriceRes: public StockRes
	{
	public:
		StockCurrentPriceRes(){};
		StockCurrentPriceRes(uchar* pData, ulong data_len):StockRes(pData, data_len){}
		void operator()();
	private:
	};

	class StockKlineRes: public StockRes
	{
	public:
		StockKlineRes() { };
		StockKlineRes(uchar* pData, ulong data_len) : StockRes(pData, data_len) { }
		void operator()();
	};



	struct StockResWrongData : public std::exception
	{
	public:
		StockResWrongData(int i = 0);

	public:
		int n_;

	public:
		virtual const char *what() const throw();
	};



	class StockTestRes : public StockRes
	{
	public:
		StockTestRes(ushort cmd) { ucmd = cmd; };

	protected:
		ushort ucmd;

	public:
		void operator()();
	};



	extern void ProcessResponsePacket(const uchar* pData, size_t len, bool test);
}
