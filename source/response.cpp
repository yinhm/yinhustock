
#include "stdafx.h"
#include "Stockdrv.h"
#include "config.h"
#include "TdxStructDef.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/static_assert.hpp>
#include <boost/scoped_ptr.hpp>
#include "zlib.h"
#include "request.h"
#include "response.h"
#include "commands.h"
#include "stock1.h"
#include "util.hpp"

namespace StockMarket
{
	StockRes::StockRes() : raw_data(0), raw_len(0), data(0), len(0), current(0), left(0), dataNew(false)
	{
	}

	StockRes::StockRes(uchar* pData, ulong data_len) : raw_data(pData), raw_len(data_len)
	{
		read(pData, data_len);
	}

	StockRes::~StockRes()
	{
		if (dataNew) delete []data;
	}

	ulong StockRes::buff_left()
	{
		return left;
	}

	bool StockRes::end()
	{
		return left == 0;
	}

	void StockRes::reset()
	{
		current = data;
		left = len;
	}

	void StockRes::skip_byte(ulong count)
	{
		if (count > left) 
		{
			throw StockResWrongData(1);
		}

		current += count;
		left -= count;
	}

	void StockRes::skip_data(ulong count)
	{
		while (count--)
		{
			if (current[0] < 0x80)
			{
				skip_byte(1);
			}
			else if (current[1] < 0x80)
			{
				skip_byte(2);
			}
			else
			{
				skip_byte(3);
			}
		}
	}

	char StockRes::get_char()
	{
		char value = *(char*)current;
		skip_byte(sizeof(char));
		return value;
	}

	uchar StockRes::get_uchar()
	{
		uchar value = *(uchar*)current;
		skip_byte(sizeof(uchar));
		return value;
	}

	short StockRes::get_short()
	{
		short value = *(short*)current;
		skip_byte(sizeof(short));
		return value;
	}

	ushort StockRes::get_ushort()
	{
		ushort value = *(ushort*)current;
		skip_byte(sizeof(ushort));
		return value;
	}

	int StockRes::get_int()
	{
		int value = *(int*)current;
		skip_byte(sizeof(int));
		return value;
	}

	uint StockRes::get_uint()
	{
		uint value = *(uint*)current;
		skip_byte(sizeof(uint));
		return value;
	}

	float StockRes::get_float()
	{
		float value = *(float*)current;
		skip_byte(sizeof(float));
		return value;
	}

	int StockRes::parse_data()
	{
		if ((current[0] >= 0x40 && current[0] < 0x80) || current[0] >= 0xC0)
		{
			return 0x40 - parse_data2();
		}
		else
		{
			return parse_data2();
		}
	}

	int StockRes::parse_data2()
	{
		// 8f ff ff ff 1f == -49
		// bd ff ff ff 1f == -3
		// b0 fe ff ff 1f == -80
		// 8c 01		 == 76
		// a8 fb b6 01 == 1017 万
		// a3 8e 11    == 14.02 万
		// 82 27         == 2498

		int nBytes = 0;
		while (current[nBytes] >= 0x80)
		{
			++nBytes;
		}
		++nBytes;

		int value;

		switch (nBytes)
		{
		case 1:
			value = current[0];
			break;
		case 2:
			value = current[1] * 0x40 + current[0] - 0x80;
			break;
		case 3:
			value = (current[2] * 0x80 + current[1] - 0x80) * 0x40 + current[0] - 0x80;
			break;
		case 4:
			value = ((current[3] * 0x80 + current[2] - 0x80) * 0x80 + current[1] - 0x80) * 0x40 + current[0] - 0x80;
			break;
		case 5:
			value = (((current[4] * 0x80 + current[3] - 0x80) * 0x80 + current[2] - 0x80) * 0x80 + current[1] - 0x80)* 0x40 + current[0] - 0x80;
			break;
		case 6:
			value = ((((current[5] * 0x80 + current[4] -0x80) * 0x80 +  current[3] - 0x80) * 0x80 + current[2] - 0x80) * 0x80 + current[1] - 0x80) * 0x40 + current[0] - 0x80;
			break;
		default:
			throw StockResWrongData(10);
		}

		skip_byte(nBytes);

		return value;
	}

