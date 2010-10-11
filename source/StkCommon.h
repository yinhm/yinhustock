
#pragma once

#include "StkStructDef.h"
#include "TdxStructDef.h"

class CStkCommon
{
public:
	CStkCommon();
	virtual ~CStkCommon();
};

int LoadStockCode(StockCodeMap& tskCodeList, TSKMarket tskMarket);
