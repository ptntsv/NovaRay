#pragma once

#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class sphere : public hittable {
    point3 center;
    double r;
    material* mat;

    ~sphere() { delete mat; }

public:
    bool hit(const ray& ray, const interval tint, hit_record& record) override {
        const vec3 oc = center - ray.origin();
        const  vec3 d = ray.direction();
        const double a = d.length_squared();
        const double t = vec::dot(d, oc);
        const double c = oc.length_squared() - r * r;
        const double discriminant = t * t - a * c;
        if (discriminant < 0)
            return false;
        const double dsqrt = std::sqrt(discriminant);
        double root = (t - dsqrt) / a;
        if (root < tint.lo || tint.hi < root) {
            root = (t + dsqrt) / a;
            if (root < tint.lo || tint.hi < root)
                return false;
        }
        record.t = root;
        record.p = ray.at(root);
        record.set_hit_side(ray, (record.p - center) / r);
        record.mat = mat;
        return true;
    }

    sphere(const point3& center, const double& r, material* mat)
        : center(center), r(std::fmax(0, r)), mat(mat) {
        hitbox = aabb{center - r, center + r};
    }
};
