#include <assert.h>

#include "src/camera.hpp"
#include "src/utils/material.hpp"
#include "src/utils/sphere.hpp"
#include "src/world.hpp"

int main() {
    hittable_list objlist;
    camera cam{};
    material* mat_ground = new lambertian_reflectance{color{0.8, 0.8, 0}};
    material* mat_center = new lambertian_reflectance{color{0.1, 0.2, 0.5}};
    material* mat_left = new metal{color{0.2, 0.9, 0.4}};
    material* mat_right = new metal{color{0.2, 0.3, 0.2}};

    objlist.add(new sphere{point3{0, 0, -1.1}, 0.5, mat_center});
    objlist.add(new sphere{point3{0, -100.5, -1}, 100, mat_ground});
    objlist.add(new sphere{point3{-1, 0, -1}, 0.5, mat_left});
    objlist.add(new sphere{point3{1, 0, -1}, 0.5, mat_right});
    cam.render(&objlist);
}
