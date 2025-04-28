#pragma once

#include <algorithm>
#include <memory>

#include "quad.hpp"
using std::make_shared;
using std::shared_ptr;

class box {
    std::vector<shared_ptr<hittable>> sides;

public:
    box(const point3& p0, const point3& p1, const shared_ptr<material>& mat) {
    }
    // bool hit(const ray& ray, interval tint, hit_record& record) const
    // override {
    //     return std::any_of(sides.begin(), sides.end(),
    //                        [&](shared_ptr<hittable> q) mutable {
    //                            return q && q->hit(ray, tint, record);
    //                        });
    // }
    operator hittable_list() { return hittable_list(sides); }
};
