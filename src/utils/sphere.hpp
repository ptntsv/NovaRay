#pragma once

#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class sphere : public hittable {
    point3 center1;
    vec3 center_vec;
    bool is_moving = false;
    double r;
    material* mat;

public:
    bool hit(const ray& ray, interval tint, hit_record& record) override {
        point3 center = is_moving ? center1 + center_vec * ray.time() : center1;
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

    sphere(const point3& center, const double& r, material* material)
        : center1(center), center_vec(0,0,0), r(std::fmax(0, r)),
          mat(material), is_moving(false) {
        hitbox = aabb{center1 - r, center1 + r};
    }

    sphere(const point3& center1, const point3& center2, const double& r, material* material)
        : center1(center1), center_vec(center2 - center1), r(std::fmax(0, r)),
          mat(material), is_moving(true) {
        aabb box1(center1 - r, center1 + r);
        aabb box2(center2 - r, center2 + r);
        hitbox = aabb(box1, box2);
    }
    ~sphere() { delete mat; }
};
