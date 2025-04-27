#pragma once

#include "hittable.hpp"
#include "utils/color.hpp"
#include "utils/ray.hpp"
#include "utils/vec3.hpp"

class material {
protected:
    color albedo;

public:
    virtual ~material() = default;
    material(const color& albedo) : albedo(albedo) {}
    virtual bool scatter(const ray& ray_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian_reflectance : public material {
public:
    lambertian_reflectance(const color& albedo) : material(albedo){};
    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 scattered_dir = rec.normal + vec::random_unit();
        if (scattered_dir.near_zero()) scattered_dir = rec.normal;
        scattered = ray{rec.p, scattered_dir};
        attenuation = albedo;
        return true;
    }
};

class metal : public material {
    double fuzz_;

public:
    metal(const color& albedo, double fuzz = 0.0)
        : material(albedo), fuzz_(fuzz < 1 ? fuzz : 1){};
    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 reflected_dir = vec::reflected(ray_in.direction(), rec.normal);

        vec3 fuzzy_direction = reflected_dir + fuzz_ * random_in_unit_sphere();

        scattered = ray(rec.p, fuzzy_direction, ray_in.time());

        attenuation = albedo;

        return (vec::dot(fuzzy_direction, rec.normal)) > 0;
    }
};
