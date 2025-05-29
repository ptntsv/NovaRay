#pragma once
#include <iostream>
#include <memory>
#include <queue>

#include "hittable.hpp"
// DO NOT TOUCH IT
#include "material.hpp"
// ---------------
#include <mutex>
#include <thread>

#include "utils/color.hpp"
#include "utils/counter.h"
#include "utils/interval.hpp"
#include "utils/ray.hpp"
#include "utils/utils.hpp"
#include "utils/vec3.hpp"

using std::cout;
using std::endl;

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::seconds;

struct viewport {
    double width, height;
    vec3 first_pixel, du, dv, center;
    viewport() {}
    viewport(int image_width, int image_height, const vec3& viewport_u,
             const vec3& viewport_v, const point3& lookfrom,
             const point3& lookat) {
        vec3 w = vec::unit(lookfrom - lookat);

        double focus_dist = (lookfrom - lookat).length();
        center = lookfrom;
        point3 q =
            center - viewport_u * 0.5 - viewport_v * 0.5 - w * focus_dist;
        du = viewport_u / image_width;
        dv = viewport_v / image_height;
        first_pixel = q + 0.5 * (du + dv);
    }
};

struct tile {
    bool done;
    int id;
    point3 p1, p2;
    tile(const point3& p1, const point3& p2, int id) : id(id), p1(p1), p2(p2) {}
};

class camera {
    // image
    int image_width_, image_height_;
    double ratio_;

    int horizontal_tiles_n = 4, vertical_tiles_n = 4;
    std::queue<tile> tiles_to_render;
    std::mutex qmutex;

    // viewport
    viewport vp_;

    // anlialiasing
    int samples_n;

    int bounces_limit;

    using color_array_t = std::shared_ptr<color[]>;
    std::shared_ptr<color_array_t[]> image;
    std::mutex image_mutex;

    friend class world;

    // camera parameters
    point3 lookfrom;  // camera position
    point3 lookat;    // point to look at
    vec3 vup;         // camera's "up" direction
    double vva;       // vertical view angle

    // generates a point inside unit square
    static point3 sample_square() {
        return point3{utility::random_double(-0.5, 0.5),
                      utility::random_double(-0.5, 0.5), 0};
    }

    void cut_screen() {
        int tile_width = image_width_ / vertical_tiles_n;
        int tile_height = image_height_ / horizontal_tiles_n;
        for (size_t i = 0; i < horizontal_tiles_n; i++) {
            for (size_t j = 0; j < vertical_tiles_n; j++) {
                point3 left_up(j * tile_width, i * tile_height, 0);
                point3 right_bot(left_up.x() + tile_width,
                                 left_up.y() + tile_height, 0);
                tiles_to_render.emplace(
                    left_up, right_bot,
                    static_cast<int>(tiles_to_render.size() + 1));
            }
        }
    }

    // generates a ray with some offset
    ray produce_sample_ray(int i, int j) const {
        vec3 offset = sample_square();
        point3 pixel_sample = vp_.first_pixel + ((i + offset.x()) * vp_.du) +
                              ((j + offset.y()) * vp_.dv);
        double ray_time = utility::random_double(0, 1);

        return ray{lookfrom, pixel_sample - lookfrom, ray_time};
    }

    // generates a ray
    ray produce_ray(int i, int j) const {
        point3 pixel{vp_.first_pixel + i * vp_.du + j * vp_.dv};
        return ray(lookfrom, pixel - lookfrom);
    }

    // calculates average color of bunch rays
    color avg_color(int i, int j, const hittable& objs) {
        color pcolor{0};
        for (size_t k = 0; k < samples_n; k++) {
            ray r(produce_sample_ray(i, j));
            pcolor += ray_color(r, objs);
        }
        return pcolor / samples_n;
    }

