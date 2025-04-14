#pragma once

#include <ostream>

#include "interval.hpp"
#include "ray.hpp"

class aabb {
    interval ts[3];

    void add_paddings() {
        double delta = 0.0001;
        if (ts[0].len() < delta)
            ts[0].expand(delta);
        if (ts[1].len() < delta)
            ts[1].expand(delta);
        if (ts[2].len() < delta)
            ts[2].expand(delta);
    }

public:
    interval at(size_t axis) const {
        if (axis > 2)
            return interval::empty;
        return ts[axis];
    }
    interval xs() const { return ts[0]; }
    interval ys() const { return ts[1]; }
    interval zs() const { return ts[2]; }

    aabb() = default;
    aabb(const interval& tx, const interval& ty, const interval& tz)
        : ts{tx, ty, tz} {
        add_paddings();
    }
    aabb(const point3& p1, const point3& p2) {
        ts[0] = interval{p1.x(), p2.x()};
        ts[1] = interval{p1.y(), p2.y()};
        ts[2] = interval{p1.z(), p2.z()};
        add_paddings();
    }
    aabb(const aabb& box1, const aabb& box2) {
        ts[0] = interval{box1.ts[0], box2.ts[0]};
        ts[1] = interval{box1.ts[1], box2.ts[1]};
        ts[2] = interval{box1.ts[2], box2.ts[2]};
        add_paddings();
    }
    bool hit(const ray& ray, interval out) const {
        point3 q = ray.origin();
        vec3 d = ray.direction();
        for (size_t axis = 0; axis < 3; axis++) {
            double dinv = 1 / d[axis];

            double t0 = (ts[axis].lo - q[axis]) * dinv;
            double t1 = (ts[axis].hi - q[axis]) * dinv;

            if (t0 < t1) {
                out.lo = std::max(t0, out.lo);
                out.hi = std::min(t1, out.hi);
            } else {
                out.lo = std::max(t1, out.lo);
                out.hi = std::min(t0, out.hi);
            }

            if (out.hi <= out.lo)
                return false;
        }
        return true;
    }

    size_t longest_axis() const {
        if (xs().len() > ys().len())
            return (xs().len() > zs().len()) ? 0 : 2;
        return (ys().len() > zs().len()) ? 1 : 2;
    }

    bool operator==(const aabb& other) const {
        return xs() == other.xs() && ys() == other.ys() && zs() == other.zs();
    }

    friend std::ostream& operator<<(std::ostream& os, const aabb& box) {
        os << "x: " << box.ts[0] << " ";
        os << "y: " << box.ts[1] << " ";
        os << "z: " << box.ts[2];
        return os;
    }
    static const aabb empty;
};

const aabb aabb::empty =
    aabb(interval::empty, interval::empty, interval::empty);
