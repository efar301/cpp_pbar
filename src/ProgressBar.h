#pragma once

#include <string>

class Progressbar {
   private:
    // bar personalization
    int color = static_cast<int>(Color::WHITE);
    char filler = '#';

    // bar stats
    bool benchmarking = false;
    int width = 100;
    bool initialized = false;
    int barWidth = width - 41;

    // col nums for where everything is placed
    int barStartCol;
    int percentCol;
    int iterTimeCol;
    int etaCol;
    int rateCol;

    std::string barTemplate;
    std::string fillBuffer;
    int lastFilled;
    int lastPercent;
    std::chrono::steady_clock::time_point lastTime{};

    // helper functions
    void Initialize(int percent);
    void UpdateBar(int filled);
    void UpdatePercentage(int percent);
    void UpdateTiming(uint iterationMs, int remaining);
    void Reset();

   public:
    enum Color {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37
    };

    // set to benchmark mode on your hardware
    void SetBenchmark(bool isBenchmarking);
    void PrintProgress(int currentValue, int maxValue);

    // set personalization
    void SetWidth(int newWidth);
    void SetFiller(char filler);
    void SetColor(enum Color);
};