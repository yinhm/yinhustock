
#pragma once

#include <string>

namespace StockMarket
{
	extern bool is_file_exist(const std::string& filename);

	extern void test_packet_from_file(const std::string& filename );
	extern 	const uchar * stock_code_search (const uchar * str, unsigned int len);
	extern bool stockcode_is_valid(const uchar * s);
	extern void log_packet(const char* data, size_t len, bool receive = false, int cmd = 0);
}
