#pragma once

#include <iostream>

#include "utilities.hpp"
#include "vec3.hpp"

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int rbyte = int(255.999 * clamp(r, 0.0, 0.999));
    int gbyte = int(255.999 * clamp(g, 0.0, 0.999));
    int bbyte = int(255.999 * clamp(b, 0.0, 0.999));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
