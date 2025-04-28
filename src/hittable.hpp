#pragma once

#include <memory>
#include <vector>

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

class hittable_list;
class hittable : public fmt_printable {
public:
    aabb hitbox = aabb::empty;
    virtual bool hit(const ray& ray, interval tint,
                     hit_record& record) const = 0;
    virtual void fmt_print(int indent) const { hitbox.fmt_print(indent); }
    virtual ~hittable() = default;
    virtual operator hittable_list() = 0;
};

struct hittable_list {
    std::vector<std::shared_ptr<hittable>> items{};
    hittable_list() = default;
    hittable_list(const std::shared_ptr<hittable>& obj) {
        items.push_back(obj);
    }
    hittable_list(const hittable_list& other) {
        for (auto& obj : other.items) {
            items.push_back(obj);
        }
    }
    hittable_list(const std::vector<std::shared_ptr<hittable>>& other) {
        for (auto& i : other) {
            items.push_back(i);
        }
    }
    ~hittable_list() = default;
    std::shared_ptr<hittable> at(size_t index) const { return items.at(index); }
    size_t size() const { return items.size(); }
    void add(const hittable_list& objs) {
        for (auto& i : objs.items) {
            items.push_back(i);
        }
    }
    void add(std::shared_ptr<hittable> obj) {
        add(static_cast<hittable_list>(*obj));
    }
};
