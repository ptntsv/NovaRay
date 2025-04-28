#pragma once

#include <algorithm>
#include <memory>

#include "hittable.hpp"
using std::make_shared;
using std::shared_ptr;

class bvh_node : public hittable {
    shared_ptr<hittable> left = nullptr;
    shared_ptr<hittable> right = nullptr;

    static bool compare_along_x(shared_ptr<hittable> o1,
                                shared_ptr<hittable> o2) {
        return comparator_along_axis(o1, o2, 0);
    }
    static bool compare_along_y(shared_ptr<hittable> o1,
                                shared_ptr<hittable> o2) {
        return comparator_along_axis(o1, o2, 1);
    }
    static bool compare_along_z(shared_ptr<hittable> o1,
                                shared_ptr<hittable> o2) {
        return comparator_along_axis(o1, o2, 2);
    }
    static bool comparator_along_axis(const shared_ptr<hittable> box1,
                                      const shared_ptr<hittable> box2,
                                      size_t axis) {
        return box1->hitbox.at(axis).lo < box2->hitbox.at(axis).lo;
    }

public:
    bvh_node() = default;
    bvh_node(std::vector<shared_ptr<hittable>>& items, int lo, int hi) {
        for (size_t i = lo; i < hi; i++) {
            hitbox = aabb(hitbox, items[i]->hitbox);
        }
        size_t axis = hitbox.longest_axis();
        auto comparator = (axis == 0)   ? compare_along_x
                          : (axis == 1) ? compare_along_y
                                        : compare_along_z;
        int gap = hi - lo;
        if (gap == 1) {
            left = items[lo];
        } else if (gap == 2) {
            left = items[lo];
            right = items[lo + 1];
        } else {
            std::sort(items.begin() + lo, items.begin() + hi, comparator);
            int mid = lo + gap / 2;
            left = make_shared<bvh_node>(items, lo, mid);
            right = make_shared<bvh_node>(items, mid, hi);
        }
        if (left) hitbox = aabb(left->hitbox, hitbox);
        if (right) hitbox = aabb(right->hitbox, hitbox);
    }
    explicit bvh_node(hittable_list objs)
        : bvh_node(objs.items, 0, objs.size()) {}
    bool hit(const ray& ray, interval tint, hit_record& record) const override {
        if (!hitbox.hit(ray, tint)) return false;

        bool lfound = left ? left->hit(ray, tint, record) : false;
        bool rfound =
            right ? right->hit(ray,
                               interval(tint.lo, lfound ? record.t : tint.hi),
                               record)
                  : false;
        return lfound || rfound;
    }
    void fmt_print(int indent) const override {
        hitbox.fmt_print(indent);
        if (left) left->fmt_print(indent + 1);
        if (right) right->fmt_print(indent + 1);
    }
    operator hittable_list() override {
        assert(false);
        return hittable_list();
    }
};
