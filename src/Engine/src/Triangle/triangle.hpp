#pragma once

#include "../../include/aabb.hpp"
#include "../../include/constante.hpp"
#include "../../include/material.hpp"
#include "../../include/ray.hpp"
#include "../HitTable/hittable.hpp"
#include "../HitTable/hittable_list.hpp"
#include "../Rect/rect.hpp"

class Triangle : public hittable {
  public:
    Triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
             shared_ptr<material> mat)
        : v0(v0), v1(v1), v2(v2), mat_ptr(mat) {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        surface_normal = normalize(unit_vector(cross(e2, e1)));
    }

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;

  private:
    vec3 v0, v1, v2;
    vec3 surface_normal;
    shared_ptr<material> mat_ptr;
};

class SmoothTriangle : public hittable {
  public:
    SmoothTriangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
                   shared_ptr<material> mat)
        : v0(v0), v1(v1), v2(v2), mat_ptr(mat) {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        surface_normal = normalize(unit_vector(cross(e2, e1)));
    }

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;
    vec3 calculate_normal(double gamma, double beta) const;

  private:
    vec3 v0, v1, v2;
    vec3 surface_normal;
    shared_ptr<material> mat_ptr;
};
