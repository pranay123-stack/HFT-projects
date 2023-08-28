#include "OMS.h"
#include "global.h"
std::mutex m2,m3;
class Strategy;

void OMS::initialize(Strategy *strategy, MarketDataReader *marketDataReader)
{

    if (!strategy || !marketDataReader)
    {
        throw std::invalid_argument("Invalid strategy or marketDataReader");
    }
    strategy_ = strategy;
    marketDataReader_ = marketDataReader;

    strategy->init();

    // Initialize other OMS components if needed
}

void OMS::runBacktest()
{
    Event event;
    while (true)
    {

        event = marketDataReader_->getNextEvent();

        if (event.type == EventType::EndOfFile)
        {
            // call strategy stop function
            //  Call the strategy's stop function
            std::cout << "Event type: Endfile" << std::endl;
            strategy_->stop();
            break;
        }

        switch (static_cast<int>(event.type))
        { // Cast EventType to int
        case static_cast<int>(EventType::MarketData):
            // std::cout << "Event type: MarketData" << std::endl;
            processMarketData(event.marketData);
            break;
        case static_cast<int>(EventType::TradePrint):
            // std::cout << "Event type: TradeData" << std::endl;
            processTradePrint(event.tradeData);
            break;
       
        default:
            break;
        }
    }
}

void OMS::processMarketData(MarketData &marketData)
{
    // Check if the instrument is in the list of interested instruments
    const std::vector<int> &interestedInstruments = strategy_->getInterestedInstruments();
    if (std::find(interestedInstruments.begin(), interestedInstruments.end(), marketData.instrumentIndex) != interestedInstruments.end())
    {
        // Update market state and order book
        marketOrderBook[marketData.instrumentIndex].push_back(marketData);
        // Call the strategy's onMarketDataUpdate method
        strategy_->onMarketDataUpdate(std::move(marketData));
    }
}

void OMS::processTradePrint(const TradeData &tradeData)
{
   // Call the strategy's onTradePrint method
    strategy_->onTradePrint(tradeData);
}

void OMS::processOrder(const Order &order)
{
   
    //  match the order with orders in the order book
    matchOrders(order);

    
}

void OMS::matchOrders(const Order &incomingOrder)
{
    std::lock_guard<std::mutex> lock(m2);
    if (incomingOrder.buysellflag == true)
    {
        int newsize;
       auto it = marketOrderBook.find(incomingOrder.instrumentIndex);
       if (it != marketOrderBook.end())
        {
           std::vector<MarketData> &orders = it->second;

           for (const MarketData &existingOrder : orders)
            {
               
                if (existingOrder.askPrice == incomingOrder.price) // error coming -seg fault in this line
                {
                   
                    if (existingOrder.askSize <= incomingOrder.size)
                    {
                       

                        newsize = incomingOrder.size - existingOrder.askSize;
                        // Update size and erase from order book if fully filled

                        if (newsize == 0)
                        {
                           
                            // Assuming 'existingOrder' is the element you want to remove from the vector
                            int keyToRemoveFrom = existingOrder.instrumentIndex; // Assuming 'instrumentIndex' is the key
                            auto it = marketOrderBook.find(keyToRemoveFrom);

                            if (it != marketOrderBook.end())
                            {
                                std::vector<MarketData> &orders = it->second;
                                orders.erase(std::remove(orders.begin(), orders.end(), existingOrder), orders.end());

                                // If the vector is now empty, you might want to remove the key from the map
                                if (orders.empty())
                                {
                                    marketOrderBook.erase(it);
                                }
                            }
                        }
                    }

                    // Update or re-insert modified order back into the order book
                    

                    
                   
                }
            }
        }

        Fill fill;
        fill.instrumentIndex = incomingOrder.instrumentIndex;
        fill.buysellflag = 1;
        fill.price = incomingOrder.price;
        fill.size = incomingOrder.size;


        
         strategy_->onFill(fill);


        
         // Exit the function after processing and calling onFill
        return; // This will immediately exit the function
    }
    

    else
    {
         int newsize;

        auto it = marketOrderBook.find(incomingOrder.instrumentIndex);

        if (it != marketOrderBook.end())
        {
           
            std::vector<MarketData> &orders = it->second;

          

            for (const MarketData &existingOrder : orders)
            {
               
                if (existingOrder.bidPrice == incomingOrder.price) // error coming -seg fault in this line
                {
                   
                    if (existingOrder.bidSize <= incomingOrder.size)
                    {
                       

                        newsize = incomingOrder.size - existingOrder.bidSize;
                        // Update size and erase from order book if fully filled

                        if (newsize == 0)
                        {
                           
                            // Assuming 'existingOrder' is the element you want to remove from the vector
                            int keyToRemoveFrom = existingOrder.instrumentIndex; // Assuming 'instrumentIndex' is the key
                            auto it = marketOrderBook.find(keyToRemoveFrom);

                            if (it != marketOrderBook.end())
                            {
                                std::vector<MarketData> &orders = it->second;
                                orders.erase(std::remove(orders.begin(), orders.end(), existingOrder), orders.end());

                                // If the vector is now empty, you might want to remove the key from the map
                                if (orders.empty())
                                {
                                    marketOrderBook.erase(it);
                                }
                            }
                        }
                    }

                    // Update or re-insert modified order back into the order book
                    

                    
                   
                }
            }
        }

        Fill fill;
        fill.instrumentIndex = incomingOrder.instrumentIndex;
        fill.buysellflag = 0;
        fill.price = incomingOrder.price;
        fill.size = incomingOrder.size;

         // Initialize the map before using it
           //portfolio_[fill.instrumentIndex] = fill; // Insert a dummy entry to initialize the map

        
         strategy_->onFill(fill );
        
         // Exit the function after processing and calling onFill
        return; // This will immediately exit the function

    }
}

// When receiving an order from the strategy
void OMS::receiveOrder(const Order &order)
{
  
    processOrder(order);
}
