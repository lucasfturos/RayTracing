#include "triangle.hpp"

bool Triangle::hit(const ray &r, double t_min, double t_max,
                   hit_record &rec) const {
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec3 h = cross(r.direction(), e2);
    double det = dot(e1, h);

    if (det > -0.00001 && det < 0.00001) {
        return false;
    }

    double inv_det = 1.0 / det;
    vec3 s = r.origin() - v0;
    double u = dot(s, h) * inv_det;

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    vec3 q = cross(s, e1);
    double v = dot(r.direction(), q) * inv_det;

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = dot(e2, q) * inv_det;

    if (t > 0.00001 && t < t_max && t > t_min) {
        rec.t = t;
        rec.p = r.at(t);
        rec.normal = surface_normal;
        rec.mat_ptr = mat_ptr;

        return true;
    }
    rec.mat_ptr = mat_ptr;

    return false;
}

bool Triangle::bounding_box(double time0, double time1,
                            aabb &output_box) const {
    double min_x = std::min(v0.x(), std::min(v1.x(), v2.x()));
    double min_y = std::min(v0.y(), std::min(v1.y(), v2.y()));
    double min_z = std::min(v0.z(), std::min(v1.z(), v2.z()));

    double max_x = std::max(v0.x(), std::max(v1.x(), v2.x()));
    double max_y = std::max(v0.y(), std::max(v1.y(), v2.y()));
    double max_z = std::max(v0.z(), std::max(v1.z(), v2.z()));

    output_box = aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
    return true;
}

bool NormalTriangle::hit(const ray &r, double t_min, double t_max,
                         hit_record &rec) const {
    vec3 e1 = vec[1] - vec[0];
    vec3 e2 = vec[2] - vec[0];
    vec3 dir = r.direction();

    vec3 pvec = cross(dir, e2);
    double det = dot(e1, pvec);

    if (det > -0.00001 && det < 0.00001) {
        return false;
    }

    double inv_det = 1.0 / det;
    vec3 tvec = r.origin() - vec[0];
    double u = dot(tvec, pvec) * inv_det;

    if (u < 0.0 || u > 1.0) {
        return false;
    }

    vec3 qvec = cross(tvec, e1);
    double v = dot(dir, qvec) * inv_det;

    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    double t = dot(e2, qvec) * inv_det;

    if (t > 0.00001 && t < t_max && t > t_min) {
        vec3 normal =
            u * normals[1] + v * normals[2] + (1.0 - u - v) * normals[0];
        rec.t = t;
        rec.p = r.at(t);
        rec.normal = normal;
        rec.mat_ptr = mat_ptr;
        return true;
    }

    return false;
}

bool NormalTriangle::bounding_box(double time0, double time1,
                                  aabb &output_box) const {
    vec3 min(std::min({vec[0].x(), vec[1].x(), vec[2].x()}),
             std::min({vec[0].y(), vec[1].y(), vec[2].y()}),
             std::min({vec[0].z(), vec[1].z(), vec[2].z()}));

    vec3 max(std::max({vec[0].x(), vec[1].x(), vec[2].x()}),
             std::max({vec[0].y(), vec[1].y(), vec[2].y()}),
             std::max({vec[0].z(), vec[1].z(), vec[2].z()}));

    output_box = aabb(min, max);
    return true;
}
