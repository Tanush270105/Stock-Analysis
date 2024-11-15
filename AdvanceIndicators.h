#ifndef STOCK_ANALYSIS_INDICATORS_H
#define STOCK_ANALYSIS_INDICATORS_H

#include <vector>
#include <tuple>

class AdvanceIndicators {
public:
    static std::tuple<double, double> calculateBollingerBands(const std::vector<double>& prices, int period);
    static double calculateMACD(const std::vector<double>& prices, int shortPeriod, int longPeriod, int signalPeriod);
    static double calculateRSI(const std::vector<double>& prices, int period);
    static double calculateATR(const std::vector<double>& highs, const std::vector<double>& lows, const std::vector<double>& closes, int period);

private:
    static double calculateEMA(const std::vector<double>& prices, int period);
};

#endif // STOCK_ANALYSIS_INDICATORS_H