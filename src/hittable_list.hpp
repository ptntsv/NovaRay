#pragma once

#include <vector>

#include "utils/bvh.hpp"
#include "utils/hittable.hpp"

class hittable_list : public hittable {
    // i hope it's good place to use unique pointers or smth
    std::vector<hittable*> objs{};
    bvh_node* bvh = nullptr;

public:
    hittable_list() = default;
    ~hittable_list() { delete bvh; }
    hittable* at(size_t index) const { return objs.at(index); }
    size_t size() const { return objs.size(); }
    void add(hittable* obj) { objs.push_back(obj); }
    void build_bvh() { bvh = new bvh_node(objs, 0, size()); }
    bool hit(const ray& ray, interval tint, hit_record& record) override {
        hit_record tmp_record{};
        bool any = false;
        if (bvh->hit(ray, tint, tmp_record)) {
            any = true;
            tint.hi = tmp_record.t;
            record = tmp_record;
        }
        // for (size_t i = 0; i < objs.size(); ++i) {
        //     if (objs.at(i)->hit(ray, tint, tmp_record)) {
        //         any = true;
        //         tint.hi = tmp_record.t;
        //         record = tmp_record;
        //     }
        // }
        return any;
    }

    void fmt_print(int indent) const override { bvh->fmt_print(indent); }
};
