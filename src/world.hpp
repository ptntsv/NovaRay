#pragma once

#include <vector>

#include "utils/bvh.hpp"
#include "utils/hittable.hpp"

class hittable_list : public hittable {
private:
    std::vector<hittable*>* objs = new std::vector<hittable*>();
    bvh_node* bvh;

public:
    hittable_list() { hitbox = aabb::empty; }
    ~hittable_list() {
        // for (auto& object : *objs) {
        //     delete object;
        // }
        // delete bvh;
        delete objs;
    }
    hittable* at(size_t index) const { return objs->at(index); }
    size_t size() const { return objs->size(); }
    void add(hittable* obj) {
        objs->push_back(obj);
        // hitbox = aabb(hitbox, obj->hitbox);
    }
    void build_bvh() { bvh = new bvh_node(*objs, 0, size()); }
    bool hit(const ray& ray, interval tint, hit_record& record) override {
        hit_record tmp_record{};
        bool any = false;
        // if (bvh->hit(ray, tint, tmp_record)) {
        //     any = true;
        //     tint.hi = tmp_record.t;
        //     record = tmp_record;
        // }
        for (size_t i = 0; i < objs->size(); ++i) {
            if (objs->at(i)->hit(ray, tint, tmp_record)) {
                any = true;
                tint.hi = tmp_record.t;
                record = tmp_record;
            }
        }
        return any;
    }
};
