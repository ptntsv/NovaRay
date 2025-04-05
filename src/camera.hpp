#pragma once
#include <iostream>

#include "utils/color.hpp"
#include "utils/material.hpp"
#include "utils/utils.hpp"
#include "utils/vec3.hpp"
#include "world.hpp"

struct viewport {
    double width, height;
    vec3 first_pixel, du, dv, center;
    viewport() {}
    viewport(const int& image_width, const int& image_height,
             const double& height, const point3& camera)
        : center(camera), height(height) {
        width = height * double(image_width) / image_height;
        double z = 1.0;
        vec3 u{width, 0, 0};
        vec3 v{0, -height, 0};

        // top left point of viewport
        point3 q{center + vec3{0, 0, -z} - 0.5 * (u + v)};

        du = u / image_width;
        dv = v / image_height;
        first_pixel = q + 0.5 * (du + dv);
    }
};

class camera {
    // image
    int image_width_, image_height_;
    double ratio_;
    point3 center_;

    // viewport
    viewport vp_;

    // anlialiasing
    int samples_n;

    int bounces_limit;

    friend class world;

    // generates a point inside unit square
    point3 sample_square() const {
        return point3{utility::random_double(-0.5, 0.5),
                      utility::random_double(-0.5, 0.5), 0};
    }

    // generates a ray with some offset
    ray produce_sample_ray(int i, int j) {
        vec3 offset = sample_square();
        point3 pixel_sample = vp_.first_pixel + ((i + offset.x()) * vp_.du) +
                              ((j + offset.y()) * vp_.dv);
        return ray{center_, pixel_sample - center_};
    }

    // generates a ray
    ray produce_ray(int i, int j) {
        point3 pixel{vp_.first_pixel + i * vp_.du + j * vp_.dv};
        return ray{center_, pixel - center_};
    }

    // calculates average color of bunch rays
    color avg_color(int i, int j, hittable_list* objs) {
        color pcolor{0};
        for (size_t k = 0; k < samples_n; k++) {
            ray r{produce_sample_ray(i, j)};
            // ray r{produce_ray(i, j)};
            pcolor += ray_color(r, *objs);
        }
        return pcolor / samples_n;
    }

    color ray_color(const ray& r, hittable_list& objs, int depth = 0) {
        if (depth >= bounces_limit)
            return color{0};
        hit_record record{};
        bool t = objs.hit(r, 0.001, utility::inf, record);
        color final_color{0};
        if (t) {
            ray scattered;
            color attenuation;
            if (record.mat->scatter(r, record, attenuation, scattered)) {
                return attenuation *
                       ray_color(ray{record.p, scattered.direction()}, objs,
                                 depth + 1);
            }
            return color{0};
        }

        vec3 unit = vec::unit(r.direction());
        double y =
            .5 * (unit.y() + 1.0);  // generally it depends on viewport height
        return (1.0 - y) * color{1.0, 1.0, 1.0} + y * color{.2, .2, 1.0};
    }

public:
    void render(hittable_list* objs) {
        std::cout << "P3\n"
                  << image_width_ << ' ' << image_height_ << "\n255\n";
        for (size_t j = 0; j < image_height_; j++) {
            for (size_t i = 0; i < image_width_; i++) {
                color final_color = avg_color(i, j, objs);
                write_color(std::cout, final_color);
            }
        }
    }
    camera() {
        center_ = point3{0, 0, 0};
        image_width_ = 800;
        ratio_ = 16.0 / 9.0;
        image_height_ = static_cast<int>(image_width_ / ratio_);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        vp_ = viewport{image_width_, image_height_, 2.0, center_};

        samples_n = 10;
        bounces_limit = 50;
    }
};
