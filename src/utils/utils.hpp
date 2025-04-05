#pragma once

// is it a bad idea to depend on headers order while including this file?
#include <limits>
#include <random>

namespace utility {
double inf = std::numeric_limits<double>::infinity();
double random_double(double min, double max) {
    static std::uniform_real_distribution<double> distr(min, max);
    static std::mt19937 gen;
    return distr(gen);
}

double clamp(double x, double lo, double hi) {
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}
}  // namespace utility
