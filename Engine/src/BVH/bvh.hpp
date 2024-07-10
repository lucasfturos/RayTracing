#pragma once

#include "../HitTable/hittable_list.hpp"

class bvh_node : public hittable {
  public:
    bvh_node() = default;

    bvh_node(hittable_list list)
        : bvh_node(list.objects, 0, list.objects.size()) {}

    bvh_node(const std::vector<shared_ptr<hittable>> objects, size_t start,
             size_t end);

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;
    aabb bounding_box() const override { return bbox; }

    static bool box_compare(const shared_ptr<hittable> a,
                            const shared_ptr<hittable> b, int axis);
    static bool box_x_compare(const shared_ptr<hittable> &a,
                              const shared_ptr<hittable> &b);
    static bool box_y_compare(const shared_ptr<hittable> &a,
                              const shared_ptr<hittable> &b);
    static bool box_z_compare(const shared_ptr<hittable> &a,
                              const shared_ptr<hittable> &b);

  public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb bbox;
};
