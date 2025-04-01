#pragma once
#include <iostream>
#include <new>
#include <vector>

#include "ray.hpp"
#include "vec3.hpp"

class camera {
    struct viewport {
        double width, height;
        vec3 first_pixel, du, dv, center;
        viewport() {}
        viewport(const int& image_width, const int& image_height,
                 const double& height, const point3& camera)
            : center(camera), height(height) {
            width = height * double(image_width) / image_height;
            vec3 u{width, 0, 0};
            vec3 v{0, -height, 0};

            // top left point of viewport
            point3 q{center + vec3{0, 0, 1} - 0.5 * (u + v)};

            du = u / image_width;
            dv = v / image_height;
            first_pixel = q + 0.5 * (du + dv);
        }
    };
    int image_width, image_height;
    double ratio_;
    point3 center_;
    viewport viewport_;

    friend class world;

public:
    std::vector<ray*> produce_rays() {
        std::vector<ray*> rays{};
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                point3 pixel{viewport_.first_pixel + i * viewport_.du +
                             j * viewport_.dv};
                // TODO: write test on this
                try {
                    ray* r = new ray{center_, pixel - center_};
                    rays.push_back(r);
                } catch (std::bad_alloc& e) {
                    for (size_t i = 0; i < rays.size(); ++i) {
                        delete rays.at(i);
                    }
                    std::cout << e.what() << std::endl;
                    throw;
                }
            }
        }
        return rays;
    }
    camera() {
        center_ = point3{0, 0, 0};
        image_width = 400;
        ratio_ = 16.0 / 9.0;
        image_height = static_cast<int>(image_width / ratio_);
        image_height = (image_height < 1) ? 1 : image_height;

        viewport_ = viewport{image_width, image_height, 2.0, center_};
    }
};
