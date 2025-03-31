#pragma once

#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class sphere : public hittable {
    point3 center;
    double r;

public:
    bool hit(const ray& ray, const double& tmin, const double& tmax,
             hit_record& record) {
        vec3 oc = center - ray.origin();
        vec3 d = ray.direction();
        double a = d.length_squared();
        double t = dot(d, oc);
        double c = oc.length_squared() - r * r;
        double discriminant = t * t - a * c;
        if (discriminant < 0)
            return false;
        double dsqrt = std::sqrt(discriminant);
        double root = (t - dsqrt) / a;
        if (root <= tmin || tmax <= root) {
            root = (t + dsqrt) / a;
            if (root <= tmin || tmax <= root)
                return false;
        }
        record.t = root;
        record.p = ray.at(root);
        record.set_hit_side(ray, (record.p - center) / r);
        return true;
    }
    sphere(const point3& center, const double& r)
        : center(center), r(std::fmax(0, r)) {}
};
