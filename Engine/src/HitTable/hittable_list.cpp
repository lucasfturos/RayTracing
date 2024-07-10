#include "hittable_list.hpp"

bool hittable_list::hit(const ray &r, interval ray_t, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closet_so_far = ray_t.max;

    for (const auto &object : objects) {
        if (object->hit(r, interval(ray_t.min, closet_so_far), temp_rec)) {
            hit_anything = true;
            closet_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

double hittable_list::pdf_value(const point3 &origin,
                                const vec3 &direction) const {
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects)
        sum += weight * object->pdf_value(origin, direction);

    return sum;
}

vec3 hittable_list::random(const point3 &origin) const {
    auto int_size = int(objects.size());
    return objects[random_int(0, int_size - 1)]->random(origin);
}