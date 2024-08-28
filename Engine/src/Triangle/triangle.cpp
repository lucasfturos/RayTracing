#include "triangle.hpp"

Triangle::Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   const vec2 &tex, shared_ptr<Material> mat)
    : v0(v0), v1(v1), v2(v2), texture(tex), mat_ptr(mat) {
    e1 = v1 - v0;
    e2 = v2 - v0;
}

bool Triangle::hit(const Ray &r, Interval ray_t, HitRecord &rec) const {
    vec3 h = cross(r.direction(), e2);
    double a = dot(e1, h);
    if (std::abs(a) < eps) {
        return false;
    }

    double f = 1.0 / a;
    vec3 s = r.origin() - v0;
    double u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    vec3 q = cross(s, e1);
    double v = f * dot(r.direction(), q);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = f * dot(e2, q);
    if (!ray_t.surrounds(t)) {
        return false;
    }

    rec.u = u + texture.x;
    rec.v = v + texture.y;

    rec.t = t;
    rec.p = r.at(t);

    vec3 normal = unitVector(cross(e1, e2));
    rec.normal = normal;
    rec.setFaceNormal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

AABB Triangle::boundingBox() const {
    point3 point_min(std::min(v0.x, std::min(v1.x, v2.x)),
                     std::min(v0.y, std::min(v1.y, v2.y)),
                     std::min(v0.z, std::min(v1.z, v2.z)));

    point3 point_max(std::max(v0.x, std::max(v1.x, v2.x)),
                     std::max(v0.y, std::max(v1.y, v2.y)),
                     std::max(v0.z, std::max(v1.z, v2.z)));

    return AABB(point_min, point_max);
}
