#include "camera.hpp"
#include "hittable_list.hpp"
#include "utils/material.hpp"
#include "utils/sphere.hpp"

void bulky_demo() {
    hittable_list objlist;
    camera cam{};
    point3 s{-0.5, 0, -1};
    for (size_t y = 0; y <= 1; y++) {
        for (size_t x = 0; x <= 2; x++) {
            objlist.add(new sphere(s + point3(x, y, 0), 0.2,
                                   new metal(color(0.6, 0.4, 0.5))));
        }
    }
    objlist.build_bvh();
    cam.render(objlist);
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
    world.build_bvh();
    cam.render(world);
}

void metals_demo() {
    hittable_list world;
    auto material_ground = new lambertian_reflectance(color(0.8, 0.8, 0.0));
    auto material_center = new lambertian_reflectance(color(0.1, 0.2, 0.5));
    auto material_left = new metal(color(0.8, 0.8, 0.8));
    auto material_right = new metal(color(0.8, 0.6, 0.2));

    world.add(new sphere(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(new sphere(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(new sphere(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(new sphere(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam({0, 0, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    world.build_bvh();
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
