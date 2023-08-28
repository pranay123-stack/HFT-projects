#include "MDR.h"
#include "Strategy.h"
#include <chrono>
#include <ctime>
#include <string>

void MarketDataReader::initialize(const std::string &marketDataFilePath, const std::string &tradePrintFilePath)
{
    marketDataFile.open(marketDataFilePath);
    tradePrintFile.open(tradePrintFilePath);

    // Use buffered I/O
    marketDataFile.sync_with_stdio(false);
    tradePrintFile.sync_with_stdio(false);

    marketDataAvailable = marketDataFile.is_open();
    tradePrintAvailable = tradePrintFile.is_open();
    // Initialize other data members
    prevEpochTime = 0;
    hasPrevEpochTime = false;

    if (!marketDataAvailable || !tradePrintAvailable)
    {
        std::cerr << "Error opening files." << std::endl;
        // Handle the error accordingly
    }

    lastTimerEventTime = std::chrono::steady_clock::now(); // Initialize last timer event time
    timerStopped = false;                                  // Initialize timerStopped
}

Event MarketDataReader::getNextEvent()
{
    Event event;
    Strategy *strategy_;

    long long timestamp;
    int instrumentIndex, price, numContracts;
    bool aggressorSide;
    int bidPrice, bidSize, askPrice, askSize;
    char comma; // To consume the comma separator

    // Get the current time
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

    // Check if 20 seconds have passed since the last timer event
    std::chrono::duration<double> timeSinceLastTimer = currentTime - lastTimerEventTime;
    if (!timerStopped && timeSinceLastTimer.count() >= 5.0)
    {

        lastTimerEventTime = currentTime;

        // Get the current time as time_t
        std::time_t currentTimeAsTimeT = std::time(nullptr);

        // Convert time_t to a string representation
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTimeAsTimeT));
        std::string currentTimeStr = buffer;

        // Trigger the timer event and send the current time as a string
        event.type = EventType::Timer;
        event.timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime).time_since_epoch().count();
        strategy_->onTimer(currentTimeStr); // Pass the formatted current time as a string
    }

    if (marketDataAvailable && (marketDataFile >> timestamp >> comma >> instrumentIndex >> comma >> bidPrice >> comma >> bidSize >> comma >> askPrice >> comma >> askSize))
    {

        event.type = EventType::MarketData;
        event.timestamp = timestamp;
        event.marketData.instrumentIndex = instrumentIndex;
        event.marketData.bidPrice = bidPrice;
        event.marketData.bidSize = bidSize;
        event.marketData.askPrice = askPrice;
        event.marketData.askSize = askSize;

        return event;
    }
    else if (tradePrintAvailable && (tradePrintFile >> timestamp >> comma >> instrumentIndex >> comma >> price >> comma >> numContracts >> comma >> aggressorSide))
    {

        event.type = EventType::TradePrint;
        event.timestamp = timestamp;
        event.tradeData.timestamp = timestamp;
        event.tradeData.instrumentIndex = instrumentIndex;
        event.tradeData.price = price;
        event.tradeData.numContracts = numContracts;
        event.tradeData.aggressorSide = aggressorSide;

        return event;
    }

    else
    {

        // Close files and handle end of data or parsing error
        marketDataFile.close();
        tradePrintFile.close();
        event.type = EventType::EndOfFile; // Mark event as end of file
        return event;
    }
}

void MarketDataReader::close()
{
    if (marketDataFile.is_open())
    {
    }
    if (tradePrintFile.is_open())
    {
        tradePrintFile.close();
    }
}
