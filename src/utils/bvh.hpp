#pragma once

#include <algorithm>
#include <iostream>

#include "hittable.hpp"

class bvh_node : public hittable {
    hittable* left = nullptr;
    hittable* right = nullptr;

    static bool compare_along_x(const hittable* o1, const hittable* o2) {
        return comparator_along_axis(o1, o2, 0);
    }
    static bool compare_along_y(const hittable* o1, const hittable* o2) {
        return comparator_along_axis(o1, o2, 1);
    }
    static bool compare_along_z(const hittable* o1, const hittable* o2) {
        return comparator_along_axis(o1, o2, 2);
    }

    static bool comparator_along_axis(const hittable* box1,
                                      const hittable* box2,
                                      const size_t axis) {
        return box1->hitbox.at(axis).lo < box2->hitbox.at(axis).lo;
    }

public:
    bvh_node() {}

    bvh_node(std::vector<hittable*> objects, const int lo, const int hi) {
        for (size_t i = lo; i < hi; i++) {
            hitbox = aabb(hitbox, objects[i]->hitbox);
        }
        // std::cout << lo << " " << hi << std::endl;
        // std::cout << hitbox << std::endl;
        // std::cout << "===========\n";
        const size_t axis = hitbox.longest_axis();
        const auto comparator = (axis == 0)   ? compare_along_x
                                : (axis == 1) ? compare_along_y
                                              : compare_along_z;
        const int gap = hi - lo;
        if (gap == 1) {
            left = objects[lo];
        } else if (gap == 2) {
            left = objects[lo];
            right = objects[lo + 1];
        } else {
            std::sort(objects.begin() + lo, objects.begin() + hi, comparator);
            const int mid = lo + gap / 2;
            left = new bvh_node(objects, lo, mid);
            right = new bvh_node(objects, mid, hi);
        }
        if (left)
            hitbox = aabb(left->hitbox, hitbox);
        if (right)
            hitbox = aabb(right->hitbox, hitbox);
    }

    bvh_node(bvh_node* left, bvh_node* right) : left(left), right(right) {}

    bool hit(const ray& ray, const interval tint, hit_record& record) override {
        if (!hitbox.hit(ray, tint))
            return false;

        bool lfound = left ? left->hit(ray, tint, record) : true;
        bool rfound =
            right ? right->hit(ray,
                               interval(tint.lo, lfound ? record.t : tint.hi),
                               record)
                  : true;
        return lfound || rfound;
    }

    ~bvh_node() {
        delete left;
        delete right;
    }
};
