#include "DatabaseManager.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <tuple>
#include <algorithm>

// Constructor - opens the database connection and ensures tables exist
DatabaseManager::DatabaseManager(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        db = nullptr;
    } else {
        createStockTableIfNotExists(); // Create table for stock data if it doesn't exist
        createPredictionTableIfNotExists(); // Create table for stock predictions if it doesn't exist
    }
}

// Destructor - closes the database connection
DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
    }
}

// Helper function to create the stock data table if it doesn't already exist
void DatabaseManager::createStockTableIfNotExists() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS stock_data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            stock_symbol TEXT NOT NULL,
            price REAL NOT NULL,
            volume INTEGER NOT NULL
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating stock_data table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Helper function to create the predictions table if it doesn't already exist
void DatabaseManager::createPredictionTableIfNotExists() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS predictions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date TEXT NOT NULL,
            stock_symbol TEXT NOT NULL,
            predicted_price REAL NOT NULL,
            upper_band REAL NOT NULL,
            lower_band REAL NOT NULL
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating predictions table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Logs historical stock data (prices and volumes) into the stock_data table
void DatabaseManager::logStockData(const std::string& stockSymbol, const std::vector<double>& prices, const std::vector<int>& volumes) {
    const char* sql = "INSERT INTO stock_data (date, stock_symbol, price, volume) VALUES (?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing stock data insert statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    time_t now = time(0);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Loop over all prices and volumes, insert each one into the database
    for (size_t i = 0; i < prices.size(); ++i) {
        sqlite3_bind_text(stmt, 1, dateStr, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, stockSymbol.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, prices[i]);
        sqlite3_bind_int(stmt, 4, volumes[i]);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error inserting stock data: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_reset(stmt); // Reset the statement for the next iteration
    }

    sqlite3_finalize(stmt);
}

// Logs a stock prediction (predicted price, upper band, lower band) into the predictions table
void DatabaseManager::logPrediction(const std::string& stockSymbol, double predictedPrice, double upperBand, double lowerBand) {
    const char* sql = "INSERT INTO predictions (date, stock_symbol, predicted_price, upper_band, lower_band) VALUES (?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing prediction insert statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    time_t now = time(0);
    char dateStr[20];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", localtime(&now));

    sqlite3_bind_text(stmt, 1, dateStr, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, stockSymbol.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 3, predictedPrice);
    sqlite3_bind_double(stmt, 4, upperBand);
    sqlite3_bind_double(stmt, 5, lowerBand);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting prediction: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

// Retrieves the historical stock data (price and volume) for a given stock symbol
std::vector<std::tuple<std::string, double, int>> DatabaseManager::getHistoricalData(const std::string& stockSymbol) {
    std::vector<std::tuple<std::string, double, int>> historicalData;
    const char* sql = "SELECT date, price, volume FROM stock_data WHERE stock_symbol = ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing historical data query: " << sqlite3_errmsg(db) << std::endl;
        return historicalData;
    }

    sqlite3_bind_text(stmt, 1, stockSymbol.c_str(), -1, SQLITE_TRANSIENT);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        double price = sqlite3_column_double(stmt, 1);
        int volume = sqlite3_column_int(stmt, 2);
        historicalData.push_back(std::make_tuple(date, price, volume));
    }

    sqlite3_finalize(stmt);
    return historicalData;
}
