#include <iostream>
#include <vector>
#include <string>
#include "DatabaseManager.h"

// Example function to simulate fetching stock data (prices and volumes)
void fetchStockData(std::vector<double>& prices, std::vector<int>& volumes) {
    // Simulating stock data for demonstration purposes
    prices = {150.23, 152.45, 154.67, 153.89, 151.22};
    volumes = {1000, 1500, 1200, 1100, 1400};
}

// Example function to simulate stock prediction (predicted price, upper and lower bands)
void makeStockPrediction(double& predictedPrice, double& upperBand, double& lowerBand) {
    // Simulating a simple stock prediction with mock values
    predictedPrice = 155.00;
    upperBand = 157.00;
    lowerBand = 153.00;
}


int main() {
    // Database file name
    std::string dbName = "stocks.db";

    // Create the DatabaseManager object (this will open the database and ensure tables exist)
    DatabaseManager dbManager(dbName);

    // Stock symbol for the demo
    std::string stockSymbol = "AAPL";

    // Step 1: Fetch stock data (prices and volumes)
    std::vector<double> prices;
    std::vector<int> volumes;
    fetchStockData(prices, volumes);

    // Step 2: Log the fetched stock data into the database
    dbManager.logStockData(stockSymbol, prices, volumes);
    std::cout << "Stock data for " << stockSymbol << " has been logged." << std::endl;

    // Step 3: Make a stock prediction (predicted price, upper band, lower band)
    double predictedPrice, upperBand, lowerBand;
    makeStockPrediction(predictedPrice, upperBand, lowerBand);

    // Step 4: Log the stock prediction into the database
    dbManager.logPrediction(stockSymbol, predictedPrice, upperBand, lowerBand);
    std::cout << "Stock prediction for " << stockSymbol << " has been logged." << std::endl;

    // Step 5: Retrieve and display historical stock data from the database
    std::vector<std::tuple<std::string, double, int>> historicalData = dbManager.getHistoricalData(stockSymbol);
    std::cout << "\nHistorical Data for " << stockSymbol << ":\n";
    for (const auto& record : historicalData) {
        std::cout << "Date: " << std::get<0>(record) << ", Price: " << std::get<1>(record) << ", Volume: " << std::get<2>(record) << std::endl;
    }

    return 0;
}