	void StockRes::umcompress_if()
	{
		len = *(ushort*)(current);
		ulong new_len = *(ushort*)(current + 2);
		ulong new_len2 = new_len;

		skip_byte(4);

		if (current[0] == 0x78 && (uchar)current[1] == 0x9C)
		{
			uchar* new_data = new uchar[new_len];
			if (uncompress((Byte*)new_data, &new_len, (Byte*)current, len) == 0 && new_len == new_len2)
			{
				dataNew = true;
				current = data = new_data;
				left = len = new_len;
			}
			else
			{
				delete []new_data;
				throw StockResWrongData(2);
			}
		}
		else
		{
			data += 16;
			current = data;
		}
	}

	uint StockRes::read(const uchar* pData, ulong data_len)
	{
		raw_data = current = data = pData;
		dataNew = false;

		ulong len1 = get_packet_len();
		len1 = len1 < data_len ? len1 : data_len;
		raw_len = left = len = len1;

		ulong len2 = *(ushort*)(pData + 14);

		skip_byte(16 - 4);

		umcompress_if();

		if (len != len2)
		{
			throw StockResWrongData(8);
		}

		return len1;
	}

	ushort StockRes::get_packet_len()
	{
		return get_packet_len(raw_data);
	}

	ushort StockRes::get_packet_len(const uchar* pData)
	{
		return (*(ushort*)(pData + 12)) + 16;
	}

	uint StockRes::get_seq_id()
	{
		return get_seq_id(raw_data);
	}

	uint StockRes::get_seq_id(const uchar* pData)
	{
		return *(uint*)(pData + 5);
	}

	ushort StockRes::get_cmd_id()
	{
		return get_cmd_id(raw_data);
	}

	ushort StockRes::get_cmd_id(const uchar* pData)
	{
		return *(ushort*)(pData + 10);
	}



	extern MarketInfo::MarketType request_type;

