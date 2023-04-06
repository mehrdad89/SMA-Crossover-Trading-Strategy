#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

// Function to calculate the Simple Moving Average (SMA) using the sliding window method
void calculateSMA(const vector<double>& data, int length, vector<double>& sma) {
    if (length <= 0 || length > data.size()) {
        throw invalid_argument("Invalid SMA length");
    }
    double sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += data[i];
    }
    sma.push_back(sum / length);
    for (int i = length; i < data.size(); i++) {
        sum += data[i] - data[i - length];
        sma.push_back(sum / length);
    }
}

// Function to generate trading signals based on SMA crossover
int generateSignal(const vector<double>& shortTermMA, const vector<double>& longTermMA) {
    if (shortTermMA.empty() || longTermMA.empty()) {
        throw invalid_argument("Empty MA vector");
    }
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

    // Calculate the short-term and long-term moving averages
    vector<double> shortTermMA, longTermMA;
    calculateSMA(stockPrices, shortTermMALength, shortTermMA);
    calculateSMA(stockPrices, longTermMALength, longTermMA);

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
}
