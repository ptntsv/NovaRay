#include "camera.hpp"
#include "hittable_list.hpp"
#include "utils/material.hpp"
#include "utils/sphere.hpp"

void bulky_demo() {
    hittable_list objlist;
    camera cam{};
    for (double y = 0; y < 2; y += 0.3) {
        for (double x = -2; x < 2; x += 0.3) {
            objlist.add(new sphere{point3{x, y, -1}, 0.1,
                                   new metal{color{0.2, 0.9, 0.4}}});
        }
    }
    // objlist.build_bvh();
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
    cam.render(world);
}
