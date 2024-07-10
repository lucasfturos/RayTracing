#pragma once

#include "../../include/constante.hpp"
#include "../HitTable/hittable.hpp"

class Triangle : public hittable {
  public:
    Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
             shared_ptr<material> mat);

    virtual bool hit(const ray &r, interval ray_t,
                     hit_record &rec) const override;

    virtual aabb bounding_box() const override;

  private:
    vec3 v0, v1, v2;
    shared_ptr<material> mat_ptr;
    vec3 e1, e2;
};
