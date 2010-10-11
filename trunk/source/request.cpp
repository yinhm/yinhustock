
#include "stdafx.h"
#include "config.h"
#include "TdxStructDef.h"

#include "ClientSocket.h"


#include <boost/thread/detail/config.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>


#include "request.h"
#include "stock1.h"
#include "util.hpp"


namespace StockMarket
{
	recursive_mutex req_queue_mutex;

	bool Request::received = true;
	uint Request::seq_id = 1;

	Request::Request() : first(true)
	{
	}

	Request::~Request()
	{
	}

	bool Request::ready()
	{
		static uint cnt = 0;

		if (!received)
		{
			// 等待 20 个周期还没有收到 response 则重发
			if ((++cnt) % 50 == 0)
			{
				++seq_id;
				return true;
			}
		}
		else
		{
			cnt = 1;
		}

		return received;
	}

	void Request::res_seq_id(uint id)
	{
		// 只有收到较后面的时候，才设置新值
		// (考虑到有些时候收发的顺序不同)
		if (id >= seq_id || id == 0)
		{
			seq_id = id;
			received = true;
		}
	}

	Request& Request::operator++()
	{
		return *this;
	}

	Request::operator bool()
	{
		if (first) 
		{
			first = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Request::next()
	{
		if (received)
		{
			operator++();
			++seq_id;
		}
	}

	TDX_REQUESTCMD Request::get_request()
	{
		TDX_REQUESTCMD rcmd;
		rcmd.cmd = 0;
		rcmd.seq_id = 0;

		return rcmd;
	}

	void Request::send(CClientSocket* soc)
	{
		TDX_REQUESTHEAD* pHead = (TDX_REQUESTHEAD*)buff();
		pHead->set_seq_id(seq_id);

		soc->Send(buff(), len());
		g_stockrequest.insert(make_pair(seq_id, get_request()));

		switch (pHead->get_cmd())
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
			log_packet(buff(), len(), false, pHead->get_cmd());
			break;
		}

		received = false;
	}



	TDX_REQUESTHEAD::TDX_REQUESTHEAD(ushort cmd_id, ushort packet_len)
	{
		zip = 0x0C;
		seq_id = 0;
		packet_type = 1;
		len = len1 = packet_len - sizeof(TDX_REQUESTHEAD) + 2;
		cmd = cmd_id;
	}

	ushort TDX_REQUESTHEAD::get_len()
	{
		return len;
	}

	void TDX_REQUESTHEAD::set_len(ushort payload_len)
	{
		len = len1 = payload_len;
	}

	uint TDX_REQUESTHEAD::get_seq_id()
	{
		return seq_id;
	}

	void TDX_REQUESTHEAD::set_seq_id(uint id)
	{
		seq_id = id;
	}



	char* StockHeartBeat::buff()
	{
		return (char*)&s;
	}
	ulong StockHeartBeat::len()
	{
		return sizeof(StockHeartBeatStruct);
	}



	StockListReq::StockListReq(MarketInfo::MarketType market_code, ushort record_offset, ushort record_count, ushort record_total)
		: s(market_code, record_offset, record_count), total(record_total)
	{
		if (record_total == 0)
		{
			total = record_count;
		}

		mtype = market_code;
	}

	StockListReq::operator bool()
	{
		return s.offset < total;
	}

	StockListReq& StockListReq::operator++()
	{
		if (s.offset + s.count < total)
		{
			s.offset = s.offset + s.count;
			s.count = (total - s.offset) > s.count ? s.count : (total - s.offset);
		}
		else
		{
			s.offset = total;
		}

		return *this;
	}

	char* StockListReq::buff()
	{
		return (char*)&s;
	}

	ulong StockListReq::len()
	{
		return sizeof(StockListStruct);
	}

	void StockListReq::send(CClientSocket* soc)
	{
		Request::send(soc);

		request_type = mtype;
	}



	bool StockMultiReq::add_stock(const string& stock_code)
	{
		return s.add_one_stock(stock_code);
	}

	bool StockMultiReq::add_stock(TDXMarket market, const string& stock_code)
	{
		return s.add_one_stock(market, stock_code);
	}

	char* StockMultiReq::buff()
	{
		return (char*)&s;
	}

	ulong StockMultiReq::len()
	{
		return sizeof(TDX_REQUESTHEAD) + sizeof(ushort) + sizeof(StockMultiStruct) * s.count;
	}




	// 记录查询KLine信息
	StockKLineReq::StockKLineReq(const char* pcode, TDXMarket market, TDXKLineType type, ushort off_start, ushort off_end)
	{
		memcpy(s.stock_code, pcode, MarketInfo::StocksCodeLen);
		s.market_locale = /*MarketInfo::get_market_location(pcode)*/(ushort)market;
		s.base_period = type;

		if (type == tdxKlineTypeMultiMin)
		{
			s.period_count = 0x0A;
		}
		else if (type == tdxKlineTypeMultiDay)
		{
			s.period_count = 0x2D;				// 45 days
		}
		else
		{
			s.period_count = 0x01;
		}

		s.offset_start = off_start;
		s.offset_end = off_end;
	}

	TDX_REQUESTCMD StockKLineReq::get_request()
	{
		TDX_REQUESTCMD rcmd;
		memset(&rcmd, 0, sizeof(TDX_REQUESTCMD));
		rcmd.cmd = s.header.get_cmd();
		rcmd.seq_id = s.header.get_seq_id();
		rcmd.market = (TDXMarket)s.market_locale;
		strcpy(rcmd.stock, s.stock_code);
		rcmd.stock[6] = '\0';

		return rcmd;
	}

	char* StockKLineReq::buff()
	{
		return (char*)&s;
	}

	ulong StockKLineReq::len()
	{
		return sizeof(StockKLineReqStruct);
	}



	TdxRequest::TdxRequest(TDX_COMMAND cmd)
	{
		TDX_REQUESTHEAD header(cmd);

		memset(&buffer[0], 0, 256);

		nlen = sizeof(header);
		pbuff = &buffer[0] + sizeof(header);
		memcpy(&buffer[0], &header, sizeof(header));

		m_nTotal = 0;
		m_nOffset = 0;
	}

	char* TdxRequest::buff()
	{
		TDX_REQUESTHEAD* p = (TDX_REQUESTHEAD*)&buffer[0];
		p->set_len(nlen - sizeof(TDX_REQUESTHEAD) + 2);

		return &buffer[0];
	}

	ulong TdxRequest::len()
	{
		return nlen;
	}

	void TdxRequest::WriteData(uint8 data8)
	{
		*(uint8*)pbuff = data8;
		nlen += sizeof(uint8);
		pbuff += sizeof(uint8);
	}

	void TdxRequest::WriteData(uint16 data16)
	{
		*(uint16*)pbuff = data16;
		nlen += sizeof(uint16);
		pbuff += sizeof(uint16);
	}

	void TdxRequest::WriteData(uint32 data32)
	{
		*(uint32*)pbuff = data32;
		nlen += sizeof(uint32);
		pbuff += sizeof(uint32);
	}

	void TdxRequest::WriteData(void *pData, uint16 nLen)
	{
		memcpy(pbuff, pData, nLen);
		nlen += nLen;
		pbuff += nLen;
	}

	void TdxRequest::AddStock(uchar market, const string& stock_code)
	{
		uint16* count = (uint16*)(buff() + sizeof(TDX_REQUESTHEAD));
		(*count)++;

		WriteData((uint8)market);
		WriteData((void*)stock_code.c_str(), 6);
	}

	TdxRequest& TdxRequest::operator ++()
	{
		TDX_REQUESTHEAD* pHead = (TDX_REQUESTHEAD*)buff();
		if (pHead->get_cmd() == CMD_CODETABLE)
		{
			if (m_nOffset < m_nTotal)
			{
				m_nOffset += 1000;
			}
			uint16 ddd = *(uint16*)(buff() + sizeof(TDX_REQUESTHEAD));
			*(uint16*)(buff() + sizeof(TDX_REQUESTHEAD) + 2) = m_nOffset;
		}
		else if (pHead->get_cmd() == CMD_FILEDOWN)
		{
			m_nOffset += 30000;
			*(uint32*)(buff() + sizeof(TDX_REQUESTHEAD)) = m_nOffset;
		}
		else if (pHead->get_cmd() == CMD_KLINE)
		{
			TDX_REQKLINE* pKline = (TDX_REQKLINE*)(buff() + sizeof(TDX_REQUESTHEAD));
			if (pKline->offset_start + 280 < m_nTotal)
			{
				pKline->offset_start += 280;
			}
		}

		return *this;
	}

	TdxRequest::operator bool()
	{
		TDX_REQUESTHEAD* pHead = (TDX_REQUESTHEAD*)buff();
		if (pHead->get_cmd() == CMD_CODETABLE)
		{
			return m_nOffset < m_nTotal;
		}
		else if (pHead->get_cmd() == CMD_FILEDOWN)
		{
			return m_nOffset < m_nTotal;
		}
		else if (pHead->get_cmd() == CMD_KLINE)
		{
			TDX_REQKLINE* pKline = (TDX_REQKLINE*)(buff() + sizeof(TDX_REQUESTHEAD));
			return pKline->offset_start + 280 < m_nTotal;
		}

		if (first) 
		{
			first = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	TDX_REQUESTCMD TdxRequest::get_request()
	{
		TDX_REQUESTHEAD* pHead = (TDX_REQUESTHEAD*)buff();

		TDX_REQUESTCMD rcmd;
		rcmd.cmd = pHead->get_cmd();
		rcmd.seq_id = pHead->get_seq_id();

		if (rcmd.cmd == CMD_CODELIST || rcmd.cmd == CMD_CODETABLE)
		{
			rcmd.market = (TDXMarket)*(uint16*)(buff() + sizeof(TDX_REQUESTHEAD));
		}
		else if (rcmd.cmd == CMD_KLINE)
		{
			TDX_REQKLINE* pKline = (TDX_REQKLINE*)(buff() + sizeof(TDX_REQUESTHEAD));
			rcmd.market = (TDXMarket)pKline->stock_market;
			strcpy(rcmd.stock, pKline->stock_code);
			rcmd.stock[6] = '\0';
		}

		return rcmd;
	}
}
