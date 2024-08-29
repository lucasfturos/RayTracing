#pragma once

#include "utils/constante.hpp"

class AABB {
  public:
    Interval x, y, z;
    static const AABB empty, universe;

    AABB() {}
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const point3 &a, const point3 &b);
    AABB(const AABB &box0, const AABB &box1);

    const Interval &axisInterval(int n) const;

    bool hit(const Ray &r, Interval ray_t) const;

    int longestAxis() const;

  private:
    void padToMinimums();
};

inline AABB operator+(const AABB &bbox, const vec3 &offset) {
    return AABB(bbox.x + offset.x, bbox.y + offset.y, bbox.z + offset.z);
}

inline AABB operator+(const vec3 &offset, const AABB &bbox) {
    return bbox + offset;
}
