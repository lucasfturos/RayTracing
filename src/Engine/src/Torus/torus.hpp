#pragma once

#include "../../include/constante.hpp"
#include "../HitTable/hittable.hpp"

class torus : public hittable {
  public:
    torus() {}
    torus(point3 cen, double r_outter, double r_inner, shared_ptr<material> m)
        : center(cen), outter_radius(r_outter), inner_radius(r_inner),
          mat_ptr(m){};

    virtual bool hit(const ray &r, double t_min, double t_max,
                     hit_record &rec) const override;

    virtual bool bounding_box(double time0, double time1,
                              aabb &output_box) const override;

  public:
    point3 center;
    double outter_radius;
    double inner_radius;
    shared_ptr<material> mat_ptr;
};
