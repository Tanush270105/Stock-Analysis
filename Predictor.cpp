#include "Predictor.h"

double predictNextDayPrice(const std::vector<double>& prices) {
    int n = prices.size();
    if (n < 2) return prices.back();  // Return last price if insufficient data

    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (int i = 0; i < n; ++i) {
        sumX += i;
        sumY += prices[i];
        sumXY += i * prices[i];
        sumX2 += i * i;
    }
    double slope = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    double intercept = (sumY - slope * sumX) / n;

    return slope * n + intercept;  // Predict next day price
}

