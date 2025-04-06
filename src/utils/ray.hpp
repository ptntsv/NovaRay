#pragma once

#include "vec3.hpp"

using point3 = vec3;

class ray {
    point3 orig;
    vec3 dir;

public:
    ray() = default;

    ray(const point3& origin, const vec3& direction)
        : orig(origin), dir(direction) {}

    ray(const ray& other) : orig(other.orig), dir(other.dir) {}

    const point3& origin() const { return orig; }
    const point3& direction() const { return dir; }
    point3 at(const double t) const { return orig + t * dir; }
};
