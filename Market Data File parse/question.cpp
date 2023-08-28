/*

Question-decoding/ parsing Thailand market data to a .csv file for backtesting purposes?


//coulmns are as follows 


flag: 0 or 1, which means 0 = tick and 1 = lob
time: receive time
match_time: if there is no match_time, then just fill in time
seq: unique sequence id
status: product status, such as opening, continuous, closing ... etc, examples be like:
    open = 8
    normal = 16
    close = 4
    not continuous = 128
    delay open = 192
    delay close = 160
product: product name
price: tick price
quantity: tick quantity
bidn,bidnv: bid1 price, bid1v quantity, bid2 price, bid2 quantity ...
bidnum: bidn's length, ex we have bid5 then bidnum = 5
askn,asknv: ask1 price, ask1v quantity, ask2 price, ask2 quantity ...
asknum: askn's length, ex we have ask5 then asknum = 5
[11:12 AM, 8/7/2023] +886 953 800 824: These are our backtest data fields for Taiwan market data, so we would love to have similar data structure in the Thailand market data





*/