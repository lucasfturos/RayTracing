#include "hittable_list.hpp"

bool HittableList::hit(const Ray &r, Interval ray_t, HitRecord &rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closet_so_far = ray_t.max;

    for (const auto &object : objects) {
        if (object->hit(r, Interval(ray_t.min, closet_so_far), temp_rec)) {
            hit_anything = true;
            closet_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

double HittableList::pdfValue(const point3 &origin,
                              const vec3 &direction) const {
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
        sum += weight * object->pdfValue(origin, direction);

    return sum;
}

vec3 HittableList::random(const point3 &origin) const {
    auto int_size = static_cast<int>(objects.size());
    return objects[randomInt(0, int_size - 1)]->random(origin);
}