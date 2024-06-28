#include "triangle.hpp"
#include <cmath>

Triangle::Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   shared_ptr<material> mat)
    : v0(v0), v1(v1), v2(v2), mat_ptr(mat) {
    e1 = v1 - v0;
    e2 = v2 - v0;
}

bool Triangle::hit(const ray &r, double t_min, double t_max,
                   hit_record &rec) const {

    vec3 ray_cross_e2 = cross(r.direction(), e2);

    double det = dot(e1, ray_cross_e2);

    if (fabs(det) > -eps && fabs(det) < eps) {
        return false;
    }

    double inv_det = 1.0 / det;
    vec3 s = r.origin() - v0;
    double u = inv_det * dot(s, ray_cross_e2);

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    vec3 s_cross_e1 = cross(s, e1);
    double v = inv_det * dot(r.direction(), s_cross_e1);

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = inv_det * dot(e2, s_cross_e1);

    if (t < t_min || t > t_max) {
        return false;
    }

    rec.t = t;
    rec.p = r.at(t);
    rec.normal = normalize<double>(cross(e1, e2));
    rec.mat_ptr = mat_ptr;

    return true;
}

bool Triangle::bounding_box(double /* time0 */, double /* time1 */,
                            aabb &output_box) const {
    double min_x = std::min(v0.x, std::min(v1.x, v2.x));
    double min_y = std::min(v0.y, std::min(v1.y, v2.y));
    double min_z = std::min(v0.z, std::min(v1.z, v2.z));

    double max_x = std::max(v0.x, std::max(v1.x, v2.x));
    double max_y = std::max(v0.y, std::max(v1.y, v2.y));
    double max_z = std::max(v0.z, std::max(v1.z, v2.z));

    output_box = aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
    return true;
}
