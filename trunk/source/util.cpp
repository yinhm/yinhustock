
#include "stdafx.h"
#include "config.h"
#include "response.h"
#include "stock1.h"
#include <sstream>


#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace StockMarket
{
	bool is_file_exist(const string& filename)
	{
		bool isOpen;

		ifstream ifs;
		ifs.open(filename.c_str());
		isOpen = ifs.is_open();
		ifs.close();

		return isOpen;
	}

	bool stockcode_is_valid(const uchar * s)
	{
		bool valid = true;
		for(int i = 0; i < MarketInfo::StocksCodeLen; ++i)
		{
			if(s[i] < '0' || s[i] > '9')
			{
				valid = false;
				break;
			}
		}
		return valid;
	}

	const uchar * stock_code_search (const uchar * str, unsigned int len)
	{
		const uchar *cp = str;
		while (len -- > 0)
		{
			if(stockcode_is_valid(cp))
				return cp;
			cp++;
		}
		return 0;
	}

	void test_packet_from_file(const string& filename )
	{

		uchar temp[10 * 1024];
		uchar *pData = &temp[0];

		const string path = "F:\\Develop\\stock\\data\\exc_log\\";
		const string fullname = path + filename;
		fstream fs(fullname.c_str(), ios::in | ios::binary);
		if(!fs) return;

		fs.read((char*)pData, 10*1024);

		uint len = fs.gcount();

		ProcessResponsePacket(pData, len, true);
	}

	void log_packet(const char* data, size_t len, bool receive, int cmd)
	{
		const string path = "E:\\StockData";
		string filename;

		for (int i = 0; i < 1000; i++)
		{
			//stringstream ss;
			//ss << i << ".dat";
			//filename = path + ss.str();
			char file[256];
			time_t t = time(&t);

			//if (cmd)
			{
				sprintf(file, "%s\\%08d - 0 - %04d - %d", path.c_str(), t, cmd, receive);
			}

			filename = file;
			if (!is_file_exist(filename))
			{
				ofstream ofs(filename.c_str(), ios::out | ios::binary);
				ofs.write(data, len);
				break;
			}
		}
	}
}
