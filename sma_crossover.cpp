#include <iostream>
#include <vector>

using namespace std;

// Function to calculate the Simple Moving Average (SMA)
double calculateSMA(const vector<double>& data, int length) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    double sma = sum / length;
    for (int i = length; i < data.size(); i++) {
        sum += data[i] - data[i - length];
        sma = sum / length;
    }
    return sma;
}

// Function to generate trading signals based on SMA crossover
int generateSignal(const vector<double>& shortTermMA, const vector<double>& longTermMA) {
    double currentShortTermMA = shortTermMA.back();
    double currentLongTermMA = longTermMA.back();
    double previousShortTermMA = shortTermMA[shortTermMA.size() - 2];
    double previousLongTermMA = longTermMA[longTermMA.size() - 2];

    if (currentShortTermMA > currentLongTermMA && previousShortTermMA <= previousLongTermMA) {
        return 1; // Buy signal
    } else if (currentShortTermMA < currentLongTermMA && previousShortTermMA >= previousLongTermMA) {
        return -1; // Sell signal
    } else {
        return 0; // No signal
    }
}

int main() {
    vector<double> stockPrices = { 100.0, 110.0, 120.0, 130.0, 140.0, 130.0, 120.0, 110.0, 100.0, 90.0 };
    int shortTermMALength = 3;
    int longTermMALength = 5;

    if (stockPrices.size() < longTermMALength) {
        cerr << "Error: stockPrices vector is too short." << endl;
        return 1;
    }

    // Calculate the short-term and long-term moving averages
    vector<double> shortTermMA;
    vector<double> longTermMA;
    for (int i = shortTermMALength; i <= stockPrices.size(); i++) {
        vector<double> shortTermPrices(stockPrices.begin() + i - shortTermMALength, stockPrices.begin() + i);
        shortTermMA.push_back(calculateSMA(shortTermPrices, shortTermMALength));
    }
    for (int i = longTermMALength; i <= stockPrices.size(); i++) {
        vector<double> longTermPrices(stockPrices.begin() + i - longTermMALength, stockPrices.begin() + i);
        longTermMA.push_back(calculateSMA(longTermPrices, longTermMALength));
    }

    if (shortTermMA.empty() || longTermMA.empty()) {
        cerr << "Error: short-term or long-term moving averages could not be calculated." << endl;
        return 1;
    }

    // Generate trading signals based on SMA crossover
    int signal = generateSignal(shortTermMA, longTermMA);
    if (signal == 1) {
        cout << "Buy signal generated!" << endl;
    } else if (signal == -1) {
        cout << "Sell signal generated!" << endl;
    } else {
        cout << "No signal generated." << endl;
    }

    return 0;
