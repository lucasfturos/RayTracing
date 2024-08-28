#pragma once

#include "hittable.hpp"

class HittableList : public HitTable {
  public:
    std::vector<shared_ptr<HitTable>> objects;

    HittableList() {}
    HittableList(shared_ptr<HitTable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<HitTable> object) {
        objects.push_back(object);
        bbox = AABB(bbox, object->boundingBox());
    }

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;

    AABB boundingBox() const override { return bbox; }

    virtual double pdfValue(const point3 &origin,
                            const vec3 &direction) const override;

    virtual vec3 random(const point3 &origin) const override;

  private:
    AABB bbox;
};

struct Scene {
    HittableList world;
    HittableList lights;
};
