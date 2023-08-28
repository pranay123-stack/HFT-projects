#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include "OMS.h"
#include "MDR.h"
#include "Strategy.h"
#include "struct.h"



int main()
{
    try
    {
        MarketDataReader marketDataReader;
        marketDataReader.initialize("m.csv", "t.csv");

        OMS oms;
       

        
        Strategy *strategy = new Strategy(); // Pass OMS instance to constructor

        oms.initialize(strategy, &marketDataReader);




        oms.runBacktest();

        delete strategy;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An exception occurred: " << e.what() << std::endl;
        return 1; // Return a non-zero exit code to indicate an error
    }

    return 0; // Return 0 to indicate successful execution
}
