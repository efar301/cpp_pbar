#include <iostream>
#include <format>
#include <algorithm>
#include <cmath>
#include <chrono>

#include "Benchmarking.h"
#include "ProgressBar.h"

Stats Benchmark(int numIterations) {
    Stats stats;
    std::vector<int64_t> msTimes = {};
    Progressbar test;
    test.SetBenchmark(true);

    // warmup
    for (int i = 0; i < 10; ++i) {
        test.PrintProgress(1, 1);
    }

    test.PrintProgress(0, numIterations);
    for (int i = 0; i < numIterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        test.PrintProgress(i + 1, numIterations);
        auto finish = std::chrono::high_resolution_clock::now();

        msTimes.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count());
    }

    int64_t sum = 0;
    int64_t max = 0;
    int64_t min = msTimes[0];
    for (int i = 0; i < numIterations; ++i) {
        min = (min > msTimes[i]) ? msTimes[i] : min;
        max = (max < msTimes[i]) ? msTimes[i] : max;
        sum += msTimes[i];
    }

    int64_t mean = sum / numIterations;
    double stdSum = 0;
    for (int i = 0; i < numIterations; ++i) {
        stdSum += std::pow(msTimes[i] - mean, 2);
    }
    double std = std::sqrt(stdSum /(numIterations - 1));

    double median = 0;
    std::sort(msTimes.begin(), msTimes.end());
    if (msTimes.size() % 2 == 0) {
        median = (msTimes[msTimes.size() / 2] + msTimes[msTimes.size() / 2 - 1]) / 2.0;
    } else {
        median = msTimes[msTimes.size() / 2];
    }

    stats.min = min;
    stats.max = max;
    stats.mean = mean;
    stats.median = median;
    stats.std = std;
    stats.numIterations = numIterations;
    
    return stats;
}

void PrintStats(Stats stats) {
    std::cout << "Stats for " << stats.numIterations << " runs:" << '\n';
    std::cout << std::format("min: {}ns\n", stats.min);
    std::cout << std::format("max: {}ns\n", stats.max);
    std::cout << std::format("mean: {}ns\n", stats.mean);
    std::cout << std::format("median: {}ns\n", stats.median);
    std::cout << std::format("std: {}ns\n", stats.std);
}