#pragma once

#include <memory>

#include "aabb.hpp"
#include "utils/ray.hpp"

class material;
struct hit_record {
    double t;
    point3 p;
    std::shared_ptr<material> mat;
    vec3 normal;
    bool outside;

    void set_hit_side(const ray& ray, const vec3& out_normal) {
        outside = vec::dot(ray.direction(), out_normal) < 0;
        normal = outside ? out_normal : -out_normal;
    }
};

class hittable : public fmt_printable {
public:
    aabb hitbox = aabb::empty;
    virtual bool hit(const ray& ray, interval tint,
                     hit_record& record) const = 0;
    virtual void fmt_print(int indent) const { hitbox.fmt_print(indent); }
    virtual ~hittable() = default;
};
