
#include "stdafx.h"
#include "config.h"
#include "TdxStructDef.h"

#include "ClientSocket.h"

//#include "HostCache.h"

#include <boost/thread/recursive_mutex.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace boost;

#include "util.hpp"

#include "request.h"

#include "stockdrv.h"
//#include "stock.h"

#include "commands.h"

namespace StockMarket
{
	unsigned int MarketInfo::stocks_count[16];
	StringSet MarketInfo::stocks_set[16];
	StringSet stocks_set_all;

	MarketInfo::MarketType request_type = MarketInfo::MARKET_UNKNOWN;

	const int BaseInfo::record_len = 0x01E1;
	fstream& operator >> (fstream& fs, BaseInfo& bs)
	{
		char code[MarketInfo::StocksCodeLen];

		fs.read(code, MarketInfo::StocksCodeLen);
		bs.stock_code = string(&code[0], MarketInfo::StocksCodeLen);

		fs >> bs.update_time;			// 更新时间
		fs >> bs.ttl_amount;			// 
		fs >> bs.state_own_amount;		// 国家股
		fs >> bs.init_amount;			// 发起股本
		fs >> bs.corp_amount;			// 法人股本
		fs >> bs.b_amount;				// B 股本
		fs >> bs.h_amount;				// H 股本
		fs >> bs.cir_amount;			// 流通 股本
		fs >> bs.empl_amount;			// 职工 股本
		fs >> bs.unknown1;				// 
		fs >> bs.ttl_asset;				// 总资产
		fs >> bs.varible_asset;			// 流动 资产
		fs >> bs.firm_asset;			// 固定 资产
		fs >> bs.invisible_asset;		// 无形 资产
		fs >> bs.long_term_invest;		// 长期投资
		fs >> bs.varible_debt;			// 流动负债
		fs >> bs.long_term_debt;		// 长期负债
		fs >> bs.accu_fund;				// 公积金
		fs >> bs.net_asset;				// 净资产
		fs >> bs.major_income;			// 主营收入
		fs >> bs.major_profit;			// 主营利润
		fs >> bs.unknown2;				// 
		fs >> bs.bussiness_income;		// 营业收入
		fs >> bs.invest_income;			// 投资收入
		fs >> bs.allowance;				// 补贴收入
		fs >> bs.non_bussiness_income;	// 业外收入
		fs >> bs.income_adjustment;		// 收入调整
		fs >> bs.ttl_profit;			// 利润总额
		fs >> bs.unknown3;				// 
		fs >> bs.net_profit;			// 税后利润
		fs >> bs.undist_profit;			// 未分配利润
		fs >> bs.per_net_assert2;		// 每股净资产2

		return fs;
	}

	// 加载股票基本资料
	bool StockBasicInfo::load_basic_info()
	{
		stock_base_info.clear();

		const string stock_data_path = "C:\\Program Files\\new_ztzq_v6";
		string full_path = stock_data_path + "\\T0002\\hq_cache\\base.dbf";

		fstream fs(full_path.c_str(), ios::in);
		if (!fs)
		{
			return false;
		}

		int record_off = 0x0522;
		fs.seekg(record_off);

		BaseInfo bs;

		char market;
		fs >> market;
		while (!fs.eof() && -1 != fs.tellg())
		{
			//cout << "position = " << hex << fs.tellg() << dec;

			fs >> bs;

			string symbol;
			symbol = market + bs.stock_code;
			stock_base_info.insert(make_pair(symbol, bs));

			record_off += BaseInfo::record_len;
			fs.seekg(record_off);

			//cout << " next position = " << hex << record_off << dec << " eof = " << fs.eof() << endl;

			fs >> market;
		}

		cout << "load baseinfo " << stock_base_info.size() << " ok" << endl;

		return true;
	}

