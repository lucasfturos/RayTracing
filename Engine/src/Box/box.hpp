#pragma once

#include "../HitTable/hittable_list.hpp"

class box : public hittable {
  public:
    box() {}
    box(const point3 &p0, const point3 &p1, shared_ptr<material> ptr);

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    aabb bounding_box() const override { return aabb(box_min, box_max); }

  public:
    point3 box_min;
    point3 box_max;
    hittable_list sides;
};