    color ray_color(const ray& r, const hittable& objs, int depth = 0,
                    bool use_black_bg = false) const {
        if (depth >= bounces_limit) return color{0};
        hit_record record{};
        auto tint = interval(0.001, utility::inf);
        bool t = objs.hit(r, tint, record);
        if (t) {
            ray scattered;
            color attenuation;
            color emitted = record.mat->emitted(record.p);
            if (!record.mat->scatter(r, record, attenuation, scattered))
                return emitted;
            return emitted +
                   attenuation * ray_color(scattered, objs, depth + 1);
        }
        if (use_black_bg) {
            return color{0, 0, 0};
        }

        vec3 unit = vec::unit(r.direction());
        double y =
            .5 * (unit.y() + 1.0);  // generally it depends on viewport height
        return (1.0 - y) * color{1.0, 1.0, 1.0} + y * color{.2, .2, 1.0};
    }

    void initialize_camera() {
        image_width_ = 1920;
        // image_width_ = 800;
        ratio_ = 16.0 / 9.0;
        image_height_ = static_cast<int>(image_width_ / ratio_);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;
        cut_screen();

        image =
            std::shared_ptr<color_array_t[]>(new color_array_t[image_height_]);
        for (size_t i = 0; i < image_height_; i++) {
            image[i] = color_array_t(new color[image_width_]);
        }

        double theta = utility::degrees_to_radians(vva);
        double h = tan(theta / 2);
        double viewport_height = 2.0 * h * (lookfrom - lookat).length();
        double viewport_width =
            viewport_height *
            (static_cast<double>(image_width_) / image_height_);

        vec3 w = vec::unit(lookfrom - lookat);
        vec3 u = vec::unit(vec::cross(vup, w));
        vec3 v = vec::cross(w, u);

        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        vp_ = viewport(image_width_, image_height_, viewport_u, viewport_v,
                       lookfrom, lookat);

        samples_n = 10;
        bounces_limit = 5;
    }

    void render_tile(const hittable& objs, std::ostream& out,
                     const tile& tile) {
        auto xfrom = static_cast<int>(tile.p1.x());
        auto xtill = static_cast<int>(tile.p2.x());

        auto yfrom = static_cast<int>(tile.p1.y());
        auto ytill = static_cast<int>(tile.p2.y());

        for (size_t j = yfrom; j < ytill; j++) {
            for (size_t i = xfrom; i < xtill; i++) {
                image[j][i] = avg_color(i, j, objs);
            }
        }
    }

public:
    void render(const hittable& objs, std::ostream& out) {
        auto t1 = high_resolution_clock::now();
        out << "P3\n" << image_width_ << ' ' << image_height_ << "\n255\n";
        size_t threads_n = std::min(horizontal_tiles_n * vertical_tiles_n, 8);
        // size_t threads_n = 16;
        std::vector<std::thread> threads;
        threads.reserve(threads_n);
        for (int i = 0; i < threads_n; ++i) {
            threads.emplace_back([&]() {
                while (true) {
                    std::unique_lock lock(qmutex);
                    if (tiles_to_render.empty()) {
                        break;
                    }
                    tile current_tile = tiles_to_render.front();
                    cout << "Thread " << std::this_thread::get_id() << " took "
                         << current_tile.id << endl;
                    tiles_to_render.pop();
                    lock.unlock();

                    render_tile(objs, out, current_tile);
                    cout << "Thread " << std::this_thread::get_id()
                         << " ended up with " << current_tile.id << endl;
                }
            });
        }
        for (auto& t : threads) {
            t.join();
        }
        auto t2 = high_resolution_clock::now();
        auto delta = duration_cast<seconds>(t2 - t1);
        std::cout << "Rendering took " << delta.count() << " seconds"
                  << std::endl;
        for (int i = 0; i < image_height_; ++i) {
            for (int j = 0; j < image_width_; ++j) {
                write_color(out, image[i][j]);
            }
        }
    }

    camera(const point3& lookfrom = {0, 0, 0},
           const point3& lookat = {0, 0, -1}, const vec3& vup = {0, 1, 0},
           double vva = 90)
        : lookfrom(lookfrom), lookat(lookat), vup(vup), vva(vva) {
        initialize_camera();
    }
};
