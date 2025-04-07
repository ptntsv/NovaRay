#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class material {
protected:
    color albedo;

public:
    material(const color& albedo) : albedo(albedo) {}

    virtual bool scatter(const ray& ray_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const {
        return false;
    }

    ~material() = default;
};

class lambertian_reflectance : public material {
public:
    lambertian_reflectance(const color& albedo) : material(albedo) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 scattered_dir = rec.normal + vec::random_unit();
        if (scattered_dir.near_zero())
            scattered_dir = rec.normal;
        scattered = ray{rec.p, scattered_dir};
        attenuation = albedo;
        return true;
    }
};

class metal : public material {
    double fuzz;
public:
    metal(const color& albedo, const double fuzz) : material(albedo),
        fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 reflected_dir = vec::reflected(ray_in.direction(), rec.normal);
        reflected_dir = vec::unit(reflected_dir) + (fuzz * vec::random_unit());
        scattered = ray{rec.p, reflected_dir};
        attenuation = albedo;
        return true;
    }
};
