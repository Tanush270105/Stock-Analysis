#include "StockAnalysisIndicators.h"
#include <cmath>
#include <numeric>
#include <algorithm>

// Bollinger Bands calculation
std::tuple<double, double> AdvanceIndicators::calculateBollingerBands(const std::vector<double>& prices, int period) {
    int n = prices.size();
    if (n < period) return {0.0, 0.0};

    double sum = std::accumulate(prices.end() - period, prices.end(), 0.0);
    double movingAverage = sum / period;

    double sq_sum = 0.0;
    for (int i = n - period; i < n; ++i) {
        sq_sum += std::pow(prices[i] - movingAverage, 2);
    }
    double stdDev = std::sqrt(sq_sum / period);

    return {movingAverage + 2 * stdDev, movingAverage - 2 * stdDev};
}

double AdvanceIndicators::calculateEMA(const std::vector<double>& prices, int period) {
    if (prices.size() < period) return 0.0;

    double multiplier = 2.0 / (period + 1);
    double ema = prices[prices.size() - period];
    for (int i = prices.size() - period + 1; i < prices.size(); ++i) {
        ema = ((prices[i] - ema) * multiplier) + ema;
    }
    return ema;
}

double AdvanceIndicators::calculateMACD(const std::vector<double>& prices, int shortPeriod, int longPeriod, int signalPeriod) {
    double shortEMA = calculateEMA(prices, shortPeriod);
    double longEMA = calculateEMA(prices, longPeriod);
    double macd = shortEMA - longEMA;

    std::vector<double> macdValues;
    for (int i = longPeriod; i < prices.size(); ++i) {
        macdValues.push_back(calculateEMA(std::vector<double>(prices.begin() + i - longPeriod, prices.begin() + i), shortPeriod) -
                             calculateEMA(std::vector<double>(prices.begin() + i - longPeriod, prices.begin() + i), longPeriod));
    }
    double signalLine = calculateEMA(macdValues, signalPeriod);
    return macd - signalLine;
}

double AdvanceIndicators::calculateRSI(const std::vector<double>& prices, int period) {
    if (prices.size() < period + 1) return 0.0;

    double gains = 0.0, losses = 0.0;
    for (int i = prices.size() - period; i < prices.size() - 1; ++i) {
        double change = prices[i + 1] - prices[i];
        if (change > 0) gains += change;
        else losses -= change;
    }
    double avgGain = gains / period;
    double avgLoss = losses / period;
    double rs = avgGain / avgLoss;
    return 100.0 - (100.0 / (1.0 + rs));
}

double AdvanceIndicators::calculateATR(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int period) {
    if (highs.size() < period + 1 || lows.size() < period + 1 || closes.size() < period + 1) return 0.0;

    std::vector<double> trueRanges;
    for (size_t i = 1; i < highs.size(); ++i) {
        double highLow = highs[i] - lows[i];
        double highClose = std::fabs(highs[i] - closes[i - 1]);
        double lowClose = std::fabs(lows[i] - closes[i - 1]);
        trueRanges.push_back(std::max({highLow, highClose, lowClose}));
    }

    return calculateEMA(trueRanges, period);
}
