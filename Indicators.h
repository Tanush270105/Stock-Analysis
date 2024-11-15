#ifndef TECHNICAL_INDICATORS_H
#define TECHNICAL_INDICATORS_H

#include <vector>

class TechnicalIndicators {
public:
    static double movingAverage(const std::vector<double>& prices, int period);
    static std::vector<double> calculateMACD(const std::vector<double>& prices);
    static std::pair<double, double> calculateBollingerBands(const std::vector<double>& prices, int period);
};

#endif

