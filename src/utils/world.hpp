#pragma once

#include "utilities.hpp"

class hittable_list : public hittable {
private:
    std::vector<hittable*>* objs = new std::vector<hittable*>();

public:
    ~hittable_list() {
        for (auto& object : *objs) {
            delete object;
        }
        delete objs;
    }
    hittable_list() {}
    void add(hittable* obj) { objs->push_back(obj); }
    bool hit(const ray& ray, const double& tmin, const double& tmax,
             hit_record& record) {
        hit_record tmp_record{};
        bool any = false;
        double nearest = tmax;
        for (size_t i = 0; i < objs->size(); ++i) {
            if (objs->at(i)->hit(ray, tmin, nearest, tmp_record)) {
                any = true;
                nearest = tmp_record.t;
                record = tmp_record;
            }
        }
        return any;
    }
    friend class world;
};

color ray_color(const ray& ray, hittable_list& objs) {
    hit_record record{};
    bool t = objs.hit(ray, 0, inf, record);
    if (t) {
        return 0.5 * color(record.normal + 1);
    }

    vec3 unit = unit_vector(ray.direction());
    double y =
        .5 * (unit.y() + 1.0);  // generally it depends on viewport height
    return (1.0 - y) * color{1.0, 1.0, 1.0} + y * color{.2, .2, 1.0};
}
