#include <iostream>
#include <vector>

using namespace std;

// Function to calculate the Simple Moving Average (SMA)
double calculateSMA(vector<double> data, int length) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += data[data.size() - 1 - i];
    }
    return sum / length;
}

// Function to generate trading signals based on SMA crossover
int generateSignal(vector<double> shortTermMA, vector<double> longTermMA) {
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
    for (int i = shortTermMALength - 1; i < stockPrices.size(); i++) {
        shortTermMA.push_back(calculateSMA(vector<double>(stockPrices.begin() + i - shortTermMALength + 1, stockPrices.begin() + i + 1), shortTermMALength));
    }
    for (int i = longTermMALength - 1; i < stockPrices.size(); i++) {
        longTermMA.push_back(calculateSMA(vector<double>(stockPrices.begin() + i - longTermMALength + 1, stockPrices.begin() + i + 1), longTermMALength));
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
