#pragma once

#include "vec3.hpp"

using point3 = vec3;

class ray {
    point3 orig;
    vec3 dir;
    double tm;

public:
    ray() {}
    ray(const point3& origin, const vec3& direction, const double& time = 0)
        : orig(origin), dir(direction), tm(time) {}
    ray(const ray& other) : orig(other.orig), dir(other.dir), tm(other.tm) {}

    const point3& origin() const { return orig; }
    const point3& direction() const { return dir; }
    const double time() const { return tm; }
    point3 at(double t) const { return orig + t * dir; }
};
