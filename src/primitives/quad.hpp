#pragma once
#include <memory>

#include "../hittable.hpp"

class quad : public hittable {
protected:
    point3 Q;
    vec3 v, u, n, w;
    double D;
    std::shared_ptr<material> mat_ptr;
    void set_hitbox() {
        auto d1 = aabb(Q, Q + u + v);
        auto d2 = aabb(Q + v, Q + u);
        hitbox = aabb(d1, d2);
    }

public:
    static hittable_list box(const point3& p0, const point3& p1,
                             const shared_ptr<material>& mat) {
        std::vector<shared_ptr<hittable>> sides;
        auto dx = vec3(p1.x() - p0.x(), 0, 0);
        auto dy = vec3(0, p1.y() - p0.y(), 0);
        auto dz = vec3(0, 0, p1.z() - p0.z());
        sides.push_back(
            make_shared<quad>(point3(p0.x(), p0.y(), p0.z()), dx, dy, mat));
        sides.push_back(
            make_shared<quad>(point3(p1.x(), p0.y(), p1.z()), -dz, dy, mat));
        sides.push_back(
            make_shared<quad>(point3(p1.x(), p0.y(), p0.z()), -dx, dy, mat));
        sides.push_back(
            make_shared<quad>(point3(p0.x(), p0.y(), p0.z()), dz, dy, mat));
        sides.push_back(
            make_shared<quad>(point3(p0.x(), p1.y(), p1.z()), dx, -dz, mat));
        sides.push_back(
            make_shared<quad>(point3(p0.x(), p0.y(), p0.z()), dx, dz, mat));
        return hittable_list(sides);
    }
    quad() = default;
    quad(const point3& q, const vec3& v, const vec3& u,
         std::shared_ptr<material> mat)
        : Q(q), v(v), u(u), mat_ptr(mat) {
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
        record.mat = mat_ptr;
        record.set_hit_side(ray, n);
        return true;
    };

    virtual bool is_interior(const point3& intersection) const {
        vec3 p = intersection - Q;
        auto alpha = vec::dot(w, vec::cross(p, v));
        auto beta = vec::dot(w, vec::cross(u, p));
        interval zeroone{0, 1};
        return zeroone.contains(alpha) && zeroone.contains(beta);
    }

    void fmt_print(int indent) const override { hitbox.fmt_print(indent); }
    operator hittable_list() override {
        return hittable_list(std::make_shared<quad>(*this));
    }
};
