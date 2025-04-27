#pragma once
#include "hittable.hpp"

class quad : public hittable {
    point3 Q;
    vec3 v, u, n, w;
    double D;
    material* mat;
    void set_hitbox() {
        auto d1 = aabb(Q, Q + u + v);
        auto d2 = aabb(Q + v, Q + u);
        hitbox = aabb(d1, d2);
    }

public:
    ~quad() { delete mat; }
    quad() = default;
    quad(const point3& q, const vec3& v, const vec3& u, material* mat)
        : Q(q), v(v), u(u), mat(mat) {
        set_hitbox();
        vec3 kn = vec::cross(u, v);
        vec3 unit_normal = vec::unit(kn);
        n = unit_normal;
        w = kn / vec::dot(kn, kn);
        D = vec::dot(n, Q);
    }

    bool hit(const ray& ray, interval tint, hit_record& record) const override {
        auto denom = vec::dot(n, ray.direction());
        if (std::abs(denom) <= 1e-8) return false;
        auto t = (D - vec::dot(n, ray.origin())) / denom;
        if (!tint.contains(t)) return false;

        point3 intersection = ray.at(t);

        if (!is_interior(intersection)) return false;

        record.t = t;
        record.p = intersection;
        record.mat = mat;
        record.set_hit_side(ray, n);
        return true;
    };

    virtual bool is_interior(const point3& intersection) const {
        vec3 p = intersection - Q;
        auto alpha = vec::dot(w, vec::cross(p, v));
        auto beta = vec::dot(w, vec::cross(u, p));
        interval zeroone{0, 1};
        // return zeroone.contains(alpha) && zeroone.contains(beta);
        return (alpha <= v.length()) && (beta <= u.length());
    }

    void fmt_print(int indent) const override { hitbox.fmt_print(indent); }
};