	// 加载板块资料
	bool StockBasicInfo::load_block_info()
	{
		stock_block_info.clear();

		const string stock_data_path = "C:\\Program Files\\new_ztzq_v6";
		string full_path = stock_data_path + "\\T0002\\hq_cache\\block.dat";

		fstream fs(full_path.c_str(), ios::in | ios::binary);
		if (!fs)
		{
			return false;
		}

		fs.seekg(0x0182);

		int p = 0;
		while (!fs.eof() && -1 != (p = fs.tellg()))
		{
			cout << "position = " << hex << p << dec;

			char temp[10];
			fs.read(&temp[0], 9);
			if (temp[0] == 0) break;

			// block name
			string block(temp);
			cout << " block = " << block;

			unsigned short cnt = 0;
			fs.read((char*)&cnt, 2);
			fs.seekg(2, ios::cur);
			cout << " count = " << cnt;

			while (cnt-- > 0)
			{
				fs.read(&temp[0], 7);
				if (temp[0] == 0) break;

				// stock name
				string stock(temp, 6);

				stock_block_info[block].push_back(stock);
			}

			fs.seekg(p + 0x0AFD);
			fs >> temp;
			fs.seekg(p + 0x0AFD);

			cout << " next position = " << hex << p + 0x0AFD << dec << " eof = " << fs.eof() << endl;
		}

		cout << "block count = " << stock_block_info.size() << endl;

		return true;
	}

	bool StockTransact::operator == (const StockTransact& t)
	{
		return (minute == t.minute
			&& price == t.price
			&& vol == t.vol
			&& bs == t.bs);
	}

	extern void output_short_index_data();

	uint date_to_uint(gregorian::date d)
	{
		return d.year() * 10000 + d.month() * 100 + d.day();
	}

	gregorian::date uint_to_date(uint d)
	{
		return gregorian::date(d / 10000, d % 10000 / 100, d % 100);
	}

	string MarketInfo::get_first_stock()
	{
		for (short m = MarketInfo::MARKET_SHANGHAI_A; m < MarketInfo::MARKET_UNKNOWN; m++)
		{
			StringSet::const_iterator iter = MarketInfo::stocks_set[m].begin();
			if (iter != MarketInfo::stocks_set[m].end())
			{
				return *iter;
			}
		}

		return "";
	}

	string MarketInfo::find_next_stock(const string& stock_code)
	{
		bool first_in_market = false;
		for (short m = MarketInfo::get_market_type(stock_code); m < MarketInfo::MARKET_UNKNOWN; m++)
		{
			StringSet::const_iterator iter;
			if (!first_in_market)
			{
				iter = std::find(MarketInfo::stocks_set[m].begin(), MarketInfo::stocks_set[m].end(), stock_code);
				if (iter != MarketInfo::stocks_set[m].end())
				{
					++iter;
					if (iter != MarketInfo::stocks_set[m].end())
					{
						return *iter;
					}
				}
				first_in_market = true;
			}
			else
			{
				iter = MarketInfo::stocks_set[m].begin();
				if (iter != MarketInfo::stocks_set[m].end())
				{
					return *iter;
				}
			}
		}

		return "";
	}


	// 查询消息队列
	CmdQueue_t& CmdQueue()
	{
		static CmdQueue_t q;
		return q;
	}

	void push_cmd_queue(CmdData_t r)
	{
		recursive_mutex::scoped_lock guard(req_queue_mutex);
		CmdQueue().push(r);
	}

	void send_queued_request(CClientSocket* cs)
	{
		static bool initial_send = true;

		//cout << "send_queued_request enter\r\n";

		recursive_mutex::scoped_lock guard(req_queue_mutex);
		CmdQueue_t &q = CmdQueue();

		if (!q.empty())
		{
			CmdData_t &d = q.front();
			if (!initial_send)
			{
				d->next();
			}
			else
			{
				initial_send = false;
			}

			if (*d)
			{
				d->send(cs);
			}
			else
			{
				initial_send = true;
				q.pop();
			}
		}

		//cout << "send_queued_request exit\r\n";
	}



