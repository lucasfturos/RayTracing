#include "aabb.hpp"

const AABB AABB::empty =
    AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe =
    AABB(Interval::universe, Interval::universe, Interval::universe);

AABB::AABB(const Interval &x, const Interval &y, const Interval &z)
    : x(x), y(y), z(z) {
    padToMinimums();
}

AABB::AABB(const point3 &a, const point3 &b) {
    x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
    y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
    z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);

    padToMinimums();
}

AABB::AABB(const AABB &box0, const AABB &box1) {
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}

const Interval &AABB::axisInterval(int n) const {
    if (n == 1) {
        return y;
    }
    if (n == 2) {
        return z;
    }
    return x;
}

bool AABB::hit(const Ray &r, Interval ray_t) const {
    const point3 &ray_orig = r.origin();
    const vec3 &ray_dir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
        const Interval &ax = axisInterval(axis);
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

int AABB::longestAxis() const {
    if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
    } else {
        return y.size() > z.size() ? 1 : 2;
    }
}

void AABB::padToMinimums() {
    double delta = eps;
    if (x.size() < delta) {
        x = x.expand(delta);
    }
    if (y.size() < delta) {
        y = y.expand(delta);
    }
    if (z.size() < delta) {
        z = z.expand(delta);
    }
}