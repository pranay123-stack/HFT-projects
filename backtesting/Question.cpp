/*

Simple Backtester with Order Management and Matching Engine

Please try to write code of basic backtesting design of strategies, try to first spend time thinking about how you would design this mini system, that's as important as the code you will write.

Read about the concept of the central limit order book if you haven't heard about them before, I can also explain over a call in a few mins.

The data required by this exercise is here: https://www.dropbox.com/scl/fo/tyyezv4ihffu953faaxmh/h?dl=0&rlkey=rqk19wb4413lemjjw2od1jecv
Note that data only captures the first level of the order book on both the bid and ask side and trade prints of a subset of instruments, we ignore the deeper levels of the order book for now.

Let me walk you over to different components:

Data:
Attached is the dropbox folder which has 1 day worth of market data and trade prints data. Two csv files or around 10G total, if you have network bandwidth or space constraints, let me know I will have to do some work to make it only for 1 hour rather than a full day.
The columns of Market data csv are the following as below, note that the data is arranged in a sorted order of timestamp format
GMT Time Stamp in the form of duration in nanoseconds from epoch will be in the range 3:45 am to 10 am GMT (9:15 am to 3:30 pm IST)
Instrument Index (int in the range of around 0 to 220 or something), this represents the unique ID of the contract for which market data is listed
Bid Price as Int
Bid Size as Int
Ask Price as Int
Ask Size as Int
Similarly, the trade prints data representing the trades done during the day on each instrument. This is again indexed by sorted order of time stamp and columns represent the following:
Timestamp same format as above
Instrument Index of the traded instrument
Price as which trade happened in int
Number of contracts traded in int
Aggressor side: bool 1 or 0, 0 means bid aggressed to meet ask and 1 means ask aggressed to meet bid for the trade to happen (not that important for this exercise)

Now the exercise is to design three classes that interact with each other and the main function that plays the whole backtest
Order Management System (OMS): Central hub of interaction to play the backtesting, acts as the dummy matching engine as well
Market Data Reader (MDR): Keeps reading the market data from these files and supplies it to OMS
Strategy: A dummy strategy that has certain functions/callbacks that should be hit and a user can write whatever code inside them, for now, you can write whatever dummy even sleep or do nothing, but you need to submit few orders and get a few fills. The callbacks that a strategy should have are:
init() : Initialization of the strategy
OnMarketDataUpdate(): strategy knows that market data has changed, the strategy owner can decide to perform some action and may decide to submit an order
OnTradePrint(): strategy knows that something traded in the market
OnTimer(): strategy gets fix callbacks every fix interval, any computations that need to done at fixed intervals need to be coded in this part of the strategy
sendOrder(): strategy sends the order to the OMS and OMS tries to fill the order (in live engine it will send the order to the exchange)
onFill(): OMS informs the strategy that one of its filled order just traded (in live engine exchange will inform OMS about the fill)
stop(): final call when the simulation ends

Here is the order of how the backtester should be run:
Step 1: OMS calls the initialization function on the strategy to ask itself about list/vector of instruments its interested in listening to, any timers (fix interval callbacks) that the strategy needs to subscribed to, can be multiple timers, may name them
Step 2: OMS calls the market data reader to initialize itself, gives a date as input and the reader warms itself up by creating a pointer to the file ready to start reading data
Step 3: In a for loop running from the start of the day to the end of day data, do the following in the time-ordered fashion
- OMS asks for the next event from MDR: which can be a new market data packet or trade print or timer update, if it is not on the list of interesting instruments from the strategy it rejects it and moves forward
- OMS stores the current market state on each instrument which gets updated as well
- OMS makes a call back on the strategy: two functions on Strategy onMarketDataUpdate() and onTradePrint() to be called, which a strategy writer can do certain computations. Once these callbacks are hit, strategy can decide to submit orders by sendOrder() method. Order submission can have the following fields: Instrument ID, Buy/Sell flag, Price, Size to buy
- If an order is received OMS checks if it can fill that by matching it against the market data, if the strategy wants to buy something at price x then the market should have an offer at price x or better of the given size
- OMS gets the order and tries to match it with market order book on that instrument (which is maintained by OMS). If trade is matched with an order, OMS informs strategy about the trade information, Instrument ID, Buy/Sell, Price, Number of Contracts traded by calling onFill() method.
- Strategy maintains a list of the current portfolio, tuple/map of instrument and position, and updates that when onFill() is called.
- If onTimer() call back is hit, strategy prints the current time
- If onTradePrint() call back is hit on strategy, it currently does nothing
Step 4: Once full-day execution is done, stop callback called on a strategy that outputs its current portfolio.  

Note that all these events like market data update, trade print, timer update, order submission, order fills needs to be executed in a time chronological order



*/