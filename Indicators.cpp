#include "Indicators.h"
#include <numeric>
#include <cmath>

double TechnicalIndicators::movingAverage(const std::vector<double>& prices, int period) {
    if (prices.size() < period) return 0.0;
    double sum = std::accumulate(prices.end() - period, prices.end(), 0.0);
    return sum / period;
}

std::vector<double> TechnicalIndicators::calculateMACD(const std::vector<double>& prices) {
    std::vector<double> macd;
    // Simplified MACD calculation for demonstration
    for (size_t i = 26; i < prices.size(); ++i) {
        double shortMA = movingAverage({prices.begin() + i - 12, prices.begin() + i}, 12);
        double longMA = movingAverage({prices.begin() + i - 26, prices.begin() + i}, 26);
        macd.push_back(shortMA - longMA);
    }
    return macd;
}

std::pair<double, double> TechnicalIndicators::calculateBollingerBands(const std::vector<double>& prices, int period) {
    double ma = movingAverage(prices, period);
    double sumSq = 0.0;
    for (size_t i = prices.size() - period; i < prices.size(); ++i) {
        sumSq += std::pow(prices[i] - ma, 2);
    }
    double stddev = std::sqrt(sumSq / period);
    return {ma + 2 * stddev, ma - 2 * stddev}; // Upper, Lower Bollinger Bands
}