#pragma once

#include "vec3.hpp"

using Point3 = Vec3;

class Ray {
    Point3 orig;
    Vec3 dir;

   public:
    Ray() {}
    const Point3& origin() const { return orig; }
    const Point3& direction() const { return dir; }
    Ray(const Point3& origin, const Vec3& direction)
        : orig(origin), dir(direction) {}
    Ray(const Ray& other) : orig(other.orig), dir(other.dir) {}
};
