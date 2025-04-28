#pragma once

#include <memory>

#include "perlin.hpp"
#include "utils/color.hpp"
#include "utils/vec3.hpp"

class texture {
public:
    virtual ~texture() = default;
    virtual color value(const point3& p) const = 0;
};

class solid_color : public texture {
public:
    solid_color(const color& albedo) : albedo(albedo) {}
    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)) {}

    color value(const point3& p) const override { return albedo; }

private:
    color albedo;
};

class noise_texture : public texture {
public:
    noise_texture(double scale = 1.0) : scale(scale) {}

    color value(const point3& p) const override {
        // simple noise
        // return color(1, 1, 1) * noise.noise(scale * p);

        // turbulence
        // return color(1, 1, 1) * noise.turb(scale * p, 7);

        // marble
        return color(0.5, 0.5, 0.5) *
               (1 + sin(scale * p.z() + 10 * noise.turb(p)));
    }

private:
    perlin noise;
    double scale;
};
