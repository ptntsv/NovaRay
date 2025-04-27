#pragma once

#include "color.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include "texture.hpp"
#include "material.hpp"

class material {
protected:
    texture* albedo_texture; //.................new
public:
    virtual ~material() { delete albedo_texture; } //.................new

    material(texture* texture) : albedo_texture(texture) {}  //.................new
    material(const color& albedo) : albedo_texture(new solid_color(albedo)) {} //.................new

    material(const material& other) : albedo_texture(other.albedo_texture) {} //.................new
    material& operator=(const material& other) { //.................new
        if (this != &other) {
            delete albedo_texture;
            albedo_texture = other.albedo_texture;
        }
        return *this;
    }

    virtual bool scatter(const ray& ray_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian_reflectance : public material {
public:
    lambertian_reflectance(texture* texture) : material(texture) {}; //.................new
    lambertian_reflectance(const color& albedo) : material(albedo) {}; //.................new

    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 scattered_dir = rec.normal + vec::random_unit();
        if (scattered_dir.near_zero())
            scattered_dir = rec.normal;
        scattered = ray{rec.p, scattered_dir};
        attenuation = albedo_texture->value(rec.p);  //.................new
        return true;
    }
};

class metal : public material {
    double fuzz_;

public:
    metal(texture* texture, double fuzz = 0.0) //.................new
        : material(texture), fuzz_(fuzz < 1 ? fuzz : 1) {};

    metal(const color& albedo, double fuzz = 0.0)
        : material(albedo), fuzz_(fuzz < 1 ? fuzz : 1) {};
    bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation,
                 ray& scattered) const override {
        vec3 reflected_dir = vec::reflected(ray_in.direction(), rec.normal);

        vec3 fuzzy_direction =
            reflected_dir +
            fuzz_ * random_in_unit_sphere();

        scattered = ray(rec.p, fuzzy_direction,
                        ray_in.time());

        attenuation = albedo_texture->value(rec.p); // ..................new

        return (vec::dot(fuzzy_direction, rec.normal)) > 0;
    }
};
