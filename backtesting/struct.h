#ifndef STRUCT_H
#define STRUCT_H

#include <mutex>

// Define the Order structure
struct Order
{
    int instrumentIndex;
    bool buysellflag;
    int price;
    int size;
};

// Define the MarketData structure
struct MarketData
{
    long long timestamp;
    int instrumentIndex;
    int bidPrice;
    int bidSize;
    int askPrice;
    int askSize;

    // Define the equality operator (==) for MarketData
   
    bool operator <= (Order &other) const
    {
        return askSize <= other.size;
    }


    bool operator==(const MarketData &other) const {
        return askSize == other.askSize; // Adjust the comparison as needed
    }
};

// Define the TradeData structure
struct TradeData
{
    long long timestamp;
    int instrumentIndex;
    int price;
    int numContracts;
    bool aggressorSide;
};

//Event type enum
enum class EventType
{
    MarketData,
    TradePrint,
    Timer,
    EndOfFile
};


//event Structure
struct Event
{
    EventType type;
    long long timestamp;
    MarketData marketData; // For MarketData events
    TradeData tradeData;   // For TradePrint events
    int timerId;           // For Timer events
};


//Fill Structure
struct Fill
{
    int instrumentIndex;
    bool buysellflag;
    int price;
    int size;
};

#endif // STRUCT_H