	void StockListRes::operator()()
	{
		StockBid bid;

		MarketInfo::stocks_count[request_type] = get_ushort();

		ushort count = get_ushort();
		RCV_REPORT_STRUCTEx* pReport = new RCV_REPORT_STRUCTEx[count];
		int i = 0;

		while (count--)
		{
			uchar market = get_uchar();
			std::string code((char*)current, MarketInfo::StocksCodeLen);

			/*
			if (MarketInfo::stocks_set[market].size() < MarketInfo::stocks_count[market] && market == MarketInfo::get_market_type(code)	&& (!(market == MarketInfo::MARKET_INDEX && code[0] < '3')))
			{
				//MarketInfo::stocks_set[market].push_back(code);
				//MarketInfo::stocks_set_all.push_back(code);
			}
			else
			{
				continue;
			}//*/

			skip_byte(MarketInfo::StocksCodeLen);

			bid.act			= get_ushort();
			bid.price		= parse_data();
			bid.y_close		= parse_data() + bid.price;
			bid.open		= parse_data() + bid.price;
			bid.high		= parse_data() + bid.price;
			bid.low			= parse_data() + bid.price;
			bid.buy			= parse_data() + bid.price;
			bid.sell		= parse_data() + bid.price;
			bid.total_vol	= parse_data2();
			bid.avail_vol	= parse_data2();

			skip_byte(4);

			bid.inner_vol	= parse_data2();
			bid.outer_vol	= parse_data2();
			bid.updown		= parse_data();

			skip_data(1);

			bid.buy_price1	= parse_data() + bid.price;
			bid.sell_price1	= parse_data() + bid.price;
			bid.buy_vol1	= parse_data2();
			bid.sell_vol1	= parse_data2();
			bid.buy_price2	= parse_data() + bid.price;
			bid.sell_price2	= parse_data() + bid.price;
			bid.buy_vol2	= parse_data2();
			bid.sell_vol2	= parse_data2();
			bid.buy_price3	= parse_data() + bid.price;
			bid.sell_price3	= parse_data() + bid.price;
			bid.buy_vol3	= parse_data2();
			bid.sell_vol3	= parse_data2();
			bid.buy_price4	= parse_data() + bid.price;
			bid.sell_price4	= parse_data() + bid.price;
			bid.buy_vol4	= parse_data2();
			bid.sell_vol4	= parse_data2();
			bid.buy_price5	= parse_data() + bid.price;
			bid.sell_price5	= parse_data() + bid.price;
			bid.buy_vol5	= parse_data2();
			bid.sell_vol5	= parse_data2();


			StockCodeMap::const_iterator sci = g_stocklist[market].find(code);
			if (sci != g_stocklist[market].end())
			{
				memset(&pReport[i], 0, sizeof(RCV_REPORT_STRUCTEx));
				pReport[i].m_cbSize = sizeof(RCV_REPORT_STRUCTEx);

				//tm tt;
				//memset(&tt, 0, sizeof(tm));
				//tt.tm_year = 2010 - 1900;
				//tt.tm_mon = 6;
				//tt.tm_mday = 7;
				//tt.tm_hour = 15;
				//tt.tm_min = 0;
				//tt.tm_sec = 0;
				//pReport[i].m_time = mktime(&tt);

				time_t t;
				pReport[i].m_time = time(&t);

				strcpy(pReport[i].m_szLabel, sci->second.stock_code.c_str());
				strcpy(pReport[i].m_szName, sci->second.stock_name.c_str());
				pReport[i].m_wMarket = market == 0 ? SZ_MARKET_EX : SH_MARKET_EX;

				pReport[i].m_fLastClose = (float)bid.y_close / 100;
				pReport[i].m_fOpen = (float)bid.open / 100;
				pReport[i].m_fHigh = (float)bid.high / 100;
				pReport[i].m_fLow = (float)bid.low / 100;
				pReport[i].m_fNewPrice = (float)bid.price / 100;

				pReport[i].m_fBuyPrice[0] = (float)bid.buy_price1 / 100;
				pReport[i].m_fBuyPrice[1] = (float)bid.buy_price2 / 100;
				pReport[i].m_fBuyPrice[2] = (float)bid.buy_price3 / 100;
				pReport[i].m_fBuyVolume[0] = (float)bid.buy_vol1;
				pReport[i].m_fBuyVolume[1] = (float)bid.buy_vol2;
				pReport[i].m_fBuyVolume[2] = (float)bid.buy_vol3;
				pReport[i].m_fSellPrice[0] = (float)bid.sell_price1 / 100;
				pReport[i].m_fSellPrice[1] = (float)bid.sell_price2 / 100;
				pReport[i].m_fSellPrice[2] = (float)bid.sell_price3 / 100;
				pReport[i].m_fSellVolume[0] = (float)bid.sell_vol1;
				pReport[i].m_fSellVolume[1] = (float)bid.sell_vol2;
				pReport[i].m_fSellVolume[2] = (float)bid.sell_vol3;
				pReport[i].m_fBuyPrice4 = (float)bid.buy_price4 / 100;
				pReport[i].m_fBuyVolume4 = (float)bid.buy_vol4;
				pReport[i].m_fBuyPrice5 = (float)bid.buy_price5 / 100;
				pReport[i].m_fBuyVolume5 = (float)bid.buy_vol5;
				pReport[i].m_fSellPrice4 = (float)bid.sell_price4 / 100;
				pReport[i].m_fSellVolume4 = (float)bid.sell_vol4;
				pReport[i].m_fSellPrice5 = (float)bid.sell_price5 / 100;
				pReport[i].m_fSellVolume5 = (float)bid.sell_vol5;
				pReport[i].m_fAmount = (float)bid.total_vol;
				pReport[i].m_fVolume = (float)bid.total_vol;

				i++;
			}

			// find next item.
			if (count != 0)
			{
				skip_byte(8);

				const uchar* p = 0;
				if ((p = stock_code_search(current, 15)) != 0)
				{
					skip_byte(p - current - 1);
				}
				else
				{
					throw StockResWrongData(3);
				}
			}
		}

		RCV_DATA* pHeader = new RCV_DATA;
		memset(pHeader, 0, sizeof(RCV_DATA));
		RCV_FILE_HEADEx file;
		memset(&file, 0, sizeof(RCV_FILE_HEADEx));
		pHeader->m_File = file;
		pHeader->m_bDISK = FALSE;
		pHeader->m_wDataType = RCV_REPORT;
		pHeader->m_nPacketNum = i;
		pHeader->m_pReport = pReport;

		if (i > 1)
		{
			g_sendlist.push_back(pHeader);
		}
	}

