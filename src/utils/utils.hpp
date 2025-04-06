#pragma once

#include <cmath>
#include <limits>
#include <random>

namespace utility {
double inf = std::numeric_limits<double>::infinity();

double random_double(const double min, const double max) {
    static std::uniform_real_distribution<double> distr(min, max);
    static std::mt19937 gen;
    return distr(gen);
}

inline double degrees_to_radians(const double degrees) {
    return degrees / 180.0 * M_PI;
}

inline double clamp(const double x, const double lo, const double hi) {
    if (x < lo)
        return lo;
    if (x > hi)
        return hi;
    return x;
}
}  // namespace utility
