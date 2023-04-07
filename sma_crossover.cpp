#include <iostream>
#include <vector>
#include <numeric>

constexpr std::size_t RSI_PERIOD = 14;
constexpr double OVERBOUGHT_THRESHOLD = 75.0;
constexpr double OVERSOLD_THRESHOLD = 25.0;

// Function to calculate the Simple Moving Average (SMA)
double calculateSMA(const std::vector<double>& data) {
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    return sum / data.size();
}

// Function to calculate the Relative Strength Index (RSI)
double calculateRSI(const std::vector<double>& data, const size_t period) {
    double sumGain = 0.0;
    double sumLoss = 0.0;
    double prevPrice = data.front();
    for (size_t i = 1; i < period; i++) {
        double diff = data[i] - prevPrice;
        if (diff > 0) {
            sumGain += diff;
        } else {
            sumLoss += std::abs(diff);
        }
        prevPrice = data[i];
    }
    double avgGain = sumGain / period;
    double avgLoss = sumLoss / period;
    double rs = avgGain / avgLoss;
    double rsi = 100 - (100 / (1 + rs));
    return rsi;
}

// Function to calculate the Moving Average Convergence Divergence (MACD)
double calculateMACD(const std::vector<double>& data, const size_t shortTermPeriod, const size_t longTermPeriod, const size_t signalPeriod) {
    std::vector<double> shortTermEMA, longTermEMA, macd;
    const double shortTermSmoothingFactor = 2.0 / (shortTermPeriod + 1);
    const double longTermSmoothingFactor = 2.0 / (longTermPeriod + 1);
    const double signalSmoothingFactor = 2.0 / (signalPeriod + 1);

    // Calculate short-term EMA
    shortTermEMA.emplace_back(data.front());
    for (const auto& price : data) {
        double ema = (price - shortTermEMA.back()) * shortTermSmoothingFactor + shortTermEMA.back();
        shortTermEMA.emplace_back(ema);
    }

    // Calculate long-term EMA
    longTermEMA.emplace_back(data.front());
    for (const auto& price : data) {
        double ema = (price - longTermEMA.back()) * longTermSmoothingFactor + longTermEMA.back();
        longTermEMA.emplace_back(ema);
    }

    // Calculate MACD line
    for (size_t i = 0; i < data.size(); i++) {
        macd.emplace_back(shortTermEMA[i] - longTermEMA[i]);
    }

    // Calculate signal line
    std::vector<double> signalLine;
    signalLine.emplace_back(macd.front());
    for (size_t i = 1; i < data.size(); i++) {
        double signal = (macd[i] - signalLine.back()) * signalSmoothingFactor + signalLine.back();
        signalLine.emplace_back(signal);
    }

    return macd.back() - signalLine.back();
}

// Function to generate trading signals based on SMA and RSI
int generateSignal(const std::vector<double>& stockPrices, const std::vector<double>& shortTermMA, const std::vector<double>& longTermMA, int rsiPeriod, double overboughtThreshold, double oversoldThreshold) {
    auto currentShortTermMA = shortTermMA.back();
    auto currentLongTermMA = longTermMA.back();
    auto previousShortTermMA = shortTermMA[shortTermMA.size() - 2];
    auto previousLongTermMA = longTermMA[longTermMA.size() - 2];

    std::vector<double> rsiPrices(stockPrices.end() - rsiPeriod, stockPrices.end());
    double rsi = calculateRSI(rsiPrices, rsiPeriod);

    if (currentShortTermMA > currentLongTermMA && previousShortTermMA <= previousLongTermMA && rsi <= oversoldThreshold) {
        return 1; // Buy signal
    } else if (currentShortTermMA < currentLongTermMA && previousShortTermMA >= previousLongTermMA && rsi >= overboughtThreshold) {
        return -1; // Sell signal
    } else {
        return 0; // No signal
    }
}

int main() {
    std::vector<double> stockPrices = { 100.0, 110.0, 120.0, 130.0, 140.0, 130.0, 120.0, 110.0, 100.0, 90.0 };
    std::size_t shortTermMALength = 3;
    std::size_t longTermMALength = 5;
    std::size_t signalPeriod = 3;

    // Calculate the short-term and long-term moving averages
    std::vector<double> shortTermMA;
    std::vector<double> longTermMA;
    std::size_t maxIndex = std::max(shortTermMALength, longTermMALength);
    for (std::size_t i = maxIndex; i < stockPrices.size(); i++) {
        std::vector<double> shortTermPrices(stockPrices.begin() + i - shortTermMALength, stockPrices.begin() + i);
        std::vector<double> longTermPrices(stockPrices.begin() + i - longTermMALength, stockPrices.begin() + i);
        shortTermMA.push_back(calculateSMA(shortTermPrices));
        longTermMA.push_back(calculateSMA(longTermPrices));
        
        // Calculate the MACD
        double macd = calculateMACD(stockPrices, shortTermMALength, longTermMALength, signalPeriod);
        std::cout << "MACD: " << macd << '\n';
    }

    // Generate trading signals based on SMA crossover
    int signal = generateSignal(stockPrices, shortTermMA, longTermMA, RSI_PERIOD, OVERBOUGHT_THRESHOLD, OVERSOLD_THRESHOLD);
    if (signal == 1) {
        std::cout << "Buy signal generated!" << std::endl;
    } else if (signal == -1) {
        std::cout << "Sell signal generated!" << std::endl;
    } else {
        std::cout << "No signal generated." << std::endl;
    }
    return 0;
}
