#pragma once

#include <assert.h>

#include <cmath>
#include <ostream>

#include "utils.hpp"

class vec3 {
    double e[3];

public:
    vec3(double e = 0) : e{e, e, e} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    vec3(const vec3& other)
        : e{
              other.e[0],
              other.e[1],
              other.e[2],
          } {};

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) { return *this *= 1 / t; }

    double length() const { return std::sqrt(length_squared()); }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    bool near_zero() const {
        double d = 1e-8;
        return (std::abs(e[0]) < d) && (std::abs(e[1]) < d) &&
               (std::abs(e[2]) < d);
    }
};

using point3 = vec3;

std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

vec3 operator+(const vec3& u, const double& t) {
    return vec3(u[0] + t, u[1] + t, u[2] + t);
}

vec3 operator+(const vec3& v, const vec3& w) {
    return vec3(v[0] + w[0], v[1] + w[1], v[2] + w[2]);
}

vec3 operator-(const vec3& v, const vec3& w) {
    return vec3(v[0] - w[0], v[1] - w[1], v[2] - w[2]);
}

vec3 operator*(const vec3& v, const vec3& w) {
    return vec3(v[0] * w[0], v[1] * w[1], v[2] * w[2]);
}
vec3 operator*(double t, const vec3& v) {
    return vec3(t * v[0], t * v[1], t * v[2]);
}

vec3 operator*(const vec3& v, double t) { return t * v; }

vec3 operator/(const vec3& v, double t) { return (1 / t) * v; }

namespace vec {

double dot(const vec3& u, const vec3& v) {
    return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
}

vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
                u[0] * v[1] - u[1] * v[0]);
}

vec3 unit(const vec3& v) { return v / v.length(); }

vec3 random(double min, double max) {
    return vec3{utility::random_double(min, max),
                utility::random_double(min, max),
                utility::random_double(min, max)};
}
vec3 random_unit() {
    while (true) {
        vec3 p = vec::random(-1, 1);
        double len_sq = p.length_squared();
        if (1e-160 < len_sq && len_sq <= 1)
            return p / sqrt(len_sq);
    }
}
// maybe it'll be better to move it to hittable
// with some fancy name like 'unit on surface'
vec3 random_unit_on_hemisphere(const vec3& normal) {
    vec3 on_surface = random_unit();
    if (vec::dot(normal, on_surface) > 0)
        return on_surface;
    return -on_surface;
}
vec3 reflected(const vec3& v, const vec3& n) { return v - 2 * dot(v, n) * n; }
}  // namespace vec
vec3 random_in_unit_sphere() {
    double theta = utility::random_double(0, 2 * M_PI);
    double phi = utility::random_double(0, M_PI);
    double r = utility::random_double(0, 1);
    return vec3(r * sin(phi) * cos(theta), r * sin(phi) * sin(theta),
                r * cos(phi));
}
