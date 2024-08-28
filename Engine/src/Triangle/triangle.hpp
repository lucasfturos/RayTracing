#pragma once

#include "../HitTable/hittable.hpp"

class Triangle : public HitTable {
  public:
    Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec2 &tex,
             shared_ptr<Material> mat);

    virtual bool hit(const Ray &r, Interval ray_t,
                     HitRecord &rec) const override;

    virtual AABB boundingBox() const override;

  private:
    vec3 v0, v1, v2;
    vec2 texture;
    shared_ptr<Material> mat_ptr;
    vec3 e1, e2;
};
