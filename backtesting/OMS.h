#ifndef OMS_H
#define OMS_H


#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <map>
#include <map>
#include <algorithm>
#include <fstream>
#include "struct.h"
#include "MDR.h"
#include "Strategy.h"
#include "global.h"
class Strategy; // Forward declaration


class OMS 
{
public:
     
    void initialize(Strategy *strategy, MarketDataReader *marketDataReader);
    void runBacktest();
    void processOrder(const Order &order); 
    void receiveOrder(const Order &order); 
    void processMarketData( MarketData &marketData);
    void processTradePrint(const TradeData &tradeData);
    void processTimerEvent(int timerId);
    void matchOrders( const Order &orderprice);

private:
    Strategy *strategy_;
     MarketDataReader *marketDataReader_;
   
};


#endif // OMS_H