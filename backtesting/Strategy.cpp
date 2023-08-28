#include "Strategy.h"
#include "MDR.h"
#include "global.h"
#include <format>
#include <ctime>
#include <chrono>
std::mutex m, n;

void Strategy::init()
{

    for (int i = 0; i < 150; i++)
    {
        interestedInstruments_.emplace_back(i); // Use emplace_back for in-place construction
    }
}

std::vector<int> Strategy::getInterestedInstruments()
{
    return interestedInstruments_;
}

void Strategy::onMarketDataUpdate(const MarketData &marketData)
{

    // Lock the mutex before accessing shared resources
    std::lock_guard<std::mutex> lock(m);

    // Example: Submit an order if bid price is higher than a threshold
    if (marketData.bidPrice > 15000)
    {
        Order order;

        order.instrumentIndex = marketData.instrumentIndex;
        order.buysellflag = true; // Buying

        order.price = marketData.askPrice;
        order.size = marketData.bidSize;

        sendOrder(order);
    }
    else
    {
        Order order;

        order.instrumentIndex = marketData.instrumentIndex;
        order.buysellflag = false; // Selling
        order.price = marketData.askPrice;
        order.size = marketData.bidSize;

        sendOrder(order);
    }
}

void Strategy::onTradePrint(const TradeData &tradeData)
{
    // Logic to handle trade print events--not required as per Question
}

void Strategy::onTimer(std::string &timerTimeStr)
{
    std::cout << "TIMER EVENT  AT: " << timerTimeStr << std::endl;
}

void Strategy::sendOrder(const Order &order)
{
    OMS *oms;
    oms->receiveOrder(order);
}

void Strategy::onFill(Fill fill)
{
    

//    std::cout<<"fill price ->"<<" "<<fill.price<<" "
//             <<"fill instrument->"<<" "<<fill.instrumentIndex<<" "
//             <<"fill size"<<" "<<fill.size<<" "
//             <<"fill status-->"<<" "<<fill.buysellflag<<std::endl;

    
   

    // Initialize the map before using it
    portfolio_[fill.instrumentIndex] = fill; // Insert a dummy entry to initialize the map

    

    // Find the Fill object associated with the instrument index
    auto it = portfolio_.find(fill.instrumentIndex);
    if (it != portfolio_.end())
    {
       
        // Instrument index exists, update the Fill object
        it->second = fill;
    }
    
}

void Strategy::stop()
{
    std::ofstream outputFile("portfolio.csv");
    if (outputFile.is_open())
    {
         // Write the header to the CSV file
        outputFile << "InstrumentIndex, NoOfContracts, Buy/Sell, Price\n";

        // Write data for each entry in the portfolio map
        for (const auto &entry : portfolio_)
        {
            std::string buySell = entry.second.buysellflag? "Buy" : "Sell";
            outputFile << entry.second.instrumentIndex << ", "
                       << entry.second.size << ", "
                       << entry.second.buysellflag << ", "
                       << entry.second.price << "\n";
        }
         outputFile.close();
        std::cout << "Portfolio saved to portfolio.csv" << std::endl;

    }
    else
    {
        std::cerr << "Error opening "
                  << "portfolio.csv"
                  << " for writing." << std::endl;
    }
}
