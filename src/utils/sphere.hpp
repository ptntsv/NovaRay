#pragma once

#include "ray.hpp"
#include "vec3.hpp"

class Sphere {
    Point3 center;
    double r;

   public:
    bool intersects(const Ray& ray) {
        Vec3 d = ray.direction();
        double a = dot(d, d);
        double b = -2 * dot(d, center - ray.origin());
        double c = dot(center - ray.origin(), center - ray.origin()) - r * r;
        return b * b - 4 * a * c >= 0;
    }
    Sphere(const Point3& center, const double& r) : center(center), r(r) {}
};
