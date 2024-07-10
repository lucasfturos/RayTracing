#include "aabb.hpp"

const aabb aabb::empty =
    aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe =
    aabb(interval::universe, interval::universe, interval::universe);

aabb::aabb(const point3 &a, const point3 &b) {
    x = (a.x <= b.x) ? interval(a.x, b.x) : interval(b.x, a.x);
    y = (a.y <= b.y) ? interval(a.y, b.y) : interval(b.y, a.y);
    z = (a.z <= b.z) ? interval(a.z, b.z) : interval(b.z, a.z);
}

aabb::aabb(const aabb &box0, const aabb &box1) {
    x = interval(box0.x, box1.x);
    y = interval(box0.y, box1.y);
    z = interval(box0.z, box1.z);
}

const interval &aabb::axis_interval(int n) const {
    if (n == 1) {
        return y;
    }
    if (n == 2) {
        return z;
    }
    return x;
}

bool aabb::hit(const ray &r, interval ray_t) const {
    const point3 &ray_orig = r.origin();
    const vec3 &ray_dir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
        const interval &ax = axis_interval(axis);
        const double adinv = 1.0 / ray_dir[axis];

        auto t0 = (ax.min - ray_orig[axis]) * adinv;
        auto t1 = (ax.max - ray_orig[axis]) * adinv;

        if (t0 < t1) {
            (t0 > ray_t.min) ? ray_t.min = t0 : 0;
            (t1 < ray_t.max) ? ray_t.max = t1 : 0;
        } else {
            (t1 > ray_t.min) ? ray_t.min = t1 : 0;
            (t0 < ray_t.max) ? ray_t.max = t0 : 0;
        }

        if (ray_t.max <= ray_t.min) {
            return false;
        }
    }
    return true;
}

int aabb::longest_axis() const {
    if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
    } else {
        return y.size() > z.size() ? 1 : 2;
    }
}