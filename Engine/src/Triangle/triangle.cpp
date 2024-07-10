#include "triangle.hpp"

Triangle::Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   shared_ptr<material> mat)
    : v0(v0), v1(v1), v2(v2), mat_ptr(mat) {
    e1 = v1 - v0;
    e2 = v2 - v0;
}

bool Triangle::hit(const ray &r, interval ray_t, hit_record &rec) const {
    vec3 dir_cross_e2 = cross(r.direction(), e2);
    double det = dot(e1, dir_cross_e2);
    if (std::abs(det) > -eps && std::abs(det) < eps) {
        return false;
    }

    double f = 1.0 / det;
    vec3 v0_to_origin = r.origin() - v0;
    double u = f * dot(v0_to_origin, dir_cross_e2);
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    vec3 origin_cross_e1 = cross(v0_to_origin, e1);
    double v = f * dot(r.direction(), origin_cross_e1);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = f * dot(e2, origin_cross_e1);
    if (t < ray_t.min || t > ray_t.max) {
        return false;
    }

    rec.u = u;
    rec.v = v;
    rec.t = t;
    rec.p = r.at(t);

    vec3 normal = unit_vector(cross(e2, e1));
    rec.normal = normal;
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

aabb Triangle::bounding_box() const {
    double min_x = std::min(v0.x, std::min(v1.x, v2.x));
    double min_y = std::min(v0.y, std::min(v1.y, v2.y));
    double min_z = std::min(v0.z, std::min(v1.z, v2.z));

    double max_x = std::max(v0.x, std::max(v1.x, v2.x));
    double max_y = std::max(v0.y, std::max(v1.y, v2.y));
    double max_z = std::max(v0.z, std::max(v1.z, v2.z));

    return aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
}
