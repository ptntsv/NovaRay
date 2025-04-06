#pragma once

#include "utils.hpp"
#include "vec3.hpp"

using color = vec3;

inline double linear_to_gamma(const double c) { return (c > 0) ? std::sqrt(c) : 0; }

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    const int rbyte = static_cast<int>(254.999 * utility::clamp(r, 0.0, 0.999));
    const int gbyte = static_cast<int>(255.999 * utility::clamp(g, 0.0, 0.999));
    const int bbyte = static_cast<int>(255.999 * utility::clamp(b, 0.0, 0.999));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
