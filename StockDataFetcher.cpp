#include "StockDataFetcher.h"
#include <iostream>

StockDataFetcher::StockDataFetcher(const std::string& api_key) : api_key(api_key) {}

std::vector<double> StockDataFetcher::fetchHistoricalPrices(const std::string& symbol, int days) {
    std::vector<double> prices;
    using namespace web::http;
    using namespace web::http::client;

    http_client client(API_ENDPOINT + symbol + "&apikey=" + api_key);
    auto request = client.request(methods::GET).get();

    if (request.status_code() == status_codes::OK) {
        auto jsonResponse = request.extract_json().get();
        auto dailyData = jsonResponse[U("Time Series (Daily)")];
        
        for (auto iter = dailyData.as_object().begin(); iter != dailyData.as_object().end() && days > 0; ++iter) {
            double closePrice = std::stod(iter->second[U("4. close")].as_string());
            prices.push_back(closePrice);
            --days;
        }
    } else {
        std::cerr << "Failed to retrieve data\n";
    }

    return prices;
}
