#pragma once

#include "../../include/constante.hpp"

class aabb {
  public:
    interval x, y, z;
    static const aabb empty, universe;

    aabb() {}
    aabb(const interval &x, const interval &y, const interval &z);
    aabb(const point3 &a, const point3 &b);
    aabb(const aabb &box0, const aabb &box1);

    const interval &axis_interval(int n) const;

    bool hit(const ray &r, interval ray_t) const;

    int longest_axis() const;

  private:
    void pad_to_minimums();
};
