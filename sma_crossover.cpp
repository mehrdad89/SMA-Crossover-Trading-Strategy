#include <iostream>
#include <vector>
#include <numeric>

// Function to calculate the Simple Moving Average (SMA)
double calculateSMA(const std::vector<double>& data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

// Function to generate trading signals based on SMA crossover
int generateSignal(const std::vector<double>& shortTermMA, const std::vector<double>& longTermMA) {
    auto currentShortTermMA = shortTermMA.back();
    auto currentLongTermMA = longTermMA.back();
    auto previousShortTermMA = shortTermMA[shortTermMA.size() - 2];
    auto previousLongTermMA = longTermMA[longTermMA.size() - 2];

    if (currentShortTermMA > currentLongTermMA && previousShortTermMA <= previousLongTermMA) {
        return 1; // Buy signal
    } else if (currentShortTermMA < currentLongTermMA && previousShortTermMA >= previousLongTermMA) {
        return -1; // Sell signal
    } else {
        return 0; // No signal
    }
}

int main() {
    std::vector<double> stockPrices = { 100.0, 110.0, 120.0, 130.0, 140.0, 130.0, 120.0, 110.0, 100.0, 90.0 };
    std::size_t shortTermMALength = 3;
    std::size_t longTermMALength = 5;

    // Calculate the short-term and long-term moving averages
    std::vector<double> shortTermMA;
    std::vector<double> longTermMA;
    std::size_t maxIndex = std::max(shortTermMALength, longTermMALength);
    for (std::size_t i = maxIndex; i < stockPrices.size(); i++) {
        std::vector<double> shortTermPrices(stockPrices.begin() + i - shortTermMALength, stockPrices.begin() + i);
        std::vector<double> longTermPrices(stockPrices.begin() + i - longTermMALength, stockPrices.begin() + i);
        shortTermMA.push_back(calculateSMA(shortTermPrices));
        longTermMA.push_back(calculateSMA(longTermPrices));
    }

    // Generate trading signals based on SMA crossover
    int signal = generateSignal(shortTermMA, longTermMA);
    if (signal == 1) {
        std::cout << "Buy signal generated!" << std::endl;
    } else if (signal == -1) {
        std::cout << "Sell signal generated!" << std::endl;
    } else {
        std::cout << "No signal generated." << std::endl;
    }
    return 0;
}