	// 股本除权结构
	void StockHoldChgRes::operator()()
	{
		TDX_GBBQ gbbq;
		ushort stock_count = get_ushort();
		while (stock_count-- > 0)
		{
			//skip_byte(1);
			TDXMarket market = (TDXMarket)get_char();

			if (!stockcode_is_valid(current))
			{
				throw StockResWrongData(4);
			}

			string stockcode((char*)current, MarketInfo::StocksCodeLen);
			int uints = atoi(stockcode.c_str());
			skip_byte(MarketInfo::StocksCodeLen);

			ushort record_count = get_ushort();
			while (record_count-- > 0)
			{
				skip_byte(MarketInfo::StocksCodeLen + 2);
				uint dt = get_uint();
				gbbq.market = market;
				gbbq.code = stockcode;
				gbbq.date = dt;
				gbbq.chg_type = get_uchar();
				gbbq.data.gb.old_cir = get_float();
				gbbq.data.gb.old_ttl = get_float();
				gbbq.data.gb.new_cir = get_float();
				gbbq.data.gb.new_ttl = get_float();
			}
		}
	}

	void StockCurrentPriceRes::operator()()
	{
		posix_time::ptime now(posix_time::second_clock::local_time());
		posix_time::time_duration td(now.time_of_day());

		skip_byte(2);

		StockBid bid;			// zero it.
		ushort count = get_ushort();
		while(count--)
		{
			std::string code((char*)(current + 1), MarketInfo::StocksCodeLen);
			skip_byte(MarketInfo::StocksCodeLen + 1);

			bid.minute = (uint)(td.hours() * 60 + td.minutes());
			if(bid.minute > 15 * 60) 
				bid.minute = 15 * 60;
			else if(11 * 60 + 30 < bid.minute && bid.minute < 13 * 60)
				bid.minute = 11 * 60 + 30;

			bid.act = get_ushort();
			bid.price = get_ushort();
			bid.y_close = parse_data() + bid.price;
			bid.open	   = parse_data() + bid.price;
			bid.high	   = parse_data() + bid.price;
			bid.low    = parse_data() + bid.price;
			bid.buy    = parse_data() + bid.price;
			bid.sell	   = parse_data() + bid.price;
			bid.total_vol = parse_data2();
			bid.avail_vol = parse_data2();
			skip_byte(4);
			bid.inner_vol = parse_data2();
			bid.outer_vol = parse_data2();
			bid.updown	 = parse_data();
			parse_data2();
			bid.buy_price1 = parse_data() + bid.price;
			bid.sell_price1 = parse_data() + bid.price;
			bid.buy_vol1 =	 parse_data2();
			bid.sell_vol1 =   parse_data2();
			bid.buy_price2 = parse_data() + bid.price;
			bid.sell_price2 = parse_data() + bid.price;
			bid.buy_vol2 =	 parse_data2();
			bid.sell_vol2 =   parse_data2();
			bid.buy_price3 = parse_data() + bid.price;
			bid.sell_price3 = parse_data() + bid.price;
			bid.buy_vol3 =	 parse_data2();
			bid.sell_vol3 =   parse_data2();
			bid.buy_price4 = parse_data() + bid.price;
			bid.sell_price4 = parse_data() + bid.price;
			bid.buy_vol4 =	 parse_data2();
			bid.sell_vol4 =   parse_data2();
			bid.buy_price5 = parse_data() + bid.price;
			bid.sell_price5 = parse_data() + bid.price;
			bid.buy_vol5 =	 parse_data2();
			bid.sell_vol5 =   parse_data2();

			// find next item.
			if(0 != count)
			{
				skip_byte(8);
				const uchar* p = 0;
				if(0 != (p = stock_code_search(current, 15)))
				{
					skip_byte(p - current - 1);
				}
				else
				{
					throw StockResWrongData(3);
				}
			}
		}
		return;
	}


