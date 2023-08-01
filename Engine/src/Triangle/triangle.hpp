#pragma once

#include "../../include/aabb.hpp"
#include "../../include/constante.hpp"
#include "../../include/material.hpp"
#include "../../include/ray.hpp"
#include "../HitTable/hittable.hpp"

class Triangle : public hittable {
  public:
    Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
             shared_ptr<material> mat)
        : v{v0, v1, v2}, mat_ptr(mat) {
        vec3 e1 = v[1] - v[0];
        vec3 e2 = v[2] - v[0];
        surface_normal = cross(e2, e1);
    }

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;

  private:
    vec3 v[3];
    vec3 surface_normal;
    shared_ptr<material> mat_ptr;
};

class NormalTriangle : public hittable {
  public:
    NormalTriangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   const vec3 &n0, const vec3 &n1, const vec3 &n2,
                   shared_ptr<material> mat)
        : vec{v0, v1, v2}, normals{n0, n1, n2}, mat_ptr(mat) {}

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;

  private:
    vec3 vec[3];
    vec3 normals[3];
    shared_ptr<material> mat_ptr;
};
