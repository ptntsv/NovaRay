#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <thread>

using std::make_shared;
using std::shared_ptr;

namespace utility {
double inf = std::numeric_limits<double>::infinity();
double random_double(double min, double max) {
    thread_local std::mt19937 gen;
    std::uniform_real_distribution<> distr(min, max);
    return distr(gen);
}

double degrees_to_radians(double degrees) { return degrees / 180.0 * M_PI; }

double clamp(double x, double lo, double hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}
}  // namespace utility