	void StockKlineRes::operator()()
	{
		StockKLine kline;
		bool first = true;

		TdxRequestMap::const_iterator si = g_stockrequest.find(get_seq_id());

		ushort count = get_ushort();
		cout << "KLine: " << si->second.market << " " << si->second.stock << " " << count;

		RCV_DATA* pHeader = new RCV_DATA;
		RCV_HISTORY_STRUCTEx* pDay = new RCV_HISTORY_STRUCTEx[count+1];
		memset(pDay, 0, sizeof(RCV_HISTORY_STRUCTEx) * (count + 1));
		RCV_EKE_HEADEx m_head;
		m_head.m_dwHeadTag = 0xFFFFFFFF;
		m_head.m_wMarket = si->second.market == 1 ? SH_MARKET_EX : SZ_MARKET_EX;
		strcpy(m_head.m_szLabel, si->second.stock);
		int i = 1;

		while (count--)
		{
			kline.time = get_uint();
			if (first)
			{
				kline.open = parse_data();
				first = false;
			}
			else
			{
				kline.open = parse_data() + kline.close;
			}
			kline.close = parse_data() + kline.open;
			kline.high = parse_data() + kline.open;
			kline.low = parse_data() + kline.open;
			kline.vol = parse_data();
			kline.money = get_uint();

			int ntime = kline.time;
			tm tt;
			memset(&tt, 0, sizeof(tm));
			tt.tm_year = ntime / 10000L - 1900;
			tt.tm_mon = (ntime - ntime / 10000L * 10000L) / 100L - 1;
			tt.tm_mday = ntime - ntime / 100L * 100L;
			pDay[i].m_time = mktime(&tt);
			pDay[i].m_fOpen = (float)kline.open / 1000;
			pDay[i].m_fClose = (float)kline.close / 1000;
			pDay[i].m_fHigh = (float)kline.high / 1000;
			pDay[i].m_fLow = (float)kline.low / 1000;
			pDay[i].m_fVolume = kline.vol;
			pDay[i].m_fAmount = kline.money;
			i++;

			cout << " " << dec << kline.time << " " << kline.open << " " << kline.vol << " " << kline.money;
		}

		float p = pDay[1].m_fOpen;
		memcpy(pDay, &m_head, sizeof(RCV_EKE_HEADEx));
		p = pDay[1].m_fOpen;

		//pHeader->m_File = 0;
		pHeader->m_bDISK = FALSE;
		pHeader->m_wDataType = FILE_HISTORY_EX;
		pHeader->m_nPacketNum = i;
		pHeader->m_pDay = pDay;

		g_sendlist.push_back(pHeader);

		cout << endl;
	}

	StockResWrongData::StockResWrongData(int i) : n_(i)
	{
	}

	const char* StockResWrongData::what() const throw()
	{
		return "StockResWrongData";
	}



	void StockTestRes::operator()()
	{
		switch (ucmd)
		{
		case CMD_CODELIST:		// 市场证券数
			{
				uint16 count = get_ushort();
				TdxRequestMap::const_iterator si = g_stockrequest.find(get_seq_id());
				if (si == g_stockrequest.end())
				{
					return;
				}

				if (si->second.cmd == CMD_CODELIST)
				{
					g_marketcount[si->second.market] = count;
				}

				g_stockrequest.erase(si);
			}
			break;

		case CMD_CODETABLE:		// 市场代码表
			{
				uint16 count = get_ushort();
				TdxRequestMap::const_iterator si = g_stockrequest.find(get_seq_id());
				if (si == g_stockrequest.end())
				{
					return;
				}

				if (si->second.cmd == CMD_CODETABLE)
				{
					TDX_STOCKCODE sc;

					while (count--)
					{
						string stockcode((char*)current, /*MarketInfo::StocksCodeLen*/6);
						sc.stock_code = stockcode;
						skip_byte(/*MarketInfo::StocksCodeLen*/6);

						sc.stock_unit = *(uint16*)current;
						skip_byte(sizeof(uint16));

						string stockname((char*)current, 8);
						stockname.erase(stockname.find_last_not_of(" \t\n\r") + 1);
						sc.stock_name = stockname;
						skip_byte(8);

						skip_byte(4);

						sc.stock_digit = *(uint8*)current;
						skip_byte(sizeof(uint8));

						sc.stock_close = *(float*)current;
						skip_byte(sizeof(float));

						skip_byte(4);

						g_stocklist[si->second.market].insert(make_pair(sc.stock_code, sc));
					}
				}

				g_stockrequest.erase(si);
			}
			break;

		case CMD_FILEINFO:
			{
				uint filelenght = get_uint();
				uchar count = get_uchar();
				string md5 = (char*)current;

				g_stockfile.filesize = filelenght;
				g_stockfile.filemd5 = md5;
			}
			break;

		case CMD_FILEDOWN:
			{
				string filename = g_stockfile.filename;

				ofstream ofs;
				if (g_stockfile.current)
				{
					ofs.open(filename.c_str(), ios::out | ios::binary | ios::app);
				}
				else
				{
					ofs.open(filename.c_str(), ios::out | ios::binary);
				}
				if (!ofs)
				{
					break;
				}

				uint filelenght = get_uint();

				if (g_stockfile.current + filelenght <= g_stockfile.filesize)
				{
					ofs.seekp(g_stockfile.current, ios::beg);
					ofs.write((char*)current, filelenght);

					g_stockfile.current += filelenght;
					int n = g_stockfile.current;
					int n1 = n;
				}
			}
			break;

		case CMD_STOCKPRICE:
			{
				ushort count = get_ushort();
				while (count--)
				{
					uchar market = get_uchar();
					string stock_code((char*)current, 6);
					skip_byte(6);

					float a = get_float();
					float b = get_float();

					uint date = get_uint();

					for (int i = 0; i < 31; i++)
					{
						float c = get_float();
					}
				}
			}
			break;

		default:
			break;
		}
	}



