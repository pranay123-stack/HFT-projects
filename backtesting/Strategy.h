#ifndef STRATEGY_H
#define STRATEGY_H
#include "OMS.h"
class OMS;

class Strategy
{
public:
      
    void init() ;
    std::vector<int> getInterestedInstruments() ;
    void onMarketDataUpdate(const MarketData &marketData) ;
    void onTradePrint(const TradeData &tradeData) ;
    void sendOrder(const Order &order) ;
    void onFill(Fill fill) ;
    void stop() ;
   void onTimer( std::string &timerTimeStr);

private:
    std::vector<int> timerIds_;
 
};

#endif // STRATEGY_H
