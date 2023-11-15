#include "torus.hpp"

bool torus::hit(const ray &r, double t_min, double t_max,
                hit_record &rec) const {
    return true;
}

bool torus::bounding_box(double time0, double time1, aabb &output_box) const {
    return true;
}
