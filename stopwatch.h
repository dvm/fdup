#pragma once

#include <iostream>
#include <chrono>

class stopwatch
{
    std::chrono::steady_clock::time_point start;

public:
    stopwatch()
    {
        start = std::chrono::steady_clock::now();
    }

    std::chrono::duration<double> elapsed() const
    {
        return std::chrono::steady_clock::now() - start;
    }

    std::chrono::duration<double> reset()
    {
        std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
        start = std::chrono::steady_clock::now();
        return elapsed;
    }
};

std::ostream& operator<<(std::ostream& os, stopwatch const& sw)
{
    os.precision(3);
    os.setf(std::ios::fixed, std::ios::floatfield);
    return os << sw.elapsed().count() << 's';
}
