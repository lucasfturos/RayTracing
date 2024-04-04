#pragma once

#include "../../include/aabb.hpp"
#include "../../include/material.hpp"
#include "../../include/ray.hpp"
#include "../HitTable/hittable.hpp"

class Triangle : public hittable {
  public:
    Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
             shared_ptr<material> mat);
    // Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &normal,
    //          shared_ptr<material> mat);

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;

  private:
    vec3 v0, v1, v2;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    vec3 e1, e2;
};
