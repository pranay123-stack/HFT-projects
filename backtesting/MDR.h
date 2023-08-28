#ifndef MDR_H
#define MDR_H

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include "struct.h"

class MarketDataReader
{
public:
    void initialize(const std::string &marketDataFilePath, const std::string &tradePrintFilePath);
    Event getNextEvent();
    void close();

private:
    std::chrono::steady_clock::time_point lastTimerEventTime; // Declare lastTimerEventTime as a member variable
    bool timerStopped;                                        // Declare timerStopped as a member variable

    std::ifstream marketDataFile;
    std::ifstream tradePrintFile;
    bool marketDataAvailable;
    bool tradePrintAvailable;
    // Add data members for timer update
    long long prevEpochTime;
    bool hasPrevEpochTime;
    

    
};

#endif // MDR_H