	StockBasicInfo basic_info;



	uint16 g_marketcount[2] = { 0 };	// 市场证券数
	uint16 g_marketinit[2] = { 0 };		// 市场初始化
	StockCodeMap g_stocklist[2];		// 市场代码表

	TDX_REQFILE g_stockfile;

	TdxRequestMap g_stockrequest;

	// 加载证券代码表
	int load_stock_code(TDXMarket market)
	{
		if (market != tdxMarketShenZhen && market != tdxMarketShangHai)
		{
			return 0;
		}

		const string stock_data_path = "C:\\Program Files\\new_ztzq_v6";
		string full_path = stock_data_path + "\\T0002\\hq_cache\\szex.tnf";
		if (market == tdxMarketShangHai)
		{
			full_path = stock_data_path + "\\T0002\\hq_cache\\shex.tnf";
		}

		g_stocklist[market].clear();

		fstream fs(full_path.c_str(), ios::in);
		if (!fs)
		{
			return 0;
		}

		int record_offset = 0x32;
		fs.seekg(record_offset);

		TDX_STOCKCODE sc;

		char szValue[16] = { 0 };
		fs >> szValue;
		while (!fs.eof() && -1 != fs.tellg())
		{
			int pos = record_offset;

			sc.stock_code = szValue;

			fs.seekg(pos + 0x18);
			fs.read(szValue, 10);
			sc.stock_name = szValue;

			fs.seekg(pos + 0xF1);
			fs >> szValue;
			sc.stock_short = szValue;

			//cout << "code = " << sc.stock_code << " name = " << sc.stock_name << " short = "  << sc.stock_short << endl;

			g_stocklist[market].insert(make_pair(sc.stock_code, sc));

			record_offset += stockocde_len;
			fs.seekg(record_offset);

			fs >> szValue;
		}

		cout << "load " << market << " stock code count = " << g_stocklist[market].size() << endl;
		return g_stocklist[market].size();
	}

	// 查询股票列表
	void AddStockListRequest(bool bAll)
	{
		int nTotal, nOneRe;

		if (bAll)
		{
			nTotal = MarketInfo::stocks_count[MarketInfo::MARKET_STOCK_ALL];
			nOneRe = 70;
			CmdData_t request(new StockListReq(MarketInfo::MARKET_STOCK_ALL, 0, nOneRe, nTotal));
			push_cmd_queue(request);
		}
		else
		{
			for (MarketInfo::MarketType mType = MarketInfo::MARKET_SHANGHAI_A; mType < MarketInfo::MARKET_UNKNOWN; mType = (MarketInfo::MarketType)(mType + 1))
			{
				nTotal = nOneRe = 1;
				CmdData_t request(new StockListReq(mType, 0, nOneRe, nTotal));
				push_cmd_queue(request);
			}
		}
	}

	// 加载权息数据
	void AddGBBQRequest()
	{
		for (MarketInfo::MarketType m = MarketInfo::MARKET_SHANGHAI_A; m < MarketInfo::MARKET_UNKNOWN; m = (MarketInfo::MarketType)(m + 1))
		{
			for (StringSet::iterator i = MarketInfo::stocks_set[m].begin(); i != MarketInfo::stocks_set[m].end(); )
			{
				//StockHoldChgReq* req = new StockHoldChgReq;
				//for (int j = 0; j < StockHoldChgReq::max_stocks_a_request && i != MarketInfo::stocks_set[m].end(); ++i, ++j)
				//{
				//	req->add_stock(*i);
				//}

				//CmdData_t request(req);
				//push_cmd_queue(request);
			}
		}
	}

	sendlistd g_sendlist;




