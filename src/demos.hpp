#pragma once
#include <memory>

#include "bvh.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "primitives//sphere.hpp"
#include "primitives/box.hpp"
#include "primitives/quad.hpp"

// #define LOG

void bulky_demo() {
    auto material_ground =
        make_shared<lambertian_reflectance>(color(0.8, 0.8, 0.0));
    hittable_list world;
    camera cam{{0, 1, 0}, {0, 0, -2}};
    world.add(
        make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    double padding = 0.5;
    point3 s{-3, 0, -2};
    for (size_t z = 0; z <= 3; z++) {
        for (size_t x = 0; x <= 10; x++) {
            if ((x + z) % 2 == 0) {
                world.add(make_shared<sphere>(
                    s + point3(x * padding, 0, z * padding), 0.2,
                    make_shared<lambertian_reflectance>(color(0.1, 0.2, 0.5))));
            } else {
                world.add(make_shared<sphere>(
                    s + point3(x * padding, 0, z * padding), 0.2,
                    make_shared<metal>(color(0.6, 0.4, 0.5))));
            }
        }
    }
    world.add(make_shared<sphere>(point3(0, 1, -4), 1,
                                  make_shared<metal>(color(0.6, 0.4, 0.5))));
    hittable* bvh = new bvh_node(world);
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(*bvh);
    delete bvh;
}
void levitating() {
    hittable_list world;
    camera cam{{0, 0, 0}, {0, 0, -1}};
    double padding = 0.6;
    point3 s{-3, -1, -1};
    for (size_t y = 0; y <= 3; y++) {
        for (size_t x = 0; x <= 10; x++) {
            if ((x + y) % 2 == 0) {
                world.add(make_shared<sphere>(
                    s + point3(x * padding, y * padding, -1), 0.2,
                    std::make_shared<lambertian_reflectance>(
                        color(0.1, 0.2, 0.5))));
            } else {
                world.add(make_shared<sphere>(
                    s + point3(x * padding, y * padding, -1), 0.2,
                    std::make_shared<metal>(color(0.6, 0.4, 0.5))));
            }
        }
    }
    hittable* bvh = new bvh_node(world);
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(*bvh);
    delete bvh;
}
void arbitrary_camera_demo() {
    hittable_list world;

    auto material_ground =
        std::make_shared<lambertian_reflectance>(color(0.8, 0.8, 0.0));
    auto material_center =
        std::make_shared<lambertian_reflectance>(color(0.1, 0.2, 0.5));
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2));
    world.add(
        make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(
        make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam({0, 1, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    hittable* bvh = new bvh_node(world);
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(*bvh);
    delete bvh;
}

void metals_demo() {
    hittable_list world;
    auto material_ground =
        std::make_shared<lambertian_reflectance>(color(0.8, 0.8, 0.0));
    auto material_center =
        std::make_shared<lambertian_reflectance>(color(0.1, 0.2, 0.5));
    auto material_left = std::make_shared<metal>(color(0.8, 0.8, 0.8));
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(
        make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(
        make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-0.5, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(0.5, 0.0, -1.0), 0.5, material_right));

    camera cam({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    hittable* bvh = new bvh_node(world);
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(*bvh);
    delete bvh;
}
void motion_blur() {
    hittable_list world;
    auto material_ground =
        std::make_shared<lambertian_reflectance>(color(0.8, 0.8, 0.0));
    auto material_center =
        std::make_shared<lambertian_reflectance>(color(0.1, 0.2, 0.5));
    auto material_right = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.3);
    world.add(
        make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(
        make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<sphere>(
        point3(0.5, 0.0, -1.0), point3(0.5, 0.25, -1.0), 0.5, material_right));
    camera cam({-1.5, 0.5, 0}, {0, 0, -1}, {0, 1, 0}, 90);

    auto bvh = new bvh_node(world);
    cam.render(*bvh);
    delete bvh;
}
void planes_demo() {
    hittable_list world;
    auto left_red =
        std::make_shared<lambertian_reflectance>(color(1.0, 0.2, 0.2));
    auto back_green =
        std::make_shared<lambertian_reflectance>(color(0.2, 1.0, 0.2));
    auto right_blue =
        std::make_shared<lambertian_reflectance>(color(0.2, 0.2, 1.0));
    auto upper_orange =
        std::make_shared<lambertian_reflectance>(color(1.0, 0.5, 0.0));
    auto lower_teal =
        std::make_shared<lambertian_reflectance>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, -5), vec3(0, 0, -4),
                                vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-3, -2, -9), vec3(4, 0, 0),
                                vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0),
                                right_blue));
    // world.add(
    //     make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4),
    //     upper_orange));
    // world.add(
    // make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4),
    // lower_teal));

    camera cam({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    auto bvh = new bvh_node(world);
    cam.render(*bvh);
    delete bvh;
}

void perlin_demo() {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                  std::make_shared<lambertian_reflectance>(
                                      std::make_shared<noise_texture>(4.0))));

    world.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                  std::make_shared<lambertian_reflectance>(
                                      std::make_shared<noise_texture>(4.0))));

    camera cam({13, 2, 3}, {0, 0, 0}, {0, 1, 0}, 20);
    hittable* bvh = new bvh_node(world);
    cam.render(*bvh);
    delete bvh;
}

void quads() {
    hittable_list world;

    // Materials
    auto left_red = make_shared<lambertian_reflectance>(color(1.0, 0.2, 0.2));
    auto back_green = make_shared<lambertian_reflectance>(color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<lambertian_reflectance>(color(0.2, 0.2, 1.0));
    auto upper_orange =
        make_shared<lambertian_reflectance>(color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<lambertian_reflectance>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3, -2, 5), vec3(0, 0, -4),
                                vec3(0, 4, 0), left_red));
    world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0),
                                back_green));
    world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0),
                                right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4),
                                upper_orange));
    world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0),
                                vec3(0, 0, -4), lower_teal));

    camera cam(point3{0, 0, 9}, {0, 0, 0}, {0, 1, 0}, 60);
    auto bvh = new bvh_node(world);
    cam.render(*bvh);
    delete bvh;
}

void boxes_demo() {
    hittable_list world;
    auto mat = make_shared<metal>(color(0.6, 0.4, 0.5));
    auto red = make_shared<lambertian_reflectance>(color(1.0, 0.2, 0.2));
    auto ground =
        std::make_shared<lambertian_reflectance>(color(0.8, 0.8, 0.0));

    world.add(make_shared<quad>(point3(-50, 0, 50), vec3(0, 0, -200),
                                vec3(100, 0, 0), ground));
    world.add(quad::box(point3(-1, 0, -2), point3(2, 2, -4), red));
    world.add(quad::box(point3(-8, 0, -1), point3(-2, 2, -8), red));
    world.add(quad::box(point3(3, 0, 0), point3(5, 2, -100), mat));

    camera cam({0, 3, 5}, {0, 0, -1}, {0, 1, 0}, 60);
    auto bvh = new bvh_node(world);
    cam.render(*bvh);
    delete bvh;
}
