#include <assert.h>

#include "src/camera.hpp"
#include "src/utils/material.hpp"
#include "src/utils/sphere.hpp"
#include "src/utils/utils.hpp"
#include "src/world.hpp"

void bulky_demo() {
    hittable_list objlist;
    camera cam{};
    for (double y = 0; y < 2; y += 0.3) {
        for (double x = -2; x < 2; x += 0.3) {
            objlist.add(new sphere{point3{x, y, -1}, 0.1,
                                   new metal{color{0.2, 0.9, 0.4}, 1.0}});
        }
    }
    objlist.build_bvh();
    cam.render(objlist);
}

int main() {
    hittable_list world;

    auto material_ground = new lambertian_reflectance(color(0.8, 0.8, 0.0));
    auto material_center = new lambertian_reflectance(color(0.1, 0.2, 0.5));
    auto material_left = new lambertian_reflectance(color(0.2, 0.9, 0.5));
    auto material_right = new metal(color(0.8, 0.6, 0.2), 0.5);

    world.add(new sphere(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(new sphere(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(new sphere(point3(1.0, 0.0, -1.0), 0.5, material_right));
    world.add(new sphere(point3(-1.0, 0.0, -2), 0.5, material_left));

    camera cam({0, 1, 0}, {0, 0, -1}, {0, 1, 0}, 90);
    cam.render(world);
    return 0;
}
