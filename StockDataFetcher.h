#ifndef STOCK_DATA_FETCHER_H
#define STOCK_DATA_FETCHER_H
#include <cpprest/http_client.h>
#include <vector>
#include <string>

class StockDataFetcher {
public:
    StockDataFetcher(const std::string& api_key);
    std::vector<double> fetchHistoricalPrices(const std::string& symbol, int days);
    
private:
    const std::string API_ENDPOINT = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=";
    std::string api_key;
};

#endif
