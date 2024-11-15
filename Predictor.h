#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <vector>

// Prediction function that predicts the next day's price based on past prices.
double predictNextDayPrice(const std::vector<double>& prices);

#endif

