#pragma once

#include <assert.h>

#include <algorithm>
#include <ostream>

#include "utils.hpp"

struct interval {
    double lo, hi;
    interval() : lo(utility::inf), hi(-utility::inf) {}
    // If there are some bugs with interval 'orientation'
    // probably coz in old good days there is an explicit cheking like
    // lo(std::min(left, right)).
    interval(const double& left, const double& right) : lo(left), hi(right) {}
    // interval(const double& left, const double& right)
    //     : lo(std::min(left, right)), hi(std::max(left, right)) {}
    interval(const interval& i1, const interval& i2) {
        lo = std::min(i1.lo, i2.lo);
        hi = std::max(i1.hi, i2.hi);
    }
    double len() const { return hi - lo; }
    bool contains(double x) const { return lo <= x && x <= hi; }
    bool overlaps(const interval& other) const {
        assert(lo <= hi);
        assert(other.lo <= other.hi);
        double tmin = std::max(other.lo, lo);
        double tmax = std::min(other.hi, hi);
        return tmin <= tmax;
    }
    friend std::ostream& operator<<(std::ostream& os, const interval& i) {
        os << "[" << i.lo << ";" << i.hi << "]";
        return os;
    }
    bool operator==(const interval& other) const {
        return other.lo == lo && other.hi == hi;
    }
    static const interval empty;
};

// const interval interval::empty{0, 0};
const interval interval::empty{utility::inf, -utility::inf};