	void ProcessResponsePacket(const uchar* pData, size_t len, bool test)
	{
		static uint packet_len = 0;
		static uint packet_curr_pos = 0;
		static uchar packet_buff[40960];

		if (test)
		{
			packet_len = len;
		}
		else
		{
			// application layer assembly 应用层组包
			if (pData[0] == 0xB1 && pData[1] == 0xCB && pData[2] == 0x74 && pData[3] == 0x00)
			{
				packet_curr_pos = 0;
				packet_len = StockRes::get_packet_len(pData);
			}
		}

		if (packet_curr_pos + len <= packet_len)
		{
			memcpy(&packet_buff[0] + packet_curr_pos, pData, len);
			packet_curr_pos += len;
		}
		else
		{
			return;
		}

		if (packet_curr_pos == packet_len)
		{
			// 组包完成, 调整指针
			pData = &packet_buff[0];
			len = packet_len;
			packet_len = packet_curr_pos = 0;
		}
		else
		{
			return;
		}

		int cmd_id = StockRes::get_cmd_id(pData);
		try
		{
			StockRes* p = NULL;

			switch (cmd_id)
			{
			case CMD_STOCKHOLD_CHANGE:			// 股本除权结构
				p = new StockHoldChgRes;
				break;
			case CMD_STOCK_LIST:
				p = new StockListRes;
				break;
			case CMD_INSTANT_TRANS:
				p = new InstantTransRes;
				break;
			case CMD_HIS_TRANS:
				p = new HisTransRes;
				break;
			case CMD_CURRENT_PRICE:
				p = new StockCurrentPriceRes;
				break;
			case CMD_KLINE:
				p = new StockKlineRes;
				break;

			default:
				p = new StockTestRes(cmd_id);
			}

			boost::scoped_ptr<StockRes> pCmd(p);

			uint processed;
			while (len > 0)
			{
				processed = pCmd->read(pData, len);

				switch (cmd_id)
				{
				//case CMD_CODELIST:
				//	break;
				//case CMD_CODETABLE:
				//	break;
				case CMD_STOCK_LIST:
					break;
				case CMD_KLINE:
					break;
				case CMD_STOCKPRICE:
					break;
				default:
					log_packet((const char*)pCmd->data, pCmd->len, true, cmd_id);
					break;
				}

				(*pCmd)();

				Request::res_seq_id(StockRes::get_seq_id(pData));

				len -= processed;
				pData += processed;
			}
		}
		catch (StockResWrongData& e)
		{
			cout << "Exception Id = " << e.n_ << endl;
			int c = g_stocklist[0].size();
			int n = c;
		}
		catch (...)
		{
			cout << "Command " << hex << cmd_id << dec << " not implemented" << endl;
		}

		packet_len = packet_curr_pos = 0;
	}
}
