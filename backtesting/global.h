#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include <vector>
#include <mutex>
#include "struct.h"

extern  std::map<int, std::vector<MarketData> > marketOrderBook; 
extern std::vector<int> interestedInstruments_;
 extern  std::map<int , Fill > portfolio_; 


#endif // GLOBAL_H
