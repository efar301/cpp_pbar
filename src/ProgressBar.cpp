#include <iostream>
#include <chrono>
#include <iomanip>
#include <chrono>
#include <string>
#include <format>

#include "ProgressBar.h"

// set width of entire progressbar (bar + timer + iteration time)
void Progressbar::SetWidth(int newWidth) {
    if (newWidth < 42) {
        std::cout << "Width must be greater than 41" << '\n';
        return;
    }
    if (width != newWidth) {
        width = newWidth;
        barWidth = width - 41;
        initialized = false;
    }
}

// set the filler
void Progressbar::SetFiller(char character) { filler = character; }

void Progressbar::SetColor(Color newColor) { 
    color = static_cast<int>(newColor); 
    if (! initialized && ! benchmarking) {
        std::cout << "\033[" << color << 'm';
        std::cout << "\x1b[2K\r" << barTemplate;
        std::cout << "\033[0m";
        std::cout.flush();        
    }
}

// set whether progressbar is being benchmarked or not, default is false
void Progressbar::SetBenchmark(bool isBenchmarking) { benchmarking = isBenchmarking; }

// resets internals when finished
void Progressbar::Reset() {
    SetFiller('#');
    initialized = false;
}

// initializes the progressbar
void Progressbar::Initialize(int percent) {
    barTemplate.clear();
    barTemplate.reserve(width);

    int col = 1;

    barTemplate.append("\033[");
    barTemplate.append(std::to_string(color));
    barTemplate.append("m[");    
    col += 1;

    barStartCol = col;
    barTemplate.append(barWidth, ' ');
    col += barWidth;

    barTemplate.append("] ");
    col += 2;

    percentCol = col;
    barTemplate.append("   % ");
    col += 5;

    barTemplate.push_back('[');
    col += 1;

    iterTimeCol = col - 1;
    barTemplate.append("00:00:00");
    col += 8;

    barTemplate.push_back('>');
    col += 1;

    etaCol = col;
    barTemplate.append("00:00:00");
    col += 8;

    barTemplate.append(", ");
    col += 2;

    rateCol = col;
    barTemplate.append("  0.00");
    col += 7;

    barTemplate.append(" it/s]\033[");
    col += 5;
    fillBuffer.assign(static_cast<size_t>(barWidth), filler);

    // init state
    lastFilled = 0;
    lastPercent = -1;
    lastTime = std::chrono::steady_clock::now();
    initialized = true;

    if (!benchmarking) {
        std::cout << "\x1b[2K\r" << barTemplate;
        std::cout.flush();
    }
}

// updates the progressbar fill amount
void Progressbar::UpdateBar(int filled) {
    if (filled <= lastFilled || benchmarking) {
        return;
    }

    std::cout << "\033[" << 0 << 'G';
    std::cout << '[';
    std::cout << "\033[" << (barStartCol + lastFilled) << 'G';
    std::cout.write(fillBuffer.data() + lastFilled, filled - lastFilled);
    std::cout << "\033[" << (barWidth + barStartCol) << 'G';
    std::cout << ']';
    lastFilled = filled;
}

// updates percentage in bar
void Progressbar::UpdatePercentage(int percent) {
    if (percent == lastPercent || benchmarking) {
        return;
    }

    std::cout << "\033[" << percentCol << 'G';
    char buf[6];
    std::snprintf(buf, sizeof(buf), "%3d%% ", percent);
    std::cout.write(buf, 5);

    lastPercent = percent;
}

// formats time
void Progressbar::UpdateTiming(uint iterationMs, int remaining) {
    if (benchmarking) {
        return;
    }
    auto formatTime = [this](uint ms, char* out) {
        uint sec = ms / 1000;
        uint h = sec / 3600;
        uint m = (sec % 3600) / 60;
        uint s = sec % 60;
        std::snprintf(out, 9, "%02u:%02u:%02u]", h, m, s);
    };

    char iterBuf[9];
    char etaBuf[9];
    formatTime(iterationMs, iterBuf);
    formatTime(iterationMs * remaining, etaBuf);

    std::cout << "\033[" << iterTimeCol << 'G';
    std::cout << '[';
    std::cout.write(iterBuf, 8);

    std::cout << "\033[" << (etaCol - 1) << 'G';
    std::cout << '>';
    std::cout.write(etaBuf, 8);
    std::cout << ',';

    double rate = iterationMs > 0 ? 1000.0 / iterationMs : 0.0;
    char rateBuf[8];
    std::snprintf(rateBuf, sizeof(rateBuf), "%7.2f", rate);


    std::cout << "\033[" << rateCol << 'G';
    std::cout.write(rateBuf, 7);
    std::cout.write("it/s]", 5);
}

// print progressbar per iteration
void Progressbar::PrintProgress(int currentValue, int maxValue) {
    if (!initialized) {
        Initialize(0);
    }

    // set color if not benchmarking
    if (! benchmarking) {
        std::cout << "\033[" << color << 'm';
    }

    // get start time and compare to last start time
    auto now = std::chrono::steady_clock::now();
    uint iterationMs = 0;
    if (lastTime != std::chrono::steady_clock::time_point{}) {
        iterationMs = static_cast<uint>(
            std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count());
    }
    lastTime = now;

    int percent = (maxValue > 0) ? currentValue * 100 / maxValue : 100;
    int filled = (percent * (barWidth)) / 100;

    UpdateBar(filled);
    UpdatePercentage(percent);

    int remaining = std::max(0, maxValue - currentValue);
    UpdateTiming(iterationMs, remaining);
    

    if (!benchmarking) {
        std::cout << "\033[0m"; 
        if (percent >= 100) {
            std::cout << '\n';
            Reset();
        } else {
            std::cout << '\r';
            std::cout.flush();
            std::cout << "\033[" << width + 1 << "G";
        } 
    }
}
