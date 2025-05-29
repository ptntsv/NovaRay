#pragma once

#include <memory>

#include "hittable.hpp"
#include "texture.hpp"
#include "utils/color.hpp"
#include "utils/ray.hpp"
#include "utils/vec3.hpp"

class material {
protected:
    std::shared_ptr<texture> albedo_texture;

public:
    virtual ~material() = default;

    material(std::shared_ptr<texture> texture) : albedo_texture(texture) {}
    material(const color& albedo)
        : albedo_texture(std::make_shared<solid_color>(albedo)) {}

    virtual color emitted(const point3& p) const { return color(0, 0, 0); }

    virtual bool scatter(const ray& ray_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian_reflectance : public material {
public:
    lambertian_reflectance(std::shared_ptr<texture> texture)
        : material(texture) {}
    lambertian_reflectance(const color& albedo) : material(albedo) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 scattered_dir = rec.normal + vec::random_unit();
        if (scattered_dir.near_zero()) scattered_dir = rec.normal;
        scattered = ray{rec.p, scattered_dir};
        attenuation = albedo_texture->value(rec.p);
        return true;
    }
};

class metal : public material {
    double fuzz_;

public:
    metal(std::shared_ptr<texture> texture, double fuzz = 0.0)
        : material(texture), fuzz_(fuzz < 1 ? fuzz : 1) {}

    metal(const color& albedo, double fuzz = 0.0)
        : material(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 reflected_dir = vec::reflected(ray_in.direction(), rec.normal);
        vec3 fuzzy_direction = reflected_dir + fuzz_ * random_in_unit_sphere();
        scattered = ray(rec.p, fuzzy_direction, ray_in.time());
        attenuation = albedo_texture->value(rec.p);
        return (vec::dot(fuzzy_direction, rec.normal)) > 0;
    }
};

class diffuse_light : public material {
public:
    diffuse_light(std::shared_ptr<texture> texture) : material(texture) {}
    diffuse_light(const color& emit) : material(emit) {}

    color emitted(const point3& p) const override {
        return albedo_texture->value(p);
    }
};
