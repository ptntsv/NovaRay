#include <assert.h>

#include <iostream>

#include "src/utils/color.hpp"
#include "src/utils/ray.hpp"
#include "src/utils/sphere.hpp"
#include "src/utils/vec3.hpp"

Color ray_color(const Ray& ray) {
    Vec3 unit = unit_vector(ray.direction());
    double y =
        .5 * (unit.y() + 1.0);  // generally it depends on viewport height
    return (1.0 - y) * Color{1.0, 1.0, 1.0} + y * Color{.2, .2, 1.0};
}

int main() {
    // resolution
    int im_width = 400;
    double aspectRatio = 16.0 / 9.0;
    int im_height = static_cast<int>(im_width / aspectRatio);
    im_height = (im_height < 1) ? 1 : im_height;

    // camera
    Point3 cam{0, 0, 0};

    // viewport
    double vp_z = 1.0;
    double vp_height = 2.0;
    double vp_width = vp_height * double(im_width) / im_height;

    Vec3 vp_u{vp_width, 0, 0};
    Vec3 vp_v{0, -vp_height, 0};

    Vec3 du = vp_u / im_width;
    Vec3 dv = vp_v / im_height;

    // top left point of viewport (denoted as q)
    Point3 vp_q{cam - Vec3{0, 0, vp_z} - 0.5 * (vp_u + vp_v)};
    Point3 p00{vp_q + 0.5 * (vp_u + vp_v)};

    // spheres
    Sphere s1{Point3{0, 0, vp_z}, .5};

    // rendering
    std::cout << "P3\n" << im_width << ' ' << im_height << "\n255\n";

    for (int j = 0; j < im_height; j++) {
        std::clog << "\rScanlines remaining: " << (im_height - j) << ' '
                  << std::flush;
        for (int i = 0; i < im_width; i++) {
            Point3 pixel{vp_q + i * du + j * dv};
            Ray r{cam, pixel - cam};
            Color pixel_color =
                s1.intersects(r) ? Color{1.0, 1.0, 0} : ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
