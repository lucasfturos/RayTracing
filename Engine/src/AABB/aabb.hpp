#pragma once

#include "../../include/constante.hpp"

class aabb {
  public:
    interval x, y, z;

    aabb() {}
    aabb(const interval &x, const interval &y, const interval &z)
        : x(x), y(y), z(z) {}
    aabb(const point3 &a, const point3 &b);
    aabb(const aabb &box0, const aabb &box1);

    const interval &axis_interval(int n) const;

    bool hit(const ray &r, interval ray_t) const;

    int longest_axis() const;

    static const aabb empty, universe;
};
