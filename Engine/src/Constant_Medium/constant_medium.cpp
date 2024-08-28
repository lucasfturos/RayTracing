#include "constant_medium.hpp"

bool ConstantMedium::hit(const Ray &r, Interval ray_t, HitRecord &rec) const {
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && randomDouble() < eps;

    HitRecord rec1, rec2;

    if (!boundary->hit(r, Interval(-infinity, infinity), rec1)) {
        return false;
    }

    if (!boundary->hit(r, Interval(rec1.t + eps, infinity), rec2)) {
        return false;
    }

    if (debugging) {
        std::cerr << "\nt_min=" << rec1.t << ", t_max=" << rec2.t << '\n';
    }

    if (rec1.t < ray_t.min) {
        rec1.t = ray_t.min;
    }
    if (rec2.t > ray_t.max) {
        rec2.t = ray_t.max;
    }

    if (rec1.t >= rec2.t) {
        return false;
    }

    if (rec1.t < 0) {
        rec1.t = 0;
    }

    const double ray_length = r.direction().length();
    const double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const double hit_distance = neg_inv_density * std::log(randomDouble());

    if (hit_distance > distance_inside_boundary) {
        return false;
    }

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);

    if (debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
                  << "rec.t = " << rec.t << '\n'
                  << "rec.p = " << rec.p << '\n';
    }

    rec.normal = vec3(1, 0, 0); // arbitrary
    rec.front_face = true;      // also arbitrary
    rec.mat_ptr = phase_function;

    return true;
}
