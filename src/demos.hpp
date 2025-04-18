#include "camera.hpp"
#include "hittable_list.hpp"
#include "utils/bvh.hpp"
#include "utils/material.hpp"
#include "utils/quad.h"
#include "utils/sphere.hpp"

// #define LOG

void bulky_demo() {
    auto material_ground = new lambertian_reflectance(color(0.8, 0.8, 0.0));
    hittable_list world;
    camera cam{{0, 1, 0}, {0, 0, -2}};
    world.add(new sphere(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    double padding = 0.5;
    point3 s{-3, 0, -2};
    for (size_t z = 0; z <= 3; z++) {
        for (size_t x = 0; x <= 10; x++) {
            if ((x + z) % 2 == 0) {
                world.add(new sphere(
                    s + point3(x * padding, 0, z * padding), 0.2,
                    new lambertian_reflectance(color(0.1, 0.2, 0.5))));
            } else {
                world.add(new sphere(s + point3(x * padding, 0, z * padding),
                                     0.2, new metal(color(0.6, 0.4, 0.5))));
            }
        }
    }
    world.add(new sphere(point3(0, 1, -4), 1, new metal(color(0.6, 0.4, 0.5))));
    hittable* bvh = new bvh_node(world.items());
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
                world.add(new sphere(
                    s + point3(x * padding, y * padding, -1), 0.2,
                    new lambertian_reflectance(color(0.1, 0.2, 0.5))));
            } else {
                world.add(new sphere(s + point3(x * padding, y * padding, -1),
                                     0.2, new metal(color(0.6, 0.4, 0.5))));
            }
        }
    }
    hittable* bvh = new bvh_node(world.items());
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(*bvh);
}
void arbitrary_camera_demo() {
    hittable_list world;

    auto material_ground = new lambertian_reflectance(color(0.8, 0.8, 0.0));
    auto material_center = new lambertian_reflectance(color(0.1, 0.2, 0.5));
    auto material_right = new metal(color(0.8, 0.6, 0.2));

    world.add(new sphere(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(new sphere(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(new sphere(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam({0, 1, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    hittable* bvh = new bvh_node(world.items());
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(*bvh);
}

void metals_demo() {
    hittable_list world;
    auto material_ground = new lambertian_reflectance(color(0.8, 0.8, 0.0));
    auto material_center = new lambertian_reflectance(color(0.1, 0.2, 0.5));
    auto material_left = new metal(color(0.8, 0.8, 0.8));
    auto material_right = new metal(color(0.8, 0.6, 0.2));

    world.add(new sphere(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(new sphere(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(new sphere(point3(-0.5, 0.0, -1.0), 0.5, material_left));
    world.add(new sphere(point3(0.5, 0.0, -1.0), 0.5, material_right));

    camera cam({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 90);
#ifdef LOG
    world.fmt_print(0);
#endif
    cam.render(world);
}
void motion_blur() {
    hittable_list world;
    auto material_ground = new lambertian_reflectance(color(0.8, 0.8, 0.0));
    auto material_center = new lambertian_reflectance(color(0.1, 0.2, 0.5));
    auto material_right = new metal(color(0.8, 0.6, 0.2), 0.3);
    world.add(new sphere(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(new sphere(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(new sphere(point3(0.5, 0.0, -1.0), point3(0.5, 0.25, -1.0), 0.5,
                         material_right));
    camera cam({-1.5, 0.5, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    cam.render(world);
}
void planes_demo() {
    hittable_list world;

    // Materials
    auto red = new lambertian_reflectance(color(1.0, 0.2, 0.2));

    // Quads
    world.add(new quad(point3(-3, -2, -2), vec3(0, 5, 0), vec3(0, 0, -5), red));
    // world.add(make_shared<quad>(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4,
    // 0),
    //                             back_green));
    // world.add(make_shared<quad>(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4,
    // 0),
    //                             right_blue));
    // world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0,
    // 4),
    //                             upper_orange));
    // world.add(make_shared<quad>(point3(-2, -3, 5), vec3(4, 0, 0),
    //                             vec3(0, 0, -4), lower_teal));

    camera cam({0, 0, 2}, {0, 0, 0}, {0, 1, 0}, 90);

    cam.render(world);
}
