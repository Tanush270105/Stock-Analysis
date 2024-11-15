#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <tuple>
#include <sqlite3.h>

class DatabaseManager {
public:
    // Constructor that opens the database
    DatabaseManager(const std::string& dbName);

    // Destructor that closes the database
    ~DatabaseManager();

    // Method to log historical stock data (prices and volumes)
    void logStockData(const std::string& stockSymbol, const std::vector<double>& prices, const std::vector<int>& volumes);

    // Method to log stock prediction (predicted price, upper band, lower band)
    void logPrediction(const std::string& stockSymbol, double predictedPrice, double upperBand, double lowerBand);

    // Method to retrieve historical stock data (price and volume) for a given stock symbol
    std::vector<std::tuple<std::string, double, int>> getHistoricalData(const std::string& stockSymbol);

private:
    sqlite3* db; // Pointer to the SQLite database

    // Helper function to create the stock data table if it doesn't exist
    void createStockTableIfNotExists();

    // Helper function to create the prediction table if it doesn't exist
    void createPredictionTableIfNotExists();
};

#endif // DATABASE_MANAGER_H