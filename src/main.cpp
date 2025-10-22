#include <iostream>
#include <thread>
#include <chrono>

#include "ProgressBar.h"
#include "Benchmarking.h"

int main() {
    Progressbar bar;
    bar.SetWidth(75);
    bar.SetColor(Progressbar::Color::RED);

    int numIter = 5;

    std::cout << "Standard Progressbar: " << '\n';
    for (int i = 0; i < numIter; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        bar.PrintProgress(i + 1, numIter);
    }

    std::cout << '\n' << "Progressbar with print after: " << '\n';
    for (int i = 0; i < numIter; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        bar.PrintProgress(i + 1, numIter);
        std::cout << "test" << '\n';
    }

    std::cout << '\n';

    int benchmarkIter = 1000000;
    Stats stats = Benchmark(benchmarkIter);
    PrintStats(stats);
    

    return 0;
}