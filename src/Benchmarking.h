#include <cmath>

struct Stats {
    int64_t min;
    int64_t max;
    double mean;
    double median; 
    double std;
    int numIterations;
};

Stats Benchmark(int numIterations);
void PrintStats(Stats stats);