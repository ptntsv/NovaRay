#include <assert.h>

#include <iostream>

#include "src/utils/color.hpp"
#include "src/utils/hittable.hpp"
#include "src/utils/ray.hpp"
#include "src/utils/sphere.hpp"
#include "src/utils/vec3.hpp"

color ray_color(const ray& ray) {
    // spheres
    point3 s_center{0, 0, -1};
    sphere s1{s_center, 2};

    hit_record record{};
    bool t = s1.hit(ray, -10, 10, record);
    if (t) {
        return 0.5 * color(record.normal + 1);
    }

    vec3 unit = unit_vector(ray.direction());
    double y =
        .5 * (unit.y() + 1.0);  // generally it depends on viewport height
    return (1.0 - y) * color{1.0, 1.0, 1.0} + y * color{.2, .2, 1.0};
}

int main() {
    // resolution
    int im_width = 400;
    double aspectRatio = 16.0 / 9.0;
    int im_height = static_cast<int>(im_width / aspectRatio);
    im_height = (im_height < 1) ? 1 : im_height;

    // camera
    point3 cam{0, 0, 0};

    // viewport
    double vp_z = 1.0;
    double vp_height = 2.0;
    double vp_width = vp_height * double(im_width) / im_height;

    vec3 vp_u{vp_width, 0, 0};
    vec3 vp_v{0, -vp_height, 0};

    vec3 du = vp_u / im_width;
    vec3 dv = vp_v / im_height;

    // top left point of viewport (denoted as q)
    point3 vp_q{cam + vec3{0, 0, -vp_z} - 0.5 * (vp_u + vp_v)};
    point3 p00{vp_q + 0.5 * (du + dv)};

    // rendering
    std::cout << "P3\n" << im_width << ' ' << im_height << "\n255\n";

    for (int j = 0; j < im_height; j++) {
        std::clog << "\rScanlines remaining: " << (im_height - j) << ' '
                  << std::flush;
        for (int i = 0; i < im_width; i++) {
            point3 pixel{p00 + i * du + j * dv};
            ray r{cam, pixel - cam};
#ifndef LOGS
            write_color(std::cout, ray_color(r));
#endif
        }
    }

    std::clog << "\rDone.                 \n";
}
