#pragma once
#include "hittable.hpp"

class quad : hittable {
    point3 Q;
    vec3 v, u, n, w;
    double D;
    material* mat;
    void set_hitbox() override {
        auto d1 = aabb(Q, Q + u + v);
        auto d2 = aabb(Q + v, Q + u);
        hitbox = aabb(d1, d2);
    }

public:
    quad() = default;
    quad(const point3& q, const vec3& v, const vec3& u)
        : Q(q),
          v(v),
          u(u),
          n(vec::unit(vec::cross(u, v))),
          w(n / vec::dot(n, n)) {
        quad::set_hitbox();
        D = vec::dot(n, Q);
    }

    bool hit(const ray& ray, interval tint, hit_record& record) override {
        auto denom = vec::dot(n, ray.direction());
        if (std::abs(denom) <= 1e-8) return false;
        auto t = D - vec::dot(n, ray.origin()) / denom;
        if (!tint.contains(t)) return false;

        point3 intersection = ray.at(t);

        if (!is_interior(intersection)) return false;

        record.t = t;
        record.p = intersection;
        record.mat = mat;
        record.set_hit_side(ray, n);
        return true;
    };

    virtual bool is_interior(point3 intersection) {
        vec3 p = intersection - Q;
        auto alpha = vec::dot(w, vec::cross(v, p));
        auto beta = vec::dot(w, vec::cross(p, u));
    }
};