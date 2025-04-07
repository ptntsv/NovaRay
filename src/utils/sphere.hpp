#pragma once

#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <iostream>

class sphere : public hittable {
    point3 center;
    double r;
    material* mat;

public:
    bool hit(const ray& ray, interval tint, hit_record& record) override {
        // std::cout << "SPHERE HIT\n";
        vec3 oc = center - ray.origin();
        vec3 d = ray.direction();
        double a = d.length_squared();
        double t = vec::dot(d, oc);
        double c = oc.length_squared() - r * r;
        double discriminant = t * t - a * c;
        if (discriminant < 0)
            return false;
        double dsqrt = std::sqrt(discriminant);
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
    ~sphere() { delete mat; }
};
