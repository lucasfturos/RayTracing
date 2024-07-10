#pragma once

#include "hittable.hpp"

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

    virtual double pdf_value(const point3 &origin,
                             const vec3 &direction) const override;

    virtual vec3 random(const point3 &origin) const override;

  private:
    aabb bbox;
};

struct Scene {
    hittable_list world;
    hittable_list lights;
};
