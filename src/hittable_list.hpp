#pragma once

#include <vector>

#include "hittable.hpp"

class hittable_list : public hittable {
    // i hope it's good place to use unique pointers or smth
    std::vector<hittable*> items_{};

public:
    hittable_list() = default;
    ~hittable_list() {
        // for (auto& item : items_) {
        //     delete item;
        // }
    }
    std::vector<hittable*> items() { return items_; }
    hittable* at(size_t index) const { return items_.at(index); }
    size_t size() const { return items_.size(); }
    void add(hittable* obj) { items_.push_back(obj); }
    bool hit(const ray& ray, interval tint, hit_record& record) const override {
        hit_record tmp_record{};
        bool any = false;
        for (auto& item : items_) {
            if (item->hit(ray, tint, tmp_record)) {
                any = true;
                tint.hi = tmp_record.t;
                record = tmp_record;
            }
        }
        return any;
    }
};
