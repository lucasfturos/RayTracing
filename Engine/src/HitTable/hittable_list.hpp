#pragma once

#include "hittable.hpp"
#include <vector>

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    aabb bounding_box() const override { return bbox; }

  private:
    aabb bbox;
};
