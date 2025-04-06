#pragma once

#include "aabb.hpp"
#include "ray.hpp"

class material;
struct hit_record {
    double t;
    point3 p;
    material* mat;
    vec3 normal;
    bool outside;

    void set_hit_side(const ray& ray, const vec3& out_normal) {
        outside = vec::dot(ray.direction(), out_normal) < 0;
        normal = outside ? out_normal : -out_normal;
    }
};

class hittable {
public:
    aabb hitbox = aabb::empty;
    virtual bool hit(const ray& ray, interval tint, hit_record& record) = 0;
    virtual ~hittable() = default;
};
