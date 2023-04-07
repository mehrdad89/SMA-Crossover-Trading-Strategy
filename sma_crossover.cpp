#include <iostream>
#include <vector>

using namespace std;

// Function to calculate the Simple Moving Average (SMA)
double calculateSMA(const vector<double>& data, const vector<double>::const_iterator& start, const vector<double>::const_iterator& end) {
    double sum = 0.0;
    for (auto it = start; it != end; ++it) {
        sum += *it;
    }
    return sum / distance(start, end);
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

    // Calculate the short-term and long-term moving averages
    vector<double> shortTermMA;
    vector<double> longTermMA;
    for (auto it = stockPrices.begin() + shortTermMALength - 1; it != stockPrices.end(); ++it) {
        shortTermMA.push_back(calculateSMA(stockPrices, it - shortTermMALength + 1, it + 1));
    }
    for (auto it = stockPrices.begin() + longTermMALength - 1; it != stockPrices.end(); ++it) {
        longTermMA.push_back(calculateSMA(stockPrices, it - longTermMALength + 1, it + 1));
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
}
