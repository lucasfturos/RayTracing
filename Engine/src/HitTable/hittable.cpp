#include "hittable.hpp"

bool translate::hit(const ray &r, interval ray_t, hit_record &rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!object->hit(moved_r, ray_t, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

rotate_y::rotate_y(shared_ptr<hittable> p, double angle)
    : ptr(p), sin_theta(std::sin(radians(angle))),
      cos_theta(std::cos(radians(angle))) {
    bbox = ptr->bounding_box();

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                auto newx = cos_theta * x + sin_theta * z;
                auto newz = -sin_theta * x + cos_theta * z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; ++c) {
                    min[c] = std::min(min[c], tester[c]);
                    max[c] = std::max(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray &r, interval ray_t, hit_record &rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin.x = cos_theta * r.origin().x - sin_theta * r.origin().z;
    origin.z = sin_theta * r.origin().x + cos_theta * r.origin().z;

    direction.x = cos_theta * r.direction().x - sin_theta * r.direction().z;
    direction.z = sin_theta * r.direction().x + cos_theta * r.direction().z;

    ray rotated_r(origin, direction, r.time());
    if (!ptr->hit(rotated_r, ray_t, rec)) {
        return false;
    }

    auto p = rec.p;
    auto normal = rec.normal;

    p.x = cos_theta * rec.p.x + sin_theta * rec.p.z;
    p.z = -sin_theta * rec.p.x + cos_theta * rec.p.z;

    normal.x = cos_theta * rec.normal.x + sin_theta * rec.normal.z;
    normal.z = -sin_theta * rec.normal.x + cos_theta * rec.normal.z;

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}