	// this is the thread to request data
	void communication()
	{
		//basic_info.load_basic_info();
		//basic_info.load_block_info();

		//load_stock_code(tdxMarketShenZhen);
		//load_stock_code(tdxMarketShangHai);

		//HostCache.ImportTDX();

		CClientSocket* pSocket = new CClientSocket();

		if (!pSocket->Create())
			return;

		if (!pSocket->Connect("121.15.137.163", 7709))
			return;

		bool initial_login_finished = false;
		bool initial_stockcode_finished = false;
		bool initial_stocksets_finished = false;
		bool initial_gbbq_info_finished = false;

		int heart_beat_timer = 0;

		while (true)
		{
			recursive_mutex::scoped_lock guard(req_queue_mutex);

			Sleep(100);

			CmdQueue_t &q = CmdQueue();
			if (q.empty())
			{
				// 初始化代码表，首先初始化变量 g_stocklist
				if (g_marketcount[0] <= 0 && g_marketinit[0] == 0)
				{
					TdxRequest* pRequest = new TdxRequest(CMD_CODELIST);
					pRequest->WriteData((uint16)tdxMarketShenZhen);
					pRequest->WriteData((uint32)0);

					CmdData_t t(pRequest);
					q.push(t);

					g_marketinit[0] = 1;
				}
				else if (g_marketcount[0] > 0 && g_stocklist[0].size() <= 0)
				{
					TdxRequest* pRequest = new TdxRequest(CMD_CODETABLE);
					pRequest->WriteData((uint16)tdxMarketShenZhen);
					pRequest->WriteData((uint16)0);
					pRequest->SetTotal(g_marketcount[0]);

					CmdData_t t(pRequest);
					q.push(t);

					g_marketinit[0] = 2;
				}

				if (g_marketcount[1] <= 0 && g_marketinit[1] == 0)
				{
					TdxRequest* pRequest = new TdxRequest(CMD_CODELIST);
					pRequest->WriteData((uint16)tdxMarketShangHai);
					pRequest->WriteData((uint32)0);

					CmdData_t t(pRequest);
					q.push(t);

					g_marketinit[0] = 1;
				}
				else if (g_marketcount[1] > 0 && g_stocklist[1].size() <= 0)
				{
					TdxRequest* pRequest = new TdxRequest(CMD_CODETABLE);
					pRequest->WriteData((uint16)tdxMarketShangHai);
					pRequest->WriteData((uint16)0);
					pRequest->SetTotal(g_marketcount[1]);

					CmdData_t t(pRequest);
					q.push(t);

					g_marketinit[1] = 2;
				}

				//if (!initial_stocksets_finished && g_marketinit[0] == 2 && g_marketinit[1] == 2)
				//{
				//	AddStockListRequest(false);
				//	if (MarketInfo::stocks_count[MarketInfo::MARKET_STOCK_ALL] > 0)
				//	{
				//		AddStockListRequest(true);
				//		initial_stocksets_finished = true;
				//	}
				//}

				//if (initial_stocksets_finished && (!initial_gbbq_info_finished))
				//{
				//	cout << "AddGBBQRequest start" << endl;
				//	AddGBBQRequest();
				//	initial_gbbq_info_finished = true;
				//}

				if (g_marketinit[0] >= 2 && g_stocklist[0].size())
				{

					static StockCodeMap::iterator stockbase = g_stocklist[tdxMarketShenZhen].begin();

					stockbase++;

					if (stockbase == g_stocklist[tdxMarketShenZhen].end())
					{
						stockbase = g_stocklist[tdxMarketShenZhen].begin();
					}

					if (q.empty() && (heart_beat_timer % 1 == 0))
					{
						//CmdData_t t(new StockKLineReq(stockbase->first.c_str(), tdxMarketShenZhen, tdxKlineTypeDay, 0, 280));

						//TdxRequest* pRequest = new TdxRequest(CMD_KLINE);
						//TDX_REQKLINE kline;
						//kline.stock_market = tdxMarketShenZhen;
						//strcpy(kline.stock_code, stockbase->first.c_str());
						//kline.base_period = tdxKlineTypeDay;
						//kline.period_count = 1;
						//kline.offset_start = 0;
						//kline.request_count = 280;

						//pRequest->WriteData((void*)&kline, sizeof(TDX_REQKLINE));
						//pRequest->SetTotal(1000);

						//CmdData_t t(pRequest);

						//q.push(t);
					}


					// 请求文件测试
					static int nFileStats = 0;
					if (nFileStats == 0)
					{
						//g_stockfile.filename = "block.dat";
						//g_stockfile.filesize = 0;
						//g_stockfile.current = 0;

						//TdxRequest* pRequest = new TdxRequest(CMD_FILEINFO);
						//pRequest->WriteData((void*)g_stockfile.filename.c_str(), TDX_FILEINFOLENGHT);

						//CmdData_t t(pRequest);
						//q.push(t);

						//nFileStats++;
					}
					else if (nFileStats == 1 && g_stockfile.filesize > 0)
					{
						//TdxRequest* pRequest = new TdxRequest(CMD_FILEDOWN);
						//pRequest->WriteData((uint32)0);
						//pRequest->WriteData((uint32)0x7530);
						//pRequest->WriteData((void*)g_stockfile.filename.c_str(), TDX_FILEDOWNLENGHT);
						//pRequest->SetTotal(g_stockfile.filesize);

						//CmdData_t t(pRequest);
						//q.push(t);

						//nFileStats++;
					}


					static bool btest = false;
					if (!btest)
					{
						//TdxRequest* pRequest = new TdxRequest(CMD_HISREALTIME);
						//uint stockdate = 20100314;
						//pRequest->WriteData((uint32)stockdate);
						//pRequest->WriteData((uint16)tdxMarketShenZhen);
						//pRequest->WriteData((void*)"000069", 6);

						//InstantTransReq* p = new InstantTransReq(tdxMarketShenZhen, "000069", 1800);

						//HisTransReq* pRequest = new HisTransReq("000069", 20100314);

						//CmdData_t t(pRequest);
						//q.push(t);

						//TdxRequest* pRequest = new TdxRequest(CMD_TEST);
						//pRequest->WriteData((uint16)0);
						//pRequest->AddStock((uchar)tdxMarketShenZhen, "000001");
						//pRequest->WriteData((uint32)0);
						//pRequest->AddStock((uchar)tdxMarketShenZhen, "000004");
						//pRequest->WriteData((uint32)0);

						//TdxRequest* pRequest = new TdxRequest(CMD_STOCKHOLD_CHANGE);
						//pRequest->WriteData((uint16)0);
						//pRequest->AddStock((uchar)tdxMarketShenZhen, "000751");
						//pRequest->AddStock((uchar)tdxMarketShangHai, "600572");

						//CmdData_t r(pRequest);
						//q.push(r);

						btest = true;
					}

					//if (heart_beat_timer % 2 == 0)
					//{
					//	TdxRequest* pRequest = new TdxRequest((TDX_COMMAND)0x000F);
					//	pRequest->WriteData((uint16)1);
					//	pRequest->WriteData((uint8)tdxMarketShenZhen);
					//	pRequest->WriteData((void*)stockbase->first.c_str(), 6);

					//	CmdData_t t(pRequest);

					//	q.push(t);
					//}


					//if (heart_beat_timer % 10000 == 0)
					//{
					//	//InstantTransReq* req = new InstantTransReq(tdxMarketShenZhen, stockbase->first.c_str(), 300);
					//}
				}

				cout << "Request count = " << g_stockrequest.size() << endl;
				heart_beat_timer++;

				//if (!q.empty())
				//{
				//	heart_beat_timer = 0;
				//}
			}

			// if yes, then check time-interval, if more than 1 minite..then add new request to it
			// 当收到 Response 的时候才设置 Ready, 这些避免发送重复的序列号
			if (Request::ready())
			{
				send_queued_request(pSocket);
			}
		}

		//Shutdown
		delete pSocket;
	}
}
